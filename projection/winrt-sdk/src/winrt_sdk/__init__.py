from importlib.resources import files


def get_include_dirs() -> list[str]:
    """
    Gets the include directories for the Windows SDK and PyWinRT runtime.

    This is intended to be used with the ``include_dirs`` argument of
    ``setuptools.setup().``
    """
    package_dir = files("winrt_sdk")
    return [str(package_dir.joinpath(d)) for d in ["cppwinrt", "pywinrt"]]
