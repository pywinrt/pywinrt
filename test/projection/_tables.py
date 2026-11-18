"""
What the tests of the projection tables share: the flag bits of a
table's records, and reading an installed table with the runtime's own
reader.
"""

import pathlib
import sys
import unittest
from typing import Any

import winrt._winrt

# The category in the low bits of a type record's flags.
CATEGORY_ENUM = 0
CATEGORY_STRUCT = 1
CATEGORY_INTERFACE = 2
CATEGORY_CLASS = 3
CATEGORY_DELEGATE = 4

TYPE_EXTERNAL = 1 << 3
TYPE_PARAMETERIZED = 1 << 6
TYPE_CONCRETE = 1 << 7
TYPE_DEFAULT_ACTIVATABLE = 1 << 8
TYPE_PYTHON_TYPE = 1 << 9
TYPE_ITERABLE = 1 << 10
TYPE_ITERATOR = 1 << 11
TYPE_SEQUENCE = 1 << 12
TYPE_MUTABLE_SEQUENCE = 1 << 13
TYPE_MAPPING = 1 << 14
TYPE_MUTABLE_MAPPING = 1 << 15
TYPE_AWAITABLE = 1 << 16
TYPE_CLOSEABLE = 1 << 17
TYPE_STRINGABLE = 1 << 18
TYPE_FLAGS_ENUM = 1 << 22

NO_REF = 0xFFFFFFFF


def find_table(*parts: str) -> bytes:
    """
    Reads an installed table by the path of its namespace package.

    The projection packages cannot be imported yet, so the file is found the way
    the import system would find it, by walking sys.path.
    """
    for entry in sys.path:
        path = pathlib.Path(entry).joinpath(*parts, "_table.pywinrt")

        if path.is_file():
            return path.read_bytes()

    raise unittest.SkipTest(f"no {'/'.join(parts)} table on sys.path")


def read(*parts: str) -> dict[str, Any]:
    return winrt._winrt.read_table(find_table(*parts))


def defined(table: dict[str, Any]) -> dict[str, dict[str, Any]]:
    """The types the namespace defines, by WinRT name."""
    return {
        t["name"]: t
        for t in table["types"]
        if not t["flags"] & TYPE_EXTERNAL and not t["flags"] & TYPE_CONCRETE
    }
