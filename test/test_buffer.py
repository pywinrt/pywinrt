import sys
import unittest

import winrt.windows.foundation as wf
import winrt.windows.storage.streams as wss


class TestBuffer(unittest.TestCase):
    def test_new(self) -> None:
        buf = wss.Buffer(5)
        self.assertEqual(buf.length, 0)
        self.assertEqual(buf.capacity, 5)

    def test_buffer_protocol(self) -> None:
        buf = wss.Buffer(5)
        with memoryview(buf) as mv:
            self.assertEqual(len(mv), 0)

        buf.length = 5
        with memoryview(buf) as mv:
            self.assertEqual(len(mv), 5)

    def test_memory_buffer(self) -> None:
        data = b"ABCDE"

        mb = wss.Buffer.create_memory_buffer_over_ibuffer(data)
        with mb.create_reference() as mbr, memoryview(mbr) as mv:
            self.assertEqual(mv, data)

    def test_len(self) -> None:
        buf = wss.Buffer(16)
        self.assertEqual(len(buf), 0)

        buf.length = 5
        self.assertEqual(len(buf), 5)

        with memoryview(buf) as mv:
            self.assertEqual(len(mv), len(buf))

    def test_memory_buffer_len(self) -> None:
        mb = wss.Buffer.create_memory_buffer_over_ibuffer(b"ABCDE")
        with mb.create_reference() as mbr:
            self.assertEqual(len(mbr), mbr.capacity)

            with memoryview(mbr) as mv:
                self.assertEqual(len(mv), len(mbr))

    def test_memory_buffer_len_closed(self) -> None:
        mb = wss.Buffer.create_memory_buffer_over_ibuffer(b"ABCDE")
        mbr = mb.create_reference()
        mbr.close()

        self.assertEqual(len(mbr), 0)

        with memoryview(mbr) as mv:
            self.assertEqual(len(mv), 0)

    @unittest.skipIf(sys.version_info < (3, 12), "requires Python 3.12 or greater")
    def test_is_collections_abc_buffer_subclass(self) -> None:
        from collections.abc import Buffer

        self.assertTrue(issubclass(wss.Buffer, Buffer))

        with (
            wf.MemoryBuffer(4) as memory_buffer,
            memory_buffer.create_reference() as reference,
        ):
            self.assertIsInstance(reference, Buffer)
