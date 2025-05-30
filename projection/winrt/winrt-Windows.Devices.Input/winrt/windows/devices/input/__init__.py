# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winrt._winrt_windows_devices_input import (
    MouseDelta,
    PointerDeviceUsage,
    KeyboardCapabilities,
    MouseCapabilities,
    MouseDevice,
    MouseEventArgs,
    PenButtonListener,
    PenDevice,
    PenDockListener,
    PenDockedEventArgs,
    PenTailButtonClickedEventArgs,
    PenTailButtonDoubleClickedEventArgs,
    PenTailButtonLongPressedEventArgs,
    PenUndockedEventArgs,
    PointerDevice,
    TouchCapabilities,
)

__all__ = [
    "PointerDeviceType",
    "MouseDelta",
    "PointerDeviceUsage",
    "KeyboardCapabilities",
    "MouseCapabilities",
    "MouseDevice",
    "MouseEventArgs",
    "PenButtonListener",
    "PenDevice",
    "PenDockListener",
    "PenDockedEventArgs",
    "PenTailButtonClickedEventArgs",
    "PenTailButtonDoubleClickedEventArgs",
    "PenTailButtonLongPressedEventArgs",
    "PenUndockedEventArgs",
    "PointerDevice",
    "TouchCapabilities",
]

class PointerDeviceType(enum.IntEnum):
    TOUCH = 0
    PEN = 1
    MOUSE = 2
    TOUCHPAD = 3

