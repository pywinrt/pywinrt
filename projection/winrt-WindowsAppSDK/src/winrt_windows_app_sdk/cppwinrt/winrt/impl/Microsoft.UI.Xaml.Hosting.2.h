// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Microsoft_UI_Xaml_Hosting_2_H
#define WINRT_Microsoft_UI_Xaml_Hosting_2_H
#include "winrt/impl/Microsoft.UI.Composition.1.h"
#include "winrt/impl/Microsoft.UI.Xaml.1.h"
#include "winrt/impl/Microsoft.UI.Xaml.Controls.1.h"
#include "winrt/impl/Windows.Foundation.1.h"
#include "winrt/impl/Microsoft.UI.Xaml.Hosting.1.h"
WINRT_EXPORT namespace winrt::Microsoft::UI::Xaml::Hosting
{
    struct WINRT_IMPL_EMPTY_BASES DesktopWindowXamlSource : winrt::Microsoft::UI::Xaml::Hosting::IDesktopWindowXamlSource,
        impl::require<DesktopWindowXamlSource, winrt::Microsoft::UI::Xaml::Hosting::IDesktopWindowXamlSource2, winrt::Windows::Foundation::IClosable>
    {
        DesktopWindowXamlSource(std::nullptr_t) noexcept {}
        DesktopWindowXamlSource(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Xaml::Hosting::IDesktopWindowXamlSource(ptr, take_ownership_from_abi) {}
        DesktopWindowXamlSource();
    };
    struct WINRT_IMPL_EMPTY_BASES DesktopWindowXamlSourceGotFocusEventArgs : winrt::Microsoft::UI::Xaml::Hosting::IDesktopWindowXamlSourceGotFocusEventArgs
    {
        DesktopWindowXamlSourceGotFocusEventArgs(std::nullptr_t) noexcept {}
        DesktopWindowXamlSourceGotFocusEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Xaml::Hosting::IDesktopWindowXamlSourceGotFocusEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES DesktopWindowXamlSourceTakeFocusRequestedEventArgs : winrt::Microsoft::UI::Xaml::Hosting::IDesktopWindowXamlSourceTakeFocusRequestedEventArgs
    {
        DesktopWindowXamlSourceTakeFocusRequestedEventArgs(std::nullptr_t) noexcept {}
        DesktopWindowXamlSourceTakeFocusRequestedEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Xaml::Hosting::IDesktopWindowXamlSourceTakeFocusRequestedEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES ElementCompositionPreview : winrt::Microsoft::UI::Xaml::Hosting::IElementCompositionPreview
    {
        ElementCompositionPreview(std::nullptr_t) noexcept {}
        ElementCompositionPreview(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Xaml::Hosting::IElementCompositionPreview(ptr, take_ownership_from_abi) {}
        static auto GetElementVisual(winrt::Microsoft::UI::Xaml::UIElement const& element);
        static auto GetElementChildVisual(winrt::Microsoft::UI::Xaml::UIElement const& element);
        static auto SetElementChildVisual(winrt::Microsoft::UI::Xaml::UIElement const& element, winrt::Microsoft::UI::Composition::Visual const& visual);
        static auto GetScrollViewerManipulationPropertySet(winrt::Microsoft::UI::Xaml::Controls::ScrollViewer const& scrollViewer);
        static auto SetImplicitShowAnimation(winrt::Microsoft::UI::Xaml::UIElement const& element, winrt::Microsoft::UI::Composition::ICompositionAnimationBase const& animation);
        static auto SetImplicitHideAnimation(winrt::Microsoft::UI::Xaml::UIElement const& element, winrt::Microsoft::UI::Composition::ICompositionAnimationBase const& animation);
        static auto SetIsTranslationEnabled(winrt::Microsoft::UI::Xaml::UIElement const& element, bool value);
        static auto GetPointerPositionPropertySet(winrt::Microsoft::UI::Xaml::UIElement const& targetElement);
    };
    struct WINRT_IMPL_EMPTY_BASES WindowsXamlManager : winrt::Microsoft::UI::Xaml::Hosting::IWindowsXamlManager,
        impl::require<WindowsXamlManager, winrt::Microsoft::UI::Xaml::Hosting::IWindowsXamlManager2, winrt::Windows::Foundation::IClosable>
    {
        WindowsXamlManager(std::nullptr_t) noexcept {}
        WindowsXamlManager(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Xaml::Hosting::IWindowsXamlManager(ptr, take_ownership_from_abi) {}
        static auto InitializeForCurrentThread();
        static auto GetForCurrentThread();
    };
    struct WINRT_IMPL_EMPTY_BASES XamlShutdownCompletedOnThreadEventArgs : winrt::Microsoft::UI::Xaml::Hosting::IXamlShutdownCompletedOnThreadEventArgs
    {
        XamlShutdownCompletedOnThreadEventArgs(std::nullptr_t) noexcept {}
        XamlShutdownCompletedOnThreadEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Xaml::Hosting::IXamlShutdownCompletedOnThreadEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES XamlSourceFocusNavigationRequest : winrt::Microsoft::UI::Xaml::Hosting::IXamlSourceFocusNavigationRequest
    {
        XamlSourceFocusNavigationRequest(std::nullptr_t) noexcept {}
        XamlSourceFocusNavigationRequest(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Xaml::Hosting::IXamlSourceFocusNavigationRequest(ptr, take_ownership_from_abi) {}
        explicit XamlSourceFocusNavigationRequest(winrt::Microsoft::UI::Xaml::Hosting::XamlSourceFocusNavigationReason const& reason);
        XamlSourceFocusNavigationRequest(winrt::Microsoft::UI::Xaml::Hosting::XamlSourceFocusNavigationReason const& reason, winrt::Windows::Foundation::Rect const& hintRect);
        XamlSourceFocusNavigationRequest(winrt::Microsoft::UI::Xaml::Hosting::XamlSourceFocusNavigationReason const& reason, winrt::Windows::Foundation::Rect const& hintRect, winrt::guid const& correlationId);
    };
    struct WINRT_IMPL_EMPTY_BASES XamlSourceFocusNavigationResult : winrt::Microsoft::UI::Xaml::Hosting::IXamlSourceFocusNavigationResult
    {
        XamlSourceFocusNavigationResult(std::nullptr_t) noexcept {}
        XamlSourceFocusNavigationResult(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Xaml::Hosting::IXamlSourceFocusNavigationResult(ptr, take_ownership_from_abi) {}
        explicit XamlSourceFocusNavigationResult(bool focusMoved);
    };
}
#endif
