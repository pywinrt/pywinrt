// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Microsoft_UI_System_0_H
#define WINRT_Microsoft_UI_System_0_H
WINRT_EXPORT namespace winrt::Microsoft::UI
{
    struct WindowId;
}
WINRT_EXPORT namespace winrt::Windows::Foundation
{
    struct EventRegistrationToken;
    template <typename TSender, typename TResult> struct WINRT_IMPL_EMPTY_BASES TypedEventHandler;
}
WINRT_EXPORT namespace winrt::Microsoft::UI::System
{
    struct IThemeSettings;
    struct IThemeSettingsStatics;
    struct ThemeSettings;
}
namespace winrt::impl
{
    template <> struct category<winrt::Microsoft::UI::System::IThemeSettings>{ using type = interface_category; };
    template <> struct category<winrt::Microsoft::UI::System::IThemeSettingsStatics>{ using type = interface_category; };
    template <> struct category<winrt::Microsoft::UI::System::ThemeSettings>{ using type = class_category; };
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::System::ThemeSettings> = L"Microsoft.UI.System.ThemeSettings";
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::System::IThemeSettings> = L"Microsoft.UI.System.IThemeSettings";
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::System::IThemeSettingsStatics> = L"Microsoft.UI.System.IThemeSettingsStatics";
    template <> inline constexpr guid guid_v<winrt::Microsoft::UI::System::IThemeSettings>{ 0x2228EE7E,0x6D15,0x563C,{ 0x8F,0x3C,0xE8,0x78,0x3B,0xA1,0x38,0x46 } }; // 2228EE7E-6D15-563C-8F3C-E8783BA13846
    template <> inline constexpr guid guid_v<winrt::Microsoft::UI::System::IThemeSettingsStatics>{ 0x1586907D,0x30DB,0x5F97,{ 0x8F,0xA1,0x89,0x40,0xC7,0x5D,0xCC,0xC0 } }; // 1586907D-30DB-5F97-8FA1-8940C75DCCC0
    template <> struct default_interface<winrt::Microsoft::UI::System::ThemeSettings>{ using type = winrt::Microsoft::UI::System::IThemeSettings; };
    template <> struct abi<winrt::Microsoft::UI::System::IThemeSettings>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall add_Changed(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_Changed(winrt::event_token) noexcept = 0;
            virtual int32_t __stdcall get_HighContrast(bool*) noexcept = 0;
            virtual int32_t __stdcall get_HighContrastScheme(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Microsoft::UI::System::IThemeSettingsStatics>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall CreateForWindowId(struct struct_Microsoft_UI_WindowId, void**) noexcept = 0;
        };
    };
    template <typename D>
    struct consume_Microsoft_UI_System_IThemeSettings
    {
        auto Changed(winrt::Windows::Foundation::TypedEventHandler<winrt::Microsoft::UI::System::ThemeSettings, winrt::Windows::Foundation::IInspectable> const& handler) const;
        using Changed_revoker = impl::event_revoker<winrt::Microsoft::UI::System::IThemeSettings, &impl::abi_t<winrt::Microsoft::UI::System::IThemeSettings>::remove_Changed>;
        [[nodiscard]] auto Changed(auto_revoke_t, winrt::Windows::Foundation::TypedEventHandler<winrt::Microsoft::UI::System::ThemeSettings, winrt::Windows::Foundation::IInspectable> const& handler) const;
        auto Changed(winrt::event_token const& token) const noexcept;
        [[nodiscard]] auto HighContrast() const;
        [[nodiscard]] auto HighContrastScheme() const;
    };
    template <> struct consume<winrt::Microsoft::UI::System::IThemeSettings>
    {
        template <typename D> using type = consume_Microsoft_UI_System_IThemeSettings<D>;
    };
    template <typename D>
    struct consume_Microsoft_UI_System_IThemeSettingsStatics
    {
        auto CreateForWindowId(winrt::Microsoft::UI::WindowId const& windowId) const;
    };
    template <> struct consume<winrt::Microsoft::UI::System::IThemeSettingsStatics>
    {
        template <typename D> using type = consume_Microsoft_UI_System_IThemeSettingsStatics<D>;
    };
}
#endif
