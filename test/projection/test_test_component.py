import copy
import sys
import unittest
from uuid import UUID

import test_winrt.testcomponent as tc
from winrt.system.hresult import (
    E_BOUNDS,
    PYWINRT_E_UNRAISABLE_PYTHON_EXCEPTION,
)
import winrt.runtime as wr

from ._util import catch_unraisable


class TestTestComponent(unittest.TestCase):
    def test_composable_isinstance(self) -> None:
        d = tc.Derived()

        self.assertIsInstance(d, tc.Composable)
        self.assertIsInstance(d, tc.IRequiredOne)

    def test_composable_issubclass(self) -> None:
        self.assertTrue(issubclass(tc.Derived, tc.Composable))
        self.assertTrue(issubclass(tc.Derived, tc.IRequiredOne))

    def test_object_equality(self) -> None:
        c1 = tc.Derived()
        c2 = c1.as_(tc.Composable)
        c3 = tc.Derived()
        c4 = ""

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

    def test_object_hashable(self) -> None:
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

    def test_struct_new(self) -> None:
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

    def test_struct_hashable(self) -> None:
        b = tc.Blittable()

        self.assertEqual(hash(b), hash(tc.Blittable()))

    def test_struct_equality(self) -> None:
        b1 = tc.Blittable()
        b2 = tc.Blittable()

        self.assertEqual(b1, b2)
        self.assertFalse(b1 != b2)

    def test_struct_inequality(self) -> None:
        b1 = tc.Blittable()
        b2 = tc.Blittable(1)

        self.assertNotEqual(b1, b2)
        self.assertFalse(b1 == b2)

    def test_struct_comparison(self) -> None:
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

    def test_struct_repr(self) -> None:
        nb = tc.NonBlittable(True, "b", "c", 4)
        r = repr(nb)

        self.assertEqual(r, "NonBlittable(a=True, b='b', c='c', d=4)")
        self.assertEqual(eval("tc." + r), nb)

    def test_struct_readonly(self) -> None:
        b = tc.Blittable()

        with self.assertRaisesRegex(AttributeError, "is not writable"):
            b.a = 1  # type: ignore

    def test_blittable_default(self) -> None:
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

    def test_blittable(self) -> None:
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

    def test_non_blittable_default(self) -> None:
        nb = tc.NonBlittable()

        self.assertIs(nb.a, False)
        self.assertEqual(nb.b, "\x00")
        self.assertEqual(nb.c, "")
        self.assertIsNone(nb.d)

    def test_non_blittable(self) -> None:
        nb = tc.NonBlittable(True, "b", "c", 4)

        self.assertIs(nb.a, True)
        self.assertEqual(nb.b, "b")
        self.assertEqual(nb.c, "c")
        self.assertEqual(nb.d, 4)

    def test_nested_default(self) -> None:
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

    def test_nested(self) -> None:
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

    def test_test_runner(self) -> None:
        tc.TestRunner.test_self()

    def test_dict_to_map(self) -> None:
        arg = {"1": "2", "3": "4"}
        tests = tc.TestRunner.make_tests()
        # collection3 is one of the few methods that takes an IMap input
        # parameter, so we use it to test that a dict can be consumed as an
        # IMap.
        result = tests.collection3(arg)
        # returns a copy of the dict both as a return value and as an out parameter
        self.assertDictEqual(dict(result[0]), arg)
        self.assertDictEqual(dict(result[1]), arg)

    def test_dict_to_map_view(self) -> None:
        arg = {"1": "2", "3": "4"}
        tests = tc.TestRunner.make_tests()
        # collection4 is one of the few methods that takes an IMapView input
        # parameter, so we use it to test that a dict can be consumed as an
        # IMapView.
        result = tests.collection4(arg)
        # returns a copy of the dict both as a return value and as an out parameter
        self.assertDictEqual(dict(result[0]), arg)
        self.assertDictEqual(dict(result[1]), arg)

    def test_list_to_vector(self) -> None:
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

    def test_list_to_vector_out_of_range(self) -> None:
        class ShortSequence:
            """A sequence that claims to have more items than it does."""

            def __len__(self) -> int:
                return 3

            def __getitem__(self, index: int) -> str:
                return ["1", "2"][index]

        tests = tc.TestRunner.make_tests()

        # An index the Python object does not have is the one failure WinRT has
        # a name for, so it is reported as E_BOUNDS. Everything else a Python
        # object can raise while WinRT is reading it still goes to the
        # unraisable hook, as in the vector view test below.
        with self.assertRaises(OSError) as ctx, catch_unraisable() as exceptions:
            tests.collection5(ShortSequence())  # type: ignore

        self.assertEqual(ctx.exception.winerror, E_BOUNDS)
        self.assertEqual(exceptions, [])

    def test_list_to_vector_view(self) -> None:
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
            self.assertRaisesRegex(OSError, "Unraisable Python exception") as ctx,
            catch_unraisable() as exceptions,
        ):
            # requires list[str] so results in an unraisable TypeError
            tests.collection6([1])  # type: ignore

        self.assertEqual(ctx.exception.winerror, PYWINRT_E_UNRAISABLE_PYTHON_EXCEPTION)
        self.assertEqual(len(exceptions), 1)
        self.assertEqual(exceptions[0].exc_type, TypeError)

    def test_tuple_to_struct(self) -> None:
        test = tc.TestRunner.make_tests()

        # can pass unspecialized tuple as projected struct; the call refuses
        # two arguments that differ, and hands the struct back twice
        expected = tc.Blittable(1, 2, 3, 4, 5, 6, 7, 8, 9, UUID(int=10))
        result = test.param13((1, 2, 3, 4, 5, 6, 7, 8, 9, UUID(int=10)), expected)

        self.assertEqual(result, (expected, expected))

    def test_tuple_to_struct_wrong_arity(self) -> None:
        test = tc.TestRunner.make_tests()

        # passing 9-tuple as 10-field struct results in TypeError
        with self.assertRaises(TypeError):
            _, _ = test.param13(
                (1, 2, 3, 4, 5, 6, 7, 8, 9),  # type: ignore
                tc.Blittable(1, 2, 3, 4, 5, 6, 7, 8, 9, UUID(int=10)),
            )

    def test_tuple_to_struct_wrong_type(self) -> None:
        test = tc.TestRunner.make_tests()

        # passing wrong type for one of the tuple elements results in TypeError
        with self.assertRaises(TypeError):
            _, _ = test.param13(
                (1, 2, 3, 4, 5, 6, 7, 8, 9, 10),  # type: ignore
                tc.Blittable(1, 2, 3, 4, 5, 6, 7, 8, 9, UUID(int=10)),
            )

    def test_tuple_to_struct_nested(self) -> None:
        test = tc.TestRunner.make_tests()

        # nested structs can also be tuples
        expected = tc.Nested(
            tc.Blittable(1, 2, 3, 4, 5, 6, 7, 8, 9, UUID(int=10)),
            tc.NonBlittable(True, "b", "c", 4),
        )
        result = test.param15(
            ((1, 2, 3, 4, 5, 6, 7, 8, 9, UUID(int=10)), (True, "b", "c", 4)), expected
        )

        self.assertEqual(result, (expected, expected))

    @unittest.skipIf(sys.version_info < (3, 13), "requires Python 3.13 or later")
    def test_struct_replace_bool(self) -> None:
        orig = tc.NonBlittable(True, "b", "c", 4)
        cpy = copy.replace(orig, a=False)
        self.assertIs(cpy.a, False)

    @unittest.skipIf(sys.version_info < (3, 13), "requires Python 3.13 or later")
    def test_struct_replace_str(self) -> None:
        orig = tc.NonBlittable(True, "b", "c", 4)
        cpy = copy.replace(orig, b="B", c="C")
        self.assertEqual(cpy.b, "B")
        self.assertEqual(cpy.c, "C")

    @unittest.skipIf(sys.version_info < (3, 13), "requires Python 3.13 or later")
    def test_struct_replace_ireference(self) -> None:
        orig = tc.NonBlittable(True, "b", "c", 4)
        cpy = copy.replace(orig, d=None)
        self.assertIsNone(cpy.d)

    @unittest.skipIf(sys.version_info < (3, 13), "requires Python 3.13 or later")
    def test_struct_replace_int(self) -> None:
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
    def test_struct_replace_float(self) -> None:
        orig = tc.Blittable(
            1, 2, 3, 4, 5, 6, 7, 8, 9, UUID("10000000-1000-1000-1000-100000000000")
        )
        cpy = copy.replace(orig, h=1.1, i=2.2)
        self.assertEqual(cpy.a, 1)
        self.assertAlmostEqual(cpy.h, 1.1)
        self.assertAlmostEqual(cpy.i, 2.2)

    @unittest.skipIf(sys.version_info < (3, 13), "requires Python 3.13 or later")
    def test_struct_replace_uuid(self) -> None:
        orig = tc.Blittable(
            1, 2, 3, 4, 5, 6, 7, 8, 9, UUID("10000000-1000-1000-1000-100000000000")
        )
        cpy = copy.replace(orig, j=UUID("20000000-2000-2000-2000-200000000000"))
        self.assertEqual(cpy.a, 1)
        self.assertEqual(cpy.j, UUID("20000000-2000-2000-2000-200000000000"))

    def test_struct_unpack(self) -> None:
        nb = tc.NonBlittable(True, "b", "c", 4)
        a, b, c, d = nb.unpack()
        self.assertIs(a, True)
        self.assertEqual(b, "b")
        self.assertEqual(c, "c")
        self.assertEqual(d, 4)

    def test_struct_unpack_nested(self) -> None:
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

    def test_dict_to_iter_of_ikeyvaluepair(self) -> None:
        arg = {"1": "2", "3": "4"}
        tests = tc.TestRunner.make_tests()
        ret, out = tests.collection2(arg)
        self.assertDictEqual({i.key: i.value for i in ret}, arg)
        self.assertDictEqual({i.key: i.value for i in out}, arg)

    def test_init_apartment_refuses_an_unknown_type(self) -> None:
        with self.assertRaisesRegex(ValueError, "^7 is not a valid ApartmentType$"):
            wr.init_apartment(7)  # type: ignore[arg-type]

    def test_uninit_apartment_releases_factories(self) -> None:
        # The last CoUninitialize() of the process unloads the DLL that
        # implements a class, so a factory kept past it crashes the next
        # constructor or static. uninit_apartment() releases the factories
        # the runtime keeps before it uninitializes. FactoryCounter counts
        # the live factory objects of CountedFactory, of which TestComponent
        # makes a new one per request and keeps none, so what it counts is
        # what the runtime keeps.
        wr.init_apartment(wr.ApartmentType.SINGLE_THREADED)
        try:
            # one factory for the static and one for the constructor, each
            # kept for the next call
            tc.CountedFactory.static_method()
            tc.CountedFactory()
            tc.CountedFactory.static_method()
            tc.CountedFactory()
            self.assertEqual(tc.FactoryCounter.counted_factory_count, 2)
        finally:
            wr.uninit_apartment()

        self.assertEqual(tc.FactoryCounter.counted_factory_count, 0)

        # and the next call asks for its factory again and keeps it
        wr.init_apartment(wr.ApartmentType.SINGLE_THREADED)
        try:
            tc.CountedFactory.static_method()
            tc.CountedFactory.static_method()
            self.assertEqual(tc.FactoryCounter.counted_factory_count, 1)
        finally:
            wr.uninit_apartment()

        self.assertEqual(tc.FactoryCounter.counted_factory_count, 0)
