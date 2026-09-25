"""Generates the C++/WinRT headers that the compiled parts of PyWinRT include.

A projection is a table that ``winrt-runtime`` interprets, so the only things
here that are compiled against C++/WinRT are the runtime itself and the eight
interop modules. Between them they include eleven namespace headers, and
``cppwinrt.exe`` emits one header per namespace in its input whatever it is
asked for, so the headers are generated in full and then the transitive closure
of those eleven is copied out. That is about 110 of the 1400 files and 6 MB of
the 116 MB the three header packages used to carry.

Each package carries the headers it includes, rather than one package carrying
them for everybody: ``winrt-runtime`` carries what ``pywinrt/base.h`` includes,
which is what every consumer of the PyWinRT headers gets as well, and each
interop package carries the closure of its own namespace on top of that. So an
interop module that starts including another namespace is a release of that
package and not of the runtime, and the runtime knows about the Windows SDK
only - the Windows App SDK headers belong to the one interop package that
includes them.

The headers are committed, like the rest of the generated tree, so this has to
be run when the NuGet packages that ``scripts/fetch-tools.ps1`` downloads move
or when a module starts including a namespace that is not listed below.
"""

import json
import pathlib
import re
import shutil
import subprocess
import tempfile
from collections import deque
from collections.abc import Iterable

import app_sdk

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

# The runtime's own directory, which is the one
# winrt._include.get_cppwinrt_include() names. An interop package's is
# "cppwinrt" beside its setup.py.
RUNTIME_OUTPUT_PATH = (
    REPO_ROOT_PATH / "runtime" / "python" / "winrt" / "include" / "cppwinrt"
)
INTEROP_PATH = REPO_ROOT_PATH / "interop"
INTEROP_OUTPUT_DIR = "cppwinrt"

WINDOWS_SDK_METADATA = (
    TOOLS_PATH
    / versioned_package("Microsoft.Windows.SDK.CPP")
    / "c"
    / "References"
    / "10.0.28000.0"
)

WEBVIEW2_METADATA = (
    TOOLS_PATH
    / versioned_package("Microsoft.Web.WebView2")
    / "lib"
    / "Microsoft.Web.WebView2.Core.winmd"
)

# What pywinrt/base.h and the runtime's own sources include. Everything that
# includes the PyWinRT headers gets these, so no interop package carries them
# again. `winrt/base.h` comes along with them.
RUNTIME_NAMESPACES = [
    "Windows.Foundation",
    "Windows.Foundation.Collections",
    "Windows.Foundation.Metadata",
    "Windows.Storage.Streams",
]

# What each interop module includes beyond that. winrt-Microsoft.UI.Interop
# includes the Windows App SDK's own <winrt/Microsoft.UI.Interop.h>, which is
# hand-written and ships in the NuGet package; what it needs from here is the
# namespace header under it.
INTEROP_NAMESPACES = {
    "winrt-Windows.Graphics.Capture.Interop": ["Windows.Graphics.Capture"],
    "winrt-Windows.Graphics.DirectX.Direct3D11.Interop": [
        "Windows.Graphics.DirectX.Direct3D11"
    ],
    "winrt-Windows.Media.Interop": ["Windows.Media"],
    "winrt-Windows.System.Interop": ["Windows.System"],
    "winrt-Windows.UI.Composition.Interop": [
        "Windows.UI.Composition",
        "Windows.UI.Composition.Desktop",
    ],
    "winrt-Windows.UI.Xaml.Hosting.Interop": [],
    "winrt-Microsoft.UI.Interop": ["Microsoft.UI"],
    "winrt-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap": [],
}

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
    names: Iterable[str],
    search_paths: Iterable[pathlib.Path],
    output_path: pathlib.Path,
) -> int:
    """Copies the headers of @p names into @p output_path.

    A header keeps the path it has in the tree it was found in, so the two
    trees flatten into one include directory. Returns how many were copied.
    """
    shutil.rmtree(output_path, ignore_errors=True)

    for name in sorted(names):
        for search_path in search_paths:
            header = search_path / name

            if header.exists():
                break
        else:
            raise RuntimeError(f"{name} is not in any of {list(search_paths)}")

        destination = output_path / name
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(header, destination)

    return len(set(names))


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
    # same version of cppwinrt.exe and used together. Every component is an
    # input, because a header of the one namespace taken from here may name a
    # type from any of them.
    subprocess.check_call(
        [
            CPPWINRT_EXE,
        ]
        + [arg for path in app_sdk.metadata_paths() for arg in ("-input", path)]
        + [
            "-reference",
            WEBVIEW2_METADATA,
            "-reference",
            WINDOWS_SDK_METADATA,
            "-output",
            windows_app_sdk_path,
        ]
    )

    search_paths = [windows_app_sdk_path, windows_sdk_path]

    runtime_closure = header_closure(RUNTIME_NAMESPACES, search_paths)
    count = copy_headers(runtime_closure, search_paths, RUNTIME_OUTPUT_PATH)
    print(f"{count} headers -> {RUNTIME_OUTPUT_PATH}")

    for package, namespaces in INTEROP_NAMESPACES.items():
        output_path = INTEROP_PATH / package / INTEROP_OUTPUT_DIR

        # what the package includes that the runtime does not already carry
        closure = header_closure(namespaces, search_paths) - runtime_closure

        count = copy_headers(closure, search_paths, output_path)
        print(f"{count} headers -> {output_path}")
