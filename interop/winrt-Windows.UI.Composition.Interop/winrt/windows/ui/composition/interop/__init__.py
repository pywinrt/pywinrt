import winrt.runtime.interop as _runtime
import winrt._winrt_windows_ui_composition_interop as _native
from winrt.windows.ui.composition import Compositor
from winrt.windows.ui.composition.desktop import DesktopWindowTarget

__all__ = ["create_desktop_window_target"]


def create_desktop_window_target(
    compositor: Compositor, hwnd_target: int, *, is_topmost: bool = False
) -> DesktopWindowTarget:
    return _runtime.wrap_interface(
        _native.create_desktop_window_target(
            _runtime.as_interface(compositor, Compositor),
            hwnd_target,
            is_topmost,
        ),
        DesktopWindowTarget,
    )
