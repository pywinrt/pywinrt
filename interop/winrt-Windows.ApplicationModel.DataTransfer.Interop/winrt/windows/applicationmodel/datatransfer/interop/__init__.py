from uuid import UUID

import winrt.runtime.interop as _runtime
import winrt._winrt_windows_applicationmodel_datatransfer_interop as _native
from winrt.windows.applicationmodel.datatransfer import DataTransferManager

__all__ = ["get_for_window", "show_share_ui_for_window"]

# what DataTransferManager's activation factory implements for desktop apps
_IID_IDATATRANSFERMANAGERINTEROP = UUID("3a3dcd6c-3eab-43dc-bcde-45671ce800c8")


def _factory() -> _runtime.InterfaceCapsule:
    return _runtime.get_activation_factory(
        DataTransferManager, _IID_IDATATRANSFERMANAGERINTEROP
    )


def get_for_window(hwnd: int, /) -> DataTransferManager:
    """
    Gets the DataTransferManager of a window, whose data_requested event asks
    for what to share when the share sheet is shown for the window.

    A packaged app calls DataTransferManager.get_for_current_view(); a desktop
    app, which has no view, calls this with its window.

    Args:
        hwnd (HWND): The window whose DataTransferManager to get.

    Returns:
        The DataTransferManager of the window.
    """
    return _runtime.wrap_interface(
        _native.get_for_window(_factory(), hwnd), DataTransferManager
    )


def show_share_ui_for_window(hwnd: int, /) -> None:
    """
    Shows the share sheet for a window. The window's DataTransferManager (see
    get_for_window()) raises data_requested, whose handler fills in what to share.

    Args:
        hwnd (HWND): The window to show the share sheet for.
    """
    _native.show_share_ui_for_window(_factory(), hwnd)
