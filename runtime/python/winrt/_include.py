import os


def get_include() -> str:
    """
    Gets the directory containing the PyWinRT C++ headers.

    The headers describe the ABI of the ``winrt-runtime`` binary that ships
    alongside them, so a projection module compiled against these headers was
    compiled against this runtime.

    This is intended to be used with the ``include_dirs`` argument of
    ``setuptools.setup()``, where the headers are then included as, e.g.,
    ``#include <pywinrt/base.h>``.
    """
    return os.path.join(os.path.dirname(__file__), "include")


def get_cppwinrt_include() -> str:
    """
    Gets the directory containing the C++/WinRT headers for the Windows SDK
    that PyWinRT is built against.

    Only ``winrt-runtime`` and the interop modules compile against C++/WinRT,
    so what ships here is the transitive closure of the handful of namespace
    headers they include rather than a whole projection. They are carried by
    ``winrt-runtime`` because an interop module builds against them and has to
    build from its source distribution with nothing but PyPI to fetch from.

    Nothing outside the Windows SDK is here. A module that also needs the
    Windows App SDK takes those headers from the App SDK itself, which is
    redistributed with an application rather than part of Windows.

    This is intended to be used with the ``include_dirs`` argument of
    ``setuptools.setup()``, where the headers are then included as, e.g.,
    ``#include <winrt/base.h>``.
    """
    return os.path.join(os.path.dirname(__file__), "include", "cppwinrt")
