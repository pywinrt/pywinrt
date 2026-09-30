"""
A WinUI 2 window, hosted in a XAML Island.

WinUI 2 is built on the system XAML in Windows.UI.Xaml, which a desktop
process can only use through a XAML Island: a DesktopWindowXamlSource attached
to a window the process creates itself. This does not run from python.exe:
build it with "uv run build.py" instead, and see README.md for why.
"""

import ctypes
from ctypes import wintypes

from typing_extensions import override
from winrt.runtime import ApartmentType, init_apartment
from winrt.system import Array
from winrt.windows.ui.xaml import Application, FrameworkElement
from winrt.windows.ui.xaml.controls import Button
from winrt.windows.ui.xaml.hosting import DesktopWindowXamlSource, WindowsXamlManager
from winrt.windows.ui.xaml.hosting.interop import DesktopWindowXamlSourceNative
from winrt.windows.ui.xaml.interop import TypeKind, TypeName
from winrt.windows.ui.xaml.markup import (
    IXamlMetadataProvider,
    IXamlType,
    XamlReader,
    XmlnsDefinition,
)
from winui2.microsoft.ui.xaml.controls import XamlControlsResources
from winui2.microsoft.ui.xaml.xamltypeinfo import XamlControlsXamlMetaDataProvider

# The framework package that WinUI 2 is installed as.
WINUI2_PACKAGE_FAMILY = "Microsoft.UI.Xaml.2.8_8wekyb3d8bbwe"

# XAML can be inline like this or saved in a separate file. Or you can do
# everything programmatically if you rather. An island has no Window of its
# own, so the root is the content that goes in it.
XAML = """
<StackPanel xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
    xmlns:muxc="using:Microsoft.UI.Xaml.Controls"
    VerticalAlignment="Center" HorizontalAlignment="Center" Spacing="10">
    <muxc:InfoBar Title="WinUI 2" Message="This is a WinUI 2 control."
        IsOpen="True" IsClosable="False"/>
    <Button Name="click" Content="Click me" HorizontalAlignment="Center"/>
    <Button Name="exit" Content="Exit" HorizontalAlignment="Center"/>
</StackPanel>
"""


# Win32
################################################################################
#
# The window the island is attached to, and the message loop that drives it.

user32 = ctypes.WinDLL("user32", use_last_error=True)
kernel32 = ctypes.WinDLL("kernel32", use_last_error=True)
kernelbase = ctypes.WinDLL("kernelbase")

LRESULT = ctypes.c_ssize_t
WNDPROC = ctypes.WINFUNCTYPE(
    LRESULT, wintypes.HWND, wintypes.UINT, wintypes.WPARAM, wintypes.LPARAM
)


class WNDCLASSW(ctypes.Structure):
    _fields_ = [
        ("style", wintypes.UINT),
        ("lpfnWndProc", WNDPROC),
        ("cbClsExtra", ctypes.c_int),
        ("cbWndExtra", ctypes.c_int),
        ("hInstance", wintypes.HINSTANCE),
        ("hIcon", wintypes.HICON),
        ("hCursor", wintypes.HANDLE),
        ("hbrBackground", wintypes.HBRUSH),
        ("lpszMenuName", wintypes.LPCWSTR),
        ("lpszClassName", wintypes.LPCWSTR),
    ]


kernel32.GetModuleHandleW.argtypes = [wintypes.LPCWSTR]
kernel32.GetModuleHandleW.restype = wintypes.HMODULE
user32.RegisterClassW.argtypes = [ctypes.POINTER(WNDCLASSW)]
user32.RegisterClassW.restype = wintypes.ATOM
user32.CreateWindowExW.argtypes = [
    wintypes.DWORD,
    wintypes.LPCWSTR,
    wintypes.LPCWSTR,
    wintypes.DWORD,
    ctypes.c_int,
    ctypes.c_int,
    ctypes.c_int,
    ctypes.c_int,
    wintypes.HWND,
    wintypes.HMENU,
    wintypes.HINSTANCE,
    wintypes.LPVOID,
]
user32.CreateWindowExW.restype = wintypes.HWND
user32.DefWindowProcW.argtypes = [
    wintypes.HWND,
    wintypes.UINT,
    wintypes.WPARAM,
    wintypes.LPARAM,
]
user32.DefWindowProcW.restype = LRESULT
user32.DestroyWindow.argtypes = [wintypes.HWND]
user32.GetClientRect.argtypes = [wintypes.HWND, ctypes.POINTER(wintypes.RECT)]
user32.SetWindowPos.argtypes = [
    wintypes.HWND,
    wintypes.HWND,
    ctypes.c_int,
    ctypes.c_int,
    ctypes.c_int,
    ctypes.c_int,
    wintypes.UINT,
]
user32.SetFocus.argtypes = [wintypes.HWND]
user32.GetMessageW.argtypes = [
    ctypes.POINTER(wintypes.MSG),
    wintypes.HWND,
    wintypes.UINT,
    wintypes.UINT,
]
user32.TranslateMessage.argtypes = [ctypes.POINTER(wintypes.MSG)]
user32.DispatchMessageW.argtypes = [ctypes.POINTER(wintypes.MSG)]

WM_DESTROY = 0x0002
WM_SIZE = 0x0005
WM_SETFOCUS = 0x0007
WS_OVERLAPPEDWINDOW = 0x00CF0000
WS_VISIBLE = 0x10000000
CW_USEDEFAULT = -0x80000000
SWP_SHOWWINDOW = 0x0040


class HostWindow:
    """
    A top-level window that fills its client area with a XAML Island.
    """

    CLASS_NAME = "PyWinRTXamlIslandHost"

    def __init__(self, title: str, width: int, height: int) -> None:
        self.island_hwnd = 0

        # The class keeps a pointer to the callback, so the callback has to be
        # kept alive for as long as the window is.
        self._wndproc = WNDPROC(self._on_message)

        hinstance = kernel32.GetModuleHandleW(None)

        wc = WNDCLASSW()
        wc.lpfnWndProc = self._wndproc
        wc.hInstance = hinstance
        wc.lpszClassName = self.CLASS_NAME

        if not user32.RegisterClassW(ctypes.byref(wc)):
            raise ctypes.WinError(ctypes.get_last_error())

        self.hwnd = user32.CreateWindowExW(
            0,
            self.CLASS_NAME,
            title,
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            width,
            height,
            None,
            None,
            hinstance,
            None,
        )

        if not self.hwnd:
            raise ctypes.WinError(ctypes.get_last_error())

    def fit_island(self) -> None:
        """
        Sizes the island to the client area of the window.
        """
        rect = wintypes.RECT()
        user32.GetClientRect(self.hwnd, ctypes.byref(rect))
        user32.SetWindowPos(
            self.island_hwnd, None, 0, 0, rect.right, rect.bottom, SWP_SHOWWINDOW
        )

    def close(self) -> None:
        user32.DestroyWindow(self.hwnd)

    def _on_message(self, hwnd: int, msg: int, wparam: int, lparam: int) -> int:
        if msg == WM_SIZE and self.island_hwnd:
            self.fit_island()
            return 0

        if msg == WM_SETFOCUS and self.island_hwnd:
            user32.SetFocus(self.island_hwnd)
            return 0

        if msg == WM_DESTROY:
            user32.PostQuitMessage(0)
            return 0

        return user32.DefWindowProcW(hwnd, msg, wparam, lparam)


def run_message_loop(source: DesktopWindowXamlSourceNative) -> None:
    """
    Runs until the window is closed, giving the island first look at each
    message so that keyboard navigation and accelerators work inside it.
    """
    msg = wintypes.MSG()

    while user32.GetMessageW(ctypes.byref(msg), None, 0, 0) > 0:
        if source.pretranslate_message(msg):
            continue

        user32.TranslateMessage(ctypes.byref(msg))
        user32.DispatchMessageW(ctypes.byref(msg))


# Dynamic dependencies
################################################################################
#
# WinUI 2 is not installed with Windows but as a framework package, and a
# process that is not packaged itself can only activate classes from one after
# adding it to its package graph. These are the Windows 11 APIs for that, from
# appmodel.h.


class PACKAGE_VERSION(ctypes.Structure):
    _fields_ = [("Version", ctypes.c_uint64)]


kernelbase.TryCreatePackageDependency.argtypes = [
    wintypes.LPVOID,
    wintypes.LPCWSTR,
    PACKAGE_VERSION,
    ctypes.c_int,
    ctypes.c_int,
    wintypes.LPCWSTR,
    ctypes.c_int,
    ctypes.POINTER(wintypes.LPWSTR),
]
kernelbase.TryCreatePackageDependency.restype = ctypes.HRESULT
kernelbase.AddPackageDependency.argtypes = [
    wintypes.LPCWSTR,
    ctypes.c_int32,
    ctypes.c_int,
    ctypes.POINTER(wintypes.HANDLE),
    ctypes.POINTER(wintypes.LPWSTR),
]
kernelbase.AddPackageDependency.restype = ctypes.HRESULT


def add_package_dependency(package_family_name: str) -> str:
    """
    Adds the newest installed version of a framework package to the package
    graph of this process, for as long as the process runs, and returns the
    full name of the package it resolved to.
    """
    dependency_id = wintypes.LPWSTR()

    # No minimum version, no architecture filter, and a dependency that lives
    # as long as the process does.
    kernelbase.TryCreatePackageDependency(
        None,
        package_family_name,
        PACKAGE_VERSION(0),
        0,
        0,
        None,
        0,
        ctypes.byref(dependency_id),
    )

    context = wintypes.HANDLE()
    package_full_name = wintypes.LPWSTR()
    kernelbase.AddPackageDependency(
        dependency_id,
        0,
        0,
        ctypes.byref(context),
        ctypes.byref(package_full_name),
    )

    return package_full_name.value or ""


# XAML
################################################################################


# An Application that implements IXamlMetadataProvider is how XAML finds the
# WinUI 2 types, and its resources are where WinUI 2's styles are merged in. In
# an island it has to exist before WindowsXamlManager does.
#
# Nothing launches an island the way the system launches a UWP app, so
# _on_launched() is never called and main() does the setup that would go there.
class App(Application, IXamlMetadataProvider):
    def __init__(self) -> None:
        self._provider = XamlControlsXamlMetaDataProvider()

    @override
    def get_xaml_type(self, type: TypeName | tuple[str, TypeKind]) -> IXamlType:
        return self._provider.get_xaml_type(type)

    @override
    def get_xaml_type_by_full_name(self, full_name: str) -> IXamlType:
        return self._provider.get_xaml_type_by_full_name(full_name)

    @override
    def get_xmlns_definitions(self) -> Array[XmlnsDefinition]:
        return self._provider.get_xmlns_definitions()


def main() -> None:
    init_apartment(ApartmentType.SINGLE_THREADED)

    add_package_dependency(WINUI2_PACKAGE_FAMILY)

    app = App()

    # XAML calls back into Python while it is torn down, so both of these are
    # closed before main() returns, even when something in it raises.
    #
    # This is what fails in an executable without the manifest in README.md.
    with WindowsXamlManager.initialize_for_current_thread():
        # Have to add some default resources here, otherwise the
        # app will crash when trying to create menus
        app.resources.merged_dictionaries.append(XamlControlsResources())

        window = HostWindow("Hello, WinUI 2", 400, 300)

        with DesktopWindowXamlSource() as source:
            native = source.as_(DesktopWindowXamlSourceNative)
            native.attach_to_window(window.hwnd)
            window.island_hwnd = native.window_handle

            # Load the XAML
            content = XamlReader.load(XAML).as_(FrameworkElement)

            # Wire up events
            button = content.find_name("click").as_(Button)
            button.add_click(lambda s, e: print("Hello, world!"))

            exit_button = content.find_name("exit").as_(Button)
            exit_button.add_click(lambda s, e: window.close())

            # Tabbing past the last control asks the host to take focus, and
            # with nothing else in the window to give it to, it goes back into
            # the island at the other end.
            source.add_take_focus_requested(lambda s, e: s.navigate_focus(e.request))

            # Show it!
            source.content = content
            window.fit_island()

            # The window had focus before the island was in it, so the island
            # is given it now; the keyboard reaches XAML only through there.
            user32.SetFocus(window.island_hwnd)

            run_message_loop(native)


if __name__ == "__main__":
    main()
