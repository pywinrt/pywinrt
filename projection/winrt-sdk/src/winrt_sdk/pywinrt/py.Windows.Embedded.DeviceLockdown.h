// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>

#include <winrt/Windows.Embedded.DeviceLockdown.h>

namespace py::proj::Windows::Embedded::DeviceLockdown
{}

namespace py::impl::Windows::Embedded::DeviceLockdown
{}

namespace py::wrapper::Windows::Embedded::DeviceLockdown
{
    using DeviceLockdownProfile = py::winrt_wrapper<winrt::Windows::Embedded::DeviceLockdown::DeviceLockdownProfile>;
    using DeviceLockdownProfileInformation = py::winrt_wrapper<winrt::Windows::Embedded::DeviceLockdown::DeviceLockdownProfileInformation>;
}

namespace py
{

    template<>
    struct py_type<winrt::Windows::Embedded::DeviceLockdown::DeviceLockdownProfile>
    {
        static constexpr const char* module_name = "winrt.windows.embedded.devicelockdown";
        static constexpr const char* type_name = "DeviceLockdownProfile";
    };

    template<>
    struct py_type<winrt::Windows::Embedded::DeviceLockdown::DeviceLockdownProfileInformation>
    {
        static constexpr const char* module_name = "winrt.windows.embedded.devicelockdown";
        static constexpr const char* type_name = "DeviceLockdownProfileInformation";
    };
}