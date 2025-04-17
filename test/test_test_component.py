import copy
import gc
import sys
import threading
import unittest
import weakref
from uuid import UUID

from typing_extensions import override

import test_winrt.testcomponent as tc
import winrt.windows.foundation.collections as wfc

from test._util import catch_unraisable


class TestTestComponent(unittest.TestCase):
    def test_struct_subclass(self):
        # subclassing a struct type is not allowed
        with self.assertRaisesRegex(TypeError, "not an acceptable base type"):

            class s(tc.Blittable):  # type: ignore
                pass

    def test_class_subclass(self):
        # subclassing a class type is not allowed
        with self.assertRaisesRegex(TypeError, "not an acceptable base type"):

            class s(tc.Class):  # type: ignore
                pass

    def test_composable_subclass(self):
        class C(tc.Composable):
            pass

        c = C()
        self.assertIsInstance(c, C)
        self.assertIsInstance(c, tc.Composable)
        self.assertEqual(c.value, 0)
        self.assertEqual(c.one(), 1)

    def test_composable_subclass_with_interface(self):
        class C(tc.Override, tc.IRequiredOne):
            @override
            def one(self) -> int:
                return 1

        c = C()

        self.assertEqual(c.one(), 1, "calling from python didn't work")
        self.assertEqual(
            tc.Composable.expect_required_one(c), 1, "calling from winrt didn't work"
        )
        self.assertIsInstance(c, tc.Override)
        self.assertIsInstance(c, tc.IRequiredOne)

    def test_overriding_new(self):
        class C(tc.Composable):
            @override
            def __new__(cls):
                return super().__new__(cls, 2)

        c = C()
        self.assertIsInstance(c, C)
        self.assertIsInstance(c, tc.Composable)
        self.assertEqual(c.value, 2)
        self.assertEqual(c.one(), 1)

    def test_overriding_method(self):
        event = threading.Event()
        base_event = threading.Event()

        class C(tc.Override):
            @override
            def _on_overridable(self) -> None:
                event.set()

        c = C()
        c.add_overridable_called(lambda s, e: base_event.set())

        c.call_overridable()
        self.assertTrue(event.is_set())
        self.assertFalse(base_event.is_set())

    def test_overriding_and_calling_super(self):
        event = threading.Event()
        base_event = threading.Event()

        class C(tc.Override):
            @override
            def _on_overridable(self) -> None:
                super()._on_overridable()
                event.set()

        c = C()
        c.add_overridable_called(lambda s, e: base_event.set())

        c.call_overridable()
        self.assertTrue(event.is_set())
        self.assertTrue(base_event.is_set())

    def test_unhandled_exception_in_override(self) -> None:
        class C(tc.Override):
            @override
            def _on_overridable(self) -> None:
                raise RuntimeError("test")

        c = C()

        with (
            self.assertRaisesRegex(OSError, "Unraisable Python exception"),
            catch_unraisable() as exceptions,
        ):
            c.call_overridable()

        self.assertIsInstance(exceptions[0].exc_value, RuntimeError)

    def test_object_round_trip(self):
        class C(tc.Composable):
            pass

        c = C()

        pset = wfc.PropertySet()
        pset.insert("c", c)

        c2 = pset.lookup("c")

        self.assertIs(
            c,
            c2,
            "user-created subclass instance should survive round-trip to WinRT-land",
        )

        c3 = c.as_(tc.Composable)

        self.assertIs(
            c,
            c3,
            "user-created subclass instance should survive round-trip from WinRT-land",
        )

    def test_object_lifetime_py(self):
        class C(tc.Composable):
            pass

        c = C()
        wr = weakref.ref(c)

        del c
        gc.collect()

        self.assertIsNone(
            wr(),
            "object should be collected when no Python or WinRT references exist",
        )

    def test_object_lifetime_winrt(self):
        class C(tc.Composable):
            pass

        c = C()
        wr = weakref.ref(c)

        pset = wfc.PropertySet()
        pset.insert("c", c)

        self.assertFalse(
            gc.is_tracked(c),
            "object should not be GC tracked while any WinRT references exist",
        )

        del c
        gc.collect()

        self.assertIsNotNone(
            wr(),
            "object should not be collected while any WinRT references exist",
        )

        pset.clear()
        gc.collect()

        self.assertIsNone(
            wr(), "object should be collected when WinRT references are gone"
        )

    def test_composable_isinstance(self):
        d = tc.Derived()

        self.assertIsInstance(d, tc.Composable)
        self.assertIsInstance(d, tc.IRequiredOne)

    def test_composable_issubclass(self):
        self.assertTrue(issubclass(tc.Derived, tc.Composable))  # type: ignore
        # FIXME: runtime subclass checking for interfaces is not implemented
        # self.assertTrue(issubclass(tc.Derived, tc.IRequiredOne))  # type: ignore

    def test_object_equality(self):
        c1 = tc.Derived()
        c2 = c1.as_(tc.Composable)
        c3 = tc.Derived()
        c4 = str()

        # these have to be true in order for tests to be valid
        self.assertIsNot(
            c1, c2, "python should create new wrappers for the same WinRT object"
        )
        self.assertNotEqual(type(c1), type(c2))

        self.assertEqual(
            c1,
            c2,
            "different wrappers of the same WinRT object instance should be equal",
        )
        self.assertFalse(
            c1 == c3, "different WinRT object instances should not be equal"
        )
        self.assertNotEqual(
            c1,
            c3,
            "different wrappers of the different WinRT object instances should not be equal",
        )
        self.assertFalse(c1 != c2, "WinRT object should be equal to itself")
        self.assertNotEqual(
            c1,
            c4,
            "WinRT object any any other type should not be equal",
        )

    def test_object_hashable(self):
        c1 = tc.Derived()
        c2 = c1.as_(tc.Composable)

        # these have to be true in order for test to be valid
        self.assertIsNot(
            c1, c2, "python should create new wrappers for the same WinRT object"
        )
        self.assertNotEqual(type(c1), type(c2))

        self.assertEqual(
            hash(c1),
            hash(c2),
            "different wrappers of the same WinRT object instance should hash the same",
        )

    def test_struct_new(self):
        b = tc.Blittable()
        self.assertEqual(b.a, 0)

        b = tc.Blittable(1)
        self.assertEqual(b.a, 1)

        b = tc.Blittable(b=1)
        self.assertEqual(b.b, 1)

        uuid = UUID("10000000-1000-1000-1000-100000000000")
        b = tc.Blittable(j=uuid)
        self.assertEqual(b.j, uuid)

        n = tc.NonBlittable(True)
        self.assertIs(n.a, True)

        s = "str"
        n = tc.NonBlittable(c=s)
        self.assertEqual(n.c, s)

    def test_struct_hashable(self):
        b = tc.Blittable()

        # structs are currently not hashable because they are mutable
        with self.assertRaisesRegex(TypeError, "unhashable type"):
            hash(b)

    def test_struct_equality(self):
        b1 = tc.Blittable()
        b2 = tc.Blittable()

        self.assertEqual(b1, b2)
        self.assertFalse(b1 != b2)

    def test_struct_inequality(self):
        b1 = tc.Blittable()
        b2 = tc.Blittable(1)

        self.assertNotEqual(b1, b2)
        self.assertFalse(b1 == b2)

    def test_struct_comparison(self):
        b1 = tc.Blittable()
        b2 = tc.Blittable()

        with self.assertRaisesRegex(TypeError, "'<' not supported"):
            b1 < b2  # type: ignore

        with self.assertRaisesRegex(TypeError, "'>' not supported"):
            b1 > b2  # type: ignore

        with self.assertRaisesRegex(TypeError, "'<=' not supported"):
            b1 <= b2  # type: ignore

        with self.assertRaisesRegex(TypeError, "'>=' not supported"):
            b1 >= b2  # type: ignore

    def test_struct_repr(self):
        nb = tc.NonBlittable(True, "b", "c", 4)
        r = repr(nb)

        self.assertEqual(r, "NonBlittable(a=True, b='b', c='c', d=4)")
        self.assertEqual(eval("tc." + r), nb)

    def test_struct_readonly(self):
        b = tc.Blittable()

        with self.assertRaisesRegex(AttributeError, "is not writable"):
            b.a = 1  # type: ignore

    def test_blittable_default(self):
        b = tc.Blittable()

        self.assertEqual(b.a, 0)
        self.assertEqual(b.b, 0)
        self.assertEqual(b.c, 0)
        self.assertEqual(b.d, 0)
        self.assertEqual(b.e, 0)
        self.assertEqual(b.f, 0)
        self.assertEqual(b.g, 0)
        self.assertEqual(b.h, 0)
        self.assertEqual(b.i, 0)
        self.assertEqual(b.j, UUID("00000000-0000-0000-0000-000000000000"))

    def test_blittable(self):
        b = tc.Blittable(
            1, 2, 3, 4, 5, 6, 7, 8, 9, UUID("10000000-1000-1000-1000-100000000000")
        )

        self.assertEqual(b.a, 1)
        self.assertEqual(b.b, 2)
        self.assertEqual(b.c, 3)
        self.assertEqual(b.d, 4)
        self.assertEqual(b.e, 5)
        self.assertEqual(b.f, 6)
        self.assertEqual(b.g, 7)
        self.assertEqual(b.h, 8)
        self.assertEqual(b.i, 9)
        self.assertEqual(b.j, UUID("10000000-1000-1000-1000-100000000000"))

    def test_non_blittable_default(self):
        nb = tc.NonBlittable()

        self.assertIs(nb.a, False)
        self.assertEqual(nb.b, "\x00")
        self.assertEqual(nb.c, "")
        self.assertIsNone(nb.d)

    def test_non_blittable(self):
        nb = tc.NonBlittable(True, "b", "c", 4)

        self.assertIs(nb.a, True)
        self.assertEqual(nb.b, "b")
        self.assertEqual(nb.c, "c")
        self.assertEqual(nb.d, 4)

    def test_nested_default(self):
        n = tc.Nested()

        self.assertEqual(n.blittable.a, 0)
        self.assertEqual(n.blittable.b, 0)
        self.assertEqual(n.blittable.c, 0)
        self.assertEqual(n.blittable.d, 0)
        self.assertEqual(n.blittable.e, 0)
        self.assertEqual(n.blittable.f, 0)
        self.assertEqual(n.blittable.g, 0)
        self.assertEqual(n.blittable.h, 0)
        self.assertEqual(n.blittable.i, 0)
        self.assertEqual(n.blittable.j, UUID("00000000-0000-0000-0000-000000000000"))

        self.assertIs(n.non_blittable.a, False)
        self.assertEqual(n.non_blittable.b, "\x00")
        self.assertEqual(n.non_blittable.c, "")
        self.assertIsNone(n.non_blittable.d)

    def test_nested(self):
        n = tc.Nested(
            tc.Blittable(
                1, 2, 3, 4, 5, 6, 7, 8, 9, UUID("10000000-1000-1000-1000-100000000000")
            ),
            tc.NonBlittable(True, "b", "c", 4),
        )

        self.assertEqual(n.blittable.a, 1)
        self.assertEqual(n.blittable.b, 2)
        self.assertEqual(n.blittable.c, 3)
        self.assertEqual(n.blittable.d, 4)
        self.assertEqual(n.blittable.e, 5)
        self.assertEqual(n.blittable.f, 6)
        self.assertEqual(n.blittable.g, 7)
        self.assertEqual(n.blittable.h, 8)
        self.assertEqual(n.blittable.i, 9)
        self.assertEqual(n.blittable.j, UUID("10000000-1000-1000-1000-100000000000"))

        self.assertIs(n.non_blittable.a, True)
        self.assertEqual(n.non_blittable.b, "b")
        self.assertEqual(n.non_blittable.c, "c")
        self.assertEqual(n.non_blittable.d, 4)

    def test_test_runner(self):
        tc.TestRunner.test_self()

    def test_dict_to_map(self):
        arg = {"1": "2", "3": "4"}
        tests = tc.TestRunner.make_tests()
        # collection3 is one of the few methods that takes an IMap input
        # parameter, so we use it to test that a dict can be consumed as an
        # IMap.
        result = tests.collection3(arg)
        # returns a copy of the dict both as a return value and as an out parameter
        self.assertDictEqual(dict(result[0]), arg)
        self.assertDictEqual(dict(result[1]), arg)

        # TODO: test wrong type in dict. currently this will cause an abort

    def test_dict_to_map_view(self):
        arg = {"1": "2", "3": "4"}
        tests = tc.TestRunner.make_tests()
        # collection4 is one of the few methods that takes an IMapView input
        # parameter, so we use it to test that a dict can be consumed as an
        # IMapView.
        result = tests.collection4(arg)
        # returns a copy of the dict both as a return value and as an out parameter
        self.assertDictEqual(dict(result[0]), arg)
        self.assertDictEqual(dict(result[1]), arg)

        # TODO: test wrong type in dict. currently this will cause an abort

    def test_list_to_vector(self):
        arg = ["1", "2", "3", "4"]
        tests = tc.TestRunner.make_tests()
        # collection5 is one of the few methods that takes an IVector input
        # parameter, so we use it to test that a list can be consumed as an
        # IVector.
        result = tests.collection5(arg)
        # returns a copy of the list both as a return value and as an out parameter
        self.assertListEqual(list(result[0]), arg)
        self.assertListEqual(list(result[1]), arg)

        # TODO: test wrong type in list. currently this will cause an abort

    def test_list_to_vector_view(self):
        arg = ["1", "2", "3", "4"]
        tests = tc.TestRunner.make_tests()
        # collection6 is one of the few methods that takes an IVectorView input
        # parameter, so we use it to test that a list can be consumed as an
        # IVectorView.
        result = tests.collection6(arg)
        # returns a copy of the list both as a return value and as an out parameter
        self.assertListEqual(list(result[0]), arg)
        self.assertListEqual(list(result[1]), arg)

        with (
            self.assertRaisesRegex(OSError, "Unraisable Python exception"),
            catch_unraisable() as exceptions,
        ):
            # requires list[str] so results in an unraisable TypeError
            tests.collection6([1])  # type: ignore

        self.assertEqual(len(exceptions), 1)
        self.assertEqual(exceptions[0].exc_type, TypeError)

    def test_tuple_to_struct(self):
        test = tc.TestRunner.make_tests()

        # can pass unspecialized tuple as projected struct
        _, _ = test.param13(
            (1, 2, 3, 4, 5, 6, 7, 8, 9, UUID(int=10)),
            tc.Blittable(1, 2, 3, 4, 5, 6, 7, 8, 9, UUID(int=10)),
        )

    def test_tuple_to_struct_wrong_arity(self):
        test = tc.TestRunner.make_tests()

        # passing 9-tuple as 10-field struct results in TypeError
        with self.assertRaises(TypeError):
            _, _ = test.param13(
                (1, 2, 3, 4, 5, 6, 7, 8, 9),  # type: ignore
                tc.Blittable(1, 2, 3, 4, 5, 6, 7, 8, 9, UUID(int=10)),
            )

    def test_tuple_to_struct_wrong_type(self):
        test = tc.TestRunner.make_tests()

        # passing wrong type for one of the tuple elements results in TypeError
        with self.assertRaises(TypeError):
            _, _ = test.param13(
                (1, 2, 3, 4, 5, 6, 7, 8, 9, 10),  # type: ignore
                tc.Blittable(1, 2, 3, 4, 5, 6, 7, 8, 9, UUID(int=10)),
            )

    def test_tuple_to_struct_nested(self):
        test = tc.TestRunner.make_tests()

        # nested structs can also be tuples
        _, _ = test.param15(
            ((1, 2, 3, 4, 5, 6, 7, 8, 9, UUID(int=10)), (True, "b", "c", 4)),
            tc.Nested(
                tc.Blittable(1, 2, 3, 4, 5, 6, 7, 8, 9, UUID(int=10)),
                tc.NonBlittable(True, "b", "c", 4),
            ),
        )

    @unittest.skipIf(sys.version_info < (3, 13), "requires Python 3.13 or later")
    def test_struct_replace_bool(self):
        orig = tc.NonBlittable(True, "b", "c", 4)
        cpy = copy.replace(orig, a=False)
        self.assertIs(cpy.a, False)

    @unittest.skipIf(sys.version_info < (3, 13), "requires Python 3.13 or later")
    def test_struct_replace_str(self):
        orig = tc.NonBlittable(True, "b", "c", 4)
        cpy = copy.replace(orig, b="B", c="C")
        self.assertEqual(cpy.b, "B")
        self.assertEqual(cpy.c, "C")

    @unittest.skipIf(sys.version_info < (3, 13), "requires Python 3.13 or later")
    def test_struct_replace_ireference(self):
        orig = tc.NonBlittable(True, "b", "c", 4)
        cpy = copy.replace(orig, d=None)
        self.assertIsNone(cpy.d)

    @unittest.skipIf(sys.version_info < (3, 13), "requires Python 3.13 or later")
    def test_struct_replace_int(self):
        orig = tc.Blittable(
            1, 2, 3, 4, 5, 6, 7, 8, 9, UUID("10000000-1000-1000-1000-100000000000")
        )
        cpy = copy.replace(orig, a=10, b=20, c=30, d=40, e=50, f=60, g=70)
        self.assertEqual(cpy.a, 10)
        self.assertEqual(cpy.b, 20)
        self.assertEqual(cpy.c, 30)
        self.assertEqual(cpy.d, 40)
        self.assertEqual(cpy.e, 50)
        self.assertEqual(cpy.f, 60)
        self.assertEqual(cpy.g, 70)
        self.assertAlmostEqual(cpy.h, 8)
        self.assertAlmostEqual(cpy.i, 9)
        self.assertEqual(cpy.j, UUID("10000000-1000-1000-1000-100000000000"))

    @unittest.skipIf(sys.version_info < (3, 13), "requires Python 3.13 or later")
    def test_struct_replace_float(self):
        orig = tc.Blittable(
            1, 2, 3, 4, 5, 6, 7, 8, 9, UUID("10000000-1000-1000-1000-100000000000")
        )
        cpy = copy.replace(orig, h=1.1, i=2.2)
        self.assertEqual(cpy.a, 1)
        self.assertAlmostEqual(cpy.h, 1.1)
        self.assertAlmostEqual(cpy.i, 2.2)

    @unittest.skipIf(sys.version_info < (3, 13), "requires Python 3.13 or later")
    def test_struct_replace_uuid(self):
        orig = tc.Blittable(
            1, 2, 3, 4, 5, 6, 7, 8, 9, UUID("10000000-1000-1000-1000-100000000000")
        )
        cpy = copy.replace(orig, j=UUID("20000000-2000-2000-2000-200000000000"))
        self.assertEqual(cpy.a, 1)
        self.assertEqual(cpy.j, UUID("20000000-2000-2000-2000-200000000000"))

    def test_struct_unpack(self):
        nb = tc.NonBlittable(True, "b", "c", 4)
        a, b, c, d = nb.unpack()
        self.assertIs(a, True)
        self.assertEqual(b, "b")
        self.assertEqual(c, "c")
        self.assertEqual(d, 4)

    def test_struct_unpack_nested(self):
        n = tc.Nested(
            tc.Blittable(1, 2, 3, 4, 5, 6, 7, 8, 9, UUID(int=10)),
            tc.NonBlittable(True, "b", "c", 4),
        )
        ((a, b, c, d, e, f, g, h, i, j), (w, x, y, z)) = n.unpack()
        self.assertEqual(a, 1)
        self.assertEqual(b, 2)
        self.assertEqual(c, 3)
        self.assertEqual(d, 4)
        self.assertEqual(e, 5)
        self.assertEqual(f, 6)
        self.assertEqual(g, 7)
        self.assertEqual(h, 8)
        self.assertEqual(i, 9)
        self.assertEqual(j, UUID(int=10))
        self.assertIs(w, True)
        self.assertEqual(x, "b")
        self.assertEqual(y, "c")
        self.assertEqual(z, 4)

    def test_dict_to_iter_of_ikeyvaluepair(self):
        arg = {"1": "2", "3": "4"}
        tests = tc.TestRunner.make_tests()
        ret, out = tests.collection2(arg)
        self.assertDictEqual({i.key: i.value for i in ret}, arg)
        self.assertDictEqual({i.key: i.value for i in out}, arg)
