# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum
import datetime
import sys
import types
import typing

import winrt.system
import winrt.windows.foundation

class CollectionChange(enum.IntEnum):
    RESET = 0
    ITEM_INSERTED = 1
    ITEM_REMOVED = 2
    ITEM_CHANGED = 3

Self = typing.TypeVar('Self')
K = typing.TypeVar('K')
T = typing.TypeVar('T')
V = typing.TypeVar('V')

class PropertySet(winrt.system.Object, typing.MutableMapping[str, winrt.system.Object]):
    size: winrt.system.UInt32
    def __len__(self) -> int: ...
    def __iter__(self) -> typing.Iterator[str]: ...
    def __contains__(self, key: object) -> bool:...
    def __setitem__(self, key: str, value: winrt.system.Object) -> None: ...
    def __getitem__(self, key: str) -> winrt.system.Object: ...
    def __delitem__(self, key: str) -> None: ...
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> PropertySet: ...
    def __new__(cls: typing.Type[PropertySet]) -> PropertySet:...
    def clear(self) -> None: ...
    def first(self) -> typing.Optional[IIterator[IKeyValuePair[str, winrt.system.Object]]]: ...
    def get_view(self) -> typing.Optional[IMapView[str, winrt.system.Object]]: ...
    def has_key(self, key: str, /) -> winrt.system.Boolean: ...
    def insert(self, key: str, value: typing.Optional[winrt.system.Object], /) -> winrt.system.Boolean: ...
    def lookup(self, key: str, /) -> typing.Optional[winrt.system.Object]: ...
    def remove(self, key: str, /) -> None: ...
    def add_map_changed(self, vhnd: MapChangedEventHandler[str, winrt.system.Object], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_map_changed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class StringMap(winrt.system.Object, typing.MutableMapping[str, str]):
    size: winrt.system.UInt32
    def __len__(self) -> int: ...
    def __iter__(self) -> typing.Iterator[str]: ...
    def __contains__(self, key: object) -> bool:...
    def __setitem__(self, key: str, value: str) -> None: ...
    def __getitem__(self, key: str) -> str: ...
    def __delitem__(self, key: str) -> None: ...
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> StringMap: ...
    def __new__(cls: typing.Type[StringMap]) -> StringMap:...
    def clear(self) -> None: ...
    def first(self) -> typing.Optional[IIterator[IKeyValuePair[str, str]]]: ...
    def get_view(self) -> typing.Optional[IMapView[str, str]]: ...
    def has_key(self, key: str, /) -> winrt.system.Boolean: ...
    def insert(self, key: str, value: str, /) -> winrt.system.Boolean: ...
    def lookup(self, key: str, /) -> str: ...
    def remove(self, key: str, /) -> None: ...
    def add_map_changed(self, vhnd: MapChangedEventHandler[str, str], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_map_changed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class ValueSet(winrt.system.Object, typing.MutableMapping[str, winrt.system.Object]):
    size: winrt.system.UInt32
    def __len__(self) -> int: ...
    def __iter__(self) -> typing.Iterator[str]: ...
    def __contains__(self, key: object) -> bool:...
    def __setitem__(self, key: str, value: winrt.system.Object) -> None: ...
    def __getitem__(self, key: str) -> winrt.system.Object: ...
    def __delitem__(self, key: str) -> None: ...
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ValueSet: ...
    def __new__(cls: typing.Type[ValueSet]) -> ValueSet:...
    def clear(self) -> None: ...
    def first(self) -> typing.Optional[IIterator[IKeyValuePair[str, winrt.system.Object]]]: ...
    def get_view(self) -> typing.Optional[IMapView[str, winrt.system.Object]]: ...
    def has_key(self, key: str, /) -> winrt.system.Boolean: ...
    def insert(self, key: str, value: typing.Optional[winrt.system.Object], /) -> winrt.system.Boolean: ...
    def lookup(self, key: str, /) -> typing.Optional[winrt.system.Object]: ...
    def remove(self, key: str, /) -> None: ...
    def add_map_changed(self, vhnd: MapChangedEventHandler[str, winrt.system.Object], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_map_changed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class IIterable(winrt.system.Object, typing.Generic[T]):
    if sys.version_info >= (3, 9):
        def __class_getitem__(cls, key: typing.Any) -> types.GenericAlias: ...
    def __iter__(self) -> IIterator[T]: ...
    def first(self) -> typing.Optional[IIterator[T]]: ...

class IIterator(winrt.system.Object, typing.Generic[T]):
    current: typing.Optional[T]
    has_current: winrt.system.Boolean
    if sys.version_info >= (3, 9):
        def __class_getitem__(cls, key: typing.Any) -> types.GenericAlias: ...
    def __iter__(self: Self) -> Self: ...
    def __next__(self) -> T: ...
    def get_many(self, items: winrt.system.Array[T], /) -> winrt.system.UInt32: ...
    def move_next(self) -> winrt.system.Boolean: ...

class IKeyValuePair(winrt.system.Object, typing.Generic[K, V]):
    key: typing.Optional[K]
    value: typing.Optional[V]
    if sys.version_info >= (3, 9):
        def __class_getitem__(cls, key: typing.Any) -> types.GenericAlias: ...

class IMapChangedEventArgs(winrt.system.Object, typing.Generic[K]):
    collection_change: CollectionChange
    key: typing.Optional[K]
    if sys.version_info >= (3, 9):
        def __class_getitem__(cls, key: typing.Any) -> types.GenericAlias: ...

class IMapView(winrt.system.Object, typing.Mapping[K, V], typing.Generic[K, V]):
    size: winrt.system.UInt32
    if sys.version_info >= (3, 9):
        def __class_getitem__(cls, key: typing.Any) -> types.GenericAlias: ...
    def __len__(self) -> int: ...
    def __iter__(self) -> typing.Iterator[K]: ...
    def __contains__(self, key: object) -> bool:...
    def __getitem__(self, key: K) -> V: ...
    def first(self) -> typing.Optional[IIterator[IKeyValuePair[K, V]]]: ...
    def has_key(self, key: typing.Optional[K], /) -> winrt.system.Boolean: ...
    def lookup(self, key: typing.Optional[K], /) -> typing.Optional[V]: ...
    def split(self) -> typing.Tuple[typing.Optional[IMapView[K, V]], typing.Optional[IMapView[K, V]]]: ...

class IMap(winrt.system.Object, typing.MutableMapping[K, V], typing.Generic[K, V]):
    size: winrt.system.UInt32
    if sys.version_info >= (3, 9):
        def __class_getitem__(cls, key: typing.Any) -> types.GenericAlias: ...
    def __len__(self) -> int: ...
    def __iter__(self) -> typing.Iterator[K]: ...
    def __contains__(self, key: object) -> bool:...
    def __setitem__(self, key: K, value: V) -> None: ...
    def __getitem__(self, key: K) -> V: ...
    def __delitem__(self, key: K) -> None: ...
    def clear(self) -> None: ...
    def first(self) -> typing.Optional[IIterator[IKeyValuePair[K, V]]]: ...
    def get_view(self) -> typing.Optional[IMapView[K, V]]: ...
    def has_key(self, key: typing.Optional[K], /) -> winrt.system.Boolean: ...
    def insert(self, key: typing.Optional[K], value: typing.Optional[V], /) -> winrt.system.Boolean: ...
    def lookup(self, key: typing.Optional[K], /) -> typing.Optional[V]: ...
    def remove(self, key: typing.Optional[K], /) -> None: ...

class IObservableMap(winrt.system.Object, typing.MutableMapping[K, V], typing.Generic[K, V]):
    size: winrt.system.UInt32
    if sys.version_info >= (3, 9):
        def __class_getitem__(cls, key: typing.Any) -> types.GenericAlias: ...
    def __len__(self) -> int: ...
    def __iter__(self) -> typing.Iterator[K]: ...
    def __contains__(self, key: object) -> bool:...
    def __setitem__(self, key: K, value: V) -> None: ...
    def __getitem__(self, key: K) -> V: ...
    def __delitem__(self, key: K) -> None: ...
    def clear(self) -> None: ...
    def first(self) -> typing.Optional[IIterator[IKeyValuePair[K, V]]]: ...
    def get_view(self) -> typing.Optional[IMapView[K, V]]: ...
    def has_key(self, key: typing.Optional[K], /) -> winrt.system.Boolean: ...
    def insert(self, key: typing.Optional[K], value: typing.Optional[V], /) -> winrt.system.Boolean: ...
    def lookup(self, key: typing.Optional[K], /) -> typing.Optional[V]: ...
    def remove(self, key: typing.Optional[K], /) -> None: ...
    def add_map_changed(self, vhnd: MapChangedEventHandler[K, V], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_map_changed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class IObservableVector(winrt.system.Object, typing.MutableSequence[T], typing.Generic[T]):
    size: winrt.system.UInt32
    if sys.version_info >= (3, 9):
        def __class_getitem__(cls, key: typing.Any) -> types.GenericAlias: ...
    def __len__(self) -> int: ...
    @typing.overload
    def __getitem__(self, index: int) -> T: ...
    @typing.overload
    def __getitem__(self, index: slice) -> winrt.system.Array[T]: ...
    def insert(self, index: int, value: T) -> None: ...
    @typing.overload
    def __delitem__(self, index: int) -> None: ...
    @typing.overload
    def __delitem__(self, index: slice) -> None: ...
    @typing.overload
    def __setitem__(self, index: int, value: T) -> None: ...
    @typing.overload
    def __setitem__(self, index: slice, value: typing.Iterable[T]) -> None: ...
    def append(self, value: typing.Optional[T], /) -> None: ...
    def clear(self) -> None: ...
    def first(self) -> typing.Optional[IIterator[T]]: ...
    def get_at(self, index: winrt.system.UInt32, /) -> typing.Optional[T]: ...
    def get_many(self, start_index: winrt.system.UInt32, items: winrt.system.Array[T], /) -> winrt.system.UInt32: ...
    def get_view(self) -> typing.Optional[IVectorView[T]]: ...
    def index_of(self, value: typing.Optional[T], /) -> typing.Tuple[winrt.system.Boolean, winrt.system.UInt32]: ...
    def insert_at(self, index: winrt.system.UInt32, value: typing.Optional[T], /) -> None: ...
    def remove_at(self, index: winrt.system.UInt32, /) -> None: ...
    def remove_at_end(self) -> None: ...
    def replace_all(self, items: winrt.system.Array[T], /) -> None: ...
    def set_at(self, index: winrt.system.UInt32, value: typing.Optional[T], /) -> None: ...
    def add_vector_changed(self, vhnd: VectorChangedEventHandler[T], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_vector_changed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class IPropertySet(winrt.system.Object, typing.MutableMapping[str, winrt.system.Object]):
    size: winrt.system.UInt32
    def __len__(self) -> int: ...
    def __iter__(self) -> typing.Iterator[str]: ...
    def __contains__(self, key: object) -> bool:...
    def __setitem__(self, key: str, value: winrt.system.Object) -> None: ...
    def __getitem__(self, key: str) -> winrt.system.Object: ...
    def __delitem__(self, key: str) -> None: ...
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> IPropertySet: ...
    def clear(self) -> None: ...
    def first(self) -> typing.Optional[IIterator[IKeyValuePair[str, winrt.system.Object]]]: ...
    def get_view(self) -> typing.Optional[IMapView[str, winrt.system.Object]]: ...
    def has_key(self, key: typing.Optional[str], /) -> winrt.system.Boolean: ...
    def insert(self, key: typing.Optional[str], value: typing.Optional[winrt.system.Object], /) -> winrt.system.Boolean: ...
    def lookup(self, key: typing.Optional[str], /) -> typing.Optional[winrt.system.Object]: ...
    def remove(self, key: typing.Optional[str], /) -> None: ...
    def add_map_changed(self, vhnd: MapChangedEventHandler[str, winrt.system.Object], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_map_changed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class IVectorChangedEventArgs(winrt.system.Object):
    collection_change: CollectionChange
    index: winrt.system.UInt32
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> IVectorChangedEventArgs: ...

class IVectorView(winrt.system.Object, typing.Sequence[T], typing.Generic[T]):
    size: winrt.system.UInt32
    if sys.version_info >= (3, 9):
        def __class_getitem__(cls, key: typing.Any) -> types.GenericAlias: ...
    def __len__(self) -> int: ...
    @typing.overload
    def __getitem__(self, index: int) -> T: ...
    @typing.overload
    def __getitem__(self, index: slice) -> winrt.system.Array[T]: ...
    def first(self) -> typing.Optional[IIterator[T]]: ...
    def get_at(self, index: winrt.system.UInt32, /) -> typing.Optional[T]: ...
    def get_many(self, start_index: winrt.system.UInt32, items: winrt.system.Array[T], /) -> winrt.system.UInt32: ...
    def index_of(self, value: typing.Optional[T], /) -> typing.Tuple[winrt.system.Boolean, winrt.system.UInt32]: ...

class IVector(winrt.system.Object, typing.MutableSequence[T], typing.Generic[T]):
    size: winrt.system.UInt32
    if sys.version_info >= (3, 9):
        def __class_getitem__(cls, key: typing.Any) -> types.GenericAlias: ...
    def __len__(self) -> int: ...
    @typing.overload
    def __getitem__(self, index: int) -> T: ...
    @typing.overload
    def __getitem__(self, index: slice) -> winrt.system.Array[T]: ...
    def insert(self, index: int, value: T) -> None: ...
    @typing.overload
    def __delitem__(self, index: int) -> None: ...
    @typing.overload
    def __delitem__(self, index: slice) -> None: ...
    @typing.overload
    def __setitem__(self, index: int, value: T) -> None: ...
    @typing.overload
    def __setitem__(self, index: slice, value: typing.Iterable[T]) -> None: ...
    def append(self, value: typing.Optional[T], /) -> None: ...
    def clear(self) -> None: ...
    def first(self) -> typing.Optional[IIterator[T]]: ...
    def get_at(self, index: winrt.system.UInt32, /) -> typing.Optional[T]: ...
    def get_many(self, start_index: winrt.system.UInt32, items: winrt.system.Array[T], /) -> winrt.system.UInt32: ...
    def get_view(self) -> typing.Optional[IVectorView[T]]: ...
    def index_of(self, value: typing.Optional[T], /) -> typing.Tuple[winrt.system.Boolean, winrt.system.UInt32]: ...
    def insert_at(self, index: winrt.system.UInt32, value: typing.Optional[T], /) -> None: ...
    def remove_at(self, index: winrt.system.UInt32, /) -> None: ...
    def remove_at_end(self) -> None: ...
    def replace_all(self, items: winrt.system.Array[T], /) -> None: ...
    def set_at(self, index: winrt.system.UInt32, value: typing.Optional[T], /) -> None: ...

MapChangedEventHandler = typing.Callable[[IObservableMap[K, V], IMapChangedEventArgs[K]], None]

VectorChangedEventHandler = typing.Callable[[IObservableVector[T], typing.Optional[IVectorChangedEventArgs]], None]
