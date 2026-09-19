import os
import unittest

import winui3.microsoft.windows.applicationmodel.dynamicdependency.bootstrap as bootstrap
import winui3.microsoft.windows.appnotifications as an

ON_CI = os.environ.get("CI")


@unittest.skipIf(ON_CI, "Windows App Runtime not installed on CI")
class TestAppNotifications(unittest.TestCase):
    def test_default_manager(self):
        with bootstrap.initialize():
            manager = an.AppNotificationManager.default
            manager.register()
            manager.unregister()
