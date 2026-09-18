"""
The type registry, which maps a WinRT type to its Python wrapper type.

A projection module asks the registry for a wrapper type by qualified name
every time it converts a value, so ``py::get_python_type_for<T>()`` memoizes
what it is told. The registry that told it belongs to the interpreter that
imported ``winrt._winrt``, and every interpreter builds its own wrapper types,
so a memo is only good for one of them. Nothing else catches a memo that
answers for the wrong interpreter - the answer is a working object of a type
that is not the one this interpreter's ``isinstance`` knows about - so it is
checked here.
"""

import unittest
import uuid

try:
    from _testcapi import run_in_subinterp  # type: ignore[import-not-found]
except ImportError:
    run_in_subinterp = None

import test_winrt.testcomponent as tc
import winrt.windows.data.json as wdj

# ten fields of a TestComponent.Blittable, which Param13() takes two of and
# returns two of, so it reaches the registry in both directions
FIELDS = (1, 2, 3, 4, 5, 6, 7, 8.0, 9.0, uuid.UUID(int=0))

# What the sub-interpreter does: the same conversions the main interpreter has
# already made, checked against the types it can see itself.
SUBINTERP_CODE = """
import uuid

import test_winrt.testcomponent as tc
import winrt.windows.data.json as wdj

value = wdj.JsonValue.create_number_value(2.0)
assert type(value) is wdj.JsonValue, "wrapper type came from another interpreter"
assert id(wdj.JsonValue) != {json_value_id}, "sharing the main interpreter's type"

fields = (1, 2, 3, 4, 5, 6, 7, 8.0, 9.0, uuid.UUID(int=0))
first, _ = tc.TestRunner.make_tests().param13(fields, tc.Blittable(*fields))
assert type(first) is tc.Blittable, "struct type came from another interpreter"
assert first.c == 3, first
"""


@unittest.skipIf(run_in_subinterp is None, "_testcapi is not available")
class TestSubinterpreter(unittest.TestCase):
    def convert_both_ways(self) -> None:
        """
        One conversion of a class and one of a struct, in both directions.
        """
        self.assertIs(type(wdj.JsonValue.create_number_value(1.0)), wdj.JsonValue)

        tests = tc.TestRunner.make_tests()
        first, _ = tests.param13(FIELDS, tc.Blittable(*FIELDS))

        self.assertIs(type(first), tc.Blittable)

    def test_wrapper_types_are_not_shared(self) -> None:
        # fill the memos here first: a memo that was never taken cannot be
        # handed to the wrong interpreter
        self.convert_both_ways()

        code = SUBINTERP_CODE.format(json_value_id=id(wdj.JsonValue))

        self.assertEqual(run_in_subinterp(code), 0)

        # the sub-interpreter's registry has been torn down by now, and a memo
        # taken while it was up would point into it
        self.convert_both_ways()
