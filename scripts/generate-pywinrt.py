import json
import pathlib
import subprocess
import sys

import app_sdk

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
        # The Windows SDK is the only family that keeps the deprecated names
        # its methods had in pywinrt v3.x. The Windows App SDK and WebView2
        # were published under the winui3 and webview2 prefixes then, so code
        # written for them has to be edited anyway, and WinUI 2 is not expected
        # to have users.
        "--legacy-method-aliases",
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

# The one namespace here is published in a distribution named after the NuGet
# package it came from rather than after itself, the way the App SDK components
# are: the package is what the family is versioned by, and a second namespace in
# a later release would belong to the same distribution.
subprocess.check_call(
    DOTNET
    + [
        PYWINRT_EXE,
        "--input",
        f"winrt;Microsoft.Web.WebView2;{WEBVIEW2_PACKAGE_METADATA}",
        "--reference",
        f"winrt;{WINDOWS_SDK}",
        "--emit-shapes",
        RUNTIME_SRC_PATH,
        "--output",
        PROJECTION_PATH / "webview2",
        "--nullability-json",
        WEBVIEW2_NULLABILITY_JSON_PATH,
        # Microsoft.Web.WebView2.Core.dll is architecture-specific and the
        # projection is not, so the .dll is redistributed in a package of its
        # own that this one imports. See scripts/generate-pyproject.py.
        "--dll-package",
        "winrt.microsoft.web.webview2.dll",
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

# The six namespaces here are published in one distribution named after the
# NuGet package they came from rather than one each, the way the App SDK
# components are: the package is what the family is versioned by, so they are
# generated and released together.
subprocess.check_call(
    DOTNET
    + [
        PYWINRT_EXE,
        "--input",
        f"winui2;Microsoft.UI.Xaml;{MICROSOFT_UI_XAML_PACKAGE_METADATA}",
        "--reference",
        f"winrt;Microsoft.Web.WebView2;{WEBVIEW2_PACKAGE_METADATA}",
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

# generate code for windows app sdk

# Each component is published as a distribution of its own, so the input says
# which one a namespace belongs to; the generator groups them and splits out
# any namespace more than one component contributes to.
WINDOWS_APP_SDK_INPUTS = [
    arg
    for distribution, path in app_sdk.metadata_inputs()
    for arg in ("--input", f"winrt;{distribution};{path}")
]
WINDOWS_APP_SDK_NULLABILITY_JSON_PATH = (
    REPO_ROOT_PATH / "nullability" / "windows-app-sdk.json"
)

subprocess.check_call(
    DOTNET
    + [
        PYWINRT_EXE,
    ]
    + WINDOWS_APP_SDK_INPUTS
    + [
        "--reference",
        f"winrt;Microsoft.Web.WebView2;{WEBVIEW2_PACKAGE_METADATA}",
        "--reference",
        f"winrt;{WINDOWS_SDK}",
        "--emit-shapes",
        RUNTIME_SRC_PATH,
        "--output",
        PROJECTION_PATH / "wasdk",
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
