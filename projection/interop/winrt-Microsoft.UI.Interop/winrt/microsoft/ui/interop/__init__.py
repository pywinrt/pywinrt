from .... import _winrt_microsoft_ui_interop as interop

__all__ = [
    "get_window_id_from_window",
    "get_window_from_window_id",
    "get_display_id_from_monitor",
    "get_monitor_from_display_id",
    "get_icon_id_from_icon",
    "get_icon_from_icon_id",
]


get_window_id_from_window = interop.get_window_id_from_window
get_window_from_window_id = interop.get_window_from_window_id
get_display_id_from_monitor = interop.get_display_id_from_monitor
get_monitor_from_display_id = interop.get_monitor_from_display_id
get_icon_id_from_icon = interop.get_icon_id_from_icon
get_icon_from_icon_id = interop.get_icon_from_icon_id
