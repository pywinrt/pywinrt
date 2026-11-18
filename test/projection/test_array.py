import array as stdlib_array
import collections.abc
import datetime
import typing
import unittest
import uuid

import test_winrt.testcomponent as tc

from winrt.system import (
    Array,
    Char16,
    Int8,
    Int16,
    Int32,
    Int64,
    Object,
    UInt8,
    UInt16,
    UInt32,
    UInt64,
)
from winrt.windows.foundation import (
    IPropertyValue,
    IStringable,
    Point,
    PropertyType,
    Rect,
    Size,
    Uri,
)
from winrt.windows.storage import FileAttributes

from ._arrays import blittable, non_blittable, pointer_size


class TestWinrtArray(unittest.TestCase):
    def test_empty_constructor(self) -> None:
        with self.assertRaises(TypeError):
            Array()  # type: ignore

    def test_constructor_takes_no_keywords(self) -> None:
        # the stub says so too, which is what makes these ignores needed
        with self.assertRaisesRegex(TypeError, "keyword arguments are not supported"):
            Array(Int32, size=2)  # type: ignore

        with self.assertRaisesRegex(TypeError, "keyword arguments are not supported"):
            Array(Int32, initializer=bytes(8))  # type: ignore

        with self.assertRaisesRegex(TypeError, "keyword arguments are not supported"):
            Array(type=Int32)  # type: ignore

    def test_bool(self) -> None:
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

    def test_int8(self) -> None:
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

    def test_uint8(self) -> None:
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

    def test_int16(self) -> None:
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

    def test_uint16(self) -> None:
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

    def test_int32(self) -> None:
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

    def test_uint32(self) -> None:
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

    def test_int64(self) -> None:
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

    def test_uint64(self) -> None:
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

    def test_char(self) -> None:
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

    def test_enum(self) -> None:
        a = Array(PropertyType, [PropertyType.INT32, PropertyType.STRING])

        self.assertEqual(a._winrt_element_type_name_, "Windows.Foundation.PropertyType")
        self.assertEqual(len(a), 2)
        self.assertEqual(list(a), [PropertyType.INT32, PropertyType.STRING])

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (2,))
            self.assertEqual(m.strides, (4,))
            self.assertEqual(m.itemsize, 4)
            self.assertEqual(m.format, "i")
            self.assertTrue(m.c_contiguous)

    def test_flags_enum(self) -> None:
        a = Array(FileAttributes, [FileAttributes.DIRECTORY, FileAttributes.READ_ONLY])

        self.assertEqual(a._winrt_element_type_name_, "Windows.Storage.FileAttributes")
        self.assertEqual(len(a), 2)
        self.assertEqual(list(a), [FileAttributes.DIRECTORY, FileAttributes.READ_ONLY])

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (2,))
            self.assertEqual(m.strides, (4,))
            self.assertEqual(m.itemsize, 4)
            self.assertEqual(m.format, "I")
            self.assertTrue(m.c_contiguous)

    def test_enum_from_a_buffer(self) -> None:
        a = Array(PropertyType, stdlib_array.array("i", [1, 2]))
        self.assertEqual(list(a), [PropertyType.UINT8, PropertyType.INT16])

        flags = Array(FileAttributes, stdlib_array.array("I", [1, 16]))
        self.assertEqual(
            list(flags), [FileAttributes.READ_ONLY, FileAttributes.DIRECTORY]
        )

        with self.assertRaisesRegex(
            BufferError, r'^requires buffer with format == "i", have "I"$'
        ):
            Array(PropertyType, stdlib_array.array("I", [1]))

        with self.assertRaisesRegex(
            BufferError, r'^requires buffer with format == "I", have "i"$'
        ):
            Array(FileAttributes, stdlib_array.array("i", [1]))

    def test_from_a_buffer_of_another_layout(self) -> None:
        with self.assertRaisesRegex(
            BufferError, r"^requires buffer with itemsize == 4, have 2$"
        ):
            Array(Int32, stdlib_array.array("h", [1]))

        flat = memoryview(stdlib_array.array("i", [1, 2]))
        with self.assertRaisesRegex(
            BufferError, r"^requires buffer with ndim == 1, have 2$"
        ):
            Array(Int32, flat.cast("B").cast("i", (1, 2)))

    def test_string(self) -> None:
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

    def test_object(self) -> None:
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

    def test_guid(self) -> None:
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

    def test_windows_foundation_datetime(self) -> None:
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

    def test_windows_foundation_timespan(self) -> None:
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

    def test_windows_foundation_point(self) -> None:
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

    def test_windows_foundation_size(self) -> None:
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

    def test_windows_foundation_rect(self) -> None:
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

    def test_runtime_type(self) -> None:
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

    def test_interface(self) -> None:
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

    def test_ambiguous_builtin_types(self) -> None:
        for builtin in (int, float):
            with self.subTest(type=builtin):
                with self.assertRaises(TypeError):
                    Array(builtin, 3)

    def test_sequence_protocol(self) -> None:
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

    def test_guid_is_uuid_itself(self) -> None:
        class UUID:
            pass

        class Derived(uuid.UUID):
            pass

        for impostor in (UUID, Derived):
            with self.subTest(type=impostor):
                with self.assertRaises(TypeError):
                    Array(impostor, 1)

    def test_a_type_with_no_instances_of_its_own_is_refused(self) -> None:
        class Derived(tc.Composable):
            pass

        class Foo:
            @classmethod
            def _assign_array_(cls, a: object) -> None:
                pass

        # A static class, the wrapper an interface's instances are returned
        # as, a Python class derived from a projected one, and a Python class
        # that defines what Array asked a type for in 3.x.
        for cls in (tc.TestRunner, getattr(tc, "_ITests"), Derived, Foo):
            with self.subTest(cls=cls):
                with self.assertRaisesRegex(TypeError, "cannot use"):
                    Array(cls, 1)

    def test_copy_from_buffer(self) -> None:
        values = [blittable(1), blittable(2)]

        a = Array(tc.Blittable, memoryview(Array(tc.Blittable, values)))

        self.assertEqual(list(a), values)

    def test_list_that_grows_while_converted(self) -> None:
        items: list[object] = [1, 2, 3]

        class Grow:
            def __index__(self) -> int:
                items.append(4)
                return 1

        items[0] = Grow()

        self.assertEqual(list(Array(Int64, items)), [1, 2, 3])

    def test_list_that_shrinks_while_converted(self) -> None:
        items: list[object] = [1, 2, 3]

        class Shrink:
            def __index__(self) -> int:
                items.clear()
                return 1

        items[0] = Shrink()

        with self.assertRaises(IndexError):
            Array(Int64, items)

    def test_from_any_iterable(self) -> None:
        class List(list[int]):
            pass

        class Tuple(tuple[int, ...]):
            pass

        for values in (
            List([1, 2, 3]),
            Tuple((1, 2, 3)),
            range(1, 4),
            (i for i in (1, 2, 3)),
            collections.deque([1, 2, 3]),
            {1: "a", 2: "b", 3: "c"}.keys(),
        ):
            with self.subTest(type(values).__name__):
                self.assertEqual(list(Array(Int32, values)), [1, 2, 3])

    def test_from_a_str_is_from_its_characters(self) -> None:
        self.assertEqual(list(Array(Char16, "abc")), ["a", "b", "c"])

    def test_from_something_not_iterable(self) -> None:
        with self.assertRaisesRegex(TypeError, "not iterable"):
            Array(Int32, 1.5)  # type: ignore[call-overload]

    def test_from_an_iterable_that_fails(self) -> None:
        def values() -> collections.abc.Iterator[int]:
            yield 1
            raise ValueError("test")

        with self.assertRaisesRegex(ValueError, "test"):
            Array(Int32, values())

    def test_no_copy_of_references_from_buffer(self) -> None:
        element: typing.Any
        values: list[typing.Any]

        for element, values in (
            (str, ["a", "b"]),
            (Uri, [Uri("https://example.com")]),
            (tc.NonBlittable, [non_blittable(1)]),
        ):
            with self.subTest(element=element):
                source = Array(element, values)

                with self.assertRaisesRegex(TypeError, "hold references"):
                    Array(element, memoryview(source))

    def test_references_are_read_only_as_a_buffer(self) -> None:
        element: typing.Any
        values: list[typing.Any]

        for element, values in (
            (str, ["a", "b"]),
            (Uri, [Uri("https://example.com")]),
            (tc.NonBlittable, [non_blittable(1)]),
        ):
            with self.subTest(element=element):
                with memoryview(Array(element, values)) as m:
                    self.assertTrue(m.readonly)

                    with m.cast("B") as b, self.assertRaises(TypeError):
                        b[0] = 1

    def test_references_are_not_replaced_while_exported(self) -> None:
        element: typing.Any
        values: list[typing.Any]

        for element, values, value in (
            (str, ["a"], "b"),
            (Uri, [Uri("https://example.com")], Uri("https://example.org")),
            (tc.NonBlittable, [non_blittable(1)], non_blittable(2)),
        ):
            with self.subTest(element=element):
                a = Array(element, values)

                with memoryview(a):
                    with self.assertRaises(BufferError):
                        a[0] = value

                    self.assertEqual([str(item) for item in a], [str(values[0])])

                a[0] = value

                self.assertEqual(str(a[0]), str(value))

    def test_values_are_replaced_while_exported(self) -> None:
        a = Array(Int32, [1, 2])

        with memoryview(a) as m:
            a[0] = 3

            self.assertEqual(m[0], 3)

    def test_values_are_writable_as_a_buffer(self) -> None:
        a = Array(Int32, [1, 2])

        with memoryview(a) as m:
            self.assertFalse(m.readonly)
            m[0] = 3

        self.assertEqual(list(a), [3, 2])

    def test_is_a_sequence_but_not_a_mutable_sequence(self) -> None:
        a = Array(Int32, [1, 2, 3])

        self.assertIsInstance(a, collections.abc.Sequence)
        self.assertNotIsInstance(a, collections.abc.MutableSequence)

    def test_item_assignment(self) -> None:
        a = Array(Int32, [1, 2, 3])

        a[0] = 4
        a[-1] = 6

        self.assertEqual(list(a), [4, 2, 6])

        with self.assertRaises(IndexError):
            a[3] = 7

    def test_size_is_fixed(self) -> None:
        a = Array(Int32, [1, 2, 3])

        with self.assertRaises(TypeError):
            del a[0]  # type: ignore

        with self.assertRaises(TypeError):
            a[0:1] = [4]  # type: ignore

        for name in ("insert", "append", "extend", "pop", "remove", "clear"):
            with self.subTest(name=name):
                self.assertFalse(hasattr(a, name))

        self.assertEqual(list(a), [1, 2, 3])

    def test_index_out_of_range(self) -> None:
        a = Array(Int32, [1, 2, 3])

        for index in (3, -4):
            with self.subTest(index=index):
                with self.assertRaises(IndexError):
                    a[index]

        with self.assertRaises(TypeError):
            a["0"]  # type: ignore

    def test_slice(self) -> None:
        element: typing.Any
        values: list[typing.Any]

        for element, values in (
            (Int32, [1, 2, 3, 4]),
            (str, ["a", "b", "c", "d"]),
            (Uri, [Uri(f"https://example.com/{i}") for i in range(4)]),
            (tc.Blittable, [blittable(i) for i in range(4)]),
            (tc.NonBlittable, [non_blittable(i) for i in range(4)]),
        ):
            with self.subTest(element=element):
                a = Array(element, values)

                for key in (
                    slice(1, 3),
                    slice(None, 2),
                    slice(-2, None),
                    slice(None),
                    slice(3, 1),
                    slice(10, 20),
                ):
                    with self.subTest(key=key):
                        b = a[key]

                        self.assertIsInstance(b, Array)
                        self.assertEqual(
                            b._winrt_element_type_name_, a._winrt_element_type_name_
                        )
                        self.assertEqual(list(b), values[key])

    def test_slice_is_a_copy(self) -> None:
        a = Array(Int32, [1, 2, 3])

        b = a[:2]
        b[0] = 4

        self.assertEqual(list(a), [1, 2, 3])

        # the references a slice holds are its own
        uris = Array(Uri, [Uri("https://example.com")])
        c = uris[:]
        del uris

        self.assertEqual(str(c[0]), "https://example.com/")

    def test_slice_step(self) -> None:
        a = Array(Int32, [1, 2, 3])

        with self.assertRaises(NotImplementedError):
            a[::2]

    def test_equality(self) -> None:
        a = Array(Int32, [1, 2, 3])

        self.assertEqual(a, Array(Int32, [1, 2, 3]))
        self.assertNotEqual(a, Array(Int32, [1, 2]))
        self.assertNotEqual(a, Array(Int32, [1, 2, 4]))
        self.assertEqual(Array(str, ["a", "b"]), Array(str, ["a", "b"]))
        self.assertEqual(
            Array(tc.Blittable, [blittable(1)]), Array(tc.Blittable, [blittable(1)])
        )

    def test_equality_across_element_types(self) -> None:
        # as with array.array, it is the values that compare
        self.assertEqual(Array(Int32, [1, 2]), Array(Int64, [1, 2]))
        self.assertNotEqual(Array(Int32, [1, 2]), Array(Int64, [1, 3]))

    def test_equals_only_an_array(self) -> None:
        a = Array(Int32, [1, 2, 3])

        # as a list equals only a list
        for other in ([1, 2, 3], (1, 2, 3), stdlib_array.array("i", [1, 2, 3]), 1):
            with self.subTest(other=other):
                self.assertFalse(a == other)
                self.assertFalse(other == a)
                self.assertTrue(a != other)

        self.assertFalse(Array(Char16, ["a", "b"]) == "ab")
        self.assertEqual(list(a), [1, 2, 3])

    def test_equality_propagates_errors(self) -> None:
        class Unequal(IStringable):
            def to_string(self) -> str:
                return "unequal"

            def __eq__(self, other: object) -> bool:
                raise ZeroDivisionError

        a = Array(IStringable, [Unequal()])

        with self.assertRaises(ZeroDivisionError):
            _ = a == Array(IStringable, [Unequal()])

    def test_not_hashable(self) -> None:
        with self.assertRaises(TypeError):
            hash(Array(Int32, [1]))

    def test_repr(self) -> None:
        self.assertEqual(repr(Array(Int32, [1, 2, 3])), "Array(Int32, [1, 2, 3])")
        self.assertEqual(repr(Array(str, ["a"])), "Array(String, ['a'])")
        self.assertEqual(repr(Array(Int32)), "Array(Int32, [])")
        self.assertEqual(
            repr(Array(FileAttributes, [FileAttributes.READ_ONLY])),
            f"Array(Windows.Storage.FileAttributes, [{FileAttributes.READ_ONLY!r}])",
        )
