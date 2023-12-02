# WARNING: Please don't edit this file. It was automatically generated.

from setuptools import Extension, setup
from winrt_sdk import get_include_dirs
from winrt_windows_app_sdk import get_include_dirs as get_app_sdk_include_dirs

setup(
    ext_modules=[
        Extension(
            "winrt._winrt_microsoft_ui_xaml_media_imaging",
            sources=["py.Microsoft.UI.Xaml.Media.Imaging.cpp"],
            include_dirs=get_include_dirs()+ get_app_sdk_include_dirs(),
            extra_compile_args=["/std:c++20", "/permissive-"],
            libraries=["windowsapp"],
        )
    ],
)
