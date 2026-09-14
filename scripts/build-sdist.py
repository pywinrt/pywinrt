import json
import os
import subprocess
from glob import iglob
from itertools import chain
from pathlib import Path

PROJECT_DIR = Path(__file__).parent.parent

TOOLS_JSON_PATH = PROJECT_DIR / ".config" / "_tools.json"

with open(TOOLS_JSON_PATH) as f:
    tools_json = json.load(f)


def versioned_package(package: str) -> str:
    return f"{package}.{tools_json[package]}"


PROJECTION_PATH = (PROJECT_DIR / "projection").resolve()
WEBVIEW2_PATH = (
    PROJECT_DIR / "_tools" / versioned_package("Microsoft.Web.WebView2")
).resolve()
MICROSOFT_UI_XAML_PATH = (
    PROJECT_DIR / "_tools" / versioned_package("Microsoft.UI.Xaml")
).resolve()
WINDOWS_APP_SDK_PATH = (
    PROJECT_DIR / "_tools" / versioned_package("Microsoft.WindowsAppSDK")
).resolve()

os.environ["WEBVIEW2_PATH"] = os.fspath(WEBVIEW2_PATH)
os.environ["MICROSOFT_UI_XAML_PATH"] = os.fspath(MICROSOFT_UI_XAML_PATH)
os.environ["WINDOWS_APP_SDK_PATH"] = os.fspath(WINDOWS_APP_SDK_PATH)

# setup.py imports winrt._include to locate the runtime headers, and the
# runtime is not installed here (its own sdist is one of the things we build),
# so point at its source tree instead
os.environ["PYTHONPATH"] = os.pathsep.join(
    filter(
        None,
        [
            os.fspath(PROJECT_DIR / "runtime" / "python"),
            os.environ.get("PYTHONPATH"),
        ],
    )
)


try:
    import winrt_sdk  # noqa: F401
    import winrt_microsoft_ui_xaml  # noqa: F401
    import winrt_windows_app_sdk  # noqa: F401
except ImportError:
    print(
        "winrt-sdk, winrt-Microsoft.UI.Xaml and winrt-WindowsAppSDK must be installed first installed in order for build to succeed."
    )
    exit(1)


for package_path in chain(
    [os.fspath(PROJECT_DIR / "runtime")],
    iglob(os.fspath(PROJECT_DIR / "interop" / "winrt-*")),
    iglob(os.fspath(PROJECT_DIR / "interop" / "winui3-*")),
    iglob(os.fspath(PROJECTION_PATH / "winrt" / "winrt-*")),
    iglob(os.fspath(PROJECTION_PATH / "winui2" / "winui2-*")),
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
