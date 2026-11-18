"""
Every distribution this repository publishes, where its source is and how its
wheels are built.

scripts/build-sdist.py and scripts/build-bdist.py both walk this list, and a
release walks part of it: PyPI refuses a file that already exists, so a
release that rebuilt everything would fail on whatever did not change.
.github/workflows/wheels.yaml therefore releases one unit at a time - a
family of generated packages, or one package that is written by hand and
carries a version of its own - and the filters below are what both scripts
mean by --family, --package and --tag.
"""

import argparse
import os
from collections.abc import Iterable, Iterator
from enum import Enum
from pathlib import Path
from typing import NamedTuple

# which family each package written by hand is released with
import families

# the version each release unit is published with, and the tag that says so
import versions

REPO_PATH = versions.REPO_PATH
PROJECTION_PATH = (REPO_PATH / "projection").resolve()
INTEROP_PATH = (REPO_PATH / "interop").resolve()
REDIST_PATH = (REPO_PATH / "redist").resolve()

# Where each architecture's runtimes are in a NuGet package, keyed by the
# platform tag of the wheel that carries them and holding the name setuptools
# builds that architecture under, since the two spell it differently. Both
# NuGet packages that PyWinRT redistributes a binary from lay their runtimes
# out this way; only the leaf directory differs.
RUNTIME_DIRS = {
    "win32": ("win32", "runtimes/win-x86"),
    "win_amd64": ("win-amd64", "runtimes/win-x64"),
    "win_arm64": ("win-arm64", "runtimes/win-arm64"),
}


class DllPayload(NamedTuple):
    """
    A Microsoft binary that PyWinRT redistributes, and where it comes from.
    """

    # the environment variable that scripts/build-sdist.py and
    # scripts/build-bdist.py point at a copy of the NuGet package it is taken
    # out of (see families.NUGET_ENVIRONMENT)
    env: str
    # the file itself, and the directory under RUNTIME_DIRS that holds it
    dll_name: str
    native_dir: str
    # the upstream license text, keyed by where it is in the NuGet package and
    # named by what the package carries it as, and the SPDX identifier of the
    # license. Neither of these licenses has an SPDX id, so both are
    # LicenseRef-.
    license_files: dict[str, str]
    license_id: str

    @property
    def nuget_package(self) -> str:
        """
        The NuGet package it is taken out of.
        """
        return families.NUGET_ENVIRONMENT[self.env].name

    @property
    def family(self) -> str:
        """
        The family whose release the NuGet package is part of.
        """
        return families.NUGET_ENVIRONMENT[self.env].family

    def dll_paths(self, wheel_tag: bool) -> dict[str, str]:
        """
        Where each architecture's .dll is, keyed the way the build that reads
        this asks for a platform.
        """
        return {
            tag if wheel_tag else plat_name: (
                f"{runtime_dir}/{self.native_dir}/{self.dll_name}"
            )
            for tag, (plat_name, runtime_dir) in RUNTIME_DIRS.items()
        }

    def nuget_path(self) -> Path:
        """
        The unpacked NuGet package that scripts/fetch-tools.ps1 downloaded.
        """
        return versions.unpacked_nuget_path(self.nuget_package)


class RedistPackage(NamedTuple):
    """
    A distribution that redistributes a component .dll, and the projection
    that needs it.
    """

    # the distribution that depends on it and imports it, and the module it
    # imports
    importer: str
    module: str
    payload: DllPayload


# The component .dlls that are redistributed in a distribution of their own,
# whose sources are under redist/. A .dll is built for one architecture and
# the projection that needs it is not, so the two cannot be published in one
# wheel: the projection depends on the package below and imports it, and
# importing it is what puts the .dll on the DLL search path. Each is released
# with the family of the NuGet package its .dll comes out of, which is the
# family of the projection that imports it, since the two move together.
REDIST_PACKAGES = {
    "winrt-Microsoft.Web.WebView2.Dll": RedistPackage(
        importer="winrt-Microsoft.Web.WebView2",
        module="winrt.microsoft.web.webview2.dll",
        payload=DllPayload(
            env="WEBVIEW2_PATH",
            dll_name="Microsoft.Web.WebView2.Core.dll",
            native_dir="native_uap",
            license_files={
                "LICENSE.txt": "LICENSE.Microsoft.Web.WebView2.txt",
                "NOTICE.txt": "NOTICE.Microsoft.Web.WebView2.txt",
            },
            license_id="LicenseRef-Microsoft-Web-WebView2",
        ),
    ),
}

# The Windows App Runtime bootstrapper, which is the one Microsoft binary that
# a compiled package redistributes. The extension module links against its
# import library, and Windows looks for a module's dependencies in the
# directory the module was loaded from, so it ships beside the .pyd rather
# than in a package of its own the way a component .dll of an
# architecture-independent projection does.
BOOTSTRAP_DLL = DllPayload(
    env="WASDK_FOUNDATION_PATH",
    dll_name="Microsoft.WindowsAppRuntime.Bootstrap.dll",
    native_dir="native",
    license_files={"license.txt": "LICENSE.Microsoft.WindowsAppSDK.txt"},
    license_id="LicenseRef-Microsoft-WindowsAppSDK",
)


# The families and units whose packages are uploaded to PyPI with trusted
# publishing rather than the account's token, each of whose projects names
# .github/workflows/wheels.yaml as its publisher. A family's name covers every
# unit that releases one of its packages, a fix to an interop module released
# with it on its own included, and "runtime" covers both winrt-runtime and
# winrt-table-compiler. A unit written by hand is named on its own. A project
# new to PyPI is created by its pending publisher and an older one has the
# publisher added by hand, so a name goes here once every project it covers
# has it. One upload authenticates one way, so a unit is on one side or the
# other as a whole.
TRUSTED_PUBLISHING = frozenset(
    {
        "winui2",
        "webview2",
        "wasdk",
        "runtime",
        "winrt-Windows.ApplicationModel.DataTransfer.Interop",
        "winrt-Microsoft.UI.Xaml.Media.DXInterop",
    }
)


def is_trusted(package: "Package") -> bool:
    """
    Whether @p package is uploaded to PyPI with trusted publishing: its family
    is, or the unit that releases it is.
    """
    return package.family in TRUSTED_PUBLISHING or package.release in TRUSTED_PUBLISHING


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
    # a cibuildwheel build per Python and architecture
    COMPILED = "compiled"
    # a cibuildwheel build per architecture: one stable ABI wheel, which serves
    # every CPython that is not free-threaded and is tested on each, and one
    # wheel per free-threaded CPython, which has no stable ABI
    STABLE_ABI = "stable-abi"


class Package(NamedTuple):
    """
    One distribution this repository publishes.
    """

    # the directory its packaging is in, which is named after the
    # distribution
    path: Path
    distribution: str
    # the upstream family it belongs to, which says which NuGet package it
    # was built against; --family groups by this
    family: str
    # what a tag releases it as, named after the directory its sources are
    # in: the family for a generated package, and the package itself for one
    # that is written by hand and so carries a version of its own, unless
    # families.FAMILY_RELEASED_INTEROP releases it with a family
    release: str
    build: Build


def check_families() -> None:
    """
    Refuses a name in scripts/families.py that matches nothing: an interop
    module with no directory, or a family that is not one this tree projects.
    Either would leave a package out of its family's release without a word.
    """
    interop = {path.name for path in INTEROP_PATH.glob("winrt-*")}
    unknown = sorted(set(families.INTEROP_NUGET_PACKAGES) - interop)

    if unknown:
        raise RuntimeError(
            f"families.INTEROP_NUGET_PACKAGES names {', '.join(unknown)},"
            " which is not a directory under interop/"
        )

    named = {package.family for package in families.NUGET_ENVIRONMENT.values()}
    unknown = sorted(named - set(versions.NUGET_PACKAGES))

    if unknown:
        raise RuntimeError(
            f"families.NUGET_ENVIRONMENT names the family {', '.join(unknown)},"
            " which is not a key of versions.NUGET_PACKAGES"
        )


def generated() -> Iterator[Package]:
    """
    Every package that scripts/generate-pyproject.py writes the packaging of,
    in the order a build takes them: the table compiler and the runtime
    first, since everything else is built against one or the other.

    The test component is one of these. It is projected into this tree so
    that the suite has something to run against and is published nowhere, so
    it has a family and a directory like the rest without being released.
    """
    check_families()

    table = REPO_PATH / "table"
    runtime = REPO_PATH / "runtime"

    yield Package(
        table, "winrt-table-compiler", versions.RUNTIME_FAMILY, table.name, Build.ANY
    )
    yield Package(
        runtime, "winrt-runtime", versions.RUNTIME_FAMILY, runtime.name, Build.COMPILED
    )

    # a projection distribution is a directory with a deps.json in it, and
    # the directory that holds it is the family
    for deps_path in sorted(PROJECTION_PATH.glob("*/*/deps.json")):
        family = deps_path.parent.parent.name

        yield Package(
            deps_path.parent, deps_path.parent.name, family, family, Build.ANY
        )

    for path in sorted(REDIST_PATH.glob("winrt-*")):
        family = REDIST_PACKAGES[path.name].payload.family

        yield Package(path, path.name, family, family, Build.REDIST)

    for path in sorted(INTEROP_PATH.glob("winrt-*")):
        distribution = path.name
        family = families.interop_family(distribution)
        release = versions.interop_release(path.name)

        yield Package(path, distribution, family, release, Build.STABLE_ABI)


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
    scripts/fetch-tools.ps1 unpacked, so that building here goes to the
    network only for a package it did not unpack.

    A package that is not there is left to the build that needs it, which
    downloads that one package and nothing else. That is why a release fetches
    nothing ahead of its builds: most units need no NuGet package at all.
    """
    for env, package in families.NUGET_ENVIRONMENT.items():
        path = versions.unpacked_nuget_path(package.name)

        if path.is_dir():
            os.environ[env] = os.fspath(path)


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
        help="build only the packages built against this upstream;"
        " 'all' and an empty value build them all",
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
        help="build the one unit that this release tag publishes",
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

    A tag names one release unit, which is a family of generated packages or
    a single package written by hand. --family groups by the upstream a
    package was built against instead, so it covers the interop modules built
    against a family that a release of that family does not.

    A name that matches nothing is an error rather than an empty build: a
    release names the packages it means, and a typo that quietly published
    nothing would be found only by looking at what arrived on PyPI.
    """
    selected = list(published())

    if args.tag:
        if args.family and args.family != "all":
            raise SystemExit(f"--tag {args.tag} and --family {args.family} disagree")

        try:
            unit = versions.unit_of_tag(args.tag)
        except ValueError as error:
            raise SystemExit(error) from error

        selected = [package for package in selected if package.release == unit]

        if unit in versions.NUGET_PACKAGES:
            # a family with fixes is released at the newest, which releases
            # only the packages that carry it
            family_version = versions.family_versions()[unit]
            fixes = versions.family_fixes(unit)
            version = versions.published_versions()[unit]
            selected = [
                package
                for package in selected
                if fixes.get(package.distribution, family_version) == version
            ]
    elif args.family and args.family != "all":
        if args.family not in {package.family for package in selected}:
            raise SystemExit(f"{args.family} is not a family that this tree publishes")

        selected = [package for package in selected if package.family == args.family]

    names = named_packages(args.package)

    if names:
        missing = sorted({*names} - {package.distribution for package in selected})

        if missing:
            raise SystemExit(f"not published by this build: {', '.join(missing)}")

        selected = [package for package in selected if package.distribution in names]

    return selected
