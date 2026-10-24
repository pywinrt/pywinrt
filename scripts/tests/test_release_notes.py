"""
What scripts/release-notes.py prints for a release tag.

The notes are cut out of a changelog written to a temporary directory, and a
tag is resolved against a version table put in place of the tree's, so
nothing here depends on what the tree publishes.
"""

import contextlib
import importlib.util
import io
import sys
import tempfile
import textwrap
import unittest
from pathlib import Path
from types import ModuleType
from unittest import mock

SCRIPTS = Path(__file__).parent.parent
SCRIPT = SCRIPTS / "release-notes.py"


def load_script() -> ModuleType:
    spec = importlib.util.spec_from_file_location("release_notes", SCRIPT)
    assert spec is not None and spec.loader is not None
    module = importlib.util.module_from_spec(spec)

    # the script imports versions.py beside it by its bare name
    sys.path.insert(0, str(SCRIPTS))

    try:
        spec.loader.exec_module(module)
    finally:
        sys.path.remove(str(SCRIPTS))

    return module


release_notes = load_script()
versions = release_notes.versions

CHANGELOG = textwrap.dedent(
    """\
    # Changelog

    What changed in `winrt-example`.

    ## [4.0.2] - 2026-10-24

    ### Fixed
    - Fixed [one thing][one], then [another][two].

    ## [4.0.1] - 2026-10-20

    ### Fixed
    - Fixed something that links to nothing.

    ## [4.0.0] - 2026-10-18

    See the [4.0 release notes][4.0] for the whole of it.

    ### Added
    - Everything, as the [4.0 release notes][4.0] say.

    [4.0]: https://example.test/4.0/
    [one]: https://example.test/one
    [two]: https://example.test/two
    [unused]: https://example.test/unused
    """
)

# what the tree publishes, as far as a tag is concerned
PUBLISHED = {
    "runtime": "4.0.2",
    "winrt": "4!10.0.1.2",
    "winui2": "4!2.8.7.1",
    "winrt-Windows.System.Interop": "4.0.0",
}


class Notes(unittest.TestCase):
    """
    The section of a changelog that a version heads.
    """

    def setUp(self) -> None:
        directory = Path(self.enterContext(tempfile.TemporaryDirectory()))
        self.changelog = directory / "CHANGELOG.md"
        self.changelog.write_text(CHANGELOG, encoding="utf-8")

    def test_the_section_is_the_one_the_version_heads(self) -> None:
        self.assertEqual(
            release_notes.notes(self.changelog, "4.0.1"),
            "### Fixed\n- Fixed something that links to nothing.\n",
        )

    def test_the_section_ends_at_the_next_heading(self) -> None:
        notes = release_notes.notes(self.changelog, "4.0.2")

        self.assertTrue(notes.startswith("### Fixed\n"))
        self.assertNotIn("4.0.1", notes)
        self.assertNotIn("links to nothing", notes)

    def test_the_last_section_ends_at_the_link_definitions(self) -> None:
        notes = release_notes.notes(self.changelog, "4.0.0")

        self.assertTrue(notes.startswith("See the [4.0 release notes][4.0]"))
        self.assertNotIn("[one]:", notes)
        self.assertNotIn("[unused]:", notes)

    def test_the_reference_links_used_carry_their_definitions(self) -> None:
        self.assertEqual(
            release_notes.notes(self.changelog, "4.0.2"),
            "### Fixed\n"
            "- Fixed [one thing][one], then [another][two].\n"
            "\n"
            "[one]: https://example.test/one\n"
            "[two]: https://example.test/two\n",
        )

    def test_a_link_used_twice_is_defined_once(self) -> None:
        notes = release_notes.notes(self.changelog, "4.0.0")

        self.assertEqual(notes.count("[4.0]: https://example.test/4.0/"), 1)
        self.assertTrue(notes.endswith("\n\n[4.0]: https://example.test/4.0/\n"))

    def test_a_section_without_reference_links_has_no_definitions(self) -> None:
        self.assertNotIn("]: ", release_notes.notes(self.changelog, "4.0.1"))

    def test_a_version_without_a_section_is_refused(self) -> None:
        with self.assertRaisesRegex(SystemExit, "no section for 4.0.3"):
            release_notes.notes(self.changelog, "4.0.3")


class ReleaseOf(unittest.TestCase):
    """
    What a tag releases: its title, its version and where its changelog is.
    """

    def setUp(self) -> None:
        self.enterContext(
            mock.patch.object(versions, "published_versions", return_value=PUBLISHED)
        )

    def test_a_family_is_titled_after_its_upstream(self) -> None:
        self.assertEqual(
            release_notes.release_of("wheels/winrt/e4v10.0.1.2"),
            (
                "Windows SDK packages",
                "4!10.0.1.2",
                versions.REPO_PATH / "projection" / "winrt",
            ),
        )

    def test_a_family_of_one_package_is_titled_after_the_package(self) -> None:
        self.assertEqual(
            release_notes.release_of("wheels/winui2/e4v2.8.7.1"),
            (
                "winui2-Microsoft.UI.Xaml",
                "4!2.8.7.1",
                versions.REPO_PATH / "projection" / "winui2",
            ),
        )

    def test_a_hand_written_package_is_titled_after_its_distribution(self) -> None:
        self.assertEqual(
            release_notes.release_of("wheels/runtime/v4.0.2"),
            ("winrt-runtime", "4.0.2", versions.REPO_PATH / "runtime"),
        )

    def test_an_interop_module_is_titled_by_its_name(self) -> None:
        self.assertEqual(
            release_notes.release_of("wheels/winrt-Windows.System.Interop/v4.0.0"),
            (
                "winrt-Windows.System.Interop",
                "4.0.0",
                versions.INTEROP_PATH / "winrt-Windows.System.Interop",
            ),
        )

    def test_the_nuget_tag_is_taken_at_its_word(self) -> None:
        # the tool carries no version in the tree, so no table says what
        # the tag publishes
        self.assertEqual(
            release_notes.release_of("nuget/PyWinRT/v4.1.0"),
            ("PyWinRT", "4.1.0", versions.REPO_PATH / "PyWinRT"),
        )

    def test_a_tag_the_tree_does_not_publish_is_refused(self) -> None:
        # the version in the tag is matched against the table, not parsed
        with self.assertRaisesRegex(SystemExit, "releases nothing") as refused:
            release_notes.release_of("wheels/runtime/v4.0.0")

        # a status that is not a number is what Python reports as exit code 1
        self.assertNotIsInstance(refused.exception.code, int)
        self.assertIn("wheels/runtime/v4.0.0", str(refused.exception.code))


class Main(unittest.TestCase):
    """
    The command line, over the table above and a changelog in a temporary
    tree.
    """

    def setUp(self) -> None:
        root = Path(self.enterContext(tempfile.TemporaryDirectory()))
        (root / "runtime").mkdir()
        (root / "runtime" / "CHANGELOG.md").write_text(CHANGELOG, encoding="utf-8")
        self.enterContext(mock.patch.object(versions, "REPO_PATH", root))
        self.enterContext(
            mock.patch.object(versions, "published_versions", return_value=PUBLISHED)
        )

    def run_script(self, *args: str) -> str:
        output = io.StringIO()

        with (
            mock.patch.object(sys, "argv", ["release-notes.py", *args]),
            contextlib.redirect_stdout(output),
        ):
            release_notes.main()

        return output.getvalue()

    def test_the_notes_are_the_tags_section(self) -> None:
        self.assertEqual(
            self.run_script("wheels/runtime/v4.0.2"),
            "### Fixed\n"
            "- Fixed [one thing][one], then [another][two].\n"
            "\n"
            "[one]: https://example.test/one\n"
            "[two]: https://example.test/two\n",
        )

    def test_the_title_is_the_unit_and_its_version(self) -> None:
        self.assertEqual(
            self.run_script("--title", "wheels/runtime/v4.0.2"),
            "winrt-runtime 4.0.2\n",
        )

    def test_an_unknown_tag_fails_with_and_without_title(self) -> None:
        for args in (["wheels/runtime/v4.0.0"], ["--title", "wheels/runtime/v4.0.0"]):
            with self.subTest(args=args):
                with self.assertRaisesRegex(SystemExit, "releases nothing") as refused:
                    self.run_script(*args)

                self.assertNotIsInstance(refused.exception.code, int)
