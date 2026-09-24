import json
import pathlib
import subprocess
import sys

REPO_ROOT_PATH = pathlib.Path(__file__).parent.parent.resolve()

TOOLS_JSON_PATH = REPO_ROOT_PATH / ".config" / "_tools.json"

with open(TOOLS_JSON_PATH) as f:
    tools_json = json.load(f)


def versioned_package(package: str) -> str:
    return f"{package}.{tools_json[package]}"


PROJECTION_PATH = REPO_ROOT_PATH / "projection"
RUNTIME_PATH = REPO_ROOT_PATH / "runtime"
# The ABI call shape census, which every run merges into and which the runtime
# compiles the trampolines from. See runtime/src/table-format.md.
RUNTIME_SRC_PATH = RUNTIME_PATH / "src"

DOTNET: list[str] = []
PYWINRT_EXE: str | pathlib.Path

# must match <TargetFramework> in PyWinRT/PyWinRT.csproj
PYWINRT_TFM = "net10.0"
PYWINRT_BUILD_PATH = REPO_ROOT_PATH / "PyWinRT" / "bin"

if "--dotnet" in sys.argv:
    DOTNET.append("dotnet")
    PYWINRT_EXE = "pywinrt"

    subprocess.check_call(DOTNET + ["tool", "list", "PyWinRT"])

elif "--debug" in sys.argv:
    PYWINRT_EXE = PYWINRT_BUILD_PATH / "Debug" / PYWINRT_TFM / "PyWinRT.exe"

    if not PYWINRT_EXE.exists():
        raise RuntimeError("PyWinRT.exe not found. Please run `dotnet build PyWinRT`")
else:
    PYWINRT_EXE = PYWINRT_BUILD_PATH / "Release" / PYWINRT_TFM / "PyWinRT.exe"

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
    / versioned_package("Microsoft.Windows.SDK.CPP")
    / "c"
    / "References"
    / "10.0.28000.0"
)
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
        "--emit-shapes",
        RUNTIME_SRC_PATH,
        "--output",
        PROJECTION_PATH / "winrt",
        "--nullability-json",
        WINDOWS_SDK_NULLABILITY_JSON_PATH,
    ]
    + include_args
)

# generate code for WebView2

WEBVIEW2_PACKAGE_METADATA = (
    REPO_ROOT_PATH
    / "_tools"
    / versioned_package("Microsoft.Web.WebView2")
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
        "--emit-shapes",
        RUNTIME_SRC_PATH,
        "--output",
        PROJECTION_PATH / "webview2",
        "--nullability-json",
        WEBVIEW2_NULLABILITY_JSON_PATH,
        "--component-dlls",
    ]
)

# generate code for microsoft ui xaml (winui2)

MICROSOFT_UI_XAML_PACKAGE_METADATA = (
    REPO_ROOT_PATH
    / "_tools"
    / versioned_package("Microsoft.UI.Xaml")
    / "lib"
    / "uap10.0"
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
        "--emit-shapes",
        RUNTIME_SRC_PATH,
        "--output",
        PROJECTION_PATH / "winui2",
        "--nullability-json",
        MICROSOFT_UI_XAML_NULLABILITY_JSON_PATH,
    ]
)

# generate code for windows app sdk (winui3)

WINDOWS_APP_SDK_PACKAGE_METADATA = (
    REPO_ROOT_PATH
    / "_tools"
    / versioned_package("Microsoft.WindowsAppSDK")
    / "lib"
    / "uap10.0"
)
WINDOWS_APP_SDK_PACKAGE_METADATA2 = (
    REPO_ROOT_PATH
    / "_tools"
    / versioned_package("Microsoft.WindowsAppSDK")
    / "lib"
    / "uap10.0.18362"
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
        "--emit-shapes",
        RUNTIME_SRC_PATH,
        "--output",
        PROJECTION_PATH / "winui3",
        "--nullability-json",
        WINDOWS_APP_SDK_NULLABILITY_JSON_PATH,
    ]
)

# generate code for test component

TEST_PACKAGE_METADATA = (
    REPO_ROOT_PATH
    / "_tools"
    / versioned_package("PyWinRT.TestWinRT")
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
        "--emit-shapes",
        RUNTIME_SRC_PATH,
        "--output",
        PROJECTION_PATH / "test-winrt",
        "--nullability-json",
        TEST_PACKAGE_NULLABILITY_JSON,
        "--component-dlls",
    ]
)
