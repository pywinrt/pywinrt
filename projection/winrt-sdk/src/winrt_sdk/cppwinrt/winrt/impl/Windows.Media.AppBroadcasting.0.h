// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Media_AppBroadcasting_0_H
#define WINRT_Windows_Media_AppBroadcasting_0_H
WINRT_EXPORT namespace winrt::Windows::Foundation
{
    struct EventRegistrationToken;
    template <typename TSender, typename TResult> struct WINRT_IMPL_EMPTY_BASES TypedEventHandler;
}
WINRT_EXPORT namespace winrt::Windows::System
{
    struct User;
}
WINRT_EXPORT namespace winrt::Windows::Media::AppBroadcasting
{
    struct IAppBroadcastingMonitor;
    struct IAppBroadcastingStatus;
    struct IAppBroadcastingStatusDetails;
    struct IAppBroadcastingUI;
    struct IAppBroadcastingUIStatics;
    struct AppBroadcastingMonitor;
    struct AppBroadcastingStatus;
    struct AppBroadcastingStatusDetails;
    struct AppBroadcastingUI;
    struct AppBroadcastingContract;
}
namespace winrt::impl
{
    template <> struct category<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingMonitor>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingStatus>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingStatusDetails>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingUI>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingUIStatics>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Media::AppBroadcasting::AppBroadcastingMonitor>{ using type = class_category; };
    template <> struct category<winrt::Windows::Media::AppBroadcasting::AppBroadcastingStatus>{ using type = class_category; };
    template <> struct category<winrt::Windows::Media::AppBroadcasting::AppBroadcastingStatusDetails>{ using type = class_category; };
    template <> struct category<winrt::Windows::Media::AppBroadcasting::AppBroadcastingUI>{ using type = class_category; };
    template <> inline constexpr auto& name_v<winrt::Windows::Media::AppBroadcasting::AppBroadcastingMonitor> = L"Windows.Media.AppBroadcasting.AppBroadcastingMonitor";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::AppBroadcasting::AppBroadcastingStatus> = L"Windows.Media.AppBroadcasting.AppBroadcastingStatus";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::AppBroadcasting::AppBroadcastingStatusDetails> = L"Windows.Media.AppBroadcasting.AppBroadcastingStatusDetails";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::AppBroadcasting::AppBroadcastingUI> = L"Windows.Media.AppBroadcasting.AppBroadcastingUI";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingMonitor> = L"Windows.Media.AppBroadcasting.IAppBroadcastingMonitor";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingStatus> = L"Windows.Media.AppBroadcasting.IAppBroadcastingStatus";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingStatusDetails> = L"Windows.Media.AppBroadcasting.IAppBroadcastingStatusDetails";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingUI> = L"Windows.Media.AppBroadcasting.IAppBroadcastingUI";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingUIStatics> = L"Windows.Media.AppBroadcasting.IAppBroadcastingUIStatics";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::AppBroadcasting::AppBroadcastingContract> = L"Windows.Media.AppBroadcasting.AppBroadcastingContract";
    template <> inline constexpr guid guid_v<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingMonitor>{ 0x00F95A68,0x8907,0x48A0,{ 0xB8,0xEF,0x24,0xD2,0x08,0x13,0x75,0x42 } }; // 00F95A68-8907-48A0-B8EF-24D208137542
    template <> inline constexpr guid guid_v<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingStatus>{ 0x1225E4DF,0x03A1,0x42F8,{ 0x8B,0x80,0xC9,0x22,0x8C,0xD9,0xCF,0x2E } }; // 1225E4DF-03A1-42F8-8B80-C9228CD9CF2E
    template <> inline constexpr guid guid_v<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingStatusDetails>{ 0x069DADA4,0xB573,0x4E3C,{ 0x8E,0x19,0x1B,0xAF,0xAC,0xD0,0x97,0x13 } }; // 069DADA4-B573-4E3C-8E19-1BAFACD09713
    template <> inline constexpr guid guid_v<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingUI>{ 0xE56F9F8F,0xEE99,0x4DCA,{ 0xA3,0xC3,0x70,0xAF,0x3D,0xB4,0x4F,0x5F } }; // E56F9F8F-EE99-4DCA-A3C3-70AF3DB44F5F
    template <> inline constexpr guid guid_v<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingUIStatics>{ 0x55A8A79D,0x23CB,0x4579,{ 0x9C,0x34,0x88,0x6F,0xE0,0x2C,0x04,0x5A } }; // 55A8A79D-23CB-4579-9C34-886FE02C045A
    template <> struct default_interface<winrt::Windows::Media::AppBroadcasting::AppBroadcastingMonitor>{ using type = winrt::Windows::Media::AppBroadcasting::IAppBroadcastingMonitor; };
    template <> struct default_interface<winrt::Windows::Media::AppBroadcasting::AppBroadcastingStatus>{ using type = winrt::Windows::Media::AppBroadcasting::IAppBroadcastingStatus; };
    template <> struct default_interface<winrt::Windows::Media::AppBroadcasting::AppBroadcastingStatusDetails>{ using type = winrt::Windows::Media::AppBroadcasting::IAppBroadcastingStatusDetails; };
    template <> struct default_interface<winrt::Windows::Media::AppBroadcasting::AppBroadcastingUI>{ using type = winrt::Windows::Media::AppBroadcasting::IAppBroadcastingUI; };
    template <> struct abi<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingMonitor>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_IsCurrentAppBroadcasting(bool*) noexcept = 0;
            virtual int32_t __stdcall add_IsCurrentAppBroadcastingChanged(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_IsCurrentAppBroadcastingChanged(winrt::event_token) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingStatus>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_CanStartBroadcast(bool*) noexcept = 0;
            virtual int32_t __stdcall get_Details(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingStatusDetails>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_IsAnyAppBroadcasting(bool*) noexcept = 0;
            virtual int32_t __stdcall get_IsCaptureResourceUnavailable(bool*) noexcept = 0;
            virtual int32_t __stdcall get_IsGameStreamInProgress(bool*) noexcept = 0;
            virtual int32_t __stdcall get_IsGpuConstrained(bool*) noexcept = 0;
            virtual int32_t __stdcall get_IsAppInactive(bool*) noexcept = 0;
            virtual int32_t __stdcall get_IsBlockedForApp(bool*) noexcept = 0;
            virtual int32_t __stdcall get_IsDisabledByUser(bool*) noexcept = 0;
            virtual int32_t __stdcall get_IsDisabledBySystem(bool*) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingUI>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall GetStatus(void**) noexcept = 0;
            virtual int32_t __stdcall ShowBroadcastUI() noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingUIStatics>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall GetDefault(void**) noexcept = 0;
            virtual int32_t __stdcall GetForUser(void*, void**) noexcept = 0;
        };
    };
    template <typename D>
    struct consume_Windows_Media_AppBroadcasting_IAppBroadcastingMonitor
    {
        [[nodiscard]] auto IsCurrentAppBroadcasting() const;
        auto IsCurrentAppBroadcastingChanged(winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::AppBroadcasting::AppBroadcastingMonitor, winrt::Windows::Foundation::IInspectable> const& handler) const;
        using IsCurrentAppBroadcastingChanged_revoker = impl::event_revoker<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingMonitor, &impl::abi_t<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingMonitor>::remove_IsCurrentAppBroadcastingChanged>;
        [[nodiscard]] auto IsCurrentAppBroadcastingChanged(auto_revoke_t, winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::AppBroadcasting::AppBroadcastingMonitor, winrt::Windows::Foundation::IInspectable> const& handler) const;
        auto IsCurrentAppBroadcastingChanged(winrt::event_token const& token) const noexcept;
    };
    template <> struct consume<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingMonitor>
    {
        template <typename D> using type = consume_Windows_Media_AppBroadcasting_IAppBroadcastingMonitor<D>;
    };
    template <typename D>
    struct consume_Windows_Media_AppBroadcasting_IAppBroadcastingStatus
    {
        [[nodiscard]] auto CanStartBroadcast() const;
        [[nodiscard]] auto Details() const;
    };
    template <> struct consume<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingStatus>
    {
        template <typename D> using type = consume_Windows_Media_AppBroadcasting_IAppBroadcastingStatus<D>;
    };
    template <typename D>
    struct consume_Windows_Media_AppBroadcasting_IAppBroadcastingStatusDetails
    {
        [[nodiscard]] auto IsAnyAppBroadcasting() const;
        [[nodiscard]] auto IsCaptureResourceUnavailable() const;
        [[nodiscard]] auto IsGameStreamInProgress() const;
        [[nodiscard]] auto IsGpuConstrained() const;
        [[nodiscard]] auto IsAppInactive() const;
        [[nodiscard]] auto IsBlockedForApp() const;
        [[nodiscard]] auto IsDisabledByUser() const;
        [[nodiscard]] auto IsDisabledBySystem() const;
    };
    template <> struct consume<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingStatusDetails>
    {
        template <typename D> using type = consume_Windows_Media_AppBroadcasting_IAppBroadcastingStatusDetails<D>;
    };
    template <typename D>
    struct consume_Windows_Media_AppBroadcasting_IAppBroadcastingUI
    {
        auto GetStatus() const;
        auto ShowBroadcastUI() const;
    };
    template <> struct consume<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingUI>
    {
        template <typename D> using type = consume_Windows_Media_AppBroadcasting_IAppBroadcastingUI<D>;
    };
    template <typename D>
    struct consume_Windows_Media_AppBroadcasting_IAppBroadcastingUIStatics
    {
        auto GetDefault() const;
        auto GetForUser(winrt::Windows::System::User const& user) const;
    };
    template <> struct consume<winrt::Windows::Media::AppBroadcasting::IAppBroadcastingUIStatics>
    {
        template <typename D> using type = consume_Windows_Media_AppBroadcasting_IAppBroadcastingUIStatics<D>;
    };
}
#endif
