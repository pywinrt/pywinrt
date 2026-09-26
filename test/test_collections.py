import asyncio
import collections.abc
import unittest
from typing import cast

import test_winrt.testcomponent as tc
import winrt.windows.foundation.collections as wfc
from winrt.system import Object, box_string, unbox_string

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
        self.assertFalse(m, "Empty map should be falsy")

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
        self.assertTrue(m, "Non-empty map should be truthy")

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

        def on_map_changed(
            sender: wfc.IObservableMap[str, str], args: wfc.IMapChangedEventArgs[str]
        ):
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


class TestCollectionsPropertySet(unittest.TestCase):
    def test_value_set(self):
        m = wfc.PropertySet()
        m.insert("hello", box_string("world"))

        self.assertTrue(m.has_key("hello"))
        self.assertFalse(m.has_key("world"))
        self.assertEqual(m.size, 1)
        self.assertEqual(unbox_string(m.lookup("hello")), "world")

        m.remove("hello")

        self.assertFalse(m.has_key("hello"))

        with self.assertRaises(OSError):
            m.lookup("hello")

        with self.assertRaises(OSError):
            m.remove("hello")

    def test_test_property_set_is_mapping(self):
        m = wfc.PropertySet()
        self.assertFalse(m, "Empty set should be falsy")

        self.assertIsInstance(m, collections.abc.Mapping)

        m["hello"] = box_string("world")

        self.assertEqual(len(m), 1)
        self.assertEqual(unbox_string(m["hello"]), "world")
        self.assertIn("hello", m)
        self.assertEqual(unbox_string(cast(Object, m.get("hello"))), "world")
        self.assertIn("hello", m.keys())
        # can't test these because there is no equality for boxed values
        # self.assertIn("world", m.values())
        # self.assertIn(("hello", "world"), m.items())
        # self.assertEqual(m, {"hello": "world"})
        # self.assertFalse(m != {"hello": "world"})
        self.assertTrue(m, "Non-empty set should be truthy")

        m["hello"] = cast(Object, None)
        self.assertEqual(m["hello"], None)

        del m["hello"]

        with self.assertRaises(KeyError):
            m["hello"]

        with self.assertRaises(KeyError):
            del m["hello"]

        self.assertNotIn("hello", m)
        self.assertIsNone(m.get("hello"))

        m.update(hello=box_string("world"))
        self.assertEqual(unbox_string(m["hello"]), "world")
        self.assertTrue(m)

        m.clear()
        self.assertFalse(m)

        m.update({"hello": box_string("world")})
        self.assertEqual(unbox_string(m.pop("hello")), "world")

        with self.assertRaises(KeyError):
            m.pop("hello")

        self.assertEqual(
            unbox_string(m.setdefault("hello", box_string("world"))), "world"
        )
        self.assertEqual(
            unbox_string(m.setdefault("hello", box_string("other"))), "world"
        )

        self.assertIsNotNone(m.popitem())

        with self.assertRaises(KeyError):
            m.popitem()


class TestCollectionsVector(unittest.TestCase):
    def setUp(self):
        self.vector = tc.TestRunner.create_string_vector()
        self.vector.extend(["a", "b"])

    def test_negative_index_counts_from_the_end(self):
        self.assertEqual(self.vector[-1], "b")
        self.assertEqual(self.vector[-2], "a")

    def test_negative_index_out_of_range(self):
        with self.assertRaisesRegex(IndexError, "index out of range"):
            self.vector[-3]

    def test_index_out_of_range(self):
        with self.assertRaisesRegex(IndexError, "index out of range"):
            self.vector[2]

        with self.assertRaisesRegex(IndexError, "index out of range"):
            self.vector[2**32]

    def test_view_index(self):
        view, _ = tc.TestRunner.make_tests().collection6(["a", "b"])

        self.assertEqual(view[-1], "b")

        with self.assertRaisesRegex(IndexError, "index out of range"):
            view[-3]

        with self.assertRaisesRegex(IndexError, "index out of range"):
            view[2]

    def test_reversed(self):
        self.assertEqual(list(reversed(self.vector)), ["b", "a"])

    def test_negative_index_assign_and_del(self):
        self.vector[-1] = "z"
        self.assertEqual(list(self.vector), ["a", "z"])

        del self.vector[-2]
        self.assertEqual(list(self.vector), ["z"])

    def test_assign_and_del_out_of_range(self):
        for index in (2, -3, 2**32):
            with self.assertRaisesRegex(IndexError, "index out of range"):
                self.vector[index] = "z"

            with self.assertRaisesRegex(IndexError, "index out of range"):
                del self.vector[index]

        self.assertEqual(list(self.vector), ["a", "b"])
