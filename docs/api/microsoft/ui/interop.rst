========================================
:mod:`winrt.microsoft.ui.interop` module
========================================

Exposes interop functions from the Microsoft.UI.Interop namespace.

See: https://learn.microsoft.com/en-us/windows/windows-app-sdk/api/win32/winrt-microsoft.ui.interop/

.. currentmodule:: winrt.microsoft.ui.interop

.. function:: get_window_id_from_window(int hwnd) -> winui3.microsoft.ui.WindowId

    Gets the WindowId that corresponds to the specified hwnd, if the provided HWND is valid.

    :param version:
        The handle of the window for which to get the WindowId.

    :returns:
        The identifier that corresponds to the specified hwnd.

    :raises OSError:
        if the the OS call failed.

    .. versionadded:: 2.1

.. function:: get_window_from_window_id(winui3.microsoft.ui.WindowId windowId) -> int

    Gets the window handle that corresponds to the specified windowId, if the
    provided windowId is valid and the system has an HWND that represents the
    window.

    :param windowId:
        The identifier for the window for which to get the HWND.

    :returns:
        The handle of the window that corresponds to the specified windowId.

    :raises OSError:
        if the the OS call failed.

    .. versionadded:: 2.1

.. function:: get_display_id_from_monitor(int hmonitor) -> winui3.microsoft.ui.DisplayId

    Gets the DisplayId that corresponds to the specified hmonitor, if the
    provided HMONITOR is valid.

    :param hmonitor:
        The handle of the display monitor for which to get the DisplayId.

    :returns:

        The identifier that corresponds to the specified hmonitor.

    :raises OSError:
        if the the OS call failed.

    .. versionadded:: 2.1

.. function:: get_monitor_from_display_id(winui3.microsoft.ui.DisplayId displayId) -> int

    Gets the display monitor handle that corresponds to the specified displayId,
    if the provided displayId is valid and the system has an HMONITOR that
    represents the display monitor.

    :param displayId:
        The identifier for the display monitor for which to get the HMONITOR.

    :returns:
        The handle of the display monitor that corresponds to the specified
        displayId.

    :raises OSError:
        if the the OS call failed.

    .. versionadded:: 2.1

.. function:: get_icon_id_from_icon(int hicon) -> winui3.microsoft.ui.IconId

    Gets the IconId that corresponds to the specified hicon, if the provided
    HICON is valid.

    :param hicon:
        The handle of the icon for which to get the IconId.

    :returns:
        The identifier that corresponds to the specified hicon.

    :raises OSError:

        if the the OS call failed.

    .. versionadded:: 2.1

.. function:: get_icon_from_icon_id(winui3.microsoft.ui.IconId iconId) -> int

    Gets the icon handle that corresponds to the specified iconId, if the
    provided iconId is valid and the system has an HICON that represents the
    icon.

    :param iconId:
        The identifier for the icon for which to get the HICON.

    :returns:
        The handle of the icon that corresponds to the specified iconId.

    :raises OSError:
        if the the OS call failed.

    .. versionadded:: 2.1
