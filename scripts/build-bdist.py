"""Builds the wheels of every package that is published.

There are three parts, built by different tools for different reasons, and
with no arguments all of them are built:

    python scripts/build-bdist.py
    python scripts/build-bdist.py --pure
    python scripts/build-bdist.py --compiled --only cp314-win_amd64
    python scripts/build-bdist.py --stable-abi --platform windows --archs AMD64

A release publishes one family at a time, so each part can be narrowed to
one family or to packages named outright:

    python scripts/build-bdist.py --family webview2
    python scripts/build-bdist.py --package winrt-Windows.Foundation

A projection package is data and winrt-table-compiler is pure Python, so each
of them is one py3-none-any wheel built once - that is the pure part. It needs
winrt-table-compiler *installed* rather than merely importable, because
hatchling finds the hook that compiles the tables through the entry point in
its metadata. The packages that redistribute a component .dll are in that part
too, since nothing in them is compiled either; what is in them is built for
one architecture, so each is three py3-none-win_* wheels rather than one
py3-none-any wheel. Nothing in the part needs a compiler.

winrt-runtime is compiled against the whole C API, so it is the compiled
part: a cibuildwheel build per Python and architecture, which --only names.

The hand-written interop modules are compiled against the stable ABI, so they
are the stable ABI part: a cibuildwheel build per architecture, in which
cibuildwheel builds one cp311-abi3 wheel and tests it on every later CPython,
and builds a wheel of its own for each free-threaded one. Given --only, the
part builds that interpreter's wheel if it is free-threaded and nothing if it
is not, since the abi3 wheel already serves it. Every argument other than the
ones above is passed to cibuildwheel.

A release unit is usually one part, so a workflow asks first which parts
have anything to build, and how the upload authenticates, and builds nothing
while it asks:

    python scripts/build-bdist.py --plan --tag wheels/table/v4.0.0
"""

import argparse
import os
import re
import shutil
import subprocess

# the packages that are published, and the filters that pick some of them
import packages

# The architectures a .dll package is built for, spelled the way
# sysconfig.get_platform() does.
PLAT_NAMES = ["win32", "win-amd64", "win-arm64"]


def plan(selected: list[packages.Package]) -> dict[str, str]:
    """
    What --plan answers for a build of @p selected: whether each part has a
    package to build, as pure, compiled and stable_abi, and whether its
    packages upload with trusted publishing, as trusted, which is mixed when
    they disagree.
    """
    builds = {package.build for package in selected}
    pure = bool(builds & {packages.Build.ANY, packages.Build.REDIST})
    compiled = packages.Build.COMPILED in builds
    stable_abi = packages.Build.STABLE_ABI in builds
    trusted = {package.family in packages.TRUSTED_PUBLISHING for package in selected}

    return {
        "pure": str(pure).lower(),
        "compiled": str(compiled).lower(),
        "stable_abi": str(stable_abi).lower(),
        "trusted": "mixed" if len(trusted) > 1 else str(True in trusted).lower(),
    }


def is_free_threaded(identifier: str) -> bool:
    """
    Whether the cibuildwheel build identifier @p identifier, such as
    cp314t-win_amd64, names a free-threaded CPython.
    """
    return re.match(r"cp3[0-9]+t-", identifier) is not None


def build_wheel(package: packages.Package) -> None:
    subprocess.check_call(
        [
            "pyproject-build",
            "--wheel",
            "--no-isolation",
            "--skip-dependency-check",
            "--outdir",
            "wheelhouse",
            os.fspath(package.path),
        ],
    )


def build_with_cibuildwheel(
    package: packages.Package, cibuildwheel_args: list[str]
) -> None:
    subprocess.check_call(
        ["cibuildwheel", os.fspath(package.path)]
        + (cibuildwheel_args or ["--platform", "windows"])
    )

    # Build directories are 10s of MBs and will cause CI to run out of
    # space!
    shutil.rmtree(package.path / "build")


def main() -> None:
    packages.set_nuget_environment()

    parser = argparse.ArgumentParser(
        description="Builds the wheels of every package that is published."
    )
    part = parser.add_mutually_exclusive_group()
    part.add_argument(
        "--pure",
        action="store_true",
        help="build only the wheels that are built without a compiler",
    )
    part.add_argument(
        "--compiled",
        action="store_true",
        help="build only the wheels that cibuildwheel builds per Python",
    )
    part.add_argument(
        "--stable-abi",
        action="store_true",
        help="build only the wheels that cibuildwheel builds for the stable ABI",
    )
    parser.add_argument(
        "--only",
        metavar="IDENTIFIER",
        help="build for the one cibuildwheel build identifier, such as cp314-win_amd64",
    )
    parser.add_argument(
        "--plan",
        action="store_true",
        help="print pure, compiled and stable_abi=true|false, the parts that have"
        " a package to build, and trusted=true|false|mixed, whether its units"
        " upload with trusted publishing, as GitHub Actions output lines, and"
        " build nothing",
    )
    packages.add_arguments(parser)
    args, cibuildwheel_args = parser.parse_known_args()

    selected = packages.select(args)

    if args.plan:
        for name, value in plan(selected).items():
            print(f"{name}={value}")

        return

    if args.only:
        cibuildwheel_args = ["--only", args.only, *cibuildwheel_args]

    if not args.compiled and not args.stable_abi:
        for package in selected:
            if package.build is packages.Build.ANY:
                build_wheel(package)
            elif package.build is packages.Build.REDIST:
                for plat_name in PLAT_NAMES:
                    os.environ["PYWINRT_PLAT_NAME"] = plat_name

                    build_wheel(package)

                del os.environ["PYWINRT_PLAT_NAME"]

    if not args.pure and not args.stable_abi:
        for package in selected:
            if package.build is packages.Build.COMPILED:
                build_with_cibuildwheel(package, cibuildwheel_args)

    if not args.pure and not args.compiled:
        for package in selected:
            if package.build is not packages.Build.STABLE_ABI:
                continue

            if args.only and not is_free_threaded(args.only):
                print(f"{package.distribution}: its abi3 wheel serves {args.only}")
                continue

            build_with_cibuildwheel(package, cibuildwheel_args)


if __name__ == "__main__":
    main()
