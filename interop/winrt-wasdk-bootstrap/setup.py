# WARNING: Please don't edit this file. It was automatically generated.

import json
import os
import pathlib

from setuptools import Extension, setup
from setuptools.command.bdist_wheel import bdist_wheel
from setuptools.command.build_ext import build_ext
from setuptools.command.build_py import build_py

from winrt._include import get_cppwinrt_include, get_include

INCLUDE_DIRS = [get_include(), get_cppwinrt_include()]

WINDOWS_APP_SDK_PATHS = {}

for name in ["WASDK_FOUNDATION_PATH", "WASDK_RUNTIME_PATH"]:
    try:
        WINDOWS_APP_SDK_PATHS[name] = pathlib.Path(os.environ[name]).resolve()
    except KeyError:
        raise RuntimeError(f"Please set the {name} environment variable")

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


def get_sbom() -> dict:
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
                "name": "winrt-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap",
                "version": "4!2.5.1",
                "purl": "pkg:pypi/winrt-microsoft-windows-applicationmodel-dynamicdependency-bootstrap@4!2.5.1",
                "licenses": [{"expression": "MIT"}],
                "supplier": {"name": "PyWinRT"},
            }
        },
        "components": [
            {
                "type": "library",
                "name": PAYLOAD_DLL_NAME,
                "version": "2.3.12",
                "purl": "pkg:nuget/Microsoft.WindowsAppSDK.Foundation@2.3.12",
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
        plat_name = self.get_finalized_command("build").plat_name
        nuget_path = WINDOWS_APP_SDK_PATHS["WASDK_FOUNDATION_PATH"]
        destination = pathlib.Path(self.build_lib, "winrt")

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
        (sboms_path / "cyclonedx.json").write_text(
            json.dumps(get_sbom(), indent=2) + "\n",
            encoding="utf-8",
            newline="\n",
        )


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
