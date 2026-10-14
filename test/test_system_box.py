import datetime
import unittest
import uuid

from winrt.system import (
    box_boolean,
    box_char16,
    box_date_time,
    box_double,
    box_guid,
    box_int8,
    box_int16,
    box_int32,
    box_int64,
    box_single,
    box_string,
    box_time_span,
    box_uint8,
    box_uint16,
    box_uint32,
    box_uint64,
    unbox_boolean,
    unbox_char16,
    unbox_date_time,
    unbox_double,
    unbox_guid,
    unbox_int8,
    unbox_int16,
    unbox_int32,
    unbox_int64,
    unbox_single,
    unbox_string,
    unbox_time_span,
    unbox_uint8,
    unbox_uint16,
    unbox_uint32,
    unbox_uint64,
)
from winrt.windows.foundation import Uri
from winrt.windows.foundation.collections import PropertySet


class TestSystemBox(unittest.TestCase):
    def test_box_unbox_boolean(self) -> None:
        self.assertIs(unbox_boolean(box_boolean(True)), True)
        self.assertIs(unbox_boolean(box_boolean(False)), False)

    def test_box_unbox_char16(self) -> None:
        self.assertEqual(unbox_char16(box_char16("\u01bf")), "ƿ")
        self.assertRaises(TypeError, box_char16, "test")

    def test_box_unbox_string(self) -> None:
        self.assertEqual(unbox_string(box_string("test\N{GRINNING FACE}")), "test😀")
        self.assertRaises(TypeError, box_string, 1)

    def test_box_unbox_int8(self) -> None:
        self.assertEqual(unbox_int8(box_int8(1)), 1)

    def test_box_unbox_uint8(self) -> None:
        self.assertEqual(unbox_uint8(box_uint8(1)), 1)

    def test_box_unbox_int16(self) -> None:
        self.assertEqual(unbox_int16(box_int16(1)), 1)

    def test_box_unbox_uint16(self) -> None:
        self.assertEqual(unbox_uint16(box_uint16(1)), 1)

    def test_box_unbox_int32(self) -> None:
        self.assertEqual(unbox_int32(box_int32(1)), 1)

    def test_box_unbox_uint32(self) -> None:
        self.assertEqual(unbox_uint32(box_uint32(1)), 1)

    def test_box_unbox_int64(self) -> None:
        self.assertEqual(unbox_int64(box_int64(1)), 1)

    def test_box_unbox_uint64(self) -> None:
        self.assertEqual(unbox_uint64(box_uint64(1)), 1)

    def test_box_unbox_single(self) -> None:
        self.assertEqual(unbox_single(box_single(1.5)), 1.5)

    def test_box_unbox_double(self) -> None:
        self.assertEqual(unbox_double(box_double(1.5)), 1.5)

    def test_box_unbox_guid(self) -> None:
        value = uuid.uuid4()
        self.assertEqual(unbox_guid(box_guid(value)), value)

    def test_box_unbox_date_time(self) -> None:
        value = datetime.datetime(2024, 1, 1, tzinfo=datetime.UTC)
        self.assertEqual(unbox_date_time(box_date_time(value)), value)

    def test_box_unbox_time_span(self) -> None:
        value = datetime.timedelta(1)
        self.assertEqual(unbox_time_span(box_time_span(value)), value)

    def test_no_keywords(self) -> None:
        # The type hints refuse these calls as well, which is what the ignores
        # check.
        with self.assertRaisesRegex(TypeError, "takes no keyword arguments"):
            box_int32(value=1)  # type: ignore[call-arg]

        with self.assertRaisesRegex(TypeError, "takes no keyword arguments"):
            unbox_int32(value=box_int32(1))  # type: ignore[call-arg]

    def test_unbox_of_another_type(self) -> None:
        # Only a value boxed as the type is unboxed, as C++/WinRT and C# do,
        # where IPropertyValue's getters convert between numeric types.
        with self.assertRaisesRegex(
            TypeError,
            r"^expected a boxed Int32, not Windows\.Foundation\.IReference`1<Int64>$",
        ):
            unbox_int32(box_int64(5))

        with self.assertRaisesRegex(
            TypeError, r"^expected a boxed String, not Windows\.Foundation\.Uri$"
        ):
            unbox_string(Uri("https://example.com/"))

        with self.assertRaisesRegex(TypeError, r"^expected a boxed Int32, not None$"):
            unbox_int32(None)  # type: ignore[arg-type]


class TestUnboxedValueForAnObject(unittest.TestCase):
    # A value that WinRT boxes, passed where any object will do, says how to
    # box it.
    def test_says_which_box_function(self) -> None:
        cases = (
            (5, "int", "box_int32"),
            (True, "bool", "box_boolean"),
            (1.5, "float", "box_double"),
            ("text", "str", "box_string"),
        )

        for value, name, function in cases:
            with (
                self.subTest(name),
                self.assertRaisesRegex(
                    TypeError,
                    rf"^expected a WinRT object, not '{name}': box it with "
                    rf"winrt\.system\.{function}\(\) or another box function$",
                ),
            ):
                PropertySet().insert("key", value)  # type: ignore[arg-type]

    def test_no_hint_for_what_is_not_boxed(self) -> None:
        with self.assertRaisesRegex(
            TypeError, r"^expected a WinRT object, not 'object'$"
        ):
            PropertySet().insert("key", object())  # type: ignore[arg-type]
