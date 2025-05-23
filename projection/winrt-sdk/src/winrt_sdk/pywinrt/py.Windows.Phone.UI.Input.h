// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Foundation.h>

#include <winrt/Windows.Phone.UI.Input.h>

namespace py::proj::Windows::Phone::UI::Input
{
}

namespace py
{

    template<>
    struct py_type<winrt::Windows::Phone::UI::Input::BackPressedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.phone.ui.input.BackPressedEventArgs";
        static constexpr const char* module_name = "winrt.windows.phone.ui.input";
        static constexpr const char* type_name = "BackPressedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Phone::UI::Input::CameraEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.phone.ui.input.CameraEventArgs";
        static constexpr const char* module_name = "winrt.windows.phone.ui.input";
        static constexpr const char* type_name = "CameraEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Phone::UI::Input::HardwareButtons>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.phone.ui.input.HardwareButtons";
        static constexpr const char* module_name = "winrt.windows.phone.ui.input";
        static constexpr const char* type_name = "HardwareButtons";
    };
}

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

namespace py::impl::Windows::Phone::UI::Input
{
}

namespace py::wrapper::Windows::Phone::UI::Input
{
    using BackPressedEventArgs = py::winrt_wrapper<winrt::Windows::Phone::UI::Input::BackPressedEventArgs>;
    using CameraEventArgs = py::winrt_wrapper<winrt::Windows::Phone::UI::Input::CameraEventArgs>;
    using HardwareButtons = py::winrt_wrapper<winrt::Windows::Phone::UI::Input::HardwareButtons>;
}

namespace py
{
}
