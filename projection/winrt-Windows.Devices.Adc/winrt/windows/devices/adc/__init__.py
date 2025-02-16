# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winrt._winrt_windows_devices_adc import (
    AdcChannel,
    AdcController,
)

__all__ = [
    "AdcChannelMode",
    "AdcChannel",
    "AdcController",
]

class AdcChannelMode(enum.IntEnum):
    SINGLE_ENDED = 0
    DIFFERENTIAL = 1

