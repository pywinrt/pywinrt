// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Media_AppBroadcasting_2_H
#define WINRT_Windows_Media_AppBroadcasting_2_H
#include "winrt/impl/Windows.System.1.h"
#include "winrt/impl/Windows.Media.AppBroadcasting.1.h"
WINRT_EXPORT namespace winrt::Windows::Media::AppBroadcasting
{
    struct WINRT_IMPL_EMPTY_BASES AppBroadcastingMonitor : winrt::Windows::Media::AppBroadcasting::IAppBroadcastingMonitor
    {
        AppBroadcastingMonitor(std::nullptr_t) noexcept {}
        AppBroadcastingMonitor(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Media::AppBroadcasting::IAppBroadcastingMonitor(ptr, take_ownership_from_abi) {}
        AppBroadcastingMonitor();
    };
    struct WINRT_IMPL_EMPTY_BASES AppBroadcastingStatus : winrt::Windows::Media::AppBroadcasting::IAppBroadcastingStatus
    {
        AppBroadcastingStatus(std::nullptr_t) noexcept {}
        AppBroadcastingStatus(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Media::AppBroadcasting::IAppBroadcastingStatus(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES AppBroadcastingStatusDetails : winrt::Windows::Media::AppBroadcasting::IAppBroadcastingStatusDetails
    {
        AppBroadcastingStatusDetails(std::nullptr_t) noexcept {}
        AppBroadcastingStatusDetails(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Media::AppBroadcasting::IAppBroadcastingStatusDetails(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES AppBroadcastingUI : winrt::Windows::Media::AppBroadcasting::IAppBroadcastingUI
    {
        AppBroadcastingUI(std::nullptr_t) noexcept {}
        AppBroadcastingUI(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Media::AppBroadcasting::IAppBroadcastingUI(ptr, take_ownership_from_abi) {}
        static auto GetDefault();
        static auto GetForUser(winrt::Windows::System::User const& user);
    };
}
#endif
