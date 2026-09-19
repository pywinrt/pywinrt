"""
A Python object that implements WinRT interfaces.

The public name of a projected interface is bound to an abstract type, and a
Python class that derives from one is saying it implements that interface. What
WinRT is handed when such an object is passed to it is a COM object with a
tearoff per interface, and these check that calls arrive at the Python object
through them.
"""

import gc
import unittest
import weakref
from typing import Any

import test_winrt.testcomponent as tc
import winrt.windows.foundation as wf
import winrt.windows.foundation.collections as wfc
from winrt.system.hresult import PYWINRT_E_UNRAISABLE_PYTHON_EXCEPTION

from ._util import catch_unraisable


class One(tc.IRequiredOne):
    def one(self) -> int:
        return 1


class Four(tc.IRequiredFour):
    def one(self) -> int:
        return 1

    def two(self) -> int:
        return 2

    def three(self) -> int:
        return 3

    def four(self) -> int:
        return 4


class TestImplements(unittest.TestCase):
    def test_method(self) -> None:
        self.assertEqual(tc.Composable.expect_required_one(One()), 1)

    def test_required_interfaces(self) -> None:
        # IRequiredFour requires the other three, and each of them is a WinRT
        # interface of its own with a vtable of its own: the object has to
        # answer all four, and each method sits in the same slot of a
        # different vtable.
        obj = Four()

        self.assertEqual(tc.Composable.expect_required_one(obj), 1)
        self.assertEqual(tc.Composable.expect_required_two(obj), 2)
        self.assertEqual(tc.Composable.expect_required_three(obj), 3)
        self.assertEqual(tc.Composable.expect_required_four(obj), 4)

    def test_isinstance(self) -> None:
        self.assertIsInstance(One(), tc.IRequiredOne)
        self.assertNotIsInstance(object(), tc.IRequiredOne)

    def test_held_by_a_winrt_collection(self) -> None:
        class Stringable(wf.IStringable):
            def to_string(self) -> str:
                return "hello"

        obj = Stringable()

        vector = tc.TestRunner.create_stringable_vector()
        vector.append(obj)

        self.assertEqual(len(vector), 1)
        # Read back out of the collection it is the object that went in, for
        # the same reason as the round trip below.
        self.assertIs(vector[0], obj)

    def test_round_trip_as_object(self) -> None:
        # Passed where WinRT wants an IInspectable and read back, the same
        # Python object comes out, because the object that stands for it
        # answers py::IPywinrtObject.
        obj = One()

        properties = wfc.PropertySet()
        # a Python object that implements a WinRT interface is not a wrapper
        properties.insert("one", obj)  # type: ignore[arg-type]

        self.assertIs(properties.lookup("one"), obj)

    def test_runtime_class_name(self) -> None:
        # An object that is not a runtime class answers with the name of the
        # first interface it implements, which is what C++/WinRT does.
        self.assertEqual(
            tc.TestRunner.expect_object(One()),  # type: ignore[arg-type]
            "TestComponent.IRequiredOne",
        )

    def test_property(self) -> None:
        class Value(wf.IPropertyValue):
            @property
            def type(self) -> wf.PropertyType:
                return wf.PropertyType.STRING

            @property
            def is_numeric_scalar(self) -> bool:
                return False

            def get_string(self) -> str:
                return "hi"

        # _from() is internal to the runtime, so no stub declares it, and
        # WinRT only ever calls the members this implements
        seen = wf.IPropertyValue._from(Value())  # type: ignore[attr-defined, abstract]

        self.assertEqual(seen.type, wf.PropertyType.STRING)
        self.assertFalse(seen.is_numeric_scalar)
        self.assertEqual(seen.get_string(), "hi")

    def test_property_setter_taking_a_delegate(self) -> None:
        # The one member shape that goes both ways at once: WinRT writes an
        # attribute of the Python object, and what it writes is a delegate,
        # which comes back to Python as a callable that invokes it.
        class Action(wf.IAsyncAction):
            def __init__(self) -> None:
                self.handler: Any = None

            @property
            def completed(self) -> Any:
                return self.handler

            @completed.setter
            def completed(self, value: Any) -> None:
                self.handler = value

            def get_results(self) -> None:
                return None

        action = Action()  # type: ignore[abstract]
        seen = wf.IAsyncAction._from(action)  # type: ignore[attr-defined, abstract]

        status = []
        seen.completed = lambda sender, value: status.append(value)

        self.assertIsNotNone(action.handler)

        action.handler(seen, wf.AsyncStatus.COMPLETED)

        self.assertEqual(status, [wf.AsyncStatus.COMPLETED])
        self.assertIsNotNone(seen.completed)

    def test_missing_method_is_unraisable(self) -> None:
        class Incomplete(tc.IRequiredOne):
            pass

        with (
            self.assertRaisesRegex(OSError, "Unraisable Python exception") as ctx,
            catch_unraisable() as exceptions,
        ):
            # leaving one() out is the point of the test
            tc.Composable.expect_required_one(Incomplete())  # type: ignore[abstract]

        self.assertEqual(ctx.exception.winerror, PYWINRT_E_UNRAISABLE_PYTHON_EXCEPTION)
        self.assertIsInstance(exceptions[0].exc_value, AttributeError)

    def test_object_is_released(self) -> None:
        obj = One()
        ref = weakref.ref(obj)

        properties = wfc.PropertySet()
        # a Python object that implements a WinRT interface is not a wrapper
        properties.insert("one", obj)  # type: ignore[arg-type]

        del obj
        gc.collect()
        self.assertIsNotNone(ref(), "the collection should still hold the object")

        properties.clear()
        del properties
        gc.collect()
        self.assertIsNone(ref())

    def test_not_an_interface(self) -> None:
        with self.assertRaisesRegex(TypeError, "expected a WinRT object"):
            tc.Composable.expect_required_one(object())  # type: ignore


if __name__ == "__main__":
    unittest.main()
