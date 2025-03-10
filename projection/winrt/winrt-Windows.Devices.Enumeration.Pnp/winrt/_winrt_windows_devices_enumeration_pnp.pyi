# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.devices.enumeration as windows_devices_enumeration
import winrt.windows.foundation as windows_foundation
import winrt.windows.foundation.collections as windows_foundation_collections

from winrt.windows.devices.enumeration.pnp import PnpObjectType

Self = typing.TypeVar('Self')

@typing.final
class PnpObject_Static(winrt._winrt.IInspectable_Static):
    # Windows.Foundation.IAsyncOperation`1<Windows.Devices.Enumeration.Pnp.PnpObject> Windows.Devices.Enumeration.Pnp.PnpObject::CreateFromIdAsync(Windows.Devices.Enumeration.Pnp.PnpObjectType,System.String,Windows.Foundation.Collections.IIterable`1<System.String>)
    def create_from_id_async(cls, type: PnpObjectType, id: str, requested_properties: typing.Iterable[str], /) -> windows_foundation.IAsyncOperation[PnpObject]: ...
    # Windows.Devices.Enumeration.Pnp.PnpObjectWatcher Windows.Devices.Enumeration.Pnp.PnpObject::CreateWatcher(Windows.Devices.Enumeration.Pnp.PnpObjectType,Windows.Foundation.Collections.IIterable`1<System.String>)
    def create_watcher(cls, type: PnpObjectType, requested_properties: typing.Iterable[str], /) -> PnpObjectWatcher: ...
    # Windows.Devices.Enumeration.Pnp.PnpObjectWatcher Windows.Devices.Enumeration.Pnp.PnpObject::CreateWatcher(Windows.Devices.Enumeration.Pnp.PnpObjectType,Windows.Foundation.Collections.IIterable`1<System.String>,System.String)
    def create_watcher_aqs_filter(cls, type: PnpObjectType, requested_properties: typing.Iterable[str], aqs_filter: str, /) -> PnpObjectWatcher: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Devices.Enumeration.Pnp.PnpObjectCollection> Windows.Devices.Enumeration.Pnp.PnpObject::FindAllAsync(Windows.Devices.Enumeration.Pnp.PnpObjectType,Windows.Foundation.Collections.IIterable`1<System.String>)
    def find_all_async(cls, type: PnpObjectType, requested_properties: typing.Iterable[str], /) -> windows_foundation.IAsyncOperation[PnpObjectCollection]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Devices.Enumeration.Pnp.PnpObjectCollection> Windows.Devices.Enumeration.Pnp.PnpObject::FindAllAsync(Windows.Devices.Enumeration.Pnp.PnpObjectType,Windows.Foundation.Collections.IIterable`1<System.String>,System.String)
    def find_all_async_aqs_filter(cls, type: PnpObjectType, requested_properties: typing.Iterable[str], aqs_filter: str, /) -> windows_foundation.IAsyncOperation[PnpObjectCollection]: ...

@typing.final
class PnpObject(winrt.system.Object, metaclass=PnpObject_Static):
    # System.Void Windows.Devices.Enumeration.Pnp.PnpObject::Update(Windows.Devices.Enumeration.Pnp.PnpObjectUpdate)
    def update(self, update_info: PnpObjectUpdate, /) -> None: ...
    # System.String Windows.Devices.Enumeration.Pnp.PnpObject::get_Id()
    @_property
    def id(self) -> str: ...
    # Windows.Foundation.Collections.IMapView`2<System.String,System.Object> Windows.Devices.Enumeration.Pnp.PnpObject::get_Properties()
    @_property
    def properties(self) -> typing.Mapping[str, winrt.system.Object]: ...
    # Windows.Devices.Enumeration.Pnp.PnpObjectType Windows.Devices.Enumeration.Pnp.PnpObject::get_Type()
    @_property
    def type(self) -> PnpObjectType: ...

@typing.final
class PnpObjectCollection(winrt.system.Object, winrt._winrt.Sequence[PnpObject]):
    def __len__(self) -> int: ...
    def __iter__(self) -> typing.Iterator[PnpObject]: ...
    @typing.overload
    def __getitem__(self, index: typing.SupportsIndex) -> PnpObject: ...
    @typing.overload
    def __getitem__(self, index: slice) -> winrt.system.Array[PnpObject]: ...
    # Windows.Foundation.Collections.IIterator`1<Windows.Devices.Enumeration.Pnp.PnpObject> Windows.Devices.Enumeration.Pnp.PnpObjectCollection::First()
    def first(self) -> windows_foundation_collections.IIterator[PnpObject]: ...
    # Windows.Devices.Enumeration.Pnp.PnpObject Windows.Devices.Enumeration.Pnp.PnpObjectCollection::GetAt(System.UInt32)
    def get_at(self, index: winrt.system.UInt32, /) -> PnpObject: ...
    # System.UInt32 Windows.Devices.Enumeration.Pnp.PnpObjectCollection::GetMany(System.UInt32,Windows.Devices.Enumeration.Pnp.PnpObject[])
    def get_many(self, start_index: winrt.system.UInt32, items: typing.Union[winrt.system.Array[PnpObject], winrt.system.WriteableBuffer], /) -> winrt.system.UInt32: ...
    # System.Boolean Windows.Devices.Enumeration.Pnp.PnpObjectCollection::IndexOf(Windows.Devices.Enumeration.Pnp.PnpObject,System.UInt32&)
    def index_of(self, value: PnpObject, /) -> typing.Tuple[bool, winrt.system.UInt32]: ...
    # System.UInt32 Windows.Devices.Enumeration.Pnp.PnpObjectCollection::get_Size()
    @_property
    def size(self) -> winrt.system.UInt32: ...

@typing.final
class PnpObjectUpdate(winrt.system.Object):
    # System.String Windows.Devices.Enumeration.Pnp.PnpObjectUpdate::get_Id()
    @_property
    def id(self) -> str: ...
    # Windows.Foundation.Collections.IMapView`2<System.String,System.Object> Windows.Devices.Enumeration.Pnp.PnpObjectUpdate::get_Properties()
    @_property
    def properties(self) -> typing.Mapping[str, winrt.system.Object]: ...
    # Windows.Devices.Enumeration.Pnp.PnpObjectType Windows.Devices.Enumeration.Pnp.PnpObjectUpdate::get_Type()
    @_property
    def type(self) -> PnpObjectType: ...

@typing.final
class PnpObjectWatcher(winrt.system.Object):
    # System.Void Windows.Devices.Enumeration.Pnp.PnpObjectWatcher::Start()
    def start(self) -> None: ...
    # System.Void Windows.Devices.Enumeration.Pnp.PnpObjectWatcher::Stop()
    def stop(self) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.Devices.Enumeration.Pnp.PnpObjectWatcher::add_Added(Windows.Foundation.TypedEventHandler`2<Windows.Devices.Enumeration.Pnp.PnpObjectWatcher,Windows.Devices.Enumeration.Pnp.PnpObject>)
    def add_added(self, handler: windows_foundation.TypedEventHandler[PnpObjectWatcher, PnpObject], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.Devices.Enumeration.Pnp.PnpObjectWatcher::remove_Added(Windows.Foundation.EventRegistrationToken)
    def remove_added(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.Devices.Enumeration.Pnp.PnpObjectWatcher::add_EnumerationCompleted(Windows.Foundation.TypedEventHandler`2<Windows.Devices.Enumeration.Pnp.PnpObjectWatcher,System.Object>)
    def add_enumeration_completed(self, handler: windows_foundation.TypedEventHandler[PnpObjectWatcher, winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.Devices.Enumeration.Pnp.PnpObjectWatcher::remove_EnumerationCompleted(Windows.Foundation.EventRegistrationToken)
    def remove_enumeration_completed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.Devices.Enumeration.Pnp.PnpObjectWatcher::add_Removed(Windows.Foundation.TypedEventHandler`2<Windows.Devices.Enumeration.Pnp.PnpObjectWatcher,Windows.Devices.Enumeration.Pnp.PnpObjectUpdate>)
    def add_removed(self, handler: windows_foundation.TypedEventHandler[PnpObjectWatcher, PnpObjectUpdate], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.Devices.Enumeration.Pnp.PnpObjectWatcher::remove_Removed(Windows.Foundation.EventRegistrationToken)
    def remove_removed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.Devices.Enumeration.Pnp.PnpObjectWatcher::add_Stopped(Windows.Foundation.TypedEventHandler`2<Windows.Devices.Enumeration.Pnp.PnpObjectWatcher,System.Object>)
    def add_stopped(self, handler: windows_foundation.TypedEventHandler[PnpObjectWatcher, winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.Devices.Enumeration.Pnp.PnpObjectWatcher::remove_Stopped(Windows.Foundation.EventRegistrationToken)
    def remove_stopped(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.Devices.Enumeration.Pnp.PnpObjectWatcher::add_Updated(Windows.Foundation.TypedEventHandler`2<Windows.Devices.Enumeration.Pnp.PnpObjectWatcher,Windows.Devices.Enumeration.Pnp.PnpObjectUpdate>)
    def add_updated(self, handler: windows_foundation.TypedEventHandler[PnpObjectWatcher, PnpObjectUpdate], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.Devices.Enumeration.Pnp.PnpObjectWatcher::remove_Updated(Windows.Foundation.EventRegistrationToken)
    def remove_updated(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Devices.Enumeration.DeviceWatcherStatus Windows.Devices.Enumeration.Pnp.PnpObjectWatcher::get_Status()
    @_property
    def status(self) -> windows_devices_enumeration.DeviceWatcherStatus: ...

