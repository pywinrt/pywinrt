import subprocess
import sys
import unittest

from winrt.windows.applicationmodel.datatransfer.interop import (
    get_for_window,
    show_share_ui_for_window,
)

# A null window is refused by the interop call itself, which is reached only
# once the activation factory has been got.
FIRST_CALL_CODE = """
import winrt.windows.applicationmodel.datatransfer.interop as interop

CO_E_NOTINITIALIZED = -2147221008

try:
    interop.get_for_window(0)
except OSError as e:
    if e.winerror == CO_E_NOTINITIALIZED:
        raise
"""


# HRESULT_FROM_WIN32(ERROR_INVALID_WINDOW_HANDLE), which the interop calls
# themselves return for a null window: getting the activation factory, or
# querying it for the interop interface, fails with other errors.
E_INVALID_WINDOW_HANDLE = -2147023496


class TestDataTransferInterop(unittest.TestCase):
    def test_get_for_window_refuses_no_window(self) -> None:
        with self.assertRaises(OSError) as caught:
            get_for_window(0)

        self.assertEqual(caught.exception.winerror, E_INVALID_WINDOW_HANDLE)

    def test_show_share_ui_for_window_refuses_no_window(self) -> None:
        with self.assertRaises(OSError) as caught:
            show_share_ui_for_window(0)

        self.assertEqual(caught.exception.winerror, E_INVALID_WINDOW_HANDLE)

    def test_first_call_in_a_process(self) -> None:
        # A process of its own, in which nothing has initialized COM.
        result = subprocess.run(
            [sys.executable, "-c", FIRST_CALL_CODE], capture_output=True, text=True
        )

        self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == "__main__":
    unittest.main()
