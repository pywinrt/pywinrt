import asyncio
import collections.abc
import unittest

import winrt.windows.foundation as wf
import winrt.windows.foundation.collections as wfc

from ._util import async_test


class TestCollectionsStringMap(unittest.TestCase):
    def test_string_map(self):
        m = wfc.StringMap()
        m.insert("hello", "world")

        self.assertTrue(m.has_key("hello"))
        self.assertFalse(m.has_key("world"))
        self.assertEqual(m.size, 1)
        self.assertEqual(m.lookup("hello"), "world")

        m.remove("hello")

        self.assertFalse(m.has_key("hello"))

        with self.assertRaises(OSError):
            m.lookup("hello")

        with self.assertRaises(OSError):
            m.remove("hello")

    def test_string_map_is_mapping(self):
        m = wfc.StringMap()

        self.assertIsInstance(m, collections.abc.Mapping)

        m["hello"] = "world"

        self.assertEqual(len(m), 1)
        self.assertEqual(m["hello"], "world")
        self.assertIn("hello", m)
        self.assertEqual(m.get("hello"), "world")
        self.assertIn("hello", m.keys())
        self.assertIn("world", m.values())
        self.assertIn(("hello", "world"), m.items())
        self.assertEqual(m, {"hello": "world"})
        self.assertFalse(m != {"hello": "world"})

        del m["hello"]

        with self.assertRaises(KeyError):
            m["hello"]

        with self.assertRaises(KeyError):
            del m["hello"]

        self.assertNotIn("hello", m)
        self.assertIsNone(m.get("hello"))

        m.update(hello="world")
        self.assertEqual(m["hello"], "world")
        self.assertTrue(m)

        m.clear()
        self.assertFalse(m)

        m.update({"hello": "world"})
        self.assertEqual(m.pop("hello"), "world")

        with self.assertRaises(KeyError):
            m.pop("hello")

        self.assertEqual(m.setdefault("hello", "world"), "world")
        self.assertEqual(m.setdefault("hello", "other"), "world")

        self.assertEqual(m.popitem(), ("hello", "world"))

        with self.assertRaises(KeyError):
            m.popitem()

    @async_test
    async def test_string_map_changed_event(self):
        loop = asyncio.get_running_loop()
        future = loop.create_future()

        def on_map_changed(sender: wfc.IMap, args: wfc.IMapChangedEventArgs):
            try:
                self.assertEqual(
                    args.collection_change, wfc.CollectionChange.ITEM_INSERTED
                )
                self.assertEqual(args.key, "dr")

                self.assertEqual(sender.size, 2)
                self.assertTrue(sender.has_key("dr"))
                self.assertTrue(sender.has_key("hello"))

                loop.call_soon_threadsafe(future.set_result, True)
            except Exception as ex:
                loop.call_soon_threadsafe(future.set_exception, ex)

        m = wfc.StringMap()
        m.insert("hello", "world")
        token = m.add_map_changed(on_map_changed)
        m.insert("dr", "who")
        m.remove_map_changed(token)

        called = await asyncio.wait_for(future, 1)
        self.assertTrue(called)

def box_value(value):
    if value is None:
        return None

    return wf.PropertyValue.create_string(value)

def unbox_value(value):
    if value is None:
        return None

    return wf.IPropertyValue._from(value).get_string()

class TestCollectionsPropertySet(unittest.TestCase):
    def test_value_set(self):
        m = wfc.PropertySet()
        m.insert("hello", box_value("world"))

        self.assertTrue(m.has_key("hello"))
        self.assertFalse(m.has_key("world"))
        self.assertEqual(m.size, 1)
        self.assertEqual(unbox_value(m.lookup("hello")), "world")

        m.remove("hello")

        self.assertFalse(m.has_key("hello"))

        with self.assertRaises(OSError):
            m.lookup("hello")

        with self.assertRaises(OSError):
            m.remove("hello")

    def test_test_property_set_is_mapping(self):
        m = wfc.PropertySet()

        self.assertIsInstance(m, collections.abc.Mapping)

        m["hello"] = box_value("world")

        self.assertEqual(len(m), 1)
        self.assertEqual(unbox_value(m["hello"]), "world")
        self.assertIn("hello", m)
        self.assertEqual(unbox_value(m.get("hello")) ,"world")
        self.assertIn("hello", m.keys())
        # can't test these because there is no equality for boxed values
        # self.assertIn("world", m.values())
        # self.assertIn(("hello", "world"), m.items())
        # self.assertEqual(m, {"hello": "world"})
        # self.assertFalse(m != {"hello": "world"})

        # TODO: this is a bug in the Mapping implementation
        # m["hello"] = box_value(None)
        # self.assertEqual(unbox_value(m["hello"]), None)

        del m["hello"]

        with self.assertRaises(KeyError):
            m["hello"]

        with self.assertRaises(KeyError):
            del m["hello"]

        self.assertNotIn("hello", m)
        self.assertIsNone(m.get("hello"))

        m.update(hello=box_value("world"))
        self.assertEqual(unbox_value(m["hello"]), "world")
        self.assertTrue(m)

        m.clear()
        self.assertFalse(m)

        m.update({"hello": box_value("world")})
        self.assertEqual(unbox_value(m.pop("hello")), "world")

        with self.assertRaises(KeyError):
            m.pop("hello")

        self.assertEqual(unbox_value(m.setdefault("hello", box_value("world"))), "world")
        self.assertEqual(unbox_value(m.setdefault("hello", box_value("other"))), "world")

        self.assertIsNotNone(m.popitem())

        with self.assertRaises(KeyError):
            m.popitem()
