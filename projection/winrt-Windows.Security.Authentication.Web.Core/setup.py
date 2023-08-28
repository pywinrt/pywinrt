# WARNING: Please don't edit this file. It was automatically generated.

from setuptools import Extension, setup
from winrt_sdk import get_include_dirs

setup(
    ext_modules=[
        Extension(
            "winrt._winrt_windows_security_authentication_web_core",
            sources=["py.Windows.Security.Authentication.Web.Core.cpp"],
            include_dirs=get_include_dirs(),
            extra_compile_args=["/std:c++20", "/permissive-"],
            libraries=["windowsapp"],
        )
    ],
    package_data={"winrt": ["*.pyi"]},
)
