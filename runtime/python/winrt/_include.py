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
