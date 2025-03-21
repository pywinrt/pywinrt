// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.System.h>

#include <winrt/Windows.Storage.AccessCache.h>

namespace py::proj::Windows::Storage::AccessCache
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Storage::AccessCache::AccessCacheOptions> = "I";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Storage::AccessCache::RecentStorageItemVisibility> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Storage::AccessCache::AccessListEntry> = "T{P:token:P:metadata:}";


    template<>
    struct py_type<winrt::Windows::Storage::AccessCache::AccessCacheOptions>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.storage.accesscache.AccessCacheOptions";
        static constexpr const char* module_name = "winrt.windows.storage.accesscache";
        static constexpr const char* type_name = "AccessCacheOptions";
    };

    template<>
    struct py_type<winrt::Windows::Storage::AccessCache::RecentStorageItemVisibility>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.storage.accesscache.RecentStorageItemVisibility";
        static constexpr const char* module_name = "winrt.windows.storage.accesscache";
        static constexpr const char* type_name = "RecentStorageItemVisibility";
    };

    template<>
    struct py_type<winrt::Windows::Storage::AccessCache::AccessListEntryView>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.storage.accesscache.AccessListEntryView";
        static constexpr const char* module_name = "winrt.windows.storage.accesscache";
        static constexpr const char* type_name = "AccessListEntryView";
    };

    template<>
    struct py_type<winrt::Windows::Storage::AccessCache::ItemRemovedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.storage.accesscache.ItemRemovedEventArgs";
        static constexpr const char* module_name = "winrt.windows.storage.accesscache";
        static constexpr const char* type_name = "ItemRemovedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Storage::AccessCache::StorageApplicationPermissions>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.storage.accesscache.StorageApplicationPermissions";
        static constexpr const char* module_name = "winrt.windows.storage.accesscache";
        static constexpr const char* type_name = "StorageApplicationPermissions";
    };

    template<>
    struct py_type<winrt::Windows::Storage::AccessCache::StorageItemAccessList>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.storage.accesscache.StorageItemAccessList";
        static constexpr const char* module_name = "winrt.windows.storage.accesscache";
        static constexpr const char* type_name = "StorageItemAccessList";
    };

    template<>
    struct py_type<winrt::Windows::Storage::AccessCache::StorageItemMostRecentlyUsedList>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.storage.accesscache.StorageItemMostRecentlyUsedList";
        static constexpr const char* module_name = "winrt.windows.storage.accesscache";
        static constexpr const char* type_name = "StorageItemMostRecentlyUsedList";
    };

    template<>
    struct py_type<winrt::Windows::Storage::AccessCache::IStorageItemAccessList>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.storage.accesscache._IStorageItemAccessList";
        static constexpr const char* module_name = "winrt.windows.storage.accesscache";
        static constexpr const char* type_name = "_IStorageItemAccessList";
    };

    template<>
    struct py_type<winrt::Windows::Storage::AccessCache::AccessListEntry>
    {
        static constexpr std::string_view from_tuple = "winrt._winrt_windows_storage_accesscache.AccessListEntry_from_tuple";
        static constexpr std::string_view qualified_name = "winrt.windows.storage.accesscache.AccessListEntry";
        static constexpr const char* module_name = "winrt.windows.storage.accesscache";
        static constexpr const char* type_name = "AccessListEntry";
    };
}

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.Storage.h")
#include "py.Windows.Storage.h"
#endif

#if __has_include("py.Windows.System.h")
#include "py.Windows.System.h"
#endif

namespace py::impl::Windows::Storage::AccessCache
{
}

namespace py::wrapper::Windows::Storage::AccessCache
{
    using AccessListEntryView = py::winrt_wrapper<winrt::Windows::Storage::AccessCache::AccessListEntryView>;
    using ItemRemovedEventArgs = py::winrt_wrapper<winrt::Windows::Storage::AccessCache::ItemRemovedEventArgs>;
    using StorageApplicationPermissions = py::winrt_wrapper<winrt::Windows::Storage::AccessCache::StorageApplicationPermissions>;
    using StorageItemAccessList = py::winrt_wrapper<winrt::Windows::Storage::AccessCache::StorageItemAccessList>;
    using StorageItemMostRecentlyUsedList = py::winrt_wrapper<winrt::Windows::Storage::AccessCache::StorageItemMostRecentlyUsedList>;
    using IStorageItemAccessList = py::winrt_wrapper<winrt::Windows::Storage::AccessCache::IStorageItemAccessList>;
    using AccessListEntry = py::winrt_struct_wrapper<winrt::Windows::Storage::AccessCache::AccessListEntry>;
}

namespace py
{
}
