# WARNING: Please don't edit this file. It was automatically generated.

from setuptools import Extension, setup
from winrt_sdk import get_include_dirs

setup(
    ext_modules=[
        Extension(
            "winrt.windows.globalization.datetimeformatting._winrt_windows_globalization_datetimeformatting",
            sources=["py.Windows.Globalization.DateTimeFormatting.cpp"],
            include_dirs=get_include_dirs(),
            extra_compile_args=["/std:c++20", "/permissive-"],
            libraries=["windowsapp"],
        )
    ]
)
