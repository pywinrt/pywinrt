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

from winrt.windows.applicationmodel.resources.management import IndexedResourceType

Self = typing.TypeVar('Self')

@typing.final
class IndexedResourceCandidate(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> IndexedResourceCandidate: ...
    def get_qualifier_value(self, qualifier_name: str, /) -> str: ...
    @_property
    def metadata(self) -> typing.Optional[winrt.windows.foundation.collections.IMapView[str, str]]: ...
    @_property
    def qualifiers(self) -> typing.Optional[winrt.windows.foundation.collections.IVectorView[IndexedResourceQualifier]]: ...
    @_property
    def type(self) -> IndexedResourceType: ...
    @_property
    def uri(self) -> typing.Optional[winrt.windows.foundation.Uri]: ...
    @_property
    def value_as_string(self) -> str: ...

@typing.final
class IndexedResourceQualifier(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> IndexedResourceQualifier: ...
    @_property
    def qualifier_name(self) -> str: ...
    @_property
    def qualifier_value(self) -> str: ...

@typing.final
class ResourceIndexer(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ResourceIndexer: ...
    @typing.overload
    def __new__(cls: typing.Type[ResourceIndexer], project_root: typing.Optional[winrt.windows.foundation.Uri], extension_dll_path: typing.Optional[winrt.windows.foundation.Uri]) -> ResourceIndexer:...
    @typing.overload
    def __new__(cls: typing.Type[ResourceIndexer], project_root: typing.Optional[winrt.windows.foundation.Uri]) -> ResourceIndexer:...
    def index_file_contents_async(self, file: typing.Optional[winrt.windows.foundation.Uri], /) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.foundation.collections.IVectorView[IndexedResourceCandidate]]: ...
    def index_file_path(self, file_path: typing.Optional[winrt.windows.foundation.Uri], /) -> typing.Optional[IndexedResourceCandidate]: ...

