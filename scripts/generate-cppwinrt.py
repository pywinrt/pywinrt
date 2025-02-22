import pathlib
import shutil
import subprocess

REPO_ROOT_PATH = pathlib.Path(__file__).parent.parent.resolve()
PROJECTION_PATH = REPO_ROOT_PATH / "projection"
CPPWINRT_EXE = (
    REPO_ROOT_PATH / "_tools" / "Microsoft.Windows.CppWinRT" / "bin" / "cppwinrt.exe"
)

if not CPPWINRT_EXE.exists():
    raise RuntimeError("cppwinrt.exe not found. Please run `./scripts/fetch-tools.cmd`")


# generate headers for windows sdk

WINDOWS_SDK = "10.0.26100.0+"
SDK_PACKAGE_PATH = PROJECTION_PATH / "winrt-sdk" / "src" / "winrt_sdk" / "cppwinrt"

shutil.rmtree(SDK_PACKAGE_PATH, ignore_errors=True)
subprocess.check_call(
    [CPPWINRT_EXE, "-input", WINDOWS_SDK, "-output", SDK_PACKAGE_PATH]
)

# apply patches
subprocess.check_call(
    [
        "git",
        "apply",
        "--quiet",
        REPO_ROOT_PATH / "patches" / "cppwinrt-windows-sdk.diff",
    ]
)

# generate headers for WebView2

WEBVIEW2_PACKAGE_METADATA = (
    REPO_ROOT_PATH
    / "_tools"
    / "Microsoft.Web.WebView2"
    / "lib"
    / "Microsoft.Web.WebView2.Core.winmd"
)

WEBVIEW2_PACKAGE_PATH = (
    PROJECTION_PATH / "winrt-Microsoft.Web.WebView2.Core" / "cppwinrt"
)

shutil.rmtree(WEBVIEW2_PACKAGE_PATH, ignore_errors=True)
subprocess.check_call(
    [
        CPPWINRT_EXE,
        "-input",
        WEBVIEW2_PACKAGE_METADATA,
        "-reference",
        WINDOWS_SDK,
        "-output",
        WEBVIEW2_PACKAGE_PATH,
    ]
)

# generate headers for windows app sdk

WINDOWS_APP_SDK_PACKAGE_METADATA = (
    REPO_ROOT_PATH / "_tools" / "Microsoft.WindowsAppSDK" / "lib" / "uap10.0"
)
WINDOWS_APP_SDK_PACKAGE_METADATA2 = (
    REPO_ROOT_PATH / "_tools" / "Microsoft.WindowsAppSDK" / "lib" / "uap10.0.18362"
)
WINDOWS_APP_SDK_PACKAGE_PATH = (
    PROJECTION_PATH
    / "winrt-WindowsAppSDK"
    / "src"
    / "winrt_windows_app_sdk"
    / "cppwinrt"
)

shutil.rmtree(WINDOWS_APP_SDK_PACKAGE_PATH, ignore_errors=True)
subprocess.check_call(
    [
        CPPWINRT_EXE,
        "-input",
        WINDOWS_APP_SDK_PACKAGE_METADATA,
        "-input",
        WINDOWS_APP_SDK_PACKAGE_METADATA2,
        "-reference",
        WEBVIEW2_PACKAGE_METADATA,
        "-reference",
        WINDOWS_SDK,
        "-output",
        WINDOWS_APP_SDK_PACKAGE_PATH,
    ]
)


# generate headers for test component

TEST_PACKAGE_METADATA = (
    REPO_ROOT_PATH
    / "_tools"
    / "PyWinRT.TestWinRT"
    / "lib"
    / "uap10.0"
    / "TestComponent.winmd"
)
TEST_PACKAGE_PATH = PROJECTION_PATH / "test-winrt" / "winrt-TestComponent" / "cppwinrt"

shutil.rmtree(TEST_PACKAGE_PATH, ignore_errors=True)
subprocess.check_call(
    [
        CPPWINRT_EXE,
        "-input",
        TEST_PACKAGE_METADATA,
        "-reference",
        WINDOWS_SDK,
        "-output",
        TEST_PACKAGE_PATH,
    ]
)

# apply patches
subprocess.check_call(
    [
        "git",
        "apply",
        "--quiet",
        REPO_ROOT_PATH / "patches" / "cppwinrt-test-winrt.diff",
    ]
)
