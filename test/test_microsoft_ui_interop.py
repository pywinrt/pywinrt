import os
import unittest

import winui3.microsoft.ui as ui
import winui3.microsoft.ui.interop as interop

ON_CI = os.environ.get("CI")


@unittest.skipIf(ON_CI, "Windows App Runtime not installed on CI")
class TestMicrosoftUIInterop(unittest.TestCase):
    def test_window(self):
        hwnd = interop.get_window_from_window_id(ui.WindowId())
        self.assertEqual(hwnd, 0)
        window_id = interop.get_window_id_from_window(0)
        self.assertEqual(window_id.value, 0)

    def test_monitor(self):
        monitor = interop.get_monitor_from_display_id(ui.DisplayId())
        self.assertEqual(monitor, 0)
        display_id = interop.get_display_id_from_monitor(0)
        self.assertEqual(display_id.value, 0)
