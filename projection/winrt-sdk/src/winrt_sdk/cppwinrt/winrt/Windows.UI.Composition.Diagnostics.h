// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_UI_Composition_Diagnostics_H
#define WINRT_Windows_UI_Composition_Diagnostics_H
#include "winrt/base.h"
static_assert(winrt::check_version(CPPWINRT_VERSION, "2.0.250303.1"), "Mismatched C++/WinRT headers.");
#define CPPWINRT_VERSION "2.0.250303.1"
#include "winrt/Windows.UI.Composition.h"
#include "winrt/impl/Windows.UI.Composition.2.h"
#include "winrt/impl/Windows.UI.Composition.Diagnostics.2.h"
namespace winrt::impl
{
    template <typename D> auto consume_Windows_UI_Composition_Diagnostics_ICompositionDebugHeatMaps<D>::Hide(winrt::Windows::UI::Composition::Visual const& subtree) const
    {
        if constexpr (!std::is_same_v<D, winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugHeatMaps>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugHeatMaps, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugHeatMaps>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->Hide(*(void**)(&subtree)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugHeatMaps>**)this;
            check_hresult(_winrt_abi_type->Hide(*(void**)(&subtree)));
        }
    }
    template <typename D> auto consume_Windows_UI_Composition_Diagnostics_ICompositionDebugHeatMaps<D>::ShowMemoryUsage(winrt::Windows::UI::Composition::Visual const& subtree) const
    {
        if constexpr (!std::is_same_v<D, winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugHeatMaps>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugHeatMaps, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugHeatMaps>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->ShowMemoryUsage(*(void**)(&subtree)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugHeatMaps>**)this;
            check_hresult(_winrt_abi_type->ShowMemoryUsage(*(void**)(&subtree)));
        }
    }
    template <typename D> auto consume_Windows_UI_Composition_Diagnostics_ICompositionDebugHeatMaps<D>::ShowOverdraw(winrt::Windows::UI::Composition::Visual const& subtree, winrt::Windows::UI::Composition::Diagnostics::CompositionDebugOverdrawContentKinds const& contentKinds) const
    {
        if constexpr (!std::is_same_v<D, winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugHeatMaps>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugHeatMaps, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugHeatMaps>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->ShowOverdraw(*(void**)(&subtree), static_cast<uint32_t>(contentKinds)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugHeatMaps>**)this;
            check_hresult(_winrt_abi_type->ShowOverdraw(*(void**)(&subtree), static_cast<uint32_t>(contentKinds)));
        }
    }
    template <typename D> auto consume_Windows_UI_Composition_Diagnostics_ICompositionDebugHeatMaps<D>::ShowRedraw(winrt::Windows::UI::Composition::Visual const& subtree) const
    {
        if constexpr (!std::is_same_v<D, winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugHeatMaps>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugHeatMaps, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugHeatMaps>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->ShowRedraw(*(void**)(&subtree)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugHeatMaps>**)this;
            check_hresult(_winrt_abi_type->ShowRedraw(*(void**)(&subtree)));
        }
    }
    template <typename D> auto consume_Windows_UI_Composition_Diagnostics_ICompositionDebugSettings<D>::HeatMaps() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugSettings>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugSettings, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugSettings>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_HeatMaps(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugSettings>**)this;
            check_hresult(_winrt_abi_type->get_HeatMaps(&value));
        }
        return winrt::Windows::UI::Composition::Diagnostics::CompositionDebugHeatMaps{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_UI_Composition_Diagnostics_ICompositionDebugSettingsStatics<D>::TryGetSettings(winrt::Windows::UI::Composition::Compositor const& compositor) const
    {
        void* result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugSettingsStatics>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugSettingsStatics, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugSettingsStatics>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->TryGetSettings(*(void**)(&compositor), &result));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugSettingsStatics>**)this;
            check_hresult(_winrt_abi_type->TryGetSettings(*(void**)(&compositor), &result));
        }
        return winrt::Windows::UI::Composition::Diagnostics::CompositionDebugSettings{ result, take_ownership_from_abi };
    }
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugHeatMaps> : produce_base<D, winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugHeatMaps>
    {
        int32_t __stdcall Hide(void* subtree) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Hide(*reinterpret_cast<winrt::Windows::UI::Composition::Visual const*>(&subtree));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall ShowMemoryUsage(void* subtree) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().ShowMemoryUsage(*reinterpret_cast<winrt::Windows::UI::Composition::Visual const*>(&subtree));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall ShowOverdraw(void* subtree, uint32_t contentKinds) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().ShowOverdraw(*reinterpret_cast<winrt::Windows::UI::Composition::Visual const*>(&subtree), *reinterpret_cast<winrt::Windows::UI::Composition::Diagnostics::CompositionDebugOverdrawContentKinds const*>(&contentKinds));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall ShowRedraw(void* subtree) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().ShowRedraw(*reinterpret_cast<winrt::Windows::UI::Composition::Visual const*>(&subtree));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugSettings> : produce_base<D, winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugSettings>
    {
        int32_t __stdcall get_HeatMaps(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Windows::UI::Composition::Diagnostics::CompositionDebugHeatMaps>(this->shim().HeatMaps());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugSettingsStatics> : produce_base<D, winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugSettingsStatics>
    {
        int32_t __stdcall TryGetSettings(void* compositor, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<winrt::Windows::UI::Composition::Diagnostics::CompositionDebugSettings>(this->shim().TryGetSettings(*reinterpret_cast<winrt::Windows::UI::Composition::Compositor const*>(&compositor)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
}
WINRT_EXPORT namespace winrt::Windows::UI::Composition::Diagnostics
{
    constexpr auto operator|(CompositionDebugOverdrawContentKinds const left, CompositionDebugOverdrawContentKinds const right) noexcept
    {
        return static_cast<CompositionDebugOverdrawContentKinds>(impl::to_underlying_type(left) | impl::to_underlying_type(right));
    }
    constexpr auto operator|=(CompositionDebugOverdrawContentKinds& left, CompositionDebugOverdrawContentKinds const right) noexcept
    {
        left = left | right;
        return left;
    }
    constexpr auto operator&(CompositionDebugOverdrawContentKinds const left, CompositionDebugOverdrawContentKinds const right) noexcept
    {
        return static_cast<CompositionDebugOverdrawContentKinds>(impl::to_underlying_type(left) & impl::to_underlying_type(right));
    }
    constexpr auto operator&=(CompositionDebugOverdrawContentKinds& left, CompositionDebugOverdrawContentKinds const right) noexcept
    {
        left = left & right;
        return left;
    }
    constexpr auto operator~(CompositionDebugOverdrawContentKinds const value) noexcept
    {
        return static_cast<CompositionDebugOverdrawContentKinds>(~impl::to_underlying_type(value));
    }
    constexpr auto operator^(CompositionDebugOverdrawContentKinds const left, CompositionDebugOverdrawContentKinds const right) noexcept
    {
        return static_cast<CompositionDebugOverdrawContentKinds>(impl::to_underlying_type(left) ^ impl::to_underlying_type(right));
    }
    constexpr auto operator^=(CompositionDebugOverdrawContentKinds& left, CompositionDebugOverdrawContentKinds const right) noexcept
    {
        left = left ^ right;
        return left;
    }
    inline auto CompositionDebugSettings::TryGetSettings(winrt::Windows::UI::Composition::Compositor const& compositor)
    {
        return impl::call_factory<CompositionDebugSettings, ICompositionDebugSettingsStatics>([&](ICompositionDebugSettingsStatics const& f) { return f.TryGetSettings(compositor); });
    }
}
namespace std
{
#ifndef WINRT_LEAN_AND_MEAN
    template<> struct hash<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugHeatMaps> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugSettings> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::UI::Composition::Diagnostics::ICompositionDebugSettingsStatics> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::UI::Composition::Diagnostics::CompositionDebugHeatMaps> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::UI::Composition::Diagnostics::CompositionDebugSettings> : winrt::impl::hash_base {};
#endif
#ifdef __cpp_lib_format
#endif
}
#endif
