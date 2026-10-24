"""
Where the Windows App SDK keeps the pieces that PyWinRT reads.

The App SDK is a metapackage over components that are published separately, so
a generator run reads one directory per component rather than one per release,
and the two hand-written interop modules compile against the components that
ship the headers they include. Which components belong to a release is read
from the release (see resolve()), scripts/move-upstream-pins.py writes them
into .config/_tools.json, and scripts/fetch-tools.ps1 installs them.

Running this file fetches the one piece that is not installed: see
fetch_version_header() for why the Runtime component is not.
"""

import json
import xml.etree.ElementTree as ET
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

META_PACKAGE = "Microsoft.WindowsAppSDK"

RUNTIME_COMPONENT = "Microsoft.WindowsAppSDK.Runtime"

# Machine learning is reached through a component that is a shell:
# Microsoft.WindowsAppSDK.ML carries no winmd and depends on this, which does.
ML_COMPONENT = f"{META_PACKAGE}.ML"
ML_PACKAGE = "Microsoft.Windows.AI.MachineLearning"

# Where the Runtime component keeps the exact component versions the release
# was built from.
COMPONENT_REFERENCE = "build/Microsoft.WindowsAppSDK.ComponentReference.targets"

# Where an App SDK release says which Windows App Runtime it expects:
# WINDOWSAPPSDK_RELEASE_MAJOR and MINOR, and the version the bootstrap interop
# module hands to MddBootstrapInitialize. scripts/generate-pyproject.py copies
# those into the module's version_info.h.
VERSION_HEADER = "include/WindowsAppSDK-VersionInfo.h"


def component_version(package: str) -> str:
    with open(TOOLS_JSON_PATH) as f:
        tools = json.load(f)

    # KeyError here means move-upstream-pins.py did not write the component,
    # which is what fetch-tools.ps1 installs the family from
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


def referenced_components(runtime_version: str) -> dict[str, str]:
    """
    The component versions the Runtime component says the release was built
    from, which is the list Microsoft's own targets file checks a project's
    references against.
    """
    targets = nuget.read_file(
        RUNTIME_COMPONENT, runtime_version, COMPONENT_REFERENCE
    ).decode("utf-8-sig")

    components: dict[str, str] = {}

    for mismatch in ET.fromstring(targets).iter():
        if not mismatch.tag.endswith("}_VersionMismatch"):
            continue

        expected = next(
            (e for e in mismatch if e.tag.endswith("}ExpectedVersion")), None
        )

        if expected is not None and expected.text:
            components[mismatch.attrib["Include"]] = expected.text.strip()

    return components


def resolve(meta_version: str) -> dict[str, str]:
    """
    The version of every package of the App SDK release @p meta_version that
    the build reads something out of, the metapackage among them, keyed by
    package.

    The App SDK is an empty metapackage over a set of component packages, each
    with a version of its own, and the components are what carry the winmd
    files this projection is generated from. Which components belong to a
    release is therefore something to read from the release rather than to
    type: this takes the set from the metapackage's nuspec and checks it
    against the list the Runtime component carries, which is what Microsoft's
    own build refuses a mismatched reference against. Editing the component
    versions by hand would be assembling a set Microsoft does not ship
    together.
    """
    components = nuget.dependencies(META_PACKAGE, meta_version)
    runtime_version = components[RUNTIME_COMPONENT]

    # The metapackage names a floor for each component, and the Runtime
    # component names the version the release was actually built from. They
    # have agreed at every release so far; if they ever stop, the projection
    # would be generated from metadata Microsoft does not ship together.
    for package, version in sorted(referenced_components(runtime_version).items()):
        if components.get(package) != version:
            raise RuntimeError(
                f"{META_PACKAGE} {meta_version} asks for {package}"
                f" {components.get(package)} and its Runtime component was built"
                f" against {version}"
            )

    # Everything the build reads something out of. Base and DWrite carry
    # neither metadata nor headers, so they are checked above and then left
    # alone: a version nothing uses is only something else to go stale.
    resolved = {
        package: components[package]
        for package in COMPONENTS
        if package.startswith(META_PACKAGE)
    }

    # KeyError here means the shell component stopped carrying the real one
    resolved[ML_PACKAGE] = nuget.dependencies(ML_COMPONENT, components[ML_COMPONENT])[
        ML_PACKAGE
    ]

    resolved[RUNTIME_COMPONENT] = runtime_version
    resolved[META_PACKAGE] = meta_version

    return resolved


def fetch_version_header() -> Path:
    """
    Puts WindowsAppSDK-VersionInfo.h where an installed Runtime component
    would have it.

    Nothing else in the Runtime component is used here and it is 170 MB of
    MSIX, so it is not installed. The header is read out of the package over
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
