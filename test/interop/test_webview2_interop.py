import contextlib
import ctypes
import threading
import unittest
from collections.abc import Generator
from ctypes import wintypes

import winrt._winrt_microsoft_web_webview2_interop as native
import winrt.microsoft.web.webview2.interop as interop
import winrt.runtime.interop as runtime
from winrt.windows.foundation import Uri

_user32 = ctypes.WinDLL("user32", use_last_error=True)

_CreateWindowExW = _user32.CreateWindowExW
_CreateWindowExW.restype = wintypes.HWND
_CreateWindowExW.argtypes = [
    wintypes.DWORD,
    wintypes.LPCWSTR,
    wintypes.LPCWSTR,
    wintypes.DWORD,
    ctypes.c_int,
    ctypes.c_int,
    ctypes.c_int,
    ctypes.c_int,
    wintypes.HWND,
    wintypes.HMENU,
    wintypes.HINSTANCE,
    wintypes.LPVOID,
]

_DestroyWindow = _user32.DestroyWindow
_DestroyWindow.restype = wintypes.BOOL
_DestroyWindow.argtypes = [wintypes.HWND]

# the signed HRESULTs that OSError.winerror holds
E_NOINTERFACE = -2147467262
# https://learn.microsoft.com/en-us/windows/win32/com/com-error-codes-1
DRAGDROP_E_NOTREGISTERED = -2147221248


def _new_window() -> int:
    """
    A window of the calling thread that is never shown.
    """
    hwnd = _CreateWindowExW(0, "Button", None, 0, 0, 0, 0, 0, None, None, None, None)
    if not hwnd:
        raise ctypes.WinError(ctypes.get_last_error())

    return hwnd


def _create_window(test: unittest.TestCase) -> int:
    """
    A window of the calling thread, destroyed when the test ends.
    """
    hwnd = _new_window()
    test.addCleanup(_DestroyWindow, hwnd)

    return hwnd


@contextlib.contextmanager
def _window_of_another_thread() -> Generator[int]:
    """
    A window that a thread of its own owns while the with block runs. Only
    that thread can destroy it, and Windows destroys it when the thread ends.
    """
    hwnds: list[int] = []
    created = threading.Event()
    done = threading.Event()

    def run() -> None:
        try:
            hwnds.append(_new_window())
        finally:
            created.set()

        done.wait()
        _DestroyWindow(hwnds[0])

    thread = threading.Thread(target=run)
    thread.start()

    try:
        created.wait()

        if not hwnds:
            raise RuntimeError("the thread made no window")

        yield hwnds[0]
    finally:
        done.set()
        thread.join()


class TestWebView2Interop(unittest.TestCase):
    def setUp(self) -> None:
        # anything that is no composition controller
        self.uri = Uri("https://example.com/")

    def test_drop_target_is_not_created_directly(self) -> None:
        with self.assertRaisesRegex(TypeError, r"register_drop_target\(\)"):
            interop.DropTarget()

    def test_register_drop_target_not_a_controller(self) -> None:
        hwnd = _create_window(self)

        with self.assertRaises(OSError) as error:
            interop.register_drop_target(self.uri, hwnd)  # type: ignore[arg-type]

        self.assertEqual(error.exception.winerror, E_NOINTERFACE)

    def test_register_drop_target_window_of_another_thread(self) -> None:
        capsule = runtime.as_interface(self.uri, Uri)

        with (
            _window_of_another_thread() as hwnd,
            self.assertRaisesRegex(ValueError, "not a window of this thread"),
        ):
            native.register_drop_target(capsule, hwnd)

    def test_register_drop_target_no_window(self) -> None:
        capsule = runtime.as_interface(self.uri, Uri)

        with self.assertRaisesRegex(ValueError, "not a window of this thread"):
            native.register_drop_target(capsule, 0)

    def test_revoke_drop_target_not_registered(self) -> None:
        hwnd = _create_window(self)

        with self.assertRaises(OSError) as error:
            native.revoke_drop_target(hwnd)

        self.assertEqual(error.exception.winerror, DRAGDROP_E_NOTREGISTERED)

    def test_revoke_drop_target_window_of_another_thread(self) -> None:
        with (
            _window_of_another_thread() as hwnd,
            self.assertRaisesRegex(ValueError, "not a window of this thread"),
        ):
            native.revoke_drop_target(hwnd)
