import pathlib
import re
import subprocess
import sys

REPO_ROOT_PATH = pathlib.Path(__file__).parent.parent.resolve()
PROJECTION_PATH = REPO_ROOT_PATH / "projection"

DOTNET: list[str] = []
PYWINRT_EXE: str | pathlib.Path

if "--dotnet" in sys.argv:
    DOTNET.append("dotnet")
    PYWINRT_EXE = "pywinrt"

    subprocess.check_call(DOTNET + ["tool", "list", "PyWinRT"])

elif "--debug" in sys.argv:
    PYWINRT_EXE = (
        REPO_ROOT_PATH / "PyWinRT" / "bin" / "Debug" / "net8.0" / "PyWinRT.exe"
    )

    if not PYWINRT_EXE.exists():
        raise RuntimeError("PyWinRT.exe not found. Please run `dotnet build PyWinRT`")
else:
    PYWINRT_EXE = (
        REPO_ROOT_PATH / "PyWinRT" / "bin" / "Release" / "net8.0" / "PyWinRT.exe"
    )

    if not PYWINRT_EXE.exists():
        raise RuntimeError(
            "PyWinRT.exe not found. Please run `dotnet build PyWinRT -c Release`"
        )


# generate code for windows sdk

MINIMAL_NAMESPACES = [
    "Windows.Data.Json",
    "Windows.Devices.Geolocation",
    "Windows.Foundation",
    "Windows.Graphics.Capture",
    "Windows.Graphics.DirectX",
    "Windows.Storage.Streams",
]
WINDOWS_SDK = (
    REPO_ROOT_PATH
    / "_tools"
    / "Microsoft.Windows.SDK.CPP"
    / "c"
    / "References"
    / "10.0.26100.0"
)
SDK_PACKAGE_PATH = PROJECTION_PATH / "winrt-sdk" / "src" / "winrt_sdk" / "pywinrt"
WINDOWS_SDK_NULLABILITY_JSON_PATH = REPO_ROOT_PATH / "nullability" / "windows-sdk.json"

include_args: list[str] = []

if "--minimal" in sys.argv:
    for ns in MINIMAL_NAMESPACES:
        include_args.extend(["-include", ns])

subprocess.check_call(
    DOTNET
    + [
        PYWINRT_EXE,
        "--input",
        f"winrt;{WINDOWS_SDK}",
        "--output",
        PROJECTION_PATH / "winrt",
        "--header-path",
        SDK_PACKAGE_PATH,
        "--nullability-json",
        WINDOWS_SDK_NULLABILITY_JSON_PATH,
    ]
    + include_args
)

# generate code for WebView2

WEBVIEW2_PACKAGE_METADATA = (
    REPO_ROOT_PATH
    / "_tools"
    / "Microsoft.Web.WebView2"
    / "lib"
    / "Microsoft.Web.WebView2.Core.winmd"
)
WEBVIEW2_NULLABILITY_JSON_PATH = REPO_ROOT_PATH / "nullability" / "webview2.json"

subprocess.check_call(
    DOTNET
    + [
        PYWINRT_EXE,
        "--input",
        f"webview2;{WEBVIEW2_PACKAGE_METADATA}",
        "--reference",
        f"winrt;{WINDOWS_SDK}",
        "--output",
        PROJECTION_PATH / "webview2",
        "--nullability-json",
        WEBVIEW2_NULLABILITY_JSON_PATH,
        "--component-dlls",
    ]
)

# generate code for microsoft ui xaml (winui2)

MICROSOFT_UI_XAML_PACKAGE_METADATA = (
    REPO_ROOT_PATH / "_tools" / "Microsoft.UI.Xaml" / "lib" / "uap10.0"
)
MICROSOFT_UI_XAML_PACKAGE_PATH = (
    PROJECTION_PATH
    / "winrt-Microsoft.UI.Xaml"
    / "src"
    / "winrt_microsoft_ui_xaml"
    / "pywinrt"
)
MICROSOFT_UI_XAML_NULLABILITY_JSON_PATH = (
    REPO_ROOT_PATH / "nullability" / "microsoft.ui.xaml.json"
)

subprocess.check_call(
    DOTNET
    + [
        PYWINRT_EXE,
        "--input",
        f"winui2;{MICROSOFT_UI_XAML_PACKAGE_METADATA}",
        "--reference",
        f"webview2;{WEBVIEW2_PACKAGE_METADATA}",
        "--reference",
        f"winrt;{WINDOWS_SDK}",
        "--output",
        PROJECTION_PATH / "winui2",
        "--header-path",
        MICROSOFT_UI_XAML_PACKAGE_PATH,
        "--nullability-json",
        MICROSOFT_UI_XAML_NULLABILITY_JSON_PATH,
    ]
)

# generate code for windows app sdk (winui3)

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
    / "pywinrt"
)
WINDOWS_APP_SDK_NULLABILITY_JSON_PATH = (
    REPO_ROOT_PATH / "nullability" / "windows-app-sdk.json"
)

subprocess.check_call(
    DOTNET
    + [
        PYWINRT_EXE,
        "--input",
        f"winui3;{WINDOWS_APP_SDK_PACKAGE_METADATA}",
        "--input",
        f"winui3;{WINDOWS_APP_SDK_PACKAGE_METADATA2}",
        "--reference",
        f"webview2;{WEBVIEW2_PACKAGE_METADATA}",
        "--reference",
        f"winrt;{WINDOWS_SDK}",
        "--output",
        PROJECTION_PATH / "winui3",
        "--header-path",
        WINDOWS_APP_SDK_PACKAGE_PATH,
        "--nullability-json",
        WINDOWS_APP_SDK_NULLABILITY_JSON_PATH,
    ]
)

# generate code for test component

TEST_PACKAGE_METADATA = (
    REPO_ROOT_PATH
    / "_tools"
    / "PyWinRT.TestWinRT"
    / "lib"
    / "uap10.0"
    / "TestComponent.winmd"
)

TEST_PACKAGE_NULLABILITY_JSON = REPO_ROOT_PATH / "nullability" / "test-winrt.json"

subprocess.check_call(
    DOTNET
    + [
        PYWINRT_EXE,
        "--input",
        f"test-winrt;{TEST_PACKAGE_METADATA}",
        "--reference",
        f"winrt;{WINDOWS_SDK}",
        "--output",
        PROJECTION_PATH / "test-winrt",
        "--nullability-json",
        TEST_PACKAGE_NULLABILITY_JSON,
        "--component-dlls",
    ]
)

# create version.txt for winrt-runtime package to keep it in sync with PyWinRT.exe

runtime_version = subprocess.check_output([PYWINRT_EXE, "--version"], text=True).split(
    "+"
)[0]  # trim git version suffix

assert re.match(r"^\d+\.\d+\.\d+$", runtime_version)

with open(PROJECTION_PATH / "winrt-runtime" / "version.txt", "w", newline="\n") as f:
    f.write(runtime_version)
