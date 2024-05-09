# WARNING: Please don't edit this file. It was automatically generated.

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext
from winrt_sdk import get_include_dirs
from winrt_windows_app_sdk import get_include_dirs as get_app_sdk_include_dirs


try:
    import os, pathlib

    WINDOWS_APP_SDK_PATH = pathlib.Path(os.environ["WINDOWS_APP_SDK_PATH"]).resolve()
    print(f"Using Windows App SDK from {WINDOWS_APP_SDK_PATH}")
except KeyError:
    raise RuntimeError("Please set WINDOWS_APP_SDK_PATH environment variable")

class build_ext_ex(build_ext):
    def build_extension(self, ext):
        if self.compiler.compiler_type == "msvc":
            ext.extra_compile_args = ["/std:c++20", "/permissive-"]
        elif self.compiler.compiler_type == "mingw32":
            ext.extra_compile_args = ["-std=c++20", "-D_WIN32_WINNT=_WIN32_WINNT_WIN10"]
            ext.extra_link_args = ["-loleaut32"]
        else:
            raise ValueError(f"Unsupported compiler: {self.compiler.compiler_type}")

        target = self.plat_name.replace("32", "-x86").replace("amd", "x").replace("win", "win10")
        ext.library_dirs = [str(WINDOWS_APP_SDK_PATH / "lib" / target)]

        build_ext.build_extension(self, ext)


setup(
    cmdclass = {'build_ext': build_ext_ex},
    ext_modules=[
        Extension(
            "winrt._winrt_microsoft_windows_applicationmodel_dynamicdependency_bootstrap",
            sources=["py.Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap.cpp"],
            include_dirs=get_include_dirs()+ get_app_sdk_include_dirs()+ [str(WINDOWS_APP_SDK_PATH / "include")],
            libraries=["windowsapp", "Microsoft.WindowsAppRuntime.Bootstrap"],
        )
    ],
)
