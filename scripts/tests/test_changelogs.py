"""
Finding a section of a changelog.
"""

import re
import unittest

import changelogs

CHANGELOG = """# Changelog

## [Unreleased]

### Fixed
- A fix.

## [1.0.0] - 2026-01-01

### Added
- A feature.

[Unreleased]: https://example.com/compare
[1.0.0]: https://example.com/1.0.0
"""

UNRELEASED = re.compile(r"^## \[Unreleased\]\n", re.MULTILINE)
RELEASE = re.compile(r"^## \[1\.0\.0\] - .*\n", re.MULTILINE)


class Section(unittest.TestCase):
    def test_ends_at_the_next_heading(self) -> None:
        span = changelogs.section(CHANGELOG, UNRELEASED)

        assert span is not None
        self.assertEqual(CHANGELOG[slice(*span)], "\n### Fixed\n- A fix.\n\n")

    def test_the_last_ends_at_the_link_definitions(self) -> None:
        span = changelogs.section(CHANGELOG, RELEASE)

        assert span is not None
        self.assertEqual(CHANGELOG[slice(*span)], "\n### Added\n- A feature.\n\n")

    def test_without_definitions_ends_at_the_end(self) -> None:
        text = "## [1.0.0] - 2026-01-01\n\n- A feature.\n"
        span = changelogs.section(text, RELEASE)

        self.assertEqual(span, (len("## [1.0.0] - 2026-01-01\n"), len(text)))

    def test_no_heading_is_none(self) -> None:
        self.assertIsNone(changelogs.section("# Changelog\n", UNRELEASED))

    def test_end_from_the_top_is_the_first_heading(self) -> None:
        self.assertEqual(
            changelogs.section_end(CHANGELOG, 0), CHANGELOG.index("## [Unreleased]")
        )
