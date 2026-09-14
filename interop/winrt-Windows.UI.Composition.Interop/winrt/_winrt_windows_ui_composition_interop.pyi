from winrt.windows.ui.composition import Compositor
from winrt.windows.ui.composition.desktop import DesktopWindowTarget

def create_desktop_window_target(
    compositor: Compositor, hwnd_target: int, *, is_topmost: bool = False
) -> DesktopWindowTarget: ...
