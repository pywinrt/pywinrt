// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#include <winrt/Windows.Foundation.h>

#include <winrt/Windows.Phone.System.Power.h>

namespace py::proj::Windows::Phone::System::Power
{}

namespace py::impl::Windows::Phone::System::Power
{}

namespace py::wrapper::Windows::Phone::System::Power
{
    using PowerManager = py::winrt_wrapper<winrt::Windows::Phone::System::Power::PowerManager>;
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Phone::System::Power::PowerSavingMode> = "i";


    template<>
    struct py_type<winrt::Windows::Phone::System::Power::PowerSavingMode>
    {
        static constexpr const char* module_name = "winrt.windows.phone.system.power";
        static constexpr const char* type_name = "PowerSavingMode";
    };

    template<>
    struct py_type<winrt::Windows::Phone::System::Power::PowerManager>
    {
        static constexpr const char* module_name = "winrt.windows.phone.system.power";
        static constexpr const char* type_name = "PowerManager";
    };
}