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
import winrt.windows.networking
import winrt.windows.networking.sockets
import winrt.windows.storage.streams

from winrt.windows.networking.proximity import PeerDiscoveryTypes, PeerRole, PeerWatcherStatus, TriggeredConnectState
from winrt.windows.networking.proximity import DeviceArrivedEventHandler, DeviceDepartedEventHandler, MessageReceivedHandler, MessageTransmittedHandler

Self = typing.TypeVar('Self')

class ConnectionRequestedEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ConnectionRequestedEventArgs: ...
    @_property
    def peer_information(self) -> typing.Optional[PeerInformation]: ...

class PeerFinder_Static(type):
    def connect_async(cls, peer_information: typing.Optional[PeerInformation], /) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.networking.sockets.StreamSocket]: ...
    def create_watcher(cls) -> typing.Optional[PeerWatcher]: ...
    def find_all_peers_async(cls) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.foundation.collections.IVectorView[PeerInformation]]: ...
    @typing.overload
    def start(cls) -> None: ...
    @typing.overload
    def start(cls, peer_message: str, /) -> None: ...
    def stop(cls) -> None: ...
    def add_connection_requested(cls, handler: winrt.windows.foundation.TypedEventHandler[winrt.system.Object, ConnectionRequestedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_connection_requested(cls, cookie: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_triggered_connection_state_changed(cls, handler: winrt.windows.foundation.TypedEventHandler[winrt.system.Object, TriggeredConnectionStateChangedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_triggered_connection_state_changed(cls, cookie: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def display_name(cls) -> str: ...
    @display_name.setter
    def display_name(cls, value: str) -> None: ...
    @_property
    def allow_wi_fi_direct(cls) -> bool: ...
    @allow_wi_fi_direct.setter
    def allow_wi_fi_direct(cls, value: bool) -> None: ...
    @_property
    def allow_infrastructure(cls) -> bool: ...
    @allow_infrastructure.setter
    def allow_infrastructure(cls, value: bool) -> None: ...
    @_property
    def allow_bluetooth(cls) -> bool: ...
    @allow_bluetooth.setter
    def allow_bluetooth(cls, value: bool) -> None: ...
    @_property
    def alternate_identities(cls) -> typing.Optional[winrt.windows.foundation.collections.IMap[str, str]]: ...
    @_property
    def supported_discovery_types(cls) -> PeerDiscoveryTypes: ...
    @_property
    def role(cls) -> PeerRole: ...
    @role.setter
    def role(cls, value: PeerRole) -> None: ...
    @_property
    def discovery_data(cls) -> typing.Optional[winrt.windows.storage.streams.IBuffer]: ...
    @discovery_data.setter
    def discovery_data(cls, value: typing.Optional[winrt.windows.storage.streams.IBuffer]) -> None: ...

class PeerFinder(winrt.system.Object, metaclass=PeerFinder_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> PeerFinder: ...

class PeerInformation(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> PeerInformation: ...
    @_property
    def display_name(self) -> str: ...
    @_property
    def discovery_data(self) -> typing.Optional[winrt.windows.storage.streams.IBuffer]: ...
    @_property
    def id(self) -> str: ...
    @_property
    def host_name(self) -> typing.Optional[winrt.windows.networking.HostName]: ...
    @_property
    def service_name(self) -> str: ...

class PeerWatcher(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> PeerWatcher: ...
    def start(self) -> None: ...
    def stop(self) -> None: ...
    def add_added(self, handler: winrt.windows.foundation.TypedEventHandler[PeerWatcher, PeerInformation], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_added(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_enumeration_completed(self, handler: winrt.windows.foundation.TypedEventHandler[PeerWatcher, winrt.system.Object], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_enumeration_completed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_removed(self, handler: winrt.windows.foundation.TypedEventHandler[PeerWatcher, PeerInformation], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_removed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_stopped(self, handler: winrt.windows.foundation.TypedEventHandler[PeerWatcher, winrt.system.Object], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_stopped(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_updated(self, handler: winrt.windows.foundation.TypedEventHandler[PeerWatcher, PeerInformation], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_updated(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def status(self) -> PeerWatcherStatus: ...

class ProximityDevice_Static(type):
    def from_id(cls, device_id: str, /) -> typing.Optional[ProximityDevice]: ...
    def get_default(cls) -> typing.Optional[ProximityDevice]: ...
    def get_device_selector(cls) -> str: ...

class ProximityDevice(winrt.system.Object, metaclass=ProximityDevice_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ProximityDevice: ...
    @typing.overload
    def publish_binary_message(self, message_type: str, message: typing.Optional[winrt.windows.storage.streams.IBuffer], /) -> winrt.system.Int64: ...
    @typing.overload
    def publish_binary_message(self, message_type: str, message: typing.Optional[winrt.windows.storage.streams.IBuffer], message_transmitted_handler: typing.Optional[MessageTransmittedHandler], /) -> winrt.system.Int64: ...
    @typing.overload
    def publish_message(self, message_type: str, message: str, /) -> winrt.system.Int64: ...
    @typing.overload
    def publish_message(self, message_type: str, message: str, message_transmitted_handler: typing.Optional[MessageTransmittedHandler], /) -> winrt.system.Int64: ...
    @typing.overload
    def publish_uri_message(self, message: typing.Optional[winrt.windows.foundation.Uri], /) -> winrt.system.Int64: ...
    @typing.overload
    def publish_uri_message(self, message: typing.Optional[winrt.windows.foundation.Uri], message_transmitted_handler: typing.Optional[MessageTransmittedHandler], /) -> winrt.system.Int64: ...
    def stop_publishing_message(self, message_id: winrt.system.Int64, /) -> None: ...
    def stop_subscribing_for_message(self, subscription_id: winrt.system.Int64, /) -> None: ...
    def subscribe_for_message(self, message_type: str, message_received_handler: typing.Optional[MessageReceivedHandler], /) -> winrt.system.Int64: ...
    def add_device_arrived(self, arrived_handler: typing.Optional[DeviceArrivedEventHandler], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_device_arrived(self, cookie: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_device_departed(self, departed_handler: typing.Optional[DeviceDepartedEventHandler], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_device_departed(self, cookie: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def bits_per_second(self) -> winrt.system.UInt64: ...
    @_property
    def device_id(self) -> str: ...
    @_property
    def max_message_bytes(self) -> winrt.system.UInt32: ...

class ProximityMessage(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ProximityMessage: ...
    @_property
    def data(self) -> typing.Optional[winrt.windows.storage.streams.IBuffer]: ...
    @_property
    def data_as_string(self) -> str: ...
    @_property
    def message_type(self) -> str: ...
    @_property
    def subscription_id(self) -> winrt.system.Int64: ...

class TriggeredConnectionStateChangedEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> TriggeredConnectionStateChangedEventArgs: ...
    @_property
    def id(self) -> winrt.system.UInt32: ...
    @_property
    def socket(self) -> typing.Optional[winrt.windows.networking.sockets.StreamSocket]: ...
    @_property
    def state(self) -> TriggeredConnectState: ...
