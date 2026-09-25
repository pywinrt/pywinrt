"""Builds the wheels of every package that is published.

There are two halves, built by different tools for different reasons, and
with no arguments both of them are built:

    python scripts/build-bdist.py
    python scripts/build-bdist.py --pure
    python scripts/build-bdist.py --compiled --only cp314-win_amd64

A projection package is data and winrt-table-compiler is pure Python, so each
of them is one py3-none-any wheel built once - that is the pure half. It needs
winrt-table-compiler *installed* rather than merely importable, because
hatchling finds the hook that compiles the tables through the entry point in
its metadata. The packages that redistribute a component .dll are in that half
too, since nothing in them is compiled either; what is in them is built for
one architecture, so each is three py3-none-win_* wheels rather than one
py3-none-any wheel. Nothing in the half needs a compiler.

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
REDIST_PATH = (PROJECT_DIR / "redist").resolve()

# The unpacked NuGet packages that a build reads something out of, named by
# the environment variable the build looks for. The Windows App SDK is
# redistributed with an app rather than part of Windows, so the two interop
# modules that call it take its headers and its import libraries from these;
# it is a metapackage over components published separately, so each module
# names the one it needs. WebView2 is where the component .dll comes from,
# and pointing at the unpacked copy is what keeps this build off the network:
# a build hook that finds no copy downloads the package itself.
NUGET_PACKAGES = {
    "WASDK_FOUNDATION_PATH": "Microsoft.WindowsAppSDK.Foundation",
    "WASDK_INTERACTIVE_EXPERIENCES_PATH": "Microsoft.WindowsAppSDK.InteractiveExperiences",
    "WASDK_RUNTIME_PATH": "Microsoft.WindowsAppSDK.Runtime",
    "WEBVIEW2_PATH": "Microsoft.Web.WebView2",
}

# The architectures a .dll package is built for, spelled the way
# sysconfig.get_platform() does.
PLAT_NAMES = ["win32", "win-amd64", "win-arm64"]

for env, package in NUGET_PACKAGES.items():
    os.environ[env] = os.fspath(
        (PROJECT_DIR / "_tools" / versioned_package(package)).resolve()
    )

parser = argparse.ArgumentParser(
    description="Builds the wheels of every package that is published."
)
half = parser.add_mutually_exclusive_group()
half.add_argument(
    "--pure",
    action="store_true",
    help="build only the wheels that are built without a compiler",
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
        (PROJECTION_PATH / "wasdk").glob("winrt-*"),
        (PROJECTION_PATH / "webview2").glob("winrt-*"),
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

    for package_path in REDIST_PATH.glob("winrt-*"):
        for plat_name in PLAT_NAMES:
            os.environ["PYWINRT_PLAT_NAME"] = plat_name

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

        del os.environ["PYWINRT_PLAT_NAME"]

if not args.pure:
    for package_path in chain(
        [PROJECT_DIR / "runtime"],
        (PROJECT_DIR / "interop").glob("winrt-*"),
    ):
        subprocess.check_call(
            ["cibuildwheel", os.fspath(package_path)]
            + (cibuildwheel_args or ["--platform", "windows"])
        )

        # Build directories are 10s of MBs and will cause CI to run out of space!
        shutil.rmtree(package_path / "build")
