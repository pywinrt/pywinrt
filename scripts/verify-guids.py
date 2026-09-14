"""Checks the precomputed parameterized interface GUIDs.

The code generator writes the GUIDs of parameterized interfaces and delegates
(``IAsyncOperation<StorageFile>`` and friends) into ``py.<package>.guids.h`` so
that C++/WinRT does not have to compute each of them with a constexpr SHA-1 in
every translation unit that uses them.

A wrong value there does not fail to compile: it makes ``QueryInterface`` ask
for an interface that does not exist, or worse, the wrong one. So this compiles
each generated GUID header once with ``PYWINRT_VERIFY_GUIDS`` defined, which
turns every GUID into a ``static_assert`` against the value C++/WinRT computes
itself. Nothing is linked or run, and the check is slow (a few minutes) because
it does all of the constexpr hashing that the generated headers avoid.

Run this after regenerating the projection with a new Windows SDK, Windows App
SDK or WinUI 2 version.
"""

import pathlib
import subprocess
import sys
import tempfile

REPO_ROOT_PATH = pathlib.Path(__file__).parent.parent.resolve()
PROJECTION_PATH = REPO_ROOT_PATH / "projection"

SDK_CPPWINRT_PATH = PROJECTION_PATH / "winrt-sdk" / "src" / "winrt_sdk" / "cppwinrt"
WEBVIEW2_PATH = (
    PROJECTION_PATH / "webview2" / "webview2-Microsoft.Web.WebView2.Core" / "webview2"
)

# package name -> (directory holding py.<package>.guids.h, extra include dirs)
PACKAGES: dict[str, tuple[pathlib.Path, list[pathlib.Path]]] = {
    "winrt": (PROJECTION_PATH / "winrt-sdk" / "src" / "winrt_sdk" / "pywinrt", []),
    "winui2": (
        PROJECTION_PATH
        / "winrt-Microsoft.UI.Xaml"
        / "src"
        / "winrt_microsoft_ui_xaml"
        / "pywinrt",
        [
            PROJECTION_PATH
            / "winrt-Microsoft.UI.Xaml"
            / "src"
            / "winrt_microsoft_ui_xaml"
            / "cppwinrt",
            WEBVIEW2_PATH / "cppwinrt",
        ],
    ),
    "winui3": (
        PROJECTION_PATH
        / "winrt-WindowsAppSDK"
        / "src"
        / "winrt_windows_app_sdk"
        / "pywinrt",
        [
            PROJECTION_PATH
            / "winrt-WindowsAppSDK"
            / "src"
            / "winrt_windows_app_sdk"
            / "cppwinrt",
            WEBVIEW2_PATH / "cppwinrt",
        ],
    ),
    "webview2": (WEBVIEW2_PATH / "pywinrt", [WEBVIEW2_PATH / "cppwinrt"]),
    "test_winrt": (
        PROJECTION_PATH
        / "test-winrt"
        / "test-winrt-TestComponent"
        / "test_winrt"
        / "pywinrt",
        [
            PROJECTION_PATH
            / "test-winrt"
            / "test-winrt-TestComponent"
            / "test_winrt"
            / "cppwinrt"
        ],
    ),
}


def verify(
    package: str, header_dir: pathlib.Path, includes: list[pathlib.Path]
) -> bool:
    header = header_dir / f"py.{package}.guids.h"

    if not header.exists():
        raise RuntimeError(
            f"{header} not found."
            " Please run `py .\\scripts\\generate-pywinrt.py` first."
        )

    count = header.read_text(encoding="utf-8").count("inline constexpr guid guid_v<")

    with tempfile.TemporaryDirectory() as temp_dir:
        source = pathlib.Path(temp_dir) / f"verify_{package}.cpp"
        source.write_text(
            "#define PYWINRT_VERIFY_GUIDS\n"
            f'#include "py.{package}.guids.h"\n',
            encoding="utf-8",
        )

        args = [
            "cl.exe",
            "-nologo",
            "-c",
            "-std:c++20",
            "-EHsc",
            "-permissive-",
            "-bigobj",
            f"-Fo{temp_dir}\\",
            f"-I{header_dir}",
        ]
        args += [f"-I{path}" for path in [*includes, SDK_CPPWINRT_PATH]]
        args.append(str(source))

        print(f"{package}: checking {count} GUIDs...", flush=True)
        result = subprocess.run(args, cwd=temp_dir, check=False)

    return result.returncode == 0


if __name__ == "__main__":
    requested = sys.argv[1:] or list(PACKAGES)
    failed = []

    for package in requested:
        if package not in PACKAGES:
            raise SystemExit(
                f"unknown package: {package}"
                f" (expected one of {', '.join(PACKAGES)})"
            )

        if not verify(package, *PACKAGES[package]):
            failed.append(package)

    if failed:
        raise SystemExit(f"GUID mismatch in: {', '.join(failed)}")

    print("All GUIDs match C++/WinRT.")
