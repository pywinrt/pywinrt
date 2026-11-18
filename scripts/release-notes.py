"""Prints the GitHub release of one release tag: its title, or its notes.

    python scripts/release-notes.py wheels/runtime/v4.0.0
    python scripts/release-notes.py --title nuget/PyWinRT/v4.0.0

Each tag releases one unit, and each unit has a changelog of its own in its
directory, so the notes of a release are the section of that changelog that
the tag's version heads, and nothing is written twice. The exception is a fix
to an interop module that is released with a family, whose section is in the
family's changelog. A reference link in the
section is given its definition from the end of the changelog, which a release
does not have.

A wheels/ tag is one that scripts/release-tags.py prints. The PyWinRT tool on
NuGet carries no version in the tree, so its tag, nuget/PyWinRT/v<version>, is
taken at its word.
"""

import argparse
import re
import sys
from pathlib import Path

# the sections of a changelog
import changelogs

# what each family is called, and which interop modules are released with one
import families

# the version each release unit is published with, and the tag that says so
import versions

NUGET_TAG = re.compile(r"nuget/PyWinRT/v(?P<version>.+)")

# What the unit of a package written by hand is called, where its directory
# is not named after it. A family is called by its title, and every other unit
# is one distribution and is called by its name.
DISTRIBUTIONS = {
    "runtime": "winrt-runtime",
    "table": "winrt-table-compiler",
}

REFERENCE = re.compile(r"\]\[([^\]]+)\]")
DEFINITION = re.compile(r"^\[([^\]]+)\]: ", re.MULTILINE)


def release_of(tag: str) -> tuple[str, str, Path]:
    """
    The title of what @p tag releases, its version, and its changelog.
    """
    nuget = NUGET_TAG.fullmatch(tag)

    if nuget:
        return "PyWinRT", nuget["version"], versions.REPO_PATH / "PyWinRT"

    try:
        unit = versions.unit_of_tag(tag)
    except ValueError as error:
        raise SystemExit(error) from error

    version = versions.published_versions()[unit]
    title = (
        families.FAMILIES[unit].title
        if unit in families.FAMILIES
        else DISTRIBUTIONS.get(unit, unit)
    )

    if unit in (versions.RUNTIME_PATH.name, versions.TABLE_PATH.name):
        path = versions.REPO_PATH / unit
    elif unit in families.FAMILY_RELEASED_INTEROP:
        # a fix to an interop module that is released with a family, which
        # shares the family's changelog
        path = versions.PROJECTION_PATH / families.FAMILY_RELEASED_INTEROP[unit]
    elif (versions.INTEROP_PATH / unit).is_dir():
        path = versions.INTEROP_PATH / unit
    else:
        path = versions.PROJECTION_PATH / unit

    return title, version, path


def notes(changelog: Path, version: str) -> str:
    """
    The section of @p changelog that @p version heads, without its heading,
    and the definitions of the reference links in it.
    """
    text = changelog.read_text(encoding="utf-8")
    heading = re.compile(rf"^## \[{re.escape(version)}\] - .*$", re.MULTILINE)
    span = changelogs.section(text, heading)

    if span is None:
        raise SystemExit(f"{changelog} has no section for {version}")

    begin, end = span
    section = text[begin:end].strip()

    definitions = {
        match[1]: line
        for match, line in (
            (DEFINITION.match(line), line) for line in text.splitlines()
        )
        if match
    }
    used = [
        definitions[name]
        for name in dict.fromkeys(REFERENCE.findall(section))
        if name in definitions
    ]

    return section + ("\n\n" + "\n".join(used) if used else "") + "\n"


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Prints the GitHub release of one release tag."
    )
    parser.add_argument("tag", help="the tag the release is for")
    parser.add_argument(
        "--title", action="store_true", help="print the title instead of the notes"
    )
    args = parser.parse_args()

    title, version, path = release_of(args.tag)

    if args.title:
        print(f"{title} {version}")
    else:
        sys.stdout.write(notes(path / "CHANGELOG.md", version))


if __name__ == "__main__":
    main()
