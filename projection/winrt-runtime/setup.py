from setuptools import Extension, setup
from winrt_sdk import get_include_dirs

setup(
    ext_modules=[
        Extension(
            "winrt._winrt",
            sources=["_winrt.cpp", "_winrt_array.cpp", "runtime.cpp"],
            include_dirs=get_include_dirs(),
            define_macros=[("WIN32", None), ("_WINDOWS", None)],
            extra_compile_args=["/std:c++20", "/permissive-", "/MT"],
            extra_link_args=["kernel32.lib", "ole32.lib", "oleaut32.lib"],
        )
    ]
)
