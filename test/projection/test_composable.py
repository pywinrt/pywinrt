import gc
import threading
import unittest
import weakref
from typing import Any, Generic, Self, TypeVar
from uuid import UUID

import test_winrt.testcomponent as tc
from winrt.system.hresult import PYWINRT_E_UNRAISABLE_PYTHON_EXCEPTION
import winrt.windows.foundation as wf
import winrt.windows.foundation.collections as wfc
from typing_extensions import override

from ._util import catch_unraisable

IID_ISTRINGABLE = UUID("96369F54-8EB6-48F0-ABCE-C1B211E627C3")

T = TypeVar("T")


class TestComposable(unittest.TestCase):
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

    def test_composed_iids_are_listed_once(self) -> None:
        # The object composed over lists the overridable interfaces as well,
        # which the composed object answers itself.
        class D(tc.DerivedOverride):
            pass

        iids = list(D()._iids_)

        self.assertEqual(len(iids), len(set(iids)))
        # IOverrideOverrides and IDerivedOverrideOverrides
        self.assertIn(UUID("151ca069-60b7-5c26-a3f3-0cf4db7c4a10"), iids)
        self.assertIn(UUID("29af5e3f-1046-5a3e-88c7-a2eadac91378"), iids)

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
        # Overriding __new__ to pass arguments to the constructor is not the
        # way, and the type hints refuse it: an __init__ that calls
        # super().__init__(2) is. It keeps working for code that does it.
        class C(tc.Composable):
            @override
            def __new__(cls) -> Self:
                return super().__new__(cls, 2)  # type: ignore

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

    def test_a_second_composable_class_is_refused(self) -> None:
        # The object is composed into the first, so it would implement none of
        # the second's members.
        with self.assertRaisesRegex(
            TypeError,
            r"^'C' cannot derive from both '.*Override' and '.*ProtectedMembers', "
            r"because a class derived from a WinRT class derives from one class, "
            r"besides the WinRT interfaces it adds$",
        ):

            class C(tc.Override, tc.ProtectedMembers):  # type: ignore[metaclass]
                pass

    def test_an_added_interface_is_answered(self) -> None:
        # Override does not implement IStringable, so the composed object
        # answers it for the Python class.
        class C(tc.Override, wf.IStringable):
            @override
            def to_string(self) -> str:
                return "c"

        c = C()

        self.assertIn(IID_ISTRINGABLE, c._iids_)
        self.assertEqual(c.as_(wf.IStringable).to_string(), "c")

    def test_a_subclass_of_a_subclass_adds_an_interface(self) -> None:
        class C(tc.Override):
            pass

        class D(C, wf.IStringable):
            @override
            def to_string(self) -> str:
                return "d"

        d = D()

        self.assertIn(IID_ISTRINGABLE, d._iids_)
        self.assertEqual(d.as_(wf.IStringable).to_string(), "d")

    def test_a_python_base_is_refused(self) -> None:
        class Mixin:
            pass

        with self.assertRaisesRegex(TypeError, r"^'_' cannot derive from 'Mixin'"):

            class _(Mixin, tc.Override):
                pass

    def test_a_second_chain_of_bases_is_refused(self) -> None:
        # Inheritance is linear, as in C#, and a class of both would have two
        # base classes.
        class A(tc.Override, wf.IStringable):
            @override
            def to_string(self) -> str:
                return "a"

        class B(tc.Override, wf.IStringable):
            @override
            def to_string(self) -> str:
                return "b"

        with self.assertRaisesRegex(
            TypeError,
            r"^'_' cannot derive from both '.*A' and '.*B', because a class "
            r"derived from a WinRT class derives from one class, besides the "
            r"WinRT interfaces it adds$",
        ):

            class _(A, B):
                pass

    def test_a_subclass_of_a_base_class_is_refused(self) -> None:
        # D is not in the chain from DerivedOverride, so it is a second chain.
        class D(tc.Override):
            pass

        with self.assertRaisesRegex(
            TypeError,
            r"^'_' cannot derive from both '.*DerivedOverride' and 'D', because a "
            r"class derived from a WinRT class derives from one class, besides "
            r"the WinRT interfaces it adds$",
        ):

            class _(tc.DerivedOverride, D):
                pass

    def test_a_generic_class_is_refused(self) -> None:
        # A WinRT class has no type parameters.
        with self.assertRaisesRegex(
            TypeError,
            r"^'_' cannot derive from '(typing\.)?Generic', because a class "
            r"derived from a WinRT class can add only WinRT interfaces to it$",
        ):

            class _(tc.Override, Generic[T]):
                pass

    def test_a_python_base_is_refused_after_a_derived_class(self) -> None:
        class Mixin:
            pass

        class D(tc.Override):
            pass

        with self.assertRaisesRegex(TypeError, r"^'_' cannot derive from 'Mixin'"):

            class _(D, Mixin):
                pass

    def test_a_python_base_is_refused_behind_a_silent_one(self) -> None:
        # A base whose __init_subclass__() does not call the next one hides the
        # class statement, so the class is refused when it is first created.
        class Silent:
            def __init_subclass__(cls) -> None:
                pass

        class C(Silent, tc.Override):
            pass

        with self.assertRaisesRegex(TypeError, r"^'C' cannot derive from 'Silent'"):
            C()

    def test_a_silent_base_after_the_class_is_refused_at_once(self) -> None:
        # The class's own __init_subclass__() comes first in the MRO, so the
        # class statement is refused.
        class Silent:
            def __init_subclass__(cls) -> None:
                pass

        with self.assertRaisesRegex(TypeError, r"^'_' cannot derive from 'Silent'"):

            class _(tc.Override, Silent):
                pass

    def test_a_protected_constructor_does_not_create_its_class(self) -> None:
        for args in ((), (5,)):
            with self.subTest(args=args):
                with self.assertRaisesRegex(
                    TypeError,
                    "^'TestComponent.ProtectedMembers' has a protected constructor, "
                    "so only a class derived from it can be created$",
                ):
                    tc.ProtectedMembers(*args)

    def test_a_protected_constructor_composes_a_derived_class(self) -> None:
        class C(tc.ProtectedMembers):
            pass

        # Not the way, as test_overriding_new says, but it works.
        class D(tc.ProtectedMembers):
            @override
            def __new__(cls) -> Self:
                return super().__new__(cls, 6)  # type: ignore

        self.assertEqual(C()._protected_value, 0)
        self.assertEqual(C(5)._protected_value, 5)
        self.assertEqual(D()._protected_value, 6)
        self.assertIsInstance(C(), tc.ProtectedMembers)

    def test_a_derived_class_with_init_takes_its_own_arguments(self) -> None:
        class C(tc.ProtectedMembers):
            @override
            def __init__(self, scale: int = 1, *, offset: int = 0) -> None:
                super().__init__()
                self.scale = scale
                self.offset = offset

        for c, scale, offset in (
            (C(), 1, 0),
            (C(3), 3, 0),
            (C(3, offset=4), 3, 4),
            (C(offset=5), 1, 5),
        ):
            with self.subTest(scale=scale, offset=offset):
                self.assertEqual((c.scale, c.offset), (scale, offset))
                self.assertEqual(c._protected_value, 0)

    def test_a_derived_class_with_init_chooses_the_constructor(self) -> None:
        class C(tc.ProtectedMembers):
            @override
            def __init__(self, name: str, initial: int) -> None:
                super().__init__(initial)
                self.name = name

        c = C("spam", 5)
        self.assertEqual(c.name, "spam")
        self.assertEqual(c._protected_value, 5)

    def test_a_derived_class_with_init_passes_on_what_a_constructor_takes(
        self,
    ) -> None:
        class C(tc.ProtectedMembers):
            @override
            def __init__(self, initial: int) -> None:
                super().__init__(initial, initial)  # type: ignore

        with self.assertRaisesRegex(TypeError, "parameter count"):
            C(5)

        class D(tc.ProtectedMembers):
            @override
            def __init__(self) -> None:
                super().__init__(initial=5)  # type: ignore

        with self.assertRaisesRegex(TypeError, "keyword arguments"):
            D()

    def test_a_class_derived_from_a_derived_class_inherits_its_init(self) -> None:
        class C(tc.Composable):
            @override
            def __init__(self, init: int) -> None:
                super().__init__(init * 2)

        class D(C):
            pass

        self.assertEqual(D(4).value, 8)

    def test_a_class_derived_from_a_derived_class_has_its_own_init(self) -> None:
        class C(tc.Composable):
            @override
            def __init__(self, init: int, tag: str = "c") -> None:
                super().__init__(init * 2)
                self.tag = tag

        class D(C):
            @override
            def __init__(self, name: str) -> None:
                super().__init__(3, tag=name)
                self.name = name

        d = D("spam")
        self.assertEqual((d.value, d.tag, d.name), (6, "spam", "spam"))

        # Not calling super().__init__() skips C's __init__, as in any Python
        # class, and the call's arguments go to the constructor.
        class E(C):
            @override
            def __init__(self, init: int) -> None:
                self.init = init

        e = E(7)
        self.assertEqual((e.value, e.init), (7, 7))
        self.assertFalse(hasattr(e, "tag"))

    def test_a_derived_class_holds_nothing_before_init_composes_it(self) -> None:
        values: list[int] = []
        errors: list[Exception] = []

        class C(tc.ProtectedMembers):
            @override
            def __init__(self) -> None:
                try:
                    values.append(self._protected_value)
                except RuntimeError as e:
                    errors.append(e)

                super().__init__(3)
                values.append(self._protected_value)

        C()

        self.assertEqual(values, [3])
        self.assertRegex(
            str(errors[0]),
            r"^'C' object holds no WinRT object until its __init__\(\) "
            r"calls super\(\)\.__init__\(\)$",
        )

    def test_a_derived_class_whose_init_does_not_compose_it(self) -> None:
        # It holds nothing while __init__() runs, and is composed with the
        # arguments of the call once __init__() has returned, as a derived
        # class without an __init__ is.
        test = self
        message = "holds no WinRT object until its __init__"

        class C(tc.Composable):
            @override
            def __init__(self, init: int) -> None:
                with test.assertRaisesRegex(RuntimeError, message):
                    self.one()
                with test.assertRaisesRegex(RuntimeError, message):
                    self.value
                with test.assertRaisesRegex(RuntimeError, message):
                    self.value = 1
                with test.assertRaisesRegex(RuntimeError, message):
                    self._iids_
                with test.assertRaisesRegex(RuntimeError, message):
                    tc.Composable.expect_required_one(self)

                # It is no other object, and is itself.
                test.assertEqual(self, self)
                test.assertNotEqual(self, tc.Composable())

                self.init = init

        c = C(5)
        self.assertEqual(c.init, 5)
        self.assertEqual(c.value, 5)
        self.assertEqual(c.one(), 1)

    def test_a_derived_class_hashes_once_composed(self) -> None:
        # A hash taken before super().__init__() would change when the object
        # is composed, so there is none until then.
        test = self
        hashes: list[int] = []

        class C(tc.Composable):
            @override
            def __init__(self) -> None:
                with test.assertRaisesRegex(
                    RuntimeError, "holds no WinRT object until its __init__"
                ):
                    hash(self)

                super().__init__()
                hashes.append(hash(self))

        c = C()
        self.assertEqual(hashes, [hash(c)])
        self.assertIn(c, {c})

    def test_a_derived_class_has_no_runtime_class_name_before_composed(self) -> None:
        test = self

        class C(tc.Composable):
            @override
            def __init__(self) -> None:
                with test.assertRaisesRegex(
                    RuntimeError, "holds no WinRT object until its __init__"
                ):
                    self._runtime_class_name_

                super().__init__()

        self.assertIsInstance(C()._runtime_class_name_, str)

    def test_a_derived_class_has_no_events_before_composed(self) -> None:
        test = self
        message = "holds no WinRT object until its __init__"
        token = tc.Override().add_overridable_called(lambda s, e: None)
        called: list[bool] = []

        class C(tc.Override):
            @override
            def __init__(self) -> None:
                with test.assertRaisesRegex(RuntimeError, message):
                    self.add_overridable_called(lambda s, e: None)
                with test.assertRaisesRegex(RuntimeError, message):
                    self.remove_overridable_called(token)

                super().__init__()
                self.token = self.add_overridable_called(
                    lambda s, e: called.append(True)
                )

        c = C()
        c.call_overridable()
        self.assertEqual(called, [True])

        c.remove_overridable_called(c.token)
        c.call_overridable()
        self.assertEqual(called, [True])

    def test_a_weak_reference_taken_before_composed(self) -> None:
        # Composing the object does not break a weak reference to it, and the
        # pair is collected with the Python object as a reference taken
        # afterwards says.
        refs: list[weakref.ReferenceType[Any]] = []

        class C(tc.Override):
            @override
            def __init__(self) -> None:
                refs.append(weakref.ref(self))
                super().__init__()

        c = C()
        self.assertIs(refs[0](), c)

        del c
        gc.collect()
        self.assertIsNone(refs[0]())

    def test_a_derived_class_whose_init_raises_after_composing_it(self) -> None:
        refs: list[weakref.ReferenceType[Any]] = []

        class C(tc.Override):
            @override
            def __init__(self) -> None:
                super().__init__()
                refs.append(weakref.ref(self))
                raise ValueError("spam")

        with self.assertRaisesRegex(ValueError, "spam"):
            C()

        gc.collect()
        self.assertIsNone(refs[0]())

    def test_a_derived_class_made_by_new_is_composed_by_init(self) -> None:
        # Only a call to the class composes an instance whose __init__ returns
        # without calling super().__init__(), so one made by __new__ by hand
        # stays uncomposed.
        class C(tc.Composable):
            @override
            def __init__(self, init: int) -> None:
                super().__init__(init)

        c = C.__new__(C)
        c.__init__(5)  # type: ignore[misc]
        self.assertEqual(c.value, 5)

        class D(tc.Composable):
            @override
            def __init__(self, init: int) -> None:
                self.init = init

        d = D.__new__(D)
        d.__init__(6)  # type: ignore[misc]
        self.assertEqual(d.init, 6)

        with self.assertRaisesRegex(RuntimeError, "holds no WinRT object"):
            d.value

    def test_a_derived_class_whose_init_keeps_python_state_only(self) -> None:
        class C(tc.ProtectedMembers):
            @override
            def __init__(self) -> None:
                self.ready = True

        c = C()
        self.assertTrue(c.ready)
        self.assertEqual(c._protected_value, 0)

    def test_a_derived_class_whose_init_takes_what_no_constructor_does(
        self,
    ) -> None:
        class C(tc.Composable):
            @override
            def __init__(self, spacing: int = 10) -> None:
                self.spacing = spacing

        message = (
            r"^{}\.__init__\(\) did not call super\(\)\.__init__\(\), and a call "
            r"with {} fits no constructor of 'TestComponent\.Composable'$"
        )

        with self.assertRaisesRegex(
            TypeError, message.format("C", "keyword arguments")
        ):
            C(spacing=12)

        class D(tc.Composable):
            @override
            def __init__(self, min_column_width: int, spacing: int) -> None:
                self.min_column_width = min_column_width
                self.spacing = spacing

        with self.assertRaisesRegex(TypeError, message.format("D", "2 arguments")):
            D(300, 12)

    def test_a_derived_class_whose_super_init_fails(self) -> None:
        # A super().__init__() that fails, here in converting its argument,
        # leaves the object holding nothing, so __init__ can compose it
        # another way, or return and get the arguments of the call.
        test = self

        class C(tc.Composable):
            @override
            def __init__(self, init: int) -> None:
                with test.assertRaises(TypeError):
                    super().__init__("spam")  # type: ignore

                super().__init__(init)

        self.assertEqual(C(5).value, 5)

        class D(tc.Composable):
            @override
            def __init__(self, init: int) -> None:
                with test.assertRaises(TypeError):
                    super().__init__("spam")  # type: ignore

        self.assertEqual(D(6).value, 6)

        class E(tc.Composable):
            @override
            def __init__(self, init: str) -> None:
                super().__init__(init)  # type: ignore

        with self.assertRaises(TypeError):
            E("spam")

    def test_a_derived_class_with_init_is_composed_once(self) -> None:
        class C(tc.Composable):
            @override
            def __init__(self) -> None:
                super().__init__()

        c = C()

        with self.assertRaisesRegex(TypeError, "already composed"):
            c.__init__()  # type: ignore[misc]

        self.assertEqual(c.one(), 1)

    def test_a_derived_class_without_init_ignores_init(self) -> None:
        # Its arguments went to the constructor, as object.__init__() ignores
        # the arguments of a class whose __new__ took them.
        class C(tc.Composable):
            pass

        c = C(5)
        c.__init__(6)  # type: ignore[misc]
        self.assertEqual(c.value, 5)

    def test_a_derived_class_without_init_takes_what_a_constructor_takes(
        self,
    ) -> None:
        # Its call goes to the constructor, as a call to the WinRT class does,
        # and so does a call to a class derived from it in turn. Keyword
        # arguments are refused before an instance is made, so there is none
        # to finalize.
        finalized: list[str] = []

        class C(tc.Composable):
            def __del__(self) -> None:
                finalized.append(type(self).__name__)

        class D(C):
            pass

        for cls in (C, D):
            with self.subTest(cls=cls.__name__):
                finalized.clear()

                with self.assertRaisesRegex(
                    TypeError, "^keyword arguments are not supported$"
                ):
                    cls(init=5)  # type: ignore[call-overload]

                self.assertEqual(finalized, [])

                with self.assertRaisesRegex(TypeError, "^invalid parameter count$"):
                    cls(5, 6)  # type: ignore[call-overload]

    def test_a_derived_class_with_init_ignores_the_arguments_of_new(self) -> None:
        # The constructor takes what __init__ passes on, as object.__new__()
        # ignores the arguments of a class that has an __init__. Overriding
        # __new__ is not the way, as test_overriding_new says.
        class C(tc.Composable):
            @override
            def __new__(cls) -> Self:
                return super().__new__(cls, 6)  # type: ignore

            @override
            def __init__(self) -> None:
                super().__init__(7)

        self.assertEqual(C().value, 7)

    def test_a_derived_class_composed_by_init_overrides_and_is_collected(
        self,
    ) -> None:
        tags: list[str] = []

        class C(tc.Override):
            @override
            def __init__(self, tag: str) -> None:
                super().__init__()
                self.tag = tag

            @override
            def _on_overridable(self) -> None:
                tags.append(self.tag)

        c = C("spam")
        c.call_overridable()
        self.assertEqual(tags, ["spam"])

        ref = weakref.ref(c)
        del c
        gc.collect()
        self.assertIsNone(ref())

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
