import unittest

from winrt.microsoft.ui.xaml.controls import Button


class TestButton(unittest.TestCase):
    def test_button_attrs(self):
        # TODO: test inherited attributes
        self.assertTrue(hasattr(Button, "flyout"))
