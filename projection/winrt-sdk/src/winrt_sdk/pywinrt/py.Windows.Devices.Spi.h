// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Devices.Spi.Provider.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>

#include <winrt/Windows.Devices.Spi.h>

namespace py::proj::Windows::Devices::Spi
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Spi::SpiMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Spi::SpiSharingMode> = "i";


    template<>
    struct py_type<winrt::Windows::Devices::Spi::SpiMode>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.spi.SpiMode";
        static constexpr const char* module_name = "winrt.windows.devices.spi";
        static constexpr const char* type_name = "SpiMode";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Spi::SpiSharingMode>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.spi.SpiSharingMode";
        static constexpr const char* module_name = "winrt.windows.devices.spi";
        static constexpr const char* type_name = "SpiSharingMode";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Spi::SpiBusInfo>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.spi.SpiBusInfo";
        static constexpr const char* module_name = "winrt.windows.devices.spi";
        static constexpr const char* type_name = "SpiBusInfo";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Spi::SpiConnectionSettings>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.spi.SpiConnectionSettings";
        static constexpr const char* module_name = "winrt.windows.devices.spi";
        static constexpr const char* type_name = "SpiConnectionSettings";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Spi::SpiController>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.spi.SpiController";
        static constexpr const char* module_name = "winrt.windows.devices.spi";
        static constexpr const char* type_name = "SpiController";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Spi::SpiDevice>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.spi.SpiDevice";
        static constexpr const char* module_name = "winrt.windows.devices.spi";
        static constexpr const char* type_name = "SpiDevice";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Spi::ISpiDeviceStatics>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.spi._ISpiDeviceStatics";
        static constexpr const char* module_name = "winrt.windows.devices.spi";
        static constexpr const char* type_name = "_ISpiDeviceStatics";
    };
}

#if __has_include("py.Windows.Devices.Spi.Provider.h")
#include "py.Windows.Devices.Spi.Provider.h"
#endif

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

namespace py::impl::Windows::Devices::Spi
{
}

namespace py::wrapper::Windows::Devices::Spi
{
    using SpiBusInfo = py::winrt_wrapper<winrt::Windows::Devices::Spi::SpiBusInfo>;
    using SpiConnectionSettings = py::winrt_wrapper<winrt::Windows::Devices::Spi::SpiConnectionSettings>;
    using SpiController = py::winrt_wrapper<winrt::Windows::Devices::Spi::SpiController>;
    using SpiDevice = py::winrt_wrapper<winrt::Windows::Devices::Spi::SpiDevice>;
    using ISpiDeviceStatics = py::winrt_wrapper<winrt::Windows::Devices::Spi::ISpiDeviceStatics>;
}

namespace py
{
}
