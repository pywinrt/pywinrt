// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.240111.5

#pragma once
#ifndef WINRT_Windows_Phone_System_H
#define WINRT_Windows_Phone_System_H
#include "winrt/base.h"
static_assert(winrt::check_version(CPPWINRT_VERSION, "2.0.240111.5"), "Mismatched C++/WinRT headers.");
#define CPPWINRT_VERSION "2.0.240111.5"
#include "winrt/impl/Windows.Phone.System.2.h"
namespace winrt::impl
{
    template <typename D> auto consume_Windows_Phone_System_ISystemProtectionStatics<D>::ScreenLocked() const
    {
        bool value{};
        check_hresult(WINRT_IMPL_SHIM(winrt::Windows::Phone::System::ISystemProtectionStatics)->get_ScreenLocked(&value));
        return value;
    }
    template <typename D> auto consume_Windows_Phone_System_ISystemProtectionUnlockStatics<D>::RequestScreenUnlock() const
    {
        check_hresult(WINRT_IMPL_SHIM(winrt::Windows::Phone::System::ISystemProtectionUnlockStatics)->RequestScreenUnlock());
    }
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::Phone::System::ISystemProtectionStatics> : produce_base<D, winrt::Windows::Phone::System::ISystemProtectionStatics>
    {
        int32_t __stdcall get_ScreenLocked(bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().ScreenLocked());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::Phone::System::ISystemProtectionUnlockStatics> : produce_base<D, winrt::Windows::Phone::System::ISystemProtectionUnlockStatics>
    {
        int32_t __stdcall RequestScreenUnlock() noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().RequestScreenUnlock();
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
}
WINRT_EXPORT namespace winrt::Windows::Phone::System
{
    inline auto SystemProtection::ScreenLocked()
    {
        return impl::call_factory_cast<bool(*)(ISystemProtectionStatics const&), SystemProtection, ISystemProtectionStatics>([](ISystemProtectionStatics const& f) { return f.ScreenLocked(); });
    }
    inline auto SystemProtection::RequestScreenUnlock()
    {
        impl::call_factory_cast<void(*)(ISystemProtectionUnlockStatics const&), SystemProtection, ISystemProtectionUnlockStatics>([](ISystemProtectionUnlockStatics const& f) { return f.RequestScreenUnlock(); });
    }
}
namespace std
{
#ifndef WINRT_LEAN_AND_MEAN
    template<> struct hash<winrt::Windows::Phone::System::ISystemProtectionStatics> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Phone::System::ISystemProtectionUnlockStatics> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Phone::System::SystemProtection> : winrt::impl::hash_base {};
#endif
#ifdef __cpp_lib_format
#endif
}
#endif
