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

# Every package carries the same classifiers, and the versions of Python named
# here are the ones winrt-runtime is built for: a projection package runs on
# whichever of them the runtime it resolved supports, so saying less would
# understate what an install can use.
CLASSIFIERS = """classifiers = [
    "Operating System :: Microsoft :: Windows",
    "Programming Language :: Python :: 3",
    "Programming Language :: Python :: 3.11",
    "Programming Language :: Python :: 3.12",
    "Programming Language :: Python :: 3.13",
    "Programming Language :: Python :: 3.14",
    "Programming Language :: Python :: 3.15",
    "Programming Language :: Python :: Implementation :: CPython",
    "Intended Audience :: Developers",
]"""

# The links PyPI shows beside a package. One repository publishes all of
# them, so only the changelog differs. There is no Homepage: that label is
# held for a website of PyWinRT's own, and neither the documentation nor the
# repository is one - both are already listed under the label that names them.
PROJECT_URLS_TEMPLATE = """[project.urls]
Documentation = "https://pywinrt.readthedocs.io"
Repository = "https://github.com/pywinrt/pywinrt"
Changelog = "https://github.com/pywinrt/pywinrt/blob/main/{changelog}"
"""

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

    return PROJECT_URLS_TEMPLATE.format(changelog=changelog)


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
license-files = ["LICENSE"]
{classifiers}
requires-python = ">=3.11"
dependencies = [
{dependencies}]
{optional_dependencies}
{project_urls}
[tool.hatch.build.targets.sdist]
# The text form of the table is what the source distribution carries, and the
# compiled form is build output wherever it turns up. Hatchling reads no
# .gitignore above the package, so it has to be told about the latter.
only-include = ["{root_dir}", "LICENSE", "README.md"]
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
license-files = ["LICENSE"]
{classifiers}
requires-python = ">=3.11"

{project_urls}
# what a projection package's build calls to compile its tables
[project.entry-points.hatch]
winrt-table-compiler = "winrt.table.build_hooks"

[tool.hatch.build.targets.sdist]
only-include = ["winrt", "LICENSE", "README.md"]

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
license = "MIT{extra_license_id}"
license-files = ["LICENSE"{extra_license_files}]
{classifiers}
dynamic = ["version", "dependencies"]
requires-python = ">=3.11"

{project_urls}
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
# extension links against into the wheel. The one DLL any of these packages
# redistributes is put there by its own build, so no package wants a repair
# step. delvewheel cannot run over them anyway: it resolves each imported name
# to a file on disk, and winrt-Windows.System.Interop reaches
# ext-ms-win-rtcore-ntuser-integration-l1-1-0.dll through CoreMessaging.dll,
# which is an API set that the loader resolves without one.
NO_WHEEL_REPAIR = """
# nothing here is copied in after the build, so there is no repair step
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
{setuptools_command_imports}
{headers}
{extra_init}{payload}

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
    cmdclass={cmdclass},
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

DLL_PYPROJECT_TOML_TEMPLATE = """\
# WARNING: Please don't edit this file. It was automatically generated.

[build-system]
requires = ["hatchling"]
build-backend = "hatchling.build"

[project]
name = "{package_name}"
version = "{version}"
description = "{description}"
readme = "README.md"
# The Python in this package is PyWinRT's and the .dll in it is Microsoft's,
# so the expression names both and the text of each one is carried.
license = "MIT AND {license_id}"
license-files = [
    "LICENSE",
{upstream_licenses}]
{classifiers}
requires-python = ">=3.11"

{project_urls}
# Nothing prebuilt goes in the source distribution: the .dll the wheel
# redistributes is fetched by the build, and the bill of materials is written
# by it.
[tool.hatch.build.targets.sdist]
only-include = ["winrt", "hatch_build.py", "LICENSE", "README.md"]

[tool.hatch.build.targets.wheel]
packages = ["winrt"]

# hatch_build.py, which takes the .dll out of the NuGet package, puts the one
# this wheel is for beside the module that registers it, tags the wheel for
# that architecture and writes the bill of materials.
[tool.hatch.build.hooks.custom]
"""

DLL_HATCH_BUILD_TEMPLATE = '''# WARNING: Please don't edit this file. It was automatically generated.

import hashlib
import io
import json
import os
import shutil
import sysconfig
import urllib.request
import zipfile
from pathlib import Path
from typing import Any

from hatchling.builders.hooks.plugin.interface import BuildHookInterface

NUGET_PACKAGE = "{nuget_package}"
NUGET_VERSION = "{nuget_version}"

# Where the .nupkg comes from and what it has to hash to. The build fetches it
# rather than the source distribution carrying the .dll, so that nothing
# calling itself source holds a prebuilt binary; the checksum, taken when this
# file was generated, is what makes the download worth trusting.
NUGET_URL = "{nuget_url}"
NUGET_SHA256 = "{nuget_sha256}"

# An unpacked copy of the NuGet package. scripts/build-bdist.py and
# scripts/build-sdist.py point this at what scripts/fetch-tools.ps1
# downloaded, and it is the escape hatch for a machine that cannot reach
# nuget.org: nothing is downloaded when it is set.
NUGET_PATH_ENV = "{env}"

# Where what the build took out of the NuGet package is kept, so that a second
# build needs neither the network nor the unpacked copy again.
PAYLOAD_DIR = "{payload_dir}"

DLL_NAME = "{dll_name}"
LICENSE_ID = "{license_id}"
MODULE_PATH = "{module_path}"
SBOM_NAME = "{sbom_name}"

# Where in the NuGet package the .dll of each architecture is, keyed by the
# platform tag of the wheel that carries it.
DLL_PATHS = {{
{dll_paths}}}

# Which architecture to build for. There is one wheel per architecture, so
# scripts/build-bdist.py says which; a user building the source distribution
# builds for the machine they are on.
PLAT_NAME_ENV = "PYWINRT_PLAT_NAME"


def get_plat_name() -> str:
    """
    The platform tag of the wheel being built.
    """
    plat_name = os.environ.get(PLAT_NAME_ENV) or sysconfig.get_platform()
    plat_name = plat_name.replace("-", "_").replace(".", "_")

    if plat_name not in DLL_PATHS:
        raise RuntimeError(f"{{NUGET_PACKAGE}} ships no .dll for {{plat_name}}")

    return plat_name


def download_nuget_package() -> bytes:
    """
    The .nupkg, checked against the hash this file was generated with.
    """
    try:
        with urllib.request.urlopen(NUGET_URL, timeout=120) as response:
            data = response.read()
    except OSError as error:
        raise RuntimeError(
            f"could not download {{NUGET_URL}}: {{error}}."
            f" Set {{NUGET_PATH_ENV}} to an unpacked copy of {{NUGET_PACKAGE}}"
            f" {{NUGET_VERSION}} to build without reaching the network."
        ) from error

    digest = hashlib.sha256(data).hexdigest()

    if digest != NUGET_SHA256:
        raise RuntimeError(f"{{NUGET_URL}} hashes to {{digest}} and not to {{NUGET_SHA256}}")

    return data


def get_sbom() -> dict[str, Any]:
    """
    The bill of materials that PEP 770 puts in the wheel.

    Two parties are named and kept apart. The distribution is PyWinRT's, and
    PyWinRT is an independent project rather than a Microsoft one, so it is
    the root component and Microsoft is not its supplier. The .dll inside it
    is Microsoft's, under Microsoft's own license, and is a component of that
    root rather than the root itself.
    """
    return {{
        "$schema": "http://cyclonedx.org/schema/bom-1.6.schema.json",
        "bomFormat": "CycloneDX",
        "specVersion": "1.6",
        "metadata": {{
            "component": {{
                "type": "library",
                "name": "{package_name}",
                "version": "{version}",
                "purl": "pkg:pypi/{normalized_name}@{version}",
                "licenses": [{{"expression": "MIT"}}],
                "supplier": {{"name": "PyWinRT"}},
            }}
        }},
        "components": [
            {{
                "type": "library",
                "name": DLL_NAME,
                "version": NUGET_VERSION,
                "purl": f"pkg:nuget/{{NUGET_PACKAGE}}@{{NUGET_VERSION}}",
                "licenses": [{{"license": {{"name": LICENSE_ID}}}}],
                "supplier": {{"name": "Microsoft Corporation"}},
            }}
        ],
    }}


class DllBuildHook(BuildHookInterface):
    """
    Puts the redistributed .dll into the wheel being built.
    """

    PLUGIN_NAME = "custom"

    def initialize(self, version: str, build_data: dict[str, Any]) -> None:
        if self.target_name != "wheel":
            return

        plat_name = get_plat_name()
        payload_path = Path(self.root) / PAYLOAD_DIR

        # A .dll is data rather than an extension module, so one wheel serves
        # every version of Python and only the architecture narrows it.
        build_data["tag"] = f"py3-none-{{plat_name}}"
        build_data["pure_python"] = False
        build_data["force_include"][
            os.fspath(self.stage_dll(payload_path, DLL_PATHS[plat_name]))
        ] = f"{{MODULE_PATH}}/{{DLL_NAME}}"

        sbom_path = payload_path / SBOM_NAME
        sbom_path.write_text(
            json.dumps(get_sbom(), indent=2) + "\\n",
            encoding="utf-8",
            newline="\\n",
        )
        build_data["sbom_files"].append(f"{{PAYLOAD_DIR}}/{{SBOM_NAME}}")

    def stage_dll(self, payload_path: Path, relative: str) -> Path:
        """
        Takes one architecture's .dll out of the NuGet package.
        """
        destination = payload_path / relative

        if destination.is_file():
            return destination

        destination.parent.mkdir(parents=True, exist_ok=True)

        nuget_path = os.environ.get(NUGET_PATH_ENV)

        if nuget_path is not None:
            shutil.copyfile(Path(nuget_path) / relative, destination)

            return destination

        # A .nupkg is a zip, and the one nuget.org serves is byte for byte the
        # one that unpacking it leaves behind, so the same hash covers both.
        with zipfile.ZipFile(io.BytesIO(download_nuget_package())) as package:
            destination.write_bytes(package.read(relative))

        return destination
'''

DLL_INIT_PY_TEMPLATE = '''# WARNING: Please don't edit this file. It was automatically generated.

"""
{dll_name}, redistributed for PyWinRT.

Importing this module puts the directory the .dll is in on the DLL search
path, which is how the Windows Runtime finds it when one of the types it
implements is activated. The projection package that needs it depends on this
one and imports it, so there is rarely a reason to import it directly.
"""

import os
from pathlib import Path

DLL_NAME = "{dll_name}"

# os.add_dll_directory hands back a handle that takes the directory off the
# search path again when it is closed, so it is held for as long as the module
# is loaded.
_dll_search_path_cookie_ = os.add_dll_directory(
    os.fspath(Path(__file__).parent.resolve())
)


def get_dll_path() -> str:
    """
    The full path of the .dll this package redistributes.
    """
    return os.fspath(Path(__file__).parent / DLL_NAME)
'''

DLL_README_TEMPLATE = """\
Redistributes `{dll_name}`
from version {nuget_version} of the `{nuget_package}` NuGet package, one wheel
per architecture.

{importer}
depends on this package and imports it, which puts the `.dll` on the DLL
search path. There is nothing else in it.
"""

# The Windows App Runtime bootstrapper that the extension module links
# against. It is redistributed in the wheel because Windows looks for a
# module's dependencies in the directory the module was loaded from, which is
# where this puts it, and the upstream license text goes with it.
BOOTSTRAP_PAYLOAD = """

# The Windows App Runtime bootstrapper that the extension module links
# against, and where in the NuGet package the one for each architecture is,
# keyed by the platform name that sysconfig.get_platform() answers with.
PAYLOAD_DLL_NAME = "{dll_name}"
PAYLOAD_LICENSE_ID = "{license_id}"
PAYLOAD_DLL_PATHS = {{
{dll_paths}}}


def get_sbom() -> dict:
    \"\"\"
    The bill of materials that PEP 770 puts in the wheel.

    Two parties are named and kept apart. The distribution is PyWinRT's, and
    PyWinRT is an independent project rather than a Microsoft one, so it is
    the root component and Microsoft is not its supplier. The .dll inside it
    is Microsoft's, under Microsoft's own license, and is a component of that
    root rather than the root itself.
    \"\"\"
    return {{
        "$schema": "http://cyclonedx.org/schema/bom-1.6.schema.json",
        "bomFormat": "CycloneDX",
        "specVersion": "1.6",
        "metadata": {{
            "component": {{
                "type": "library",
                "name": "{package_name}",
                "version": "{version}",
                "purl": "pkg:pypi/{normalized_name}@{version}",
                "licenses": [{{"expression": "MIT"}}],
                "supplier": {{"name": "PyWinRT"}},
            }}
        }},
        "components": [
            {{
                "type": "library",
                "name": PAYLOAD_DLL_NAME,
                "version": "{nuget_version}",
                "purl": "pkg:nuget/{nuget_package}@{nuget_version}",
                "licenses": [{{"license": {{"name": PAYLOAD_LICENSE_ID}}}}],
                "supplier": {{"name": "Microsoft Corporation"}},
            }}
        ],
    }}


class build_py_ex(build_py):
    def run(self) -> None:
        build_py.run(self)

        # Windows looks for an extension module's dependencies in the
        # directory the module was loaded from, so the bootstrapper goes
        # beside the .pyd that links it rather than in a package of its own.
        plat_name = self.get_finalized_command("build").plat_name
        nuget_path = WINDOWS_APP_SDK_PATHS["{env}"]
        destination = pathlib.Path(self.build_lib, "{root_package}")

        self.mkpath(os.fspath(destination))
        self.copy_file(
            os.fspath(nuget_path / PAYLOAD_DLL_PATHS[plat_name]),
            os.fspath(destination / PAYLOAD_DLL_NAME),
        )


class bdist_wheel_ex(bdist_wheel):
    def write_wheelfile(self, wheelfile_base: str, *args, **kwargs) -> None:
        bdist_wheel.write_wheelfile(self, wheelfile_base, *args, **kwargs)

        # Nothing in setuptools writes a PEP 770 bill of materials, and this
        # is the last thing it does to the .dist-info directory before the
        # wheel is archived from it.
        sboms_path = pathlib.Path(wheelfile_base) / "sboms"
        sboms_path.mkdir(exist_ok=True)
        (sboms_path / "{sbom_name}").write_text(
            json.dumps(get_sbom(), indent=2) + "\\n",
            encoding="utf-8",
            newline="\\n",
        )
"""

# The setuptools commands a generated setup.py subclasses. Every one of them
# replaces build_ext; only the package that redistributes a .dll also has
# something to say about what goes into the build and into the wheel.
SETUPTOOLS_COMMAND_IMPORTS = """\
from setuptools.command.build_ext import build_ext
"""

BOOTSTRAP_SETUPTOOLS_COMMAND_IMPORTS = """\
from setuptools.command.bdist_wheel import bdist_wheel
from setuptools.command.build_ext import build_ext
from setuptools.command.build_py import build_py
"""

CMDCLASS = '{"build_ext": build_ext_ex}'

BOOTSTRAP_CMDCLASS = """{
        "bdist_wheel": bdist_wheel_ex,
        "build_ext": build_ext_ex,
        "build_py": build_py_ex,
    }"""

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

WASDK_README_IMPORTANT = """\
**IMPORTANT**: Windows App SDK packages cannot be used without the Windows App
Runtime. This has to be installed manually by the end user. Read the
[PyWinRT Windows App SDK documentation](https://pywinrt.readthedocs.io/en/latest/api/microsoft.html)
for more information.

"""

PROJECTION_PATH = (Path(__file__).parent.parent / "projection").resolve()
TABLE_PATH = (Path(__file__).parent.parent / "table").resolve()
RUNTIME_PATH = (Path(__file__).parent.parent / "runtime").resolve()
INTEROP_PATH = (Path(__file__).parent.parent / "interop").resolve()
REDIST_PATH = (Path(__file__).parent.parent / "redist").resolve()
TOOLS_PATH = (Path(__file__).parent.parent / "_tools").resolve()

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
        version = NUGET_PACKAGE_VERSIONS[self.nuget_package]

        return TOOLS_PATH / f"{self.nuget_package}.{version}"

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


# The Windows App SDK components each hand-written module compiles against,
# named by the environment variable that says where fetch-tools.ps1 unpacked
# each one. The App SDK is a metapackage, so what a module includes is spread
# over several of them.
APP_SDK_INTEROP_COMPONENTS = {
    # Microsoft.UI.Interop.h
    "winrt-Microsoft.UI.Interop": ["WASDK_INTERACTIVE_EXPERIENCES_PATH"],
    # MddBootstrap.h and its import library, and WindowsAppSDK-VersionInfo.h
    "winrt-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap": [
        "WASDK_FOUNDATION_PATH",
        "WASDK_RUNTIME_PATH",
    ],
}


def is_app_sdk_interop_package(name: str) -> bool:
    return name in APP_SDK_INTEROP_COMPONENTS


# The distribution in each interop package's directory. A directory is named
# after the distribution in it, except where that name is long enough to push
# a build path past the 260 characters Windows allows by default: this one is
# 68, and a wheel build puts a .dist-info directory named after it, and a
# licenses directory inside that, underneath it.
INTEROP_DISTRIBUTIONS = {
    "winrt-wasdk-bootstrap": (
        "winrt-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap"
    ),
}


def interop_distribution(package_path: Path) -> str:
    return INTEROP_DISTRIBUTIONS.get(package_path.name, package_path.name)


# Which family each package belongs to, so that a requirement on another one
# can be written with that package's version instead of this one's. Every
# distribution but WinUI 2's is published as winrt-*, so the name says nothing
# about which upstream a package came from: a projection package's family is
# the directory it is generated into, and an interop package's is the family
# whose headers the hand-written C++ compiles against.
package_families = {
    deps_path.parent.name: deps_path.parent.parent.name
    for deps_path in PROJECTION_PATH.glob("**/deps.json")
} | {
    distribution: "wasdk" if is_app_sdk_interop_package(distribution) else "winrt"
    for distribution in map(interop_distribution, INTEROP_PATH.glob("winrt-*"))
}


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
                    WASDK_README_IMPORTANT
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
            COMPILED_PYPROJECT_TOML_TEMPLATE.format(
                classifiers=CLASSIFIERS,
                project_urls=format_project_urls(
                    None if is_runtime else package_families[package_name]
                ),
                extra_requires="" if is_runtime else ', "winrt-runtime"',
                package_name=package_name,
                description="Python projection of Windows Runtime (WinRT) APIs",
                packages_find=(
                    RUNTIME_PACKAGE_FIND
                    if is_runtime
                    else INTEROP_PACKAGE_FIND.format(root_package=root_package)
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
                test_command=RUNTIME_TEST_COMMAND if is_runtime else "",
                local_runtime=(
                    ""
                    if is_runtime
                    else LOCAL_RUNTIME.format(runtime_relative=runtime_relative)
                ),
                extra_cibuildwheel_windows=NO_WHEEL_REPAIR,
            )
        )

    write_license(package_path, redistributes)

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
                setup_py_imports=format_setup_py_imports(
                    ["os", "pathlib"]
                    if is_runtime or needs_app_sdk or own_headers
                    else [],
                    ["json"] if redistributes else [],
                ),
                setuptools_command_imports=(
                    BOOTSTRAP_SETUPTOOLS_COMMAND_IMPORTS
                    if redistributes
                    else SETUPTOOLS_COMMAND_IMPORTS
                ),
                payload=(
                    BOOTSTRAP_PAYLOAD.format(
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
                cmdclass=BOOTSTRAP_CMDCLASS if redistributes else CMDCLASS,
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
                classifiers=CLASSIFIERS,
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
            DLL_PYPROJECT_TOML_TEMPLATE.format(
                classifiers=CLASSIFIERS,
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
            DLL_HATCH_BUILD_TEMPLATE.format(
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
        f.write(DLL_INIT_PY_TEMPLATE.format(dll_name=payload.dll_name))

    with open_if_changed(package_path / module_path / "py.typed"):
        pass

    with open_if_changed(package_path / "README.md") as f:
        f.write(README_TEMPLATE.format(package_name=package_name))
        f.write(
            DLL_README_TEMPLATE.format(
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
    family = package_families[interop_distribution(path)]

    # KeyError here means a new interop package needs a row in the table
    interop_deps = INTEROP_DEPENDENCIES[interop_distribution(path)]

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
            classifiers=CLASSIFIERS,
            project_urls=format_project_urls(),
            package_name="winrt-table-compiler",
            version=RUNTIME_VERSION,
            description="Compiler for the projection tables of PyWinRT",
        )
    )

with open_if_changed(TABLE_PATH / "README.md") as f:
    f.write(README_TEMPLATE.format(package_name="winrt-table-compiler"))
    f.write(TABLE_README)

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
    package_name = interop_distribution(package_path)
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
        family = package_families[interop_distribution(path)]

        f.write(f"{FAMILY_VERSIONS[family]}\n")
