// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.h>

#include <winrt/Windows.Foundation.Diagnostics.h>

namespace py::proj::Windows::Foundation::Diagnostics
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::Diagnostics::CausalityRelation> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::Diagnostics::CausalitySource> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::Diagnostics::CausalitySynchronousWork> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::Diagnostics::CausalityTraceLevel> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::Diagnostics::ErrorOptions> = "I";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::Diagnostics::LoggingFieldFormat> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::Diagnostics::LoggingLevel> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::Diagnostics::LoggingOpcode> = "i";


    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::CausalityRelation>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics.CausalityRelation";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "CausalityRelation";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::CausalitySource>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics.CausalitySource";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "CausalitySource";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::CausalitySynchronousWork>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics.CausalitySynchronousWork";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "CausalitySynchronousWork";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::CausalityTraceLevel>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics.CausalityTraceLevel";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "CausalityTraceLevel";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::ErrorOptions>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics.ErrorOptions";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "ErrorOptions";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::LoggingFieldFormat>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics.LoggingFieldFormat";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "LoggingFieldFormat";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::LoggingLevel>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics.LoggingLevel";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "LoggingLevel";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::LoggingOpcode>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics.LoggingOpcode";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "LoggingOpcode";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::AsyncCausalityTracer>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics.AsyncCausalityTracer";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "AsyncCausalityTracer";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::ErrorDetails>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics.ErrorDetails";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "ErrorDetails";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::FileLoggingSession>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics.FileLoggingSession";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "FileLoggingSession";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::LogFileGeneratedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics.LogFileGeneratedEventArgs";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "LogFileGeneratedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::LoggingActivity>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics.LoggingActivity";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "LoggingActivity";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::LoggingChannel>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics.LoggingChannel";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "LoggingChannel";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::LoggingChannelOptions>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics.LoggingChannelOptions";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "LoggingChannelOptions";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::LoggingFields>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics.LoggingFields";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "LoggingFields";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::LoggingOptions>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics.LoggingOptions";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "LoggingOptions";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::LoggingSession>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics.LoggingSession";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "LoggingSession";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::RuntimeBrokerErrorSettings>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics.RuntimeBrokerErrorSettings";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "RuntimeBrokerErrorSettings";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::TracingStatusChangedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics.TracingStatusChangedEventArgs";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "TracingStatusChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::IErrorReportingSettings>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics._IErrorReportingSettings";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "_IErrorReportingSettings";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::IFileLoggingSession>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics._IFileLoggingSession";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "_IFileLoggingSession";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::ILoggingChannel>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics._ILoggingChannel";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "_ILoggingChannel";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::ILoggingSession>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics._ILoggingSession";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "_ILoggingSession";
    };

    template<>
    struct py_type<winrt::Windows::Foundation::Diagnostics::ILoggingTarget>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.foundation.diagnostics._ILoggingTarget";
        static constexpr const char* module_name = "winrt.windows.foundation.diagnostics";
        static constexpr const char* type_name = "_ILoggingTarget";
    };
}

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Storage.h")
#include "py.Windows.Storage.h"
#endif

namespace py::impl::Windows::Foundation::Diagnostics
{
}

namespace py::wrapper::Windows::Foundation::Diagnostics
{
    using AsyncCausalityTracer = py::winrt_wrapper<winrt::Windows::Foundation::Diagnostics::AsyncCausalityTracer>;
    using ErrorDetails = py::winrt_wrapper<winrt::Windows::Foundation::Diagnostics::ErrorDetails>;
    using FileLoggingSession = py::winrt_wrapper<winrt::Windows::Foundation::Diagnostics::FileLoggingSession>;
    using LogFileGeneratedEventArgs = py::winrt_wrapper<winrt::Windows::Foundation::Diagnostics::LogFileGeneratedEventArgs>;
    using LoggingActivity = py::winrt_wrapper<winrt::Windows::Foundation::Diagnostics::LoggingActivity>;
    using LoggingChannel = py::winrt_wrapper<winrt::Windows::Foundation::Diagnostics::LoggingChannel>;
    using LoggingChannelOptions = py::winrt_wrapper<winrt::Windows::Foundation::Diagnostics::LoggingChannelOptions>;
    using LoggingFields = py::winrt_wrapper<winrt::Windows::Foundation::Diagnostics::LoggingFields>;
    using LoggingOptions = py::winrt_wrapper<winrt::Windows::Foundation::Diagnostics::LoggingOptions>;
    using LoggingSession = py::winrt_wrapper<winrt::Windows::Foundation::Diagnostics::LoggingSession>;
    using RuntimeBrokerErrorSettings = py::winrt_wrapper<winrt::Windows::Foundation::Diagnostics::RuntimeBrokerErrorSettings>;
    using TracingStatusChangedEventArgs = py::winrt_wrapper<winrt::Windows::Foundation::Diagnostics::TracingStatusChangedEventArgs>;
    using IErrorReportingSettings = py::winrt_wrapper<winrt::Windows::Foundation::Diagnostics::IErrorReportingSettings>;
    using IFileLoggingSession = py::winrt_wrapper<winrt::Windows::Foundation::Diagnostics::IFileLoggingSession>;
    using ILoggingChannel = py::winrt_wrapper<winrt::Windows::Foundation::Diagnostics::ILoggingChannel>;
    using ILoggingSession = py::winrt_wrapper<winrt::Windows::Foundation::Diagnostics::ILoggingSession>;
    using ILoggingTarget = py::winrt_wrapper<winrt::Windows::Foundation::Diagnostics::ILoggingTarget>;
}

namespace py
{
}
