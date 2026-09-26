"""
The functions the interop packages reach the runtime through.

An interop package's compiled code shares no C ABI with winrt-runtime. It
takes a WinRT object's interface as an interface pointer capsule from
``as_interface()``, hands one back to ``wrap_interface()`` to be made into a
projected object, and raises what ``hresult_error()`` builds for a failed call.
"""

import ctypes
import datetime
import unittest
import uuid

import winrt._winrt as runtime
from winrt.windows.foundation import Uri

IID_IINSPECTABLE = uuid.UUID("AF86E2E0-B12D-4C6A-9C5A-D7AA65101E90")
IID_IUNKNOWN = uuid.UUID("00000000-0000-0000-C000-000000000046")
# IStringable, which a Uri implements and a PropertyValue does not
IID_ISTRINGABLE = uuid.UUID("96369F54-8EB6-48F0-ABCE-C1B211E627C3")

E_FAIL = -2147467259
E_INVALIDARG = -2147024809

# The capsule keeps a pointer to its name, so the name has to outlive it.
INTERFACE_CAPSULE_NAME = b"winrt.interface"


class ErrorInfo:
    """
    The error info that originating ``hresult`` with ``message`` leaves on this
    thread, taken the way an interop extension takes it, as an interface
    pointer capsule that does not release it, so that the test can.
    """

    def __init__(self, hresult: int, message: str) -> None:
        combase = ctypes.windll.combase

        hstring = ctypes.c_void_p()
        if combase.WindowsCreateString(message, len(message), ctypes.byref(hstring)):
            raise OSError("WindowsCreateString failed")

        try:
            combase.RoOriginateError(ctypes.c_long(hresult), hstring)
        finally:
            combase.WindowsDeleteString(hstring)

        self.abi = ctypes.c_void_p()
        if ctypes.windll.oleaut32.GetErrorInfo(0, ctypes.byref(self.abi)) != 0:
            raise OSError("GetErrorInfo found nothing")

        new_capsule = ctypes.pythonapi.PyCapsule_New
        new_capsule.restype = ctypes.py_object
        new_capsule.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_void_p]
        self.capsule = new_capsule(self.abi, INTERFACE_CAPSULE_NAME, None)

    def release(self) -> None:
        vtable = ctypes.cast(self.abi, ctypes.POINTER(ctypes.POINTER(ctypes.c_void_p)))
        release = ctypes.WINFUNCTYPE(ctypes.c_ulong, ctypes.c_void_p)(vtable[0][2])
        release(self.abi)


class TestInterfaceCapsule(unittest.TestCase):
    def test_round_trip(self) -> None:
        uri = Uri("https://example.com/")

        capsule = runtime.as_interface(uri, IID_IINSPECTABLE)
        wrapped = runtime.wrap_interface(capsule, "winrt.windows.foundation.Uri")

        self.assertIsInstance(wrapped, Uri)
        self.assertEqual(wrapped, uri)
        self.assertEqual(wrapped.absolute_uri, "https://example.com/")

    def test_capsule_holds_its_own_reference(self) -> None:
        capsule = runtime.as_interface(Uri("https://example.com/"), IID_IUNKNOWN)

        wrapped = runtime.wrap_interface(capsule, "winrt.windows.foundation.Uri")
        del capsule

        self.assertEqual(wrapped.absolute_uri, "https://example.com/")

    def test_none(self) -> None:
        self.assertIsNone(runtime.as_interface(None, IID_IINSPECTABLE))
        self.assertIsNone(runtime.wrap_interface(None, "winrt.windows.foundation.Uri"))

    def test_not_a_winrt_object(self) -> None:
        with self.assertRaisesRegex(TypeError, "expected a WinRT object"):
            runtime.as_interface(object(), IID_IINSPECTABLE)  # type: ignore[call-overload]

    def test_interface_not_implemented(self) -> None:
        boxed = runtime.box_int32(1)

        with self.assertRaises(OSError) as cm:
            runtime.as_interface(boxed, IID_ISTRINGABLE)

        self.assertEqual(cm.exception.winerror, -2147467262)  # E_NOINTERFACE

    def test_wrong_capsule_name(self) -> None:
        with self.assertRaises(ValueError):
            runtime.wrap_interface(
                datetime.datetime_CAPI,  # type: ignore[attr-defined]
                "winrt.windows.foundation.Uri",
            )


class TestHresultError(unittest.TestCase):
    def test_hresult(self) -> None:
        error = runtime.hresult_error(E_INVALIDARG)

        self.assertIs(type(error), OSError)
        self.assertEqual(error.winerror, E_INVALIDARG)
        self.assertEqual(error.strerror, "The parameter is incorrect.")

    def test_unsigned_hresult(self) -> None:
        error = runtime.hresult_error(0x80070057)

        self.assertEqual(error.winerror, E_INVALIDARG)

    def test_not_a_failure(self) -> None:
        with self.assertRaises(ValueError):
            runtime.hresult_error(0)

        with self.assertRaises(OverflowError):
            runtime.hresult_error(1 << 32)

    def test_error_info(self) -> None:
        info = ErrorInfo(E_FAIL, "the details")
        try:
            error = runtime.hresult_error(E_FAIL, info.capsule)
        finally:
            info.release()

        self.assertEqual(error.winerror, E_FAIL)
        self.assertEqual(error.strerror, "the details")

    def test_error_info_about_another_hresult(self) -> None:
        info = ErrorInfo(E_FAIL, "the details")
        try:
            error = runtime.hresult_error(E_INVALIDARG, info.capsule)
        finally:
            info.release()

        self.assertEqual(error.winerror, E_INVALIDARG)
        self.assertEqual(error.strerror, "The parameter is incorrect.")

    def test_not_error_info(self) -> None:
        capsule = runtime.as_interface(Uri("https://example.com/"), IID_IINSPECTABLE)

        with self.assertRaises(TypeError):
            runtime.hresult_error(E_FAIL, capsule)
