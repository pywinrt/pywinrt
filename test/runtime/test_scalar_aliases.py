import struct
import typing
import unittest
import warnings

from winrt.system import (
    Array,
    BufferFormat,
    Char16,
    Double,
    Int8,
    Int16,
    Int32,
    Int64,
    Single,
    StructFormat,
    UInt8,
    UInt16,
    UInt32,
    UInt64,
    WinrtSignature,
)

# TypeForm is newer than the typing_extensions winrt-runtime requires, so it
# is only named in an annotation that is never evaluated.
if typing.TYPE_CHECKING:
    from typing_extensions import TypeForm

#: Every winrt.system scalar alias with the three formats it is annotated
#: with - buffer, struct and WinRT signature - and the WinRT type it names.
SCALAR_ALIASES: "list[tuple[TypeForm[typing.Any], str, str, str, str]]" = [
    (Int8, "b", "b", "i1", "Int8"),
    (UInt8, "B", "B", "u1", "UInt8"),
    (Int16, "h", "h", "i2", "Int16"),
    (UInt16, "H", "H", "u2", "UInt16"),
    (Int32, "i", "i", "i4", "Int32"),
    (UInt32, "I", "I", "u4", "UInt32"),
    (Int64, "q", "q", "i8", "Int64"),
    (UInt64, "Q", "Q", "u8", "UInt64"),
    (Single, "f", "f", "f4", "Single"),
    (Double, "d", "d", "f8", "Double"),
    (Char16, "u", "H", "c2", "Char16"),
]

#: Every format string Array takes, with the WinRT type it names. Passing one
#: is deprecated, so this is the list of spellings that have to go on working
#: until they are removed.
FORMAT_STRINGS = [
    ("?", "Boolean"),
    ("b", "Int8"),
    ("B", "UInt8"),
    ("h", "Int16"),
    ("H", "UInt16"),
    ("i", "Int32"),
    ("I", "UInt32"),
    ("q", "Int64"),
    ("Q", "UInt64"),
    ("f", "Single"),
    ("d", "Double"),
    ("u", "Char16"),
]


class TestScalarAliases(unittest.TestCase):
    """
    The winrt.system aliases that name the scalars no Python type names, and
    the three annotations each one carries.
    """

    def test_array_of_each_alias(self) -> None:
        for alias, buffer_format, _, _, winrt_name in SCALAR_ALIASES:
            with self.subTest(winrt_type=winrt_name):
                a: Array[typing.Any] = Array(alias, 3)

                self.assertEqual(a._winrt_element_type_name_, winrt_name)

                with memoryview(a) as m:
                    self.assertEqual(m.format, buffer_format)

    def test_buffer_format_comes_first(self) -> None:
        # Code written against the single string these aliases used to carry
        # reads the first annotation by position, so the buffer format is the
        # one that has to come first.
        for alias, buffer_format, _, _, winrt_name in SCALAR_ALIASES:
            with self.subTest(winrt_type=winrt_name):
                self.assertEqual(typing.get_args(alias)[1], buffer_format)

    def test_annotations_are_found_by_type(self) -> None:
        for alias, buffer_format, struct_format, signature, name in SCALAR_ALIASES:
            with self.subTest(winrt_type=name):
                annotations = typing.get_args(alias)[1:]

                for kind, expected in (
                    (BufferFormat, buffer_format),
                    (StructFormat, struct_format),
                    (WinrtSignature, signature),
                ):
                    found = [a for a in annotations if isinstance(a, kind)]
                    self.assertEqual(found, [expected])

    def test_struct_format_matches_the_element(self) -> None:
        # The struct module has no "u" format at all, which is why Char16 is
        # the one alias whose struct format is not its buffer format.
        for alias, _, struct_format, _, winrt_name in SCALAR_ALIASES:
            with self.subTest(winrt_type=winrt_name):
                with memoryview(Array(alias, 1)) as m:
                    self.assertEqual(struct.calcsize(struct_format), m.itemsize)


class TestDeprecatedFormatStrings(unittest.TestCase):
    """
    The format string an array can still be named with, and the warning that
    says which spelling replaced it.
    """

    def test_each_format_string_warns(self) -> None:
        for buffer_format, winrt_name in FORMAT_STRINGS:
            with self.subTest(format=buffer_format):
                with self.assertWarns(DeprecationWarning):
                    a: Array[typing.Any] = Array(buffer_format, 3)  # type: ignore[deprecated]

                self.assertEqual(a._winrt_element_type_name_, winrt_name)

    def test_naming_the_type_does_not_warn(self) -> None:
        with warnings.catch_warnings():
            warnings.simplefilter("error", DeprecationWarning)

            for alias, _, _, _, winrt_name in SCALAR_ALIASES:
                with self.subTest(winrt_type=winrt_name):
                    Array(alias, 3)

            Array(bool, 3)
