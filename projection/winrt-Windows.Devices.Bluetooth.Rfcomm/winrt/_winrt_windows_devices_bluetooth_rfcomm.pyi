# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.devices.bluetooth
import winrt.windows.devices.enumeration
import winrt.windows.foundation
import winrt.windows.foundation.collections
import winrt.windows.networking
import winrt.windows.networking.sockets
import winrt.windows.storage.streams

Self = typing.TypeVar('Self')

class RfcommDeviceService_Static(type):
    def from_id_async(cls, device_id: str, /) -> winrt.windows.foundation.IAsyncOperation[RfcommDeviceService]: ...
    def get_device_selector(cls, service_id: typing.Optional[RfcommServiceId], /) -> str: ...
    @typing.overload
    def get_device_selector_for_bluetooth_device(cls, bluetooth_device: typing.Optional[winrt.windows.devices.bluetooth.BluetoothDevice], /) -> str: ...
    @typing.overload
    def get_device_selector_for_bluetooth_device(cls, bluetooth_device: typing.Optional[winrt.windows.devices.bluetooth.BluetoothDevice], cache_mode: winrt.windows.devices.bluetooth.BluetoothCacheMode, /) -> str: ...
    @typing.overload
    def get_device_selector_for_bluetooth_device_and_service_id(cls, bluetooth_device: typing.Optional[winrt.windows.devices.bluetooth.BluetoothDevice], service_id: typing.Optional[RfcommServiceId], /) -> str: ...
    @typing.overload
    def get_device_selector_for_bluetooth_device_and_service_id(cls, bluetooth_device: typing.Optional[winrt.windows.devices.bluetooth.BluetoothDevice], service_id: typing.Optional[RfcommServiceId], cache_mode: winrt.windows.devices.bluetooth.BluetoothCacheMode, /) -> str: ...

class RfcommDeviceService(winrt.system.Object, metaclass=RfcommDeviceService_Static):
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, *args) -> None: ...
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> RfcommDeviceService: ...
    def close(self) -> None: ...
    @typing.overload
    def get_sdp_raw_attributes_async(self) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.foundation.collections.IMapView[winrt.system.UInt32, winrt.windows.storage.streams.IBuffer]]: ...
    @typing.overload
    def get_sdp_raw_attributes_async(self, cache_mode: winrt.windows.devices.bluetooth.BluetoothCacheMode, /) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.foundation.collections.IMapView[winrt.system.UInt32, winrt.windows.storage.streams.IBuffer]]: ...
    def request_access_async(self) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.devices.enumeration.DeviceAccessStatus]: ...
    @_property
    def connection_host_name(self) -> typing.Optional[winrt.windows.networking.HostName]: ...
    @_property
    def connection_service_name(self) -> str: ...
    @_property
    def max_protection_level(self) -> winrt.windows.networking.sockets.SocketProtectionLevel: ...
    @_property
    def protection_level(self) -> winrt.windows.networking.sockets.SocketProtectionLevel: ...
    @_property
    def service_id(self) -> typing.Optional[RfcommServiceId]: ...
    @_property
    def device(self) -> typing.Optional[winrt.windows.devices.bluetooth.BluetoothDevice]: ...
    @_property
    def device_access_information(self) -> typing.Optional[winrt.windows.devices.enumeration.DeviceAccessInformation]: ...

class RfcommDeviceServicesResult(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> RfcommDeviceServicesResult: ...
    @_property
    def error(self) -> winrt.windows.devices.bluetooth.BluetoothError: ...
    @_property
    def services(self) -> typing.Optional[winrt.windows.foundation.collections.IVectorView[RfcommDeviceService]]: ...

class RfcommServiceId_Static(type):
    def from_short_id(cls, short_id: winrt.system.UInt32, /) -> typing.Optional[RfcommServiceId]: ...
    def from_uuid(cls, uuid: _uuid.UUID, /) -> typing.Optional[RfcommServiceId]: ...
    @_property
    def generic_file_transfer(cls) -> typing.Optional[RfcommServiceId]: ...
    @_property
    def obex_file_transfer(cls) -> typing.Optional[RfcommServiceId]: ...
    @_property
    def obex_object_push(cls) -> typing.Optional[RfcommServiceId]: ...
    @_property
    def phone_book_access_pce(cls) -> typing.Optional[RfcommServiceId]: ...
    @_property
    def phone_book_access_pse(cls) -> typing.Optional[RfcommServiceId]: ...
    @_property
    def serial_port(cls) -> typing.Optional[RfcommServiceId]: ...

class RfcommServiceId(winrt.system.Object, metaclass=RfcommServiceId_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> RfcommServiceId: ...
    def as_short_id(self) -> winrt.system.UInt32: ...
    def as_string(self) -> str: ...
    @_property
    def uuid(self) -> _uuid.UUID: ...

class RfcommServiceProvider_Static(type):
    def create_async(cls, service_id: typing.Optional[RfcommServiceId], /) -> winrt.windows.foundation.IAsyncOperation[RfcommServiceProvider]: ...

class RfcommServiceProvider(winrt.system.Object, metaclass=RfcommServiceProvider_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> RfcommServiceProvider: ...
    @typing.overload
    def start_advertising(self, listener: typing.Optional[winrt.windows.networking.sockets.StreamSocketListener], /) -> None: ...
    @typing.overload
    def start_advertising(self, listener: typing.Optional[winrt.windows.networking.sockets.StreamSocketListener], radio_discoverable: bool, /) -> None: ...
    def stop_advertising(self) -> None: ...
    @_property
    def sdp_raw_attributes(self) -> typing.Optional[winrt.windows.foundation.collections.IMap[winrt.system.UInt32, winrt.windows.storage.streams.IBuffer]]: ...
    @_property
    def service_id(self) -> typing.Optional[RfcommServiceId]: ...
