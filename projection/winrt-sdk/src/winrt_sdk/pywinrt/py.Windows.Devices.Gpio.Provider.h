// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>

#include <winrt/Windows.Devices.Gpio.Provider.h>

namespace py::proj::Windows::Devices::Gpio::Provider
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Gpio::Provider::ProviderGpioPinDriveMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Gpio::Provider::ProviderGpioPinEdge> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Gpio::Provider::ProviderGpioPinValue> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Gpio::Provider::ProviderGpioSharingMode> = "i";


    template<>
    struct py_type<winrt::Windows::Devices::Gpio::Provider::ProviderGpioPinDriveMode>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.gpio.provider.ProviderGpioPinDriveMode";
        static constexpr const char* module_name = "winrt.windows.devices.gpio.provider";
        static constexpr const char* type_name = "ProviderGpioPinDriveMode";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Gpio::Provider::ProviderGpioPinEdge>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.gpio.provider.ProviderGpioPinEdge";
        static constexpr const char* module_name = "winrt.windows.devices.gpio.provider";
        static constexpr const char* type_name = "ProviderGpioPinEdge";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Gpio::Provider::ProviderGpioPinValue>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.gpio.provider.ProviderGpioPinValue";
        static constexpr const char* module_name = "winrt.windows.devices.gpio.provider";
        static constexpr const char* type_name = "ProviderGpioPinValue";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Gpio::Provider::ProviderGpioSharingMode>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.gpio.provider.ProviderGpioSharingMode";
        static constexpr const char* module_name = "winrt.windows.devices.gpio.provider";
        static constexpr const char* type_name = "ProviderGpioSharingMode";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Gpio::Provider::GpioPinProviderValueChangedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.gpio.provider.GpioPinProviderValueChangedEventArgs";
        static constexpr const char* module_name = "winrt.windows.devices.gpio.provider";
        static constexpr const char* type_name = "GpioPinProviderValueChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Gpio::Provider::IGpioControllerProvider>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.gpio.provider._IGpioControllerProvider";
        static constexpr const char* module_name = "winrt.windows.devices.gpio.provider";
        static constexpr const char* type_name = "_IGpioControllerProvider";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Gpio::Provider::IGpioPinProvider>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.gpio.provider._IGpioPinProvider";
        static constexpr const char* module_name = "winrt.windows.devices.gpio.provider";
        static constexpr const char* type_name = "_IGpioPinProvider";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Gpio::Provider::IGpioProvider>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.gpio.provider._IGpioProvider";
        static constexpr const char* module_name = "winrt.windows.devices.gpio.provider";
        static constexpr const char* type_name = "_IGpioProvider";
    };
}

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

namespace py::impl::Windows::Devices::Gpio::Provider
{
}

namespace py::wrapper::Windows::Devices::Gpio::Provider
{
    using GpioPinProviderValueChangedEventArgs = py::winrt_wrapper<winrt::Windows::Devices::Gpio::Provider::GpioPinProviderValueChangedEventArgs>;
    using IGpioControllerProvider = py::winrt_wrapper<winrt::Windows::Devices::Gpio::Provider::IGpioControllerProvider>;
    using IGpioPinProvider = py::winrt_wrapper<winrt::Windows::Devices::Gpio::Provider::IGpioPinProvider>;
    using IGpioProvider = py::winrt_wrapper<winrt::Windows::Devices::Gpio::Provider::IGpioProvider>;
}

namespace py
{
}
