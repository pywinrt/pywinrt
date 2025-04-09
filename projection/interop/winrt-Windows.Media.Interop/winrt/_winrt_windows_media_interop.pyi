from winrt.windows.media import SystemMediaTransportControls

def get_for_window(window: int) -> SystemMediaTransportControls:
    """
    Gets an instance of the SystemMediaTransportControls interface for the specified window.

    Args:
        window (HWND): The top-level app window for which the ISystemMediaTransportControls interface is retrieved.

    Returns:
        The ISystemMediaTransportControls that corresponds to the appWindow window.
    """
