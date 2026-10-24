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
from pathlib import Path

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


class Changelogs(unittest.TestCase):
    """
    The changelog that each package's Changelog link on PyPI points at.

    Each release unit keeps one in its directory, and every package it releases
    links that one, so a reader following the link finds what changed in the
    version they installed. A link to a file that is not in the tree stays dead
    on PyPI for as long as the version that carries it is published.
    """

    LINK = "https://github.com/pywinrt/pywinrt/blob/main/"

    def changelog_link(self, package: Path) -> str:
        pyproject = tomllib.loads(
            (package / "pyproject.toml").read_text(encoding="utf-8")
        )
        link: str = pyproject["project"]["urls"]["Changelog"]

        self.assertTrue(link.startswith(self.LINK), link)

        return link.removeprefix(self.LINK)

    def test_every_release_unit_has_a_changelog(self) -> None:
        for unit in versions.published_versions():
            with self.subTest(unit=unit):
                if unit in (versions.RUNTIME_PATH.name, versions.TABLE_PATH.name):
                    path = versions.REPO_PATH / unit
                elif (versions.REPO_PATH / "projection" / unit).is_dir():
                    path = versions.REPO_PATH / "projection" / unit
                else:
                    path = versions.INTEROP_PATH / unit

                self.assertTrue((path / "CHANGELOG.md").is_file())

    def test_a_package_links_the_changelog_of_the_unit_that_releases_it(
        self,
    ) -> None:
        expected = {
            versions.RUNTIME_PATH: "runtime/CHANGELOG.md",
            versions.TABLE_PATH: "table/CHANGELOG.md",
        }

        for package in sorted(versions.INTEROP_PATH.glob("winrt-*")):
            family = versions.FAMILY_RELEASED_INTEROP.get(package.name)
            expected[package] = (
                f"projection/{family}/CHANGELOG.md"
                if family
                else f"interop/{package.name}/CHANGELOG.md"
            )

        for package in sorted(
            p.parent
            for p in (versions.REPO_PATH / "projection").glob("*/*/pyproject.toml")
        ):
            family = package.parent.name
            expected[package] = (
                "CHANGELOG.md"
                if family in versions.UNPUBLISHED_FAMILIES
                else f"projection/{family}/CHANGELOG.md"
            )

        for package, changelog in expected.items():
            with self.subTest(package=package.name):
                self.assertEqual(self.changelog_link(package), changelog)
                self.assertTrue((versions.REPO_PATH / changelog).is_file())

    def test_a_redistributed_dll_links_the_changelog_of_its_family(self) -> None:
        # it is released with the family of the projection that imports it
        for package in sorted((versions.REPO_PATH / "redist").glob("winrt-*")):
            with self.subTest(package=package.name):
                changelog = Path(self.changelog_link(package))

                self.assertEqual(changelog.parent.parent.name, "projection")
                self.assertTrue((versions.REPO_PATH / changelog).is_file())


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


class NestedStructs(unittest.TestCase):
    """
    The distributions that define the structs nested in a package's structs.

    A struct field of a struct is resolved when the struct's type is built,
    as its module is imported, which imports the distribution that defines
    the nested struct, so that has to be installed rather than offered in the
    [all] extra.
    """

    def test_every_nested_struct_is_a_dependency(self) -> None:
        projection = versions.REPO_PATH / "projection"
        packages = sorted(p.parent for p in projection.glob("*/*/pyproject.toml"))

        tables = {
            package: list(package.rglob("_table.pywinrt.txt")) for package in packages
        }

        # Each namespace's distribution, from the table that projects it.
        distribution_of = {
            namespace: package.name
            for package, paths in tables.items()
            for path in paths
            for namespace in re.findall(
                r"^namespace ([\w.]+)$",
                path.read_text(encoding="utf-8"),
                re.MULTILINE,
            )
        }

        for package, paths in tables.items():
            with self.subTest(package=package.name):
                project = tomllib.loads(
                    (package / "pyproject.toml").read_text(encoding="utf-8")
                )["project"]
                required = {Requirement(r).name for r in project["dependencies"]}

                nested = {
                    distribution_of[namespace]
                    for path in paths
                    for namespace in re.findall(
                        r"^    field \w+ \w+ struct type=([\w.]+)\.\w+$",
                        path.read_text(encoding="utf-8"),
                        re.MULTILINE,
                    )
                    if namespace in distribution_of
                }

                self.assertLessEqual(nested - {package.name}, required)
