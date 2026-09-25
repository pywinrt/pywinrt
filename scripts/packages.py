"""
Every distribution this repository publishes, where its source is and how its
wheels are built.

scripts/build-sdist.py and scripts/build-bdist.py both walk this list, and a
release walks part of it: PyPI refuses a file that already exists, so a
release that rebuilt every family would fail on the families that did not
change. .github/workflows/wheels.yaml therefore releases one family at a
time, and the filters below are what both scripts mean by --family and
--package.
"""

import argparse
import os
from collections.abc import Iterable, Iterator
from enum import Enum
from pathlib import Path
from typing import NamedTuple

# the family table and the version each family is published with
import versions

REPO_PATH = versions.REPO_PATH
PROJECTION_PATH = (REPO_PATH / "projection").resolve()
INTEROP_PATH = (REPO_PATH / "interop").resolve()
REDIST_PATH = (REPO_PATH / "redist").resolve()

# The unpacked NuGet packages that a build reads something out of, named by
# the environment variable the build looks for. The Windows App SDK is
# redistributed with an app rather than part of Windows, so the two interop
# modules that call it take its headers and its import libraries from these;
# it is a metapackage over components published separately, so each module
# names the one it needs. WebView2 is where the component .dll comes from,
# and pointing at the unpacked copy is what keeps a build in this tree off
# the network: a build hook that finds no copy downloads the package itself.
NUGET_ENVIRONMENT = {
    "WASDK_FOUNDATION_PATH": "Microsoft.WindowsAppSDK.Foundation",
    "WASDK_INTERACTIVE_EXPERIENCES_PATH": "Microsoft.WindowsAppSDK.InteractiveExperiences",
    "WASDK_RUNTIME_PATH": "Microsoft.WindowsAppSDK.Runtime",
    "WEBVIEW2_PATH": "Microsoft.Web.WebView2",
}

# The distribution in each interop package's directory. A directory is named
# after the distribution in it, except where that name is long enough to push
# a build path past the 260 characters Windows allows by default: this one is
# 68, and a wheel build puts a .dist-info directory named after it, and a
# licenses directory inside that, underneath it.
INTEROP_DISTRIBUTIONS = {
    "winrt-wasdk-bootstrap": (
        "winrt-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap"
    ),
}

# The Windows App SDK components each hand-written module compiles against,
# named by the environment variable above that says where fetch-tools.ps1
# unpacked each one. The App SDK is a metapackage, so what a module includes
# is spread over several of them. Compiling against the App SDK is also what
# puts a module in its family: these two are released with it rather than
# with the Windows SDK that the other six interop modules follow.
APP_SDK_INTEROP_COMPONENTS = {
    # Microsoft.UI.Interop.h
    "winrt-Microsoft.UI.Interop": ["WASDK_INTERACTIVE_EXPERIENCES_PATH"],
    # MddBootstrap.h and its import library, and WindowsAppSDK-VersionInfo.h
    "winrt-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap": [
        "WASDK_FOUNDATION_PATH",
        "WASDK_RUNTIME_PATH",
    ],
}

# A package that redistributes a component .dll is released with the family
# of the projection that imports it, since the .dll and the projection come
# out of one NuGet package and move together.
REDIST_FAMILIES = {
    "winrt-Microsoft.Web.WebView2.Dll": "webview2",
}


class Build(Enum):
    """
    What building a package's wheels takes.
    """

    # one py3-none-any wheel, built by pyproject-build without a compiler
    ANY = "any"
    # one py3-none-win_* wheel per architecture, built the same way: nothing
    # in the package is compiled either, but what it carries is one
    # architecture's binary
    REDIST = "redist"
    # the 15-build cibuildwheel matrix
    COMPILED = "compiled"


class Package(NamedTuple):
    """
    One distribution this repository publishes.
    """

    # the directory its packaging is in, which is named after the
    # distribution except where INTEROP_DISTRIBUTIONS says otherwise
    path: Path
    distribution: str
    # the family it is released with, which is what says where its version
    # comes from and which tag publishes it
    family: str
    build: Build


def interop_distribution(package_path: Path) -> str:
    return INTEROP_DISTRIBUTIONS.get(package_path.name, package_path.name)


def generated() -> Iterator[Package]:
    """
    Every package that scripts/generate-pyproject.py writes the packaging of,
    in the order a build takes them: the table compiler and the runtime
    first, since everything else is built against one or the other.

    The test component is one of these. It is projected into this tree so
    that the suite has something to run against and is published nowhere, so
    it has a family and a directory like the rest without being released.
    """
    yield Package(
        REPO_PATH / "table", "winrt-table-compiler", versions.RUNTIME_FAMILY, Build.ANY
    )
    yield Package(
        REPO_PATH / "runtime", "winrt-runtime", versions.RUNTIME_FAMILY, Build.COMPILED
    )

    # a projection distribution is a directory with a deps.json in it, and
    # the directory that holds it is the family
    for deps_path in sorted(PROJECTION_PATH.glob("*/*/deps.json")):
        family = deps_path.parent.parent.name

        yield Package(deps_path.parent, deps_path.parent.name, family, Build.ANY)

    for path in sorted(REDIST_PATH.glob("winrt-*")):
        yield Package(path, path.name, REDIST_FAMILIES[path.name], Build.REDIST)

    for path in sorted(INTEROP_PATH.glob("winrt-*")):
        distribution = interop_distribution(path)
        family = "wasdk" if distribution in APP_SDK_INTEROP_COMPONENTS else "winrt"

        yield Package(path, distribution, family, Build.COMPILED)


def published() -> Iterator[Package]:
    """
    Every package this repository uploads to PyPI.
    """
    return (
        package
        for package in generated()
        if package.family not in versions.UNPUBLISHED_FAMILIES
    )


def distribution_families() -> dict[str, str]:
    """
    The family of every distribution in this tree, keyed by distribution
    name.
    """
    return {package.distribution: package.family for package in generated()}


def set_nuget_environment() -> None:
    """
    Points every build in this tree at the NuGet packages that
    scripts/fetch-tools.ps1 unpacked, so that building here never goes to the
    network for one.
    """
    for env, package in NUGET_ENVIRONMENT.items():
        os.environ[env] = os.fspath(versions.unpacked_nuget_path(package))


def add_arguments(parser: argparse.ArgumentParser) -> None:
    """
    Adds the filters that say which packages a build covers.

    Each of them reads an empty string as "not given", because that is what a
    workflow input that was left blank arrives as.
    """
    parser.add_argument(
        "--family",
        default="",
        metavar="NAME",
        help="build only this family; 'all' and an empty value build them all",
    )
    parser.add_argument(
        "--package",
        action="append",
        default=[],
        metavar="NAME[,NAME...]",
        help="build only these distributions; may be given more than once",
    )
    parser.add_argument(
        "--tag",
        default="",
        metavar="TAG",
        help="build the family that this release tag publishes",
    )


def named_packages(values: Iterable[str]) -> list[str]:
    """
    The distribution names --package was given, which may be one name per
    option or a comma-separated list, since a workflow input is one string.
    """
    return [name for value in values for name in value.split(",") if name]


def select(args: argparse.Namespace) -> list[Package]:
    """
    The packages that the filters in add_arguments() cover.

    A name that matches nothing is an error rather than an empty build: a
    release names the packages it means, and a typo that quietly published
    nothing would be found only by looking at what arrived on PyPI.
    """
    family = args.family

    if args.tag:
        if family and family != "all":
            raise SystemExit(f"--tag {args.tag} and --family {family} disagree")

        try:
            family = versions.family_of_tag(args.tag)
        except ValueError as error:
            raise SystemExit(error) from error

    selected = list(published())

    if family and family != "all":
        if family not in versions.published_versions():
            raise SystemExit(f"{family} is not a family that this tree publishes")

        selected = [package for package in selected if package.family == family]

    names = named_packages(args.package)

    if names:
        missing = sorted({*names} - {package.distribution for package in selected})

        if missing:
            raise SystemExit(f"not published by this build: {', '.join(missing)}")

        selected = [package for package in selected if package.distribution in names]

    return selected
