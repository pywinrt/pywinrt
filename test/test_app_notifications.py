import os
import unittest

import winrt.microsoft.windows.applicationmodel.dynamicdependency.bootstrap as bootstrap
import winrt.microsoft.windows.appnotifications as an

ON_CI = os.environ.get("CI")


@unittest.skipIf(ON_CI, "Windows App Runtime not installed on CI")
class TestAppNotifications(unittest.TestCase):
    def test_default_manager(self) -> None:
        with bootstrap.initialize():
            self.assertTrue(an.AppNotificationManager.is_supported())
            manager = an.AppNotificationManager.default
            self.assertIsInstance(manager.setting, an.AppNotificationSetting)
            manager.register()
            manager.unregister()
