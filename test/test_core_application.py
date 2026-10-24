import unittest

import winrt.windows.applicationmodel.core as core
import winrt.windows.foundation as wf


class TestCoreApplication(unittest.TestCase):
    def test_static_event(self) -> None:
        # Ensure that a static event can be added to and removed
        token = core.CoreApplication.add_unhandled_error_detected(lambda s, e: None)
        self.assertIsInstance(token, wf.EventRegistrationToken)
        self.assertNotEqual(token, 0)
        core.CoreApplication.remove_unhandled_error_detected(token)
