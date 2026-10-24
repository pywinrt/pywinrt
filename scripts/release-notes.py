"""Prints the GitHub release of one release tag: its title, or its notes.

    python scripts/release-notes.py wheels/runtime/v4.0.0
    python scripts/release-notes.py --title nuget/PyWinRT/v4.0.0

Each tag releases one unit, and each unit has a changelog of its own in its
directory, so the notes of a release are the section of that changelog that
the tag's version heads, and nothing is written twice. A reference link in the
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

# the version each release unit is published with, and the tag that says so
import versions

NUGET_TAG = re.compile(r"nuget/PyWinRT/v(?P<version>.+)")

# What a unit of several distributions is called. Every other unit is one
# distribution and is called by its name.
FAMILY_TITLES = {
    "winrt": "Windows SDK packages",
    "wasdk": "Windows App SDK packages",
    "webview2": "WebView2 packages",
}

DISTRIBUTIONS = {
    "runtime": "winrt-runtime",
    "table": "winrt-table-compiler",
    "winui2": "winui2-Microsoft.UI.Xaml",
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
    title = FAMILY_TITLES.get(unit) or DISTRIBUTIONS.get(unit) or unit

    if unit in (versions.RUNTIME_PATH.name, versions.TABLE_PATH.name):
        path = versions.REPO_PATH / unit
    elif (versions.INTEROP_PATH / unit).is_dir():
        path = versions.INTEROP_PATH / unit
    else:
        path = versions.REPO_PATH / "projection" / unit

    return title, version, path


def notes(changelog: Path, version: str) -> str:
    """
    The section of @p changelog that @p version heads, without its heading,
    and the definitions of the reference links in it.
    """
    text = changelog.read_text(encoding="utf-8")
    heading = re.compile(rf"^## \[{re.escape(version)}\] - .*$", re.MULTILINE)
    start = heading.search(text)

    if start is None:
        raise SystemExit(f"{changelog} has no section for {version}")

    end = re.compile(r"^## \[|^\[[^\]]+\]: ", re.MULTILINE).search(text, start.end())
    section = text[start.end() : end.start() if end else len(text)].strip()

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
