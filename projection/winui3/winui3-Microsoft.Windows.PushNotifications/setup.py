# WARNING: Please don't edit this file. It was automatically generated.

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

        build_ext.build_extension(self, ext)


setup(
    cmdclass={"build_ext": build_ext_ex},
    ext_modules=[
        Extension(
            "winui3._winui3_microsoft_windows_pushnotifications",
            sources=["py.Microsoft.Windows.PushNotifications.cpp"],
            include_dirs=[get_include()] + CPPWINRT_INCLUDE_DIRS,
            libraries=["windowsapp"],
        )
    ],
)
