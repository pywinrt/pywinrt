from uuid import UUID

import winrt.runtime.interop as _runtime
import winrt._winrt_windows_media_interop as _native
from winrt.windows.media import SystemMediaTransportControls

__all__ = ["get_for_window"]

# what SystemMediaTransportControls' activation factory implements for desktop apps
_IID_ISYSTEMMEDIATRANSPORTCONTROLSINTEROP = UUID("ddb0472d-c911-4a1f-86d9-dc3d71a95f5a")


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
    factory = _runtime.get_activation_factory(
        SystemMediaTransportControls, _IID_ISYSTEMMEDIATRANSPORTCONTROLSINTEROP
    )

    return _runtime.wrap_interface(
        _native.get_for_window(factory, window),
        SystemMediaTransportControls,
    )
