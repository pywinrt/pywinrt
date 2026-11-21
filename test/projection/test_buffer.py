import ctypes
import datetime
import sys
import threading
import unittest
import uuid
from collections.abc import Callable

import typing_extensions
import winrt.system
import winrt.windows.foundation as wf
import winrt.windows.graphics.imaging as wgi
import winrt.windows.media.core as wmc
import winrt.windows.storage.streams as wss
from winrt.runtime import interop

from ._util import async_test

IID_IBUFFER = uuid.UUID("905A0FE0-BC53-11DF-8C49-001E4FC686DA")

# The destination contexts of CoMarshalInterface: another process on this
# machine, as the Bluetooth service is, and another apartment of this one.
MSHCTX_LOCAL = 0
MSHCTX_INPROC = 3
MSHLFLAGS_NORMAL = 0
STREAM_SEEK_SET = 0

# The capsule keeps a pointer to its name, so the name has to outlive it.
INTERFACE_CAPSULE_NAME = b"winrt.interface"


def method(abi: ctypes.c_void_p, slot: int, *argtypes: type) -> Callable[..., int]:
    """
    The COM method in vtable slot ``slot`` of the interface ``abi`` points at,
    as a function that ctypes calls with the GIL released.
    """
    vtable = ctypes.cast(abi, ctypes.POINTER(ctypes.POINTER(ctypes.c_void_p)))
    prototype = ctypes.WINFUNCTYPE(ctypes.c_long, ctypes.c_void_p, *argtypes)

    return prototype(vtable[0][slot])


def python_ibuffer(data: typing_extensions.Buffer) -> wss.IBuffer:
    """
    The IBuffer that PyWinRT makes of ``data``, as WinRT holds it.
    """
    # MediaStreamSample hands back the IBuffer it was made from.
    return wmc.MediaStreamSample.create_from_buffer(data, datetime.timedelta()).buffer


def marshal_round_trip(buffer: wss.IBuffer, context: int) -> wss.IBuffer:
    """
    ``buffer`` marshaled for ``context`` and unmarshaled again, as COM passes it
    to another process or apartment, on this thread with the GIL released.
    """
    ole32 = ctypes.oledll.ole32
    iid = (ctypes.c_ubyte * 16).from_buffer_copy(IID_IBUFFER.bytes_le)
    source = interop.as_interface(buffer, IID_IBUFFER)

    stream = ctypes.c_void_p()
    ole32.CreateStreamOnHGlobal(None, True, ctypes.byref(stream))

    unmarshaled = ctypes.c_void_p()
    try:
        ole32.CoMarshalInterface(
            stream,
            ctypes.byref(iid),
            ctypes.c_void_p(interop.interface_pointer(source)),
            context,
            None,
            MSHLFLAGS_NORMAL,
        )

        seek = method(stream, 5, ctypes.c_longlong, ctypes.c_ulong, ctypes.c_void_p)
        if seek(stream, 0, STREAM_SEEK_SET, None) != 0:
            raise OSError("IStream::Seek failed")

        ole32.CoUnmarshalInterface(stream, ctypes.byref(iid), ctypes.byref(unmarshaled))
    finally:
        method(stream, 2)(stream)

    try:
        new_capsule = ctypes.pythonapi.PyCapsule_New
        new_capsule.restype = ctypes.py_object
        new_capsule.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_void_p]
        capsule: interop.InterfaceCapsule = new_capsule(
            unmarshaled, INTERFACE_CAPSULE_NAME, None
        )

        return interop.wrap_interface(capsule, wss.IBuffer)
    finally:
        method(unmarshaled, 2)(unmarshaled)


class TestBufferMarshaling(unittest.TestCase):
    """
    A Python buffer that WinRT holds as an IBuffer marshals by value, as a
    Windows.Storage.Streams.Buffer does: another process gets a copy of its
    bytes, which a service that never calls back into this one can read, and
    another apartment of this one shares its memory.
    """

    def assert_copied(
        self, data: typing_extensions.Buffer, expected: bytes
    ) -> wss.IBuffer:
        original = python_ibuffer(data)

        copy = marshal_round_trip(original, MSHCTX_LOCAL)

        self.assertEqual(len(copy), len(expected))
        self.assertEqual(copy.capacity, len(expected))
        self.assertEqual(bytes(copy), expected)

        # a copy, not the Python buffer: it is writable even where the Python
        # buffer is not, and writing into it leaves the original as it was
        with memoryview(copy) as view:
            self.assertFalse(view.readonly)
            view[0] ^= 0xFF

        self.assertEqual(bytes(original), expected)
        self.assertEqual(bytes(data), expected)

        return copy

    def test_bytes_marshals_by_value(self) -> None:
        self.assert_copied(b"abc", b"abc")

    def test_bytearray_marshals_by_value(self) -> None:
        data = bytearray(b"abc")
        copy = self.assert_copied(data, b"abc")

        # nor does the copy see what is written into the original
        data[1] = ord("B")
        self.assertEqual(bytes(copy)[1:], b"bc")

    def test_memoryview_marshals_by_value(self) -> None:
        backing = bytearray(b"xabcx")
        data = memoryview(backing)[1:4]
        copy = self.assert_copied(data, b"abc")

        backing[2] = ord("B")
        self.assertEqual(bytes(copy)[1:], b"bc")

    def test_length_marshals_with_the_bytes(self) -> None:
        data = bytearray(b"abcde")
        original = python_ibuffer(data)
        original.length = 2

        copy = marshal_round_trip(original, MSHCTX_LOCAL)

        self.assertEqual(len(copy), 2)
        self.assertEqual(bytes(copy), b"ab")

    def test_marshals_in_process_by_reference(self) -> None:
        data = bytearray(b"abc")
        original = python_ibuffer(data)

        shared = marshal_round_trip(original, MSHCTX_INPROC)

        self.assertEqual(bytes(shared), b"abc")

        with memoryview(shared) as view:
            view[0] = ord("A")

        self.assertEqual(data, b"Abc")

        data[1] = ord("B")
        self.assertEqual(bytes(shared), b"ABc")

    def test_marshals_on_threads_without_the_gil(self) -> None:
        # COM calls the marshaling methods, and through them the buffer's
        # Length, Capacity and Buffer, on whatever thread marshals it, which
        # need not hold the GIL. Here each thread marshals with it released,
        # while the others run Python, and lets go of its copy again.
        data = bytearray(b"abc")
        original = python_ibuffer(data)
        threads = 8
        barrier = threading.Barrier(threads)
        errors: list[BaseException] = []

        def work() -> None:
            try:
                barrier.wait()
                for _ in range(50):
                    self.assertEqual(
                        bytes(marshal_round_trip(original, MSHCTX_LOCAL)), b"abc"
                    )
            except BaseException as e:
                errors.append(e)

        workers = [threading.Thread(target=work) for _ in range(threads)]
        for t in workers:
            t.start()
        for t in workers:
            t.join()

        if errors:
            raise errors[0]

        self.assertEqual(data, b"abc")


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

    @async_test
    async def test_python_buffer_takes_the_length_winrt_sets(self) -> None:
        stream = wss.InMemoryRandomAccessStream()
        await stream.write_async(b"hi")
        stream.seek(0)

        target = bytearray(b"xxxxx")
        read = await stream.read_async(target, len(target), wss.InputStreamOptions.NONE)

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

    def test_wrapped_buffer_winrt_fills_refuses_read_only(self) -> None:
        # MediaStreamSample hands back the IBuffer it was made from, which is
        # the Python buffer as WinRT holds it.
        data = b"abc"
        sample = wmc.MediaStreamSample.create_from_buffer(data, datetime.timedelta())

        with self.assertRaisesRegex(
            TypeError,
            r"^create_memory_buffer_over_ibuffer\(\) argument 1 must be read-write "
            r"bytes-like object, not a read-only Python buffer wrapped as an IBuffer$",
        ):
            wss.Buffer.create_memory_buffer_over_ibuffer(sample.buffer)

        self.assertEqual(data, b"abc")

    def test_wrapped_writable_buffer_winrt_fills(self) -> None:
        target = bytearray(b"abc")
        sample = wmc.MediaStreamSample.create_from_buffer(target, datetime.timedelta())

        mb = wss.Buffer.create_memory_buffer_over_ibuffer(sample.buffer)
        with memoryview(mb) as view:
            view[0] = ord("A")

        self.assertEqual(target, b"Abc")

    def test_read_only_buffer_refuses_a_length(self) -> None:
        # MediaStreamSample hands back the IBuffer it was made from, which is
        # how a writer that no rule names would find it.
        sample = wmc.MediaStreamSample.create_from_buffer(b"abc", datetime.timedelta())
        buffer = sample.buffer

        with self.assertRaises(PermissionError):
            buffer.length = 1

        self.assertEqual(len(buffer), 3)

        sample = wmc.MediaStreamSample.create_from_buffer(
            bytearray(b"abc"), datetime.timedelta()
        )
        buffer = sample.buffer

        buffer.length = 1
        self.assertEqual(bytes(buffer), b"a")

    def test_python_buffer_keeps_its_read_only_view(self) -> None:
        # MediaStreamSample hands back the IBuffer it was made from, which is
        # the Python buffer as WinRT holds it.
        data = b"abc"

        sample = wmc.MediaStreamSample.create_from_buffer(data, datetime.timedelta())
        with memoryview(sample.buffer) as view:
            self.assertTrue(view.readonly)

            with self.assertRaises(TypeError):
                view[0] = ord("A")

        self.assertEqual(data, b"abc")

        target = bytearray(b"abc")

        sample = wmc.MediaStreamSample.create_from_buffer(target, datetime.timedelta())
        with memoryview(sample.buffer) as view:
            self.assertFalse(view.readonly)
            view[0] = ord("A")

        self.assertEqual(target, b"Abc")

    def test_memory_buffer_holds_the_python_buffer(self) -> None:
        data = bytearray(b"ABCDE")

        mb = wss.Buffer.create_memory_buffer_over_ibuffer(data)

        with self.assertRaises(BufferError):
            data.append(ord("F"))

        with memoryview(mb) as view:
            view[0] = ord("a")

        self.assertEqual(data, b"aBCDE")

        del mb
        data.append(ord("F"))

        self.assertEqual(data, b"aBCDEF")

    def test_memory_buffer(self) -> None:
        data = bytearray(b"ABCDE")

        mb = wss.Buffer.create_memory_buffer_over_ibuffer(data)
        with mb.create_reference() as mbr, memoryview(mbr) as mv:  # type: ignore[deprecated]
            self.assertEqual(mv, data)

            mv[0] = ord("a")

        self.assertEqual(data, b"aBCDE")

    def test_memory_buffer_over_ibuffer_refuses_read_only(self) -> None:
        # the memory buffer would let anyone write into the bytes
        target = b"ABCDE"

        with self.assertRaisesRegex(
            TypeError,
            r"^create_memory_buffer_over_ibuffer\(\) argument 1 must be read-write "
            r"bytes-like object, not bytes$",
        ):
            wss.Buffer.create_memory_buffer_over_ibuffer(target)

        with self.assertRaises(TypeError):
            wss.Buffer.create_memory_buffer_over_ibuffer(
                memoryview(bytearray(5)).toreadonly()
            )

    def test_len(self) -> None:
        buf = wss.Buffer(16)
        self.assertEqual(len(buf), 0)

        buf.length = 5
        self.assertEqual(len(buf), 5)

        with memoryview(buf) as mv:
            self.assertEqual(len(mv), len(buf))

    def test_memory_buffer_len(self) -> None:
        mb = wss.Buffer.create_memory_buffer_over_ibuffer(bytearray(b"ABCDE"))
        with mb.create_reference() as mbr:  # type: ignore[deprecated]
            self.assertEqual(len(mbr), mbr.capacity)

            with memoryview(mbr) as mv:
                self.assertEqual(len(mv), len(mbr))

    def test_memory_buffer_len_closed(self) -> None:
        mb = wss.Buffer.create_memory_buffer_over_ibuffer(bytearray(b"ABCDE"))
        mbr = mb.create_reference()  # type: ignore[deprecated]
        mbr.close()

        self.assertEqual(len(mbr), 0)

        with memoryview(mbr) as mv:
            self.assertEqual(len(mv), 0)

    def test_memory_buffer_reference_is_not_closed_while_exported(self) -> None:
        with wf.MemoryBuffer(4) as memory_buffer:
            reference = memory_buffer.create_reference()  # type: ignore[deprecated]
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
            reference = memory_buffer.create_reference()  # type: ignore[deprecated]

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

    def test_with_closes_the_memory_buffer(self) -> None:
        memory_buffer = wf.MemoryBuffer(4)

        with memory_buffer as entered:
            self.assertIs(entered, memory_buffer)

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

    def test_system_buffer_is_deprecated(self) -> None:
        with self.assertWarnsRegex(DeprecationWarning, "use collections.abc.Buffer"):
            from winrt.system import Buffer  # pyright: ignore[reportDeprecated]

        self.assertIs(Buffer, typing_extensions.Buffer)  # pyright: ignore[reportDeprecated]
        self.assertNotIn("Buffer", winrt.system.__all__)

    @unittest.skipIf(sys.version_info < (3, 12), "requires Python 3.12 or greater")
    def test_is_collections_abc_buffer_subclass(self) -> None:
        from collections.abc import Buffer

        # https://github.com/microsoft/pyright/issues/11834
        self.assertTrue(issubclass(wss.Buffer, Buffer))  # pyright: ignore[reportGeneralTypeIssues]

        with (
            wf.MemoryBuffer(4) as memory_buffer,
            memory_buffer.create_reference() as reference,  # type: ignore[deprecated]
        ):
            self.assertIsInstance(memory_buffer, Buffer)
            self.assertIsInstance(reference, Buffer)
