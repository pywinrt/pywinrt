# WARNING: Please don't edit this file. It was automatically generated.

import os
import pathlib

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext

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
    cmdclass={"build_ext": build_ext_ex},
    ext_modules=[
        Extension(
            "winui3._winui3_microsoft_windows_applicationmodel_dynamicdependency_bootstrap",
            sources=["py.Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap.cpp"],
            include_dirs=INCLUDE_DIRS,
            libraries=["windowsapp", "Microsoft.WindowsAppRuntime.Bootstrap"],
        )
    ],
)
