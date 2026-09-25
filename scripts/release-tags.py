"""Prints the tag that releases each unit of this tree.

    python scripts/release-tags.py
    python scripts/release-tags.py --unit webview2

A unit is what one tag releases: a family of generated packages, or one
package that is written by hand and so carries a version of its own. Pushing
the tag runs .github/workflows/wheels.yaml, which builds that unit and
uploads it to PyPI. The tags are printed rather than typed because the
version in one is the version the tree carries, and a tag whose version says
something else releases nothing - the build refuses it.

They are printed in the order a full release goes in, which is the order the
packages can be resolved in: winrt-runtime first because everything requires
it, then the compiler that builds a projection package, then the generated
families, then the interop modules, each of which names a projection package.
"""

import argparse

# the version each unit is published with, and the tag that says so
import versions

parser = argparse.ArgumentParser(
    description="Prints the tag that releases each unit of this tree."
)
parser.add_argument(
    "--unit",
    metavar="NAME",
    help="print only this unit's tag",
)
args = parser.parse_args()

tags = versions.release_tags()

if args.unit:
    if args.unit not in tags:
        raise SystemExit(f"{args.unit} is not a unit that this tree publishes")

    tags = {args.unit: tags[args.unit]}

width = max(len(unit) for unit in tags)

for unit, tag in tags.items():
    print(f"{unit.ljust(width)}  {tag}")
