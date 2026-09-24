"""
The version every PyWinRT distribution is published with.

Nothing here builds or installs anything: the scheme is arithmetic on
strings, and what has to hold of it is that the strings a release writes sort
and resolve the way the packaging plan says they do. So the conversions are
checked against ``packaging``, which is what pip resolves with, rather than
against a second copy of the rules written out by hand.
"""

import unittest

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

    def test_the_table_compiler_is_capped_the_same_way(self):
        self.assertEqual(
            versions.table_compiler_requirement().removeprefix("winrt-table-compiler"),
            versions.runtime_requirement().removeprefix("winrt-runtime"),
        )


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
