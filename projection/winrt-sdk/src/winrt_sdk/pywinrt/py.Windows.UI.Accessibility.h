// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Foundation.h>

#include <winrt/Windows.UI.Accessibility.h>

namespace py::proj::Windows::UI::Accessibility
{
}

namespace py
{

    template<>
    struct py_type<winrt::Windows::UI::Accessibility::ScreenReaderPositionChangedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.accessibility.ScreenReaderPositionChangedEventArgs";
        static constexpr const char* module_name = "winrt.windows.ui.accessibility";
        static constexpr const char* type_name = "ScreenReaderPositionChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::UI::Accessibility::ScreenReaderService>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.accessibility.ScreenReaderService";
        static constexpr const char* module_name = "winrt.windows.ui.accessibility";
        static constexpr const char* type_name = "ScreenReaderService";
    };
}

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

namespace py::impl::Windows::UI::Accessibility
{
}

namespace py::wrapper::Windows::UI::Accessibility
{
    using ScreenReaderPositionChangedEventArgs = py::winrt_wrapper<winrt::Windows::UI::Accessibility::ScreenReaderPositionChangedEventArgs>;
    using ScreenReaderService = py::winrt_wrapper<winrt::Windows::UI::Accessibility::ScreenReaderService>;
}

namespace py
{
}
