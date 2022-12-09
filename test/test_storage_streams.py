from datetime import datetime, timedelta, timezone
import struct
import unittest
import uuid

import winrt.windows.storage.streams as wss


class TestWindowsStorageStreams(unittest.TestCase):
    def test_DataReader(self):
        bool_value = True
        buffer_value = b"\x02\x04\x05"
        byte_value = 6
        bytes_value = b"\x07\x08\x09"
        date_time_value = 974450060070
        double_value = 10.0
        guid_value = uuid.uuid4()
        int16_value = 11
        int32_value = 12
        int64_value = 13
        single_value = 14.0
        string_value = "string"
        time_span_value = 864020000030
        uint16_value = 15
        uint32_value = 16
        uint64_value = 17

        format = "?3sB3sqdI2HQhiqf6sqHIQ"
        size = struct.calcsize(format)

        for order in wss.ByteOrder:
            with self.subTest(order=order.name):
                buf = wss.Buffer(size)
                buf.length = buf.capacity

                struct.pack_into(
                    (">" if order == wss.ByteOrder.BIG_ENDIAN else "<") + format,
                    buf,
                    0,
                    bool_value,
                    buffer_value,
                    byte_value,
                    bytes_value,
                    date_time_value,
                    double_value,
                    guid_value.time_low,
                    guid_value.time_mid,
                    guid_value.time_hi_version,
                    int.from_bytes(guid_value.bytes[8:], byteorder="little"),
                    int16_value,
                    int32_value,
                    int64_value,
                    single_value,
                    string_value.encode(),
                    time_span_value,
                    uint16_value,
                    uint32_value,
                    uint64_value,
                )

                reader = wss.DataReader.from_buffer(buf)
                reader.byte_order = order

                self.assertEqual(reader.read_boolean(), bool_value)
                self.assertEqual(bytes(reader.read_buffer(3)), buffer_value)
                self.assertEqual(reader.read_byte(), byte_value)
                self.assertEqual(reader.read_bytes(3), list(bytes_value))
                self.assertEqual(
                    reader.read_date_time(),
                    datetime(1601, 1, 2, 3, 4, 5, 6007, timezone.utc),
                )
                self.assertEqual(reader.read_double(), double_value)
                self.assertEqual(reader.read_guid(), guid_value)
                self.assertEqual(reader.read_int16(), int16_value)
                self.assertEqual(reader.read_int32(), int32_value)
                self.assertEqual(reader.read_int64(), int64_value)
                self.assertEqual(reader.read_single(), single_value)
                self.assertEqual(reader.read_string(6), string_value)
                self.assertEqual(
                    reader.read_time_span(),
                    timedelta(days=1, seconds=2, microseconds=3),
                )
                self.assertEqual(reader.read_uint16(), uint16_value)
                self.assertEqual(reader.read_uint32(), uint32_value)
                self.assertEqual(reader.read_uint64(), uint64_value)

    def test_DataWriter(self):
        bool_value = True
        buffer_value = b"\x02\x04\x05"
        byte_value = 6
        bytes_value = b"\x07\x08\x09"
        date_time_value = datetime(1601, 1, 2, 3, 4, 5, 6007, timezone.utc)
        double_value = 10.0
        guid_value = uuid.uuid4()
        int16_value = 11
        int32_value = 12
        int64_value = 13
        single_value = 14.0
        string_value = "string"
        time_span_value = timedelta(days=1, seconds=2, microseconds=3)
        uint16_value = 15
        uint32_value = 16
        uint64_value = 17

        buf = wss.Buffer(len(buffer_value))
        buf.length = buf.capacity

        with memoryview(buf) as m:
            m[:] = buffer_value

        format = "?3sB3sqdI2HQhiqf6sqHIQ"

        for byte_order in wss.ByteOrder:
            with self.subTest(byte_order=byte_order.name):
                writer = wss.DataWriter()
                writer.byte_order = byte_order

                writer.write_boolean(bool_value)
                writer.write_buffer(buf)
                writer.write_byte(byte_value)
                writer.write_bytes(bytes_value)
                writer.write_date_time(date_time_value)
                writer.write_double(double_value)
                writer.write_guid(guid_value)
                writer.write_int16(int16_value)
                writer.write_int32(int32_value)
                writer.write_int64(int64_value)
                writer.write_single(single_value)
                writer.write_string(string_value)
                writer.write_time_span(time_span_value)
                writer.write_uint16(uint16_value)
                writer.write_uint32(uint32_value)
                writer.write_uint64(uint64_value)

                for actual, expected in zip(
                    struct.unpack(
                        (">" if byte_order == wss.ByteOrder.BIG_ENDIAN else "<")
                        + format,
                        writer.detach_buffer(),
                    ),
                    (
                        bool_value,
                        buffer_value,
                        byte_value,
                        bytes_value,
                        974450060070,
                        double_value,
                        guid_value.time_low,
                        guid_value.time_mid,
                        guid_value.time_hi_version,
                        int.from_bytes(guid_value.bytes[8:], byteorder="little"),
                        int16_value,
                        int32_value,
                        int64_value,
                        single_value,
                        string_value.encode(),
                        864020000030,
                        uint16_value,
                        uint32_value,
                        uint64_value,
                    ),
                ):
                    self.assertEqual(actual, expected)
