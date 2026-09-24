from collections.abc import Iterator
from contextlib import contextmanager
import io
from itertools import chain
import json
import os
from pathlib import Path

# the version scheme, which scripts/tests/test_versions.py covers
import versions

# A projection package is data - a table, an __init__.py and a type stub - so
# its metadata is static and its only build step is compiling the table, which
# winrt-table-compiler's hatchling build hook does. The packages that compile
# C++ stay on setuptools, because that is what knows how to drive a compiler.
PROJECTION_PYPROJECT_TOML_TEMPLATE = """\
# WARNING: Please don't edit this file. It was automatically generated.

[build-system]
requires = ["hatchling", "{table_compiler_requirement}"]
build-backend = "hatchling.build"

[project]
name = "{package_name}"
version = "{version}"
description = "{description}"
readme = "README.md"
license = "MIT"
classifiers = [
    "Operating System :: Microsoft :: Windows",
    "Programming Language :: Python :: 3",
    "Programming Language :: Python :: Implementation :: CPython",
    "Intended Audience :: Developers",
]
requires-python = ">=3.11"
dependencies = [
{dependencies}]
{optional_dependencies}
[project.urls]
# Homepage = "https://github.com/pywinrt/pywinrt"
Documentation = "https://pywinrt.readthedocs.io"
Repository = "https://github.com/pywinrt/pywinrt"
Changelog = "https://github.com/pywinrt/pywinrt/blob/main/CHANGELOG.md"

[tool.hatch.build.targets.sdist]
# The text form of the table is what the source distribution carries, and the
# compiled form is build output wherever it turns up. Hatchling reads no
# .gitignore above the package, so it has to be told about the latter.
only-include = ["{root_dir}", "README.md"]
exclude = ["**/_table.pywinrt"]

[tool.hatch.build.targets.wheel]
packages = ["{root_dir}"]
exclude = ["**/_table.pywinrt.txt"]

# winrt.table.build_hooks in winrt-table-compiler, which compiles each table
# the package carries and puts the result beside the __init__.py that loads it.
[tool.hatch.build.targets.wheel.hooks.winrt-table-compiler]
"""

# winrt-table-compiler compiles a projection table and is what a projection
# package builds with. It is pure Python on purpose: a projection package is
# data, and making it build-depend on winrt-runtime would mean a compiler
# wherever there is no runtime wheel for the interpreter doing the building.
TABLE_PYPROJECT_TOML_TEMPLATE = """# WARNING: Please don't edit this file. It was automatically generated.

[build-system]
requires = ["hatchling"]
build-backend = "hatchling.build"

[project]
name = "{package_name}"
version = "{version}"
description = "{description}"
readme = "README.md"
license = "MIT"
classifiers = [
    "Operating System :: Microsoft :: Windows",
    "Programming Language :: Python :: 3",
    "Programming Language :: Python :: Implementation :: CPython",
    "Intended Audience :: Developers",
]
requires-python = ">=3.11"

[project.urls]
# Homepage = "https://github.com/pywinrt/pywinrt"
Documentation = "https://pywinrt.readthedocs.io"
Repository = "https://github.com/pywinrt/pywinrt"
Changelog = "https://github.com/pywinrt/pywinrt/blob/main/CHANGELOG.md"

# what a projection package's build calls to compile its tables
[project.entry-points.hatch]
winrt-table-compiler = "winrt.table.build_hooks"

[tool.hatch.build.targets.sdist]
only-include = ["winrt", "README.md"]

[tool.hatch.build.targets.wheel]
packages = ["winrt"]
"""

COMPILED_PYPROJECT_TOML_TEMPLATE = """\
# WARNING: Please don't edit this file. It was automatically generated.

[build-system]
requires = ["setuptools>=78"{extra_requires}]
build-backend = "setuptools.build_meta"

[project]
name = "{package_name}"
description = "{description}"
readme = "README.md"
license = "MIT"
classifiers = [
    "Operating System :: Microsoft :: Windows",
    "Programming Language :: Python :: 3",
    "Programming Language :: Python :: Implementation :: CPython",
    "Intended Audience :: Developers",
]
dynamic = ["version", "dependencies"]
requires-python = ">=3.11"

[project.urls]
# Homepage = "https://github.com/pywinrt/pywinrt"
Documentation = "https://pywinrt.readthedocs.io"
Repository = "https://github.com/pywinrt/pywinrt"
Changelog = "https://github.com/pywinrt/pywinrt/blob/main/CHANGELOG.md"

[tool.setuptools.dynamic]
version = {{ file = "version.txt" }}
dependencies = {{ file = "requirements.txt" }}
{packages_find}
[tool.setuptools.package-data]
"*" = ["*.pyi", "py.typed"{extra_package_data}]

[tool.cibuildwheel]{local_runtime}
# don't install winrt-runtime from PyPI
build-frontend = {{ name = "build[uv]", args = ["--skip-dependency-check", "--no-isolation"] }}
before-build = "uv pip install setuptools"
# don't build for PyPy or for the free-threaded interpreters, which the
# projection doesn't support yet
skip = "pp* cp*t-*"
# suppress warnings about ARM64 testing
test-skip = "*-win_arm64"{test_command}

[tool.cibuildwheel.windows]
archs = ["x86", "AMD64", "ARM64"]{extra_cibuildwheel_windows}
"""

# Importing the wheel is the only check that it links against nothing that only
# the machine it was built on happened to have. Python does not look for an
# extension's dependencies on PATH, so a module that picked one up says so here
# even though the builder has it - which is what makes this worth running on the
# machine that just produced it.
#
# Only winrt-runtime can be checked this way. An interop package depends on a
# projection package as well, and those are built by the other half of
# scripts/build-bdist.py, so its test environment cannot be resolved from what
# the compiled half has to hand.
RUNTIME_TEST_COMMAND = '\ntest-command = "python -c \\"import winrt._winrt\\""'

# cibuildwheel's Windows default is to run delvewheel, which copies the DLLs an
# extension links against into the wheel. Only the Windows App Runtime
# bootstrapper is redistributed that way, and the package that loads it says so
# for itself, so every other package wants no repair step at all. delvewheel
# cannot run over them anyway: it resolves each imported name to a file on disk,
# and winrt-Windows.System.Interop reaches
# ext-ms-win-rtcore-ntuser-integration-l1-1-0.dll through CoreMessaging.dll,
# which is an API set that the loader resolves without one.
NO_WHEEL_REPAIR = """
# this package redistributes no DLL of its own, so there is nothing to copy in
repair-wheel-command = \"\""""

# The runtime keeps its Python package tree in python/ so that src/ can hold the
# C++ sources of the extension module. The C++ headers that the rest of the
# projection compiles against live inside the package itself, as numpy and
# pybind11 ship theirs, so that winrt._include can find them from __file__ both
# in a wheel and in this source tree.
RUNTIME_PACKAGE_FIND = """
[tool.setuptools.packages.find]
where = ["python"]
"""

# These are private to the runtime's own translation units, so they are not
# package data the way the public headers are, but the sdist still has to carry
# them or building winrt-runtime from source fails. shapes-generated.h is the
# census of ABI call shapes that the trampolines are instantiated from.
RUNTIME_MANIFEST_IN = """\
# WARNING: Please don't edit this file. It was automatically generated.

include src/arrays.h
include src/async.h
include src/callbacks.h
include src/compose.h
include src/delegates.h
include src/enums.h
include src/generics.h
include src/implements.h
include src/interp.h
include src/members.h
include src/metaclass.h
include src/module_state.h
include src/numerics-statics.h
include src/numerics-values.h
include src/numerics.h
include src/objects.h
include src/protocols.h
include src/pycollections.h
include src/pymapping.h
include src/pysequence.h
include src/shapes-generated.h
include src/shapes.h
include src/structs.h
include src/table.h
include src/types.h
"""

# The runtime carries the headers an interop module compiles against, so the
# build is pointed at the one in this tree rather than at whichever one PyPI
# would hand it. winrt-runtime's own build needs nothing of the sort.
LOCAL_RUNTIME = """
# use the local winrt-runtime build dependency
environment = {{ PYTHONPATH="{runtime_relative}/python" }}"""

# An interop package's directory holds its C++ source, the C++/WinRT headers it
# includes and the Python package it installs, so which of those is the Python
# package has to be said rather than guessed.
INTEROP_PACKAGE_FIND = """
[tool.setuptools.packages.find]
include = ["{root_package}*"]
"""

SETUP_PY_TEMPLATE = """# WARNING: Please don't edit this file. It was automatically generated.
{setup_py_imports}
from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext

{headers}
{extra_init}

class build_ext_ex(build_ext):
    def build_extension(self, ext):
        # nothing in the projection uses dynamic_cast or typeid, so disabling
        # RTTI costs nothing and makes the module about 15% smaller
        if self.compiler.compiler_type == "msvc":
            ext.extra_compile_args = ["/std:c++20", "/permissive-", "/GR-"]
        elif self.compiler.compiler_type == "mingw32":
            ext.extra_compile_args = [
                "-std=c++20",
                "-fno-rtti",
                "-D_WIN32_WINNT=_WIN32_WINNT_WIN10",
            ]
            # GCC 15 complains about some things that it didn't in the past
            ext.extra_compile_args += ["-Wno-strict-aliasing", "-Wno-template-body"]
            ext.extra_link_args = ["-loleaut32"]
        else:
            raise ValueError(f"Unsupported compiler: {{self.compiler.compiler_type}}")
{extra_build}
        build_ext.build_extension(self, ext)


setup(
    cmdclass={{"build_ext": build_ext_ex}},
    ext_modules=[
        Extension(
            "{root_package}.{ext_module}",
            sources={sources},
            include_dirs=INCLUDE_DIRS,
            libraries=["windowsapp"{extra_libraries}],
        )
    ],
)
"""

# winrt-runtime is the package that carries the headers, so it names them by
# path rather than asking winrt._include for them: the module that would
# answer is the one being built, and a source distribution has to build
# without the package it is building being importable.
RUNTIME_HEADERS = """PACKAGE_PATH = pathlib.Path(__file__).parent / "python" / "winrt"

INCLUDE_DIRS = [
    os.fspath(PACKAGE_PATH / "include"),
    os.fspath(PACKAGE_PATH / "include" / "cppwinrt"),
]"""

# Everything else build-depends on winrt-runtime, so the PyWinRT headers and
# the C++/WinRT headers that pywinrt/base.h includes come from whichever
# winrt-runtime the build resolved.
HEADERS = """from winrt._include import get_cppwinrt_include, get_include

INCLUDE_DIRS = [get_include(), get_cppwinrt_include()]"""

# The headers of the namespaces a package includes that the runtime does not
# carry are its own, written beside its setup.py by
# scripts/generate-cppwinrt.py, so that including another namespace is a
# release of this package rather than of the runtime.
OWN_HEADERS = """

# the namespaces this package includes that the runtime does not carry
INCLUDE_DIRS.append(os.fspath(pathlib.Path(__file__).parent / "cppwinrt"))"""

# The Windows App SDK is redistributed with an app rather than part of Windows,
# so its own headers and import libraries come from the NuGet packages that
# scripts/fetch-tools.ps1 downloads. It is a metapackage over components that
# are published separately, so each module names the component that ships what
# it includes.
APP_SDK_INIT = """
WINDOWS_APP_SDK_PATHS = {{}}

for name in [{envs}]:
    try:
        WINDOWS_APP_SDK_PATHS[name] = pathlib.Path(os.environ[name]).resolve()
    except KeyError:
        raise RuntimeError(f"Please set the {{name}} environment variable")

    print(f"Using {{name}} from {{WINDOWS_APP_SDK_PATHS[name]}}")
    INCLUDE_DIRS.append(os.fspath(WINDOWS_APP_SDK_PATHS[name] / "include"))
"""

# An interop package's own headers are not package data - nothing installs
# them - but the source distribution has to carry them or it cannot be built.
INTEROP_MANIFEST_IN = """# WARNING: Please don't edit this file. It was automatically generated.

recursive-include cppwinrt *.h
"""

# The bootstrapper is the only App SDK import library anything here links, and
# it is in the Foundation component.
APP_SDK_EXTRA_BUILD = """
        arch = {"win32": "x86", "win-amd64": "x64", "win-arm64": "arm64"}[self.plat_name]
        ext.library_dirs = [
            os.fspath(WINDOWS_APP_SDK_PATHS["WASDK_FOUNDATION_PATH"] / "lib" / "native" / arch)
        ]
"""

README_TEMPLATE = """\
<!-- warning: Please don't edit this file. It was automatically generated. -->

# {package_name}

"""

BINARY_README_TEMPLATE = """\
{important}Windows Runtime (WinRT) APIs for {namespaces}.

{provides}

The WinRT APIs in it are those of version {nuget_version} of the
`{nuget_package}` NuGet package.{component}
"""

TABLE_README = """\
Compiles the projection tables of Windows Runtime (WinRT) APIs from the
text form a package carries into the binary form `winrt-runtime` reads.

This is a build-time dependency of every PyWinRT projection package.
There is nothing in it to import.
"""

WINUI3_README_IMPORTANT = """\
**IMPORTANT**: Packages in the `winui3-*` namespace cannot be used without the
Windows App Runtime. This has to be installed manually by the end user. Read the
[PyWinRT winui3 documentation](https://pywinrt.readthedocs.io/en/latest/api/winui3/index.html)
for more information.

"""

PROJECTION_PATH = (Path(__file__).parent.parent / "projection").resolve()
TABLE_PATH = (Path(__file__).parent.parent / "table").resolve()
RUNTIME_PATH = (Path(__file__).parent.parent / "runtime").resolve()
INTEROP_PATH = (Path(__file__).parent.parent / "interop").resolve()

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
NUGET_PACKAGE_VERSIONS = json.loads(versions.TOOLS_JSON_PATH.read_text())

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
    "winui3-Microsoft.UI.Interop": [
        # converts the WindowId, DisplayId and IconId structs, which the
        # InteractiveExperiences component owns
        "winui3-Microsoft.WindowsAppSDK.InteractiveExperiences",
    ],
    # only calls the bootstrapper, which has no projected types
    "winui3-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap": [],
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


def avoid_keyword(name: str) -> str:
    if name in PYTHON_KEYWORDS:
        return f"{name}_"

    return name


# The Windows App SDK components each hand-written module compiles against,
# named by the environment variable that says where fetch-tools.ps1 unpacked
# each one. The App SDK is a metapackage, so what a module includes is spread
# over several of them.
APP_SDK_INTEROP_COMPONENTS = {
    # Microsoft.UI.Interop.h
    "winui3-Microsoft.UI.Interop": ["WASDK_INTERACTIVE_EXPERIENCES_PATH"],
    # MddBootstrap.h and its import library, and WindowsAppSDK-VersionInfo.h
    "winui3-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap": [
        "WASDK_FOUNDATION_PATH",
        "WASDK_RUNTIME_PATH",
    ],
}


def is_app_sdk_interop_package(name: str) -> bool:
    return name in APP_SDK_INTEROP_COMPONENTS


def is_app_sdk_bootstrap_package(name: str) -> bool:
    return (
        name == "winui3-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap"
    )


def is_webview2_package(name: str) -> bool:
    return name.startswith("webview2-Microsoft.Web.WebView2.")


def is_windows_app_package(name: str) -> bool:
    return name.startswith("winui3-Microsoft.") or is_app_sdk_interop_package(name)


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
        f.write(README_TEMPLATE.format(package_name=package_name))

        f.write(
            BINARY_README_TEMPLATE.format(
                important=(
                    WINUI3_README_IMPORTANT
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
                    "\n\n"
                    + f"Its metadata comes from the `{component}` component,"
                    + "\n"
                    + f"version {NUGET_PACKAGE_VERSIONS[component]}."
                    if component is not None
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
    # winrt-runtime is the one package whose directory name is not its
    # distribution name, since it lives outside of projection/
    package_name = package_name or package_path.name
    is_runtime = package_name == "winrt-runtime"
    runtime_relative = os.path.relpath(RUNTIME_PATH, package_path).replace(os.sep, "/")
    root_package = module_name.split(".")[0]

    with open_if_changed(package_path / "pyproject.toml") as f:
        f.write(
            COMPILED_PYPROJECT_TOML_TEMPLATE.format(
                extra_requires="" if is_runtime else ', "winrt-runtime"',
                package_name=package_name,
                description="Python projection of Windows Runtime (WinRT) APIs",
                packages_find=(
                    RUNTIME_PACKAGE_FIND
                    if is_runtime
                    else INTEROP_PACKAGE_FIND.format(root_package=root_package)
                ),
                extra_package_data=', "*.h"' if is_runtime else "",
                test_command=RUNTIME_TEST_COMMAND if is_runtime else "",
                local_runtime=(
                    ""
                    if is_runtime
                    else LOCAL_RUNTIME.format(runtime_relative=runtime_relative)
                ),
                extra_cibuildwheel_windows=(
                    '\nrepair-wheel-command = "python scripts/add_bootstrap_dll.py {wheel} {dest_dir}"'
                    if is_app_sdk_bootstrap_package(package_name)
                    else NO_WHEEL_REPAIR
                ),
            )
        )

    needs_app_sdk = is_app_sdk_interop_package(package_name)
    own_headers = not is_runtime and any((package_path / "cppwinrt").glob("**/*.h"))

    if is_runtime:
        with open_if_changed(package_path / "MANIFEST.in") as f:
            f.write(RUNTIME_MANIFEST_IN)
    elif own_headers:
        with open_if_changed(package_path / "MANIFEST.in") as f:
            f.write(INTEROP_MANIFEST_IN)
    else:
        remove_if_present(package_path / "MANIFEST.in")

    with open_if_changed(package_path / "setup.py") as f:
        f.write(
            SETUP_PY_TEMPLATE.format(
                setup_py_imports=(
                    "\nimport os\nimport pathlib\n"
                    if is_runtime or needs_app_sdk or own_headers
                    else ""
                ),
                headers=(
                    RUNTIME_HEADERS
                    if is_runtime
                    else HEADERS + (OWN_HEADERS if own_headers else "")
                ),
                extra_init=(
                    APP_SDK_INIT.format(
                        envs=", ".join(
                            f'"{name}"'
                            for name in APP_SDK_INTEROP_COMPONENTS[package_name]
                        )
                    )
                    if needs_app_sdk
                    else ""
                ),
                root_package=root_package,
                ext_module=ext_module_name,
                sources=format_sources(sources),
                extra_build=(
                    APP_SDK_EXTRA_BUILD
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
            PROJECTION_PYPROJECT_TOML_TEMPLATE.format(
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


# Which family each package belongs to, so that a requirement on another one
# can be written with that package's version instead of this one's. A
# projection package's family is the directory it is generated into, and an
# interop package is hand-written C++ compiled against one family's headers
# and named for it.

package_families = {
    deps_path.parent.name: deps_path.parent.parent.name
    for deps_path in PROJECTION_PATH.glob("**/deps.json")
} | {
    path.name: path.name[: path.name.rindex("-")]
    for path in chain(INTEROP_PATH.glob("winrt-*"), INTEROP_PATH.glob("winui3-*"))
}

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

for path in chain(INTEROP_PATH.glob("winrt-*"), INTEROP_PATH.glob("winui3-*")):
    family = package_families[path.name]

    # KeyError here means a new interop package needs a row in the table
    interop_deps = INTEROP_DEPENDENCIES[path.name]

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
        TABLE_PYPROJECT_TOML_TEMPLATE.format(
            package_name="winrt-table-compiler",
            version=RUNTIME_VERSION,
            description="Compiler for the projection tables of PyWinRT",
        )
    )

with open_if_changed(TABLE_PATH / "README.md") as f:
    f.write(README_TEMPLATE.format(package_name="winrt-table-compiler"))
    f.write(TABLE_README)

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

for package_path in chain(
    INTEROP_PATH.glob("winrt-*"),
    INTEROP_PATH.glob("winui3-*"),
):
    package_name = package_path.name
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

for path in chain(INTEROP_PATH.glob("winrt-*"), INTEROP_PATH.glob("winui3-*")):
    with open_if_changed(path / "version.txt") as f:
        f.write(f"{FAMILY_VERSIONS[package_families[path.name]]}\n")
