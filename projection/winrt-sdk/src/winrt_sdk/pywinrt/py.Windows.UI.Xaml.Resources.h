// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");

#include <winrt/Windows.UI.Xaml.Resources.h>

namespace py::proj::Windows::UI::Xaml::Resources
{
}

namespace py
{

    template<>
    struct py_type<winrt::Windows::UI::Xaml::Resources::CustomXamlResourceLoader>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.ui.xaml.resources.CustomXamlResourceLoader";
        static constexpr const char* module_name = "winrt.windows.ui.xaml.resources";
        static constexpr const char* type_name = "CustomXamlResourceLoader";
    };
}

namespace py::impl::Windows::UI::Xaml::Resources
{
}

namespace py::wrapper::Windows::UI::Xaml::Resources
{
    using CustomXamlResourceLoader = py::winrt_wrapper<winrt::Windows::UI::Xaml::Resources::CustomXamlResourceLoader>;
}

namespace py
{
}
