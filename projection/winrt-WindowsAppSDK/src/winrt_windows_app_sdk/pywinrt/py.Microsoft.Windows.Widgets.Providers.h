// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.Windows.Widgets.h>

#include <winrt/Microsoft.Windows.Widgets.Providers.h>

namespace py::proj::Microsoft::Windows::Widgets::Providers
{
}

namespace py
{

    template<>
    struct py_type<winrt::Microsoft::Windows::Widgets::Providers::WidgetActionInvokedArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.windows.widgets.providers.WidgetActionInvokedArgs";
        static constexpr const char* module_name = "winui3.microsoft.windows.widgets.providers";
        static constexpr const char* type_name = "WidgetActionInvokedArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::Windows::Widgets::Providers::WidgetAnalyticsInfoReportedArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.windows.widgets.providers.WidgetAnalyticsInfoReportedArgs";
        static constexpr const char* module_name = "winui3.microsoft.windows.widgets.providers";
        static constexpr const char* type_name = "WidgetAnalyticsInfoReportedArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::Windows::Widgets::Providers::WidgetContext>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.windows.widgets.providers.WidgetContext";
        static constexpr const char* module_name = "winui3.microsoft.windows.widgets.providers";
        static constexpr const char* type_name = "WidgetContext";
    };

    template<>
    struct py_type<winrt::Microsoft::Windows::Widgets::Providers::WidgetContextChangedArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.windows.widgets.providers.WidgetContextChangedArgs";
        static constexpr const char* module_name = "winui3.microsoft.windows.widgets.providers";
        static constexpr const char* type_name = "WidgetContextChangedArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::Windows::Widgets::Providers::WidgetCustomizationRequestedArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.windows.widgets.providers.WidgetCustomizationRequestedArgs";
        static constexpr const char* module_name = "winui3.microsoft.windows.widgets.providers";
        static constexpr const char* type_name = "WidgetCustomizationRequestedArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::Windows::Widgets::Providers::WidgetErrorInfoReportedArgs>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.windows.widgets.providers.WidgetErrorInfoReportedArgs";
        static constexpr const char* module_name = "winui3.microsoft.windows.widgets.providers";
        static constexpr const char* type_name = "WidgetErrorInfoReportedArgs";
    };

    template<>
    struct py_type<winrt::Microsoft::Windows::Widgets::Providers::WidgetInfo>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.windows.widgets.providers.WidgetInfo";
        static constexpr const char* module_name = "winui3.microsoft.windows.widgets.providers";
        static constexpr const char* type_name = "WidgetInfo";
    };

    template<>
    struct py_type<winrt::Microsoft::Windows::Widgets::Providers::WidgetManager>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.windows.widgets.providers.WidgetManager";
        static constexpr const char* module_name = "winui3.microsoft.windows.widgets.providers";
        static constexpr const char* type_name = "WidgetManager";
    };

    template<>
    struct py_type<winrt::Microsoft::Windows::Widgets::Providers::WidgetUpdateRequestOptions>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.windows.widgets.providers.WidgetUpdateRequestOptions";
        static constexpr const char* module_name = "winui3.microsoft.windows.widgets.providers";
        static constexpr const char* type_name = "WidgetUpdateRequestOptions";
    };

    template<>
    struct py_type<winrt::Microsoft::Windows::Widgets::Providers::IWidgetManager>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.windows.widgets.providers._IWidgetManager";
        static constexpr const char* module_name = "winui3.microsoft.windows.widgets.providers";
        static constexpr const char* type_name = "_IWidgetManager";
    };

    template<>
    struct py_type<winrt::Microsoft::Windows::Widgets::Providers::IWidgetProvider>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.windows.widgets.providers._IWidgetProvider";
        static constexpr const char* module_name = "winui3.microsoft.windows.widgets.providers";
        static constexpr const char* type_name = "_IWidgetProvider";
    };

    template<>
    struct py_type<winrt::Microsoft::Windows::Widgets::Providers::IWidgetProvider2>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.windows.widgets.providers._IWidgetProvider2";
        static constexpr const char* module_name = "winui3.microsoft.windows.widgets.providers";
        static constexpr const char* type_name = "_IWidgetProvider2";
    };

    template<>
    struct py_type<winrt::Microsoft::Windows::Widgets::Providers::IWidgetProviderAnalytics>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.windows.widgets.providers._IWidgetProviderAnalytics";
        static constexpr const char* module_name = "winui3.microsoft.windows.widgets.providers";
        static constexpr const char* type_name = "_IWidgetProviderAnalytics";
    };

    template<>
    struct py_type<winrt::Microsoft::Windows::Widgets::Providers::IWidgetProviderErrors>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.windows.widgets.providers._IWidgetProviderErrors";
        static constexpr const char* module_name = "winui3.microsoft.windows.widgets.providers";
        static constexpr const char* type_name = "_IWidgetProviderErrors";
    };
}

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Microsoft.Windows.Widgets.h")
#include "py.Microsoft.Windows.Widgets.h"
#endif

namespace py::impl::Microsoft::Windows::Widgets::Providers
{
}

namespace py::wrapper::Microsoft::Windows::Widgets::Providers
{
    using WidgetActionInvokedArgs = py::winrt_wrapper<winrt::Microsoft::Windows::Widgets::Providers::WidgetActionInvokedArgs>;
    using WidgetAnalyticsInfoReportedArgs = py::winrt_wrapper<winrt::Microsoft::Windows::Widgets::Providers::WidgetAnalyticsInfoReportedArgs>;
    using WidgetContext = py::winrt_wrapper<winrt::Microsoft::Windows::Widgets::Providers::WidgetContext>;
    using WidgetContextChangedArgs = py::winrt_wrapper<winrt::Microsoft::Windows::Widgets::Providers::WidgetContextChangedArgs>;
    using WidgetCustomizationRequestedArgs = py::winrt_wrapper<winrt::Microsoft::Windows::Widgets::Providers::WidgetCustomizationRequestedArgs>;
    using WidgetErrorInfoReportedArgs = py::winrt_wrapper<winrt::Microsoft::Windows::Widgets::Providers::WidgetErrorInfoReportedArgs>;
    using WidgetInfo = py::winrt_wrapper<winrt::Microsoft::Windows::Widgets::Providers::WidgetInfo>;
    using WidgetManager = py::winrt_wrapper<winrt::Microsoft::Windows::Widgets::Providers::WidgetManager>;
    using WidgetUpdateRequestOptions = py::winrt_wrapper<winrt::Microsoft::Windows::Widgets::Providers::WidgetUpdateRequestOptions>;
    using IWidgetManager = py::winrt_wrapper<winrt::Microsoft::Windows::Widgets::Providers::IWidgetManager>;
    using IWidgetProvider = py::winrt_wrapper<winrt::Microsoft::Windows::Widgets::Providers::IWidgetProvider>;
    using IWidgetProvider2 = py::winrt_wrapper<winrt::Microsoft::Windows::Widgets::Providers::IWidgetProvider2>;
    using IWidgetProviderAnalytics = py::winrt_wrapper<winrt::Microsoft::Windows::Widgets::Providers::IWidgetProviderAnalytics>;
    using IWidgetProviderErrors = py::winrt_wrapper<winrt::Microsoft::Windows::Widgets::Providers::IWidgetProviderErrors>;
}

namespace py
{
}
