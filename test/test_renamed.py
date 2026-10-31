import unittest

import test_winrt.testcomponent as tc
import winrt.runtime._internals as internals
from winrt.system import Object
import winrt.windows.foundation as wf
import winrt.windows.foundation.metadata as wfm
import winrt.windows.globalization as wg
import winrt.windows.globalization.numberformatting as wgn

# The aliases are added to the real projected types, which the rest of the
# tests share, so every old name here is one no projection has.


class TestRenamed(unittest.TestCase):
    def test_method(self) -> None:
        internals.renamed_method(
            wgn.DecimalFormatter, "renamed_test_format", "format_double"
        )
        formatter = wgn.DecimalFormatter()

        with self.assertWarns(DeprecationWarning) as cm:
            value = getattr(formatter, "renamed_test_format")(2.0)

        self.assertEqual(value, formatter.format_double(2.0))
        self.assertEqual(
            str(cm.warning),
            "DecimalFormatter.renamed_test_format() is deprecated, "
            "use format_double() instead",
        )
        self.assertEqual(cm.filename, __file__)

    def test_static_method(self) -> None:
        internals.renamed_static_method(
            wfm.ApiInformation, "renamed_test_is_type_present", "is_type_present"
        )

        with self.assertWarns(DeprecationWarning) as cm:
            value = getattr(wfm.ApiInformation, "renamed_test_is_type_present")(
                "Windows.Foundation.Uri"
            )

        self.assertTrue(value)
        self.assertEqual(
            str(cm.warning),
            "ApiInformation.renamed_test_is_type_present() is deprecated, "
            "use is_type_present() instead",
        )
        self.assertEqual(cm.filename, __file__)

    def test_interface_method(self) -> None:
        tests = tc.TestRunner.make_tests()

        # an interface's members are reached through a wrapper type of its own
        internals.renamed_method(type(tests), "renamed_test_simple", "simple")

        with self.assertWarns(DeprecationWarning) as cm:
            getattr(tests, "renamed_test_simple")()

        self.assertEqual(
            str(cm.warning),
            "ITests.renamed_test_simple() is deprecated, use simple() instead",
        )

    def test_property(self) -> None:
        internals.renamed_property(
            wgn.DecimalFormatter, "renamed_test_digits", "fraction_digits"
        )
        formatter = wgn.DecimalFormatter()

        with self.assertWarns(DeprecationWarning) as cm:
            setattr(formatter, "renamed_test_digits", 3)

        self.assertEqual(formatter.fraction_digits, 3)
        self.assertEqual(cm.filename, __file__)

        with self.assertWarns(DeprecationWarning) as cm:
            value = getattr(formatter, "renamed_test_digits")

        self.assertEqual(value, 3)
        self.assertEqual(
            str(cm.warning),
            "DecimalFormatter.renamed_test_digits is deprecated, "
            "use fraction_digits instead",
        )
        self.assertEqual(cm.filename, __file__)

    def test_static_property(self) -> None:
        internals.renamed_static_property(
            wg.CalendarIdentifiers, "renamed_test_gregorian", "gregorian"
        )

        with self.assertWarns(DeprecationWarning) as cm:
            value = getattr(wg.CalendarIdentifiers, "renamed_test_gregorian")

        self.assertEqual(value, wg.CalendarIdentifiers.gregorian)
        self.assertEqual(
            str(cm.warning),
            "CalendarIdentifiers.renamed_test_gregorian is deprecated, "
            "use gregorian instead",
        )
        self.assertEqual(cm.filename, __file__)

        # setting it is setting the new name, which is read-only
        with self.assertWarns(DeprecationWarning):
            with self.assertRaises(AttributeError):
                setattr(wg.CalendarIdentifiers, "renamed_test_gregorian", "")

    def test_enum_member(self) -> None:
        internals.renamed_enum_member(
            wf.AsyncStatus, "RENAMED_TEST_COMPLETED", "COMPLETED"
        )

        with self.assertWarns(DeprecationWarning) as cm:
            value = getattr(wf.AsyncStatus, "RENAMED_TEST_COMPLETED")

        self.assertIs(value, wf.AsyncStatus.COMPLETED)
        self.assertEqual(
            str(cm.warning),
            "AsyncStatus.RENAMED_TEST_COMPLETED is deprecated, use COMPLETED instead",
        )
        self.assertEqual(cm.filename, __file__)

        # an alias is not a member
        self.assertNotIn("RENAMED_TEST_COMPLETED", wf.AsyncStatus.__members__)

        with self.assertRaises(KeyError):
            wf.AsyncStatus["RENAMED_TEST_COMPLETED"]

    def test_flag_enum_member(self) -> None:
        internals.renamed_enum_member(
            wfm.AttributeTargets, "RENAMED_TEST_METHOD", "METHOD"
        )

        with self.assertWarns(DeprecationWarning):
            value = getattr(wfm.AttributeTargets, "RENAMED_TEST_METHOD")

        self.assertIs(value, wfm.AttributeTargets.METHOD)

    def test_real_attribute_wins(self) -> None:
        # static members are implemented by the metaclass of the type
        api_information = type(wfm.ApiInformation)
        calendar_identifiers = type(wg.CalendarIdentifiers)

        cases = [
            (
                internals.renamed_method,
                wgn.DecimalFormatter,
                wgn.DecimalFormatter,
                "format_int",
            ),
            (
                internals.renamed_property,
                wgn.DecimalFormatter,
                wgn.DecimalFormatter,
                "integer_digits",
            ),
            (
                internals.renamed_static_method,
                wfm.ApiInformation,
                api_information,
                "is_type_present",
            ),
            (
                internals.renamed_static_property,
                wg.CalendarIdentifiers,
                calendar_identifiers,
                "julian",
            ),
            (internals.renamed_enum_member, wf.AsyncStatus, wf.AsyncStatus, "ERROR"),
        ]

        for rename, typ, owner, old in cases:
            with self.subTest(old):
                before = vars(owner)[old]
                rename(typ, old, "spam")
                self.assertIs(vars(owner)[old], before)


class TestRenamedMembers(unittest.TestCase):
    """
    The aliases the generator writes for the list of renames that the test
    component is generated with, whose old names are made up.
    """

    def test_method(self) -> None:
        tests = tc.TestRunner.make_tests()

        with self.assertWarns(DeprecationWarning) as cm:
            tests.renamed_simple()  # type: ignore[deprecated]

        self.assertEqual(
            str(cm.warning),
            "ITests.renamed_simple() is deprecated, use simple() instead",
        )

    def test_static_method(self) -> None:
        with self.assertWarns(DeprecationWarning):
            tests = tc.TestRunner.renamed_make_tests()  # type: ignore[deprecated]

        self.assertIsInstance(tests, tc.ITests)

    def test_property(self) -> None:
        source = tc.AsyncActionSource()

        with self.assertWarns(DeprecationWarning):
            source.renamed_defers_completed_handler = True  # type: ignore[deprecated]

        self.assertTrue(source.defers_completed_handler)

        with self.assertWarns(DeprecationWarning):
            value = source.renamed_defers_completed_handler  # pyright: ignore[reportDeprecated]

        self.assertTrue(value)

    def test_interface_property(self) -> None:
        tests = tc.TestRunner.make_tests()

        with self.assertWarns(DeprecationWarning):
            value = tests.renamed_percentage  # type: ignore[deprecated]

        self.assertEqual(value, tests.percentage)

    def test_event(self) -> None:
        calls: list[object] = []
        obj = tc.Override()

        with self.assertWarns(DeprecationWarning):
            token = obj.add_renamed_overridable_called(  # type: ignore[deprecated]
                lambda sender, args: calls.append(sender)
            )

        obj.call_overridable()
        self.assertEqual(len(calls), 1)

        with self.assertWarns(DeprecationWarning):
            obj.remove_renamed_overridable_called(token)  # type: ignore[deprecated]

        obj.call_overridable()
        self.assertEqual(len(calls), 1, "the handler was not removed")

    def test_interface_event(self) -> None:
        tests = tc.TestRunner.make_tests()

        def handler(sender: Object, args: int) -> None:
            pass

        with self.assertWarns(DeprecationWarning):
            token = tests.add_renamed_event1(handler)  # type: ignore[deprecated]

        with self.assertWarns(DeprecationWarning):
            tests.remove_renamed_event1(token)  # type: ignore[deprecated]


if __name__ == "__main__":
    unittest.main()
