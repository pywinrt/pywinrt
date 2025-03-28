# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winrt._winrt_windows_devices_input_preview import (
    GazeDevicePreview,
    GazeDeviceWatcherAddedPreviewEventArgs,
    GazeDeviceWatcherPreview,
    GazeDeviceWatcherRemovedPreviewEventArgs,
    GazeDeviceWatcherUpdatedPreviewEventArgs,
    GazeEnteredPreviewEventArgs,
    GazeExitedPreviewEventArgs,
    GazeInputSourcePreview,
    GazeMovedPreviewEventArgs,
    GazePointPreview,
)

__all__ = [
    "GazeDeviceConfigurationStatePreview",
    "GazeDevicePreview",
    "GazeDeviceWatcherAddedPreviewEventArgs",
    "GazeDeviceWatcherPreview",
    "GazeDeviceWatcherRemovedPreviewEventArgs",
    "GazeDeviceWatcherUpdatedPreviewEventArgs",
    "GazeEnteredPreviewEventArgs",
    "GazeExitedPreviewEventArgs",
    "GazeInputSourcePreview",
    "GazeMovedPreviewEventArgs",
    "GazePointPreview",
]

class GazeDeviceConfigurationStatePreview(enum.IntEnum):
    UNKNOWN = 0
    READY = 1
    CONFIGURING = 2
    SCREEN_SETUP_NEEDED = 3
    USER_CALIBRATION_NEEDED = 4

