"""Prints the tag that releases each family of packages from this tree.

    python scripts/release-tags.py
    python scripts/release-tags.py --family webview2

Pushing one of these tags is what releases a family: .github/workflows/wheels.yaml
builds that family and uploads it to PyPI. They are printed rather than typed
because the version in a tag is the version the tree carries, and a tag whose
version says something else releases nothing - the workflow refuses it.

The families are printed in the order a full release goes in, which is the
order their packages can be resolved in: winrt-runtime first, because
everything requires it, and the Windows SDK before the families whose
packages depend on one of its.
"""

import argparse

# the version each family is published with, and the tag that says so
import versions

parser = argparse.ArgumentParser(
    description="Prints the tag that releases each family of packages."
)
parser.add_argument(
    "--family",
    metavar="NAME",
    help="print only this family's tag",
)
args = parser.parse_args()

tags = versions.release_tags()

if args.family:
    if args.family not in tags:
        raise SystemExit(f"{args.family} is not a family that this tree publishes")

    tags = {args.family: tags[args.family]}

width = max(len(family) for family in tags)

for family, tag in tags.items():
    print(f"{family.ljust(width)}  {tag}")
