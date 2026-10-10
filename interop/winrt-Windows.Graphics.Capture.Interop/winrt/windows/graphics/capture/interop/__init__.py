from uuid import UUID

import winrt.runtime.interop as _runtime
import winrt._winrt_windows_graphics_capture_interop as _native
from winrt.windows.graphics.capture import GraphicsCaptureItem

__all__ = ["create_for_monitor", "create_for_window"]

# what GraphicsCaptureItem's activation factory implements for desktop apps
_IID_IGRAPHICSCAPTUREITEMINTEROP = UUID("3628e81b-3cac-4c60-b7f4-23ce0e0c3356")


def create_for_monitor(monitor: int) -> GraphicsCaptureItem:
    """
    Targets a monitor(s) for the creation of a graphics capture item.

    Args:
        monitor (HMONITOR): The monitor handle that represents the monitor to
            capture.

    Returns:
        A graphics capture item.

    Requirements:
        Windows 10 1903 (build 18334)
    """
    factory = _runtime.get_activation_factory(
        GraphicsCaptureItem, _IID_IGRAPHICSCAPTUREITEMINTEROP
    )

    return _runtime.wrap_interface(
        _native.create_for_monitor(factory, monitor),
        GraphicsCaptureItem,
    )


def create_for_window(window: int) -> GraphicsCaptureItem:
    """
    Targets a single window for the creation of a graphics capture item.

    Args:
        window (HWND): The window handle that represents the window to capture.

    Returns:
        A graphics capture item.

    Requirements:
        Windows 10 1903 (build 18334)
    """
    factory = _runtime.get_activation_factory(
        GraphicsCaptureItem, _IID_IGRAPHICSCAPTUREITEMINTEROP
    )

    return _runtime.wrap_interface(
        _native.create_for_window(factory, window),
        GraphicsCaptureItem,
    )
