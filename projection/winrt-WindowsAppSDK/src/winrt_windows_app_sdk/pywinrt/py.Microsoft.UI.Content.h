// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");

#if __has_include("py.Microsoft.UI.h")
#include "py.Microsoft.UI.h"
#endif

#if __has_include("py.Microsoft.UI.Composition.h")
#include "py.Microsoft.UI.Composition.h"
#endif

#if __has_include("py.Microsoft.UI.Dispatching.h")
#include "py.Microsoft.UI.Dispatching.h"
#endif

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.Foundation.Numerics.h")
#include "py.Windows.Foundation.Numerics.h"
#endif

#if __has_include("py.Windows.Graphics.h")
#include "py.Windows.Graphics.h"
#endif

#if __has_include("py.Windows.UI.Composition.h")
#include "py.Windows.UI.Composition.h"
#endif

#include <winrt/Microsoft.UI.h>
#include <winrt/Microsoft.UI.Composition.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Windows.Graphics.h>
#include <winrt/Windows.UI.Composition.h>

#include <winrt/Microsoft.UI.Content.h>

namespace py::proj::Microsoft::UI::Content
{}

namespace py::impl::Microsoft::UI::Content
{}

namespace py::wrapper::Microsoft::UI::Content
{
    using ContentCoordinateConverter = py::winrt_wrapper<winrt::Microsoft::UI::Content::ContentCoordinateConverter>;
    using ContentDeferral = py::winrt_wrapper<winrt::Microsoft::UI::Content::ContentDeferral>;
    using ContentEnvironmentSettingChangedEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Content::ContentEnvironmentSettingChangedEventArgs>;
    using ContentEnvironmentStateChangedEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Content::ContentEnvironmentStateChangedEventArgs>;
    using ContentIsland = py::winrt_wrapper<winrt::Microsoft::UI::Content::ContentIsland>;
    using ContentIslandAutomationProviderRequestedEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Content::ContentIslandAutomationProviderRequestedEventArgs>;
    using ContentIslandEnvironment = py::winrt_wrapper<winrt::Microsoft::UI::Content::ContentIslandEnvironment>;
    using ContentIslandStateChangedEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Content::ContentIslandStateChangedEventArgs>;
    using ContentSite = py::winrt_wrapper<winrt::Microsoft::UI::Content::ContentSite>;
    using ContentSiteEnvironment = py::winrt_wrapper<winrt::Microsoft::UI::Content::ContentSiteEnvironment>;
    using ContentSiteEnvironmentView = py::winrt_wrapper<winrt::Microsoft::UI::Content::ContentSiteEnvironmentView>;
    using ContentSiteRequestedStateChangedEventArgs = py::winrt_wrapper<winrt::Microsoft::UI::Content::ContentSiteRequestedStateChangedEventArgs>;
    using ContentSiteView = py::winrt_wrapper<winrt::Microsoft::UI::Content::ContentSiteView>;
    using DesktopChildSiteBridge = py::winrt_wrapper<winrt::Microsoft::UI::Content::DesktopChildSiteBridge>;
    using DesktopSiteBridge = py::winrt_wrapper<winrt::Microsoft::UI::Content::DesktopSiteBridge>;
    using IContentSiteBridge = py::winrt_wrapper<winrt::Microsoft::UI::Content::IContentSiteBridge>;
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Content::ContentCoordinateRoundingMode> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Content::ContentLayoutDirection> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::UI::Content::ContentSizePolicy> = "i";


    template<>
    struct py_type<winrt::Microsoft::UI::Content::ContentCoordinateRoundingMode>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.content";
        static constexpr const char* type_name = "ContentCoordinateRoundingMode";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Content::ContentLayoutDirection>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.content";
        static constexpr const char* type_name = "ContentLayoutDirection";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Content::ContentSizePolicy>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.content";
        static constexpr const char* type_name = "ContentSizePolicy";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Content::ContentCoordinateConverter>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.content";
        static constexpr const char* type_name = "ContentCoordinateConverter";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Content::ContentDeferral>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.content";
        static constexpr const char* type_name = "ContentDeferral";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Content::ContentEnvironmentSettingChangedEventArgs>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.content";
        static constexpr const char* type_name = "ContentEnvironmentSettingChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Content::ContentEnvironmentStateChangedEventArgs>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.content";
        static constexpr const char* type_name = "ContentEnvironmentStateChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Content::ContentIsland>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.content";
        static constexpr const char* type_name = "ContentIsland";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Content::ContentIslandAutomationProviderRequestedEventArgs>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.content";
        static constexpr const char* type_name = "ContentIslandAutomationProviderRequestedEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Content::ContentIslandEnvironment>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.content";
        static constexpr const char* type_name = "ContentIslandEnvironment";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Content::ContentIslandStateChangedEventArgs>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.content";
        static constexpr const char* type_name = "ContentIslandStateChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Content::ContentSite>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.content";
        static constexpr const char* type_name = "ContentSite";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Content::ContentSiteEnvironment>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.content";
        static constexpr const char* type_name = "ContentSiteEnvironment";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Content::ContentSiteEnvironmentView>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.content";
        static constexpr const char* type_name = "ContentSiteEnvironmentView";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Content::ContentSiteRequestedStateChangedEventArgs>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.content";
        static constexpr const char* type_name = "ContentSiteRequestedStateChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Content::ContentSiteView>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.content";
        static constexpr const char* type_name = "ContentSiteView";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Content::DesktopChildSiteBridge>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.content";
        static constexpr const char* type_name = "DesktopChildSiteBridge";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Content::DesktopSiteBridge>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.content";
        static constexpr const char* type_name = "DesktopSiteBridge";
    };

    template<>
    struct py_type<winrt::Microsoft::UI::Content::IContentSiteBridge>
    {
        static constexpr const char* module_name = "winrt.microsoft.ui.content";
        static constexpr const char* type_name = "IContentSiteBridge";
    };
}