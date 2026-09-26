"""
The version every PyWinRT distribution is published with.

Nothing here builds or installs anything: the scheme is arithmetic on
strings, and what has to hold of it is that the strings a release writes sort
and resolve the way the packaging plan says they do. So the conversions are
checked against ``packaging``, which is what pip resolves with, rather than
against a second copy of the rules written out by hand.
"""

import ast
import tempfile
import textwrap
import unittest
from pathlib import Path
from unittest import mock

from packaging.specifiers import SpecifierSet
from packaging.version import Version

from scripts import versions


class NuGetToPep440(unittest.TestCase):
    """
    Converting an upstream NuGet version to the one PyWinRT publishes.
    """

    def test_release_version_is_unchanged(self):
        # both schemes spell a release the same way, however many segments it
        # has, so nothing about these is rewritten
        self.assertEqual(versions.nuget_to_pep440("2.8.7"), "2.8.7")
        self.assertEqual(versions.nuget_to_pep440("10.0.26100.4188"), "10.0.26100.4188")
        self.assertEqual(versions.nuget_to_pep440("1.7.250513003"), "1.7.250513003")

    def test_prerelease_labels_become_phases(self):
        self.assertEqual(versions.nuget_to_pep440("1.8.0-experimental1"), "1.8.0a1")
        self.assertEqual(versions.nuget_to_pep440("1.8.0-preview2"), "1.8.0b2")
        self.assertEqual(versions.nuget_to_pep440("1.8.0-prerelease.3"), "1.8.0rc3")

    def test_prerelease_label_without_a_number_is_the_zeroth(self):
        # PEP 440 reads a phase with no number as zero, so writing the zero
        # out changes nothing and leaves the version in its normal form
        self.assertEqual(versions.nuget_to_pep440("1.8.0-preview"), "1.8.0b0")
        self.assertEqual(Version("1.8.0b0"), Version("1.8.0b"))

    def test_prereleases_sort_the_way_upstream_ships_them(self):
        self.assertEqual(
            sorted(
                Version(versions.nuget_to_pep440(v))
                for v in [
                    "1.8.0",
                    "1.8.0-prerelease.1",
                    "1.8.0-experimental1",
                    "1.8.0-preview1",
                ]
            ),
            [Version(v) for v in ["1.8.0a1", "1.8.0b1", "1.8.0rc1", "1.8.0"]],
        )

    def test_an_unconvertible_version_is_refused(self):
        # a version PEP 440 reads differently from the way NuGet meant it
        # would sort wrongly on PyPI without anything saying so, so a spelling
        # this has not been taught stops the generation instead
        for version in ["1.8.0-alpha", "1.8.0-preview1+build7", "", "next"]:
            with self.subTest(version=version):
                with self.assertRaises(ValueError):
                    versions.nuget_to_pep440(version)


class CompatibilityGeneration(unittest.TestCase):
    """
    The one number that the ABI major, the table format major, the runtime's
    own major and the epoch of every other family all are.
    """

    def test_the_tree_agrees_on_it(self):
        # compatibility_generation() raises if the three sources disagree,
        # which is the whole of its job; this is the case that says the tree
        # in front of us is one of the ones it accepts
        generation = versions.compatibility_generation()

        self.assertEqual(Version(versions.runtime_version()).major, generation)
        self.assertEqual(versions.runtime_table_format()[0], generation)

    def test_every_family_carries_it_as_an_epoch(self):
        generation = versions.compatibility_generation()

        for family, version in versions.family_versions().items():
            with self.subTest(family=family):
                self.assertEqual(Version(version).epoch, generation)

    def test_the_epoch_sorts_above_everything_published_before_it(self):
        # 3.2.1 is the last release of the previous scheme, where a projection
        # package carried the generator's version and no epoch
        for version in versions.family_versions().values():
            with self.subTest(version=version):
                self.assertGreater(Version(version), Version("3.2.1"))

    def test_the_epoch_sorts_below_the_next_generation(self):
        generation = versions.compatibility_generation()

        for version in versions.family_versions().values():
            with self.subTest(version=version):
                self.assertLess(Version(version), Version(f"{generation + 1}!0"))


class RuntimeRequirement(unittest.TestCase):
    """
    The requirement every package that the runtime executes carries, and the
    only upper bound anywhere in PyWinRT's metadata.
    """

    def setUp(self):
        self.generation = versions.compatibility_generation()
        self.runtime_version = versions.runtime_version()
        self.specifier = SpecifierSet(
            versions.runtime_requirement().removeprefix("winrt-runtime")
        )

    def test_the_runtime_of_this_tree_satisfies_it(self):
        self.assertIn(self.runtime_version, self.specifier)

    def test_a_later_runtime_of_this_generation_satisfies_it(self):
        self.assertIn(f"{self.generation}.99.99", self.specifier)

    def test_an_earlier_runtime_does_not(self):
        self.assertNotIn("3.2.1", self.specifier)

    def test_the_next_generation_does_not(self):
        # the cap is here because this refusal is known rather than guessed: a
        # package of one generation is refused by the runtime of the next, and
        # without it pip would upgrade the runtime on its own into that error
        self.assertNotIn(f"{self.generation + 1}.0.0", self.specifier)


class TableCompilerRequirement(unittest.TestCase):
    """
    What a projection package asks of the compiler that builds its tables.

    It is a build requirement, so it never reaches a wheel, and it is the
    only requirement in PyWinRT's metadata whose floor is not the version of
    the thing in this tree.
    """

    def setUp(self):
        self.generation = versions.compatibility_generation()
        self.specifier = SpecifierSet(
            versions.table_compiler_requirement().removeprefix("winrt-table-compiler")
        )

    def test_the_compiler_of_this_tree_satisfies_it(self):
        self.assertIn(versions.table_compiler_version(), self.specifier)

    def test_the_floor_is_the_generation_and_not_the_runtime_version(self):
        # a compiler writes the table format, which does not move when the
        # runtime is patched, so a runtime fix must not oblige us to publish
        # a compiler with nothing in it changed
        self.assertIn(f"{self.generation}.0.0", self.specifier)

    def test_the_previous_generation_does_not(self):
        self.assertNotIn(f"{self.generation - 1}.99.99", self.specifier)

    def test_the_next_generation_does_not(self):
        # the same known incompatibility the runtime requirement is capped
        # for: that compiler writes a table this generation cannot read
        self.assertNotIn(f"{self.generation + 1}.0.0", self.specifier)


class FamilyRequirement(unittest.TestCase):
    """
    What one package says about another that its types name.
    """

    def setUp(self):
        self.version = versions.family_versions()["winrt"]
        self.pinned = SpecifierSet(f"~={self.version}.0")
        self.floor = SpecifierSet(f">={self.version}")

    def test_the_pin_admits_the_version_it_was_written_for(self):
        self.assertIn(self.version, self.pinned)

    def test_the_pin_admits_a_re_release_of_that_version(self):
        # .postN is PyWinRT re-releasing one upstream version, which is why
        # #137 rejected == for these
        self.assertIn(f"{self.version}.post1", self.pinned)

    def test_the_pin_refuses_the_next_upstream_version(self):
        upstream = Version(self.version)
        later = f"{upstream.epoch}!{upstream.release[0] + 1}.0"

        self.assertNotIn(later, self.pinned)

    def test_the_floor_admits_the_next_upstream_version(self):
        upstream = Version(self.version)
        later = f"{upstream.epoch}!{upstream.release[0] + 1}.0"

        self.assertIn(later, self.floor)

    def test_the_floor_refuses_the_previous_generation(self):
        # an epoch is what keeps the two apart without an upper bound
        self.assertNotIn("3.2.1", self.floor)


class ReleaseTags(unittest.TestCase):
    """
    The tag that releases one unit.

    A unit is a family of generated packages or a single package written by
    hand, so the tag has to say which one it releases and at which version,
    and the workflow has to be able to read the first back out of it and
    check the second against the tree.
    """

    def test_every_published_unit_has_one(self):
        self.assertEqual(
            sorted(versions.release_tags()), sorted(versions.published_versions())
        )

    def test_the_runtime_is_released_before_anything_that_requires_it(self):
        # the order published_versions() is in is the order a release goes
        # in, and every other package depends on winrt-runtime
        self.assertEqual(
            next(iter(versions.published_versions())), versions.RUNTIME_FAMILY
        )

    def test_a_package_written_by_hand_is_versioned_without_an_epoch(self):
        # its version says what changed in that package rather than which
        # upstream release it was generated from, so there is no epoch to
        # sort above - only the 3.x versions it follows on PyPI
        for path in versions.version_files():
            with self.subTest(unit=path.parent.name):
                version = Version(versions.read_version(path))

                self.assertEqual(version.epoch, 0)
                self.assertGreater(version, Version("3.2.1"))

    def test_every_version_written_by_hand_is_of_this_generation(self):
        generation = versions.compatibility_generation()

        for path in versions.version_files():
            with self.subTest(unit=path.parent.name):
                self.assertEqual(Version(versions.read_version(path)).major, generation)

    def test_the_test_component_is_not_released(self):
        self.assertIn("test-winrt", versions.family_versions())
        self.assertNotIn("test-winrt", versions.published_versions())

    def test_the_tag_carries_the_version_the_unit_is_published_with(self):
        for unit, version in versions.published_versions().items():
            with self.subTest(unit=unit):
                tag = versions.release_tags()[unit]

                self.assertTrue(tag.startswith(f"{versions.TAG_PREFIX}/{unit}/"))
                self.assertEqual(Version(self.spelled_version(tag)), Version(version))

    def test_the_epoch_is_spelled_without_a_character_a_shell_would_eat(self):
        for unit, tag in versions.release_tags().items():
            with self.subTest(unit=unit):
                self.assertNotIn("!", tag.rpartition("/")[2])

    @staticmethod
    def spelled_version(tag: str) -> str:
        """
        The PEP 440 version that a release tag spells as [e<epoch>]v<version>.
        """
        epoch, _, release = tag.rpartition("/")[2].partition("v")

        return f"{epoch.removeprefix('e')}!{release}" if epoch else release

    def test_a_tag_names_the_unit_it_releases(self):
        for unit, tag in versions.release_tags().items():
            with self.subTest(unit=unit):
                self.assertEqual(versions.unit_of_tag(tag), unit)

    def test_a_tag_at_a_version_this_tree_does_not_publish_is_refused(self):
        # the point of matching rather than parsing: a tag pushed at the
        # wrong commit, or copied from another unit, releases nothing
        with self.assertRaises(ValueError):
            versions.unit_of_tag("wheels/runtime/v1.2.3")

    def test_a_tag_that_names_no_unit_is_refused(self):
        with self.assertRaises(ValueError):
            versions.unit_of_tag("wheels/v3.2.1")


class InteropRuntimeRequirement(unittest.TestCase):
    """
    What an interop package requires of the runtime: the newest runtime that a
    function it calls first shipped in, so that a fix to it does not force a
    runtime upgrade on someone who keeps an older one.
    """

    def setUp(self):
        self.generation = versions.compatibility_generation()

    def test_calling_nothing_new_requires_the_generation(self):
        self.assertEqual(
            versions.interop_runtime_requirement(["as_interface"]),
            f"winrt-runtime>={self.generation}.0.0,<{self.generation + 1}",
        )

    def test_the_newest_function_it_calls_sets_the_floor(self):
        functions = {
            "older": f"{self.generation}.1.0",
            "newer": f"{self.generation}.2.0",
        }

        with (
            mock.patch.dict(versions.RUNTIME_FUNCTIONS, functions),
            mock.patch.object(
                versions, "runtime_version", return_value=f"{self.generation}.3.0"
            ),
        ):
            requirement = versions.interop_runtime_requirement(["older", "newer"])

        specifier = SpecifierSet(requirement.removeprefix("winrt-runtime"))

        self.assertIn(f"{self.generation}.2.0", specifier)
        self.assertNotIn(f"{self.generation}.1.0", specifier)

    def test_an_unlisted_function_is_refused(self):
        with self.assertRaises(RuntimeError):
            versions.interop_runtime_requirement(["not_a_runtime_function"])

    def test_a_function_newer_than_this_tree_is_refused(self):
        # listing it with the version it will ship in, before the runtime
        # carries that version, would require a runtime nobody can install
        with mock.patch.dict(
            versions.RUNTIME_FUNCTIONS, {"unreleased": f"{self.generation}.99.0"}
        ):
            with self.assertRaises(RuntimeError):
                versions.interop_runtime_requirement(["unreleased"])

    def test_every_listed_function_is_one_the_runtime_has(self):
        stub = versions.RUNTIME_PATH / "python" / "winrt" / "_winrt.pyi"
        declared = {
            node.name
            for node in ast.parse(stub.read_text(encoding="utf-8")).body
            if isinstance(node, ast.FunctionDef)
        }

        self.assertLessEqual(set(versions.RUNTIME_FUNCTIONS), declared)

    def test_every_way_of_importing_the_runtime_is_seen(self):
        with tempfile.TemporaryDirectory() as root:
            module = Path(root) / "winrt" / "a" / "interop" / "__init__.py"
            module.parent.mkdir(parents=True)
            module.write_text(
                textwrap.dedent(
                    """
                    import winrt._winrt
                    import winrt._winrt as _runtime
                    from winrt import _winrt
                    from winrt import _winrt as _other
                    from winrt._winrt import direct

                    winrt._winrt.plain()
                    _runtime.aliased()
                    _winrt.from_winrt()
                    _other.from_winrt_aliased()
                    """
                ),
                encoding="utf-8",
            )

            called = versions.runtime_functions_called(Path(root))

        self.assertLessEqual(
            {"plain", "aliased", "from_winrt", "from_winrt_aliased", "direct"},
            called,
        )
        # interop.h raises through hresult_error() in every module
        self.assertIn("hresult_error", called)

    def test_every_interop_package_calls_only_listed_functions(self):
        for package in sorted(versions.INTEROP_PATH.glob("winrt-*")):
            with self.subTest(package=package.name):
                self.assertLessEqual(
                    versions.runtime_functions_called(package),
                    set(versions.RUNTIME_FUNCTIONS),
                )
