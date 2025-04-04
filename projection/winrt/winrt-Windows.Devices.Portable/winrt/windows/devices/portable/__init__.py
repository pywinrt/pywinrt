# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winrt._winrt_windows_devices_portable import (
    ServiceDevice,
    StorageDevice,
)

__all__ = [
    "ServiceDeviceType",
    "ServiceDevice",
    "StorageDevice",
]

class ServiceDeviceType(enum.IntEnum):
    CALENDAR_SERVICE = 0
    CONTACTS_SERVICE = 1
    DEVICE_STATUS_SERVICE = 2
    NOTES_SERVICE = 3
    RINGTONES_SERVICE = 4
    SMS_SERVICE = 5
    TASKS_SERVICE = 6

