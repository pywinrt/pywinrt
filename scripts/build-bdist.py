"""Builds the wheels of every package that is published.

There are two halves, built by different tools for different reasons, and
with no arguments both of them are built:

    python scripts/build-bdist.py
    python scripts/build-bdist.py --pure
    python scripts/build-bdist.py --compiled --only cp314-win_amd64

A release publishes one family at a time, so either half can be narrowed to
one family or to packages named outright:

    python scripts/build-bdist.py --family webview2
    python scripts/build-bdist.py --package winrt-Windows.Foundation

A projection package is data and winrt-table-compiler is pure Python, so each
of them is one py3-none-any wheel built once - that is the pure half. It needs
winrt-table-compiler *installed* rather than merely importable, because
hatchling finds the hook that compiles the tables through the entry point in
its metadata. The packages that redistribute a component .dll are in that half
too, since nothing in them is compiled either; what is in them is built for
one architecture, so each is three py3-none-win_* wheels rather than one
py3-none-any wheel. Nothing in the half needs a compiler.

winrt-runtime and the eight hand-written interop modules do compile, so they
are what is left of the 15-build cibuildwheel matrix. Every argument other
than the ones above is passed to cibuildwheel.
"""

import argparse
import os
import shutil
import subprocess

# the packages that are published, and the filters that pick some of them
import packages

# The architectures a .dll package is built for, spelled the way
# sysconfig.get_platform() does.
PLAT_NAMES = ["win32", "win-amd64", "win-arm64"]

packages.set_nuget_environment()

parser = argparse.ArgumentParser(
    description="Builds the wheels of every package that is published."
)
half = parser.add_mutually_exclusive_group()
half.add_argument(
    "--pure",
    action="store_true",
    help="build only the wheels that are built without a compiler",
)
half.add_argument(
    "--compiled",
    action="store_true",
    help="build only the wheels that cibuildwheel builds",
)
packages.add_arguments(parser)
args, cibuildwheel_args = parser.parse_known_args()

selected = packages.select(args)


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


if not args.compiled:
    for package in selected:
        if package.build is packages.Build.ANY:
            build_wheel(package)
        elif package.build is packages.Build.REDIST:
            for plat_name in PLAT_NAMES:
                os.environ["PYWINRT_PLAT_NAME"] = plat_name

                build_wheel(package)

            del os.environ["PYWINRT_PLAT_NAME"]

if not args.pure:
    for package in selected:
        if package.build is not packages.Build.COMPILED:
            continue

        subprocess.check_call(
            ["cibuildwheel", os.fspath(package.path)]
            + (cibuildwheel_args or ["--platform", "windows"])
        )

        # Build directories are 10s of MBs and will cause CI to run out of space!
        shutil.rmtree(package.path / "build")
