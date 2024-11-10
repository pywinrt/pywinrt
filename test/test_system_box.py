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


class TestSystemBox(unittest.TestCase):
    def test_box_unbox_boolean(self):
        self.assertIs(unbox_boolean(box_boolean(True)), True)
        self.assertIs(unbox_boolean(box_boolean(False)), False)

    def test_box_unbox_char16(self):
        self.assertEqual(unbox_char16(box_char16("\u01bf")), "ƿ")
        self.assertRaises(TypeError, box_char16, "test")

    def test_box_unbox_string(self):
        self.assertEqual(unbox_string(box_string("test\N{GRINNING FACE}")), "test😀")
        self.assertRaises(TypeError, box_string, 1)

    def test_box_unbox_int8(self):
        self.assertEqual(unbox_int8(box_int8(1)), 1)

    def test_box_unbox_uint8(self):
        self.assertEqual(unbox_uint8(box_uint8(1)), 1)

    def test_box_unbox_int16(self):
        self.assertEqual(unbox_int16(box_int16(1)), 1)

    def test_box_unbox_uint16(self):
        self.assertEqual(unbox_uint16(box_uint16(1)), 1)

    def test_box_unbox_int32(self):
        self.assertEqual(unbox_int32(box_int32(1)), 1)

    def test_box_unbox_uint32(self):
        self.assertEqual(unbox_uint32(box_uint32(1)), 1)

    def test_box_unbox_int64(self):
        self.assertEqual(unbox_int64(box_int64(1)), 1)

    def test_box_unbox_uint64(self):
        self.assertEqual(unbox_uint64(box_uint64(1)), 1)

    def test_box_unbox_single(self):
        self.assertEqual(unbox_single(box_single(1.5)), 1.5)

    def test_box_unbox_double(self):
        self.assertEqual(unbox_double(box_double(1.5)), 1.5)

    def test_box_unbox_guid(self):
        value = uuid.uuid4()
        self.assertEqual(unbox_guid(box_guid(value)), value)

    def test_box_unbox_date_time(self):
        value = datetime.datetime(2024, 1, 1, tzinfo=datetime.timezone.utc)
        self.assertEqual(unbox_date_time(box_date_time(value)), value)

    def test_box_unbox_time_span(self):
        value = datetime.timedelta(1)
        self.assertEqual(unbox_time_span(box_time_span(value)), value)
