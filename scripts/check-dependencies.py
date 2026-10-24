"""Checks that every projection package requires the structs its structs nest.

    python scripts/check-dependencies.py

A struct field of a struct is resolved when the struct's type is built, as its
module is imported, which imports the distribution that defines the nested
struct. So that distribution has to be one the package requires rather than
one it offers in its [all] extra, or the import fails wherever the extra was
not installed.

What is checked is the committed tree rather than the generator: once a
package is on PyPI its requirements cannot be changed. It reads every
projection table, which is more than the unit tests can afford on each run, so
.github/workflows/generate.yaml runs it after the generators.
"""

import re
import sys
import tomllib

# the projection packages' directories
import versions

NAMESPACE = re.compile(r"^namespace ([\w.]+)$", re.MULTILINE)
NESTED_STRUCT = re.compile(
    r"^    field \w+ \w+ struct type=([\w.]+)\.\w+$", re.MULTILINE
)
# a requirement is the name of the distribution it requires, then what it asks
# of that distribution
AFTER_NAME = re.compile(r"[^\w.-]")


def main() -> int:
    projection = versions.REPO_PATH / "projection"
    packages = sorted(p.parent for p in projection.glob("*/*/pyproject.toml"))

    tables = {
        package: [
            path.read_text(encoding="utf-8")
            for path in package.rglob("_table.pywinrt.txt")
        ]
        for package in packages
    }

    # Each namespace's distribution, from the table that projects it.
    distribution_of = {
        namespace: package.name
        for package, texts in tables.items()
        for text in texts
        for namespace in NAMESPACE.findall(text)
    }

    failed = 0

    for package, texts in tables.items():
        project = tomllib.loads(
            (package / "pyproject.toml").read_text(encoding="utf-8")
        )["project"]
        dependencies: list[str] = project["dependencies"]
        required = {AFTER_NAME.split(r, maxsplit=1)[0] for r in dependencies}

        nested = {
            distribution_of[namespace]
            for text in texts
            for namespace in NESTED_STRUCT.findall(text)
            if namespace in distribution_of
        }
        missing = sorted(nested - required - {package.name})

        if missing:
            print(
                f"{package.name} does not require {', '.join(missing)}", file=sys.stderr
            )
            failed += 1

    print(f"checked {len(packages)} packages, {failed} of which are wrong")

    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
