# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winrt import _winrt_microsoft_ui_input_dragdrop

__all__ = [
    "DragDropModifiers",
    "DragUIContentMode",
    "DragDropManager",
    "DragInfo",
    "DragOperation",
    "DragUIOverride",
    "DropOperationTargetRequestedEventArgs",
    "IDropOperationTarget",
]

class DragDropModifiers(enum.IntFlag):
    NONE = 0
    SHIFT = 0x1
    CONTROL = 0x2
    ALT = 0x4
    LEFT_BUTTON = 0x8
    MIDDLE_BUTTON = 0x10
    RIGHT_BUTTON = 0x20

class DragUIContentMode(enum.IntEnum):
    AUTO = 0
    DEFERRED = 1

DragDropManager = _winrt_microsoft_ui_input_dragdrop.DragDropManager
DragInfo = _winrt_microsoft_ui_input_dragdrop.DragInfo
DragOperation = _winrt_microsoft_ui_input_dragdrop.DragOperation
DragUIOverride = _winrt_microsoft_ui_input_dragdrop.DragUIOverride
DropOperationTargetRequestedEventArgs = _winrt_microsoft_ui_input_dragdrop.DropOperationTargetRequestedEventArgs
IDropOperationTarget = _winrt_microsoft_ui_input_dragdrop.IDropOperationTarget