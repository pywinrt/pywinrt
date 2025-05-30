// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Microsoft_UI_System_H
#define WINRT_Microsoft_UI_System_H
#include "winrt/base.h"
static_assert(winrt::check_version(CPPWINRT_VERSION, "2.0.250303.1"), "Mismatched C++/WinRT headers.");
#define CPPWINRT_VERSION "2.0.250303.1"
#include "winrt/Microsoft.UI.h"
#include "winrt/impl/Microsoft.UI.2.h"
#include "winrt/impl/Windows.Foundation.2.h"
#include "winrt/impl/Microsoft.UI.System.2.h"
namespace winrt::impl
{
    template <typename D> auto consume_Microsoft_UI_System_IThemeSettings<D>::Changed(winrt::Windows::Foundation::TypedEventHandler<winrt::Microsoft::UI::System::ThemeSettings, winrt::Windows::Foundation::IInspectable> const& handler) const
    {
        winrt::event_token token{};
        if constexpr (!std::is_same_v<D, winrt::Microsoft::UI::System::IThemeSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::UI::System::IThemeSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::UI::System::IThemeSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->add_Changed(*(void**)(&handler), put_abi(token)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::UI::System::IThemeSettings>**)this;
            check_hresult(_winrt_abi_type->add_Changed(*(void**)(&handler), put_abi(token)));
        }
        return token;
    }
    template <typename D> auto consume_Microsoft_UI_System_IThemeSettings<D>::Changed(auto_revoke_t, winrt::Windows::Foundation::TypedEventHandler<winrt::Microsoft::UI::System::ThemeSettings, winrt::Windows::Foundation::IInspectable> const& handler) const
    {
        return impl::make_event_revoker<D, Changed_revoker>(this, Changed(handler));
    }
    template <typename D> auto consume_Microsoft_UI_System_IThemeSettings<D>::Changed(winrt::event_token const& token) const noexcept
    {
        if constexpr (!std::is_same_v<D, winrt::Microsoft::UI::System::IThemeSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::UI::System::IThemeSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::UI::System::IThemeSettings>**)&_winrt_casted_result;
            _winrt_abi_type->remove_Changed(impl::bind_in(token));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::UI::System::IThemeSettings>**)this;
            _winrt_abi_type->remove_Changed(impl::bind_in(token));
        }
    }
    template <typename D> auto consume_Microsoft_UI_System_IThemeSettings<D>::HighContrast() const
    {
        bool value{};
        if constexpr (!std::is_same_v<D, winrt::Microsoft::UI::System::IThemeSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::UI::System::IThemeSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::UI::System::IThemeSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_HighContrast(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::UI::System::IThemeSettings>**)this;
            check_hresult(_winrt_abi_type->get_HighContrast(&value));
        }
        return value;
    }
    template <typename D> auto consume_Microsoft_UI_System_IThemeSettings<D>::HighContrastScheme() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Microsoft::UI::System::IThemeSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::UI::System::IThemeSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::UI::System::IThemeSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_HighContrastScheme(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::UI::System::IThemeSettings>**)this;
            check_hresult(_winrt_abi_type->get_HighContrastScheme(&value));
        }
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Microsoft_UI_System_IThemeSettingsStatics<D>::CreateForWindowId(winrt::Microsoft::UI::WindowId const& windowId) const
    {
        void* result{};
        if constexpr (!std::is_same_v<D, winrt::Microsoft::UI::System::IThemeSettingsStatics>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Microsoft::UI::System::IThemeSettingsStatics, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::UI::System::IThemeSettingsStatics>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->CreateForWindowId(impl::bind_in(windowId), &result));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Microsoft::UI::System::IThemeSettingsStatics>**)this;
            check_hresult(_winrt_abi_type->CreateForWindowId(impl::bind_in(windowId), &result));
        }
        return winrt::Microsoft::UI::System::ThemeSettings{ result, take_ownership_from_abi };
    }
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Microsoft::UI::System::IThemeSettings> : produce_base<D, winrt::Microsoft::UI::System::IThemeSettings>
    {
        int32_t __stdcall add_Changed(void* handler, winrt::event_token* token) noexcept final try
        {
            zero_abi<winrt::event_token>(token);
            typename D::abi_guard guard(this->shim());
            *token = detach_from<winrt::event_token>(this->shim().Changed(*reinterpret_cast<winrt::Windows::Foundation::TypedEventHandler<winrt::Microsoft::UI::System::ThemeSettings, winrt::Windows::Foundation::IInspectable> const*>(&handler)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall remove_Changed(winrt::event_token token) noexcept final
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Changed(*reinterpret_cast<winrt::event_token const*>(&token));
            return 0;
        }
        int32_t __stdcall get_HighContrast(bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().HighContrast());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_HighContrastScheme(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().HighContrastScheme());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Microsoft::UI::System::IThemeSettingsStatics> : produce_base<D, winrt::Microsoft::UI::System::IThemeSettingsStatics>
    {
        int32_t __stdcall CreateForWindowId(struct struct_Microsoft_UI_WindowId windowId, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<winrt::Microsoft::UI::System::ThemeSettings>(this->shim().CreateForWindowId(*reinterpret_cast<winrt::Microsoft::UI::WindowId const*>(&windowId)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
}
WINRT_EXPORT namespace winrt::Microsoft::UI::System
{
    inline auto ThemeSettings::CreateForWindowId(winrt::Microsoft::UI::WindowId const& windowId)
    {
        return impl::call_factory<ThemeSettings, IThemeSettingsStatics>([&](IThemeSettingsStatics const& f) { return f.CreateForWindowId(windowId); });
    }
}
namespace std
{
#ifndef WINRT_LEAN_AND_MEAN
    template<> struct hash<winrt::Microsoft::UI::System::IThemeSettings> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Microsoft::UI::System::IThemeSettingsStatics> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Microsoft::UI::System::ThemeSettings> : winrt::impl::hash_base {};
#endif
#ifdef __cpp_lib_format
#endif
}
#endif
