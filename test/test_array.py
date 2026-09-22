import array as stdlib_array
import datetime
import struct
import sys
import typing
import unittest
import uuid

import test_winrt.testcomponent as tc
from winrt.system import (
    Array,
    BufferFormat,
    Char16,
    Double,
    Int8,
    Int16,
    Int32,
    Int64,
    Object,
    Single,
    StructFormat,
    UInt8,
    UInt16,
    UInt32,
    UInt64,
    WinrtSignature,
)
from winrt.windows.foundation import (
    IPropertyValue,
    IStringable,
    Point,
    Rect,
    Size,
    Uri,
)

#: Every winrt.system scalar alias with the three formats it is annotated
#: with - buffer, struct and WinRT signature - and the WinRT type it names.
SCALAR_ALIASES = [
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

is_64bits = sys.maxsize > 2**32
pointer_size = 8 if is_64bits else 4


class TestWinRTArray(unittest.TestCase):
    def test_empty_constructor(self):
        with self.assertRaises(TypeError):
            Array()  # type: ignore

    def test_bool(self):
        a = Array(bool, [False, True])

        self.assertEqual(a._winrt_element_type_name_, "Boolean")
        self.assertEqual(len(a), 2)
        self.assertFalse(a[0])
        self.assertTrue(a[1])

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (2,))
            self.assertEqual(m.strides, (1,))
            self.assertEqual(m.itemsize, 1)
            self.assertEqual(m.format, "?")
            self.assertTrue(m.c_contiguous)

    def test_int8(self):
        a = Array(Int8, [1, 2, 3])

        self.assertEqual(a._winrt_element_type_name_, "Int8")
        self.assertEqual(len(a), 3)
        self.assertEqual(list(a), [1, 2, 3])

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (3,))
            self.assertEqual(m.strides, (1,))
            self.assertEqual(m.itemsize, 1)
            self.assertEqual(m.format, "b")
            self.assertTrue(m.c_contiguous)

    def test_uint8(self):
        a = Array(UInt8, [1, 2, 3])

        self.assertEqual(a._winrt_element_type_name_, "UInt8")
        self.assertEqual(len(a), 3)
        self.assertEqual(list(a), [1, 2, 3])

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (3,))
            self.assertEqual(m.strides, (1,))
            self.assertEqual(m.itemsize, 1)
            self.assertEqual(m.format, "B")
            self.assertTrue(m.c_contiguous)

    def test_int16(self):
        a = Array(Int16, [1, 2, 3])

        self.assertEqual(a._winrt_element_type_name_, "Int16")
        self.assertEqual(len(a), 3)
        self.assertEqual(list(a), [1, 2, 3])

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (3,))
            self.assertEqual(m.strides, (2,))
            self.assertEqual(m.itemsize, 2)
            self.assertEqual(m.format, "h")
            self.assertTrue(m.c_contiguous)

    def test_uint16(self):
        a = Array(UInt16, [1, 2, 3])

        self.assertEqual(a._winrt_element_type_name_, "UInt16")
        self.assertEqual(len(a), 3)
        self.assertEqual(list(a), [1, 2, 3])

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (3,))
            self.assertEqual(m.strides, (2,))
            self.assertEqual(m.itemsize, 2)
            self.assertEqual(m.format, "H")
            self.assertTrue(m.c_contiguous)

    def test_int32(self):
        a = Array(Int32, [1, 2, 3])

        self.assertEqual(a._winrt_element_type_name_, "Int32")
        self.assertEqual(len(a), 3)
        self.assertEqual(list(a), [1, 2, 3])

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (3,))
            self.assertEqual(m.strides, (4,))
            self.assertEqual(m.itemsize, 4)
            self.assertEqual(m.format, "i")
            self.assertTrue(m.c_contiguous)

    def test_uint32(self):
        a = Array(UInt32, [1, 2, 3])

        self.assertEqual(a._winrt_element_type_name_, "UInt32")
        self.assertEqual(len(a), 3)
        self.assertEqual(list(a), [1, 2, 3])

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (3,))
            self.assertEqual(m.strides, (4,))
            self.assertEqual(m.itemsize, 4)
            self.assertEqual(m.format, "I")
            self.assertTrue(m.c_contiguous)

    def test_int64(self):
        a = Array(Int64, [1, 2, 3])

        self.assertEqual(a._winrt_element_type_name_, "Int64")
        self.assertEqual(len(a), 3)
        self.assertEqual(list(a), [1, 2, 3])

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (3,))
            self.assertEqual(m.strides, (8,))
            self.assertEqual(m.itemsize, 8)
            self.assertEqual(m.format, "q")
            self.assertTrue(m.c_contiguous)

    def test_uint64(self):
        a = Array(UInt64, [1, 2, 3])

        self.assertEqual(a._winrt_element_type_name_, "UInt64")
        self.assertEqual(len(a), 3)
        self.assertEqual(list(a), [1, 2, 3])

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (3,))
            self.assertEqual(m.strides, (8,))
            self.assertEqual(m.itemsize, 8)
            self.assertEqual(m.format, "Q")
            self.assertTrue(m.c_contiguous)

    def test_char(self):
        a = Array(Char16, ["A", "B", "\u1234"])

        self.assertEqual(a._winrt_element_type_name_, "Char16")
        self.assertEqual(len(a), 3)
        self.assertEqual(list(a), ["A", "B", "\u1234"])

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (3,))
            self.assertEqual(m.strides, (2,))
            self.assertEqual(m.itemsize, 2)
            self.assertEqual(m.format, "u")
            self.assertTrue(m.c_contiguous)

    def test_string(self):
        a = Array(str, ["A", "B", "CDE"])

        self.assertEqual(a._winrt_element_type_name_, "String")
        self.assertEqual(len(a), 3)
        self.assertEqual(list(a), ["A", "B", "CDE"])

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (3,))
            self.assertEqual(m.strides, (pointer_size,))
            self.assertEqual(m.itemsize, pointer_size)
            self.assertEqual(m.format, "P")
            self.assertTrue(m.c_contiguous)

    def test_object(self):
        a = Array(Object, 3)

        self.assertEqual(a._winrt_element_type_name_, "Object")
        self.assertEqual(len(a), 3)
        self.assertEqual(list(a), [None, None, None])

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (3,))
            self.assertEqual(m.strides, (pointer_size,))
            self.assertEqual(m.itemsize, pointer_size)
            self.assertEqual(m.format, "P")
            self.assertTrue(m.c_contiguous)

    def test_guid(self):
        actual = [
            uuid.UUID("00112233-4455-6677-8899-AABBCCDDEEFF"),
            uuid.uuid4(),
            uuid.uuid4(),
        ]
        a = Array(uuid.UUID, actual)

        self.assertEqual(a._winrt_element_type_name_, "Guid")
        self.assertEqual(len(a), 3)
        self.assertEqual(list(a), actual)

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (3,))
            self.assertEqual(m.strides, (16,))
            self.assertEqual(m.itemsize, 16)
            self.assertEqual(m.format, "T{I2H8B}")
            self.assertTrue(m.c_contiguous)

            self.assertEqual(m[0:1].hex(), "33221100554477668899aabbccddeeff")

    def test_windows_foundation_datetime(self):
        actual = [
            datetime.datetime.now(datetime.UTC),
            datetime.datetime(2000, 1, 1, tzinfo=datetime.UTC),
        ]
        a = Array(datetime.datetime, actual)

        self.assertEqual(a._winrt_element_type_name_, "Windows.Foundation.DateTime")
        self.assertEqual(len(a), 2)
        self.assertEqual(list(a), actual)

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (2,))
            self.assertEqual(m.strides, (8,))
            self.assertEqual(m.itemsize, 8)
            self.assertEqual(m.format, "q")
            self.assertTrue(m.c_contiguous)

    def test_windows_foundation_timespan(self):
        actual = [
            datetime.timedelta(0),
            datetime.timedelta(1),
        ]
        a = Array(datetime.timedelta, actual)

        self.assertEqual(a._winrt_element_type_name_, "Windows.Foundation.TimeSpan")
        self.assertEqual(len(a), 2)
        self.assertEqual(list(a), actual)

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (2,))
            self.assertEqual(m.strides, (8,))
            self.assertEqual(m.itemsize, 8)
            self.assertEqual(m.format, "q")
            self.assertTrue(m.c_contiguous)

    def test_windows_foundation_point(self):
        actual = [
            Point(1, 2),
            Point(3, 4),
        ]
        a = Array(Point, actual)

        self.assertEqual(a._winrt_element_type_name_, "Windows.Foundation.Point")
        self.assertEqual(len(a), 2)
        self.assertEqual(list(a), actual)

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (2,))
            self.assertEqual(m.strides, (8,))
            self.assertEqual(m.itemsize, 8)
            self.assertEqual(m.format, "T{f:x:f:y:}")
            self.assertTrue(m.c_contiguous)

    def test_windows_foundation_size(self):
        actual = [
            Size(1, 2),
            Size(3, 4),
        ]
        a = Array(Size, actual)

        self.assertEqual(a._winrt_element_type_name_, "Windows.Foundation.Size")
        self.assertEqual(len(a), 2)
        self.assertEqual(list(a), actual)

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (2,))
            self.assertEqual(m.strides, (8,))
            self.assertEqual(m.itemsize, 8)
            self.assertEqual(m.format, "T{f:width:f:height:}")
            self.assertTrue(m.c_contiguous)

    def test_windows_foundation_rect(self):
        actual = [
            Rect(1, 2, 3, 4),
            Rect(3, 4, 5, 6),
        ]
        a = Array(Rect, actual)

        self.assertEqual(a._winrt_element_type_name_, "Windows.Foundation.Rect")
        self.assertEqual(len(a), 2)
        self.assertEqual(list(a), actual)

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (2,))
            self.assertEqual(m.strides, (16,))
            self.assertEqual(m.itemsize, 16)
            self.assertEqual(m.format, "T{f:x:f:y:f:width:f:height:}")
            self.assertTrue(m.c_contiguous)

    def test_runtime_type(self):
        a = Array(Uri, [Uri("https://example.com")])

        self.assertEqual(a._winrt_element_type_name_, "Windows.Foundation.Uri")
        self.assertEqual(len(a), 1)
        self.assertEqual(str(a[0]), "https://example.com/")

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (1,))
            self.assertEqual(m.strides, (pointer_size,))
            self.assertEqual(m.itemsize, pointer_size)
            self.assertEqual(m.format, "P")
            self.assertTrue(m.c_contiguous)

    def test_interface(self):
        a = Array(IPropertyValue, 2)

        self.assertEqual(
            a._winrt_element_type_name_, "Windows.Foundation.IPropertyValue"
        )
        self.assertEqual(len(a), 2)
        self.assertEqual(list(a), [None, None])

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (2,))
            self.assertEqual(m.strides, (pointer_size,))
            self.assertEqual(m.itemsize, pointer_size)
            self.assertEqual(m.format, "P")
            self.assertTrue(m.c_contiguous)

    def test_ambiguous_builtin_types(self):
        for builtin in (int, float):
            with self.subTest(type=builtin):
                with self.assertRaises(TypeError):
                    Array(builtin, 3)

    def test_sequence_protocol(self):
        a = Array(UInt8, list(range(10)))

        for i, v in enumerate(a):
            # positive index
            self.assertEqual(a[i], v)
            # negative index
            self.assertEqual(a[i - len(a)], v)
            # __contains__
            self.assertIn(v, a)

        self.assertEqual(a.count(5), 1)
        self.assertEqual(a.index(5), 5)


#: One value of each element type an ArrayN member of the test component
#: takes, with the winrt.system.Array type argument that spells it. Every
#: ArrayN member does the same thing with them: it fills the array it is lent
#: from the array it is passed, and hands the same values back as an output
#: and as the return value.
ARRAY_MEMBERS = [
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


def blittable(seed: int) -> tc.Blittable:
    return tc.Blittable(
        seed,
        seed + 1,
        seed + 2,
        seed + 3,
        seed + 4,
        seed + 5,
        seed + 6,
        seed + 7.5,
        seed + 8.5,
        uuid.UUID(int=seed),
    )


def non_blittable(seed: int) -> tc.NonBlittable:
    return tc.NonBlittable(seed % 2 == 0, chr(ord("a") + seed), f"value {seed}", seed)


def nested(seed: int) -> tc.Nested:
    return tc.Nested(blittable(seed), non_blittable(seed))


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

    def check_forward(self, name: str, element, values: list) -> None:
        passed = Array(element, values)
        lent = Array(element, len(values))

        returned, received = getattr(self.tests, name)(passed, lent)

        self.assertEqual(list(lent), values)
        self.assertEqual(list(received), values)
        self.assertEqual(list(returned), values)

    def check_reverse(self, name: str, element, values: list) -> None:
        seen = []

        def handler(passed, lent):
            seen.append((list(passed), len(lent)))

            for index, value in enumerate(passed):
                lent[index] = value

            return Array(element, list(passed)), Array(element, list(passed))

        getattr(self.tests, f"{name}_call")(handler)

        # The test component checks what the handler filled and handed back,
        # so getting this far is the assertion. What it passes is its own.
        self.assertEqual(len(seen), 1)
        self.assertEqual(seen[0][1], len(seen[0][0]))

    def test_fundamental_types(self):
        for name, element, values in ARRAY_MEMBERS:
            with self.subTest(member=name):
                self.check_forward(name, element, values)
                self.check_reverse(name, element, values)

    def test_blittable_struct(self):
        values = [blittable(1), blittable(2)]
        self.check_forward("array13", tc.Blittable, values)
        self.check_reverse("array13", tc.Blittable, values)

    def test_non_blittable_struct(self):
        values = [non_blittable(1), non_blittable(2)]
        self.check_forward("array14", tc.NonBlittable, values)
        self.check_reverse("array14", tc.NonBlittable, values)

    def test_nested_struct(self):
        values = [nested(1), nested(2)]
        self.check_forward("array15", tc.Nested, values)
        self.check_reverse("array15", tc.Nested, values)

    def test_interface(self):
        passed = Array(IStringable, [IStringable._from(Uri("https://example.com"))])
        lent = Array(IStringable, 1)

        returned, received = self.tests.array16(passed, lent)

        for array in (lent, received, returned):
            self.assertEqual([str(value) for value in array], ["https://example.com/"])

    def test_interface_reverse(self):
        seen = []

        def handler(passed, lent):
            seen.append([str(value) for value in passed])

            for index, value in enumerate(passed):
                lent[index] = value

            return Array(IStringable, list(passed)), Array(IStringable, list(passed))

        self.tests.array16_call(handler)

        self.assertEqual(len(seen), 1)

    def test_lent_array_is_taken_back(self):
        kept = []

        def handler(passed, lent):
            kept.append(lent)

            for index, value in enumerate(passed):
                lent[index] = value

            return Array(bool, list(passed)), Array(bool, list(passed))

        self.tests.array1_call(handler)

        # A lent array is the caller's own elements, and the call it was lent
        # for is over.
        self.assertEqual(len(kept[0]), 0)

    def test_passed_array_must_hold_the_declared_element(self):
        with self.assertRaises(BufferError):
            self.tests.array7(Array(Int16, [1, 2]), Array(Int32, 2))

    def test_lent_array_must_be_writable(self):
        with self.assertRaises(BufferError):
            self.tests.array2(bytes([1, 2]), bytes(2))

    def test_any_buffer_of_the_right_shape(self):
        lent = stdlib_array.array("i", [0, 0, 0])

        returned, received = self.tests.array7(stdlib_array.array("i", [1, 2, 3]), lent)

        self.assertEqual(list(lent), [1, 2, 3])
        self.assertEqual(list(received), [1, 2, 3])
        self.assertEqual(list(returned), [1, 2, 3])


class TestScalarAliases(unittest.TestCase):
    """
    The winrt.system aliases that name the scalars no Python type names, and
    the three annotations each one carries.
    """

    def test_array_of_each_alias(self):
        for alias, buffer_format, _, _, winrt_name in SCALAR_ALIASES:
            with self.subTest(winrt_type=winrt_name):
                a = Array(alias, 3)

                self.assertEqual(a._winrt_element_type_name_, winrt_name)

                with memoryview(a) as m:
                    self.assertEqual(m.format, buffer_format)

    def test_buffer_format_comes_first(self):
        # Code written against the single string these aliases used to carry
        # reads the first annotation by position, so the buffer format is the
        # one that has to come first.
        for alias, buffer_format, _, _, winrt_name in SCALAR_ALIASES:
            with self.subTest(winrt_type=winrt_name):
                self.assertEqual(typing.get_args(alias)[1], buffer_format)

    def test_annotations_are_found_by_type(self):
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

    def test_struct_format_matches_the_element(self):
        # The struct module has no "u" format at all, which is why Char16 is
        # the one alias whose struct format is not its buffer format.
        for alias, _, struct_format, _, winrt_name in SCALAR_ALIASES:
            with self.subTest(winrt_type=winrt_name):
                with memoryview(Array(alias, 1)) as m:
                    self.assertEqual(struct.calcsize(struct_format), m.itemsize)
