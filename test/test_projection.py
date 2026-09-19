"""
The Python types that ``winrt-runtime`` builds from a projection table.

A projection package ships a table and no compiled code, so everything a
generated extension module used to provide - the classes, the interfaces, the
structs, and the members bound to them - is made at import time by
``runtime/src/types.cpp`` and called by ``runtime/src/interp.cpp``. This is what
checks that what comes out is the same shape as what the thunks were.

``test_table.py`` checks that the generator and the runtime agree about the
bytes. This checks that the runtime turns those bytes into the projection.
"""

import importlib
import sys
import unittest
import uuid

import test_winrt.testcomponent as tc
import winrt.windows.data.json as wdj
import winrt.windows.foundation as wf
from winrt.system.hresult import E_FAIL

BLITTABLE_FIELDS = (1, 2, 3, 4, 5, 6, 7, 8.0, 9.0, uuid.UUID(int=10))


class TestModule(unittest.TestCase):
    def test_a_type_belongs_to_the_package_that_projects_it(self) -> None:
        # There is no hidden extension module any more, so a type is created
        # straight into the public module and says so.
        self.assertEqual(wf.Uri.__module__, "winrt.windows.foundation")
        self.assertEqual(tc.Class.__module__, "test_winrt.testcomponent")

    def test_an_interface_is_bound_twice(self) -> None:
        # The wrapper an instance is returned as, and the public name that
        # carries the IID and that a Python implementation would derive from.
        self.assertIsNot(tc.ITests, tc._ITests)
        self.assertEqual(tc._ITests.__name__, "_ITests")
        self.assertEqual(tc.ITests.__name__, "ITests")

    def test_reloading_keeps_the_types(self) -> None:
        before = tc.Class
        importlib.reload(sys.modules["test_winrt.testcomponent"])
        self.assertIs(sys.modules["test_winrt.testcomponent"].Class, before)


class TestClasses(unittest.TestCase):
    def test_activation(self) -> None:
        self.assertIsInstance(tc.Class(), tc.Class)

    def test_a_class_that_cannot_be_activated(self) -> None:
        with self.assertRaises(TypeError):
            tc.TestRunner()

    def test_a_member_of_a_required_interface(self) -> None:
        # Class implements IRequiredOne, which is not its default interface, so
        # reaching one() queries for it.
        self.assertEqual(tc.Class().one(), 1)

    def test_as_and_isinstance(self) -> None:
        instance = tc.Class()

        self.assertIsInstance(instance.as_(tc.IRequiredOne), tc._IRequiredOne)
        self.assertIsInstance(instance, tc.IRequiredOne)
        self.assertNotIsInstance(instance, tc.IRequiredTwo)

    def test_as_an_interface_the_object_does_not_implement(self) -> None:
        with self.assertRaises(OSError):
            tc.Class().as_(tc.IRequiredTwo)

    def test_an_interface_carries_its_iid(self) -> None:
        self.assertIsInstance(tc.IRequiredOne._guid_(), uuid.UUID)

    def test_a_property_reads_and_writes(self) -> None:
        instance = tc.Composable()

        self.assertEqual(instance.value, 0)
        instance.value = 7
        self.assertEqual(instance.value, 7)

    def test_a_derived_class_reaches_its_base(self) -> None:
        instance = tc.Derived()

        self.assertIsInstance(instance, tc.Composable)
        self.assertEqual(instance.one(), 1)
        self.assertEqual(instance.value, 0)

    def test_a_static_method(self) -> None:
        self.assertEqual(tc.TestRunner.create_time_span(1500).total_seconds(), 1.5)

    def test_a_static_method_that_returns_an_object(self) -> None:
        self.assertIsInstance(wdj.JsonValue.create_number_value(1.0), wdj.JsonValue)

    def test_an_object_from_another_package(self) -> None:
        # The parameter's type lives in a package the table only names, so the
        # type registry has to find it before the call can be made.
        self.assertEqual(wf.Uri("http://example.com/a?b=c").host, "example.com")

    def test_a_failing_hresult_becomes_an_exception(self) -> None:
        value = wdj.JsonValue.create_number_value(1.0)

        with self.assertRaises(OSError):
            value.get_string()


class TestMembers(unittest.TestCase):
    def setUp(self) -> None:
        self.tests = tc.TestRunner.make_tests()

    def test_a_method_with_no_arguments_and_no_result(self) -> None:
        self.assertIsNone(self.tests.simple())

    def test_a_method_with_several_outputs(self) -> None:
        self.assertEqual(self.tests.param7(42), (42, 42))

    def test_a_string_argument(self) -> None:
        self.assertEqual(self.tests.param12("hello"), ("hello", "hello"))

    def test_the_wrong_number_of_arguments(self) -> None:
        with self.assertRaises(TypeError):
            self.tests.param7(1, 2)

    def test_keyword_arguments_are_refused(self) -> None:
        with self.assertRaises(TypeError):
            self.tests.param7(a=1)

    def test_a_descriptor_names_itself(self) -> None:
        descriptor = tc._ITests.simple

        self.assertEqual(descriptor.__name__, "simple")
        self.assertIs(descriptor.__objclass__, tc._ITests)

    def test_a_descriptor_refuses_another_type(self) -> None:
        with self.assertRaises(TypeError):
            tc._ITests.simple(tc.Class())

    def test_an_overload_is_chosen_by_argument_count(self) -> None:
        obj = wdj.JsonObject.parse('{ "spam": "eggs" }')

        self.assertEqual(obj.get_named_string("spam"), "eggs")
        self.assertEqual(obj.get_named_string("ham", "sausage"), "sausage")


class TestStructs(unittest.TestCase):
    """
    A struct is the one value the runtime lays out itself, because the table
    says what its fields are and never where they sit: a pointer is not the
    same width on every architecture one table serves. The offsets the reader
    computes are checked against ``struct.calcsize`` in ``test_table.py``; what
    is checked here is that a value written through those offsets comes back
    unchanged from WinRT, which no arithmetic can fake.
    """

    def test_positional_fields(self) -> None:
        value = tc.Blittable(*BLITTABLE_FIELDS)

        self.assertEqual(value.a, 1)
        self.assertEqual(value.i, 9.0)
        self.assertEqual(value.j, uuid.UUID(int=10))

    def test_keyword_fields(self) -> None:
        self.assertEqual(tc.Blittable(a=1, b=2), tc.Blittable(1, 2))

    def test_fields_left_out_are_zero(self) -> None:
        self.assertEqual(tc.Blittable().a, 0)

    def test_an_unknown_field(self) -> None:
        with self.assertRaises(TypeError):
            tc.Blittable(nope=1)

    def test_equality_with_a_tuple(self) -> None:
        self.assertEqual(tc.Blittable(*BLITTABLE_FIELDS), BLITTABLE_FIELDS)

    def test_repr(self) -> None:
        self.assertTrue(repr(tc.Blittable()).startswith("Blittable(a=0, "))

    def test_replace(self) -> None:
        value = tc.Blittable(*BLITTABLE_FIELDS)
        replaced = value.__replace__(a=99)

        self.assertEqual(replaced.a, 99)
        self.assertEqual(replaced.b, value.b)
        self.assertEqual(value.a, 1)

    def test_a_round_trip_through_winrt(self) -> None:
        tests = tc.TestRunner.make_tests()
        value = tc.Blittable(*BLITTABLE_FIELDS)

        self.assertEqual(tests.param13(value, value), (value, value))

    def test_a_tuple_in_place_of_a_struct(self) -> None:
        tests = tc.TestRunner.make_tests()

        self.assertEqual(
            tests.param13(BLITTABLE_FIELDS, BLITTABLE_FIELDS),
            (tc.Blittable(*BLITTABLE_FIELDS), tc.Blittable(*BLITTABLE_FIELDS)),
        )

    def test_a_tuple_of_the_wrong_length(self) -> None:
        tests = tc.TestRunner.make_tests()
        value = tc.Blittable(*BLITTABLE_FIELDS)

        with self.assertRaises(TypeError):
            tests.param13(BLITTABLE_FIELDS[:-1], value)


class TestIntegerStructs(unittest.TestCase):
    """
    Two WinRT structs hold one integer each and are projected as subclasses of
    ``int`` rather than as wrappers with a field in them, which is how the
    table's ``hresult`` and ``event_token`` codes differ from ``int32`` and
    ``int64``: the ABI is the integer and the Python value is an instance of
    the type the record names, the way an enum works.
    """

    def failed(self) -> wf.IAsyncAction:
        action = tc.TestRunner.create_async_action_with_error(10, E_FAIL)

        with self.assertRaises(OSError):
            action.get()

        return action

    def test_an_hresult_is_an_int(self) -> None:
        self.assertTrue(issubclass(wf.HResult, int))
        self.assertTrue(issubclass(wf.EventRegistrationToken, int))

    def test_an_output_comes_back_as_the_type(self) -> None:
        error_code = self.failed().error_code

        self.assertIsInstance(error_code, wf.HResult)
        self.assertEqual(error_code, E_FAIL)

    def test_an_input_takes_a_plain_int(self) -> None:
        # E_FAIL is a plain int and the parameter is an HRESULT, which is what
        # the action above was asked for.
        self.assertEqual(self.failed().error_code, E_FAIL)

    def test_an_event_token_comes_back_as_the_type(self) -> None:
        obj = tc.Override()
        token = obj.add_overridable_called(lambda s, e: None)

        self.assertIsInstance(token, wf.EventRegistrationToken)

        # and goes back as the integer it is
        obj.remove_overridable_called(int(token))

    def test_the_field_the_type_used_to_have(self) -> None:
        error_code = self.failed().error_code

        with self.assertWarns(DeprecationWarning):
            value = error_code.value  # type: ignore[deprecated]

        self.assertEqual(value, E_FAIL)


if __name__ == "__main__":
    unittest.main()
