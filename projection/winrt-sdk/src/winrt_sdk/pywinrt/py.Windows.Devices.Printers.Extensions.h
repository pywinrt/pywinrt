// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Foundation.h>

#include <winrt/Windows.Devices.Printers.Extensions.h>

namespace py::proj::Windows::Devices::Printers::Extensions
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Printers::Extensions::Print3DWorkflowDetail> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::Printers::Extensions::Print3DWorkflowStatus> = "i";


    template<>
    struct py_type<winrt::Windows::Devices::Printers::Extensions::Print3DWorkflowDetail>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.printers.extensions.Print3DWorkflowDetail";
        static constexpr const char* module_name = "winrt.windows.devices.printers.extensions";
        static constexpr const char* type_name = "Print3DWorkflowDetail";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Printers::Extensions::Print3DWorkflowStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.printers.extensions.Print3DWorkflowStatus";
        static constexpr const char* module_name = "winrt.windows.devices.printers.extensions";
        static constexpr const char* type_name = "Print3DWorkflowStatus";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Printers::Extensions::Print3DWorkflow>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.printers.extensions.Print3DWorkflow";
        static constexpr const char* module_name = "winrt.windows.devices.printers.extensions";
        static constexpr const char* type_name = "Print3DWorkflow";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Printers::Extensions::Print3DWorkflowPrintRequestedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.printers.extensions.Print3DWorkflowPrintRequestedEventArgs";
        static constexpr const char* module_name = "winrt.windows.devices.printers.extensions";
        static constexpr const char* type_name = "Print3DWorkflowPrintRequestedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Printers::Extensions::Print3DWorkflowPrinterChangedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.printers.extensions.Print3DWorkflowPrinterChangedEventArgs";
        static constexpr const char* module_name = "winrt.windows.devices.printers.extensions";
        static constexpr const char* type_name = "Print3DWorkflowPrinterChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Printers::Extensions::PrintExtensionContext>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.printers.extensions.PrintExtensionContext";
        static constexpr const char* module_name = "winrt.windows.devices.printers.extensions";
        static constexpr const char* type_name = "PrintExtensionContext";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Printers::Extensions::PrintNotificationEventDetails>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.printers.extensions.PrintNotificationEventDetails";
        static constexpr const char* module_name = "winrt.windows.devices.printers.extensions";
        static constexpr const char* type_name = "PrintNotificationEventDetails";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Printers::Extensions::PrintTaskConfiguration>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.printers.extensions.PrintTaskConfiguration";
        static constexpr const char* module_name = "winrt.windows.devices.printers.extensions";
        static constexpr const char* type_name = "PrintTaskConfiguration";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Printers::Extensions::PrintTaskConfigurationSaveRequest>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.printers.extensions.PrintTaskConfigurationSaveRequest";
        static constexpr const char* module_name = "winrt.windows.devices.printers.extensions";
        static constexpr const char* type_name = "PrintTaskConfigurationSaveRequest";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Printers::Extensions::PrintTaskConfigurationSaveRequestedDeferral>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.printers.extensions.PrintTaskConfigurationSaveRequestedDeferral";
        static constexpr const char* module_name = "winrt.windows.devices.printers.extensions";
        static constexpr const char* type_name = "PrintTaskConfigurationSaveRequestedDeferral";
    };

    template<>
    struct py_type<winrt::Windows::Devices::Printers::Extensions::PrintTaskConfigurationSaveRequestedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.printers.extensions.PrintTaskConfigurationSaveRequestedEventArgs";
        static constexpr const char* module_name = "winrt.windows.devices.printers.extensions";
        static constexpr const char* type_name = "PrintTaskConfigurationSaveRequestedEventArgs";
    };
}

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

namespace py::impl::Windows::Devices::Printers::Extensions
{
}

namespace py::wrapper::Windows::Devices::Printers::Extensions
{
    using Print3DWorkflow = py::winrt_wrapper<winrt::Windows::Devices::Printers::Extensions::Print3DWorkflow>;
    using Print3DWorkflowPrintRequestedEventArgs = py::winrt_wrapper<winrt::Windows::Devices::Printers::Extensions::Print3DWorkflowPrintRequestedEventArgs>;
    using Print3DWorkflowPrinterChangedEventArgs = py::winrt_wrapper<winrt::Windows::Devices::Printers::Extensions::Print3DWorkflowPrinterChangedEventArgs>;
    using PrintExtensionContext = py::winrt_wrapper<winrt::Windows::Devices::Printers::Extensions::PrintExtensionContext>;
    using PrintNotificationEventDetails = py::winrt_wrapper<winrt::Windows::Devices::Printers::Extensions::PrintNotificationEventDetails>;
    using PrintTaskConfiguration = py::winrt_wrapper<winrt::Windows::Devices::Printers::Extensions::PrintTaskConfiguration>;
    using PrintTaskConfigurationSaveRequest = py::winrt_wrapper<winrt::Windows::Devices::Printers::Extensions::PrintTaskConfigurationSaveRequest>;
    using PrintTaskConfigurationSaveRequestedDeferral = py::winrt_wrapper<winrt::Windows::Devices::Printers::Extensions::PrintTaskConfigurationSaveRequestedDeferral>;
    using PrintTaskConfigurationSaveRequestedEventArgs = py::winrt_wrapper<winrt::Windows::Devices::Printers::Extensions::PrintTaskConfigurationSaveRequestedEventArgs>;
}

namespace py
{
}
