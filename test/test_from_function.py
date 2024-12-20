import unittest

import winrt.windows.foundation.collections as wfc


class TestQueryInterface(unittest.TestCase):
    def test_as_function(self):
        propset = wfc.PropertySet()
        propset.insert("strmap", wfc.StringMap())
        self.assertTrue(propset.has_key("strmap"))
        o = propset.lookup("strmap")
        strmap = o.as_(wfc.StringMap)
        self.assertEqual(type(strmap), wfc.StringMap)
