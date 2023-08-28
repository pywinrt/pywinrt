from .windows.graphics.capture import GraphicsCaptureItem

def create_for_monitor(monitor: int) -> GraphicsCaptureItem:
    """
    Targets a monitor(s) for the creation of a graphics capture item.

    Args:
        monitor (HMONITOR): The monitor handle that represents the monitor to capture.

    Returns:
        A graphics capture item.

    Requirements:
        Windows 10 1903 (build 18334)
    """

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
