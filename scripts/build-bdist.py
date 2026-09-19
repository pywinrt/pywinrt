"""Builds the wheels of every package that is published.

There are two halves, built by different tools for different reasons, and
with no arguments both of them are built:

    python scripts/build-bdist.py
    python scripts/build-bdist.py --pure
    python scripts/build-bdist.py --compiled --only cp314-win_amd64

A projection package is data and winrt-table is pure Python, so each of them
is one py3-none-any wheel built once - that is the pure half. It needs
winrt-table *installed* rather than merely importable, because hatchling finds
the hook that compiles the tables through the entry point in its metadata.
Nothing in that half compiles, so nothing in it needs a compiler.

winrt-runtime and the eight hand-written interop modules do compile, so they
are what is left of the 15-build cibuildwheel matrix. Every argument other
than the two below is passed to cibuildwheel.
"""

import argparse
import json
import os
import shutil
import subprocess
from itertools import chain
from pathlib import Path

PROJECT_DIR = Path(__file__).parent.parent

TOOLS_JSON_PATH = PROJECT_DIR / ".config" / "_tools.json"

with open(TOOLS_JSON_PATH) as f:
    tools_json = json.load(f)


def versioned_package(package: str) -> str:
    return f"{package}.{tools_json[package]}"


PROJECTION_PATH = (PROJECT_DIR / "projection").resolve()
WINDOWS_APP_SDK_PATH = (
    PROJECT_DIR / "_tools" / versioned_package("Microsoft.WindowsAppSDK")
).resolve()

parser = argparse.ArgumentParser(
    description="Builds the wheels of every package that is published."
)
half = parser.add_mutually_exclusive_group()
half.add_argument(
    "--pure",
    action="store_true",
    help="build only the py3-none-any wheels",
)
half.add_argument(
    "--compiled",
    action="store_true",
    help="build only the wheels that cibuildwheel builds",
)
args, cibuildwheel_args = parser.parse_known_args()

if not args.compiled:
    for package_path in chain(
        [PROJECT_DIR / "table"],
        (PROJECTION_PATH / "winrt").glob("winrt-*"),
        (PROJECTION_PATH / "winui2").glob("winui2-*"),
        (PROJECTION_PATH / "winui3").glob("winui3-*"),
        (PROJECTION_PATH / "webview2").glob("webview2-*"),
    ):
        subprocess.check_call(
            [
                "pyproject-build",
                "--wheel",
                "--no-isolation",
                "--skip-dependency-check",
                "--outdir",
                "wheelhouse",
                os.fspath(package_path),
            ],
        )

if not args.pure:
    # the Windows App SDK is redistributed with an app rather than part of
    # Windows, so the two interop modules that call it take its headers and
    # its import libraries from the NuGet package
    os.environ["WINDOWS_APP_SDK_PATH"] = os.fspath(WINDOWS_APP_SDK_PATH)

    for package_path in chain(
        [PROJECT_DIR / "runtime"],
        (PROJECT_DIR / "interop").glob("winrt-*"),
        (PROJECT_DIR / "interop").glob("winui3-*"),
    ):
        subprocess.check_call(
            ["cibuildwheel", os.fspath(package_path)]
            + (cibuildwheel_args or ["--platform", "windows"])
        )

        # Build directories are 10s of MBs and will cause CI to run out of space!
        shutil.rmtree(package_path / "build")
