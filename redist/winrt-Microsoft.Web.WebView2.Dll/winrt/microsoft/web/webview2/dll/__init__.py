# WARNING: Please don't edit this file. It was automatically generated.

"""
Microsoft.Web.WebView2.Core.dll, redistributed for PyWinRT.

Importing this module puts the directory the .dll is in on the DLL search
path, which is how the Windows Runtime finds it when one of the types it
implements is activated. The projection package that needs it depends on this
one and imports it, so there is rarely a reason to import it directly.
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


def get_dll_path() -> str:
    """
    The full path of the .dll this package redistributes.
    """
    return os.fspath(Path(__file__).parent / DLL_NAME)
