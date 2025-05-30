// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Management_Core_0_H
#define WINRT_Windows_Management_Core_0_H
WINRT_EXPORT namespace winrt::Windows::Storage
{
    struct ApplicationData;
}
WINRT_EXPORT namespace winrt::Windows::Management::Core
{
    struct IApplicationDataManager;
    struct IApplicationDataManagerStatics;
    struct ApplicationDataManager;
}
namespace winrt::impl
{
    template <> struct category<winrt::Windows::Management::Core::IApplicationDataManager>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Management::Core::IApplicationDataManagerStatics>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Management::Core::ApplicationDataManager>{ using type = class_category; };
    template <> inline constexpr auto& name_v<winrt::Windows::Management::Core::ApplicationDataManager> = L"Windows.Management.Core.ApplicationDataManager";
    template <> inline constexpr auto& name_v<winrt::Windows::Management::Core::IApplicationDataManager> = L"Windows.Management.Core.IApplicationDataManager";
    template <> inline constexpr auto& name_v<winrt::Windows::Management::Core::IApplicationDataManagerStatics> = L"Windows.Management.Core.IApplicationDataManagerStatics";
    template <> inline constexpr guid guid_v<winrt::Windows::Management::Core::IApplicationDataManager>{ 0x74D10432,0x2E99,0x4000,{ 0x9A,0x3A,0x64,0x30,0x7E,0x85,0x81,0x29 } }; // 74D10432-2E99-4000-9A3A-64307E858129
    template <> inline constexpr guid guid_v<winrt::Windows::Management::Core::IApplicationDataManagerStatics>{ 0x1E1862E3,0x698E,0x49A1,{ 0x97,0x52,0xDE,0xE9,0x49,0x25,0xB9,0xB3 } }; // 1E1862E3-698E-49A1-9752-DEE94925B9B3
    template <> struct default_interface<winrt::Windows::Management::Core::ApplicationDataManager>{ using type = winrt::Windows::Management::Core::IApplicationDataManager; };
    template <> struct abi<winrt::Windows::Management::Core::IApplicationDataManager>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
        };
    };
    template <> struct abi<winrt::Windows::Management::Core::IApplicationDataManagerStatics>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall CreateForPackageFamily(void*, void**) noexcept = 0;
        };
    };
    template <typename D>
    struct consume_Windows_Management_Core_IApplicationDataManager
    {
    };
    template <> struct consume<winrt::Windows::Management::Core::IApplicationDataManager>
    {
        template <typename D> using type = consume_Windows_Management_Core_IApplicationDataManager<D>;
    };
    template <typename D>
    struct consume_Windows_Management_Core_IApplicationDataManagerStatics
    {
        auto CreateForPackageFamily(param::hstring const& packageFamilyName) const;
    };
    template <> struct consume<winrt::Windows::Management::Core::IApplicationDataManagerStatics>
    {
        template <typename D> using type = consume_Windows_Management_Core_IApplicationDataManagerStatics<D>;
    };
}
#endif
