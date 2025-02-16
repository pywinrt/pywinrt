# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winrt._winrt_windows_devices_spi import (
    SpiBusInfo,
    SpiConnectionSettings,
    SpiController,
    SpiDevice,
    _ISpiDeviceStatics,
    ISpiDeviceStatics,
)

__all__ = [
    "SpiMode",
    "SpiSharingMode",
    "SpiBusInfo",
    "SpiConnectionSettings",
    "SpiController",
    "SpiDevice",
    "ISpiDeviceStatics",
]

class SpiMode(enum.IntEnum):
    MODE0 = 0
    MODE1 = 1
    MODE2 = 2
    MODE3 = 3

class SpiSharingMode(enum.IntEnum):
    EXCLUSIVE = 0
    SHARED = 1

