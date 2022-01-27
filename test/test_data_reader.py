import struct
import unittest

import winrt.windows.storage.streams as wss


class TestBuffer(unittest.TestCase):
    def test_from_buffer(self):
        SIZE = 5

        buf = wss.Buffer(SIZE)
        buf.length = SIZE
        struct.pack_into("b" * SIZE, buf, 0, *range(SIZE))

        reader = wss.DataReader.from_buffer(buf)

        for i in range(SIZE):
            self.assertEqual(reader.read_byte(), i)

if __name__ == '__main__':
    unittest.main()
