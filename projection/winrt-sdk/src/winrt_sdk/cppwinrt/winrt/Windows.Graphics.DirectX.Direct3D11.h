// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Graphics_DirectX_Direct3D11_H
#define WINRT_Windows_Graphics_DirectX_Direct3D11_H
#include "winrt/base.h"
static_assert(winrt::check_version(CPPWINRT_VERSION, "2.0.250303.1"), "Mismatched C++/WinRT headers.");
#define CPPWINRT_VERSION "2.0.250303.1"
#include "winrt/Windows.Graphics.DirectX.h"
#include "winrt/impl/Windows.Graphics.DirectX.Direct3D11.2.h"
namespace winrt::impl
{
    template <typename D> auto consume_Windows_Graphics_DirectX_Direct3D11_IDirect3DDevice<D>::Trim() const
    {
        if constexpr (!std::is_same_v<D, winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->Trim());
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice>**)this;
            check_hresult(_winrt_abi_type->Trim());
        }
    }
    template <typename D> auto consume_Windows_Graphics_DirectX_Direct3D11_IDirect3DSurface<D>::Description() const
    {
        winrt::Windows::Graphics::DirectX::Direct3D11::Direct3DSurfaceDescription value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_Description(put_abi(value)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface>**)this;
            check_hresult(_winrt_abi_type->get_Description(put_abi(value)));
        }
        return value;
    }
    template <typename D>
    struct produce<D, winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice> : produce_base<D, winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice>
    {
        int32_t __stdcall Trim() noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().Trim();
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
    template <typename D>
    struct produce<D, winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface> : produce_base<D, winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface>
    {
        int32_t __stdcall get_Description(struct struct_Windows_Graphics_DirectX_Direct3D11_Direct3DSurfaceDescription* value) noexcept final try
        {
            zero_abi<winrt::Windows::Graphics::DirectX::Direct3D11::Direct3DSurfaceDescription>(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Windows::Graphics::DirectX::Direct3D11::Direct3DSurfaceDescription>(this->shim().Description());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
}
WINRT_EXPORT namespace winrt::Windows::Graphics::DirectX::Direct3D11
{
    constexpr auto operator|(Direct3DBindings const left, Direct3DBindings const right) noexcept
    {
        return static_cast<Direct3DBindings>(impl::to_underlying_type(left) | impl::to_underlying_type(right));
    }
    constexpr auto operator|=(Direct3DBindings& left, Direct3DBindings const right) noexcept
    {
        left = left | right;
        return left;
    }
    constexpr auto operator&(Direct3DBindings const left, Direct3DBindings const right) noexcept
    {
        return static_cast<Direct3DBindings>(impl::to_underlying_type(left) & impl::to_underlying_type(right));
    }
    constexpr auto operator&=(Direct3DBindings& left, Direct3DBindings const right) noexcept
    {
        left = left & right;
        return left;
    }
    constexpr auto operator~(Direct3DBindings const value) noexcept
    {
        return static_cast<Direct3DBindings>(~impl::to_underlying_type(value));
    }
    constexpr auto operator^(Direct3DBindings const left, Direct3DBindings const right) noexcept
    {
        return static_cast<Direct3DBindings>(impl::to_underlying_type(left) ^ impl::to_underlying_type(right));
    }
    constexpr auto operator^=(Direct3DBindings& left, Direct3DBindings const right) noexcept
    {
        left = left ^ right;
        return left;
    }
}
namespace std
{
#ifndef WINRT_LEAN_AND_MEAN
    template<> struct hash<winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface> : winrt::impl::hash_base {};
#endif
#ifdef __cpp_lib_format
#endif
}
#endif
