import unittest

import winrt.windows.applicationmodel.core as core


class TestCoreApplication(unittest.TestCase):
    def test_static_event(self):
        # Ensure that a static event can be added to and removed
        token = core.CoreApplication.add_unhandled_error_detected(lambda s, e: None)
        self.assertIsNotNone(token)
        core.CoreApplication.remove_unhandled_error_detected(token)
