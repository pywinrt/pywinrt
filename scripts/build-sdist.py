import os
import subprocess
from glob import iglob
from itertools import chain
from pathlib import Path

PROJECT_DIR = Path(__file__).parent.parent
PROJECTION_PATH = (PROJECT_DIR / "projection").resolve()
WEBVIEW2_PATH = (PROJECT_DIR / "_tools/Microsoft.Web.WebView2").resolve()
WINDOWS_APP_SDK_PATH = (PROJECT_DIR / "_tools/Microsoft.WindowsAppSDK").resolve()

os.environ["WEBVIEW2_PATH"] = os.fspath(WEBVIEW2_PATH)
os.environ["WINDOWS_APP_SDK_PATH"] = os.fspath(WINDOWS_APP_SDK_PATH)


try:
    import winrt_sdk  # noqa: F401
    import winrt_windows_app_sdk  # noqa: F401
except ImportError:
    print(
        "winrt-sdk and winrt-WindowsAppSDK must be installed first installed in order for build to succeed."
    )
    exit(1)


for package_path in chain(
    [os.fspath(PROJECTION_PATH / "winrt-runtime")],
    iglob(os.fspath(PROJECTION_PATH / "interop" / "winrt-*")),
    iglob(os.fspath(PROJECTION_PATH / "winrt" / "winrt-*")),
    iglob(os.fspath(PROJECTION_PATH / "winui3" / "winui3-*")),
    iglob(os.fspath(PROJECTION_PATH / "webview2" / "webview2-*")),
):
    subprocess.check_call(
        [
            "pyproject-build",
            "--sdist",
            "--no-isolation",
            "--skip-dependency-check",
            "--outdir",
            "wheelhouse",
            package_path,
        ],
    )
