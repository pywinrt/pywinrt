import datetime
import unittest

from winrt.windows.foundation import PropertyType, Point, Size, Rect, Uri
from winrt.windows.foundation.interop import box, unbox

class TestWindowsFoundationInteropBoxUnbox(unittest.TestCase):
    def test_box_unbox_none(self):
       self.assertIsNone(box(None))
       self.assertIsNone(unbox(None))
       self.assertIs(unbox(box(None)), None)

    def test_box_unbox_bool(self):
       self.assertIs(unbox(box(True)), True)
       self.assertIs(unbox(box(False)), False)

    def test_box_unbox_str(self):
       self.assertEqual(unbox(box("test")), "test")

    def test_box_unbox_int(self):
       with self.assertRaises(TypeError):
           box(1)

       self.assertEqual(unbox(box(1, PropertyType.UINT8)), 1)
       self.assertEqual(unbox(box(1, PropertyType.INT16)), 1)
       self.assertEqual(unbox(box(1, PropertyType.UINT16)), 1)
       self.assertEqual(unbox(box(1, PropertyType.INT32)), 1)
       self.assertEqual(unbox(box(1, PropertyType.UINT32)), 1)
       self.assertEqual(unbox(box(1, PropertyType.INT64)), 1)
       self.assertEqual(unbox(box(1, PropertyType.UINT64)), 1)

    def test_box_unbox_float(self):
       with self.assertRaises(TypeError):
           box(1.5)

       self.assertEqual(unbox(box(1.5, PropertyType.SINGLE)), 1.5)
       self.assertEqual(unbox(box(1.5, PropertyType.DOUBLE)), 1.5)

    def test_box_unbox_point(self):
       self.assertIsInstance(unbox(box(Point())), Point)

    def test_box_unbox_size(self):
       self.assertIsInstance(unbox(box(Size())), Size)

    def test_box_unbox_rect(self):
       self.assertIsInstance(unbox(box(Rect())), Rect)

    def test_box_unbox_object(self):
       obj = Uri("http://example.com")
       self.assertIs(unbox(box(obj)), obj)

    def test_box_unbox_date_time(self):
       value = datetime.datetime(2024, 1, 1, tzinfo=datetime.timezone.utc)
       self.assertEqual(unbox(box(value)), value)

    def test_box_unbox_time_span(self):
       value = datetime.timedelta()
       self.assertEqual(unbox(box(value)), value)
