import winrt._winrt as _runtime
import winrt._winrt_windows_media_interop as _native
from winrt.windows.media import SystemMediaTransportControls

__all__ = ["get_for_window"]


def get_for_window(window: int) -> SystemMediaTransportControls:
    """
    Gets an instance of the SystemMediaTransportControls interface for the
    specified window.

    Args:
        window (HWND): The top-level app window for which the
            ISystemMediaTransportControls interface is retrieved.

    Returns:
        The ISystemMediaTransportControls that corresponds to the appWindow
        window.
    """
    return _runtime.wrap_interface(
        _native.get_for_window(window),
        "winrt.windows.media.SystemMediaTransportControls",
    )
