import unittest

import winrt.microsoft.windows.applicationmodel.dynamicdependency.bootstrap as bootstrap


class TestBootstrap(unittest.TestCase):
    def test_initialize_no_match(self):
        with self.assertRaises(OSError):
            bootstrap.initialize("99.99")
