import winrt._winrt_microsoft_ui_interop as _native
from winrt.microsoft.ui import DisplayId, IconId, WindowId

__all__ = [
    "get_window_id_from_window",
    "get_window_from_window_id",
    "get_display_id_from_monitor",
    "get_monitor_from_display_id",
    "get_icon_id_from_icon",
    "get_icon_from_icon_id",
]


def get_window_id_from_window(hwnd: int) -> WindowId:
    """
    Gets the WindowId that corresponds to the specified hwnd, if the provided
    HWND is valid.
    """
    return WindowId(_native.get_window_id_from_window(hwnd))


def get_window_from_window_id(window_id: WindowId) -> int:
    """
    Gets the window handle that corresponds to the specified windowId, if the
    provided windowId is valid and the system has an HWND that represents the
    window.
    """
    return _native.get_window_from_window_id(window_id.value)


def get_display_id_from_monitor(hmonitor: int) -> DisplayId:
    """
    Gets the DisplayId that corresponds to the specified hmonitor, if the
    provided HMONITOR is valid.
    """
    return DisplayId(_native.get_display_id_from_monitor(hmonitor))


def get_monitor_from_display_id(display_id: DisplayId) -> int:
    """
    Gets the display monitor handle that corresponds to the specified displayId,
    if the provided displayId is valid and the system has an HMONITOR that
    represents the display monitor.
    """
    return _native.get_monitor_from_display_id(display_id.value)


def get_icon_id_from_icon(hicon: int) -> IconId:
    """
    Gets the IconId that corresponds to the specified hicon, if the provided
    HICON is valid.
    """
    return IconId(_native.get_icon_id_from_icon(hicon))


def get_icon_from_icon_id(icon_id: IconId) -> int:
    """
    Gets the icon handle that corresponds to the specified iconId, if the
    provided iconId is valid and the system has an HICON that represents the
    icon.
    """
    return _native.get_icon_from_icon_id(icon_id.value)
