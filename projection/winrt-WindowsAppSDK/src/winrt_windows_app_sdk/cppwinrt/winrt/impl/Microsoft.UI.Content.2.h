// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Microsoft_UI_Content_2_H
#define WINRT_Microsoft_UI_Content_2_H
#include "winrt/impl/Microsoft.UI.1.h"
#include "winrt/impl/Microsoft.UI.Composition.1.h"
#include "winrt/impl/Microsoft.UI.Dispatching.1.h"
#include "winrt/impl/Windows.Foundation.1.h"
#include "winrt/impl/Windows.UI.Composition.1.h"
#include "winrt/impl/Microsoft.UI.Content.1.h"
WINRT_EXPORT namespace winrt::Microsoft::UI::Content
{
    struct WINRT_IMPL_EMPTY_BASES ChildSiteLink : winrt::Microsoft::UI::Content::IChildSiteLink,
        impl::require<ChildSiteLink, winrt::Windows::Foundation::IClosable, winrt::Microsoft::UI::IClosableNotifier, winrt::Microsoft::UI::Content::IContentSiteAutomation, winrt::Microsoft::UI::Content::IContentSiteInput, winrt::Microsoft::UI::Content::IContentSiteLink>
    {
        ChildSiteLink(std::nullptr_t) noexcept {}
        ChildSiteLink(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Content::IChildSiteLink(ptr, take_ownership_from_abi) {}
        static auto Create(winrt::Microsoft::UI::Content::ContentIsland const& parent, winrt::Microsoft::UI::Composition::ContainerVisual const& placementVisual);
        static auto CreateForSystemVisual(winrt::Microsoft::UI::Content::ContentIsland const& parent, winrt::Windows::UI::Composition::ContainerVisual const& placementVisual);
    };
    struct WINRT_IMPL_EMPTY_BASES ContentCoordinateConverter : winrt::Microsoft::UI::Content::IContentCoordinateConverter
    {
        ContentCoordinateConverter(std::nullptr_t) noexcept {}
        ContentCoordinateConverter(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Content::IContentCoordinateConverter(ptr, take_ownership_from_abi) {}
        static auto CreateForWindowId(winrt::Microsoft::UI::WindowId const& windowId);
    };
    struct WINRT_IMPL_EMPTY_BASES ContentDeferral : winrt::Microsoft::UI::Content::IContentDeferral
    {
        ContentDeferral(std::nullptr_t) noexcept {}
        ContentDeferral(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Content::IContentDeferral(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ContentEnvironmentSettingChangedEventArgs : winrt::Microsoft::UI::Content::IContentEnvironmentSettingChangedEventArgs
    {
        ContentEnvironmentSettingChangedEventArgs(std::nullptr_t) noexcept {}
        ContentEnvironmentSettingChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Content::IContentEnvironmentSettingChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ContentEnvironmentStateChangedEventArgs : winrt::Microsoft::UI::Content::IContentEnvironmentStateChangedEventArgs,
        impl::require<ContentEnvironmentStateChangedEventArgs, winrt::Microsoft::UI::Content::IContentEnvironmentStateChangedEventArgs2>
    {
        ContentEnvironmentStateChangedEventArgs(std::nullptr_t) noexcept {}
        ContentEnvironmentStateChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Content::IContentEnvironmentStateChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ContentIsland : winrt::Microsoft::UI::Content::IContentIsland,
        impl::require<ContentIsland, winrt::Microsoft::UI::Content::IContentIsland2, winrt::Microsoft::UI::Content::IContentIslandAutomation, winrt::Windows::Foundation::IClosable, winrt::Microsoft::UI::IClosableNotifier, winrt::Microsoft::UI::Composition::ICompositionSupportsSystemBackdrop>
    {
        ContentIsland(std::nullptr_t) noexcept {}
        ContentIsland(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Content::IContentIsland(ptr, take_ownership_from_abi) {}
        static auto Create(winrt::Microsoft::UI::Composition::Visual const& Root);
        static auto FindAllForCompositor(winrt::Microsoft::UI::Composition::Compositor const& compositor);
        static auto FindAllForCurrentThread();
        static auto GetByVisual(winrt::Microsoft::UI::Composition::Visual const& child);
        static auto GetFromId(uint64_t id);
        static auto CreateForSystemVisual(winrt::Microsoft::UI::Dispatching::DispatcherQueue const& queue, winrt::Windows::UI::Composition::Visual const& root);
        static auto FindAllForSystemCompositor(winrt::Windows::UI::Composition::Compositor const& compositor);
        static auto GetBySystemVisual(winrt::Windows::UI::Composition::Visual const& child);
    };
    struct WINRT_IMPL_EMPTY_BASES ContentIslandAutomationProviderRequestedEventArgs : winrt::Microsoft::UI::Content::IContentIslandAutomationProviderRequestedEventArgs
    {
        ContentIslandAutomationProviderRequestedEventArgs(std::nullptr_t) noexcept {}
        ContentIslandAutomationProviderRequestedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Content::IContentIslandAutomationProviderRequestedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ContentIslandEnvironment : winrt::Microsoft::UI::Content::IContentIslandEnvironment,
        impl::require<ContentIslandEnvironment, winrt::Microsoft::UI::Content::IContentIslandEnvironment2>
    {
        ContentIslandEnvironment(std::nullptr_t) noexcept {}
        ContentIslandEnvironment(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Content::IContentIslandEnvironment(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ContentIslandStateChangedEventArgs : winrt::Microsoft::UI::Content::IContentIslandStateChangedEventArgs,
        impl::require<ContentIslandStateChangedEventArgs, winrt::Microsoft::UI::Content::IContentIslandStateChangedEventArgs2>
    {
        ContentIslandStateChangedEventArgs(std::nullptr_t) noexcept {}
        ContentIslandStateChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Content::IContentIslandStateChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ContentSite : winrt::Microsoft::UI::Content::IContentSite,
        impl::require<ContentSite, winrt::Microsoft::UI::Content::IContentSite2, winrt::Windows::Foundation::IClosable, winrt::Microsoft::UI::IClosableNotifier>
    {
        ContentSite(std::nullptr_t) noexcept {}
        ContentSite(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Content::IContentSite(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ContentSiteAutomationProviderRequestedEventArgs : winrt::Microsoft::UI::Content::IContentSiteAutomationProviderRequestedEventArgs
    {
        ContentSiteAutomationProviderRequestedEventArgs(std::nullptr_t) noexcept {}
        ContentSiteAutomationProviderRequestedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Content::IContentSiteAutomationProviderRequestedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ContentSiteEnvironment : winrt::Microsoft::UI::Content::IContentSiteEnvironment,
        impl::require<ContentSiteEnvironment, winrt::Microsoft::UI::Content::IContentSiteEnvironment2>
    {
        ContentSiteEnvironment(std::nullptr_t) noexcept {}
        ContentSiteEnvironment(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Content::IContentSiteEnvironment(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ContentSiteEnvironmentView : winrt::Microsoft::UI::Content::IContentSiteEnvironmentView,
        impl::require<ContentSiteEnvironmentView, winrt::Microsoft::UI::Content::IContentSiteEnvironmentView2>
    {
        ContentSiteEnvironmentView(std::nullptr_t) noexcept {}
        ContentSiteEnvironmentView(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Content::IContentSiteEnvironmentView(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ContentSiteRequestedStateChangedEventArgs : winrt::Microsoft::UI::Content::IContentSiteRequestedStateChangedEventArgs
    {
        ContentSiteRequestedStateChangedEventArgs(std::nullptr_t) noexcept {}
        ContentSiteRequestedStateChangedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Content::IContentSiteRequestedStateChangedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ContentSiteView : winrt::Microsoft::UI::Content::IContentSiteView,
        impl::require<ContentSiteView, winrt::Microsoft::UI::Content::IContentSiteView2, winrt::Microsoft::UI::Content::IContentSiteViewAutomation>
    {
        ContentSiteView(std::nullptr_t) noexcept {}
        ContentSiteView(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Content::IContentSiteView(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES DesktopAttachedSiteBridge : winrt::Microsoft::UI::Content::IDesktopAttachedSiteBridge,
        impl::require<DesktopAttachedSiteBridge, winrt::Windows::Foundation::IClosable, winrt::Microsoft::UI::IClosableNotifier, winrt::Microsoft::UI::Content::IContentSiteBridge, winrt::Microsoft::UI::Content::IContentSiteInput>
    {
        DesktopAttachedSiteBridge(std::nullptr_t) noexcept {}
        DesktopAttachedSiteBridge(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Content::IDesktopAttachedSiteBridge(ptr, take_ownership_from_abi) {}
        static auto CreateFromWindowId(winrt::Microsoft::UI::Dispatching::DispatcherQueue const& queue, winrt::Microsoft::UI::WindowId const& windowId);
    };
    struct WINRT_IMPL_EMPTY_BASES DesktopChildSiteBridge : winrt::Microsoft::UI::Content::IDesktopChildSiteBridge,
        impl::base<DesktopChildSiteBridge, winrt::Microsoft::UI::Content::DesktopSiteBridge>,
        impl::require<DesktopChildSiteBridge, winrt::Microsoft::UI::Content::IDesktopSiteBridge, winrt::Microsoft::UI::IClosableNotifier, winrt::Windows::Foundation::IClosable, winrt::Microsoft::UI::Content::IContentSiteBridge>
    {
        DesktopChildSiteBridge(std::nullptr_t) noexcept {}
        DesktopChildSiteBridge(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Content::IDesktopChildSiteBridge(ptr, take_ownership_from_abi) {}
        static auto Create(winrt::Microsoft::UI::Composition::Compositor const& compositor, winrt::Microsoft::UI::WindowId const& parentWindowId);
        static auto CreateWithDispatcherQueue(winrt::Microsoft::UI::Dispatching::DispatcherQueue const& queue, winrt::Microsoft::UI::WindowId const& parentWindowId);
    };
    struct WINRT_IMPL_EMPTY_BASES DesktopPopupSiteBridge : winrt::Microsoft::UI::Content::IDesktopPopupSiteBridge,
        impl::require<DesktopPopupSiteBridge, winrt::Windows::Foundation::IClosable, winrt::Microsoft::UI::IClosableNotifier, winrt::Microsoft::UI::Content::IContentSiteAutomation, winrt::Microsoft::UI::Content::IContentSiteBridge, winrt::Microsoft::UI::Content::IContentSiteLink>
    {
        DesktopPopupSiteBridge(std::nullptr_t) noexcept {}
        DesktopPopupSiteBridge(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Content::IDesktopPopupSiteBridge(ptr, take_ownership_from_abi) {}
        static auto Create(winrt::Microsoft::UI::Content::ContentIsland const& parent);
    };
    struct WINRT_IMPL_EMPTY_BASES DesktopSiteBridge : winrt::Microsoft::UI::Content::IDesktopSiteBridge,
        impl::require<DesktopSiteBridge, winrt::Microsoft::UI::IClosableNotifier, winrt::Windows::Foundation::IClosable, winrt::Microsoft::UI::Content::IContentSiteBridge>
    {
        DesktopSiteBridge(std::nullptr_t) noexcept {}
        DesktopSiteBridge(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Content::IDesktopSiteBridge(ptr, take_ownership_from_abi) {}
        static auto IsSupported();
    };
}
#endif
