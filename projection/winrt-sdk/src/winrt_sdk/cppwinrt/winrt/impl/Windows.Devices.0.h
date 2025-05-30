// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Devices_0_H
#define WINRT_Windows_Devices_0_H
WINRT_EXPORT namespace winrt::Windows::Devices::Adc::Provider
{
    struct IAdcControllerProvider;
}
WINRT_EXPORT namespace winrt::Windows::Devices::Gpio::Provider
{
    struct IGpioControllerProvider;
}
WINRT_EXPORT namespace winrt::Windows::Devices::I2c::Provider
{
    struct II2cControllerProvider;
}
WINRT_EXPORT namespace winrt::Windows::Devices::Pwm::Provider
{
    struct IPwmControllerProvider;
}
WINRT_EXPORT namespace winrt::Windows::Devices::Spi::Provider
{
    struct ISpiControllerProvider;
}
WINRT_EXPORT namespace winrt::Windows::Devices
{
    struct ILowLevelDevicesAggregateProvider;
    struct ILowLevelDevicesAggregateProviderFactory;
    struct ILowLevelDevicesController;
    struct ILowLevelDevicesControllerStatics;
    struct LowLevelDevicesAggregateProvider;
    struct LowLevelDevicesController;
    struct DevicesLowLevelContract;
}
namespace winrt::impl
{
    template <> struct category<winrt::Windows::Devices::ILowLevelDevicesAggregateProvider>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Devices::ILowLevelDevicesAggregateProviderFactory>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Devices::ILowLevelDevicesController>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Devices::ILowLevelDevicesControllerStatics>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Devices::LowLevelDevicesAggregateProvider>{ using type = class_category; };
    template <> struct category<winrt::Windows::Devices::LowLevelDevicesController>{ using type = class_category; };
    template <> inline constexpr auto& name_v<winrt::Windows::Devices::LowLevelDevicesAggregateProvider> = L"Windows.Devices.LowLevelDevicesAggregateProvider";
    template <> inline constexpr auto& name_v<winrt::Windows::Devices::LowLevelDevicesController> = L"Windows.Devices.LowLevelDevicesController";
    template <> inline constexpr auto& name_v<winrt::Windows::Devices::ILowLevelDevicesAggregateProvider> = L"Windows.Devices.ILowLevelDevicesAggregateProvider";
    template <> inline constexpr auto& name_v<winrt::Windows::Devices::ILowLevelDevicesAggregateProviderFactory> = L"Windows.Devices.ILowLevelDevicesAggregateProviderFactory";
    template <> inline constexpr auto& name_v<winrt::Windows::Devices::ILowLevelDevicesController> = L"Windows.Devices.ILowLevelDevicesController";
    template <> inline constexpr auto& name_v<winrt::Windows::Devices::ILowLevelDevicesControllerStatics> = L"Windows.Devices.ILowLevelDevicesControllerStatics";
    template <> inline constexpr auto& name_v<winrt::Windows::Devices::DevicesLowLevelContract> = L"Windows.Devices.DevicesLowLevelContract";
    template <> inline constexpr guid guid_v<winrt::Windows::Devices::ILowLevelDevicesAggregateProvider>{ 0xA73E561C,0xAAC1,0x4EC7,{ 0xA8,0x52,0x47,0x9F,0x70,0x60,0xD0,0x1F } }; // A73E561C-AAC1-4EC7-A852-479F7060D01F
    template <> inline constexpr guid guid_v<winrt::Windows::Devices::ILowLevelDevicesAggregateProviderFactory>{ 0x9AC4AAF6,0x3473,0x465E,{ 0x96,0xD5,0x36,0x28,0x1A,0x2C,0x57,0xAF } }; // 9AC4AAF6-3473-465E-96D5-36281A2C57AF
    template <> inline constexpr guid guid_v<winrt::Windows::Devices::ILowLevelDevicesController>{ 0x2EC23DD4,0x179B,0x45DE,{ 0x9B,0x39,0x3A,0xE0,0x25,0x27,0xDE,0x52 } }; // 2EC23DD4-179B-45DE-9B39-3AE02527DE52
    template <> inline constexpr guid guid_v<winrt::Windows::Devices::ILowLevelDevicesControllerStatics>{ 0x093E926A,0xFCCB,0x4394,{ 0xA6,0x97,0x19,0xDE,0x63,0x7C,0x2D,0xB3 } }; // 093E926A-FCCB-4394-A697-19DE637C2DB3
    template <> struct default_interface<winrt::Windows::Devices::LowLevelDevicesAggregateProvider>{ using type = winrt::Windows::Devices::ILowLevelDevicesAggregateProvider; };
    template <> struct default_interface<winrt::Windows::Devices::LowLevelDevicesController>{ using type = winrt::Windows::Devices::ILowLevelDevicesController; };
    template <> struct abi<winrt::Windows::Devices::ILowLevelDevicesAggregateProvider>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_AdcControllerProvider(void**) noexcept = 0;
            virtual int32_t __stdcall get_PwmControllerProvider(void**) noexcept = 0;
            virtual int32_t __stdcall get_GpioControllerProvider(void**) noexcept = 0;
            virtual int32_t __stdcall get_I2cControllerProvider(void**) noexcept = 0;
            virtual int32_t __stdcall get_SpiControllerProvider(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Devices::ILowLevelDevicesAggregateProviderFactory>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall Create(void*, void*, void*, void*, void*, void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Devices::ILowLevelDevicesController>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
        };
    };
    template <> struct abi<winrt::Windows::Devices::ILowLevelDevicesControllerStatics>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_DefaultProvider(void**) noexcept = 0;
            virtual int32_t __stdcall put_DefaultProvider(void*) noexcept = 0;
        };
    };
    template <typename D>
    struct consume_Windows_Devices_ILowLevelDevicesAggregateProvider
    {
        [[nodiscard]] auto AdcControllerProvider() const;
        [[nodiscard]] auto PwmControllerProvider() const;
        [[nodiscard]] auto GpioControllerProvider() const;
        [[nodiscard]] auto I2cControllerProvider() const;
        [[nodiscard]] auto SpiControllerProvider() const;
    };
    template <> struct consume<winrt::Windows::Devices::ILowLevelDevicesAggregateProvider>
    {
        template <typename D> using type = consume_Windows_Devices_ILowLevelDevicesAggregateProvider<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_ILowLevelDevicesAggregateProviderFactory
    {
        auto Create(winrt::Windows::Devices::Adc::Provider::IAdcControllerProvider const& adc, winrt::Windows::Devices::Pwm::Provider::IPwmControllerProvider const& pwm, winrt::Windows::Devices::Gpio::Provider::IGpioControllerProvider const& gpio, winrt::Windows::Devices::I2c::Provider::II2cControllerProvider const& i2c, winrt::Windows::Devices::Spi::Provider::ISpiControllerProvider const& spi) const;
    };
    template <> struct consume<winrt::Windows::Devices::ILowLevelDevicesAggregateProviderFactory>
    {
        template <typename D> using type = consume_Windows_Devices_ILowLevelDevicesAggregateProviderFactory<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_ILowLevelDevicesController
    {
    };
    template <> struct consume<winrt::Windows::Devices::ILowLevelDevicesController>
    {
        template <typename D> using type = consume_Windows_Devices_ILowLevelDevicesController<D>;
    };
    template <typename D>
    struct consume_Windows_Devices_ILowLevelDevicesControllerStatics
    {
        [[nodiscard]] auto DefaultProvider() const;
        auto DefaultProvider(winrt::Windows::Devices::ILowLevelDevicesAggregateProvider const& value) const;
    };
    template <> struct consume<winrt::Windows::Devices::ILowLevelDevicesControllerStatics>
    {
        template <typename D> using type = consume_Windows_Devices_ILowLevelDevicesControllerStatics<D>;
    };
}
#endif
