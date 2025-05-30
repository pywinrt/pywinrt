// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Management_Update_2_H
#define WINRT_Windows_Management_Update_2_H
#include "winrt/impl/Windows.Foundation.1.h"
#include "winrt/impl/Windows.Management.Update.1.h"
WINRT_EXPORT namespace winrt::Windows::Management::Update
{
    struct WINRT_IMPL_EMPTY_BASES PreviewBuildsManager : winrt::Windows::Management::Update::IPreviewBuildsManager
    {
        PreviewBuildsManager(std::nullptr_t) noexcept {}
        PreviewBuildsManager(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Management::Update::IPreviewBuildsManager(ptr, take_ownership_from_abi) {}
        static auto GetDefault();
        static auto IsSupported();
    };
    struct WINRT_IMPL_EMPTY_BASES PreviewBuildsState : winrt::Windows::Management::Update::IPreviewBuildsState
    {
        PreviewBuildsState(std::nullptr_t) noexcept {}
        PreviewBuildsState(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Management::Update::IPreviewBuildsState(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES WindowsUpdate : winrt::Windows::Management::Update::IWindowsUpdate
    {
        WindowsUpdate(std::nullptr_t) noexcept {}
        WindowsUpdate(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Management::Update::IWindowsUpdate(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES WindowsUpdateActionCompletedEventArgs : winrt::Windows::Management::Update::IWindowsUpdateActionCompletedEventArgs
    {
        WindowsUpdateActionCompletedEventArgs(std::nullptr_t) noexcept {}
        WindowsUpdateActionCompletedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Management::Update::IWindowsUpdateActionCompletedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES WindowsUpdateActionProgress : winrt::Windows::Management::Update::IWindowsUpdateActionProgress
    {
        WindowsUpdateActionProgress(std::nullptr_t) noexcept {}
        WindowsUpdateActionProgress(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Management::Update::IWindowsUpdateActionProgress(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES WindowsUpdateActionResult : winrt::Windows::Management::Update::IWindowsUpdateActionResult
    {
        WindowsUpdateActionResult(std::nullptr_t) noexcept {}
        WindowsUpdateActionResult(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Management::Update::IWindowsUpdateActionResult(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES WindowsUpdateAdministrator : winrt::Windows::Management::Update::IWindowsUpdateAdministrator
    {
        WindowsUpdateAdministrator(std::nullptr_t) noexcept {}
        WindowsUpdateAdministrator(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Management::Update::IWindowsUpdateAdministrator(ptr, take_ownership_from_abi) {}
        static auto GetRegisteredAdministrator(param::hstring const& organizationName);
        static auto RegisterForAdministration(param::hstring const& organizationName, winrt::Windows::Management::Update::WindowsUpdateAdministratorOptions const& options);
        static auto UnregisterForAdministration(param::hstring const& organizationName);
        static auto GetRegisteredAdministratorName();
        static auto RequestRestart(winrt::Windows::Management::Update::WindowsUpdateRestartRequestOptions const& restartOptions);
        static auto CancelRestartRequest(param::hstring const& requestRestartToken);
    };
    struct WINRT_IMPL_EMPTY_BASES WindowsUpdateApprovalData : winrt::Windows::Management::Update::IWindowsUpdateApprovalData
    {
        WindowsUpdateApprovalData(std::nullptr_t) noexcept {}
        WindowsUpdateApprovalData(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Management::Update::IWindowsUpdateApprovalData(ptr, take_ownership_from_abi) {}
        WindowsUpdateApprovalData();
    };
    struct WINRT_IMPL_EMPTY_BASES WindowsUpdateAttentionRequiredInfo : winrt::Windows::Management::Update::IWindowsUpdateAttentionRequiredInfo
    {
        WindowsUpdateAttentionRequiredInfo(std::nullptr_t) noexcept {}
        WindowsUpdateAttentionRequiredInfo(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Management::Update::IWindowsUpdateAttentionRequiredInfo(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES WindowsUpdateAttentionRequiredReasonChangedEventArgs : winrt::Windows::Management::Update::IWindowsUpdateAttentionRequiredReasonChangedEventArgs
    {
        WindowsUpdateAttentionRequiredReasonChangedEventArgs(std::nullptr_t) noexcept {}
        WindowsUpdateAttentionRequiredReasonChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Management::Update::IWindowsUpdateAttentionRequiredReasonChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES WindowsUpdateGetAdministratorResult : winrt::Windows::Management::Update::IWindowsUpdateGetAdministratorResult
    {
        WindowsUpdateGetAdministratorResult(std::nullptr_t) noexcept {}
        WindowsUpdateGetAdministratorResult(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Management::Update::IWindowsUpdateGetAdministratorResult(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES WindowsUpdateItem : winrt::Windows::Management::Update::IWindowsUpdateItem
    {
        WindowsUpdateItem(std::nullptr_t) noexcept {}
        WindowsUpdateItem(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Management::Update::IWindowsUpdateItem(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES WindowsUpdateManager : winrt::Windows::Management::Update::IWindowsUpdateManager
    {
        WindowsUpdateManager(std::nullptr_t) noexcept {}
        WindowsUpdateManager(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Management::Update::IWindowsUpdateManager(ptr, take_ownership_from_abi) {}
        explicit WindowsUpdateManager(param::hstring const& clientId);
    };
    struct WINRT_IMPL_EMPTY_BASES WindowsUpdateProgressChangedEventArgs : winrt::Windows::Management::Update::IWindowsUpdateProgressChangedEventArgs
    {
        WindowsUpdateProgressChangedEventArgs(std::nullptr_t) noexcept {}
        WindowsUpdateProgressChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Management::Update::IWindowsUpdateProgressChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES WindowsUpdateRestartRequestOptions : winrt::Windows::Management::Update::IWindowsUpdateRestartRequestOptions
    {
        WindowsUpdateRestartRequestOptions(std::nullptr_t) noexcept {}
        WindowsUpdateRestartRequestOptions(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Management::Update::IWindowsUpdateRestartRequestOptions(ptr, take_ownership_from_abi) {}
        WindowsUpdateRestartRequestOptions();
        WindowsUpdateRestartRequestOptions(param::hstring const& title, param::hstring const& description, winrt::Windows::Foundation::Uri const& moreInfoUrl, int32_t complianceDeadlineInDays, int32_t complianceGracePeriodInDays);
    };
    struct WINRT_IMPL_EMPTY_BASES WindowsUpdateScanCompletedEventArgs : winrt::Windows::Management::Update::IWindowsUpdateScanCompletedEventArgs
    {
        WindowsUpdateScanCompletedEventArgs(std::nullptr_t) noexcept {}
        WindowsUpdateScanCompletedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Management::Update::IWindowsUpdateScanCompletedEventArgs(ptr, take_ownership_from_abi) {}
    };
}
#endif
