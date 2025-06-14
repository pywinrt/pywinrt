# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.runtime._internals
import winrt.system
from winrt._winrt_windows_storage_accesscache import (
    AccessListEntry,
    AccessListEntryView,
    ItemRemovedEventArgs,
    StorageApplicationPermissions,
    StorageItemAccessList,
    StorageItemMostRecentlyUsedList,
    _IStorageItemAccessList,
    IStorageItemAccessList,
)

__all__ = [
    "AccessCacheOptions",
    "RecentStorageItemVisibility",
    "AccessListEntry",
    "AccessListEntryView",
    "ItemRemovedEventArgs",
    "StorageApplicationPermissions",
    "StorageItemAccessList",
    "StorageItemMostRecentlyUsedList",
    "IStorageItemAccessList",
]

class AccessCacheOptions(enum.IntFlag):
    NONE = 0x0
    DISALLOW_USER_INPUT = 0x1
    FAST_LOCATIONS_ONLY = 0x2
    USE_READ_ONLY_CACHED_COPY = 0x4
    SUPPRESS_ACCESS_TIME_UPDATE = 0x8

class RecentStorageItemVisibility(enum.IntEnum):
    APP_ONLY = 0
    APP_AND_SYSTEM = 1

winrt.runtime._internals.mixin_sequence(AccessListEntryView)
