"""
The text that scripts/generate-pyproject.py writes.

Every file the generator produces is one of these templates filled in, so the
two are kept apart: what a generated file says is here, and which packages get
which of them is next door. A template is written the way the file it produces
reads, which is why the ones that hold Python double the braces of everything
in them that is not a substitution.
"""

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
PROJECT_URLS = """[project.urls]
Documentation = "https://pywinrt.readthedocs.io"
Repository = "https://github.com/pywinrt/pywinrt"
Changelog = "https://github.com/pywinrt/pywinrt/blob/main/{changelog}"
"""

# A projection package is data - a table, an __init__.py and a type stub - so
# its metadata is static and its only build step is compiling the table, which
# winrt-table-compiler's hatchling build hook does. The packages that compile
# C++ stay on setuptools, because that is what knows how to drive a compiler.
PROJECTION_PYPROJECT_TOML = """\
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
TABLE_PYPROJECT_TOML = """# WARNING: Please don't edit this file. It was automatically generated.

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

# The version winrt-table-compiler stamps into every binary table it writes.
# It is the same version the distribution is published with, and it is a module
# rather than metadata read at build time because the compiler is run straight
# from this tree - by the CMake build and by the tests - as well as installed.
TABLE_VERSION_PY = '''# WARNING: Please don't edit this file. It was automatically generated.
"""The version this compiler is published with."""

VERSION = "{version}"
'''

COMPILED_PYPROJECT_TOML = """\
# WARNING: Please don't edit this file. It was automatically generated.

[build-system]
requires = ["setuptools>=78"]
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
"*" = ["*.pyi", "py.typed"]

[tool.cibuildwheel]
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
# An interop package depends on a projection package as well, and those are
# built by the other half of scripts/build-bdist.py, so its test environment
# can only be resolved where that half's wheels are to hand:
# .github/workflows/wheels.yaml hands them to the job that runs this.
TEST_COMMAND = '\ntest-command = "python -c \\"import {module}\\""'

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
# C++ sources of the extension module and the headers they include, none of
# which is installed.
RUNTIME_PACKAGE_FIND = """
[tool.setuptools.packages.find]
where = ["python"]

# A projection package is a table beside an __init__.py, and the type a call
# hands back is resolved by importing the module that owns it while the
# program runs. A freezer sees neither, so winrt-runtime ships the hook that
# tells PyInstaller about both, and this is how PyInstaller finds it without
# the application being frozen having to configure anything.
[project.entry-points.pyinstaller40]
hook-dirs = "winrt.__pyinstaller:get_hook_dirs"
"""

# An interop package's directory holds its C++ source, the C++/WinRT headers it
# includes and the Python package it installs, so which of those is the Python
# package has to be said rather than guessed.
INTEROP_PACKAGE_FIND = """
[tool.setuptools.packages.find]
include = ["{root_package}*"]
"""

SETUP_PY = """# WARNING: Please don't edit this file. It was automatically generated.
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

# winrt-runtime's headers are beside its sources.
RUNTIME_HEADERS = """INCLUDE_PATH = pathlib.Path(__file__).parent / "src" / "include"

INCLUDE_DIRS = [
    os.fspath(INCLUDE_PATH),
    os.fspath(INCLUDE_PATH / "cppwinrt"),
]"""

# An interop module reaches the runtime through Python objects only, so it
# needs no header from winrt-runtime, and one written against raw COM needs no
# C++/WinRT header either.
HEADERS = """INCLUDE_DIRS = []"""

# One written against C++/WinRT carries the whole closure of the namespaces it
# includes, winrt/base.h among them, written beside its setup.py by
# scripts/generate-cppwinrt.py.
CPPWINRT_HEADERS = """INCLUDE_DIRS = [
    os.fspath(pathlib.Path(__file__).parent / "cppwinrt"),
]"""

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

# An interop package's headers are not package data - nothing installs them -
# but the source distribution has to carry them or it cannot be built.
INTEROP_MANIFEST_IN = """# WARNING: Please don't edit this file. It was automatically generated.

include {interop_header}
"""

OWN_HEADERS_MANIFEST_IN = """recursive-include cppwinrt *.h
"""

# The copy of interop/interop.h that each interop package builds with.
INTEROP_HEADER = """// WARNING: Please don't edit this file. It was copied from interop/{interop_header}
// by scripts/generate-pyproject.py.

{text}"""

# The bootstrapper is the only App SDK import library anything here links, and
# it is in the Foundation component.
APP_SDK_EXTRA_BUILD = """
        arch = {"win32": "x86", "win-amd64": "x64", "win-arm64": "arm64"}[self.plat_name]
        ext.library_dirs = [
            os.fspath(WINDOWS_APP_SDK_PATHS["WASDK_FOUNDATION_PATH"] / "lib" / "native" / arch)
        ]
"""

DLL_PYPROJECT_TOML = """\
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

DLL_HATCH_BUILD = '''# WARNING: Please don't edit this file. It was automatically generated.

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

DLL_INIT_PY = '''# WARNING: Please don't edit this file. It was automatically generated.

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

DLL_README = """\
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

README = """\
<!-- warning: Please don't edit this file. It was automatically generated. -->

# {package_name}

"""

BINARY_README = """\
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
