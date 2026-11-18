import datetime
import unittest

import winrt._winrt_microsoft_ui_xaml_media_dxinterop as _native
from winrt.runtime.interop import as_interface
from winrt.system.hresult import E_NOINTERFACE
from winrt.windows.foundation import Uri
from winrt.microsoft.ui.xaml.media.dxinterop import SwapChainPanelNative

# A SwapChainPanel needs a WinUI 3 Application running on the thread, so these
# check only what does not: that the class is one as_() takes, that it refuses
# an object that is not a panel, and the error paths of the extension module.


class TestSwapChainPanelNative(unittest.TestCase):
    def test_no_instantiation(self) -> None:
        with self.assertRaisesRegex(TypeError, r"cannot create '.*' instances"):
            SwapChainPanelNative()

    def test_no_subclass(self) -> None:
        with self.assertRaisesRegex(TypeError, r"not an acceptable base type"):

            class _(SwapChainPanelNative):  # type: ignore
                pass

    def test_not_a_panel(self) -> None:
        with self.assertRaises(OSError) as cm:
            Uri("https://example.com/").as_(SwapChainPanelNative)

        self.assertEqual(cm.exception.winerror, E_NOINTERFACE)
        # the failed query for ISwapChainPanelNative2 is not shown with it
        self.assertIsNone(cm.exception.__context__)


class TestExtensionModule(unittest.TestCase):
    def setUp(self) -> None:
        self.not_a_panel = as_interface(Uri("https://example.com/"), Uri)

    def test_set_swap_chain_not_a_panel(self) -> None:
        with self.assertRaises(OSError) as cm:
            _native.set_swap_chain(self.not_a_panel, None)

        self.assertEqual(cm.exception.winerror, E_NOINTERFACE)

    def test_set_swap_chain_not_a_swap_chain(self) -> None:
        with self.assertRaises(OSError) as cm:
            _native.set_swap_chain(self.not_a_panel, self.not_a_panel)

        self.assertEqual(cm.exception.winerror, E_NOINTERFACE)

    def test_set_swap_chain_wrong_capsule(self) -> None:
        with self.assertRaises(ValueError):
            _native.set_swap_chain(self.not_a_panel, datetime.datetime_CAPI)  # type: ignore[arg-type]

    def test_set_swap_chain_handle_not_a_panel(self) -> None:
        # only ISwapChainPanelNative2 has SetSwapChainHandle()
        with self.assertRaises(OSError) as cm:
            _native.set_swap_chain_handle(self.not_a_panel, 1)

        self.assertEqual(cm.exception.winerror, E_NOINTERFACE)

    def test_set_swap_chain_handle_not_an_int(self) -> None:
        with self.assertRaises(TypeError):
            _native.set_swap_chain_handle(self.not_a_panel, "handle")  # type: ignore[arg-type]


if __name__ == "__main__":
    unittest.main()
