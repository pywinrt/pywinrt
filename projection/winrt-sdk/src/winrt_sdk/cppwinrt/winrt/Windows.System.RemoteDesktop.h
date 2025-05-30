// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_System_RemoteDesktop_H
#define WINRT_Windows_System_RemoteDesktop_H
#include "winrt/base.h"
static_assert(winrt::check_version(CPPWINRT_VERSION, "2.0.250303.1"), "Mismatched C++/WinRT headers.");
#define CPPWINRT_VERSION "2.0.250303.1"
#include "winrt/Windows.System.h"
#include "winrt/impl/Windows.System.RemoteDesktop.2.h"
namespace winrt::impl
{
    template <typename D> auto consume_Windows_System_RemoteDesktop_IInteractiveSessionStatics<D>::IsRemote() const
    {
        bool value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::System::RemoteDesktop::IInteractiveSessionStatics>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::System::RemoteDesktop::IInteractiveSessionStatics, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::RemoteDesktop::IInteractiveSessionStatics>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_IsRemote(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::System::RemoteDesktop::IInteractiveSessionStatics>**)this;
            check_hresult(_winrt_abi_type->get_IsRemote(&value));
        }
        return value;
    }
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::System::RemoteDesktop::IInteractiveSessionStatics> : produce_base<D, winrt::Windows::System::RemoteDesktop::IInteractiveSessionStatics>
    {
        int32_t __stdcall get_IsRemote(bool* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<bool>(this->shim().IsRemote());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
}
WINRT_EXPORT namespace winrt::Windows::System::RemoteDesktop
{
    inline auto InteractiveSession::IsRemote()
    {
        return impl::call_factory_cast<bool(*)(IInteractiveSessionStatics const&), InteractiveSession, IInteractiveSessionStatics>([](IInteractiveSessionStatics const& f) { return f.IsRemote(); });
    }
}
namespace std
{
#ifndef WINRT_LEAN_AND_MEAN
    template<> struct hash<winrt::Windows::System::RemoteDesktop::IInteractiveSessionStatics> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::System::RemoteDesktop::InteractiveSession> : winrt::impl::hash_base {};
#endif
#ifdef __cpp_lib_format
#endif
}
#endif
