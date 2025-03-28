// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.240111.5

#pragma once
#ifndef WINRT_Microsoft_UI_Input_Interop_H
#define WINRT_Microsoft_UI_Input_Interop_H
#include "winrt/base.h"
static_assert(winrt::check_version(CPPWINRT_VERSION, "2.0.240111.5"), "Mismatched C++/WinRT headers.");
#define CPPWINRT_VERSION "2.0.240111.5"
#include "winrt/Microsoft.UI.Input.h"
#include "winrt/impl/Microsoft.UI.Input.2.h"
#include "winrt/impl/Windows.Devices.Input.2.h"
#include "winrt/impl/Microsoft.UI.Input.Interop.2.h"
namespace winrt::impl
{
    template <typename D> auto consume_Microsoft_UI_Input_Interop_IPenDeviceInteropStatics<D>::FromPointerPoint(winrt::Microsoft::UI::Input::PointerPoint const& pointerPoint) const
    {
        void* result{};
        check_hresult(WINRT_IMPL_SHIM(winrt::Microsoft::UI::Input::Interop::IPenDeviceInteropStatics)->FromPointerPoint(*(void**)(&pointerPoint), &result));
        return winrt::Windows::Devices::Input::PenDevice{ result, take_ownership_from_abi };
    }
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Microsoft::UI::Input::Interop::IPenDeviceInteropStatics> : produce_base<D, winrt::Microsoft::UI::Input::Interop::IPenDeviceInteropStatics>
    {
        int32_t __stdcall FromPointerPoint(void* pointerPoint, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<winrt::Windows::Devices::Input::PenDevice>(this->shim().FromPointerPoint(*reinterpret_cast<winrt::Microsoft::UI::Input::PointerPoint const*>(&pointerPoint)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
}
WINRT_EXPORT namespace winrt::Microsoft::UI::Input::Interop
{
    inline auto PenDeviceInterop::FromPointerPoint(winrt::Microsoft::UI::Input::PointerPoint const& pointerPoint)
    {
        return impl::call_factory<PenDeviceInterop, IPenDeviceInteropStatics>([&](IPenDeviceInteropStatics const& f) { return f.FromPointerPoint(pointerPoint); });
    }
}
namespace std
{
#ifndef WINRT_LEAN_AND_MEAN
    template<> struct hash<winrt::Microsoft::UI::Input::Interop::IPenDeviceInteropStatics> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Microsoft::UI::Input::Interop::PenDeviceInterop> : winrt::impl::hash_base {};
#endif
#ifdef __cpp_lib_format
#endif
}
#endif
