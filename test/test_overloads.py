from test._util import skip_without_projection

skip_without_projection()

import types
import unittest

import test_winrt.testcomponent as tc
import winrt.windows.data.json as wdj
import winrt.windows.foundation.metadata as wfm
import winrt.windows.globalization.numberformatting as wgn
import winrt.windows.ui.windowmanagement as wuw


class TestOverloads(unittest.TestCase):
    def test_call_overloads_by_arg_count(self):
        o = wdj.JsonObject.parse('{ "spam": "eggs" }')
        self.assertEqual(o.get_named_string("spam"), "eggs")
        self.assertEqual(o.get_named_string("ham", "sausage"), "sausage")

    def test_wrong_arg_count_raises(self):
        o = wdj.JsonObject.parse('{ "spam": "eggs" }')
        with self.assertRaises(TypeError):
            o.get_named_string()

    def test_deprecated_alias(self):
        o = wdj.JsonObject.parse('{ "spam": "eggs" }')

        with self.assertWarns(DeprecationWarning):
            value = o.get_named_string_or_default("ham", "sausage")  # type: ignore[deprecated]

        self.assertEqual(value, "sausage")

    def test_call_static_overloads_by_arg_count(self):
        self.assertTrue(
            wfm.ApiInformation.is_method_present(
                "Windows.Storage.StorageFolder", "CreateFileAsync"
            )
        )
        self.assertTrue(
            wfm.ApiInformation.is_method_present(
                "Windows.Storage.StorageFolder", "CreateFileAsync", 2
            )
        )
        self.assertFalse(
            wfm.ApiInformation.is_method_present(
                "Windows.Storage.StorageFolder", "CreateFileAsync", 9
            )
        )

    def test_deprecated_static_alias(self):
        with self.assertWarns(DeprecationWarning):
            value = wfm.ApiInformation.is_method_present_with_arity(  # type: ignore[deprecated]
                "Windows.Storage.StorageFolder", "CreateFileAsync", 2
            )

        self.assertTrue(value)

    def test_shadowed_overloads_keep_unique_names(self):
        # Format() has three overloads that all take one argument, so only the
        # default one gets the shared name
        formatter = wgn.DecimalFormatter()

        self.assertEqual(formatter.format(2.0), formatter.format_double(2.0))
        self.assertIsInstance(formatter.format_int(2), str)
        self.assertIsInstance(formatter.format_uint(2), str)

    def test_overload_without_a_unique_name_keeps_the_shared_name(self):
        # RequestPresentation(AppWindowPresentationKind) is the default overload
        # but it is the only one of the two that has a unique name, so the other
        # one gets the shared name instead of becoming uncallable
        presenter = wuw.AppWindowPresenter

        self.assertTrue(hasattr(presenter, "request_presentation"))
        self.assertFalse(_is_alias(presenter, "request_presentation"))
        self.assertTrue(hasattr(presenter, "request_presentation_by_kind"))
        self.assertFalse(_is_alias(presenter, "request_presentation_by_kind"))

    def test_overridable_overloads_keep_unique_names(self):
        # Python methods can't be overloaded, so each overload of an overridable
        # method keeps a name of its own and gets no alias
        self.assertTrue(hasattr(tc.OverloadClass, "_overload_with_one"))
        self.assertTrue(hasattr(tc.OverloadClass, "_overload_with_two"))
        self.assertFalse(_is_alias(tc.OverloadClass, "_overload_with_one"))
        self.assertFalse(_is_alias(tc.OverloadClass, "_overload_with_two"))

    def test_protected_overloads_are_grouped(self):
        # the three argument overload is protected but not overridable, so it is
        # called by argument count like a public method
        self.assertTrue(hasattr(tc.OverloadClass, "_overload"))
        self.assertTrue(_is_alias(tc.OverloadClass, "_overload_with_three"))


def _is_alias(typ: type, name: str) -> bool:
    """Tests if a name of a projected type is a deprecated alias.

    Aliases are added in Python, so they are plain functions, while the methods
    of a projected type are implemented in C.
    """
    return isinstance(vars(typ).get(name), types.FunctionType)


if __name__ == "__main__":
    unittest.main()
