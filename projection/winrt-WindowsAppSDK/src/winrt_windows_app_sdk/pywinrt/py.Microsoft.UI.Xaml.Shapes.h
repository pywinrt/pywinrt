// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>

#include <winrt/Microsoft.UI.Xaml.Shapes.h>

namespace py::proj::Microsoft::UI::Xaml::Shapes
{
}

namespace py
{

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Shapes::Ellipse>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.xaml.shapes.Ellipse";
        static constexpr const char* module_name = "winui3.microsoft.ui.xaml.shapes";
        static constexpr const char* type_name = "Ellipse";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Shapes::Line>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.xaml.shapes.Line";
        static constexpr const char* module_name = "winui3.microsoft.ui.xaml.shapes";
        static constexpr const char* type_name = "Line";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Shapes::Path>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.xaml.shapes.Path";
        static constexpr const char* module_name = "winui3.microsoft.ui.xaml.shapes";
        static constexpr const char* type_name = "Path";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Shapes::Polygon>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.xaml.shapes.Polygon";
        static constexpr const char* module_name = "winui3.microsoft.ui.xaml.shapes";
        static constexpr const char* type_name = "Polygon";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Shapes::Polyline>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.xaml.shapes.Polyline";
        static constexpr const char* module_name = "winui3.microsoft.ui.xaml.shapes";
        static constexpr const char* type_name = "Polyline";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Shapes::Rectangle>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.xaml.shapes.Rectangle";
        static constexpr const char* module_name = "winui3.microsoft.ui.xaml.shapes";
        static constexpr const char* type_name = "Rectangle";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Shapes::Shape>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.ui.xaml.shapes.Shape";
        static constexpr const char* module_name = "winui3.microsoft.ui.xaml.shapes";
        static constexpr const char* type_name = "Shape";
    };
}

#if __has_include("py.Microsoft.UI.Composition.h")
#include "py.Microsoft.UI.Composition.h"
#endif

#if __has_include("py.Microsoft.UI.Xaml.h")
#include "py.Microsoft.UI.Xaml.h"
#endif

#if __has_include("py.Microsoft.UI.Xaml.Media.h")
#include "py.Microsoft.UI.Xaml.Media.h"
#endif

namespace py::impl::Microsoft::UI::Xaml::Shapes
{
}

namespace py::wrapper::Microsoft::UI::Xaml::Shapes
{
    using Ellipse = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Shapes::Ellipse>;
    using Line = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Shapes::Line>;
    using Path = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Shapes::Path>;
    using Polygon = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Shapes::Polygon>;
    using Polyline = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Shapes::Polyline>;
    using Rectangle = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Shapes::Rectangle>;
    using Shape = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Shapes::Shape>;
}

namespace py
{
}
