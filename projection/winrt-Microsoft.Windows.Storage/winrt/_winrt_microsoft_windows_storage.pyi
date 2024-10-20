# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.foundation as windows_foundation
import winrt.windows.foundation.collections as windows_foundation_collections
import winrt.windows.storage as windows_storage
import winrt.windows.system as windows_system

from winrt.microsoft.windows.storage import ApplicationDataCreateDisposition, ApplicationDataLocality

Self = typing.TypeVar('Self')

@typing.final
class ApplicationData_Static(type):
    def get_default(cls) -> typing.Optional[ApplicationData]: ...
    def get_for_package_family(cls, package_family_name: str, /) -> typing.Optional[ApplicationData]: ...
    def get_for_user(cls, user: typing.Optional[windows_system.User], /) -> typing.Optional[ApplicationData]: ...

@typing.final
class ApplicationData(winrt.system.Object, metaclass=ApplicationData_Static):
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, *args) -> None: ...
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ApplicationData: ...
    def clear_async(self, locality: ApplicationDataLocality, /) -> windows_foundation.IAsyncAction: ...
    def clear_publisher_cache_folder_async(self, folder_name: str, /) -> windows_foundation.IAsyncAction: ...
    def close(self) -> None: ...
    def get_publisher_cache_folder(self, folder_name: str, /) -> typing.Optional[windows_storage.StorageFolder]: ...
    def get_publisher_cache_path(self, folder_name: str, /) -> str: ...
    @_property
    def is_machine_path_supported(self) -> bool: ...
    @_property
    def local_cache_folder(self) -> typing.Optional[windows_storage.StorageFolder]: ...
    @_property
    def local_cache_path(self) -> str: ...
    @_property
    def local_folder(self) -> typing.Optional[windows_storage.StorageFolder]: ...
    @_property
    def local_path(self) -> str: ...
    @_property
    def local_settings(self) -> typing.Optional[ApplicationDataContainer]: ...
    @_property
    def machine_folder(self) -> typing.Optional[windows_storage.StorageFolder]: ...
    @_property
    def machine_path(self) -> str: ...
    @_property
    def shared_local_folder(self) -> typing.Optional[windows_storage.StorageFolder]: ...
    @_property
    def shared_local_path(self) -> str: ...
    @_property
    def temporary_folder(self) -> typing.Optional[windows_storage.StorageFolder]: ...
    @_property
    def temporary_path(self) -> str: ...

@typing.final
class ApplicationDataContainer(winrt.system.Object):
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, *args) -> None: ...
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ApplicationDataContainer: ...
    def close(self) -> None: ...
    def create_container(self, name: str, disposition: ApplicationDataCreateDisposition, /) -> typing.Optional[ApplicationDataContainer]: ...
    def delete_container(self, name: str, /) -> None: ...
    @_property
    def containers(self) -> typing.Optional[windows_foundation_collections.IMap[str, ApplicationDataContainer]]: ...
    @_property
    def locality(self) -> ApplicationDataLocality: ...
    @_property
    def name(self) -> str: ...
    @_property
    def values(self) -> typing.Optional[windows_foundation_collections.IPropertySet]: ...

