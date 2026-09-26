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


def runtime_requirements(lines: list[str]) -> list[Requirement]:
    requirements = [Requirement(line) for line in lines]

    return [r for r in requirements if r.name == "winrt-runtime"]


class InteropBuildRequirement(unittest.TestCase):
    """
    What an interop package needs of winrt-runtime to build from its sdist.
    """

    def test_it_is_the_range_it_installs_with_or_nothing(self):
        for package in sorted(versions.INTEROP_PATH.glob("winrt-*")):
            with self.subTest(package=package.name):
                pyproject = tomllib.loads(
                    (package / "pyproject.toml").read_text(encoding="utf-8")
                )
                requirements = (package / "requirements.txt").read_text(
                    encoding="utf-8"
                )
                build = runtime_requirements(pyproject["build-system"]["requires"])
                (installed,) = runtime_requirements(
                    [
                        line
                        for line in requirements.splitlines()
                        if line and not line.startswith("#")
                    ]
                )

                # only a module that compiles against the C++/WinRT headers
                # the runtime carries needs the runtime to build
                if "winrt._include" in (package / "setup.py").read_text(
                    encoding="utf-8"
                ):
                    self.assertEqual(build, [installed])
                else:
                    self.assertEqual(build, [])
                    self.assertNotIn("environment", pyproject["tool"]["cibuildwheel"])
                    self.assertNotIn(
                        "build-frontend", pyproject["tool"]["cibuildwheel"]
                    )
