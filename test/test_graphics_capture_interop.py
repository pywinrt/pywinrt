import os
import unittest

from winrt.windows.graphics.capture.interop import create_for_monitor

ON_CI = os.environ.get("CI")


class TestGraphicsCaptureInterop(unittest.TestCase):
    @unittest.skipIf(ON_CI, "CI does not have a monitor")
    def test_create_for_monitor(self):
        """test create_for_monitor() method"""
        item = create_for_monitor(0)
        self.assertIsInstance(item.display_name, str)
