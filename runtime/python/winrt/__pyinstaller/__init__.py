"""
Where PyInstaller finds PyWinRT's hook.

``winrt-runtime`` declares this module in the ``pyinstaller40`` entry point
group, so PyInstaller asks it for the directory the hook is in and needs no
configuration from the application being frozen.
"""

import os


def get_hook_dirs() -> list[str]:
    return [os.path.dirname(__file__)]
