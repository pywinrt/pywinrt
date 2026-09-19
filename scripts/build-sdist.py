"""Builds the source distribution of every package that is published.

A source distribution has to build on a machine that has nothing but PyPI, so
nothing here tells a build where to find something in this checkout: the
C++/WinRT headers that winrt-runtime and the interop modules compile against
ride inside the packages that include them, and a projection package's only
build step is compiling its tables, which the hatchling hook that winrt-table
ships does.
"""

import json
import os
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

# the Windows App SDK is redistributed with an app rather than part of
# Windows, so the two interop modules that call it take its headers and its
# import libraries from the NuGet package
os.environ["WINDOWS_APP_SDK_PATH"] = os.fspath(WINDOWS_APP_SDK_PATH)

# setup.py imports winrt._include to locate the headers it compiles against,
# and the runtime is not installed here (its own sdist is one of the things we
# build), so point at its source tree instead
os.environ["PYTHONPATH"] = os.pathsep.join(
    filter(
        None,
        [
            os.fspath(PROJECT_DIR / "runtime" / "python"),
            os.environ.get("PYTHONPATH"),
        ],
    )
)


for package_path in chain(
    [PROJECT_DIR / "table", PROJECT_DIR / "runtime"],
    (PROJECT_DIR / "interop").glob("winrt-*"),
    (PROJECT_DIR / "interop").glob("winui3-*"),
    (PROJECTION_PATH / "winrt").glob("winrt-*"),
    (PROJECTION_PATH / "winui2").glob("winui2-*"),
    (PROJECTION_PATH / "winui3").glob("winui3-*"),
    (PROJECTION_PATH / "webview2").glob("webview2-*"),
):
    subprocess.check_call(
        [
            "pyproject-build",
            "--sdist",
            "--no-isolation",
            "--skip-dependency-check",
            "--outdir",
            "wheelhouse",
            os.fspath(package_path),
        ],
    )
