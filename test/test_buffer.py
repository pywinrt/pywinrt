
import unittest

import winrt.windows.foundation as wf
import winrt.windows.storage.streams as wss


class TestBuffer(unittest.TestCase):
    def test_new(self):
        buf = wss.Buffer(5)
        self.assertEqual(buf.length, 0)
        self.assertEqual(buf.capacity, 5)

    def test_buffer_protocol(self):
        buf = wss.Buffer(5)
        with memoryview(buf) as mv:
            self.assertEqual(len(mv), 0)
        
        buf.length = 5
        with memoryview(buf) as mv:
            self.assertEqual(len(mv), 5)

    def test_memory_buffer(self):
        buf = wss.Buffer(5)
        buf.length =  5
        data = b"ABCDE"

        with memoryview(buf) as mv:
            mv[:] = data

        mb = wss.Buffer.create_memory_buffer_over_i_buffer(buf)
        with mb.create_reference() as mbr, memoryview(mbr) as mv:
            self.assertEqual(mv, data)

if __name__ == '__main__':
    unittest.main()
