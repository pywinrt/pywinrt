from collections.abc import Iterator
from contextlib import contextmanager
import io
from itertools import chain
import itertools
import json
import os
from pathlib import Path
import re

# A projection package is data - a table, an __init__.py and a type stub - so
# its metadata is static and its only build step is compiling the table, which
# winrt-runtime's hatchling build hook does. The packages that compile C++ stay
# on setuptools, because that is what knows how to drive a compiler.
PROJECTION_PYPROJECT_TOML_TEMPLATE = """\
# WARNING: Please don't edit this file. It was automatically generated.

[build-system]
requires = ["hatchling", "winrt-table"]
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

# winrt.table.build_hooks in winrt-table, which compiles each table the
# package carries and puts the result beside the __init__.py that loads it.
[tool.hatch.build.targets.wheel.hooks.winrt-table]
"""

# winrt-table compiles a projection table and is what a projection package
# builds with. It is pure Python on purpose: a projection package is data, and
# making it build-depend on winrt-runtime would mean a compiler wherever there
# is no runtime wheel for the interpreter doing the building.
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
winrt-table = "winrt.table.build_hooks"

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
# so its own headers and import libraries come from the NuGet package that
# scripts/fetch-tools.ps1 downloads.
APP_SDK_INIT = """
try:
    WINDOWS_APP_SDK_PATH = pathlib.Path(os.environ["WINDOWS_APP_SDK_PATH"]).resolve()
    print(f"Using Windows App SDK from {WINDOWS_APP_SDK_PATH}")
except KeyError:
    raise RuntimeError("Please set the WINDOWS_APP_SDK_PATH environment variable")

INCLUDE_DIRS.append(os.fspath(WINDOWS_APP_SDK_PATH / "include"))
"""

# An interop package's own headers are not package data - nothing installs
# them - but the source distribution has to carry them or it cannot be built.
INTEROP_MANIFEST_IN = """# WARNING: Please don't edit this file. It was automatically generated.

recursive-include cppwinrt *.h
"""

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

TABLE_HEADER_PATH = RUNTIME_PATH / "src" / "table.h"

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


def remove_if_present(path: Path) -> None:
    """
    Deletes a file this script used to write.

    A projection package carried a setup.py, a version.txt and two
    requirements files while it was a compiled extension. Nothing generates
    them any more, so a tree that still has them is a tree where they would go
    stale.
    """
    path.unlink(missing_ok=True)


def read_table_format(header: Path) -> tuple[int, int]:
    """
    Reads the table format version the runtime in this tree understands.
    """
    text = header.read_text(encoding="utf-8")
    fields = {}

    for name in ["format_major", "format_minor"]:
        match = re.search(rf"constexpr uint16_t {name} = (\d+);", text)

        if not match:
            raise RuntimeError(f"{header} does not declare {name}")

        fields[name] = int(match.group(1))

    return fields["format_major"], fields["format_minor"]


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

    Every interop package has a single source file, which fits on the line it
    is written on. winrt-runtime has nine, which do not.
    """
    if len(sources) == 1:
        return f'["{sources[0]}"]'

    lines = "".join(f'                "{source}",\n' for source in sources)

    return f"[\n{lines}            ]"


def format_dependencies(requirements: list[str]) -> str:
    return "".join(f'    "{requirement}",\n' for requirement in requirements)


def write_readme(package_path: Path, package_name: str, module_name: str) -> None:
    with open_if_changed(package_path / "README.md") as f:
        f.write(README_TEMPLATE.format(package_name=package_name))

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
                    else ""
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
                extra_init=APP_SDK_INIT if needs_app_sdk else "",
                root_package=root_package,
                ext_module=ext_module_name,
                sources=format_sources(sources),
                extra_build=APP_SDK_EXTRA_BUILD if needs_app_sdk else "",
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

    if not is_runtime:
        write_readme(package_path, package_name, module_name)


def write_projection_project_files(
    package_path: Path,
    module_name: str,
    version: str,
    dependencies: list[str],
    optional_dependencies: list[str],
) -> None:
    """
    Writes the packaging of one namespace's projection package.

    Everything in one is data, so the wheel is py3-none-any and the metadata
    is static: the version and the requirements are written here rather than
    read back out of files at build time.
    """
    package_name = package_path.name
    root_dir = module_name.split(".")[0]

    with open_if_changed(package_path / "pyproject.toml") as f:
        f.write(
            PROJECTION_PYPROJECT_TOML_TEMPLATE.format(
                package_name=package_name,
                version=version,
                description="Python projection of Windows Runtime (WinRT) APIs",
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

    with open_if_changed(package_path / Path(*module_name.split(".")) / "py.typed"):
        pass

    write_readme(package_path, package_name, module_name)

    # what a compiled projection package used to carry
    remove_if_present(package_path / "setup.py")
    remove_if_present(package_path / "version.txt")
    remove_if_present(package_path / "requirements.txt")
    remove_if_present(package_path / "all-requirements.txt")


# the version the whole tree is stamped with, which PyWinRT.exe writes here
# when scripts/generate-pywinrt.py runs

with open(RUNTIME_PATH / "version.txt", newline="") as version_file:
    version_txt = version_file.read()

version = version_txt.strip()

# create pyproject.toml files for the projection packages

runtime_table_format = read_table_format(TABLE_HEADER_PATH)

for deps_path in sorted(PROJECTION_PATH.glob("**/deps.json")):
    package_path = deps_path.parent

    with open(deps_path) as deps_file:
        deps = json.load(deps_file)

    # A projection package's only requirement on the runtime is that the
    # runtime reads its table, so the two have to be generated from one tree.
    # The floor this turns into is the pin below until there is a release
    # history to derive one from.
    table_format = (deps["table_format"]["major"], deps["table_format"]["minor"])

    if table_format != runtime_table_format:
        raise RuntimeError(
            f"{deps_path} is table format {table_format[0]}.{table_format[1]}"
            f" and {TABLE_HEADER_PATH.name} reads"
            f" {runtime_table_format[0]}.{runtime_table_format[1]}"
        )

    package_name = package_path.name
    root_package = package_name[: package_name.rindex("-")]
    namespace = package_name.removeprefix(f"{root_package}-")
    module_name = (
        f"{root_package.replace('-', '_')}.{winrt_ns_to_py_package(namespace)}"
    )

    write_projection_project_files(
        package_path,
        module_name,
        version,
        [f"winrt-runtime~={version}.0"]
        + [f"{dep}~={version}.0" for dep in deps["required"]],
        [f"{dep}[all]~={version}.0" for dep in deps["referenced"]],
    )

# create requirements.txt for the hand-written interop projects

for path in chain(INTEROP_PATH.glob("winrt-*"), INTEROP_PATH.glob("winui3-*")):
    # KeyError here means a new interop package needs a row in the table
    interop_deps = INTEROP_DEPENDENCIES[path.name]

    with open_if_changed(path / "requirements.txt") as req:
        req.writelines(
            [
                "# This file is generated by scripts/generate-pyproject.py\n",
                "\n",
                f"winrt-runtime~={version}.0\n",
            ]
            + [f"{dep}~={version}.0\n" for dep in interop_deps]
        )

# create the packaging of the table compiler

with open_if_changed(TABLE_PATH / "pyproject.toml") as f:
    f.write(
        TABLE_PYPROJECT_TOML_TEMPLATE.format(
            package_name="winrt-table",
            version=version,
            description="Compiler for the projection tables of PyWinRT",
        )
    )

with open_if_changed(TABLE_PATH / "README.md") as f:
    f.write(README_TEMPLATE.format(package_name="winrt-table"))
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
    root_package = package_path.name[: package_path.name.rindex("-")]
    namespace = package_path.name.removeprefix(f"{root_package}-")
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

# create version.txt for the projects that read one

for path in itertools.chain(
    INTEROP_PATH.glob("winrt-*"),
    INTEROP_PATH.glob("winui3-*"),
):
    with open_if_changed(path / "version.txt") as f:
        f.write(version_txt)
