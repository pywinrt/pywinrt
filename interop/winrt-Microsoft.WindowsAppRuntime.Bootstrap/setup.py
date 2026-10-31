# WARNING: Please don't edit this file. It was automatically generated.
# setuptools does not annotate the build_ext methods this file overrides.
# pyright: reportUnknownMemberType=false

import hashlib
import io
import json
import os
import pathlib
import shutil
import urllib.request
import zipfile

from setuptools import Extension, setup
from setuptools.command.bdist_wheel import bdist_wheel
from setuptools.command.build import build
from setuptools.command.build_ext import build_ext
from setuptools.command.build_py import build_py

INCLUDE_DIRS: list[str] = []

# Each Windows App SDK component the module compiles against, named by the
# environment variable that points at an unpacked copy of it: what it is
# unpacked as, where nuget.org serves it, and what it has to hash to.
APP_SDK_PACKAGES = {
    "WASDK_FOUNDATION_PATH": (
        "Microsoft.WindowsAppSDK.Foundation.2.6.4",
        "https://api.nuget.org/v3-flatcontainer/microsoft.windowsappsdk.foundation/2.6.4/microsoft.windowsappsdk.foundation.2.6.4.nupkg",
        "d23b968700025000f93d38107708a9771b18dbe0fae57e9843c310556cf84091",
    ),
}


def unpack_app_sdk_package(env: str) -> pathlib.Path:
    """
    Where the component named by @p env is unpacked: the copy the environment
    variable points at, or else one downloaded from nuget.org into build/.
    """
    if env in os.environ:
        return pathlib.Path(os.environ[env]).resolve()

    name, url, sha256 = APP_SDK_PACKAGES[env]
    path = pathlib.Path(__file__).parent.resolve() / "build" / "nuget" / name

    if path.is_dir():
        return path

    try:
        with urllib.request.urlopen(url, timeout=120) as response:
            data = response.read()
    except OSError as error:
        raise RuntimeError(
            f"could not download {url}: {error}. Set {env} to an unpacked"
            f" copy of {name} to build without reaching the network."
        ) from error

    digest = hashlib.sha256(data).hexdigest()

    if digest != sha256:
        raise RuntimeError(f"{url} hashes to {digest} and not to {sha256}")

    # Unpacked beside where it goes and then moved there, so that a build
    # stopped halfway leaves nothing the next one would take for unpacked.
    partial = path.with_name(path.name + ".partial")
    shutil.rmtree(partial, ignore_errors=True)

    with zipfile.ZipFile(io.BytesIO(data)) as package:
        package.extractall(partial)

    partial.rename(path)

    return path


WINDOWS_APP_SDK_PATHS: dict[str, pathlib.Path] = {}

for name in APP_SDK_PACKAGES:
    WINDOWS_APP_SDK_PATHS[name] = unpack_app_sdk_package(name)
    print(f"Using {name} from {WINDOWS_APP_SDK_PATHS[name]}")
    INCLUDE_DIRS.append(os.fspath(WINDOWS_APP_SDK_PATHS[name] / "include"))


# The Windows App Runtime bootstrapper that the extension module links
# against, and where in the NuGet package the one for each architecture is,
# keyed by the platform name that sysconfig.get_platform() answers with.
PAYLOAD_DLL_NAME = "Microsoft.WindowsAppRuntime.Bootstrap.dll"
PAYLOAD_LICENSE_ID = "LicenseRef-Microsoft-WindowsAppSDK"
PAYLOAD_DLL_PATHS = {
    "win32": "runtimes/win-x86/native/Microsoft.WindowsAppRuntime.Bootstrap.dll",
    "win-amd64": "runtimes/win-x64/native/Microsoft.WindowsAppRuntime.Bootstrap.dll",
    "win-arm64": "runtimes/win-arm64/native/Microsoft.WindowsAppRuntime.Bootstrap.dll",
}


def get_sbom() -> dict[str, object]:
    """
    The bill of materials that PEP 770 puts in the wheel.

    Two parties are named and kept apart. The distribution is PyWinRT's, and
    PyWinRT is an independent project rather than a Microsoft one, so it is
    the root component and Microsoft is not its supplier. The .dll inside it
    is Microsoft's, under Microsoft's own license, and is a component of that
    root rather than the root itself.
    """
    return {
        "$schema": "http://cyclonedx.org/schema/bom-1.6.schema.json",
        "bomFormat": "CycloneDX",
        "specVersion": "1.6",
        "metadata": {
            "component": {
                "type": "library",
                "name": "winrt-Microsoft.WindowsAppRuntime.Bootstrap",
                "version": "4!2.6.4",
                "purl": "pkg:pypi/winrt-microsoft-windowsappruntime-bootstrap@4!2.6.4",
                "licenses": [{"expression": "MIT"}],
                "supplier": {"name": "PyWinRT"},
            }
        },
        "components": [
            {
                "type": "library",
                "name": PAYLOAD_DLL_NAME,
                "version": "2.6.4",
                "purl": "pkg:nuget/Microsoft.WindowsAppSDK.Foundation@2.6.4",
                "licenses": [{"license": {"name": PAYLOAD_LICENSE_ID}}],
                "supplier": {"name": "Microsoft Corporation"},
            }
        ],
    }


class build_py_ex(build_py):
    def run(self) -> None:
        build_py.run(self)

        # Windows looks for an extension module's dependencies in the
        # directory the module was loaded from, so the bootstrapper goes
        # beside the .pyd that links it rather than in a package of its own.
        build_command = self.get_finalized_command("build")
        assert isinstance(build_command, build)
        plat_name = build_command.plat_name
        nuget_path = WINDOWS_APP_SDK_PATHS["WASDK_FOUNDATION_PATH"]
        destination = pathlib.Path(self.build_lib, "winrt")

        self.mkpath(os.fspath(destination))
        self.copy_file(
            os.fspath(nuget_path / PAYLOAD_DLL_PATHS[plat_name]),
            os.fspath(destination / PAYLOAD_DLL_NAME),
        )


class bdist_wheel_ex(bdist_wheel):
    def write_wheelfile(self, wheelfile_base: str, *args: str, **kwargs: str) -> None:
        bdist_wheel.write_wheelfile(self, wheelfile_base, *args, **kwargs)

        # Nothing in setuptools writes a PEP 770 bill of materials, and this
        # is the last thing it does to the .dist-info directory before the
        # wheel is archived from it.
        sboms_path = pathlib.Path(wheelfile_base) / "sboms"
        sboms_path.mkdir(exist_ok=True)
        (sboms_path / "cyclonedx.json").write_text(
            json.dumps(get_sbom(), indent=2) + "\n",
            encoding="utf-8",
            newline="\n",
        )


class build_ext_ex(build_ext):
    def build_extension(self, ext: Extension) -> None:
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
            raise ValueError(f"Unsupported compiler: {self.compiler.compiler_type}")

        arch = {"win32": "x86", "win-amd64": "x64", "win-arm64": "arm64"}[self.plat_name]
        ext.library_dirs = [
            os.fspath(WINDOWS_APP_SDK_PATHS["WASDK_FOUNDATION_PATH"] / "lib" / "native" / arch)
        ]

        build_ext.build_extension(self, ext)


setup(
    cmdclass={
        "bdist_wheel": bdist_wheel_ex,
        "build_ext": build_ext_ex,
        "build_py": build_py_ex,
    },
    ext_modules=[
        Extension(
            "winrt._winrt_microsoft_windows_applicationmodel_dynamicdependency_bootstrap",
            sources=["py.Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap.cpp"],
            include_dirs=INCLUDE_DIRS,
            libraries=["windowsapp", "Microsoft.WindowsAppRuntime.Bootstrap"],
        )
    ],
)
