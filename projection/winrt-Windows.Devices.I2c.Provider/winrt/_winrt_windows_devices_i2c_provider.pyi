# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.foundation
import winrt.windows.foundation.collections

from winrt.windows.devices.i2c.provider import ProviderI2cBusSpeed, ProviderI2cSharingMode, ProviderI2cTransferStatus

Self = typing.TypeVar('Self')

class ProviderI2cTransferResult:
    status: ProviderI2cTransferStatus
    bytes_transferred: winrt.system.UInt32
    def __init__(self, status: ProviderI2cTransferStatus, bytes_transferred: winrt.system.UInt32) -> None: ...

class ProviderI2cConnectionSettings(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ProviderI2cConnectionSettings: ...
    @_property
    def slave_address(self) -> winrt.system.Int32: ...
    @slave_address.setter
    def slave_address(self, value: winrt.system.Int32) -> None: ...
    @_property
    def sharing_mode(self) -> ProviderI2cSharingMode: ...
    @sharing_mode.setter
    def sharing_mode(self, value: ProviderI2cSharingMode) -> None: ...
    @_property
    def bus_speed(self) -> ProviderI2cBusSpeed: ...
    @bus_speed.setter
    def bus_speed(self, value: ProviderI2cBusSpeed) -> None: ...

class II2cControllerProvider(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> II2cControllerProvider: ...
    def get_device_provider(self, settings: typing.Optional[ProviderI2cConnectionSettings], /) -> typing.Optional[II2cDeviceProvider]: ...

class II2cDeviceProvider(winrt.system.Object):
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, *args) -> None: ...
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> II2cDeviceProvider: ...
    def close(self) -> None: ...
    def read(self, buffer: winrt.system.Array[winrt.system.UInt8], /) -> None: ...
    def read_partial(self, buffer: winrt.system.Array[winrt.system.UInt8], /) -> ProviderI2cTransferResult: ...
    def write(self, buffer: winrt.system.Array[winrt.system.UInt8], /) -> None: ...
    def write_partial(self, buffer: winrt.system.Array[winrt.system.UInt8], /) -> ProviderI2cTransferResult: ...
    def write_read(self, write_buffer: winrt.system.Array[winrt.system.UInt8], read_buffer: winrt.system.Array[winrt.system.UInt8], /) -> None: ...
    def write_read_partial(self, write_buffer: winrt.system.Array[winrt.system.UInt8], read_buffer: winrt.system.Array[winrt.system.UInt8], /) -> ProviderI2cTransferResult: ...
    @_property
    def device_id(self) -> str: ...

class II2cProvider(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> II2cProvider: ...
    def get_controllers_async(self) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.foundation.collections.IVectorView[II2cControllerProvider]]: ...
