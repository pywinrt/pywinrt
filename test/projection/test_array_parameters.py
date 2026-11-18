import array as stdlib_array
import typing
import unittest
from collections.abc import Callable

import test_winrt.testcomponent as tc
from typing_extensions import TypeForm

from winrt.system import (
    Array,
    Char16,
    Double,
    Int16,
    Int32,
    Int64,
    Single,
    UInt8,
    UInt16,
    UInt32,
    UInt64,
)
from winrt.windows.foundation import IStringable, Uri

from ._arrays import blittable, nested, non_blittable, pointer_size
from ._util import catch_unraisable

#: One value of each element type an ArrayN member of the test component
#: takes, with the winrt.system.Array type argument that spells it. Every
#: ArrayN member does the same thing with them: it fills the array it is lent
#: from the array it is passed, and hands the same values back as an output
#: and as the return value.
ARRAY_MEMBERS: list[tuple[str, typing.Any, list[typing.Any]]] = [
    ("array1", bool, [True, False, True]),
    ("array2", UInt8, [1, 2, 3]),
    ("array3", UInt16, [1, 2, 3]),
    ("array4", UInt32, [1, 2, 3]),
    ("array5", UInt64, [1, 2, 3]),
    ("array6", Int16, [-1, 2, -3]),
    ("array7", Int32, [-1, 2, -3]),
    ("array8", Int64, [-1, 2, -3]),
    ("array9", Single, [1.5, 2.5, 3.5]),
    ("array10", Double, [1.5, 2.5, 3.5]),
    ("array11", Char16, ["A", "B", "\u1234"]),
    ("array12", str, ["one", "two", "three"]),
]


class TestArrayParameters(unittest.TestCase):
    """
    The three WinRT array parameter categories, in both directions.

    Every ArrayN member of the test component takes one array to read and one
    to fill and hands back two more, so one member per element type covers a
    passed, a lent and two received arrays at once. ArrayNCall runs the same
    member the other way round, with the test component as the caller and a
    Python callable as the delegate.
    """

    def setUp(self) -> None:
        self.tests = tc.TestRunner.make_tests()

    def check_forward(
        self, name: str, element: TypeForm[typing.Any], values: list[typing.Any]
    ) -> None:
        passed = Array(element, values)
        lent = Array(element, len(values))

        returned, received = getattr(self.tests, name)(passed, lent)

        self.assertEqual(list(lent), values)
        self.assertEqual(list(received), values)
        self.assertEqual(list(returned), values)

    def check_reverse(
        self, name: str, element: TypeForm[typing.Any], values: list[typing.Any]
    ) -> None:
        seen: list[tuple[list[typing.Any], int]] = []

        def handler(
            passed: Array[typing.Any], lent: Array[typing.Any]
        ) -> tuple[Array[typing.Any], Array[typing.Any]]:
            seen.append((list(passed), len(lent)))

            for index, value in enumerate(passed):
                lent[index] = value

            return Array(element, list(passed)), Array(element, list(passed))

        getattr(self.tests, f"{name}_call")(handler)

        # The test component checks what the handler filled and handed back,
        # so getting this far is the assertion. What it passes is its own.
        self.assertEqual(len(seen), 1)
        self.assertEqual(seen[0][1], len(seen[0][0]))

    def test_fundamental_types(self) -> None:
        for name, element, values in ARRAY_MEMBERS:
            with self.subTest(member=name):
                self.check_forward(name, element, values)
                self.check_reverse(name, element, values)

    def test_blittable_struct(self) -> None:
        values = [blittable(1), blittable(2)]
        self.check_forward("array13", tc.Blittable, values)
        self.check_reverse("array13", tc.Blittable, values)

    def test_non_blittable_struct(self) -> None:
        values = [non_blittable(1), non_blittable(2)]
        self.check_forward("array14", tc.NonBlittable, values)
        self.check_reverse("array14", tc.NonBlittable, values)

    def test_nested_struct(self) -> None:
        values = [nested(1), nested(2)]
        self.check_forward("array15", tc.Nested, values)
        self.check_reverse("array15", tc.Nested, values)

    def test_interface(self) -> None:
        passed = Array(IStringable, [Uri("https://example.com").as_(IStringable)])
        lent = Array(IStringable, 1)

        returned, received = self.tests.array16(passed, lent)

        for array in (lent, received, returned):
            self.assertEqual([str(value) for value in array], ["https://example.com/"])

    def test_interface_reverse(self) -> None:
        seen: list[list[str]] = []

        def handler(
            passed: Array[IStringable], lent: Array[IStringable]
        ) -> tuple[Array[IStringable], Array[IStringable]]:
            seen.append([str(value) for value in passed])

            for index, value in enumerate(passed):
                lent[index] = value

            return Array(IStringable, list(passed)), Array(IStringable, list(passed))

        self.tests.array16_call(handler)

        self.assertEqual(len(seen), 1)

    def test_lent_array_is_taken_back(self) -> None:
        kept: list[Array[bool]] = []

        def handler(
            passed: Array[bool], lent: Array[bool]
        ) -> tuple[Array[bool], Array[bool]]:
            kept.append(lent)

            for index, value in enumerate(passed):
                lent[index] = value

            return Array(bool, list(passed)), Array(bool, list(passed))

        self.tests.array1_call(handler)

        # A lent array is the caller's own elements, and the call it was lent
        # for is over.
        self.assertEqual(len(kept[0]), 0)

    def check_fill_array_refuses(
        self,
        name: str,
        element: TypeForm[typing.Any],
        export: Callable[[Array[typing.Any], Array[typing.Any]], object],
    ) -> None:
        kept: list[Array[typing.Any]] = []
        errors: list[BufferError] = []

        def handler(
            passed: Array[typing.Any], lent: Array[typing.Any]
        ) -> tuple[Array[typing.Any], Array[typing.Any]]:
            kept.append(lent)

            try:
                export(passed, lent)
            except BufferError as e:
                errors.append(e)

            for index, value in enumerate(passed):
                lent[index] = value

            return Array(element, list(passed)), Array(element, list(passed))

        getattr(self.tests, f"{name}_call")(handler)

        self.assertEqual(len(errors), 1)
        self.assertRegex(str(errors[0]), "fill array cannot be exported")
        self.assertEqual(len(kept[0]), 0)

    def test_fill_array_refuses_a_memoryview(self) -> None:
        def export(passed: Array[typing.Any], lent: Array[typing.Any]) -> None:
            with memoryview(lent):
                pass

        self.check_fill_array_refuses("array7", Int32, export)
        self.check_fill_array_refuses("array12", str, export)

    def test_fill_array_refuses_bytes(self) -> None:
        self.check_fill_array_refuses("array7", Int32, lambda _, lent: bytes(lent))

    def test_fill_array_cannot_be_passed_on(self) -> None:
        def export(passed: Array[str], lent: Array[str]) -> None:
            self.tests.array12(passed, lent)

        self.check_fill_array_refuses("array12", str, export)

    def test_fill_array_is_read_by_item(self) -> None:
        seen: list[tuple[list[str], Array[str], list[str]]] = []

        def handler(
            passed: Array[str], lent: Array[str]
        ) -> tuple[Array[str], Array[str]]:
            for index, value in enumerate(passed):
                lent[index] = value

            seen.append((list(passed), lent[:], list(lent)))

            return Array(str, list(passed)), Array(str, list(passed))

        self.tests.array12_call(handler)

        passed, copy, items = seen[0]
        self.assertEqual(list(copy), passed)
        self.assertEqual(items, passed)

    def test_passed_array_must_hold_the_declared_element(self) -> None:
        with self.assertRaisesRegex(BufferError, "itemsize == 4, have 2"):
            self.tests.array7(Array(Int16, [1, 2]), Array(Int32, 2))

    def test_values_only_from_a_buffer(self) -> None:
        # A list holds Python objects, not the elements' layout.
        with self.assertRaisesRegex(
            TypeError,
            r"^elements of type Int32 can only be passed in a winrt\.system\.Array "
            r"or a buffer, not in list$",
        ):
            self.tests.array7([1, 2], Array(Int32, 2))  # type: ignore[arg-type]

    def test_lent_array_must_be_writable(self) -> None:
        with self.assertRaises(BufferError):
            self.tests.array2(bytes([1, 2]), bytes(2))

    def test_any_buffer_of_the_right_shape(self) -> None:
        lent = stdlib_array.array("i", [0, 0, 0])

        returned, received = self.tests.array7(stdlib_array.array("i", [1, 2, 3]), lent)

        self.assertEqual(list(lent), [1, 2, 3])
        self.assertEqual(list(received), [1, 2, 3])
        self.assertEqual(list(returned), [1, 2, 3])

    def test_references_only_from_an_array(self) -> None:
        # The same bytes and format as an array of strings, but nothing says
        # what the pointers point at.
        pointers = memoryview(bytes(3 * pointer_size)).cast("P")

        with self.assertRaisesRegex(TypeError, "winrt.system.Array"):
            self.tests.array12(pointers, Array(str, 3))

        with self.assertRaisesRegex(TypeError, "winrt.system.Array"):
            self.tests.array12(Array(str, 3), memoryview(Array(str, 3)))

        with self.assertRaisesRegex(TypeError, "winrt.system.Array"):
            self.tests.array14(
                memoryview(Array(tc.NonBlittable, [non_blittable(1)])),
                Array(tc.NonBlittable, 1),
            )

    def test_references_only_from_an_array_of_the_declared_element(self) -> None:
        uris = Array(Uri, [Uri("https://example.com")])

        with self.assertRaisesRegex(TypeError, "IStringable, not of .*Uri"):
            self.tests.array16(uris, Array(IStringable, 1))

    def test_references_returned_only_in_an_array(self) -> None:
        def handler(
            passed: Array[str], lent: Array[str]
        ) -> tuple[memoryview, Array[str]]:
            return memoryview(Array(str, list(passed))), Array(str, list(passed))

        with (
            self.assertRaises(OSError),
            catch_unraisable() as exceptions,
        ):
            self.tests.array12_call(handler)

        self.assertIsInstance(exceptions[0].exc_value, TypeError)
