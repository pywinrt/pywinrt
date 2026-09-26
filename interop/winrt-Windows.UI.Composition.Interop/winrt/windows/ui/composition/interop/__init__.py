from uuid import UUID

import winrt._winrt as _runtime
import winrt._winrt_windows_ui_composition_interop as _native
from winrt.windows.ui.composition import Compositor
from winrt.windows.ui.composition.desktop import DesktopWindowTarget

__all__ = ["create_desktop_window_target"]

_IID_IINSPECTABLE = UUID("AF86E2E0-B12D-4C6A-9C5A-D7AA65101E90")


def create_desktop_window_target(
    compositor: Compositor, hwnd_target: int, *, is_topmost: bool = False
) -> DesktopWindowTarget:
    return _runtime.wrap_interface(
        _native.create_desktop_window_target(
            _runtime.as_interface(compositor, _IID_IINSPECTABLE),
            hwnd_target,
            is_topmost,
        ),
        "winrt.windows.ui.composition.desktop.DesktopWindowTarget",
    )
