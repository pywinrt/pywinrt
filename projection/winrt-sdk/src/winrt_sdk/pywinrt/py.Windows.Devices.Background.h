// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Foundation.h>

#include <winrt/Windows.Devices.Background.h>

namespace py::proj::Windows::Devices::Background
{
}

namespace py
{

    template<>
    struct py_type<winrt::Windows::Devices::Background::DeviceServicingDetails>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.background.DeviceServicingDetails";
        static constexpr const char* module_name = "winrt.windows.devices.background";
        static constexpr const char* type_name = "DeviceServicingDetails";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Background::DeviceUseDetails>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.background.DeviceUseDetails";
        static constexpr const char* module_name = "winrt.windows.devices.background";
        static constexpr const char* type_name = "DeviceUseDetails";
    };
}

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

namespace py::impl::Windows::Devices::Background
{
}

namespace py::wrapper::Windows::Devices::Background
{
    using DeviceServicingDetails = py::winrt_wrapper<winrt::Windows::Devices::Background::DeviceServicingDetails>;
    using DeviceUseDetails = py::winrt_wrapper<winrt::Windows::Devices::Background::DeviceUseDetails>;
}

namespace py
{
}
