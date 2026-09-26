from typing_extensions import CapsuleType

def create_desktop_window_target(
    compositor: CapsuleType, hwnd_target: int, is_topmost: bool, /
) -> CapsuleType: ...
