// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");

#include <winrt/Microsoft.UI.Xaml.Automation.Text.h>

namespace py::proj::Microsoft::UI::Xaml::Automation::Text
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Xaml::Automation::Text::TextPatternRangeEndpoint> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Xaml::Automation::Text::TextUnit> = "i";


    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Automation::Text::TextPatternRangeEndpoint>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.xaml.automation.text.TextPatternRangeEndpoint";
        static constexpr const char* module_name = "winui3.microsoft.ui.xaml.automation.text";
        static constexpr const char* type_name = "TextPatternRangeEndpoint";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Automation::Text::TextUnit>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.xaml.automation.text.TextUnit";
        static constexpr const char* module_name = "winui3.microsoft.ui.xaml.automation.text";
        static constexpr const char* type_name = "TextUnit";
    };
}

namespace py::impl::Microsoft::UI::Xaml::Automation::Text
{
}

namespace py::wrapper::Microsoft::UI::Xaml::Automation::Text
{
}

namespace py
{
}
