// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_UI_Input_Inking_Preview_H
#define WINRT_Windows_UI_Input_Inking_Preview_H
#include "winrt/base.h"
static_assert(winrt::check_version(CPPWINRT_VERSION, "2.0.250303.1"), "Mismatched C++/WinRT headers.");
#define CPPWINRT_VERSION "2.0.250303.1"
#include "winrt/Windows.UI.Input.Inking.h"
#include "winrt/impl/Windows.Foundation.2.h"
#include "winrt/impl/Windows.UI.Composition.2.h"
#include "winrt/impl/Windows.UI.Input.Inking.Preview.2.h"
namespace winrt::impl
{
    template <typename D> auto consume_Windows_UI_Input_Inking_Preview_IPalmRejectionDelayZonePreviewStatics<D>::CreateForVisual(winrt::Windows::UI::Composition::Visual const& inputPanelVisual, winrt::Windows::Foundation::Rect const& inputPanelRect) const
    {
        void* result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::UI::Input::Inking::Preview::IPalmRejectionDelayZonePreviewStatics>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::UI::Input::Inking::Preview::IPalmRejectionDelayZonePreviewStatics, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::UI::Input::Inking::Preview::IPalmRejectionDelayZonePreviewStatics>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->CreateForVisual(*(void**)(&inputPanelVisual), impl::bind_in(inputPanelRect), &result));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::UI::Input::Inking::Preview::IPalmRejectionDelayZonePreviewStatics>**)this;
            check_hresult(_winrt_abi_type->CreateForVisual(*(void**)(&inputPanelVisual), impl::bind_in(inputPanelRect), &result));
        }
        return winrt::Windows::UI::Input::Inking::Preview::PalmRejectionDelayZonePreview{ result, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_UI_Input_Inking_Preview_IPalmRejectionDelayZonePreviewStatics<D>::CreateForVisual(winrt::Windows::UI::Composition::Visual const& inputPanelVisual, winrt::Windows::Foundation::Rect const& inputPanelRect, winrt::Windows::UI::Composition::Visual const& viewportVisual, winrt::Windows::Foundation::Rect const& viewportRect) const
    {
        void* result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::UI::Input::Inking::Preview::IPalmRejectionDelayZonePreviewStatics>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::UI::Input::Inking::Preview::IPalmRejectionDelayZonePreviewStatics, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::UI::Input::Inking::Preview::IPalmRejectionDelayZonePreviewStatics>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->CreateForVisualWithViewportClip(*(void**)(&inputPanelVisual), impl::bind_in(inputPanelRect), *(void**)(&viewportVisual), impl::bind_in(viewportRect), &result));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::UI::Input::Inking::Preview::IPalmRejectionDelayZonePreviewStatics>**)this;
            check_hresult(_winrt_abi_type->CreateForVisualWithViewportClip(*(void**)(&inputPanelVisual), impl::bind_in(inputPanelRect), *(void**)(&viewportVisual), impl::bind_in(viewportRect), &result));
        }
        return winrt::Windows::UI::Input::Inking::Preview::PalmRejectionDelayZonePreview{ result, take_ownership_from_abi };
    }
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::UI::Input::Inking::Preview::IPalmRejectionDelayZonePreview> : produce_base<D, winrt::Windows::UI::Input::Inking::Preview::IPalmRejectionDelayZonePreview>
    {
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::UI::Input::Inking::Preview::IPalmRejectionDelayZonePreviewStatics> : produce_base<D, winrt::Windows::UI::Input::Inking::Preview::IPalmRejectionDelayZonePreviewStatics>
    {
        int32_t __stdcall CreateForVisual(void* inputPanelVisual, winrt::Windows::Foundation::Rect inputPanelRect, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<winrt::Windows::UI::Input::Inking::Preview::PalmRejectionDelayZonePreview>(this->shim().CreateForVisual(*reinterpret_cast<winrt::Windows::UI::Composition::Visual const*>(&inputPanelVisual), *reinterpret_cast<winrt::Windows::Foundation::Rect const*>(&inputPanelRect)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall CreateForVisualWithViewportClip(void* inputPanelVisual, winrt::Windows::Foundation::Rect inputPanelRect, void* viewportVisual, winrt::Windows::Foundation::Rect viewportRect, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<winrt::Windows::UI::Input::Inking::Preview::PalmRejectionDelayZonePreview>(this->shim().CreateForVisual(*reinterpret_cast<winrt::Windows::UI::Composition::Visual const*>(&inputPanelVisual), *reinterpret_cast<winrt::Windows::Foundation::Rect const*>(&inputPanelRect), *reinterpret_cast<winrt::Windows::UI::Composition::Visual const*>(&viewportVisual), *reinterpret_cast<winrt::Windows::Foundation::Rect const*>(&viewportRect)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
}
WINRT_EXPORT namespace winrt::Windows::UI::Input::Inking::Preview
{
    inline auto PalmRejectionDelayZonePreview::CreateForVisual(winrt::Windows::UI::Composition::Visual const& inputPanelVisual, winrt::Windows::Foundation::Rect const& inputPanelRect)
    {
        return impl::call_factory<PalmRejectionDelayZonePreview, IPalmRejectionDelayZonePreviewStatics>([&](IPalmRejectionDelayZonePreviewStatics const& f) { return f.CreateForVisual(inputPanelVisual, inputPanelRect); });
    }
    inline auto PalmRejectionDelayZonePreview::CreateForVisual(winrt::Windows::UI::Composition::Visual const& inputPanelVisual, winrt::Windows::Foundation::Rect const& inputPanelRect, winrt::Windows::UI::Composition::Visual const& viewportVisual, winrt::Windows::Foundation::Rect const& viewportRect)
    {
        return impl::call_factory<PalmRejectionDelayZonePreview, IPalmRejectionDelayZonePreviewStatics>([&](IPalmRejectionDelayZonePreviewStatics const& f) { return f.CreateForVisual(inputPanelVisual, inputPanelRect, viewportVisual, viewportRect); });
    }
}
namespace std
{
#ifndef WINRT_LEAN_AND_MEAN
    template<> struct hash<winrt::Windows::UI::Input::Inking::Preview::IPalmRejectionDelayZonePreview> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::UI::Input::Inking::Preview::IPalmRejectionDelayZonePreviewStatics> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::UI::Input::Inking::Preview::PalmRejectionDelayZonePreview> : winrt::impl::hash_base {};
#endif
#ifdef __cpp_lib_format
#endif
}
#endif
