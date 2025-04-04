// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Foundation.Collections.h>

#include <winrt/Microsoft.Windows.Globalization.h>

namespace py::proj::Microsoft::Windows::Globalization
{
}

namespace py
{

    template<>
    struct py_type<winrt::Microsoft::Windows::Globalization::ApplicationLanguages>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.windows.globalization.ApplicationLanguages";
        static constexpr const char* module_name = "winui3.microsoft.windows.globalization";
        static constexpr const char* type_name = "ApplicationLanguages";
    };
}

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

namespace py::impl::Microsoft::Windows::Globalization
{
}

namespace py::wrapper::Microsoft::Windows::Globalization
{
    using ApplicationLanguages = py::winrt_wrapper<winrt::Microsoft::Windows::Globalization::ApplicationLanguages>;
}

namespace py
{
}
