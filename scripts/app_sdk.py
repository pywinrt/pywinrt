"""
Where the Windows App SDK keeps the pieces that PyWinRT reads.

The App SDK is a metapackage over components that are published separately, so
a generator run reads one directory per component rather than one per release,
and the two hand-written interop modules compile against the components that
ship the headers they include. scripts/resolve-wasdk.py writes the resolved
component versions into .config/_tools.json and scripts/fetch-tools.ps1
installs them.

Running this file fetches the one piece that is not installed: see
fetch_version_header() for why the Runtime component is not.
"""

import json
from pathlib import Path

import nuget

REPO_PATH = Path(__file__).parent.parent

TOOLS_JSON_PATH = REPO_PATH / ".config" / "_tools.json"
TOOLS_PATH = REPO_PATH / "_tools"

# Only what is under metadata/ is projected: the AI component also ships the
# private winmd files of the framework package under runtimes-framework/, and
# InteractiveExperiences ships two sets of which 10.0.18362.0 is the superset.
# Base and DWrite have no metadata, and machine learning is reached through a
# shell component whose dependency is the package that has the winmd in it.
COMPONENTS = {
    "Microsoft.WindowsAppSDK.Foundation": "metadata",
    "Microsoft.WindowsAppSDK.InteractiveExperiences": "metadata/10.0.18362.0",
    "Microsoft.WindowsAppSDK.WinUI": "metadata",
    "Microsoft.WindowsAppSDK.Widgets": "metadata",
    "Microsoft.WindowsAppSDK.AI": "metadata",
    "Microsoft.WindowsAppSDK.Search": "metadata",
    "Microsoft.Windows.AI.MachineLearning": "metadata",
}

RUNTIME_COMPONENT = "Microsoft.WindowsAppSDK.Runtime"

# What the bootstrap interop module includes to learn which Windows App
# Runtime an App SDK release expects: WINDOWSAPPSDK_RELEASE_MAJOR and MINOR,
# and the version it hands to MddBootstrapInitialize.
VERSION_HEADER = "include/WindowsAppSDK-VersionInfo.h"


def component_version(package: str) -> str:
    with open(TOOLS_JSON_PATH) as f:
        tools = json.load(f)

    # KeyError here means resolve-wasdk.py did not write the component, which
    # is what fetch-tools.ps1 installs the family from
    return tools[package]


def component_path(package: str) -> Path:
    """
    Where scripts/fetch-tools.ps1 unpacked @p package.
    """
    return TOOLS_PATH / f"{package}.{component_version(package)}"


def metadata_inputs() -> list[tuple[str, Path]]:
    """
    The distribution each component's namespaces are published in, and the
    winmd directory they are read from.

    A component is one NuGet package and one release of it, so it is also what
    a namespace from it is published in: winrt-Microsoft.WindowsAppSDK.WinUI
    carries every namespace the WinUI component owns. Naming the distribution
    after the package it came from is what makes the two line up.
    """
    return [
        (package, component_path(package) / metadata)
        for package, metadata in COMPONENTS.items()
    ]


def metadata_paths() -> list[Path]:
    """
    The winmd directory of every component that has one, in the order
    COMPONENTS lists them.
    """
    return [path for _, path in metadata_inputs()]


def fetch_version_header() -> Path:
    """
    Puts WindowsAppSDK-VersionInfo.h where an installed Runtime component
    would have it.

    Nothing else in the Runtime component is used here and it is 161 MB of
    MSIX, so it is not installed; the header is read out of the package over
    range requests instead, into the layout the rest of the build expects so
    that nothing downstream has to know the difference.
    """
    version = component_version(RUNTIME_COMPONENT)
    path = component_path(RUNTIME_COMPONENT) / VERSION_HEADER

    if path.exists():
        return path

    print(f"Reading {VERSION_HEADER} from {RUNTIME_COMPONENT} {version}")

    header = nuget.read_file(RUNTIME_COMPONENT, version, VERSION_HEADER)

    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(header)

    return path


if __name__ == "__main__":
    print(fetch_version_header())
