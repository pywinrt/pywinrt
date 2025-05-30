// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Media_Casting_0_H
#define WINRT_Windows_Media_Casting_0_H
WINRT_EXPORT namespace winrt::Windows::Devices::Enumeration
{
    struct DeviceInformation;
    struct DevicePickerAppearance;
}
WINRT_EXPORT namespace winrt::Windows::Foundation
{
    struct EventRegistrationToken;
    struct Rect;
    template <typename TSender, typename TResult> struct WINRT_IMPL_EMPTY_BASES TypedEventHandler;
    struct Uri;
}
WINRT_EXPORT namespace winrt::Windows::Storage::Streams
{
    struct IRandomAccessStreamWithContentType;
}
WINRT_EXPORT namespace winrt::Windows::UI::Popups
{
    enum class Placement : int32_t;
}
WINRT_EXPORT namespace winrt::Windows::Media::Casting
{
    enum class CastingConnectionErrorStatus : int32_t
    {
        Succeeded = 0,
        DeviceDidNotRespond = 1,
        DeviceError = 2,
        DeviceLocked = 3,
        ProtectedPlaybackFailed = 4,
        InvalidCastingSource = 5,
        Unknown = 6,
    };
    enum class CastingConnectionState : int32_t
    {
        Disconnected = 0,
        Connected = 1,
        Rendering = 2,
        Disconnecting = 3,
        Connecting = 4,
    };
    enum class CastingPlaybackTypes : uint32_t
    {
        None = 0,
        Audio = 0x1,
        Video = 0x2,
        Picture = 0x4,
    };
    struct ICastingConnection;
    struct ICastingConnectionErrorOccurredEventArgs;
    struct ICastingDevice;
    struct ICastingDevicePicker;
    struct ICastingDevicePickerFilter;
    struct ICastingDeviceSelectedEventArgs;
    struct ICastingDeviceStatics;
    struct ICastingSource;
    struct CastingConnection;
    struct CastingConnectionErrorOccurredEventArgs;
    struct CastingDevice;
    struct CastingDevicePicker;
    struct CastingDevicePickerFilter;
    struct CastingDeviceSelectedEventArgs;
    struct CastingSource;
}
namespace winrt::impl
{
    template <> struct category<winrt::Windows::Media::Casting::ICastingConnection>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Media::Casting::ICastingConnectionErrorOccurredEventArgs>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Media::Casting::ICastingDevice>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Media::Casting::ICastingDevicePicker>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Media::Casting::ICastingDevicePickerFilter>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Media::Casting::ICastingDeviceSelectedEventArgs>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Media::Casting::ICastingDeviceStatics>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Media::Casting::ICastingSource>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Media::Casting::CastingConnection>{ using type = class_category; };
    template <> struct category<winrt::Windows::Media::Casting::CastingConnectionErrorOccurredEventArgs>{ using type = class_category; };
    template <> struct category<winrt::Windows::Media::Casting::CastingDevice>{ using type = class_category; };
    template <> struct category<winrt::Windows::Media::Casting::CastingDevicePicker>{ using type = class_category; };
    template <> struct category<winrt::Windows::Media::Casting::CastingDevicePickerFilter>{ using type = class_category; };
    template <> struct category<winrt::Windows::Media::Casting::CastingDeviceSelectedEventArgs>{ using type = class_category; };
    template <> struct category<winrt::Windows::Media::Casting::CastingSource>{ using type = class_category; };
    template <> struct category<winrt::Windows::Media::Casting::CastingConnectionErrorStatus>{ using type = enum_category; };
    template <> struct category<winrt::Windows::Media::Casting::CastingConnectionState>{ using type = enum_category; };
    template <> struct category<winrt::Windows::Media::Casting::CastingPlaybackTypes>{ using type = enum_category; };
    template <> inline constexpr auto& name_v<winrt::Windows::Media::Casting::CastingConnection> = L"Windows.Media.Casting.CastingConnection";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::Casting::CastingConnectionErrorOccurredEventArgs> = L"Windows.Media.Casting.CastingConnectionErrorOccurredEventArgs";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::Casting::CastingDevice> = L"Windows.Media.Casting.CastingDevice";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::Casting::CastingDevicePicker> = L"Windows.Media.Casting.CastingDevicePicker";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::Casting::CastingDevicePickerFilter> = L"Windows.Media.Casting.CastingDevicePickerFilter";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::Casting::CastingDeviceSelectedEventArgs> = L"Windows.Media.Casting.CastingDeviceSelectedEventArgs";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::Casting::CastingSource> = L"Windows.Media.Casting.CastingSource";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::Casting::CastingConnectionErrorStatus> = L"Windows.Media.Casting.CastingConnectionErrorStatus";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::Casting::CastingConnectionState> = L"Windows.Media.Casting.CastingConnectionState";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::Casting::CastingPlaybackTypes> = L"Windows.Media.Casting.CastingPlaybackTypes";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::Casting::ICastingConnection> = L"Windows.Media.Casting.ICastingConnection";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::Casting::ICastingConnectionErrorOccurredEventArgs> = L"Windows.Media.Casting.ICastingConnectionErrorOccurredEventArgs";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::Casting::ICastingDevice> = L"Windows.Media.Casting.ICastingDevice";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::Casting::ICastingDevicePicker> = L"Windows.Media.Casting.ICastingDevicePicker";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::Casting::ICastingDevicePickerFilter> = L"Windows.Media.Casting.ICastingDevicePickerFilter";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::Casting::ICastingDeviceSelectedEventArgs> = L"Windows.Media.Casting.ICastingDeviceSelectedEventArgs";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::Casting::ICastingDeviceStatics> = L"Windows.Media.Casting.ICastingDeviceStatics";
    template <> inline constexpr auto& name_v<winrt::Windows::Media::Casting::ICastingSource> = L"Windows.Media.Casting.ICastingSource";
    template <> inline constexpr guid guid_v<winrt::Windows::Media::Casting::ICastingConnection>{ 0xCD951653,0xC2F1,0x4498,{ 0x8B,0x78,0x5F,0xB4,0xCD,0x36,0x40,0xDD } }; // CD951653-C2F1-4498-8B78-5FB4CD3640DD
    template <> inline constexpr guid guid_v<winrt::Windows::Media::Casting::ICastingConnectionErrorOccurredEventArgs>{ 0xA7FB3C69,0x8719,0x4F00,{ 0x81,0xFB,0x96,0x18,0x63,0xC7,0x9A,0x32 } }; // A7FB3C69-8719-4F00-81FB-961863C79A32
    template <> inline constexpr guid guid_v<winrt::Windows::Media::Casting::ICastingDevice>{ 0xDE721C83,0x4A43,0x4AD1,{ 0xA6,0xD2,0x24,0x92,0xA7,0x96,0xC3,0xF2 } }; // DE721C83-4A43-4AD1-A6D2-2492A796C3F2
    template <> inline constexpr guid guid_v<winrt::Windows::Media::Casting::ICastingDevicePicker>{ 0xDCD39924,0x0591,0x49BE,{ 0xAA,0xCB,0x4B,0x82,0xEE,0x75,0x6A,0x95 } }; // DCD39924-0591-49BE-AACB-4B82EE756A95
    template <> inline constexpr guid guid_v<winrt::Windows::Media::Casting::ICastingDevicePickerFilter>{ 0xBE8C619C,0xB563,0x4354,{ 0xAE,0x33,0x9F,0xDA,0xAD,0x8C,0x62,0x91 } }; // BE8C619C-B563-4354-AE33-9FDAAD8C6291
    template <> inline constexpr guid guid_v<winrt::Windows::Media::Casting::ICastingDeviceSelectedEventArgs>{ 0xDC439E86,0xDD57,0x4D0D,{ 0x94,0x00,0xAF,0x45,0xE4,0xFB,0x36,0x63 } }; // DC439E86-DD57-4D0D-9400-AF45E4FB3663
    template <> inline constexpr guid guid_v<winrt::Windows::Media::Casting::ICastingDeviceStatics>{ 0xE7D958D7,0x4D13,0x4237,{ 0xA3,0x65,0x4C,0x4F,0x6A,0x4C,0xFD,0x2F } }; // E7D958D7-4D13-4237-A365-4C4F6A4CFD2F
    template <> inline constexpr guid guid_v<winrt::Windows::Media::Casting::ICastingSource>{ 0xF429EA72,0x3467,0x47E6,{ 0xA0,0x27,0x52,0x29,0x23,0xE9,0xD7,0x27 } }; // F429EA72-3467-47E6-A027-522923E9D727
    template <> struct default_interface<winrt::Windows::Media::Casting::CastingConnection>{ using type = winrt::Windows::Media::Casting::ICastingConnection; };
    template <> struct default_interface<winrt::Windows::Media::Casting::CastingConnectionErrorOccurredEventArgs>{ using type = winrt::Windows::Media::Casting::ICastingConnectionErrorOccurredEventArgs; };
    template <> struct default_interface<winrt::Windows::Media::Casting::CastingDevice>{ using type = winrt::Windows::Media::Casting::ICastingDevice; };
    template <> struct default_interface<winrt::Windows::Media::Casting::CastingDevicePicker>{ using type = winrt::Windows::Media::Casting::ICastingDevicePicker; };
    template <> struct default_interface<winrt::Windows::Media::Casting::CastingDevicePickerFilter>{ using type = winrt::Windows::Media::Casting::ICastingDevicePickerFilter; };
    template <> struct default_interface<winrt::Windows::Media::Casting::CastingDeviceSelectedEventArgs>{ using type = winrt::Windows::Media::Casting::ICastingDeviceSelectedEventArgs; };
    template <> struct default_interface<winrt::Windows::Media::Casting::CastingSource>{ using type = winrt::Windows::Media::Casting::ICastingSource; };
    template <> struct abi<winrt::Windows::Media::Casting::ICastingConnection>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_State(int32_t*) noexcept = 0;
            virtual int32_t __stdcall get_Device(void**) noexcept = 0;
            virtual int32_t __stdcall get_Source(void**) noexcept = 0;
            virtual int32_t __stdcall put_Source(void*) noexcept = 0;
            virtual int32_t __stdcall add_StateChanged(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_StateChanged(winrt::event_token) noexcept = 0;
            virtual int32_t __stdcall add_ErrorOccurred(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_ErrorOccurred(winrt::event_token) noexcept = 0;
            virtual int32_t __stdcall RequestStartCastingAsync(void*, void**) noexcept = 0;
            virtual int32_t __stdcall DisconnectAsync(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Media::Casting::ICastingConnectionErrorOccurredEventArgs>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_ErrorStatus(int32_t*) noexcept = 0;
            virtual int32_t __stdcall get_Message(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Media::Casting::ICastingDevice>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_Id(void**) noexcept = 0;
            virtual int32_t __stdcall get_FriendlyName(void**) noexcept = 0;
            virtual int32_t __stdcall get_Icon(void**) noexcept = 0;
            virtual int32_t __stdcall GetSupportedCastingPlaybackTypesAsync(void**) noexcept = 0;
            virtual int32_t __stdcall CreateCastingConnection(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Media::Casting::ICastingDevicePicker>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_Filter(void**) noexcept = 0;
            virtual int32_t __stdcall get_Appearance(void**) noexcept = 0;
            virtual int32_t __stdcall add_CastingDeviceSelected(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_CastingDeviceSelected(winrt::event_token) noexcept = 0;
            virtual int32_t __stdcall add_CastingDevicePickerDismissed(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_CastingDevicePickerDismissed(winrt::event_token) noexcept = 0;
            virtual int32_t __stdcall Show(winrt::Windows::Foundation::Rect) noexcept = 0;
            virtual int32_t __stdcall ShowWithPlacement(winrt::Windows::Foundation::Rect, int32_t) noexcept = 0;
            virtual int32_t __stdcall Hide() noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Media::Casting::ICastingDevicePickerFilter>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_SupportsAudio(bool*) noexcept = 0;
            virtual int32_t __stdcall put_SupportsAudio(bool) noexcept = 0;
            virtual int32_t __stdcall get_SupportsVideo(bool*) noexcept = 0;
            virtual int32_t __stdcall put_SupportsVideo(bool) noexcept = 0;
            virtual int32_t __stdcall get_SupportsPictures(bool*) noexcept = 0;
            virtual int32_t __stdcall put_SupportsPictures(bool) noexcept = 0;
            virtual int32_t __stdcall get_SupportedCastingSources(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Media::Casting::ICastingDeviceSelectedEventArgs>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_SelectedCastingDevice(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Media::Casting::ICastingDeviceStatics>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall GetDeviceSelector(uint32_t, void**) noexcept = 0;
            virtual int32_t __stdcall GetDeviceSelectorFromCastingSourceAsync(void*, void**) noexcept = 0;
            virtual int32_t __stdcall FromIdAsync(void*, void**) noexcept = 0;
            virtual int32_t __stdcall DeviceInfoSupportsCastingAsync(void*, void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Media::Casting::ICastingSource>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_PreferredSourceUri(void**) noexcept = 0;
            virtual int32_t __stdcall put_PreferredSourceUri(void*) noexcept = 0;
        };
    };
    template <typename D>
    struct consume_Windows_Media_Casting_ICastingConnection
    {
        [[nodiscard]] auto State() const;
        [[nodiscard]] auto Device() const;
        [[nodiscard]] auto Source() const;
        auto Source(winrt::Windows::Media::Casting::CastingSource const& value) const;
        auto StateChanged(winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::Casting::CastingConnection, winrt::Windows::Foundation::IInspectable> const& handler) const;
        using StateChanged_revoker = impl::event_revoker<winrt::Windows::Media::Casting::ICastingConnection, &impl::abi_t<winrt::Windows::Media::Casting::ICastingConnection>::remove_StateChanged>;
        [[nodiscard]] auto StateChanged(auto_revoke_t, winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::Casting::CastingConnection, winrt::Windows::Foundation::IInspectable> const& handler) const;
        auto StateChanged(winrt::event_token const& token) const noexcept;
        auto ErrorOccurred(winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::Casting::CastingConnection, winrt::Windows::Media::Casting::CastingConnectionErrorOccurredEventArgs> const& handler) const;
        using ErrorOccurred_revoker = impl::event_revoker<winrt::Windows::Media::Casting::ICastingConnection, &impl::abi_t<winrt::Windows::Media::Casting::ICastingConnection>::remove_ErrorOccurred>;
        [[nodiscard]] auto ErrorOccurred(auto_revoke_t, winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::Casting::CastingConnection, winrt::Windows::Media::Casting::CastingConnectionErrorOccurredEventArgs> const& handler) const;
        auto ErrorOccurred(winrt::event_token const& token) const noexcept;
        auto RequestStartCastingAsync(winrt::Windows::Media::Casting::CastingSource const& value) const;
        auto DisconnectAsync() const;
    };
    template <> struct consume<winrt::Windows::Media::Casting::ICastingConnection>
    {
        template <typename D> using type = consume_Windows_Media_Casting_ICastingConnection<D>;
    };
    template <typename D>
    struct consume_Windows_Media_Casting_ICastingConnectionErrorOccurredEventArgs
    {
        [[nodiscard]] auto ErrorStatus() const;
        [[nodiscard]] auto Message() const;
    };
    template <> struct consume<winrt::Windows::Media::Casting::ICastingConnectionErrorOccurredEventArgs>
    {
        template <typename D> using type = consume_Windows_Media_Casting_ICastingConnectionErrorOccurredEventArgs<D>;
    };
    template <typename D>
    struct consume_Windows_Media_Casting_ICastingDevice
    {
        [[nodiscard]] auto Id() const;
        [[nodiscard]] auto FriendlyName() const;
        [[nodiscard]] auto Icon() const;
        auto GetSupportedCastingPlaybackTypesAsync() const;
        auto CreateCastingConnection() const;
    };
    template <> struct consume<winrt::Windows::Media::Casting::ICastingDevice>
    {
        template <typename D> using type = consume_Windows_Media_Casting_ICastingDevice<D>;
    };
    template <typename D>
    struct consume_Windows_Media_Casting_ICastingDevicePicker
    {
        [[nodiscard]] auto Filter() const;
        [[nodiscard]] auto Appearance() const;
        auto CastingDeviceSelected(winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::Casting::CastingDevicePicker, winrt::Windows::Media::Casting::CastingDeviceSelectedEventArgs> const& handler) const;
        using CastingDeviceSelected_revoker = impl::event_revoker<winrt::Windows::Media::Casting::ICastingDevicePicker, &impl::abi_t<winrt::Windows::Media::Casting::ICastingDevicePicker>::remove_CastingDeviceSelected>;
        [[nodiscard]] auto CastingDeviceSelected(auto_revoke_t, winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::Casting::CastingDevicePicker, winrt::Windows::Media::Casting::CastingDeviceSelectedEventArgs> const& handler) const;
        auto CastingDeviceSelected(winrt::event_token const& token) const noexcept;
        auto CastingDevicePickerDismissed(winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::Casting::CastingDevicePicker, winrt::Windows::Foundation::IInspectable> const& handler) const;
        using CastingDevicePickerDismissed_revoker = impl::event_revoker<winrt::Windows::Media::Casting::ICastingDevicePicker, &impl::abi_t<winrt::Windows::Media::Casting::ICastingDevicePicker>::remove_CastingDevicePickerDismissed>;
        [[nodiscard]] auto CastingDevicePickerDismissed(auto_revoke_t, winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Media::Casting::CastingDevicePicker, winrt::Windows::Foundation::IInspectable> const& handler) const;
        auto CastingDevicePickerDismissed(winrt::event_token const& token) const noexcept;
        auto Show(winrt::Windows::Foundation::Rect const& selection) const;
        auto Show(winrt::Windows::Foundation::Rect const& selection, winrt::Windows::UI::Popups::Placement const& preferredPlacement) const;
        auto Hide() const;
    };
    template <> struct consume<winrt::Windows::Media::Casting::ICastingDevicePicker>
    {
        template <typename D> using type = consume_Windows_Media_Casting_ICastingDevicePicker<D>;
    };
    template <typename D>
    struct consume_Windows_Media_Casting_ICastingDevicePickerFilter
    {
        [[nodiscard]] auto SupportsAudio() const;
        auto SupportsAudio(bool value) const;
        [[nodiscard]] auto SupportsVideo() const;
        auto SupportsVideo(bool value) const;
        [[nodiscard]] auto SupportsPictures() const;
        auto SupportsPictures(bool value) const;
        [[nodiscard]] auto SupportedCastingSources() const;
    };
    template <> struct consume<winrt::Windows::Media::Casting::ICastingDevicePickerFilter>
    {
        template <typename D> using type = consume_Windows_Media_Casting_ICastingDevicePickerFilter<D>;
    };
    template <typename D>
    struct consume_Windows_Media_Casting_ICastingDeviceSelectedEventArgs
    {
        [[nodiscard]] auto SelectedCastingDevice() const;
    };
    template <> struct consume<winrt::Windows::Media::Casting::ICastingDeviceSelectedEventArgs>
    {
        template <typename D> using type = consume_Windows_Media_Casting_ICastingDeviceSelectedEventArgs<D>;
    };
    template <typename D>
    struct consume_Windows_Media_Casting_ICastingDeviceStatics
    {
        auto GetDeviceSelector(winrt::Windows::Media::Casting::CastingPlaybackTypes const& type) const;
        auto GetDeviceSelectorFromCastingSourceAsync(winrt::Windows::Media::Casting::CastingSource const& castingSource) const;
        auto FromIdAsync(param::hstring const& value) const;
        auto DeviceInfoSupportsCastingAsync(winrt::Windows::Devices::Enumeration::DeviceInformation const& device) const;
    };
    template <> struct consume<winrt::Windows::Media::Casting::ICastingDeviceStatics>
    {
        template <typename D> using type = consume_Windows_Media_Casting_ICastingDeviceStatics<D>;
    };
    template <typename D>
    struct consume_Windows_Media_Casting_ICastingSource
    {
        [[nodiscard]] auto PreferredSourceUri() const;
        auto PreferredSourceUri(winrt::Windows::Foundation::Uri const& value) const;
    };
    template <> struct consume<winrt::Windows::Media::Casting::ICastingSource>
    {
        template <typename D> using type = consume_Windows_Media_Casting_ICastingSource<D>;
    };
}
#endif
