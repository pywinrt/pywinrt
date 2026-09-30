# /// script
# dependencies = [
#   "pyinstaller>=6",
#   "typing-extensions>=4.5",
#   "winrt-runtime>=4",
#   "winrt-Windows.ApplicationModel",
#   "winrt-Windows.ApplicationModel.Activation",
#   "winrt-Windows.UI.Xaml.Hosting",
#   "winrt-Windows.UI.Xaml.Hosting.Interop",
#   "winrt-Windows.UI.Xaml.Interop",
#   "winrt-Windows.UI.Xaml.Markup",
#   "winui2-Microsoft.UI.Xaml",
# ]
# ///

"""
Builds hello_app.exe with PyInstaller: uv run build.py

hello_app.py cannot run from python.exe, so this is how it is run: build it,
then start dist\\hello_app\\hello_app.exe. See README.md for why.
"""

from pathlib import Path

import PyInstaller.__main__

here = Path(__file__).parent

PyInstaller.__main__.run(
    [
        "--noconfirm",
        "--distpath",
        str(here / "dist"),
        "--workpath",
        str(here / "build"),
        str(here / "hello_app.spec"),
    ]
)
