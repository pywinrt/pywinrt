import datetime
import sys
import unittest

import winrt.windows.foundation as wf
import winrt.windows.graphics.imaging as wgi
import winrt.windows.media.core as wmc
import winrt.windows.storage.streams as wss

from ._util import async_test


class TestBuffer(unittest.TestCase):
    def reference_of(
        self, memory_buffer: wf.IMemoryBuffer
    ) -> wf.IMemoryBufferReference:
        """
        A reference to the memory of a memory buffer, made with the deprecated
        create_reference(). The reference's own buffer export stays, since it
        is the only one WebView2's CoreWebView2SharedBuffer.buffer has.
        """
        with self.assertWarns(DeprecationWarning):
            return memory_buffer.create_reference()  # type: ignore[deprecated]

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

    @async_test
    async def test_python_buffer_takes_the_length_winrt_sets(self) -> None:
        stream = wss.InMemoryRandomAccessStream()
        await stream.write_async(b"hi")
        stream.seek(0)

        target = bytearray(b"xxxxx")
        read = await stream.read_async(target, len(target), wss.InputStreamOptions.NONE)
        assert isinstance(read, wss.IBuffer)

        self.assertEqual(target, b"hixxx")
        self.assertEqual(read.capacity, 5)
        self.assertEqual(len(read), 2)
        self.assertEqual(bytes(read), b"hi")

        read.length = 5
        self.assertEqual(bytes(read), b"hixxx")

        with self.assertRaises(OSError):
            read.length = 6

        self.assertEqual(len(read), 5)

    @async_test
    async def test_buffer_winrt_fills_refuses_read_only(self) -> None:
        stream = wss.InMemoryRandomAccessStream()
        await stream.write_async(b"hi")
        stream.seek(0)

        target = bytes(5)

        with self.assertRaisesRegex(
            TypeError,
            r"^read_async\(\) argument 1 must be read-write bytes-like object, "
            r"not bytes$",
        ):
            await stream.read_async(target, 5, wss.InputStreamOptions.NONE)

        with self.assertRaises(TypeError):
            await stream.read_async(
                memoryview(bytearray(5)).toreadonly(), 5, wss.InputStreamOptions.NONE
            )

        self.assertEqual(target, bytes(5))

    def test_read_only_buffer_refuses_a_length(self) -> None:
        # MediaStreamSample hands back the IBuffer it was made from, which is
        # how a writer that no rule names would find it.
        sample = wmc.MediaStreamSample.create_from_buffer(b"abc", datetime.timedelta())
        buffer = sample.buffer
        assert isinstance(buffer, wss.IBuffer)

        with self.assertRaises(PermissionError):
            buffer.length = 1

        self.assertEqual(len(buffer), 3)

        sample = wmc.MediaStreamSample.create_from_buffer(
            bytearray(b"abc"), datetime.timedelta()
        )
        buffer = sample.buffer
        assert isinstance(buffer, wss.IBuffer)

        buffer.length = 1
        self.assertEqual(bytes(buffer), b"a")

    def test_memory_buffer(self) -> None:
        data = b"ABCDE"

        mb = wss.Buffer.create_memory_buffer_over_ibuffer(data)
        with self.reference_of(mb) as mbr, memoryview(mbr) as mv:
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
        with self.reference_of(mb) as mbr:
            self.assertEqual(len(mbr), mbr.capacity)

            with memoryview(mbr) as mv:
                self.assertEqual(len(mv), len(mbr))

    def test_memory_buffer_len_closed(self) -> None:
        mb = wss.Buffer.create_memory_buffer_over_ibuffer(b"ABCDE")
        mbr = self.reference_of(mb)
        mbr.close()

        self.assertEqual(len(mbr), 0)

        with memoryview(mbr) as mv:
            self.assertEqual(len(mv), 0)

    def test_memory_buffer_reference_is_not_closed_while_exported(self) -> None:
        with wf.MemoryBuffer(4) as memory_buffer:
            reference = self.reference_of(memory_buffer)
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
            reference = self.reference_of(memory_buffer)

            with memoryview(reference):
                with memoryview(reference):
                    pass

                with self.assertRaises(BufferError):
                    reference.close()

            reference.close()

    def test_memory_buffer_exports_a_buffer(self) -> None:
        with wf.MemoryBuffer(4) as memory_buffer:
            with memoryview(memory_buffer) as view:
                self.assertEqual(len(view), 4)
                view[0] = 42

            with memoryview(memory_buffer) as view:
                self.assertEqual(view[0], 42)

    def test_memory_buffer_view_outlives_the_buffer(self) -> None:
        memory_buffer = wf.MemoryBuffer(4)
        view = memoryview(memory_buffer)
        view[0] = 42

        memory_buffer.close()
        del memory_buffer

        self.assertEqual(view[0], 42)
        view.release()

    def test_closed_memory_buffer_exports_nothing(self) -> None:
        memory_buffer = wf.MemoryBuffer(4)
        memory_buffer.close()

        with memoryview(memory_buffer) as view:
            self.assertEqual(len(view), 0)

    def test_bitmap_stays_locked_while_a_view_exists(self) -> None:
        bitmap = wgi.SoftwareBitmap(wgi.BitmapPixelFormat.BGRA8, 2, 2)

        with bitmap.lock_buffer(wgi.BitmapBufferAccessMode.READ_WRITE) as locked:
            view = memoryview(locked)

        self.assertEqual(len(view), 2 * 2 * 4)

        with self.assertRaises(PermissionError):
            bitmap.lock_buffer(wgi.BitmapBufferAccessMode.READ)

        view.release()
        bitmap.lock_buffer(wgi.BitmapBufferAccessMode.READ).close()

    def test_create_reference_is_deprecated(self) -> None:
        bitmap = wgi.SoftwareBitmap(wgi.BitmapPixelFormat.BGRA8, 2, 2)

        with bitmap.lock_buffer(wgi.BitmapBufferAccessMode.READ) as locked:
            with self.assertWarnsRegex(
                DeprecationWarning, "use the IMemoryBuffer itself as a buffer"
            ):
                locked.create_reference().close()  # type: ignore[deprecated]

    @unittest.skipIf(sys.version_info < (3, 12), "requires Python 3.12 or greater")
    def test_is_collections_abc_buffer_subclass(self) -> None:
        from collections.abc import Buffer

        # https://github.com/microsoft/pyright/issues/11834
        self.assertTrue(issubclass(wss.Buffer, Buffer))  # pyright: ignore[reportGeneralTypeIssues]

        with (
            wf.MemoryBuffer(4) as memory_buffer,
            self.reference_of(memory_buffer) as reference,
        ):
            self.assertIsInstance(memory_buffer, Buffer)
            self.assertIsInstance(reference, Buffer)
