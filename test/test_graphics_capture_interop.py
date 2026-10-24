import os
import subprocess
import sys
import unittest

from winrt.windows.graphics.capture.interop import create_for_monitor

ON_CI = os.environ.get("CI")

# A null window is refused by the interop call itself, which is reached only
# once the activation factory has been got.
FIRST_CALL_CODE = """
import winrt.windows.graphics.capture.interop as interop

CO_E_NOTINITIALIZED = -2147221008

try:
    interop.create_for_window(0)
except OSError as e:
    if e.winerror == CO_E_NOTINITIALIZED:
        raise
"""


class TestGraphicsCaptureInterop(unittest.TestCase):
    @unittest.skipIf(ON_CI, "CI does not have a monitor")
    def test_create_for_monitor(self) -> None:
        """test create_for_monitor() method"""
        item = create_for_monitor(0)
        self.assertIsInstance(item.display_name, str)

    def test_first_call_in_a_process(self) -> None:
        # A process of its own, in which nothing has initialized COM.
        result = subprocess.run(
            [sys.executable, "-c", FIRST_CALL_CODE], capture_output=True, text=True
        )

        self.assertEqual(result.returncode, 0, result.stderr)
