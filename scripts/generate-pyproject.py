from collections.abc import Iterator
from contextlib import contextmanager
import io
from itertools import chain
import itertools
import json
import os
from pathlib import Path

PYPROJECT_TOML_TEMPLATE = """\
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
dynamic = ["version"{extra_dynamic}]
requires-python = ">=3.11"

[project.urls]
# Homepage = "https://github.com/pywinrt/pywinrt"
Documentation = "https://pywinrt.readthedocs.io"
Repository = "https://github.com/pywinrt/pywinrt"
Changelog = "https://github.com/pywinrt/pywinrt/blob/main/CHANGELOG.md"

[tool.setuptools.dynamic]
version = {{ file = "version.txt" }}{dependencies}{optional_dependencies}
{find_src}
"""

# The runtime keeps its Python package tree in python/ so that src/ can hold the
# C++ sources of the extension module. The C++ headers that the rest of the
# projection compiles against live inside the package itself, as numpy and
# pybind11 ship theirs, so that winrt._include.get_include() can find them from
# __file__ both in a wheel and in this source tree.
RUNTIME_PACKAGE_FIND_SRC = """
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

BINARY_PACKAGE_TEMPLATE = """\
[tool.setuptools.package-data]
"*" = ["*.pyi", "py.typed"{component_package_data}]

[tool.cibuildwheel]
# use the local winrt-runtime build dependency
environment = {{ PYTHONPATH="{runtime_relative}/python" }}
# don't install winrt-runtime from PyPI
build-frontend = {{ name = "build[uv]", args = ["--skip-dependency-check", "--no-isolation"] }}
before-build = "uv pip install setuptools"
# don't build for PyPy or for the free-threaded interpreters, which the
# projection doesn't support yet
skip = "pp* cp*t-*"
# suppress warnings about ARM64 testing
test-skip = "*-win_arm64"

[tool.cibuildwheel.windows]
archs = ["x86", "AMD64", "ARM64"]{extra_cibuildwheel_windows}
"""

DEPENDENCIES = """
dependencies = { file = "requirements.txt" }"""

OPTIONAL_DEPENDENCIES = """
optional-dependencies.all = { file = "all-requirements.txt" }"""

FIND_SRC = """
[tool.setuptools.packages.find]
where = ["src"]
"""

SETUP_PY_TEMPLATE = """# WARNING: Please don't edit this file. It was automatically generated.

import os
import pathlib

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext
from winrt._include import get_include

# The C++/WinRT headers that scripts/generate-cppwinrt.py writes. They are
# build output rather than a distribution - the only things that compile
# against them are winrt-runtime and the interop modules - so the build is
# told where they are rather than finding them in site-packages.
try:
    CPPWINRT_PATH = pathlib.Path(os.environ["CPPWINRT_PATH"]).resolve()
except KeyError:
    raise RuntimeError("Please set the CPPWINRT_PATH environment variable")

CPPWINRT_INCLUDE_DIRS = [os.fspath(CPPWINRT_PATH / "windows-sdk")]
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
            include_dirs=[get_include()] + CPPWINRT_INCLUDE_DIRS{extra_include_dirs},
            libraries=["windowsapp"{extra_libraries}],
        ){extra_extension}
    ],
)
"""

EXTRA_EXT_MODULES = """,
        Extension(
            "{root_package}.{ext_module}",
            sources={sources},
            include_dirs=[get_include()] + CPPWINRT_INCLUDE_DIRS,
            libraries=["windowsapp"],
        ),
"""

APP_SDK_INIT = """
CPPWINRT_INCLUDE_DIRS.append(os.fspath(CPPWINRT_PATH / "windows-app-sdk"))

try:
    WINDOWS_APP_SDK_PATH = pathlib.Path(os.environ["WINDOWS_APP_SDK_PATH"]).resolve()
    print(f"Using Windows App SDK from {WINDOWS_APP_SDK_PATH}")
except KeyError:
    raise RuntimeError("Please set the WINDOWS_APP_SDK_PATH environment variable")
"""

SETUP_PY_APP_SDK_INCLUDE_DIRS = (
    '\n                + [os.fspath(WINDOWS_APP_SDK_PATH / "include")]'
)

APP_SDK_EXTRA_BUILD = """
        target = self.plat_name.replace("32", "-x86").replace("amd", "x").replace("win", "win10")
        ext.library_dirs = [os.fspath(WINDOWS_APP_SDK_PATH / "lib" / target)]
"""

README_TEMPLATE = """\
<!-- warning: Please don't edit this file. It was automatically generated. -->

# {package_name}

"""

BINARY_README_TEMPLATE = """\
{important}Windows Runtime (WinRT) APIs for for the `{namespace}` namespace.

This package provides the `{module_name}` module.
"""

WINUI3_README_IMPORTANT = """\
**IMPORTANT**: Packages in the `winui3-*` namespace cannot be used without the
Windows App Runtime. This has to be installed manually by the end user. Read the
[PyWinRT winui3 documentation](https://pywinrt.readthedocs.io/en/latest/api/winui3/index.html)
for more information.

"""

PROJECTION_PATH = (Path(__file__).parent.parent / "projection").resolve()
RUNTIME_PATH = (Path(__file__).parent.parent / "runtime").resolve()
INTEROP_PATH = (Path(__file__).parent.parent / "interop").resolve()

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
        # converts the WindowId, DisplayId and IconId structs
        "winui3-Microsoft.UI",
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


def avoid_keyword(name: str) -> str:
    if name in PYTHON_KEYWORDS:
        return f"{name}_"

    return name


def is_app_sdk_interop_package(name: str) -> bool:
    return name in [
        "winui3-Microsoft.UI.Interop",
        "winui3-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap",
    ]


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

    Every generated package has a single source file, which fits on the line
    it is written on. winrt-runtime has nine, which do not.
    """
    if len(sources) == 1:
        return f'["{sources[0]}"]'

    lines = "".join(f'                "{source}",\n' for source in sources)

    return f"[\n{lines}            ]"


def write_project_files(
    package_path: Path,
    module_name: str,
    ext_module_name: str,
    sources: list[str],
    second_ext_source_file: str | None = None,
    package_name: str | None = None,
) -> None:
    # winrt-runtime is the one package whose directory name is not its
    # distribution name, since it lives outside of projection/
    package_name = package_name or package_path.name
    runtime_relative = os.path.relpath(RUNTIME_PATH, package_path).replace(os.sep, "/")
    has_requirements = (package_path / "requirements.txt").exists()
    has_all_requirements = (package_path / "all-requirements.txt").exists()
    root_package = module_name.split(".")[0]

    with open_if_changed(package_path / "pyproject.toml") as f:
        f.write(
            PYPROJECT_TOML_TEMPLATE.format(
                extra_requires=(
                    "" if package_name == "winrt-runtime" else ', "winrt-runtime"'
                ),
                package_name=package_name,
                description="Python projection of Windows Runtime (WinRT) APIs",
                module_name=module_name,
                extra_dynamic=(', "dependencies"' if has_requirements else "")
                + (', "optional-dependencies"' if has_all_requirements else ""),
                dependencies=DEPENDENCIES if has_requirements else "",
                optional_dependencies=(
                    OPTIONAL_DEPENDENCIES if has_all_requirements else ""
                ),
                find_src=(
                    RUNTIME_PACKAGE_FIND_SRC
                    if package_name == "winrt-runtime"
                    else (FIND_SRC if (package_path / "src").exists() else "")
                ),
            )
        )

        f.write(
            BINARY_PACKAGE_TEMPLATE.format(
                component_package_data=(
                    ', "*.h"' if package_name == "winrt-runtime" else ""
                ),
                runtime_relative=runtime_relative,
                extra_cibuildwheel_windows=(
                    '\nrepair-wheel-command = "python scripts/add_bootstrap_dll.py {wheel} {dest_dir}"'
                    if is_app_sdk_bootstrap_package(package_name)
                    else (
                        '\nrepair-wheel-command = "python scripts/add_webview2_dll.py {wheel} {dest_dir}"'
                        if is_webview2_package(package_name)
                        else ""
                    )
                ),
            )
        )

    if package_name == "winrt-runtime":
        with open_if_changed(package_path / "MANIFEST.in") as f:
            f.write(RUNTIME_MANIFEST_IN)

    with open_if_changed(package_path / "setup.py") as f:
        f.write(
            SETUP_PY_TEMPLATE.format(
                extra_init=(
                    APP_SDK_INIT if is_app_sdk_interop_package(package_name) else ""
                ),
                root_package=root_package,
                ext_module=ext_module_name,
                sources=format_sources(sources),
                extra_include_dirs=(
                    SETUP_PY_APP_SDK_INCLUDE_DIRS
                    if is_app_sdk_interop_package(package_name)
                    else ""
                ),
                extra_build=(
                    APP_SDK_EXTRA_BUILD
                    if is_app_sdk_interop_package(package_name)
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
                extra_extension=(
                    EXTRA_EXT_MODULES.format(
                        root_package=root_package,
                        ext_module=f"{ext_module_name}_2",
                        sources=format_sources([second_ext_source_file]),
                    )
                    if second_ext_source_file
                    and (package_path / second_ext_source_file).exists()
                    else ""
                ),
            )
        )

    if package_name != "winrt-runtime":
        with open_if_changed(package_path / Path(*module_name.split(".")) / "py.typed"):
            pass

        with open_if_changed(package_path / "README.md") as f:
            f.write(
                README_TEMPLATE.format(
                    package_name=package_name,
                )
            )

            f.write(
                BINARY_README_TEMPLATE.format(
                    important=(
                        WINUI3_README_IMPORTANT
                        if is_windows_app_package(package_name)
                        else ""
                    ),
                    namespace=package_name.removeprefix("winrt-"),
                    module_name=module_name,
                )
            )


# create requirements.txt for projected projects

for path in PROJECTION_PATH.glob("**/deps.json"):
    with open(path) as f:
        deps = json.load(f)

    version = deps["pywinrt"]["version"]

    with open_if_changed(path.parent / "requirements.txt") as req:
        req.writelines(
            [
                "# This file is generated by scripts/generate-pyproject.py\n",
                "\n",
                f"winrt-runtime~={version}.0\n",
            ]
            + [f"{dep}~={version}.0\n" for dep in deps["required"]]
        )

    if any(deps["referenced"]):
        with open_if_changed(path.parent / "all-requirements.txt") as req:
            req.writelines(
                [
                    "# This file is generated by scripts/generate-pyproject.py\n",
                    "\n",
                ]
                + [f"{dep}[all]~={version}.0\n" for dep in deps["referenced"]]
            )

# create requirements.txt for the hand-written interop projects

with open(RUNTIME_PATH / "version.txt", newline="") as f:
    version_txt = f.read()

interop_version = version_txt.strip()

for path in chain(INTEROP_PATH.glob("winrt-*"), INTEROP_PATH.glob("winui3-*")):
    # KeyError here means a new interop package needs a row in the table
    interop_deps = INTEROP_DEPENDENCIES[path.name]

    with open_if_changed(path / "requirements.txt") as req:
        req.writelines(
            [
                "# This file is generated by scripts/generate-pyproject.py\n",
                "\n",
                f"winrt-runtime~={interop_version}.0\n",
            ]
            + [f"{dep}~={interop_version}.0\n" for dep in interop_deps]
        )

# create pyproject.toml files for all projected projects

write_project_files(
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
    (PROJECTION_PATH / "winrt").glob("winrt-*"),
    (PROJECTION_PATH / "winui2").glob("winui2-*"),
    (PROJECTION_PATH / "winui3").glob("winui3-*"),
    (PROJECTION_PATH / "webview2").glob("webview2-*"),
    [PROJECTION_PATH / "test-winrt" / "test-winrt-TestComponent"],
):
    root_package = package_path.name[: package_path.name.rindex("-")]
    namespace = package_path.name.removeprefix(f"{root_package}-")
    module_name = (
        f"{root_package.replace('-', '_')}.{winrt_ns_to_py_package(namespace)}"
    )
    ext_module_name = f"_{module_name.replace('.', '_')}"
    source_file = f"py.{namespace}.cpp"
    second_ext_source_file = f"py.{namespace}_2.cpp"

    write_project_files(
        package_path,
        module_name,
        ext_module_name,
        [source_file],
        second_ext_source_file,
    )

# create version.txt for all projects

for path in itertools.chain(
    (PROJECTION_PATH / "winrt").glob("winrt-*"),
    (PROJECTION_PATH / "test-winrt").glob("test-winrt-*"),
    (PROJECTION_PATH / "webview2").glob("webview2-*"),
    (PROJECTION_PATH / "winui2").glob("winui2-*"),
    (PROJECTION_PATH / "winui3").glob("winui3-*"),
    INTEROP_PATH.glob("winrt-*"),
    INTEROP_PATH.glob("winui3-*"),
):
    with open_if_changed(path / "version.txt") as f:
        f.write(version_txt)
