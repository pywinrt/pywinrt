# WARNING: Please don't edit this file. It was automatically generated.
# setuptools does not annotate the build_ext methods this file overrides.
# pyright: reportUnknownMemberType=false

import sysconfig

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext

INCLUDE_DIRS: list[str] = []

# compiled against the stable ABI of Python 3.11, except where the interpreter
# is free-threaded, which has none, or MSYS2's, whose modules link against the
# version's own DLL
STABLE_ABI = not (
    sysconfig.get_config_var("Py_GIL_DISABLED")
    or sysconfig.get_platform().startswith("mingw")
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

        build_ext.build_extension(self, ext)


setup(
    cmdclass={"build_ext": build_ext_ex},
    ext_modules=[
        Extension(
            "winrt._winrt_windows_graphics_capture_interop",
            sources=["py.Windows.Graphics.Capture.Interop.cpp"],
            include_dirs=INCLUDE_DIRS,
            libraries=["windowsapp"],
            define_macros=[("Py_LIMITED_API", "0x030B0000")] if STABLE_ABI else [],
            py_limited_api=STABLE_ABI,
        )
    ],
    options={"bdist_wheel": {"py_limited_api": "cp311"}} if STABLE_ABI else {},
)
