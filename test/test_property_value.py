
import unittest
from array import array
from datetime import datetime, timedelta, timezone
from uuid import UUID, uuid4

from winrt.system import Array
import winrt.windows.foundation as wf

class TestPropertyValue(unittest.TestCase):

    def test_create_empty(self):
        o = wf.PropertyValue.create_empty()
        self.assertIsNone(o)

    def test_create_uint8(self):
        o = wf.PropertyValue.create_uint8(250)
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.UINT8)
        self.assertTrue(ipv.get_uint8(), 250)
        self.assertRaises(OverflowError, lambda: wf.PropertyValue.create_uint8(2**8))
        self.assertRaises(OverflowError, lambda: wf.PropertyValue.create_uint8(-1))

    def test_create_int16(self):
        o = wf.PropertyValue.create_int16(-32000)
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.INT16)
        self.assertTrue(ipv.get_int16(), -32000)
        self.assertRaises(OverflowError, lambda: wf.PropertyValue.create_int16(2**15))
        self.assertRaises(OverflowError, lambda: wf.PropertyValue.create_int16(-(2**15 + 1)))

    def test_create_uint16(self):
        o = wf.PropertyValue.create_uint16(65000)
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.UINT16)
        self.assertTrue(ipv.get_uint16(), 65000)
        self.assertRaises(OverflowError, lambda: wf.PropertyValue.create_uint16(2**16))
        self.assertRaises(OverflowError, lambda: wf.PropertyValue.create_uint16(-1))

    def test_create_int32(self):
        o = wf.PropertyValue.create_int32(-2147483640)
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.INT32)
        self.assertTrue(ipv.get_int32(), -2147483640)
        self.assertRaises(OverflowError, lambda: wf.PropertyValue.create_int32(2**31))
        self.assertRaises(OverflowError, lambda: wf.PropertyValue.create_int32(-(2**31 + 1)))

    def test_create_uint32(self):
        o = wf.PropertyValue.create_uint32(4294967290)
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.UINT32)
        self.assertTrue(ipv.get_uint32(), 4294967290)
        self.assertRaises(OverflowError, lambda: wf.PropertyValue.create_uint32(2**32))
        self.assertRaises(OverflowError, lambda: wf.PropertyValue.create_uint32(-1))

    def test_create_int64(self):
        o = wf.PropertyValue.create_int64(-9223372036854775800)
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.INT64)
        self.assertTrue(ipv.get_int64(), -9223372036854775800)
        self.assertRaises(OverflowError, lambda: wf.PropertyValue.create_int32(2**63))
        self.assertRaises(OverflowError, lambda: wf.PropertyValue.create_int32(-(2**63 + 1)))

    def test_create_uint64(self):
        o = wf.PropertyValue.create_uint64(18446744073709551610)
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.UINT64)
        self.assertTrue(ipv.get_uint64(), 18446744073709551610)
        self.assertRaises(OverflowError, lambda: wf.PropertyValue.create_uint64(2**64))
        self.assertRaises(OverflowError, lambda: wf.PropertyValue.create_uint64(-1))

    def test_create_single(self):
        o = wf.PropertyValue.create_single(3.14)
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.SINGLE)
        self.assertAlmostEqual(ipv.get_single(), 3.14, 5)

    def test_create_double(self):
        o = wf.PropertyValue.create_double(3.14)
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.DOUBLE)
        self.assertEqual(ipv.get_double(), 3.14)

    def test_create_char16(self):
        o = wf.PropertyValue.create_char16("c")
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.CHAR16)
        self.assertEqual(ipv.get_char16(), "c")

    def test_create_boolean(self):
       o = wf.PropertyValue.create_boolean(True)
       ipv = wf.IPropertyValue._from(o)
       self.assertEqual(ipv.type, wf.PropertyType.BOOLEAN)
       self.assertTrue(ipv.get_boolean())

    def test_create_string(self):
        o = wf.PropertyValue.create_string("Ni!")
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.STRING)
        self.assertEqual(ipv.get_string(), "Ni!")

    # # TODO: CreateInspectable

    def test_create_datetime(self):
        now = datetime.now(timezone.utc)
        o = wf.PropertyValue.create_date_time(now)
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.DATE_TIME)
        self.assertEqual(ipv.get_date_time(), now)

    def test_create_TimeSpan(self):
        td = timedelta(days=-1, seconds=2, microseconds=3)
        o = wf.PropertyValue.create_time_span(td)
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.TIME_SPAN)
        self.assertEqual(ipv.get_time_span(), td)

    def test_create_Guid(self):
        u = UUID('01234567-89ab-cdef-0123456789abcdef')
        o = wf.PropertyValue.create_guid(u)
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.GUID)
        self.assertEqual(ipv.get_guid(), u)

    def test_create_Point(self):
        o = wf.PropertyValue.create_point(wf.Point(2, 4))
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.POINT)
        s = ipv.get_point()
        self.assertEqual(s.x, 2)
        self.assertEqual(s.y, 4)

    def test_create_Size(self):
        o = wf.PropertyValue.create_size(wf.Size(2, 4))
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.SIZE)
        s = ipv.get_size()
        self.assertEqual(s.width, 2)
        self.assertEqual(s.height, 4)

    def test_create_Rect(self):
        o = wf.PropertyValue.create_rect(wf.Rect(2, 4, 6, 8))
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.RECT)
        s = ipv.get_rect()
        self.assertEqual(s.x, 2)
        self.assertEqual(s.y, 4)
        self.assertEqual(s.width, 6)
        self.assertEqual(s.height, 8)

    def test_create_uint8_array(self):
        o = wf.PropertyValue.create_uint8_array(array('B', [1, 2, 3, 4, 5]))
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.UINT8_ARRAY)
        a = ipv.get_uint8_array()
        self.assertEqual(len(a), 5)
        for x in range(5):
            self.assertEqual(a[x], x+1)

    def test_create_int16_array(self):
        o = wf.PropertyValue.create_int16_array(array('h', [1, 2, 3, 4, 5]))
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.INT16_ARRAY)
        a = ipv.get_int16_array()
        self.assertEqual(len(a), 5)
        for x in range(5):
            self.assertEqual(a[x], x+1)

    def test_create_uint16_array(self):
        o = wf.PropertyValue.create_uint16_array(array('H', [1, 2, 3, 4, 5]))
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.UINT16_ARRAY)
        a = ipv.get_uint16_array()
        self.assertEqual(len(a), 5)
        for x in range(5):
            self.assertEqual(a[x], x+1)

    def test_create_int32_array(self):
        o = wf.PropertyValue.create_int32_array(array('i', [1, 2, 3, 4, 5]))
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.INT32_ARRAY)
        a = ipv.get_int32_array()
        self.assertEqual(len(a), 5)
        for x in range(5):
            self.assertEqual(a[x], x+1)

    def test_create_uint32_array(self):
        o = wf.PropertyValue.create_uint32_array(array('I', [1, 2, 3, 4, 5]))
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.UINT32_ARRAY)
        a = ipv.get_uint32_array()
        self.assertEqual(len(a), 5)
        for x in range(5):
            self.assertEqual(a[x], x+1)

    def test_create_int64_array(self):
        o = wf.PropertyValue.create_int64_array(array('q', [1, 2, 3, 4, 5]))
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.INT64_ARRAY)
        a = ipv.get_int64_array()
        self.assertEqual(len(a), 5)
        for x in range(5):
            self.assertEqual(a[x], x+1)

    def test_create_uint64_array(self):
        o = wf.PropertyValue.create_uint64_array(array('Q', [1, 2, 3, 4, 5]))
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.UINT64_ARRAY)
        a = ipv.get_uint64_array()
        self.assertEqual(len(a), 5)
        for x in range(5):
            self.assertEqual(a[x], x+1)

    def test_create_double_array(self):
        o = wf.PropertyValue.create_double_array(array('d', [1, 2, 3, 4, 5]))
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.DOUBLE_ARRAY)
        a = ipv.get_double_array()
        self.assertEqual(len(a), 5)
        for x in range(5):
            self.assertEqual(a[x], x+1)

    def test_create_single_array(self):
        o = wf.PropertyValue.create_single_array(array('f', [1, 2, 3, 4, 5]))
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.SINGLE_ARRAY)
        a = ipv.get_single_array()
        self.assertEqual(len(a), 5)
        for x in range(5):
            self.assertEqual(a[x], x+1)

    def test_create_char16_array(self):
        o = wf.PropertyValue.create_char16_array(array('u', ["A", "B", "C", "D", "E"]))
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.CHAR16_ARRAY)
        a = ipv.get_char16_array()
        self.assertEqual(len(a), 5)
        for x in range(5):
            self.assertEqual(a[x], chr(x + ord("A")))

    def test_create_guid_array(self):
        uuids = [uuid4() for _ in range(5)]
        o = wf.PropertyValue.create_guid_array(Array(UUID, uuids))
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.GUID_ARRAY)
        a = ipv.get_guid_array()
        self.assertEqual(len(a), 5)
        for x in range(5):
            self.assertEqual(a[x], uuids[x])

    def test_create_date_time_array(self):
        now = [datetime.now(timezone.utc) for _ in range(5)]
        o = wf.PropertyValue.create_date_time_array(Array(datetime, now))
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.DATE_TIME_ARRAY)
        a = ipv.get_date_time_array()
        self.assertEqual(len(a), 5)
        for x in range(5):
            self.assertEqual(a[x], now[x])

    def test_create_time_span_array(self):
        times = [timedelta(x) for x in range(5)]
        o = wf.PropertyValue.create_time_span_array(Array(timedelta, times))
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.TIME_SPAN_ARRAY)
        a = ipv.get_time_span_array()
        self.assertEqual(len(a), 5)
        for x in range(5):
            self.assertEqual(a[x], times[x])

    def test_create_point_array(self):
        actual = [wf.Point(x, x+1) for x in range(5)]
        o = wf.PropertyValue.create_point_array(Array(wf.Point, actual))
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.POINT_ARRAY)
        a = ipv.get_point_array()
        self.assertEqual(len(a), 5)
        for x in range(5):
            self.assertEqual(a[x].x, actual[x].x)
            self.assertEqual(a[x].y, actual[x].y)

    def test_create_size_array(self):
        actual = [wf.Size(x, x+1) for x in range(5)]
        o = wf.PropertyValue.create_size_array(Array(wf.Size, actual))
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.SIZE_ARRAY)
        a = ipv.get_size_array()
        self.assertEqual(len(a), 5)
        for x in range(5):
            self.assertEqual(a[x].width, actual[x].width)
            self.assertEqual(a[x].height, actual[x].height)

    def test_create_rect_array(self):
        actual = [wf.Rect(x, x+1, x+2, x+3) for x in range(5)]
        o = wf.PropertyValue.create_rect_array(Array(wf.Rect, actual))
        ipv = wf.IPropertyValue._from(o)
        self.assertEqual(ipv.type, wf.PropertyType.RECT_ARRAY)
        a = ipv.get_rect_array()
        self.assertEqual(len(a), 5)
        for x in range(5):
            self.assertEqual(a[x].x, actual[x].x)
            self.assertEqual(a[x].y, actual[x].y)
            self.assertEqual(a[x].width, actual[x].width)
            self.assertEqual(a[x].height, actual[x].height)



if __name__ == '__main__':
    unittest.main()
