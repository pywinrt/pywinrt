// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");

#if __has_include("py.Microsoft.UI.Composition.h")
#include "py.Microsoft.UI.Composition.h"
#endif

#if __has_include("py.Microsoft.UI.Xaml.Controls.h")
#include "py.Microsoft.UI.Xaml.Controls.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.UI.h")
#include "py.Windows.UI.h"
#endif

#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.h>

#include <winrt/Microsoft.UI.Xaml.Controls.AnimatedVisuals.h>

namespace py::proj::Microsoft::UI::Xaml::Controls::AnimatedVisuals
{}

namespace py::impl::Microsoft::UI::Xaml::Controls::AnimatedVisuals
{}

namespace py::wrapper::Microsoft::UI::Xaml::Controls::AnimatedVisuals
{
    using AnimatedAcceptVisualSource = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Controls::AnimatedVisuals::AnimatedAcceptVisualSource>;
    using AnimatedBackVisualSource = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Controls::AnimatedVisuals::AnimatedBackVisualSource>;
    using AnimatedChevronDownSmallVisualSource = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Controls::AnimatedVisuals::AnimatedChevronDownSmallVisualSource>;
    using AnimatedChevronRightDownSmallVisualSource = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Controls::AnimatedVisuals::AnimatedChevronRightDownSmallVisualSource>;
    using AnimatedChevronUpDownSmallVisualSource = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Controls::AnimatedVisuals::AnimatedChevronUpDownSmallVisualSource>;
    using AnimatedFindVisualSource = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Controls::AnimatedVisuals::AnimatedFindVisualSource>;
    using AnimatedGlobalNavigationButtonVisualSource = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Controls::AnimatedVisuals::AnimatedGlobalNavigationButtonVisualSource>;
    using AnimatedSettingsVisualSource = py::winrt_wrapper<winrt::Microsoft::UI::Xaml::Controls::AnimatedVisuals::AnimatedSettingsVisualSource>;
}

namespace py
{

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Controls::AnimatedVisuals::AnimatedAcceptVisualSource>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.controls.animatedvisuals";
        static constexpr const char* type_name = "AnimatedAcceptVisualSource";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Controls::AnimatedVisuals::AnimatedBackVisualSource>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.controls.animatedvisuals";
        static constexpr const char* type_name = "AnimatedBackVisualSource";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Controls::AnimatedVisuals::AnimatedChevronDownSmallVisualSource>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.controls.animatedvisuals";
        static constexpr const char* type_name = "AnimatedChevronDownSmallVisualSource";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Controls::AnimatedVisuals::AnimatedChevronRightDownSmallVisualSource>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.controls.animatedvisuals";
        static constexpr const char* type_name = "AnimatedChevronRightDownSmallVisualSource";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Controls::AnimatedVisuals::AnimatedChevronUpDownSmallVisualSource>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.controls.animatedvisuals";
        static constexpr const char* type_name = "AnimatedChevronUpDownSmallVisualSource";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Controls::AnimatedVisuals::AnimatedFindVisualSource>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.controls.animatedvisuals";
        static constexpr const char* type_name = "AnimatedFindVisualSource";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Controls::AnimatedVisuals::AnimatedGlobalNavigationButtonVisualSource>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.controls.animatedvisuals";
        static constexpr const char* type_name = "AnimatedGlobalNavigationButtonVisualSource";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Xaml::Controls::AnimatedVisuals::AnimatedSettingsVisualSource>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.xaml.controls.animatedvisuals";
        static constexpr const char* type_name = "AnimatedSettingsVisualSource";
    };
}