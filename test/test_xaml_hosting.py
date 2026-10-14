import unittest

from winrt.system.hresult import E_NOINTERFACE
from winrt.windows.foundation import Uri
from winrt.windows.ui.xaml.hosting import WindowsXamlManager, DesktopWindowXamlSource
from winrt.windows.ui.xaml.hosting.interop import DesktopWindowXamlSourceNative

from ._manifest import (
    is_maxversiontested_in_manifest,
    get_activation_context_detailed_information,
    create_window,
)

"""
Xaml Islands require that the maxversiontested attribute is set in the
application manifest. Python ships with a manifest that does not set this
attribute. So we have all of this extra checking with QueryActCtxW to see
if the manifest has been updated to include this attribute.

See ``scripts/fix_python_manifest_for_xaml_islands.ps1`` for a way to update
the manifest.
"""


class TestXamlHost(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        # XAML shuts down asynchronously once a thread's last manager is
        # closed, and initializing it again before then fails, so one manager
        # is held across the tests that start XAML.
        if is_maxversiontested_in_manifest():
            manager = WindowsXamlManager.initialize_for_current_thread()
            cls.addClassCleanup(manager.close)

    def test_no_instantiation(self) -> None:
        with self.assertRaisesRegex(TypeError, r"cannot create '.*' instances"):
            DesktopWindowXamlSourceNative()

    def test_no_subclass(self) -> None:
        with self.assertRaisesRegex(TypeError, r"not an acceptable base type"):

            class _(DesktopWindowXamlSourceNative):  # type: ignore
                pass

    def test_from_is_deprecated(self) -> None:
        # The warning comes before the query, so an object that is no source
        # shows it without XAML.
        with (
            self.assertWarnsRegex(
                DeprecationWarning, r"use obj\.as_\(type\)"
            ) as warning,
            self.assertRaises(OSError) as error,
        ):
            DesktopWindowXamlSourceNative._from(Uri("https://example.com/"))  # type: ignore[deprecated]

        self.assertEqual(warning.filename, __file__)
        self.assertEqual(error.exception.winerror, E_NOINTERFACE)

    @unittest.skipIf(
        not is_maxversiontested_in_manifest(),
        f"Manifest for '{get_activation_context_detailed_information()[0]}' does not specify maxversiontested",
    )
    def test_xaml_host(self) -> None:
        with WindowsXamlManager.initialize_for_current_thread():
            source = DesktopWindowXamlSource()
            native = source.as_(DesktopWindowXamlSourceNative)
            self.assertEqual(native.window_handle, 0)

            hwnd = create_window()
            native.attach_to_window(hwnd)
            self.assertNotEqual(native.window_handle, 0)

    @unittest.skipIf(
        not is_maxversiontested_in_manifest(),
        f"Manifest for '{get_activation_context_detailed_information()[0]}' does not specify maxversiontested",
    )
    def test_from_is_a_deprecated_alias_of_query_interface_(self) -> None:
        with (
            WindowsXamlManager.initialize_for_current_thread(),
            DesktopWindowXamlSource() as source,
        ):
            with self.assertWarns(DeprecationWarning):
                native = DesktopWindowXamlSourceNative._from(source)  # type: ignore[deprecated]

            self.assertEqual(native, source.as_(DesktopWindowXamlSourceNative))
