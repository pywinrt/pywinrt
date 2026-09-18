import sys
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
        data = b"ABCDE"

        mb = wss.Buffer.create_memory_buffer_over_ibuffer(data)
        with mb.create_reference() as mbr, memoryview(mbr) as mv:
            self.assertEqual(mv, data)

    @unittest.skipIf(sys.version_info < (3, 12), "requires Python 3.12 or greater")
    def test_is_collections_abc_buffer_subclass(self):
        from collections.abc import Buffer

        self.assertTrue(issubclass(wss.Buffer, Buffer))  # type: ignore
        # FIXME: IMemoryBufferReference is not currently runtime-checkable
        # due to missing __buffer__ method.
        # self.assertTrue(issubclass(wf.IMemoryBufferReference, Buffer))  # type: ignore
