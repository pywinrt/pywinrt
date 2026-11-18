"""
The sections of a changelog.

Every changelog in this tree keeps the Keep a Changelog layout: a heading per
release, under one for what is not released yet when there is any, and the
definitions of the reference links at the end of the file.
"""

import re

# What ends a section: the next heading, or the link definitions at the bottom
# of the file.
SECTION_END = re.compile(r"^## \[|^\[[^\]]+\]: ", re.MULTILINE)


def section_end(text: str, pos: int) -> int:
    """
    Where the next section, or the link definitions, start in @p text at or
    after @p pos, or the end of @p text when neither does.
    """
    end = SECTION_END.search(text, pos)

    return end.start() if end else len(text)


def section(text: str, heading: re.Pattern[str]) -> tuple[int, int] | None:
    """
    Where in @p text the section under the first heading that @p heading
    matches is, without the heading: from the end of the match to the end of
    the section. None when @p heading matches nothing.
    """
    match = heading.search(text)

    if match is None:
        return None

    return match.end(), section_end(text, match.end())
