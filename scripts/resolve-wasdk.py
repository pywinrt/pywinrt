"""
Resolves the current stable Windows App SDK and writes it into _tools.json.

The App SDK is an empty metapackage over a set of component packages, each
with a version of its own, and the components are what carry the winmd files
this projection is generated from. Which components belong to a release is
therefore something to read from the release rather than to type: this asks
NuGet for the newest stable Microsoft.WindowsAppSDK, takes the set from its
nuspec, and checks it against the list the Runtime component carries, which is
what Microsoft's own build refuses a mismatched reference against.

Run this to move the App SDK pin. Editing the component versions by hand would
be assembling a set Microsoft does not ship together.
"""

import json
import xml.etree.ElementTree as ET

from packaging.version import Version

import app_sdk
import nuget

META_PACKAGE = "Microsoft.WindowsAppSDK"

# Machine learning is reached through a component that is a shell:
# Microsoft.WindowsAppSDK.ML carries no winmd and depends on this, which does.
ML_COMPONENT = f"{META_PACKAGE}.ML"
ML_PACKAGE = "Microsoft.Windows.AI.MachineLearning"

# Where the Runtime component keeps the exact component versions the release
# was built from.
COMPONENT_REFERENCE = "build/Microsoft.WindowsAppSDK.ComponentReference.targets"


def referenced_components(runtime_version: str) -> dict[str, str]:
    """
    The component versions the Runtime component says the release was built
    from, which is the list Microsoft's own targets file checks a project's
    references against.
    """
    targets = nuget.read_file(
        app_sdk.RUNTIME_COMPONENT, runtime_version, COMPONENT_REFERENCE
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


def main() -> None:
    meta_version = max(nuget.stable_versions(META_PACKAGE), key=Version)
    print(f"{META_PACKAGE} {meta_version}")

    components = nuget.dependencies(META_PACKAGE, meta_version)
    runtime_version = components[app_sdk.RUNTIME_COMPONENT]

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
        for package in app_sdk.COMPONENTS
        if package.startswith(META_PACKAGE)
    }

    # KeyError here means the shell component stopped carrying the real one
    resolved[ML_PACKAGE] = nuget.dependencies(ML_COMPONENT, components[ML_COMPONENT])[
        ML_PACKAGE
    ]

    resolved[app_sdk.RUNTIME_COMPONENT] = runtime_version
    resolved[META_PACKAGE] = meta_version

    with open(app_sdk.TOOLS_JSON_PATH) as f:
        tools = json.load(f)

    # every key that is not part of this family keeps whatever it had
    tools = {k: v for k, v in tools.items() if not k.startswith(META_PACKAGE)}
    tools.update(resolved)
    tools = dict(sorted(tools.items()))

    for package, version in tools.items():
        print(f"    {package} {version}")

    app_sdk.TOOLS_JSON_PATH.write_bytes((json.dumps(tools, indent=4) + "\n").encode())


if __name__ == "__main__":
    main()
