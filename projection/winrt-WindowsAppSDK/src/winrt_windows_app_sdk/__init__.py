from importlib.resources import files
from typing import List


def get_include_dirs() -> List[str]:
    """
    Gets the include directories for the WindowsAppSDK.

    This is intended to be used with the ``include_dirs`` argument of
    ``setuptools.setup().``
    """
    package_dir = files("winrt_windows_app_sdk")
    return [package_dir.joinpath(d) for d in ["cppwinrt", "pywinrt"]]
