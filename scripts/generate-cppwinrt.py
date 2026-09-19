"""Generates the C++/WinRT headers that the compiled parts of PyWinRT include.

A projection is a table that ``winrt-runtime`` interprets, so the only things
here that are compiled against C++/WinRT are the runtime itself and the eight
interop modules. Between them they include eleven namespace headers, and
``cppwinrt.exe`` emits one header per namespace in its input whatever it is
asked for, so the headers are generated in full and then the transitive closure
of those eleven is copied out. That is about 110 of the 1400 files and 6 MB of
the 116 MB the three header packages used to carry.

The output is build output: it is regenerated from the NuGet packages that
``scripts/fetch-tools.ps1`` downloads, it is not committed, and nothing
publishes it.
"""

import json
import pathlib
import re
import shutil
import subprocess
import tempfile
from collections import deque
from collections.abc import Iterable

REPO_ROOT_PATH = pathlib.Path(__file__).parent.parent.resolve()

TOOLS_JSON_PATH = REPO_ROOT_PATH / ".config" / "_tools.json"

with open(TOOLS_JSON_PATH) as f:
    tools_json = json.load(f)


def versioned_package(package: str) -> str:
    return f"{package}.{tools_json[package]}"


TOOLS_PATH = REPO_ROOT_PATH / "_tools"

CPPWINRT_EXE = (
    TOOLS_PATH
    / versioned_package("Microsoft.Windows.CppWinRT")
    / "bin"
    / "cppwinrt.exe"
)

if not CPPWINRT_EXE.exists():
    raise RuntimeError("cppwinrt.exe not found. Please run `./scripts/fetch-tools.ps1`")

# Where the headers are written. Both trees are include directories in their own
# right and the Windows App SDK one references the Windows SDK one, so a module
# that needs Microsoft.* puts both on its include path, in this order.
CPPWINRT_PATH = REPO_ROOT_PATH / "_cppwinrt"
WINDOWS_SDK_OUTPUT_PATH = CPPWINRT_PATH / "windows-sdk"
WINDOWS_APP_SDK_OUTPUT_PATH = CPPWINRT_PATH / "windows-app-sdk"

WINDOWS_SDK_METADATA = (
    TOOLS_PATH
    / versioned_package("Microsoft.Windows.SDK.CPP")
    / "c"
    / "References"
    / "10.0.26100.0"
)

WINDOWS_APP_SDK_PATH = TOOLS_PATH / versioned_package("Microsoft.WindowsAppSDK")
WEBVIEW2_METADATA = (
    TOOLS_PATH
    / versioned_package("Microsoft.Web.WebView2")
    / "lib"
    / "Microsoft.Web.WebView2.Core.winmd"
)

# The namespaces the runtime and the interop modules include, which is what
# decides how much of each projection is kept. `winrt/base.h` is always kept.
WINDOWS_SDK_NAMESPACES = [
    "Windows.Foundation",
    "Windows.Foundation.Collections",
    "Windows.Foundation.Metadata",
    "Windows.Graphics.Capture",
    "Windows.Graphics.DirectX.Direct3D11",
    "Windows.Media",
    "Windows.Storage.Streams",
    "Windows.System",
    "Windows.UI.Composition",
    "Windows.UI.Composition.Desktop",
]

# winui3-Microsoft.UI.Interop includes the Windows App SDK's own
# <winrt/Microsoft.UI.Interop.h>, which is hand-written and ships in the NuGet
# package; what it needs from here is the namespace header under it.
WINDOWS_APP_SDK_NAMESPACES = ["Microsoft.UI"]

INCLUDE_RE = re.compile(r'#include\s+["<](winrt/[^">]+)[">]')


def header_closure(
    namespaces: Iterable[str], search_paths: Iterable[pathlib.Path]
) -> set[str]:
    """Finds every header reachable from the headers of @p namespaces.

    Each path is returned relative to the include directory it was found in,
    e.g. ``winrt/impl/Windows.Foundation.2.h``. A header is looked up in each
    of @p search_paths in turn, so a tree that references another one is given
    both.
    """
    pending = deque(f"winrt/{namespace}.h" for namespace in namespaces)
    pending.append("winrt/base.h")
    found: set[str] = set()

    while pending:
        name = pending.popleft()

        if name in found:
            continue

        for search_path in search_paths:
            header = search_path / name

            if header.exists():
                break
        else:
            raise RuntimeError(f"{name} is not in any of {list(search_paths)}")

        found.add(name)
        pending.extend(INCLUDE_RE.findall(header.read_text(encoding="utf-8")))

    return found


def copy_headers(
    names: Iterable[str], source_path: pathlib.Path, output_path: pathlib.Path
) -> int:
    """Copies the headers of @p names that are in @p source_path.

    A name that is not there belongs to one of the other trees, so it is
    skipped. Returns how many were copied.
    """
    count = 0

    for name in sorted(names):
        header = source_path / name

        if not header.exists():
            continue

        destination = output_path / name
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(header, destination)
        count += 1

    return count


shutil.rmtree(CPPWINRT_PATH, ignore_errors=True)

with tempfile.TemporaryDirectory(prefix="pywinrt-cppwinrt-") as temp_dir:
    windows_sdk_path = pathlib.Path(temp_dir) / "windows-sdk"
    windows_app_sdk_path = pathlib.Path(temp_dir) / "windows-app-sdk"

    subprocess.check_call(
        [CPPWINRT_EXE, "-input", WINDOWS_SDK_METADATA, "-output", windows_sdk_path]
    )

    # The reference toggles a Python object between strong and weak when WinRT
    # takes or drops the last reference to it, which C++/WinRT's own
    # implements<> has no hook for.
    subprocess.check_call(
        [
            "git",
            "apply",
            "--quiet",
            REPO_ROOT_PATH / "patches" / "cppwinrt-windows-sdk.diff",
        ],
        cwd=windows_sdk_path,
    )

    # The Windows App SDK projection has no base.h and no Windows.* headers of
    # its own: it includes the Windows SDK's, so the two are generated from the
    # same version of cppwinrt.exe and used together.
    subprocess.check_call(
        [
            CPPWINRT_EXE,
            "-input",
            WINDOWS_APP_SDK_PATH / "lib" / "uap10.0",
            "-input",
            WINDOWS_APP_SDK_PATH / "lib" / "uap10.0.18362",
            "-reference",
            WEBVIEW2_METADATA,
            "-reference",
            WINDOWS_SDK_METADATA,
            "-output",
            windows_app_sdk_path,
        ]
    )

    search_paths = [windows_app_sdk_path, windows_sdk_path]
    closure = header_closure(
        WINDOWS_SDK_NAMESPACES + WINDOWS_APP_SDK_NAMESPACES, search_paths
    )

    app_sdk_count = copy_headers(
        closure, windows_app_sdk_path, WINDOWS_APP_SDK_OUTPUT_PATH
    )
    sdk_count = copy_headers(closure, windows_sdk_path, WINDOWS_SDK_OUTPUT_PATH)

print(f"{sdk_count} Windows SDK headers -> {WINDOWS_SDK_OUTPUT_PATH}")
print(f"{app_sdk_count} Windows App SDK headers -> {WINDOWS_APP_SDK_OUTPUT_PATH}")
