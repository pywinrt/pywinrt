from collections.abc import Iterator
import hashlib
from typing import NamedTuple
from contextlib import contextmanager
import io
import json
import os
import re
from pathlib import Path

# the version scheme, which scripts/tests/test_versions.py covers
import versions

# the distributions this repository publishes and the layout they sit in
import packages

# the text of every file this writes
import pyproject_templates as templates

PROJECTION_PATH = (Path(__file__).parent.parent / "projection").resolve()
TABLE_PATH = (Path(__file__).parent.parent / "table").resolve()
RUNTIME_PATH = (Path(__file__).parent.parent / "runtime").resolve()
INTEROP_PATH = (Path(__file__).parent.parent / "interop").resolve()
REDIST_PATH = (Path(__file__).parent.parent / "redist").resolve()

# PEP 639 has a distribution name its license as an SPDX expression and carry
# the text of what it names, and a license file is only found beside the
# pyproject.toml that names it, so every package gets a copy of the one at the
# root of this repository.
LICENSE_TEXT = (Path(__file__).parent.parent / "LICENSE").read_text(encoding="utf-8")

# Where a build keeps what it took out of a NuGet package, so that a second
# build needs neither the network nor an unpacked copy again. It is build
# output: nothing carries it, least of all a source distribution.
PAYLOAD_DIR = "nuget"

# What PEP 770 calls the bill of materials in a wheel.
SBOM_NAME = "cyclonedx.json"

# Where each architecture's runtimes are in a NuGet package, keyed by the
# platform tag of the wheel that carries them and holding the name setuptools
# builds that architecture under, since the two spell it differently. Both
# NuGet packages that PyWinRT redistributes a binary from lay their runtimes
# out this way; only the leaf directory differs.
RUNTIME_DIRS = {
    "win32": ("win32", "runtimes/win-x86"),
    "win_amd64": ("win-amd64", "runtimes/win-x64"),
    "win_arm64": ("win-arm64", "runtimes/win-arm64"),
}


class DllPayload(NamedTuple):
    """
    A Microsoft binary that PyWinRT redistributes, and where it comes from.
    """

    # the NuGet package it is taken out of, and the environment variable that
    # scripts/build-sdist.py and scripts/build-bdist.py point at a copy of it
    nuget_package: str
    env: str
    # the file itself, and the directory under RUNTIME_DIRS that holds it
    dll_name: str
    native_dir: str
    # the upstream license text, keyed by where it is in the NuGet package and
    # named by what the package carries it as, and the SPDX identifier of the
    # license. Neither of these licenses has an SPDX id, so both are
    # LicenseRef-.
    license_files: dict[str, str]
    license_id: str

    def dll_paths(self, wheel_tag: bool) -> dict[str, str]:
        """
        Where each architecture's .dll is, keyed the way the build that reads
        this asks for a platform.
        """
        return {
            tag if wheel_tag else plat_name: (
                f"{runtime_dir}/{self.native_dir}/{self.dll_name}"
            )
            for tag, (plat_name, runtime_dir) in RUNTIME_DIRS.items()
        }

    def nuget_path(self) -> Path:
        """
        The unpacked NuGet package that scripts/fetch-tools.ps1 downloaded.
        """
        return versions.unpacked_nuget_path(self.nuget_package)

    def nupkg_name(self) -> str:
        version = NUGET_PACKAGE_VERSIONS[self.nuget_package]

        return f"{self.nuget_package}.{version}.nupkg"

    def nupkg_url(self) -> str:
        """
        Where nuget.org serves the package, which a build that has no unpacked
        copy downloads it from.
        """
        version = NUGET_PACKAGE_VERSIONS[self.nuget_package]
        package = self.nuget_package.lower()

        return (
            "https://api.nuget.org/v3-flatcontainer"
            f"/{package}/{version}/{package}.{version}.nupkg"
        )

    def nupkg_sha256(self) -> str:
        """
        What the downloaded package has to hash to.

        It is taken from the copy in _tools/, which nuget left there when it
        unpacked the package and which is byte for byte what nuget.org serves.
        """
        nupkg = self.nuget_path() / self.nupkg_name()

        return hashlib.sha256(nupkg.read_bytes()).hexdigest()


# The component .dlls that are redistributed in a distribution of their own,
# whose sources are under redist/. A .dll is built for one architecture and
# the projection that needs it is not, so the two cannot be published in one
# wheel: the projection depends on the package below and imports it, and
# importing it is what puts the .dll on the DLL search path.
REDIST_PACKAGES = {
    "winrt-Microsoft.Web.WebView2.Dll": (
        # the distribution that depends on it and imports it
        "winrt-Microsoft.Web.WebView2",
        "winrt.microsoft.web.webview2.dll",
        DllPayload(
            nuget_package="Microsoft.Web.WebView2",
            env="WEBVIEW2_PATH",
            dll_name="Microsoft.Web.WebView2.Core.dll",
            native_dir="native_uap",
            license_files={
                "LICENSE.txt": "LICENSE.Microsoft.Web.WebView2.txt",
                "NOTICE.txt": "NOTICE.Microsoft.Web.WebView2.txt",
            },
            license_id="LicenseRef-Microsoft-Web-WebView2",
        ),
    ),
}

# The Windows App Runtime bootstrapper, which is the one Microsoft binary that
# a compiled package redistributes. The extension module links against its
# import library, and Windows looks for a module's dependencies in the
# directory the module was loaded from, so it ships beside the .pyd rather
# than in a package of its own the way a component .dll of an
# architecture-independent projection does.
BOOTSTRAP_DLL = DllPayload(
    nuget_package="Microsoft.WindowsAppSDK.Foundation",
    env="WASDK_FOUNDATION_PATH",
    dll_name="Microsoft.WindowsAppRuntime.Bootstrap.dll",
    native_dir="native",
    license_files={"license.txt": "LICENSE.Microsoft.WindowsAppSDK.txt"},
    license_id="LicenseRef-Microsoft-WindowsAppSDK",
)

# The three namespaces that nearly everything in the projection hands a type
# back from. A package that references one of these is broken without it, so
# it is a hard dependency rather than something the [all] extra offers; the
# three together are less than a megabyte and they reference nothing outside
# the set, so a minimal install grows by them and by nothing else.
HUBS = frozenset(
    {
        "winrt-Windows.Foundation",
        "winrt-Windows.Foundation.Collections",
        "winrt-Windows.Storage.Streams",
    }
)

# What each package is published with. A version says which metadata the
# package was generated from rather than which generator read it, so these
# come from the upstream each family tracks and from the hand-maintained
# runtime/version.txt, and nothing generated carries the generator's own
# version any more.
# Every version in .config/_tools.json, so that a distribution named after a
# NuGet package can say which one its metadata came from. That is the Windows
# App SDK components: the family is versioned by the metapackage, which says
# nothing about which component a given namespace was generated from.
NUGET_PACKAGE_VERSIONS = versions.nuget_package_versions()

RUNTIME_REQUIREMENT = versions.runtime_requirement()
TABLE_COMPILER_REQUIREMENT = versions.table_compiler_requirement()
RUNTIME_VERSION = versions.runtime_version()
FAMILY_VERSIONS = versions.family_versions()
NUGET_VERSIONS = versions.nuget_versions()

# The interop packages are written by hand, so there is no deps.json to derive
# their requirements from. Every one of them needs winrt-runtime. The ones that
# hand a projected type back to Python need that namespace's package as well,
# because they name the type and the runtime resolves the name by importing the
# module that owns it (see py::get_python_type() in runtime/src/runtime.cpp).
# Interop that only deals in IInspectable needs nothing extra, since
# winrt.system.Object lives in the runtime.
INTEROP_DEPENDENCIES: dict[str, list[str]] = {
    "winrt-Windows.Graphics.Capture.Interop": [
        # returns GraphicsCaptureItem
        "winrt-Windows.Graphics.Capture",
    ],
    "winrt-Windows.Graphics.DirectX.Direct3D11.Interop": [
        # returns IDirect3DDevice and IDirect3DSurface
        "winrt-Windows.Graphics.DirectX.Direct3D11",
    ],
    "winrt-Windows.Media.Interop": [
        # returns SystemMediaTransportControls
        "winrt-Windows.Media",
    ],
    "winrt-Windows.System.Interop": [
        # returns DispatcherQueueController
        "winrt-Windows.System",
    ],
    "winrt-Windows.UI.Composition.Interop": [
        # takes a Compositor
        "winrt-Windows.UI.Composition",
        # returns a DesktopWindowTarget
        "winrt-Windows.UI.Composition.Desktop",
    ],
    # only converts to IInspectable
    "winrt-Windows.UI.Xaml.Hosting.Interop": [],
    "winrt-Microsoft.UI.Interop": [
        # converts the WindowId, DisplayId and IconId structs, which the
        # InteractiveExperiences component owns
        "winrt-Microsoft.WindowsAppSDK.InteractiveExperiences",
    ],
    # only calls the bootstrapper, which has no projected types
    "winrt-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap": [],
}

PYTHON_KEYWORDS = {
    "and",
    "as",
    "assert",
    "async",
    "await",
    "break",
    "class",
    "continue",
    "def",
    "del",
    "elif",
    "else",
    "except",
    "finally",
    "for",
    "from",
    "global",
    "if",
    "import",
    "in",
    "is",
    "lambda",
    "nonlocal",
    "not",
    "or",
    "pass",
    "raise",
    "return",
    "try",
    "while",
    "with",
    "yield",
}


@contextmanager
def open_if_changed(path: Path) -> Iterator[io.StringIO]:
    """
    Opens a text buffer for path. On exit, the file is written only if what was
    written to the buffer differs from what the file already holds.

    Everything this script generates is watched by something else: CMake
    reconfigures a build tree when the pyproject.toml it configured from is
    newer than it is, and the editor language servers that take pyproject.toml
    for their own configuration restart when it changes. Rewriting all 420 of
    them byte for byte on every run is not free.
    """
    buffer = io.StringIO()

    yield buffer

    data = buffer.getvalue().encode()

    if path.exists() and path.read_bytes() == data:
        return

    path.write_bytes(data)


def remove_if_present(path: Path) -> None:
    """
    Deletes a file this script used to write.

    A projection package carried a setup.py, a version.txt and two
    requirements files while it was a compiled extension. Nothing generates
    them any more, so a tree that still has them is a tree where they would go
    stale.
    """
    path.unlink(missing_ok=True)


def write_license(package_path: Path, payload: DllPayload | None = None) -> None:
    """
    Copies the license of everything in one package into it.

    A package that redistributes a Microsoft binary carries that license as
    well as PyWinRT's, since the two cover different files in the same wheel.
    """
    with open_if_changed(package_path / "LICENSE") as f:
        f.write(LICENSE_TEXT)

    if payload is None:
        return

    for relative, name in payload.license_files.items():
        with open_if_changed(package_path / name) as f:
            f.write((payload.nuget_path() / relative).read_text(encoding="utf-8"))


def normalize_package_name(package_name: str) -> str:
    """
    The name a distribution is known by on PyPI, which a package URL names it
    with.
    """
    return re.sub(r"[-_.]+", "-", package_name).lower()


def avoid_keyword(name: str) -> str:
    if name in PYTHON_KEYWORDS:
        return f"{name}_"

    return name


def is_app_sdk_interop_package(name: str) -> bool:
    return name in packages.APP_SDK_INTEROP_COMPONENTS


# Which family each package belongs to, so that a requirement on another one
# can be written with that package's version instead of this one's. Every
# distribution but WinUI 2's is published as winrt-*, so the name says nothing
# about which upstream a package came from.
package_families = packages.distribution_families()


def is_app_sdk_bootstrap_package(name: str) -> bool:
    return (
        name == "winrt-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap"
    )


def is_windows_app_package(name: str) -> bool:
    return package_families[name] == "wasdk"


def is_dispatcher_queue_package(name: str) -> bool:
    return name == "winrt-Windows.System.Interop"


def is_direct3d11_package(name: str) -> bool:
    return name == "winrt-Windows.Graphics.DirectX.Direct3D11.Interop"


def winrt_ns_to_py_package(ns: str) -> str:
    return ".".join(avoid_keyword(x.lower()) for x in ns.split("."))


def format_sources(sources: list[str]) -> str:
    """
    Renders the sources of an Extension.

    Every interop package has a single source file, which fits on the line it
    is written on. winrt-runtime has nine, which do not.
    """
    if len(sources) == 1:
        return f'["{sources[0]}"]'

    lines = "".join(f'                "{source}",\n' for source in sources)

    return f"[\n{lines}            ]"


def format_dependencies(requirements: list[str]) -> str:
    return "".join(f'    "{requirement}",\n' for requirement in requirements)


# Each family of packages is released on its own upstream's schedule, so each
# has a changelog of its own and a reader following one from PyPI finds what
# changed in the version they installed. The one at the root of the repository
# is winrt-runtime's, the code generator's and everything that affects every
# package; a family with nothing of its own, such as the test component, is
# pointed at it too.
FAMILY_CHANGELOGS = frozenset({"winrt", "wasdk", "winui2", "webview2"})


def format_project_urls(family: str | None = None) -> str:
    changelog = (
        f"projection/{family}/CHANGELOG.md"
        if family in FAMILY_CHANGELOGS
        else "CHANGELOG.md"
    )

    return templates.PROJECT_URLS.format(changelog=changelog)


def write_readme(
    package_path: Path,
    package_name: str,
    namespaces: list[str],
    module_names: list[str],
    nuget_package: str,
    nuget_version: str,
    component: str | None = None,
) -> None:
    """
    Writes the README that PyPI shows for one package.

    It names the NuGet package the family was generated from and the version
    of it, because that is what a reader who wants to know which APIs are in
    here has to look up, and the only other place it appears is folded into
    the package's own version. A package that carries more than one namespace
    lists them, since its own name is then the distribution they are published
    in rather than any one of them.
    """
    with open_if_changed(package_path / "README.md") as f:
        f.write(templates.README.format(package_name=package_name))

        f.write(
            templates.BINARY_README.format(
                important=(
                    templates.WASDK_README_IMPORTANT
                    if is_windows_app_package(package_name)
                    else ""
                ),
                namespaces=(
                    f"the `{namespaces[0]}` namespace"
                    if len(namespaces) == 1
                    else f"{len(namespaces)} namespaces"
                ),
                provides=(
                    f"This package provides the `{module_names[0]}` module."
                    if len(module_names) == 1
                    else "This package provides the following modules:"
                    + "\n"
                    + "".join("\n" + f"- `{module}`" for module in module_names)
                ),
                nuget_package=nuget_package,
                nuget_version=nuget_version,
                component=(
                    # Which component of a metapackage family the namespaces
                    # came from, and at what version, since the family's own
                    # version is the metapackage's and says neither. A family
                    # that is one NuGet package has said both already.
                    "\n\n"
                    + f"Its metadata comes from the `{component}` component,"
                    + "\n"
                    + f"version {NUGET_PACKAGE_VERSIONS[component]}."
                    if component is not None and component != nuget_package
                    else ""
                ),
            )
        )


def format_hard_requirement(package: str, version: str, same_family: bool) -> str:
    """
    Renders a requirement on another package that this one's types name.

    Within one family the two come from one winmd set and one generator run,
    so they are released together and ~= is what says so - and unlike the ==
    that #137 rejected, it still admits a .postN of either one on its own.
    Across families the other package follows its own upstream on its own
    schedule, so all that can honestly be said about it is a floor.
    """
    if same_family:
        return f"{package}~={version}.0"

    return f"{package}>={version}"


def write_compiled_project_files(
    package_path: Path,
    module_name: str,
    ext_module_name: str,
    sources: list[str],
    package_name: str | None = None,
) -> None:
    """
    Writes the packaging of winrt-runtime or one of the interop packages.

    These are the only packages left that compile anything, so they are the
    only ones that still need setuptools, a setup.py and the 15-build
    cibuildwheel matrix.
    """
    # winrt-runtime lives outside of projection/, and the bootstrap package's
    # directory is shortened, so the caller says which distribution this is
    package_name = package_name or package_path.name
    is_runtime = package_name == "winrt-runtime"
    runtime_relative = os.path.relpath(RUNTIME_PATH, package_path).replace(os.sep, "/")
    root_package = module_name.split(".")[0]

    # The bootstrap module is the only compiled package that redistributes a
    # Microsoft binary, so it is the only one whose license names two parties
    # and whose build stages something into the package before it runs.
    redistributes = (
        BOOTSTRAP_DLL if is_app_sdk_bootstrap_package(package_name) else None
    )

    with open_if_changed(package_path / "pyproject.toml") as f:
        f.write(
            templates.COMPILED_PYPROJECT_TOML.format(
                classifiers=templates.CLASSIFIERS,
                project_urls=format_project_urls(
                    None if is_runtime else package_families[package_name]
                ),
                extra_requires="" if is_runtime else ', "winrt-runtime"',
                package_name=package_name,
                description="Python projection of Windows Runtime (WinRT) APIs",
                packages_find=(
                    templates.RUNTIME_PACKAGE_FIND
                    if is_runtime
                    else templates.INTEROP_PACKAGE_FIND.format(
                        root_package=root_package
                    )
                ),
                extra_license_id=(
                    f" AND {redistributes.license_id}" if redistributes else ""
                ),
                extra_license_files=(
                    "".join(
                        f', "{name}"' for name in redistributes.license_files.values()
                    )
                    if redistributes
                    else ""
                ),
                extra_package_data=', "*.h"' if is_runtime else "",
                test_command=templates.TEST_COMMAND.format(
                    module=f"{root_package}.{ext_module_name}"
                ),
                local_runtime=(
                    ""
                    if is_runtime
                    else templates.LOCAL_RUNTIME.format(
                        runtime_relative=runtime_relative
                    )
                ),
                extra_cibuildwheel_windows=templates.NO_WHEEL_REPAIR,
            )
        )

    write_license(package_path, redistributes)

    needs_app_sdk = is_app_sdk_interop_package(package_name)
    own_headers = not is_runtime and any((package_path / "cppwinrt").glob("**/*.h"))

    if is_runtime:
        with open_if_changed(package_path / "MANIFEST.in") as f:
            f.write(templates.RUNTIME_MANIFEST_IN)
    elif own_headers:
        with open_if_changed(package_path / "MANIFEST.in") as f:
            f.write(templates.INTEROP_MANIFEST_IN)
    else:
        remove_if_present(package_path / "MANIFEST.in")

    with open_if_changed(package_path / "setup.py") as f:
        f.write(
            templates.SETUP_PY.format(
                setup_py_imports=format_setup_py_imports(
                    ["os", "pathlib"]
                    if is_runtime or needs_app_sdk or own_headers
                    else [],
                    ["json"] if redistributes else [],
                ),
                setuptools_command_imports=(
                    templates.BOOTSTRAP_SETUPTOOLS_COMMAND_IMPORTS
                    if redistributes
                    else templates.SETUPTOOLS_COMMAND_IMPORTS
                ),
                payload=(
                    templates.BOOTSTRAP_PAYLOAD.format(
                        dll_name=redistributes.dll_name,
                        dll_paths=format_dll_paths(redistributes, wheel_tag=False),
                        env=redistributes.env,
                        sbom_name=SBOM_NAME,
                        package_name=package_name,
                        normalized_name=normalize_package_name(package_name),
                        version=FAMILY_VERSIONS[package_families[package_name]],
                        nuget_package=redistributes.nuget_package,
                        nuget_version=NUGET_PACKAGE_VERSIONS[
                            redistributes.nuget_package
                        ],
                        license_id=redistributes.license_id,
                        root_package=root_package,
                    )
                    if redistributes
                    else ""
                ),
                cmdclass=(
                    templates.BOOTSTRAP_CMDCLASS
                    if redistributes
                    else templates.CMDCLASS
                ),
                headers=(
                    templates.RUNTIME_HEADERS
                    if is_runtime
                    else templates.HEADERS
                    + (templates.OWN_HEADERS if own_headers else "")
                ),
                extra_init=(
                    templates.APP_SDK_INIT.format(
                        envs=", ".join(
                            f'"{name}"'
                            for name in packages.APP_SDK_INTEROP_COMPONENTS[
                                package_name
                            ]
                        )
                    )
                    if needs_app_sdk
                    else ""
                ),
                root_package=root_package,
                ext_module=ext_module_name,
                sources=format_sources(sources),
                extra_build=(
                    templates.APP_SDK_EXTRA_BUILD
                    if is_app_sdk_bootstrap_package(package_name)
                    else ""
                ),
                extra_libraries=(
                    ', "Microsoft.WindowsAppRuntime.Bootstrap"'
                    if is_app_sdk_bootstrap_package(package_name)
                    else ""
                )
                + (
                    ', "CoreMessaging"'
                    if is_dispatcher_queue_package(package_name)
                    else ""
                )
                + (', "D3D11"' if is_direct3d11_package(package_name) else ""),
            )
        )


def write_projection_project_files(
    package_path: Path,
    namespaces: list[str],
    module_names: list[str],
    version: str,
    dependencies: list[str],
    optional_dependencies: list[str],
    nuget_package: str,
    nuget_version: str,
    component: str | None,
) -> None:
    """
    Writes the packaging of one projection package.

    Everything in one is data, so the wheel is py3-none-any and the metadata
    is static: the version and the requirements are written here rather than
    read back out of files at build time. A package carries the namespaces
    that are published together, which is one of them for the Windows SDK and
    a NuGet component's worth for the Windows App SDK.
    """
    package_name = package_path.name
    root_dir = module_names[0].split(".")[0]

    with open_if_changed(package_path / "pyproject.toml") as f:
        f.write(
            templates.PROJECTION_PYPROJECT_TOML.format(
                classifiers=templates.CLASSIFIERS,
                project_urls=format_project_urls(package_families[package_name]),
                package_name=package_name,
                version=version,
                description="Python projection of Windows Runtime (WinRT) APIs",
                table_compiler_requirement=TABLE_COMPILER_REQUIREMENT,
                dependencies=format_dependencies(dependencies),
                optional_dependencies=(
                    "\n[project.optional-dependencies]\nall = [\n"
                    + format_dependencies(optional_dependencies)
                    + "]\n"
                    if optional_dependencies
                    else ""
                ),
                root_dir=root_dir,
            )
        )

    # one per namespace: each is a package of its own to a type checker
    for module_name in module_names:
        with open_if_changed(package_path / Path(*module_name.split(".")) / "py.typed"):
            pass

    write_license(package_path)

    write_readme(
        package_path,
        package_name,
        namespaces,
        module_names,
        nuget_package,
        nuget_version,
        component,
    )

    # what a compiled projection package used to carry
    remove_if_present(package_path / "setup.py")
    remove_if_present(package_path / "version.txt")
    remove_if_present(package_path / "requirements.txt")
    remove_if_present(package_path / "all-requirements.txt")


def format_setup_py_imports(*groups: list[str]) -> str:
    """
    Renders the stdlib imports of a generated setup.py.

    Which of them are needed depends on what the package does, and a setup.py
    that needs none of them has no import block at all rather than a blank
    one.
    """
    modules = sorted({module for group in groups for module in group})

    if not modules:
        return ""

    return "\n" + "".join(f"import {module}\n" for module in modules)


def format_dll_paths(payload: DllPayload, wheel_tag: bool) -> str:
    return "".join(
        f'    "{plat_name}": "{path}",\n'
        for plat_name, path in payload.dll_paths(wheel_tag).items()
    )


def write_dll_project_files(
    package_name: str, importer: str, module: str, payload: DllPayload
) -> None:
    """
    Writes the packaging of one package that redistributes a component .dll.

    Nothing in it is compiled, but what is in it is built for one
    architecture, so it is three py3-none-win_* wheels rather than the one
    py3-none-any wheel that a projection package is.
    """
    package_path = REDIST_PATH / package_name
    module_path = "/".join(module.split("."))
    version = FAMILY_VERSIONS[package_families[importer]]
    nuget_version = NUGET_PACKAGE_VERSIONS[payload.nuget_package]
    upstream_licenses = "".join(
        f'    "{name}",\n' for name in payload.license_files.values()
    )

    (package_path / module_path).mkdir(parents=True, exist_ok=True)

    with open_if_changed(package_path / "pyproject.toml") as f:
        f.write(
            templates.DLL_PYPROJECT_TOML.format(
                classifiers=templates.CLASSIFIERS,
                project_urls=format_project_urls(package_families[importer]),
                package_name=package_name,
                version=version,
                description=f"{payload.dll_name}, redistributed for PyWinRT",
                license_id=payload.license_id,
                upstream_licenses=upstream_licenses,
            )
        )

    with open_if_changed(package_path / "hatch_build.py") as f:
        f.write(
            templates.DLL_HATCH_BUILD.format(
                package_name=package_name,
                normalized_name=normalize_package_name(package_name),
                version=version,
                nuget_package=payload.nuget_package,
                nuget_version=nuget_version,
                nuget_url=payload.nupkg_url(),
                nuget_sha256=payload.nupkg_sha256(),
                env=payload.env,
                payload_dir=PAYLOAD_DIR,
                sbom_name=SBOM_NAME,
                dll_name=payload.dll_name,
                module_path=module_path,
                dll_paths=format_dll_paths(payload, wheel_tag=True),
                license_id=payload.license_id,
            )
        )

    with open_if_changed(package_path / module_path / "__init__.py") as f:
        f.write(templates.DLL_INIT_PY.format(dll_name=payload.dll_name))

    with open_if_changed(package_path / module_path / "py.typed"):
        pass

    with open_if_changed(package_path / "README.md") as f:
        f.write(templates.README.format(package_name=package_name))
        f.write(
            templates.DLL_README.format(
                dll_name=payload.dll_name,
                nuget_package=payload.nuget_package,
                nuget_version=nuget_version,
                importer=f"`{importer}`",
            )
        )

    write_license(package_path, payload)


# create pyproject.toml files for the projection packages

runtime_table_format = versions.runtime_table_format()

for deps_path in sorted(PROJECTION_PATH.glob("**/deps.json")):
    package_path = deps_path.parent

    with open(deps_path) as deps_file:
        deps = json.load(deps_file)

    # A projection package's only requirement on the runtime is that the
    # runtime reads its table, so the two have to be generated from one tree.
    table_format = (deps["table_format"]["major"], deps["table_format"]["minor"])

    if table_format != runtime_table_format:
        raise RuntimeError(
            f"{deps_path} is table format {table_format[0]}.{table_format[1]}"
            f" and {versions.TABLE_HEADER_PATH.name} reads"
            f" {runtime_table_format[0]}.{runtime_table_format[1]}"
        )

    package_name = package_path.name
    family = package_families[package_name]
    root_package = package_name[: package_name.rindex("-")]

    # Which namespaces a package carries is what the generator decided when it
    # grouped them, so it is read rather than taken from the package's name:
    # the name of a Windows App SDK component's package is the component and
    # not any one of the namespaces in it.
    distribution = package_name.removeprefix(f"{root_package}-")
    namespaces = deps["namespaces"]
    module_names = [
        f"{root_package.replace('-', '_')}.{winrt_ns_to_py_package(ns)}"
        for ns in namespaces
    ]

    required = set(deps["required"])
    referenced = set(deps["referenced"])
    component = distribution if distribution in NUGET_PACKAGE_VERSIONS else None

    # A family published as NuGet components is released as a set - Microsoft's
    # own build refuses a project that mixes versions of them - so a component
    # depends outright on the others it hands types back from, rather than
    # offering them in the [all] extra. That also carries a namespace that
    # moves between components across releases: the whole set moves in one
    # resolver transaction, so the new owner is never installed beside the old.
    same_family = (
        {dep for dep in referenced if package_families[dep] == family}
        if component
        else set()
    )

    dependencies = [RUNTIME_REQUIREMENT]

    for dep in sorted(required | same_family | (HUBS & referenced)):
        dep_version = FAMILY_VERSIONS[package_families[dep]]

        if dep in HUBS:
            # A hub is a hard dependency because of how much of the projection
            # hands one of its types back, not because the two are coupled, so
            # a floor is all it takes; nothing that already installs is broken
            # by a later one.
            dependencies.append(f"{dep}>={dep_version}")
        else:
            dependencies.append(
                format_hard_requirement(
                    dep, dep_version, package_families[dep] == family
                )
            )

    # A component .dll is redistributed in a package of its own, since it is
    # built for one architecture and this one is not. Nothing here activates
    # without it, so the dependency is hard, and a floor is all of it: a
    # newer component .dll serves an older projection.
    dependencies.extend(
        f"{dll_package}>={FAMILY_VERSIONS[family]}"
        for dll_package, (importer, _, _) in REDIST_PACKAGES.items()
        if importer == package_name
    )

    # What the [all] extra offers is the closure of everything a package can
    # hand back, so it keeps the hubs as well: a hard dependency on one of
    # them installs that package but not what it in turn references.
    optional_dependencies = [
        f"{dep}[all]>={FAMILY_VERSIONS[package_families[dep]]}"
        for dep in sorted(referenced)
    ]

    write_projection_project_files(
        package_path,
        namespaces,
        module_names,
        FAMILY_VERSIONS[family],
        dependencies,
        optional_dependencies,
        versions.NUGET_PACKAGES[family],
        NUGET_VERSIONS[family],
        component,
    )

# create requirements.txt for the hand-written interop projects

for path in INTEROP_PATH.glob("winrt-*"):
    family = package_families[packages.interop_distribution(path)]

    # KeyError here means a new interop package needs a row in the table
    interop_deps = INTEROP_DEPENDENCIES[packages.interop_distribution(path)]

    with open_if_changed(path / "requirements.txt") as req:
        req.writelines(
            [
                "# This file is generated by scripts/generate-pyproject.py\n",
                "\n",
                f"{RUNTIME_REQUIREMENT}\n",
            ]
            + [
                format_hard_requirement(
                    dep,
                    FAMILY_VERSIONS[package_families[dep]],
                    package_families[dep] == family,
                )
                + "\n"
                for dep in interop_deps
            ]
        )

# create the packaging of the table compiler

with open_if_changed(TABLE_PATH / "pyproject.toml") as f:
    f.write(
        templates.TABLE_PYPROJECT_TOML.format(
            classifiers=templates.CLASSIFIERS,
            project_urls=format_project_urls(),
            package_name="winrt-table-compiler",
            version=RUNTIME_VERSION,
            description="Compiler for the projection tables of PyWinRT",
        )
    )

with open_if_changed(TABLE_PATH / "winrt" / "table" / "version.py") as f:
    f.write(templates.TABLE_VERSION_PY.format(version=RUNTIME_VERSION))

with open_if_changed(TABLE_PATH / "README.md") as f:
    f.write(templates.README.format(package_name="winrt-table-compiler"))
    f.write(templates.TABLE_README)

write_license(TABLE_PATH)

# create the packaging of the packages that redistribute a component .dll

for package_name, (importer, module, payload) in REDIST_PACKAGES.items():
    write_dll_project_files(package_name, importer, module, payload)

# create pyproject.toml files for the packages that compile

write_compiled_project_files(
    RUNTIME_PATH,
    "winrt.system",
    "_winrt",
    [
        "src/_winrt.cpp",
        "src/_winrt_array.cpp",
        "src/_winrt_box.cpp",
        "src/_winrt_buffer.cpp",
        "src/arrays.cpp",
        "src/async.cpp",
        "src/callbacks.cpp",
        "src/collections.cpp",
        "src/compose.cpp",
        "src/delegates.cpp",
        "src/enums.cpp",
        "src/errors.cpp",
        "src/generics.cpp",
        "src/implements.cpp",
        "src/interp.cpp",
        "src/members.cpp",
        "src/numerics-statics.cpp",
        "src/numerics.cpp",
        "src/objects.cpp",
        "src/protocols.cpp",
        "src/pycollections.cpp",
        "src/pymapping.cpp",
        "src/pysequence.cpp",
        "src/runtime.cpp",
        "src/shapes.cpp",
        "src/structs.cpp",
        "src/table.cpp",
        "src/types.cpp",
    ],
    package_name="winrt-runtime",
)

for package_path in INTEROP_PATH.glob("winrt-*"):
    package_name = packages.interop_distribution(package_path)
    family = package_families[package_name]
    root_package = package_name[: package_name.rindex("-")]
    namespace = package_name.removeprefix(f"{root_package}-")
    module_name = (
        f"{root_package.replace('-', '_')}.{winrt_ns_to_py_package(namespace)}"
    )
    ext_module_name = f"_{module_name.replace('.', '_')}"

    write_compiled_project_files(
        package_path,
        module_name,
        ext_module_name,
        [f"py.{namespace}.cpp"],
        package_name=package_name,
    )

    write_readme(
        package_path,
        package_name,
        [namespace],
        [module_name],
        versions.NUGET_PACKAGES[family],
        NUGET_VERSIONS[family],
    )

# An interop package is still built by setuptools, which reads the version out
# of a file rather than out of pyproject.toml.

for path in INTEROP_PATH.glob("winrt-*"):
    with open_if_changed(path / "version.txt") as f:
        family = package_families[packages.interop_distribution(path)]

        f.write(f"{FAMILY_VERSIONS[family]}\n")
