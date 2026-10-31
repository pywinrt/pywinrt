"""Lists the names a family published that its packages now spell differently.

    python scripts/list-renames.py
    python scripts/list-renames.py --family wasdk

When the generator comes to spell a Python name differently, as it did when it
learned to keep an acronym in one word, code written against the name a family
published stops working. So each family that has been released keeps a list
of the old names in its renames.json, which scripts/generate-pywinrt.py hands
to the generator, and each one keeps working as a deprecated alias.

This compares the projection tables at the family's newest release with the
ones in the tree, which have to be generated with the new spelling first, and
adds every member whose name changed to the list, under the release that will
first publish the new names: how long a name has been deprecated decides when
its alias moves on to the next stage and when it is removed. Then generate the
projection again, so that the aliases are written.

A member is found in both by what WinRT calls it rather than by its Python
name, which is what changed: a method by its overloads, a property by its
getter, an event by its add method, and a member of an enum by its value.
"""

import argparse
import json
import re
import subprocess
from collections import defaultdict
from collections.abc import Callable

# the version each release unit is published with, and the tag that says so
import versions

TABLE = "_table.pywinrt.txt"
RENAMES_NAME = "renames.json"

# what a member of a type is, with what identifies it besides its Python name
Key = tuple[str, ...]


def git(*args: str) -> str:
    return subprocess.run(
        ["git", *args],
        cwd=versions.REPO_PATH,
        capture_output=True,
        text=True,
        encoding="utf-8",
        check=True,
    ).stdout


def version_key(version: str) -> tuple[int, tuple[int, ...], int, int]:
    """
    A family's version, such as 4!2.6.4, 4!10.0.28000.2705.1 or 4!2.7.0b1, as
    something that compares the way the versions do.
    """
    match = re.fullmatch(r"(?:(\d+)!)?(\d+(?:\.\d+)*)(?:(a|b|rc)(\d+))?", version)

    if not match:
        raise ValueError(f"{version!r} is not the version of a family")

    epoch, release, phase, number = match.groups()

    return (
        int(epoch or 0),
        tuple(int(part) for part in release.split(".")),
        ["a", "b", "rc", None].index(phase),
        int(number or 0),
    )


def released_tags(family: str) -> list[str]:
    """
    The tags that released the family, newest last.
    """
    prefix = f"{versions.TAG_PREFIX}/{family}/"

    def version(tag: str) -> tuple[int, tuple[int, ...], int, int]:
        match = re.fullmatch(r"(?:e(\d+))?v(.+)", tag.removeprefix(prefix))

        if not match:
            raise RuntimeError(f"{tag} does not name a version")

        epoch, release = match.groups()

        return version_key(f"{epoch}!{release}" if epoch else release)

    return sorted(git("tag", "--list", f"{prefix}*").split(), key=version)


def read_members(text: str) -> dict[str, dict[Key, str]]:
    """
    The Python name of each member of each type in a table, keyed by the full
    name of the type and then by what WinRT calls the member.

    A method is keyed once for each of its overloads, since a release can add
    one, and is renamed as a whole.
    """
    members: dict[str, dict[Key, str]] = defaultdict(dict)
    record = ""
    member: list[str] = []
    values: dict[str, int] = defaultdict(int)

    for line in text.splitlines():
        if not line.strip():
            continue

        indent = len(line) - len(line.lstrip(" "))
        words = line.split()

        if indent == 0:
            record = words[1]
            member = []
            values.clear()
        elif indent == 4:
            member = words

            if words[0] == "constant":
                # two members of an enum can have one value, so the order of
                # those is what tells them apart
                value = words[2]
                members[record][("constant", value, str(values[value]))] = words[1]
                values[value] += 1
            elif words[0] == "field":
                members[record][("field", words[2])] = words[1]
        elif indent == 8 and member:
            kind, rest = member[0], tuple(member[2:])
            declaring = next((w for w in words if w.startswith("declaring=")), "")

            if kind == "method" and words[0] == "method":
                members[record][(kind, *rest, words[1], declaring)] = member[1]
            elif kind == "property" and words[0] == "get":
                members[record][(kind, *rest, words[1], declaring)] = member[1]
            elif kind == "event" and words[0] == "add":
                members[record][(kind, *rest, words[1], declaring)] = member[1]

    return members


def read_tables(
    paths: list[str], read: Callable[[str], str]
) -> dict[str, dict[Key, str]]:
    members: dict[str, dict[Key, str]] = {}

    for path in paths:
        members.update(read_members(read(path)))

    return members


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Lists the names a family published that its packages now spell"
        " differently."
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

    published = versions.published_versions()

    for family in families:
        tags = released_tags(family)

        if not tags:
            print(f"{family}: never released, so nothing was renamed")
            continue

        tag = tags[-1]
        family_path = versions.PROJECTION_PATH / family
        prefix = family_path.relative_to(versions.REPO_PATH).as_posix()

        old = read_tables(
            [
                path
                for path in git("ls-tree", "-r", "--name-only", tag, prefix).split()
                if path.endswith(f"/{TABLE}")
            ],
            lambda path: git("show", f"{tag}:{path}"),
        )
        new = read_tables(
            [
                path.relative_to(versions.REPO_PATH).as_posix()
                for path in sorted(family_path.glob(f"*/**/{TABLE}"))
            ],
            lambda path: (versions.REPO_PATH / path).read_text(encoding="utf-8"),
        )

        renamed: dict[str, dict[str, str]] = defaultdict(dict)

        for record, members in old.items():
            now = new.get(record, {})
            names = set(now.values())

            for key, name in members.items():
                if key not in now or now[key] == name or name in names:
                    continue

                if key[0] == "field":
                    raise RuntimeError(
                        f"{record}: the field {name} is now {now[key]}, and a"
                        " struct's fields cannot be aliased"
                    )

                renamed[record][name] = now[key]

        path = family_path / RENAMES_NAME
        renames: dict[str, dict[str, dict[str, str]]] = (
            json.loads(path.read_text(encoding="utf-8")) if path.exists() else {}
        )

        # a name renamed before keeps pointing at the member, whatever it is
        # called now
        for group in renames.values():
            for record, names in group.items():
                for old_name, name in names.items():
                    names[old_name] = renamed.get(record, {}).get(name, name)

        count = sum(len(names) for names in renamed.values())

        if not count:
            print(f"{family}: nothing renamed since {tag}")
            continue

        release = published[family]

        if versions.release_tag(family, release) in tags:
            raise SystemExit(
                f"{family}: {count} names renamed since {tag}, but the tree is"
                f" still at {release}, which is released: run fix-versions.py first"
            )

        group = renames.setdefault(release, {})

        for record, names in renamed.items():
            group.setdefault(record, {}).update(names)

        renames = {
            release: {
                record: dict(sorted(names.items()))
                for record, names in sorted(group.items())
                if names
            }
            for release, group in (
                (release, renames[release])
                for release in sorted(renames, key=version_key)
            )
        }

        path.write_bytes((json.dumps(renames, indent=4) + "\n").encode())
        print(f"{family}: {count} names renamed since {tag}, listed under {release}")


if __name__ == "__main__":
    main()
