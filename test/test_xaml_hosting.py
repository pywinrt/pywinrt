import unittest

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
    def test_no_instanciation(self):
        with self.assertRaisesRegex(TypeError, r"cannot create '.*' instances"):
            DesktopWindowXamlSourceNative()

    def test_no_subclass(self):
        with self.assertRaisesRegex(TypeError, r"not an acceptable base type"):

            class _(DesktopWindowXamlSourceNative):  # type: ignore
                pass

    @unittest.skipIf(
        not is_maxversiontested_in_manifest(),
        f"Manifest for '{get_activation_context_detailed_information()[0]}' does not specify maxversiontested",
    )
    def test_xaml_host(self):
        with WindowsXamlManager.initialize_for_current_thread():
            source = DesktopWindowXamlSource()
            native = source.as_(DesktopWindowXamlSourceNative)
            self.assertEqual(native.window_handle, 0)

            hwnd = create_window()
            native.attach_to_window(hwnd)
            self.assertNotEqual(native.window_handle, 0)
