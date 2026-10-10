"""Checks that every wheel in wheelhouse/ carries what it says it carries.

    python scripts/check-wheel-contents.py
    python scripts/check-wheel-contents.py path/to/wheelhouse

A PyWinRT wheel redistributes a Microsoft binary only when the packaging says
it does, and twice in the past one turned up where nothing said so: #68 and
#86 were both a .dll copied into a wheel by a repair step that nobody had
asked for. Nothing copies a .dll into a wheel after the build any more, so
this is what keeps it that way.

The rule is that a wheel's PEP 770 bill of materials is the list of binaries
it redistributes, and it has to agree with what is in the wheel: a wheel with
no bill of materials carries no .dll, and a wheel with one carries exactly the
.dlls the bill names. That needs no list of package names here, so a new
package that redistributes something is covered the day it is built. The bill
also names the distribution itself as its root component, and that has to be
the version the wheel is, since a wheel on PyPI cannot be corrected after it
is uploaded.

No wheel carries a C or C++ header either: nothing compiles against an
installed PyWinRT package, and each package that compiles carries the headers
it includes in its source distribution rather than as package data.
"""

import argparse
import email
import json
import sys
import zipfile
from pathlib import Path

PROJECT_DIR = Path(__file__).parent.parent


def get_sbom_components(wheel: zipfile.ZipFile) -> set[str]:
    """
    The binaries that a wheel's bill of materials says it redistributes.

    The root component is the distribution itself rather than something
    redistributed, so it is not one of these.
    """
    components: set[str] = set()

    for name in wheel.namelist():
        if not name.endswith(".json"):
            continue

        if ".dist-info/sboms/" not in name:
            continue

        sbom = json.loads(wheel.read(name))

        for component in sbom["components"]:
            components.add(component["name"])

    return components


def get_sbom_versions(wheel: zipfile.ZipFile) -> set[str]:
    """
    The versions that a wheel's bills of materials give the distribution.
    """
    versions: set[str] = set()

    for name in wheel.namelist():
        if not name.endswith(".json"):
            continue

        if ".dist-info/sboms/" not in name:
            continue

        sbom = json.loads(wheel.read(name))
        versions.add(sbom["metadata"]["component"]["version"])

    return versions


def get_version(wheel: zipfile.ZipFile) -> str:
    """
    The version that a wheel's core metadata says it is.
    """
    for name in wheel.namelist():
        if name.endswith(".dist-info/METADATA"):
            return email.message_from_bytes(wheel.read(name))["Version"]

    raise ValueError("the wheel has no METADATA")


def get_binaries(wheel: zipfile.ZipFile) -> set[str]:
    """
    The .dlls in a wheel.

    An extension module is a .pyd and is the wheel's own build output rather
    than something redistributed, so it is not one of these.
    """
    return {
        name.rpartition("/")[2]
        for name in wheel.namelist()
        if name.lower().endswith(".dll")
    }


def get_headers(wheel: zipfile.ZipFile) -> set[str]:
    """
    The C and C++ headers in a wheel.
    """
    return {name for name in wheel.namelist() if name.lower().endswith(".h")}


def get_license_files(wheel: zipfile.ZipFile) -> set[str]:
    """
    The license texts a wheel carries, which PEP 639 puts in the .dist-info.
    """
    return {
        name.rpartition("/")[2]
        for name in wheel.namelist()
        if ".dist-info/licenses/" in name
    }


def check_wheel(path: Path) -> list[str]:
    """
    Everything wrong with one wheel, as a list of lines to print.
    """
    problems: list[str] = []

    with zipfile.ZipFile(path) as wheel:
        binaries = get_binaries(wheel)
        components = get_sbom_components(wheel)
        sbom_versions = get_sbom_versions(wheel)
        version = get_version(wheel)
        headers = get_headers(wheel)
        license_files = get_license_files(wheel)

    for name in sorted(binaries - components):
        problems.append(
            f"redistributes {name}, which its bill of materials does not name"
        )

    for name in sorted(components - binaries):
        problems.append(f"names {name} in its bill of materials but does not carry it")

    for sbom_version in sorted(sbom_versions - {version}):
        problems.append(
            f"is {version} but its bill of materials says it is {sbom_version}"
        )

    for name in sorted(headers):
        problems.append(f"carries the header {name}")

    # PyWinRT's own license is in every wheel, and a wheel that redistributes
    # something carries that license as well.
    if not license_files:
        problems.append("carries no license text")
    elif components and len(license_files) < 2:
        problems.append("redistributes a binary but carries only one license text")

    return problems


def main(wheelhouse: Path) -> int:
    wheels = sorted(wheelhouse.glob("*.whl"))

    if not wheels:
        print(f"no wheels in {wheelhouse}", file=sys.stderr)
        return 1

    failed = 0

    for wheel in wheels:
        problems = check_wheel(wheel)

        for problem in problems:
            print(f"{wheel.name} {problem}", file=sys.stderr)

        failed += bool(problems)

    print(f"checked {len(wheels)} wheels, {failed} of which are wrong")

    return 1 if failed else 0


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Checks that every wheel in a wheelhouse carries what it says"
        " it carries."
    )
    parser.add_argument(
        "wheelhouse",
        nargs="?",
        type=Path,
        default=PROJECT_DIR / "wheelhouse",
        help="the directory of wheels to check (default: wheelhouse/)",
    )
    args = parser.parse_args()

    sys.exit(main(args.wheelhouse))
