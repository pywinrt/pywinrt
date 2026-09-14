import json
import pathlib
import shutil
import subprocess

REPO_ROOT_PATH = pathlib.Path(__file__).parent.parent.resolve()

TOOLS_JSON_PATH = REPO_ROOT_PATH / ".config" / "_tools.json"

with open(TOOLS_JSON_PATH) as f:
    tools_json = json.load(f)


def versioned_package(package: str) -> str:
    return f"{package}.{tools_json[package]}"


PROJECTION_PATH = REPO_ROOT_PATH / "projection"
CPPWINRT_EXE = (
    REPO_ROOT_PATH
    / "_tools"
    / versioned_package("Microsoft.Windows.CppWinRT")
    / "bin"
    / "cppwinrt.exe"
)

if not CPPWINRT_EXE.exists():
    raise RuntimeError("cppwinrt.exe not found. Please run `./scripts/fetch-tools.cmd`")


def use_parent_namespace_declarations(package_path: pathlib.Path) -> None:
    """Replaces the parent namespace include of each generated header.

    C++/WinRT makes every ``winrt/A.B.C.h`` include the *full*
    ``winrt/A.B.h``, even though it separately includes the declaration-only
    ``winrt/impl/*.2.h`` of everything else it references. The full parent
    header is only needed to *call* a parent type's methods, which the
    generated PyWinRT code does from its own translation unit, where the
    generator includes the parent header explicitly.

    Pointing these at ``winrt/impl/A.B.2.h`` instead saves parsing tens of
    megabytes of header text across the projection, most of it in the XAML
    namespaces, where the parent is the multi-megabyte
    ``Windows.UI.Xaml.Controls.h`` or ``Windows.UI.Xaml.h``.
    """
    for header in sorted((package_path / "winrt").glob("*.h")):
        namespace = header.stem

        if "." not in namespace:
            continue

        parent = namespace.rsplit(".", 1)[0]
        full_include = f'#include "winrt/{parent}.h"\n'
        text = header.read_text(encoding="utf-8")

        if full_include not in text:
            continue

        text = text.replace(full_include, f'#include "winrt/impl/{parent}.2.h"\n', 1)

        with open(header, "w", encoding="utf-8", newline="\n") as f:
            f.write(text)


# generate headers for windows sdk

WINDOWS_SDK = (
    REPO_ROOT_PATH
    / "_tools"
    / versioned_package("Microsoft.Windows.SDK.CPP")
    / "c"
    / "References"
    / "10.0.26100.0"
)
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

use_parent_namespace_declarations(SDK_PACKAGE_PATH)

# generate headers for WebView2

WEBVIEW2_PACKAGE_METADATA = (
    REPO_ROOT_PATH
    / "_tools"
    / versioned_package("Microsoft.Web.WebView2")
    / "lib"
    / "Microsoft.Web.WebView2.Core.winmd"
)

WEBVIEW2_PACKAGE_PATH = (
    PROJECTION_PATH
    / "webview2"
    / "webview2-Microsoft.Web.WebView2.Core"
    / "webview2"
    / "cppwinrt"
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

use_parent_namespace_declarations(WEBVIEW2_PACKAGE_PATH)

# generate headers for Microsoft.UI.Xaml (winui2)

MICROSOFT_UI_XAML_PACKAGE_METADATA = (
    REPO_ROOT_PATH
    / "_tools"
    / versioned_package("Microsoft.UI.Xaml")
    / "lib"
    / "uap10.0"
)
MICROSOFT_UI_XAML_PACKAGE_PATH = (
    PROJECTION_PATH
    / "winrt-Microsoft.UI.Xaml"
    / "src"
    / "winrt_microsoft_ui_xaml"
    / "cppwinrt"
)

shutil.rmtree(MICROSOFT_UI_XAML_PACKAGE_PATH, ignore_errors=True)
subprocess.check_call(
    [
        CPPWINRT_EXE,
        "-input",
        MICROSOFT_UI_XAML_PACKAGE_METADATA,
        "-reference",
        WEBVIEW2_PACKAGE_METADATA,
        "-reference",
        WINDOWS_SDK,
        "-output",
        MICROSOFT_UI_XAML_PACKAGE_PATH,
    ]
)

use_parent_namespace_declarations(MICROSOFT_UI_XAML_PACKAGE_PATH)


# generate headers for windows app sdk (winui3)

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

use_parent_namespace_declarations(WINDOWS_APP_SDK_PACKAGE_PATH)


# generate headers for test component

TEST_PACKAGE_METADATA = (
    REPO_ROOT_PATH
    / "_tools"
    / versioned_package("PyWinRT.TestWinRT")
    / "lib"
    / "uap10.0"
    / "TestComponent.winmd"
)
TEST_PACKAGE_PATH = (
    PROJECTION_PATH
    / "test-winrt"
    / "test-winrt-TestComponent"
    / "test_winrt"
    / "cppwinrt"
)

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

use_parent_namespace_declarations(TEST_PACKAGE_PATH)
