// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_UI_Xaml_Resources_2_H
#define WINRT_Windows_UI_Xaml_Resources_2_H
#include "winrt/impl/Windows.UI.Xaml.Resources.1.h"
WINRT_EXPORT namespace winrt::Windows::UI::Xaml::Resources
{
    struct WINRT_IMPL_EMPTY_BASES CustomXamlResourceLoader : winrt::Windows::UI::Xaml::Resources::ICustomXamlResourceLoader
    {
        CustomXamlResourceLoader(std::nullptr_t) noexcept {}
        CustomXamlResourceLoader(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::UI::Xaml::Resources::ICustomXamlResourceLoader(ptr, take_ownership_from_abi) {}
        CustomXamlResourceLoader();
        [[nodiscard]] static auto Current();
        static auto Current(winrt::Windows::UI::Xaml::Resources::CustomXamlResourceLoader const& value);
    };
    template <typename D>
    class ICustomXamlResourceLoaderOverridesT
    {
        D& shim() noexcept { return *static_cast<D*>(this); }
        D const& shim() const noexcept { return *static_cast<const D*>(this); }
    public:
        using ICustomXamlResourceLoaderOverrides = winrt::Windows::UI::Xaml::Resources::ICustomXamlResourceLoaderOverrides;
        auto GetResource(param::hstring const& resourceId, param::hstring const& objectType, param::hstring const& propertyName, param::hstring const& propertyType) const;
    };
}
#endif
