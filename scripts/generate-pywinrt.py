import pathlib
import subprocess
import sys
from pathlib import Path

VERSION_EPOCH = "1"

def get_wheel_version_from_nuget_version(nuget_version:str) -> str:
    # These are the pre-release version formats that can appear in the covered packages.
    # Please adjust based on the package you want to convert.
    version = nuget_version.replace('-experimental', '.dev')
    version = version.replace('-preview', '.dev')
    version = version.replace('-prerelease.', '.dev')
    version = version.replace('-prerelease', '.dev1')
    version = f"{VERSION_EPOCH}!{version}"
    return version

def get_wheel_version_for_nuget_package(nuget_path: Path) -> str:
    # The version file is added when downloading the nuget packages.
    # See fetch-tools.ps1 for details
    # The _528c suffix is here to avoid file name collisions within the nuget package.
    version_file = nuget_path / "pywinrt_version_52c8"
    if not version_file.exists():
        raise FileNotFoundError()
    with open(version_file, "r") as f:
        return get_wheel_version_from_nuget_version(f.read().strip())

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
WINDOWS_SDK_VERSION = get_wheel_version_for_nuget_package(
    REPO_ROOT_PATH / "_tools" / "Microsoft.Windows.SDK.CPP"
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
        f"winrt;{WINDOWS_SDK};{WINDOWS_SDK_VERSION}",
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
WEBVIEW2_VERSION = get_wheel_version_for_nuget_package(
    REPO_ROOT_PATH / "_tools" / "Microsoft.Web.WebView2"
)
WEBVIEW2_NULLABILITY_JSON_PATH = REPO_ROOT_PATH / "nullability" / "webview2.json"

subprocess.check_call(
    DOTNET
    + [
        PYWINRT_EXE,
        "--input",
        f"webview2;{WEBVIEW2_PACKAGE_METADATA};{WEBVIEW2_VERSION}",
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
MICROSOFT_UI_XAML_VERSION = get_wheel_version_for_nuget_package(
    REPO_ROOT_PATH / "_tools" / "Microsoft.UI.Xaml"
)
MICROSOFT_UI_XAML_DEPENDENCY_ON_WEBVIEW2 = f">={get_wheel_version_from_nuget_version('1.0.2849.39')}"
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
        f"winui2;{MICROSOFT_UI_XAML_PACKAGE_METADATA};{MICROSOFT_UI_XAML_VERSION}",
        "--reference",
        f"webview2;{WEBVIEW2_PACKAGE_METADATA};{MICROSOFT_UI_XAML_DEPENDENCY_ON_WEBVIEW2}",
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
WINDOWS_APP_SDK_VERSION = get_wheel_version_for_nuget_package(
    REPO_ROOT_PATH / "_tools" / "Microsoft.WindowsAppSDK"
)
WINDOWS_APP_SDK_DEPENDENCY_ON_WEBVIEW2 = f">={get_wheel_version_from_nuget_version('1.0.2903.40')}"
WINDOWS_APP_SDK_DEPENDENCY_ON_WINDOWS_SDK = f">={get_wheel_version_from_nuget_version('10.0.22621.756')}"
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
        f"winui3;{WINDOWS_APP_SDK_PACKAGE_METADATA};{WINDOWS_APP_SDK_VERSION}",
        "--input",
        f"winui3;{WINDOWS_APP_SDK_PACKAGE_METADATA2};{WINDOWS_APP_SDK_VERSION}",
        "--reference",
        f"webview2;{WEBVIEW2_PACKAGE_METADATA};{WINDOWS_APP_SDK_DEPENDENCY_ON_WEBVIEW2}",
        "--reference",
        f"winrt;{WINDOWS_SDK};{WINDOWS_APP_SDK_DEPENDENCY_ON_WINDOWS_SDK}",
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
TEST_PACKAGE_VERSION = get_wheel_version_for_nuget_package(
    REPO_ROOT_PATH / "_tools" / "PyWinRT.TestWinRT"
)

TEST_PACKAGE_NULLABILITY_JSON = REPO_ROOT_PATH / "nullability" / "test-winrt.json"

subprocess.check_call(
    DOTNET
    + [
        PYWINRT_EXE,
        "--input",
        f"test-winrt;{TEST_PACKAGE_METADATA};{TEST_PACKAGE_VERSION}",
        "--reference",
        f"winrt;{WINDOWS_SDK}",
        "--output",
        PROJECTION_PATH / "test-winrt",
        "--nullability-json",
        TEST_PACKAGE_NULLABILITY_JSON,
        "--component-dlls",
    ]
)

project_version = subprocess.run(
    DOTNET
    + [
        PYWINRT_EXE,
        "--version"
    ],
    capture_output=True,
    text=True
).stdout
# Remove the local git hash
project_version = project_version.split('+')[0]

for package, version in [
    ("winrt-sdk", WINDOWS_SDK_VERSION),
    ("winrt-Microsoft.UI.Xaml", MICROSOFT_UI_XAML_VERSION),
    ("winrt-WindowsAppSDK", WINDOWS_APP_SDK_VERSION),
    ("winrt-runtime", project_version)
]:
    with open(PROJECTION_PATH / package / "version.txt", "w") as f:
        f.write(version)

for package, version in [
    ("winrt-Windows.Graphics.Capture.Interop", WINDOWS_SDK_VERSION),
    ("winrt-Windows.Graphics.DirectX.Direct3D11.Interop", WINDOWS_SDK_VERSION),
    ("winrt-Windows.Media.Interop", WINDOWS_SDK_VERSION),
    ("winrt-Windows.System.Interop", WINDOWS_SDK_VERSION),
    ("winrt-Windows.UI.Composition.Interop", WINDOWS_SDK_VERSION),
    ("winrt-Windows.UI.Xaml.Hosting.Interop", WINDOWS_SDK_VERSION),
    ("winui3-Microsoft.UI.Interop", WINDOWS_APP_SDK_VERSION),
    ("winui3-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap", WINDOWS_APP_SDK_VERSION)
]:
    with open(PROJECTION_PATH / "interop" / package / "version.txt", "w") as f:
        f.write(version)

# create dependencies for non-generated packages
for package, dependencies in [
    ("winrt-Microsoft.UI.Xaml", [
        "winrt-sdk",
        f"webview2-Microsoft.Web.WebView2.Core{MICROSOFT_UI_XAML_DEPENDENCY_ON_WEBVIEW2}", 
    ]),
    ("winrt-WindowsAppSDK", [
        f"winrt-sdk{WINDOWS_APP_SDK_DEPENDENCY_ON_WINDOWS_SDK}",
        f"webview2-Microsoft.Web.WebView2.Core{WINDOWS_APP_SDK_DEPENDENCY_ON_WEBVIEW2}",
    ])
]:
    with open(PROJECTION_PATH / package / "requirements.txt", "w", newline="\n") as f:
        f.writelines(
            [
                "# This file is generated by scripts/generate-pywinrt.py\n",
                "\n",
                *[f"{dependency}\n" for dependency in dependencies]
            ]
        )
