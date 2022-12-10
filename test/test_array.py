import datetime
import sys
import unittest
import uuid

from winrt import Array
from winrt.windows.foundation import Rect, Size, Point

is_64bits = sys.maxsize > 2**32
pointer_size = 8 if is_64bits else 4


class TestWinRTArray(unittest.TestCase):
    def test_bool(self):
        a = Array("?", [False, True])

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
        a = Array("b", [1, 2, 3])

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
        a = Array("B", [1, 2, 3])

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
        a = Array("h", [1, 2, 3])

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
        a = Array("H", [1, 2, 3])

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
        a = Array("i", [1, 2, 3])

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
        a = Array("I", [1, 2, 3])

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
        a = Array("q", [1, 2, 3])

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
        a = Array("Q", [1, 2, 3])

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
        a = Array("u", ["A", "B", "\u1234"])

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

        self.assertEqual(len(a), 3)
        self.assertEqual(list(a), ["A", "B", "CDE"])

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (3,))
            self.assertEqual(m.strides, (pointer_size,))
            self.assertEqual(m.itemsize, pointer_size)
            self.assertEqual(m.format, "P")
            self.assertTrue(m.c_contiguous)

    def test_guid(self):
        actual = [uuid.UUID("00112233-4455-6677-8899-AABBCCDDEEFF"), uuid.uuid4(), uuid.uuid4()]
        a = Array(uuid.UUID, actual)

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
            datetime.datetime.now(datetime.timezone.utc),
            datetime.datetime(2000, 1, 1, tzinfo=datetime.timezone.utc),
        ]
        a = Array(datetime.datetime, actual)

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

        self.assertEqual(len(a), 2)
        # FIXME: Point does not override object.__eq__
        # self.assertEqual(list(a), actual)

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (2,))
            self.assertEqual(m.strides, (8,))
            self.assertEqual(m.itemsize, 8)
            self.assertEqual(m.format, "2f")
            self.assertTrue(m.c_contiguous)

    def test_windows_foundation_size(self):
        actual = [
            Size(1, 2),
            Size(3, 4),
        ]
        a = Array(Size, actual)

        self.assertEqual(len(a), 2)
        # FIXME: Size does not override object.__eq__
        # self.assertEqual(list(a), actual)

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (2,))
            self.assertEqual(m.strides, (8,))
            self.assertEqual(m.itemsize, 8)
            self.assertEqual(m.format, "2f")
            self.assertTrue(m.c_contiguous)

    def test_windows_foundation_rect(self):
        actual = [
            Rect(1, 2, 3, 4),
            Rect(3, 4, 5, 6),
        ]
        a = Array(Rect, actual)

        self.assertEqual(len(a), 2)
        # FIXME: Rect does not override object.__eq__
        # self.assertEqual(list(a), actual)

        with memoryview(a) as m:
            self.assertEqual(m.ndim, 1)
            self.assertEqual(m.shape, (2,))
            self.assertEqual(m.strides, (16,))
            self.assertEqual(m.itemsize, 16)
            self.assertEqual(m.format, "4f")
            self.assertTrue(m.c_contiguous)
