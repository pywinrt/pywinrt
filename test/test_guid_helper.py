import unittest
import uuid

from winrt.windows.foundation import GuidHelper


class TestGuidHelper(unittest.TestCase):
    def test_empty(self):
        # this tests that static properties in general work
        self.assertEqual(GuidHelper.empty, uuid.UUID(bytes=bytes(16)))
