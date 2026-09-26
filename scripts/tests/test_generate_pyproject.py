"""
The packaging that scripts/generate-pyproject.py writes.

What is checked is the committed output rather than the script, which writes
the whole tree when it runs: an sdist is built from these files, and once one
is on PyPI they cannot be changed.
"""

import tomllib
import unittest

from packaging.requirements import Requirement

from scripts import versions


class InteropBuildRequirement(unittest.TestCase):
    """
    What an interop package needs of winrt-runtime to build from its sdist.
    """

    def test_no_interop_package_build_requires_winrt_runtime(self):
        for package in sorted(versions.INTEROP_PATH.glob("winrt-*")):
            with self.subTest(package=package.name):
                pyproject = tomllib.loads(
                    (package / "pyproject.toml").read_text(encoding="utf-8")
                )
                build = [Requirement(r) for r in pyproject["build-system"]["requires"]]

                self.assertNotIn("winrt-runtime", [r.name for r in build])
                self.assertNotIn("environment", pyproject["tool"]["cibuildwheel"])
                self.assertNotIn("build-frontend", pyproject["tool"]["cibuildwheel"])
