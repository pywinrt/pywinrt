"""Builds the source distribution of every package that is published.

    python scripts/build-sdist.py
    python scripts/build-sdist.py --family webview2
    python scripts/build-sdist.py --package winrt-Windows.Foundation

A source distribution has to build on a machine that has nothing but PyPI, so
nothing here tells a build where to find something in this checkout: the
C++/WinRT headers that winrt-runtime and the interop modules compile against
ride inside the packages that include them, and a projection package's only
build step is compiling its tables, which the hatchling hook that
winrt-table-compiler ships does.

A package that redistributes a Microsoft binary carries no copy of it, since
nothing prebuilt belongs in a source distribution; its build fetches the NuGet
package instead, and an environment variable names an unpacked copy for a
machine that cannot reach nuget.org. The variables are set below so that
building here never goes to the network.
"""

import argparse
import os
import subprocess

# the packages that are published, and the filters that pick some of them
import packages

packages.set_nuget_environment()

parser = argparse.ArgumentParser(
    description="Builds the source distribution of every package that is published."
)
packages.add_arguments(parser)
args = parser.parse_args()

for package in packages.select(args):
    subprocess.check_call(
        [
            "pyproject-build",
            "--sdist",
            "--no-isolation",
            "--skip-dependency-check",
            "--outdir",
            "wheelhouse",
            os.fspath(package.path),
        ],
    )
