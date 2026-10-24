"""Brings the tree to the upstream releases its families are pinned at.

    python scripts/move-upstream-pins.py
    python scripts/move-upstream-pins.py --family webview2
    python scripts/move-upstream-pins.py --report report.md

A family of generated packages takes its version from one upstream NuGet
package, and .config/upstream/upstream.csproj is where its pin is moved: by
Dependabot when it proposes a new release, or by hand. Everything else reads
the copy of the pins in .config/_tools.json, so for each pin that differs from
the committed copy this writes the copy - for the Windows App SDK with the
components of that release - and deletes the fixes the family's last release
left behind, which a moved pin makes meaningless. It says so under
[Unreleased] in the family's changelog, with a link to upstream's notes on the
release. Then it fetches the packages and generates everything from them
again, and says in Markdown what the change would release, for the pull
request that carries it.
"""

import argparse
import json
import math
import re
import subprocess
import sys
import urllib.error
import urllib.request
from pathlib import Path

# where the App SDK says which components belong to a release
import app_sdk

# which units are published by trusted publishing
import packages

# the version each release unit is published with, and the tag that says so
import versions

SCRIPTS_PATH = versions.REPO_PATH / "scripts"

# How many projects PyPI let this account create in a day in 2026. It
# publishes no figure, and a release that adds more is refused partway
# through until the next day.
NEW_PROJECTS_PER_DAY = 4

# What a family's changelog calls its upstream, keyed the way
# versions.NUGET_PACKAGES is.
UPSTREAM_NAMES = {
    "winrt": "the Windows SDK",
    "winui2": "WinUI 2",
    "wasdk": "the Windows App SDK",
    "webview2": "WebView2",
}

# Where upstream says what a release changed. The App SDK and WinUI 2 publish
# a GitHub release for each, and the other two do not, so for them it is the
# release's page on NuGet, which points on to whatever notes there are.
UPSTREAM_NOTES = {
    "winrt": "https://www.nuget.org/packages/Microsoft.Windows.SDK.CPP/{version}",
    "winui2": "https://github.com/microsoft/microsoft-ui-xaml/releases/tag/v{version}",
    "wasdk": "https://github.com/microsoft/WindowsAppSDK/releases/tag/v{version}",
    "webview2": "https://www.nuget.org/packages/Microsoft.Web.WebView2/{version}",
}

# The heading of the section a changelog collects changes in before they are
# released, and what ends a section: the next heading, or the link
# definitions at the bottom of the file.
UNRELEASED = re.compile(r"^## \[Unreleased\]\n", re.MULTILINE)
SECTION_END = re.compile(r"^## \[|^\[[^\]]+\]: ", re.MULTILINE)


def committed_tools() -> dict[str, str]:
    """
    The NuGet package versions in .config/_tools.json as committed.

    A pin is moved relative to these rather than to the file in the working
    tree, so that a run that stopped after writing the copy - a fetch that
    failed, say - is finished by running this again.
    """
    tools = subprocess.run(
        ["git", "show", "HEAD:.config/_tools.json"],
        cwd=versions.REPO_PATH,
        capture_output=True,
        text=True,
        check=True,
    )

    return json.loads(tools.stdout)


def moved_pins(pins: dict[str, str], tools: dict[str, str]) -> dict[str, str]:
    """
    The families whose pin differs from what @p tools carries, and the version
    each is pinned at now.
    """
    return {
        family: version
        for family, version in pins.items()
        if tools[versions.NUGET_PACKAGES[family]] != version
    }


def tools_with_pins(tools: dict[str, str], moved: dict[str, str]) -> dict[str, str]:
    """
    @p tools with the moved pins in it, in the order _tools.json keeps.

    The App SDK is pinned by its metapackage, and its components belong to
    the release, so the whole set is replaced with the one the release names
    and a component the release no longer has goes with it.
    """
    tools = dict(tools)

    for family, version in moved.items():
        if family != "wasdk":
            tools[versions.NUGET_PACKAGES[family]] = version
            continue

        resolved = app_sdk.resolve(version)
        tools = {
            package: pin
            for package, pin in tools.items()
            if not package.startswith(app_sdk.META_PACKAGE) and package not in resolved
        } | resolved

    return dict(sorted(tools.items()))


def stale_fixes(family: str) -> list[Path]:
    """
    The files that give a package of @p family the version of a fix, which
    scripts/generate-pyproject.py refuses once the family's pin has moved.
    """
    return [versions.PROJECTION_PATH / family / versions.FIXES_NAME] + [
        versions.INTEROP_PATH / name / "version.txt"
        for name, released_with in versions.FAMILY_RELEASED_INTEROP.items()
        if released_with == family
    ]


def committed_packages(family: str) -> set[str]:
    """
    The distributions of @p family as committed.
    """
    family_path = (versions.PROJECTION_PATH / family).relative_to(versions.REPO_PATH)
    files = subprocess.run(
        ["git", "ls-tree", "-r", "--name-only", "HEAD", family_path.as_posix()],
        cwd=versions.REPO_PATH,
        capture_output=True,
        text=True,
        check=True,
    )

    return {
        Path(path).parent.name
        for path in files.stdout.splitlines()
        if Path(path).name == "deps.json"
    }


def generated_packages(family: str) -> set[str]:
    """
    The distributions of @p family in the working tree.
    """
    family_path = versions.PROJECTION_PATH / family

    return {path.parent.name for path in family_path.glob("*/deps.json")}


def on_pypi(distribution: str) -> bool:
    try:
        with urllib.request.urlopen(f"https://pypi.org/pypi/{distribution}/json"):
            return True
    except urllib.error.HTTPError as error:
        if error.code == 404:
            return False

        raise


def changelog_with_update(
    changelog: str, family: str, previous: str, version: str
) -> str:
    """
    @p changelog with a line under [Unreleased] saying that @p family moved
    from upstream's release @p previous to @p version, linked to upstream's
    notes on it.

    A line from an earlier move that has not been released yet is replaced
    rather than added to, keeping the release it moved from, so that the
    changelog says what the family's next release changes.
    """
    name = UPSTREAM_NAMES[family]
    label = f"{name.removeprefix('the ')} {version}"

    if not UNRELEASED.search(changelog):
        # above the newest release
        newest = SECTION_END.search(changelog)
        at = newest.start() if newest else len(changelog)
        changelog = changelog[:at] + "## [Unreleased]\n\n" + changelog[at:]

    unreleased = UNRELEASED.search(changelog)
    assert unreleased is not None
    end = SECTION_END.search(changelog, unreleased.end())
    stop = end.start() if end else len(changelog)
    section = changelog[unreleased.end() : stop]
    earlier = re.search(
        rf"^- Updated {re.escape(name)} to \[[^\]]+\]\[([^\]]+)\], from (\S+)\.\n",
        section,
        re.MULTILINE,
    )
    start = earlier[2] if earlier else previous
    line = f"- Updated {name} to [{version}][{label}], from {start}.\n"

    if earlier:
        section = section[: earlier.start()] + line + section[earlier.end() :]
    elif changed := re.search(r"^### Changed\n", section, re.MULTILINE):
        section = section[: changed.end()] + line + section[changed.end() :]
    else:
        # Keep a Changelog puts Changed after Added
        added = re.search(r"^### Added\n(?:- .*\n)*\n?", section, re.MULTILINE)
        at = added.end() if added else len(section) - len(section.lstrip("\n"))
        section = section[:at] + "### Changed\n" + line + "\n" + section[at:]

    changelog = changelog[: unreleased.end()] + section + changelog[stop:]

    if earlier:
        definition = re.compile(rf"^\[{re.escape(earlier[1])}\]: .*\n", re.MULTILINE)
        changelog = definition.sub("", changelog)

    url = UPSTREAM_NOTES[family].format(version=version)

    # beside the other link definitions, or as the first of them
    if not re.search(r"^\[[^\]]+\]: .*\n\Z", changelog, re.MULTILINE):
        changelog += "\n"

    return changelog + f"[{label}]: {url}\n"


def regenerate() -> None:
    """
    Fetches the pinned packages and generates everything from them, as
    .github/workflows/generate.yaml does.
    """
    subprocess.run(
        [
            "powershell",
            "-NoProfile",
            "-ExecutionPolicy",
            "Bypass",
            "-File",
            SCRIPTS_PATH / "fetch-tools.ps1",
        ],
        check=True,
    )

    for script in [
        "generate-cppwinrt.py",
        "generate-pywinrt.py",
        "generate-pyproject.py",
        "check-dependencies.py",
    ]:
        subprocess.run([sys.executable, SCRIPTS_PATH / script], check=True)


def family_report(
    family: str,
    previous: str,
    added: set[str],
    removed: set[str],
    new_on_pypi: set[str],
) -> list[str]:
    """
    What moving @p family's pin from @p previous releases, as Markdown lines.
    """
    package = versions.NUGET_PACKAGES[family]
    version = versions.family_versions()[family]
    lines = [
        f"## {package} {previous} → {versions.nuget_versions()[family]}",
        "",
        f"Released as `{version}` by the tag"
        f" `{versions.release_tag(family, version)}`.",
        "",
        f"- Packages added: {', '.join(f'`{name}`' for name in sorted(added)) or 'none'}",
        f"- Packages removed: {', '.join(f'`{name}`' for name in sorted(removed)) or 'none'}",
    ]

    if not new_on_pypi:
        return lines

    count = len(new_on_pypi)
    lines.append(f"- Not on PyPI yet: {count}")

    if family in packages.TRUSTED_PUBLISHING:
        lines.append(
            "  - Each needs a pending publisher on PyPI naming `wheels.yaml`"
            " before the tag is pushed."
        )
    elif count > NEW_PROJECTS_PER_DAY:
        days = math.ceil(count / NEW_PROJECTS_PER_DAY)
        lines.append(
            f"  - PyPI creates about {NEW_PROJECTS_PER_DAY} new projects a day,"
            f" so the upload takes about {days} days: re-run the `publish` job"
            " each day until it passes."
        )

    return lines


def changes_report() -> list[str]:
    """
    How big the change is and what it touches outside projection/, as
    Markdown lines.
    """
    stat = subprocess.run(
        ["git", "diff", "--shortstat", "HEAD"],
        cwd=versions.REPO_PATH,
        capture_output=True,
        text=True,
        check=True,
    ).stdout.strip()
    changed = subprocess.run(
        ["git", "diff", "--name-only", "HEAD"],
        cwd=versions.REPO_PATH,
        capture_output=True,
        text=True,
        check=True,
    ).stdout.splitlines()
    untracked = subprocess.run(
        ["git", "ls-files", "--others", "--exclude-standard"],
        cwd=versions.REPO_PATH,
        capture_output=True,
        text=True,
        check=True,
    ).stdout.splitlines()

    # a directory or two deep is enough to say which package it is
    outside = sorted(
        {
            "/".join(path.split("/")[:2])
            for path in changed + untracked
            if not path.startswith(("projection/", ".config/", "nullability/"))
        }
    )

    lines = [
        "## The change",
        "",
        f"- {stat}, and {len(untracked)} new files",
    ]

    if outside:
        lines.append(
            "- Outside the projection: " + ", ".join(f"`{path}`" for path in outside)
        )

    return lines


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Brings the tree to the upstream releases its families are"
        " pinned at."
    )
    parser.add_argument(
        "--family",
        metavar="NAME",
        help="only this family",
    )
    parser.add_argument(
        "--report",
        metavar="PATH",
        type=Path,
        help="where to write what the change releases, rather than stdout",
    )
    args = parser.parse_args()

    pins = versions.upstream_pins()

    if args.family:
        if args.family not in pins:
            raise SystemExit(f"{args.family} is not a family that this tree publishes")

        pins = {args.family: pins[args.family]}

    committed = committed_tools()
    moved = moved_pins(pins, committed)

    if not moved:
        print("Every pin is where .config/_tools.json has it, so nothing moved")
        return

    for family, version in moved.items():
        package = versions.NUGET_PACKAGES[family]
        print(f"{family}: {package} {committed[package]} -> {version}")

    previous = {family: committed_packages(family) for family in moved}
    tools = tools_with_pins(versions.nuget_package_versions(), moved)
    versions.TOOLS_JSON_PATH.write_bytes((json.dumps(tools, indent=4) + "\n").encode())

    for family, version in moved.items():
        for path in stale_fixes(family):
            path.unlink(missing_ok=True)

        package = versions.NUGET_PACKAGES[family]
        path = versions.PROJECTION_PATH / family / "CHANGELOG.md"
        changelog = path.read_bytes().decode()
        path.write_bytes(
            changelog_with_update(
                changelog, family, committed[package], version
            ).encode()
        )

    regenerate()

    lines: list[str] = []

    for family in moved:
        package = versions.NUGET_PACKAGES[family]
        generated = generated_packages(family)
        added = generated - previous[family]
        lines += family_report(
            family,
            committed[package],
            added,
            previous[family] - generated,
            {name for name in added if not on_pypi(name)},
        )
        lines.append("")

    report = "\n".join(lines + changes_report()) + "\n"

    if args.report:
        args.report.write_bytes(report.encode())
    else:
        print()
        print(report, end="")


if __name__ == "__main__":
    main()
