"""
The packaging that scripts/generate-pyproject.py writes.

What is checked is the committed output rather than the script, which writes
the whole tree when it runs: an sdist is built from these files, and once one
is on PyPI they cannot be changed.
"""

import ast
import re
import tomllib
import unittest

from packaging.requirements import Requirement

from scripts import versions


class InteropBuildRequirement(unittest.TestCase):
    """
    What an interop package needs of winrt-runtime to build from its sdist.
    """

    def test_no_interop_package_build_requires_winrt_runtime(self) -> None:
        for package in sorted(versions.INTEROP_PATH.glob("winrt-*")):
            with self.subTest(package=package.name):
                pyproject = tomllib.loads(
                    (package / "pyproject.toml").read_text(encoding="utf-8")
                )
                build = [Requirement(r) for r in pyproject["build-system"]["requires"]]

                self.assertNotIn("winrt-runtime", [r.name for r in build])
                self.assertNotIn("environment", pyproject["tool"]["cibuildwheel"])
                self.assertNotIn("build-frontend", pyproject["tool"]["cibuildwheel"])


class RuntimeSources(unittest.TestCase):
    """
    The source files winrt-runtime is compiled from.

    CMake and setup.py each list them, and a file that only CMake lists still
    builds and passes the tests while the wheel and the sdist fail to link.
    """

    def test_setup_py_and_cmake_compile_every_source_file(self) -> None:
        on_disk = sorted(
            f.relative_to(versions.RUNTIME_PATH).as_posix()
            for f in (versions.RUNTIME_PATH / "src").glob("*.cpp")
        )

        setup_py = ast.parse(
            (versions.RUNTIME_PATH / "setup.py").read_text(encoding="utf-8")
        )
        sources = next(
            k.value
            for n in ast.walk(setup_py)
            if isinstance(n, ast.Call)
            for k in n.keywords
            if k.arg == "sources"
        )
        self.assertEqual(sorted(ast.literal_eval(sources)), on_disk)

        cmake = (versions.RUNTIME_PATH / "CMakeLists.txt").read_text(encoding="utf-8")
        match = re.search(r"set\(WINRT_RUNTIME_SOURCES\s+(.*?)\)", cmake, re.DOTALL)
        assert match is not None
        self.assertEqual(
            sorted(
                s.strip('"').replace("${WINRT_RUNTIME_SRC_PATH}", "src")
                for s in match[1].split()
            ),
            on_disk,
        )


class StubImports(unittest.TestCase):
    """
    The distributions that the stubs of a projection package import.

    A class's stub names its base class and the interfaces it implements as
    its bases, so a distribution the stubs import that the package neither
    depends on nor offers in its [all] extra leaves a base that a type checker
    cannot resolve, and the whole class unknown to it.
    """

    def test_every_imported_distribution_is_a_dependency(self) -> None:
        projection = versions.REPO_PATH / "projection"
        packages = sorted(p.parent for p in projection.glob("*/*/pyproject.toml"))

        # Each module's distribution, from where its stub is.
        distribution_of = {
            ".".join(stub.parent.relative_to(package).parts): package.name
            for package in packages
            for stub in package.rglob("__init__.pyi")
        }

        for package in packages:
            with self.subTest(package=package.name):
                project = tomllib.loads(
                    (package / "pyproject.toml").read_text(encoding="utf-8")
                )["project"]
                offered = {
                    Requirement(r).name
                    for r in project["dependencies"]
                    + project.get("optional-dependencies", {}).get("all", [])
                }

                imported = {
                    distribution_of[module]
                    for stub in package.rglob("__init__.pyi")
                    for module in re.findall(
                        r"^import ([\w.]+) as \w+$",
                        stub.read_text(encoding="utf-8"),
                        re.MULTILINE,
                    )
                    if module in distribution_of
                }

                self.assertLessEqual(imported - {package.name}, offered)
