# WARNING: Please don't edit this file. It was automatically generated.

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext
from winrt_sdk import get_include_dirs
from winrt_windows_app_sdk import get_include_dirs as get_app_sdk_include_dirs


class build_ext_ex(build_ext):
    def build_extension(self, ext):
        if self.compiler.compiler_type == "msvc":
            ext.extra_compile_args = ["/std:c++20", "/permissive-"]
        else:
            ext.extra_compile_args = ["-std=c++20"]
            ext.extra_link_args = ["-loleaut32"]

        build_ext.build_extension(self, ext)


setup(
    cmdclass = {'build_ext': build_ext_ex},
    ext_modules=[
        Extension(
            "winrt._winrt_microsoft_graphics_directx",
            sources=["py.Microsoft.Graphics.DirectX.cpp"],
            include_dirs=get_include_dirs()+ get_app_sdk_include_dirs(),
            libraries=["windowsapp"],
        )
    ],
)