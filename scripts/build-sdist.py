"""Builds the source distribution of every package that is published.

A source distribution has to build on a machine that has nothing but PyPI, so
nothing here tells a build where to find something in this checkout: the
C++/WinRT headers that winrt-runtime and the interop modules compile against
ride inside the packages that include them, and a projection package's only
build step is compiling its tables, which the hatchling hook that
winrt-table-compiler ships does.

A package that redistributes a Microsoft binary carries no copy of it, since
nothing prebuilt belongs in a source distribution; its build fetches the NuGet
package instead, and an environment variable names an unpacked copy for a
machine that cannot reach nuget.org. The variables are set below so that
building here never goes to the network.
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


# The unpacked NuGet packages that a build reads something out of, named by
# the environment variable the build looks for. The Windows App SDK is
# redistributed with an app rather than part of Windows, so the two interop
# modules that call it take its headers and its import libraries from these;
# it is a metapackage over components published separately, so each module
# names the one it needs. WebView2 is where the component .dll comes from,
# and pointing at the unpacked copy is what keeps this build off the network.
NUGET_PACKAGES = {
    "WASDK_FOUNDATION_PATH": "Microsoft.WindowsAppSDK.Foundation",
    "WASDK_INTERACTIVE_EXPERIENCES_PATH": "Microsoft.WindowsAppSDK.InteractiveExperiences",
    "WASDK_RUNTIME_PATH": "Microsoft.WindowsAppSDK.Runtime",
    "WEBVIEW2_PATH": "Microsoft.Web.WebView2",
}

PROJECTION_PATH = (PROJECT_DIR / "projection").resolve()
REDIST_PATH = (PROJECT_DIR / "redist").resolve()

for env, package in NUGET_PACKAGES.items():
    os.environ[env] = os.fspath(
        (PROJECT_DIR / "_tools" / versioned_package(package)).resolve()
    )

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
    REDIST_PATH.glob("winrt-*"),
    (PROJECT_DIR / "interop").glob("winrt-*"),
    (PROJECTION_PATH / "winrt").glob("winrt-*"),
    (PROJECTION_PATH / "winui2").glob("winui2-*"),
    (PROJECTION_PATH / "wasdk").glob("winrt-*"),
    (PROJECTION_PATH / "webview2").glob("winrt-*"),
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
