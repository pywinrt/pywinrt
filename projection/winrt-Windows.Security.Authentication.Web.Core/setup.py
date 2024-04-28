# WARNING: Please don't edit this file. It was automatically generated.

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext
from winrt_sdk import get_include_dirs


class build_ext_ex(build_ext):
    def build_extension(self, ext):
        if self.compiler.compiler_type == "msvc":
            ext.extra_compile_args = ["/std:c++20", "/permissive-"]
        elif self.compiler.compiler_type == "mingw32":
            ext.extra_compile_args = ["-std=c++20", "-D_WIN32_WINNT=_WIN32_WINNT_WIN10"]
            ext.extra_link_args = ["-loleaut32"]
        else:
            raise ValueError(f"Unsupported compiler: {self.compiler.compiler_type}")

        build_ext.build_extension(self, ext)


setup(
    cmdclass = {'build_ext': build_ext_ex},
    ext_modules=[
        Extension(
            "winrt._winrt_windows_security_authentication_web_core",
            sources=["py.Windows.Security.Authentication.Web.Core.cpp"],
            include_dirs=get_include_dirs(),
            libraries=["windowsapp"],
        )
    ],
)
