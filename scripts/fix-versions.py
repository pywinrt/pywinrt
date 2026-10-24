"""Gives every package that changed after its family was released a fix.

    python scripts/fix-versions.py
    python scripts/fix-versions.py --family winrt

A family of generated packages is released at the version of its upstream,
and a package of it that changes before the pin next moves - because the
generator now writes it differently - is released again on its own with a
number on that version, such as 4!10.0.28000.2705.1. This compares each
package with the newest release of its family, writes the packages that
differ into the family's fixes.json at the next number, and regenerates the
packaging, so the fix is ready to be committed with its changelog and
released with the tag scripts/release-tags.py prints. A family that has not
been released at its pin is left alone: its changes go out with that release.
"""

import argparse
import json
import subprocess
import sys

# the version each release unit is published with, and the tag that says so
import versions

parser = argparse.ArgumentParser(
    description="Gives every package that changed after its family was released a fix."
)
parser.add_argument(
    "--family",
    metavar="NAME",
    help="only this family",
)
args = parser.parse_args()

families = [
    family
    for family in versions.NUGET_PACKAGES
    if family not in versions.UNPUBLISHED_FAMILIES
]

if args.family:
    if args.family not in families:
        raise SystemExit(f"{args.family} is not a family that this tree publishes")

    families = [args.family]


def tag_exists(tag: str) -> bool:
    return (
        subprocess.run(
            ["git", "rev-parse", "--quiet", "--verify", f"refs/tags/{tag}"],
            cwd=versions.REPO_PATH,
            capture_output=True,
        ).returncode
        == 0
    )


for family in families:
    family_version = versions.family_versions()[family]
    released = [
        version
        for version in {family_version, *versions.family_fixes(family).values()}
        if tag_exists(versions.release_tag(family, version))
    ]

    if family_version not in released:
        print(f"{family}: not released at {family_version}, so nothing to fix")
        continue

    newest = max(
        released, key=lambda version: versions.fix_number(version, family_version)
    )
    tag = versions.release_tag(family, newest)
    family_path = versions.PROJECTION_PATH / family
    # deps.json is what generate-pyproject.py writes the packaging from, and
    # is in neither the sdist nor the wheel, so a change to it alone changes
    # nothing that is published
    diff = subprocess.run(
        [
            "git",
            "diff",
            "--name-only",
            tag,
            "--",
            family_path.as_posix(),
            f":(exclude){family_path.as_posix()}/*/deps.json",
        ],
        cwd=versions.REPO_PATH,
        capture_output=True,
        text=True,
        check=True,
    )
    changed = {
        parts[2]
        for parts in (path.split("/") for path in diff.stdout.splitlines())
        if len(parts) > 3 and (family_path / parts[2] / "deps.json").exists()
    }
    fixes = versions.next_fixes(family, released, changed)
    fixes_path = family_path / versions.FIXES_NAME

    if fixes:
        fixes_path.write_bytes((json.dumps(fixes, indent=4) + "\n").encode())
    else:
        fixes_path.unlink(missing_ok=True)

    if changed:
        fix = f"{family_version}.{versions.fix_number(newest, family_version) + 1}"
        packages = "package" if len(changed) == 1 else "packages"
        print(f"{family}: {len(changed)} {packages} changed since {tag}, now {fix}")
    else:
        print(f"{family}: nothing changed since {tag}")

subprocess.run(
    [sys.executable, versions.REPO_PATH / "scripts" / "generate-pyproject.py"],
    check=True,
)
