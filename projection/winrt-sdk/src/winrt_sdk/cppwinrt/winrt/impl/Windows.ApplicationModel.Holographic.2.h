// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_ApplicationModel_Holographic_2_H
#define WINRT_Windows_ApplicationModel_Holographic_2_H
#include "winrt/impl/Windows.ApplicationModel.Holographic.1.h"
WINRT_EXPORT namespace winrt::Windows::ApplicationModel::Holographic
{
    struct WINRT_IMPL_EMPTY_BASES HolographicKeyboard : winrt::Windows::ApplicationModel::Holographic::IHolographicKeyboard
    {
        HolographicKeyboard(std::nullptr_t) noexcept {}
        HolographicKeyboard(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Holographic::IHolographicKeyboard(ptr, take_ownership_from_abi) {}
        static auto GetDefault();
    };
}
#endif
