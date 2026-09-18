from test._util import skip_without_projection

skip_without_projection()

import unittest
import uuid

from winrt.windows.foundation import GuidHelper


class TestGuidHelper(unittest.TestCase):
    def test_empty(self):
        # this tests that static properties in general work
        self.assertEqual(GuidHelper.empty, uuid.UUID(bytes=bytes(16)))

    def test_equals(self):
        # Equals takes both of its GUIDs by reference, which is the only
        # method in the Windows SDK that does. Passing one by value instead
        # costs nothing on x64 and corrupts the stack on x86.
        value = uuid.UUID("2c3f2e0b-2d4d-4a1e-8b6f-6f4d2a7c9e31")

        self.assertTrue(GuidHelper.equals(value, value))
        self.assertFalse(GuidHelper.equals(value, GuidHelper.empty))
