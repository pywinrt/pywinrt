import os
import shutil
import subprocess
import sys
from itertools import chain
from pathlib import Path

PROJECT_DIR = Path(__file__).parent.parent
PROJECTION_PATH = (PROJECT_DIR / "projection").resolve()
WEBVIEW2_PATH = (PROJECT_DIR / "_tools/Microsoft.Web.WebView2").resolve()
WINDOWS_APP_SDK_PATH = (PROJECT_DIR / "_tools/Microsoft.WindowsAppSDK").resolve()

os.environ["WEBVIEW2_PATH"] = os.fspath(WEBVIEW2_PATH)
os.environ["WINDOWS_APP_SDK_PATH"] = os.fspath(WINDOWS_APP_SDK_PATH)

for package_path in chain(
    [PROJECTION_PATH / "winrt-runtime"],
    (PROJECTION_PATH / "interop").glob("winrt-*"),
    PROJECTION_PATH.glob("winrt-Windows.*"),
    PROJECTION_PATH.glob("winrt-Microsoft.*"),
):
    subprocess.check_call(
        [
            "cibuildwheel",
            os.fspath(package_path),
        ]
        + (sys.argv[1:] or ["--platform", "windows"])
    )

    # Build directories are 10s of MBs and will cause CI to run out of space!
    shutil.rmtree(package_path / "build")
