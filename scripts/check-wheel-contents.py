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
package that redistributes something is covered the day it is built.
"""

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
    components = set()

    for name in wheel.namelist():
        if not name.endswith(".json"):
            continue

        if ".dist-info/sboms/" not in name:
            continue

        sbom = json.loads(wheel.read(name))

        for component in sbom["components"]:
            components.add(component["name"])

    return components


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
    problems = []

    with zipfile.ZipFile(path) as wheel:
        binaries = get_binaries(wheel)
        components = get_sbom_components(wheel)
        license_files = get_license_files(wheel)

    for name in sorted(binaries - components):
        problems.append(
            f"redistributes {name}, which its bill of materials does not name"
        )

    for name in sorted(components - binaries):
        problems.append(f"names {name} in its bill of materials but does not carry it")

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
    argument = sys.argv[1] if len(sys.argv) > 1 else PROJECT_DIR / "wheelhouse"

    sys.exit(main(Path(argument)))
