// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Microsoft_UI_System_2_H
#define WINRT_Microsoft_UI_System_2_H
#include "winrt/impl/Microsoft.UI.1.h"
#include "winrt/impl/Microsoft.UI.System.1.h"
WINRT_EXPORT namespace winrt::Microsoft::UI::System
{
    struct WINRT_IMPL_EMPTY_BASES ThemeSettings : winrt::Microsoft::UI::System::IThemeSettings
    {
        ThemeSettings(std::nullptr_t) noexcept {}
        ThemeSettings(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::System::IThemeSettings(ptr, take_ownership_from_abi) {}
        static auto CreateForWindowId(winrt::Microsoft::UI::WindowId const& windowId);
    };
}
#endif
