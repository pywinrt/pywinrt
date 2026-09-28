from winrt.runtime.interop import InterfaceCapsule

def create_desktop_window_target(
    compositor: InterfaceCapsule, hwnd_target: int, is_topmost: bool, /
) -> InterfaceCapsule: ...
