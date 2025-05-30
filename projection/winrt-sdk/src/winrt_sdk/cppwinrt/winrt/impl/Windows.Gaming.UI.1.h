// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Gaming_UI_1_H
#define WINRT_Windows_Gaming_UI_1_H
#include "winrt/impl/Windows.ApplicationModel.Activation.0.h"
#include "winrt/impl/Windows.Gaming.UI.0.h"
WINRT_EXPORT namespace winrt::Windows::Gaming::UI
{
    struct WINRT_IMPL_EMPTY_BASES IGameBarStatics :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IGameBarStatics>
    {
        IGameBarStatics(std::nullptr_t = nullptr) noexcept {}
        IGameBarStatics(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IGameChatMessageReceivedEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IGameChatMessageReceivedEventArgs>
    {
        IGameChatMessageReceivedEventArgs(std::nullptr_t = nullptr) noexcept {}
        IGameChatMessageReceivedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IGameChatOverlay :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IGameChatOverlay>
    {
        IGameChatOverlay(std::nullptr_t = nullptr) noexcept {}
        IGameChatOverlay(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IGameChatOverlayMessageSource :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IGameChatOverlayMessageSource>
    {
        IGameChatOverlayMessageSource(std::nullptr_t = nullptr) noexcept {}
        IGameChatOverlayMessageSource(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IGameChatOverlayStatics :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IGameChatOverlayStatics>
    {
        IGameChatOverlayStatics(std::nullptr_t = nullptr) noexcept {}
        IGameChatOverlayStatics(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES IGameUIProviderActivatedEventArgs :
        winrt::Windows::Foundation::IInspectable,
        impl::consume_t<IGameUIProviderActivatedEventArgs>,
        impl::require<winrt::Windows::Gaming::UI::IGameUIProviderActivatedEventArgs, winrt::Windows::ApplicationModel::Activation::IActivatedEventArgs>
    {
        IGameUIProviderActivatedEventArgs(std::nullptr_t = nullptr) noexcept {}
        IGameUIProviderActivatedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IInspectable(ptr, take_ownership_from_abi) {}
    };
}
#endif
