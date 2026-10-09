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

    def test_memory_buffer_reference_is_not_closed_while_exported(self) -> None:
        with wf.MemoryBuffer(4) as memory_buffer:
            reference = memory_buffer.create_reference()
            view = memoryview(reference)
            view[0] = 42

            with self.assertRaises(BufferError):
                reference.close()

            with self.assertRaises(BufferError):
                reference.as_(wf.IClosable).close()

            with self.assertRaises(BufferError):
                with reference:
                    pass

        # closing the memory buffer alone leaves the memory to the reference
        self.assertEqual(len(reference), 4)
        self.assertEqual(view[0], 42)

        view.release()
        reference.close()
        self.assertEqual(len(reference), 0)

    def test_memory_buffer_reference_waits_for_every_export(self) -> None:
        with wf.MemoryBuffer(4) as memory_buffer:
            reference = memory_buffer.create_reference()

            with memoryview(reference):
                with memoryview(reference):
                    pass

                with self.assertRaises(BufferError):
                    reference.close()

            reference.close()

    @unittest.skipIf(sys.version_info < (3, 12), "requires Python 3.12 or greater")
    def test_is_collections_abc_buffer_subclass(self) -> None:
        from collections.abc import Buffer

        # https://github.com/microsoft/pyright/issues/11834
        self.assertTrue(issubclass(wss.Buffer, Buffer))  # pyright: ignore[reportGeneralTypeIssues]

        with (
            wf.MemoryBuffer(4) as memory_buffer,
            memory_buffer.create_reference() as reference,
        ):
            self.assertIsInstance(reference, Buffer)
