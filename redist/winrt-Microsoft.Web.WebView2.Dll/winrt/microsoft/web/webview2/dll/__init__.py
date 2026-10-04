# WARNING: Please don't edit this file. It was automatically generated.

"""
Microsoft.Web.WebView2.Core.dll, redistributed for PyWinRT.

Importing this module puts the directory the .dll is in on the DLL search
path, which is how the Windows Runtime finds it when one of the types it
implements is activated. The projection package that needs it depends on this
one and imports it, so there is rarely a reason to import it directly other
than to call load().
"""

import os
from pathlib import Path

DLL_NAME = "Microsoft.Web.WebView2.Core.dll"

# os.add_dll_directory hands back a handle that takes the directory off the
# search path again when it is closed, so it is held for as long as the module
# is loaded.
_dll_search_path_cookie_ = os.add_dll_directory(
    os.fspath(Path(__file__).parent.resolve())
)


# The handle load() holds, so that the .dll stays loaded.
_dll_ = None


def get_dll_path() -> str:
    """
    The full path of the .dll this package redistributes.
    """
    return os.fspath(Path(__file__).parent / DLL_NAME)


def load() -> None:
    """
    Load the .dll into the process and keep it loaded.

    Being on the DLL search path is enough for the Windows Runtime, but not for
    code that loads the .dll by name with LoadLibraryW, which does not search
    the directories added to the path; that finds the .dll only once it is
    loaded. The WinUI WebView2 control from Windows App SDK 2.x does that when
    it creates its default environment
    (https://github.com/microsoft/microsoft-ui-xaml/issues/12158). A packaged
    application that carries the .dll itself does not need this.
    """
    global _dll_

    if _dll_ is None:
        import ctypes

        _dll_ = ctypes.WinDLL(get_dll_path())
