import unittest

from winrt.system import unbox_string
from winrt.windows.foundation import Uri
from winrt.windows.foundation.collections import PropertySet


class TestSystemBox(unittest.TestCase):
    def test_unbox_of_another_type(self) -> None:
        with self.assertRaisesRegex(
            TypeError, r"^expected a boxed String, not Windows\.Foundation\.Uri$"
        ):
            unbox_string(Uri("https://example.com/"))


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
