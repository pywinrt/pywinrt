# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winrt._winrt_windows_system_preview import (
    TwoPanelHingedDevicePosturePreview,
    TwoPanelHingedDevicePosturePreviewReading,
    TwoPanelHingedDevicePosturePreviewReadingChangedEventArgs,
)

__all__ = [
    "HingeState",
    "TwoPanelHingedDevicePosturePreview",
    "TwoPanelHingedDevicePosturePreviewReading",
    "TwoPanelHingedDevicePosturePreviewReadingChangedEventArgs",
]

class HingeState(enum.IntEnum):
    UNKNOWN = 0
    CLOSED = 1
    CONCAVE = 2
    FLAT = 3
    CONVEX = 4
    FULL = 5

