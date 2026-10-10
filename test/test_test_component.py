import asyncio
import contextlib
import copy
import ctypes
import gc
import sys
import threading
import traceback
import unittest
import weakref
from typing import Any, Generic, TypedDict, TypeVar
from uuid import UUID

import test_winrt.testcomponent as tc
from winrt.system.hresult import (
    E_BOUNDS,
    E_FAIL,
    E_ILLEGAL_DELEGATE_ASSIGNMENT,
    WIN32_ERROR_CANCELLED,
    PYWINRT_E_UNRAISABLE_PYTHON_EXCEPTION,
)
import winrt.runtime as wr
import winrt.windows.foundation as wf
import winrt.windows.foundation.collections as wfc
from typing_extensions import override

from ._util import async_test, catch_unraisable


class TestTestComponent(unittest.TestCase):
    def test_struct_subclass(self) -> None:
        # subclassing a struct type is not allowed
        with self.assertRaisesRegex(TypeError, "not an acceptable base type"):

            class s(tc.Blittable):  # type: ignore
                pass

    def test_class_subclass(self) -> None:
        # subclassing a class type is not allowed
        with self.assertRaisesRegex(TypeError, "not an acceptable base type"):

            class s(tc.Class):  # type: ignore
                pass

    def test_composable_subclass(self) -> None:
        class C(tc.Composable):
            pass

        c = C()
        self.assertIsInstance(c, C)
        self.assertIsInstance(c, tc.Composable)
        self.assertEqual(c.value, 0)
        self.assertEqual(c.one(), 1)

    def test_composable_subclass_with_interface(self) -> None:
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

    def test_composable_subclass_of_a_composed_class(self) -> None:
        # Derived derives from Composable and is composable in turn, so a
        # Python class composed into it has three objects behind it and the
        # members of both classes.
        class C(tc.Derived):
            pass

        c = C()

        self.assertIsInstance(c, tc.Derived)
        self.assertIsInstance(c, tc.Composable)
        self.assertEqual(c.value, 0)
        self.assertEqual(c.one(), 1)

    def test_composable_runtime_class_name(self) -> None:
        # C++/WinRT names a composed object after the first interface the
        # derived object implements itself, so a subclass of a class that
        # leaves nothing to be overridden has no name of its own.
        class C(tc.Composable):
            pass

        class D(tc.Override):
            pass

        class E(tc.DerivedOverride):
            pass

        self.assertEqual(tc.TestRunner.expect_object(C()), "")
        self.assertEqual(
            tc.TestRunner.expect_object(D()), "TestComponent.IOverrideOverrides"
        )
        self.assertEqual(
            tc.TestRunner.expect_object(E()),
            "TestComponent.IDerivedOverrideOverrides",
        )

    def test_declared_runtime_class_name(self) -> None:
        # A class may say what it is called instead, which is what a XAML
        # metadata provider needs, and a class derived from it inherits that.
        class C(tc.Composable, runtime_class_name="Spam.Eggs"):
            pass

        class D(C):
            pass

        class E(C, runtime_class_name="Spam.Ham"):
            pass

        self.assertEqual(tc.TestRunner.expect_object(C()), "Spam.Eggs")
        self.assertEqual(C()._runtime_class_name_, "Spam.Eggs")
        self.assertEqual(tc.TestRunner.expect_object(D()), "Spam.Eggs")
        self.assertEqual(tc.TestRunner.expect_object(E()), "Spam.Ham")

    def test_declared_runtime_class_name_misspelled(self) -> None:
        with self.assertRaises(TypeError):

            class _(tc.Composable, runtime_class_nam="Spam.Eggs"):  # pyright: ignore[reportGeneralTypeIssues]
                pass

    def test_declared_runtime_class_name_must_be_str(self) -> None:
        with self.assertRaisesRegex(TypeError, "must be str, not int"):

            class _(tc.Composable, runtime_class_name=1):  # pyright: ignore[reportArgumentType]
                pass

    def test_declared_runtime_class_name_is_not_none(self) -> None:
        # A class inherits the name of the class it derives from, which None
        # would seem to take away and could not, so it is refused.
        class C(tc.Composable, runtime_class_name="Spam.Eggs"):
            pass

        with self.assertRaisesRegex(TypeError, "must be str, not NoneType"):

            class _(C, runtime_class_name=None):  # pyright: ignore[reportArgumentType]
                pass

    def test_declared_runtime_class_name_set_afterwards_must_be_str(self) -> None:
        class C(tc.Composable, runtime_class_name="Spam.Eggs"):
            pass

        setattr(C, "_declared_runtime_class_name_", 1)

        with self.assertRaisesRegex(
            TypeError, "_declared_runtime_class_name_ must be str, not int"
        ):
            C()

    def test_implementation_has_no_declared_runtime_class_name(self) -> None:
        # A Python implementation of an interface is named after the first
        # interface it implements, as C++/WinRT names one.
        with self.assertRaises(TypeError):

            class _(wf.IStringable, runtime_class_name="Spam.Eggs"):
                @override
                def to_string(self) -> str:
                    return "spam"

    def test_overriding_new(self) -> None:
        class C(tc.Composable):
            @override
            def __new__(cls):
                return super().__new__(cls, 2)

        c = C()
        self.assertIsInstance(c, C)
        self.assertIsInstance(c, tc.Composable)
        self.assertEqual(c.value, 2)
        self.assertEqual(c.one(), 1)

    def test_overriding_method(self) -> None:
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

    def test_overriding_and_calling_super(self) -> None:
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

    def test_overriding_methods_of_every_base(self) -> None:
        # Each class in the chain declares its own overridable interface, and
        # the composed object answers all of them, not only the one the class
        # derived from declares.
        called: list[str] = []

        class C(tc.DerivedOverride):
            @override
            def _on_overridable(self) -> None:
                called.append("base")

            @override
            def _on_derived_overridable(self) -> None:
                called.append("derived")

        c = C()
        c.call_overridable()
        c.call_derived_overridable()

        self.assertEqual(called, ["base", "derived"])

    def test_a_protected_constructor_does_not_create_its_class(self) -> None:
        for args in ((), (5,)):
            with self.subTest(args=args):
                with self.assertRaisesRegex(
                    TypeError,
                    "'TestComponent.ProtectedMembers' cannot be created directly; "
                    "its constructor is protected",
                ):
                    tc.ProtectedMembers(*args)

    def test_a_protected_constructor_composes_a_derived_class(self) -> None:
        class C(tc.ProtectedMembers):
            pass

        class D(tc.ProtectedMembers):
            @override
            def __new__(cls):
                return super().__new__(cls, 6)

        self.assertEqual(C()._protected_value, 0)
        self.assertEqual(C(5)._protected_value, 5)
        self.assertEqual(D()._protected_value, 6)
        self.assertIsInstance(C(), tc.ProtectedMembers)

    def test_a_protected_property_and_method(self) -> None:
        class C(tc.ProtectedMembers):
            pass

        c = C()
        c._protected_value = 7

        self.assertEqual(c._protected_value, 7)
        self.assertEqual(c._protected_method(), 7)

    def test_overriding_a_property(self) -> None:
        # WinRT reads the property through the overridable interface, so what
        # it gets is the derived class's own, and the class's own otherwise.
        class C(tc.ProtectedMembers):
            @property
            @override
            def _overridable_value(self) -> int:
                return 42

        class D(tc.ProtectedMembers):
            pass

        # super() reaches the class's own, rather than coming back here
        class E(tc.ProtectedMembers):
            @property
            @override
            def _overridable_value(self) -> int:
                return super()._overridable_value + 100

        self.assertEqual(C(3).read_overridable_value(), 42)
        self.assertEqual(D(3).read_overridable_value(), 3)
        self.assertEqual(E(3).read_overridable_value(), 103)
        self.assertEqual(E(3)._overridable_value, 103)

    def test_unhandled_exception_in_override(self) -> None:
        class C(tc.Override):
            @override
            def _on_overridable(self) -> None:
                raise RuntimeError("test")

        c = C()

        with (
            self.assertRaisesRegex(OSError, "Unraisable Python exception") as ctx,
            catch_unraisable() as exceptions,
        ):
            c.call_overridable()

        self.assertEqual(ctx.exception.winerror, PYWINRT_E_UNRAISABLE_PYTHON_EXCEPTION)
        self.assertIsInstance(exceptions[0].exc_value, RuntimeError)
        # the override, which the default hook names in "Exception ignored in"
        self.assertEqual(exceptions[0].object, c._on_overridable)

    def test_object_round_trip(self) -> None:
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

    def test_object_lifetime_py(self) -> None:
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

    def test_object_lifetime_winrt(self) -> None:
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

        # TODO: test wrong type in dict. currently this will cause an abort

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

        # TODO: test wrong type in dict. currently this will cause an abort

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

        # can pass unspecialized tuple as projected struct
        _, _ = test.param13(
            (1, 2, 3, 4, 5, 6, 7, 8, 9, UUID(int=10)),
            tc.Blittable(1, 2, 3, 4, 5, 6, 7, 8, 9, UUID(int=10)),
        )

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
        _, _ = test.param15(
            ((1, 2, 3, 4, 5, 6, 7, 8, 9, UUID(int=10)), (True, "b", "c", 4)),
            tc.Nested(
                tc.Blittable(1, 2, 3, 4, 5, 6, 7, 8, 9, UUID(int=10)),
                tc.NonBlittable(True, "b", "c", 4),
            ),
        )

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

    def test_async_action_get_sta(self) -> None:
        wr.init_apartment(wr.ApartmentType.SINGLE_THREADED)
        try:
            with self.assertRaises(RuntimeError):
                tc.TestRunner.create_async_action(10).get()
        finally:
            wr.uninit_apartment()

    def test_async_action_get(self) -> None:
        tc.TestRunner.create_async_action(10).get()

    def test_async_action_get_cancel(self) -> None:
        op = tc.TestRunner.create_async_action(10)
        op.cancel()
        with self.assertRaises(OSError) as ctx:
            op.get()

        self.assertEqual(ctx.exception.winerror, WIN32_ERROR_CANCELLED)

    def test_async_action_get_error(self) -> None:
        with self.assertRaises(OSError) as ctx:
            tc.TestRunner.create_async_action_with_error(10, E_FAIL).get()

        self.assertEqual(ctx.exception.winerror, E_FAIL)

    def test_async_action_wait_sta(self) -> None:
        wr.init_apartment(wr.ApartmentType.SINGLE_THREADED)
        try:
            with self.assertRaises(RuntimeError):
                tc.TestRunner.create_async_action(10).wait(1)
        finally:
            wr.uninit_apartment()

    def test_async_action_wait(self) -> None:
        status = tc.TestRunner.create_async_action(10).wait(1)
        self.assertEqual(status, wf.AsyncStatus.COMPLETED)

    def test_async_action_wait_timeout(self) -> None:
        status = tc.TestRunner.create_async_action(1000).wait(0.1)
        self.assertEqual(status, wf.AsyncStatus.STARTED)

    def test_async_action_wait_not_a_duration(self) -> None:
        # A timeout that is not a positive number of seconds asks for the
        # status as it stands rather than for a wait. Each one gets its own
        # operation because a WinRT async object accepts only one completed
        # handler, so a second wait on a running one is an error.
        for timeout in (0, -1, float("nan")):
            with self.subTest(timeout=timeout):
                op = tc.TestRunner.create_async_action(1000)
                self.assertEqual(op.wait(timeout), wf.AsyncStatus.STARTED)

    def test_async_action_wait_cancel(self) -> None:
        op = tc.TestRunner.create_async_action(10)
        op.cancel()
        status = op.wait(1)
        self.assertEqual(status, wf.AsyncStatus.CANCELED)

    def test_async_action_wait_error(self) -> None:
        status = tc.TestRunner.create_async_action_with_error(10, E_FAIL).wait(1)
        self.assertEqual(status, wf.AsyncStatus.ERROR)

    def test_async_operation_get(self) -> None:
        expected = 1
        actual = tc.TestRunner.create_async_operation(10, expected).get()
        self.assertEqual(expected, actual)

    def test_async_operation_get_sta(self) -> None:
        wr.init_apartment(wr.ApartmentType.SINGLE_THREADED)
        try:
            with self.assertRaises(RuntimeError):
                tc.TestRunner.create_async_operation(10, 1).get()
        finally:
            wr.uninit_apartment()

    def test_async_operation_get_cancel(self) -> None:
        op = tc.TestRunner.create_async_operation(10, 1)
        op.cancel()
        with self.assertRaises(OSError) as ctx:
            op.get()

        self.assertEqual(ctx.exception.winerror, WIN32_ERROR_CANCELLED)

    def test_async_operation_get_error(self) -> None:
        with self.assertRaises(OSError) as ctx:
            tc.TestRunner.create_async_operation_with_error(10, 1, E_FAIL).get()

        self.assertEqual(ctx.exception.winerror, E_FAIL)

    def test_async_operation_wait_sta(self) -> None:
        wr.init_apartment(wr.ApartmentType.SINGLE_THREADED)
        try:
            with self.assertRaises(RuntimeError):
                tc.TestRunner.create_async_operation(10, 1).wait(1)
        finally:
            wr.uninit_apartment()

    def test_async_operation_wait(self) -> None:
        status = tc.TestRunner.create_async_operation(10, 1).wait(1)
        self.assertEqual(status, wf.AsyncStatus.COMPLETED)

    def test_async_operation_wait_timeout(self) -> None:
        status = tc.TestRunner.create_async_operation(1000, 1).wait(0.1)
        self.assertEqual(status, wf.AsyncStatus.STARTED)

    def test_async_operation_wait_cancel(self) -> None:
        op = tc.TestRunner.create_async_operation(10, 1)
        op.cancel()
        status = op.wait(1)
        self.assertEqual(status, wf.AsyncStatus.CANCELED)

    def test_async_operation_wait_error(self) -> None:
        status = tc.TestRunner.create_async_operation_with_error(10, 1, E_FAIL).wait(1)
        self.assertEqual(status, wf.AsyncStatus.ERROR)

    def test_async_action_with_progress_get(self) -> None:
        tc.TestRunner.create_async_action_with_progress(10, [1, 2]).get()

    def test_async_action_progress_handler_is_replaced(self) -> None:
        first: list[int] = []
        second: list[int] = []

        op = tc.TestRunner.create_async_action_with_progress(10, [1, 2])
        # https://github.com/microsoft/pyright/issues/11833
        op.progress = lambda sender, value: first.append(value)  # pyright: ignore[reportAttributeAccessIssue, reportUnknownLambdaType, reportUnknownArgumentType]
        op.progress = lambda sender, value: second.append(value)  # pyright: ignore[reportAttributeAccessIssue, reportUnknownLambdaType, reportUnknownArgumentType]
        op.get()

        self.assertEqual(first, [])
        self.assertEqual(second, [1, 2])

    def test_async_action_with_progress_wait(self) -> None:
        status = tc.TestRunner.create_async_action_with_progress(10, [1, 2]).wait(1)
        self.assertEqual(status, wf.AsyncStatus.COMPLETED)

    def test_async_operation_with_progress_get(self) -> None:
        expected = 3
        actual = tc.TestRunner.create_async_operation_with_progress(
            10, [1, 2], expected
        ).get()
        self.assertEqual(expected, actual)

    def test_async_operation_with_progress_wait(self) -> None:
        status = tc.TestRunner.create_async_operation_with_progress(10, [1, 2], 3).wait(
            1
        )
        self.assertEqual(status, wf.AsyncStatus.COMPLETED)

    @async_test
    async def test_async_action(self) -> None:
        op = tc.TestRunner.create_async_action(10)
        await op

        self.assertEqual(op.status, wf.AsyncStatus.COMPLETED)

    @async_test
    async def test_async_action_cancel(self) -> None:
        op = tc.TestRunner.create_async_action(500)

        with self.assertRaises(asyncio.TimeoutError):
            async with asyncio.timeout(0.1):
                await op

        # The Python asyncio.Future propagates the cancellation to the WinRT action.
        self.assertEqual(op.status, wf.AsyncStatus.CANCELED)

    @async_test
    async def test_async_action_cancel_with_shield(self) -> None:
        errors = self._record_loop_errors()
        source = tc.AsyncActionSource()
        op = source.operation

        try:
            with self.assertRaises(asyncio.TimeoutError):
                async with asyncio.timeout(0):
                    await asyncio.shield(op)

            await self._run_callbacks()

            # The shield kept the cancellation from reaching the operation,
            # which is still running.
            self.assertEqual(source.cancel_request_count, 0)
            self.assertEqual(op.status, wf.AsyncStatus.STARTED)
        finally:
            # Only the source finishes the operation, which the shielded task
            # is still waiting for.
            source.complete()

        await self._run_callbacks()

        self.assertEqual(op.status, wf.AsyncStatus.COMPLETED)
        self.assertEqual(errors, [])

    @async_test
    async def test_async_action_cancel_with_cancel(self) -> None:
        errors = self._record_loop_errors()
        source = tc.AsyncActionSource()
        op = source.operation

        async def wait_with_timeout() -> None:
            async with asyncio.timeout(0):
                try:
                    await op
                except asyncio.CancelledError:
                    # what an app that cancels the operation itself does
                    op.cancel()
                    raise

        task = asyncio.create_task(wait_with_timeout())

        try:
            await self._run_callbacks()

            # The timeout asked the operation to cancel, and the await goes on
            # until it has.
            self.assertEqual(source.cancel_request_count, 1)
            self.assertFalse(task.done())
        finally:
            source.cancel()

        with self.assertRaises(TimeoutError):
            await task

        # Cancelling an operation that has finished changes nothing.
        self.assertEqual(op.status, wf.AsyncStatus.CANCELED)
        self.assertEqual(errors, [])

    @async_test
    async def test_async_action_error(self) -> None:
        op = tc.TestRunner.create_async_action_with_error(10, E_FAIL)

        with self.assertRaises(OSError) as ctx:
            await op

        self.assertEqual(ctx.exception.winerror, E_FAIL)

    @async_test
    async def test_async_action_with_progress(self) -> None:
        expected = [1, 2, 3]
        op = tc.TestRunner.create_async_action_with_progress(10, expected)

        actual: list[int] = []

        def on_progress(op: wf.IAsyncActionWithProgress[int], value: int) -> None:
            actual.append(value)

        op.progress = on_progress

        await op

        self.assertEqual(op.status, wf.AsyncStatus.COMPLETED)
        self.assertListEqual(actual, expected)

    @unittest.skipIf(sys.version_info < (3, 13), "requires Python 3.13 or later")
    @async_test
    async def test_async_action_with_progress_iter(self) -> None:
        expected = [1, 2, 3]
        op = tc.TestRunner.create_async_action_with_progress(10, expected)

        actual: list[int] = []

        async for value in WinrtAiter(op):
            actual.append(value)

        self.assertEqual(op.status, wf.AsyncStatus.COMPLETED)
        self.assertListEqual(actual, expected)

    @unittest.skipIf(sys.version_info < (3, 13), "requires Python 3.13 or later")
    @async_test
    async def test_async_action_with_progress_iter_cancel(self) -> None:
        expected = [1, 2, 3]
        op = tc.TestRunner.create_async_action_with_progress(500, expected)
        asyncio.get_running_loop().call_later(0.1, op.cancel)

        actual: list[int] = []

        with self.assertRaises(asyncio.CancelledError):
            async for value in WinrtAiter(op):
                actual.append(value)

        self.assertEqual(op.status, wf.AsyncStatus.CANCELED)
        self.assertLess(len(actual), len(expected))

    @unittest.skipIf(sys.version_info < (3, 13), "requires Python 3.13 or later")
    @async_test
    async def test_async_action_with_progress_iter_cancel2(self) -> None:
        expected = [1, 2, 3, 4, 5]
        op = tc.TestRunner.create_async_action_with_progress(100, expected)

        actual: list[int] = []

        with self.assertRaises(asyncio.TimeoutError):
            async with (
                asyncio.timeout(0.2),
                # Have to do this otherwise the async op isn't canceled on timeout!
                contextlib.aclosing(WinrtAiter(op)) as aiter,
            ):
                async for value in aiter:
                    actual.append(value)

        self.assertEqual(op.status, wf.AsyncStatus.CANCELED)
        self.assertLess(len(actual), len(expected))

    @unittest.skipIf(sys.version_info < (3, 13), "requires Python 3.13 or later")
    @async_test
    async def test_async_action_with_progress_iter_error(self) -> None:
        expected = [1, 2, 3]
        op = tc.TestRunner.create_async_action_with_progress_with_error(
            10, expected, E_FAIL
        )

        actual: list[int] = []

        with self.assertRaises(OSError) as ctx:
            async for value in WinrtAiter(op):
                actual.append(value)

        self.assertEqual(ctx.exception.winerror, E_FAIL)
        self.assertEqual(op.status, wf.AsyncStatus.ERROR)
        self.assertListEqual(actual, expected)

    @async_test
    async def test_async_operation(self) -> None:
        op = tc.TestRunner.create_async_operation(10, 1)
        result = await op

        self.assertEqual(op.status, wf.AsyncStatus.COMPLETED)
        self.assertEqual(result, 1)

    @async_test
    async def test_async_operation_with_progress(self) -> None:
        expected = [1, 2, 3]
        expected_result = 4
        op = tc.TestRunner.create_async_operation_with_progress(
            10, expected, expected_result
        )

        actual: list[int] = []

        def on_progress(
            op: wf.IAsyncOperationWithProgress[int, int], value: int
        ) -> None:
            actual.append(value)

        op.progress = on_progress

        actual_result = await op

        self.assertEqual(op.status, wf.AsyncStatus.COMPLETED)
        self.assertEqual(actual_result, expected_result)
        self.assertListEqual(actual, expected)

    @async_test
    async def test_async_operation_is_not_a_future(self) -> None:
        op = tc.TestRunner.create_async_operation(10, 1)

        # Where asyncio needs a future, it makes a task around the operation.
        self.assertFalse(asyncio.isfuture(op))
        task = asyncio.ensure_future(op)
        self.assertIsInstance(task, asyncio.Task)
        self.assertEqual(await task, 1)

    @async_test
    async def test_async_operation_gather(self) -> None:
        op1 = tc.TestRunner.create_async_operation(10, 1)
        op2 = tc.TestRunner.create_async_operation(20, 2)

        self.assertEqual(await asyncio.gather(op1, op2), [1, 2])

    def _record_loop_errors(self) -> list[dict[str, Any]]:
        """
        Collects what the running loop is asked to report, which is where an
        error in a done callback goes.
        """
        errors: list[dict[str, Any]] = []

        def handler(loop: asyncio.AbstractEventLoop, context: dict[str, Any]) -> None:
            errors.append(context)

        asyncio.get_running_loop().set_exception_handler(handler)

        return errors

    async def _run_callbacks(self) -> None:
        """
        Runs the loop for long enough that a completion delivered from the
        handler, or a cancellation, reaches the tasks it is for and the tasks
        that wait for those.
        """
        for _ in range(10):
            await asyncio.sleep(0)

    @async_test
    async def test_async_operation_gather_timeout(self) -> None:
        errors = self._record_loop_errors()
        sources = [tc.AsyncOperationSource() for _ in range(2)]
        ops = [source.operation for source in sources]

        timeouts: list[asyncio.Timeout] = []

        async def gather() -> list[int]:
            async with asyncio.timeout(None) as timeout:
                timeouts.append(timeout)
                return await asyncio.gather(*ops)

        task = asyncio.create_task(gather())

        # The timeout cancels the gathering future, which cancels the task
        # around each operation, which asks WinRT to cancel it, and gather
        # finishes once the operations have. A task cancelled before it has
        # started never awaits its operation, so the timeout expires once
        # they are being awaited.
        try:
            await self._run_callbacks()
            self.assertTrue(all(source.has_completed_handler for source in sources))
            timeouts[0].reschedule(asyncio.get_running_loop().time())
            await self._run_callbacks()
            self.assertEqual([s.cancel_request_count for s in sources], [1, 1])
        finally:
            # Only the sources finish the operations, and the task does not
            # end until they have, so a failed assertion has to finish them.
            for source in sources:
                source.cancel()

        await self._run_callbacks()

        self.assertTrue(task.done())

        with self.assertRaises(TimeoutError):
            await task

        self.assertEqual([op.status for op in ops], [wf.AsyncStatus.CANCELED] * 2)
        self.assertEqual(errors, [])

    @async_test
    async def test_async_operation_gather_cancelled(self) -> None:
        errors = self._record_loop_errors()
        source = tc.AsyncOperationSource()
        op = source.operation

        # An operation cancelled through cancel() rather than through its task
        # has failed as far as asyncio is concerned.
        op.cancel()
        source.cancel()

        with self.assertRaises(OSError) as ctx:
            await asyncio.gather(op)

        self.assertEqual(ctx.exception.winerror, WIN32_ERROR_CANCELLED)
        self.assertEqual(errors, [])

    @async_test
    async def test_async_operation_gather_return_exceptions(self) -> None:
        errors = self._record_loop_errors()
        sources = [tc.AsyncOperationSource() for _ in range(2)]
        op1, op2 = (source.operation for source in sources)

        op1.cancel()
        sources[0].cancel()
        sources[1].complete(2)

        results = await asyncio.gather(op1, op2, return_exceptions=True)

        self.assertIsInstance(results[0], OSError)
        assert isinstance(results[0], OSError)
        self.assertEqual(results[0].winerror, WIN32_ERROR_CANCELLED)
        self.assertEqual(results[1], 2)
        self.assertEqual(errors, [])

    @async_test
    async def test_async_operation_shield_timeout(self) -> None:
        errors = self._record_loop_errors()
        source = tc.AsyncOperationSource()
        op = source.operation

        with self.assertRaises(TimeoutError):
            async with asyncio.timeout(0):
                await asyncio.shield(op)

        # The shield keeps the operation from being cancelled.
        self.assertEqual(source.cancel_request_count, 0)

        source.fail(E_FAIL)
        await self._run_callbacks()
        gc.collect()

        # Nothing is left to retrieve the error, so the loop reports it: since
        # 3.14 the shield does, and before that the task around the operation
        # does when it is freed.
        self.assertEqual(op.status, wf.AsyncStatus.ERROR)
        self.assertEqual(len(errors), 1)
        self.assertIsInstance(errors[0]["exception"], OSError)
        self.assertEqual(errors[0]["exception"].winerror, E_FAIL)

    @async_test
    async def test_async_operation_asyncio_wait(self) -> None:
        op1 = tc.TestRunner.create_async_operation(10, 1)
        op2 = tc.TestRunner.create_async_operation(20, 2)

        # A task around each operation is what matches the answers of
        # asyncio.wait() up with the operations.
        tasks = [asyncio.ensure_future(op) for op in (op1, op2)]
        done, pending = await asyncio.wait(tasks)

        self.assertEqual(done, set(tasks))
        self.assertEqual(pending, set())
        self.assertEqual({task.result() for task in done}, {1, 2})

    @async_test
    async def test_async_operation_falsy_result(self) -> None:
        op = tc.TestRunner.create_async_operation(10, 0)

        self.assertEqual(await op, 0)

    @async_test
    async def test_async_operation_await_twice(self) -> None:
        op = tc.TestRunner.create_async_operation(10, 1)

        self.assertEqual(await op, 1)
        self.assertEqual(await op, 1)

    @async_test
    async def test_async_operation_error_state(self) -> None:
        op = tc.TestRunner.create_async_operation_with_error(10, 1, E_FAIL)

        with self.assertRaises(OSError) as ctx:
            await op

        self.assertEqual(ctx.exception.winerror, E_FAIL)
        self.assertEqual(op.status, wf.AsyncStatus.ERROR)

    @async_test
    async def test_async_operation_cancel_is_winrts(self) -> None:
        source = tc.AsyncOperationSource()
        op = source.operation

        # cancel() is IAsyncInfo.Cancel(), a request that WinRT answers by
        # finishing the operation, which an await then raises for.
        op.cancel()
        self.assertEqual(source.cancel_request_count, 1)
        source.cancel()

        with self.assertRaises(OSError) as ctx:
            await op

        self.assertEqual(ctx.exception.winerror, WIN32_ERROR_CANCELLED)
        self.assertEqual(op.status, wf.AsyncStatus.CANCELED)

    @async_test
    async def test_async_operation_cancelled_by_the_operation(self) -> None:
        source = tc.AsyncOperationSource()
        op = source.operation

        # An operation that cancels itself, as Windows does when a device goes
        # away, has failed rather than been cancelled by Python.
        source.cancel()

        with self.assertRaises(OSError) as ctx:
            await op

        self.assertEqual(ctx.exception.winerror, WIN32_ERROR_CANCELLED)
        self.assertEqual(source.cancel_request_count, 0)

    def test_async_info_cancel(self) -> None:
        source = tc.AsyncActionSource()
        info = source.operation.as_(wf.IAsyncInfo)

        info.cancel()
        self.assertEqual(source.cancel_request_count, 1)

        source.complete()

    def test_async_handlers_refuse_none(self) -> None:
        source = tc.AsyncActionSource()
        op = source.operation

        with self.assertRaisesRegex(TypeError, "takes a callable, not None"):
            op.completed = None  # type: ignore[assignment]

        self.assertFalse(source.has_completed_handler)
        source.complete()

    def test_async_progress_refuses_none(self) -> None:
        # A C++/WinRT coroutine, which dereferences a null handler.
        op = tc.TestRunner.create_async_action_with_progress(0, [])

        with self.assertRaisesRegex(TypeError, "takes a callable, not None"):
            op.progress = None  # type: ignore[assignment]

        with self.assertRaisesRegex(TypeError, "takes a callable, not None"):
            op.completed = None  # type: ignore[assignment]

        op.get()

    async def _await_recording_cancellation(
        self, op: wf.IAsyncAction, seen: list[bool]
    ) -> None:
        try:
            await op
        except asyncio.CancelledError:
            seen.append(op.status != wf.AsyncStatus.STARTED)
            raise

    async def _assert_task_waits_for_the_operation(
        self,
        source: tc.AsyncActionSource,
        op: wf.IAsyncAction,
        task: asyncio.Task[None],
    ) -> None:
        # A task that was resumed by its cancellation alone would be done
        # within a couple of loop iterations.
        for _ in range(5):
            await asyncio.sleep(0)

        self.assertEqual(source.cancel_request_count, 1)
        self.assertEqual(op.status, wf.AsyncStatus.STARTED)
        self.assertFalse(task.done())

    @async_test
    async def test_async_operation_cancel_waits_for_the_operation(self) -> None:
        source = tc.AsyncActionSource()
        op = source.operation
        seen: list[bool] = []

        async def wait_with_timeout() -> None:
            async with asyncio.timeout(0):
                await self._await_recording_cancellation(op, seen)

        task = asyncio.create_task(wait_with_timeout())

        try:
            await self._assert_task_waits_for_the_operation(source, op, task)
            self.assertEqual(seen, [])
        finally:
            # Only the source finishes the operation, and the task does not
            # end until it has, so a failed assertion has to finish it too.
            source.cancel()

        with self.assertRaises(TimeoutError):
            await task

        # The task was resumed by the operation finishing, not before.
        self.assertEqual(seen, [True])
        self.assertEqual(op.status, wf.AsyncStatus.CANCELED)

    @async_test
    async def test_async_operation_task_cancel_waits_for_the_operation(self) -> None:
        for cancels in (1, 2):
            with self.subTest(cancels=cancels):
                source = tc.AsyncActionSource()
                op = source.operation
                seen: list[bool] = []

                task = asyncio.create_task(self._await_recording_cancellation(op, seen))
                await asyncio.sleep(0)

                for _ in range(cancels):
                    task.cancel()

                # Cancelling the task twice before it runs is one
                # cancellation, which asks the operation once.
                try:
                    await self._assert_task_waits_for_the_operation(source, op, task)
                    self.assertEqual(seen, [])
                finally:
                    source.cancel()

                with self.assertRaises(asyncio.CancelledError):
                    await task

                self.assertEqual(seen, [True])

    @async_test
    async def test_async_operation_second_cancel_gives_up_waiting(self) -> None:
        source = tc.AsyncActionSource()
        op = source.operation
        seen: list[bool] = []

        task = asyncio.create_task(self._await_recording_cancellation(op, seen))
        await asyncio.sleep(0)
        task.cancel()

        try:
            await self._assert_task_waits_for_the_operation(source, op, task)

            # An operation that never answers the request would keep the task
            # for good, so cancelling the task again gives up waiting for it
            # and leaves it running.
            task.cancel()
            await self._run_callbacks()

            self.assertTrue(task.cancelled())
            self.assertEqual(seen, [False])
            self.assertEqual(op.status, wf.AsyncStatus.STARTED)
            self.assertEqual(source.cancel_request_count, 1)
        finally:
            source.cancel()

    @async_test
    async def test_async_operation_abandoned_failure_is_reported(self) -> None:
        for finish in ("fail", "complete", "cancel"):
            with self.subTest(finish=finish):
                errors = self._record_loop_errors()
                source = tc.AsyncActionSource()
                op = source.operation

                task = asyncio.ensure_future(op)
                await asyncio.sleep(0)
                task.cancel()
                await self._run_callbacks()
                task.cancel()
                await self._run_callbacks()

                self.assertTrue(task.cancelled())

                # Nothing waits for the operation any more, so an error goes to
                # the loop's exception handler, and any other end is what the
                # cancellation asked for.
                if finish == "fail":
                    source.fail(E_FAIL)
                elif finish == "complete":
                    source.complete()
                else:
                    source.cancel()

                await self._run_callbacks()

                if finish == "fail":
                    self.assertEqual(len(errors), 1)
                    self.assertIn("gave up waiting", errors[0]["message"])
                    self.assertIsInstance(errors[0]["exception"], OSError)
                    self.assertEqual(errors[0]["exception"].winerror, E_FAIL)
                else:
                    self.assertEqual(errors, [])

    def test_async_operation_abandoned_failure_after_its_loop_closed(self) -> None:
        source = tc.AsyncActionSource()
        op = source.operation
        loop = asyncio.new_event_loop()
        errors: list[dict[str, Any]] = []

        def handler(loop: asyncio.AbstractEventLoop, context: dict[str, Any]) -> None:
            errors.append(context)

        loop.set_exception_handler(handler)

        async def wait() -> None:
            await op

        def run_callbacks() -> None:
            for _ in range(5):
                loop.run_until_complete(asyncio.sleep(0))

        task = loop.create_task(wait())
        run_callbacks()
        task.cancel()
        run_callbacks()
        task.cancel()
        run_callbacks()

        self.assertTrue(task.cancelled())

        loop.close()

        # There is no loop to hand the error to, and the closed loop's
        # exception handler is asked to report it all the same.
        source.fail(E_FAIL)

        self.assertEqual(len(errors), 1)
        self.assertIsInstance(errors[0]["exception"], OSError)
        self.assertEqual(errors[0]["exception"].winerror, E_FAIL)

    @async_test
    async def test_async_operation_cancel_ignored(self) -> None:
        source = tc.AsyncOperationSource()
        op = source.operation

        task = asyncio.ensure_future(op)
        await asyncio.sleep(0)
        task.cancel()

        try:
            await self._run_callbacks()
            self.assertEqual(source.cancel_request_count, 1)
        finally:
            # An operation that finishes anyway still leaves the task
            # cancelled, as the task was asked to be.
            source.complete(1)

        with self.assertRaises(asyncio.CancelledError):
            await task

        self.assertEqual(op.status, wf.AsyncStatus.COMPLETED)

    @async_test
    async def test_async_operation_waits_for_the_handler(self) -> None:
        source = tc.AsyncOperationSource()
        source.defers_completed_handler = True
        op = source.operation

        task = asyncio.ensure_future(op)
        await asyncio.sleep(0)

        self.assertTrue(source.has_completed_handler)

        source.complete(1)
        await self._run_callbacks()

        # The status says the operation has finished before its completed
        # handler has said so, and an await that is waiting for the handler
        # goes on waiting for it.
        self.assertEqual(op.status, wf.AsyncStatus.COMPLETED)
        self.assertFalse(task.done())

        source.invoke_completed_handler()

        self.assertEqual(await task, 1)

    @async_test
    async def test_async_operation_completed_on_another_thread(self) -> None:
        source = tc.AsyncOperationSource()
        op = source.operation
        thread = threading.Thread(target=source.complete, args=(1,))

        task = asyncio.ensure_future(op)
        await asyncio.sleep(0)
        thread.start()

        self.assertEqual(await task, 1)

        thread.join()

    @async_test
    async def test_async_operation_released(self) -> None:
        source = tc.AsyncOperationSource()
        op = source.operation
        source.complete(1)

        self.assertEqual(await op, 1)

        # Neither the future nor the completed handler that the operation
        # keeps holds the operation once it has been awaited. The loop lets go
        # of the callback that resumed this coroutine one iteration later.
        del op
        await asyncio.sleep(0)

        self.assertFalse(source.is_operation_alive)

    @async_test
    async def test_async_operation_awaited_by_two_tasks(self) -> None:
        source = tc.AsyncOperationSource()
        op = source.operation

        # WinRT takes one completed handler, so a second await of a running
        # operation fails, and a task around it is what two tasks can share.
        shared = asyncio.ensure_future(op)
        await asyncio.sleep(0)

        with self.assertRaises(OSError) as ctx:
            await op

        self.assertEqual(ctx.exception.winerror, E_ILLEGAL_DELEGATE_ASSIGNMENT)

        async def waiter() -> int:
            return await shared

        tasks = [asyncio.create_task(waiter()) for _ in range(2)]
        source.complete(1)

        self.assertEqual(await asyncio.gather(*tasks), [1, 1])

    def test_async_operation_completed_after_its_loop_closed(self) -> None:
        source = tc.AsyncOperationSource()
        op = source.operation
        loop = asyncio.new_event_loop()
        # The task is still pending when it is freed, which the loop reports.
        loop.set_exception_handler(lambda loop, context: None)

        async def wait() -> None:
            await op

        task = loop.create_task(wait())
        loop.run_until_complete(asyncio.sleep(0))
        loop.close()

        self.assertTrue(source.has_completed_handler)

        # The completion has no loop to go to, and the handler lets it go.
        with catch_unraisable() as unraisable:
            source.complete(1)

        self.assertEqual(unraisable, [])

        del task, op
        gc.collect()

        self.assertFalse(source.is_operation_alive)

    @async_test
    async def test_async_operation_error_raised_afresh(self) -> None:
        source = tc.AsyncOperationSource()
        op = source.operation
        source.fail(E_FAIL)

        errors: list[OSError] = []

        for _ in range(3):
            try:
                await op
            except OSError as error:
                errors.append(error)
                # As in CPython's test_future_traceback: the traceback does
                # not grow with every await.
                text = "".join(traceback.format_tb(error.__traceback__))
                self.assertEqual(text.count("await op"), 1)

        # Each one comes from get_results() itself, because a kept exception
        # would hold the operation through the frames of its traceback.
        self.assertEqual([e.winerror for e in errors], [E_FAIL] * 3)
        self.assertIsNot(errors[1], errors[0])

        del op, errors
        await asyncio.sleep(0)

        self.assertFalse(source.is_operation_alive)


T = TypeVar("T")


class Result(TypedDict, total=False):
    exception: BaseException


class WinrtAiter(Generic[T]):
    def __init__(self, op: wf.IAsyncActionWithProgress[T]):
        self._op = op
        # self._exception: Optional[BaseException] = None

        queue: asyncio.Queue[T] = asyncio.Queue()
        loop = asyncio.get_running_loop()
        result = Result()

        # Caution: don't add closure on self in these callbacks to avoid reference cycle

        def on_progress(op: wf.IAsyncActionWithProgress[T], value: T) -> None:
            loop.call_soon_threadsafe(queue.put_nowait, value)

        def on_completed(
            op: wf.IAsyncActionWithProgress[T], status: wf.AsyncStatus
        ) -> None:
            if status == wf.AsyncStatus.COMPLETED:
                pass
            elif status == wf.AsyncStatus.CANCELED:
                result["exception"] = asyncio.CancelledError()
            elif status == wf.AsyncStatus.ERROR:
                result["exception"] = ctypes.WinError(op.error_code)
            else:
                raise RuntimeError("unexpected status")

            # FIXME: shutdown() was introduced in 3.13, so we can't use it generally
            loop.call_soon_threadsafe(queue.shutdown)

        self._op.progress = on_progress
        self._op.completed = on_completed
        self._queue = queue
        self._loop = loop
        self._result = result

    def __aiter__(self) -> "WinrtAiter[T]":
        return self

    async def __anext__(self) -> T:
        try:
            return await self._queue.get()
        except asyncio.QueueShutDown:
            # this acts as signal that the operation is done
            pass

        if ex := self._result.pop("exception", None):
            raise ex

        raise StopAsyncIteration

    def __del__(self) -> None:
        if ex := self._result.pop("exception", None):
            self._loop.call_exception_handler(
                context={
                    "message": "WinrtAiter was not run to completion and has outstanding exception",
                    "exception": ex,
                }
            )

    async def aclose(self) -> None:
        if self._op.status == wf.AsyncStatus.STARTED:
            self._op.cancel()

        while True:
            try:
                await self._queue.get()
            except asyncio.QueueShutDown:
                self._result.pop("exception", None)
                break
