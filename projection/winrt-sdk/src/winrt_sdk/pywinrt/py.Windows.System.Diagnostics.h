// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"

#if __has_include("py.Windows.Data.Json.h")
#include "py.Windows.Data.Json.h"
#endif

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.System.h")
#include "py.Windows.System.h"
#endif

#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.System.h>

#include <winrt/Windows.System.Diagnostics.h>

namespace py::proj::Windows::System::Diagnostics
{}

namespace py::impl::Windows::System::Diagnostics
{}

namespace py::wrapper::Windows::System::Diagnostics
{
    using DiagnosticActionResult = py::winrt_wrapper<winrt::Windows::System::Diagnostics::DiagnosticActionResult>;
    using DiagnosticInvoker = py::winrt_wrapper<winrt::Windows::System::Diagnostics::DiagnosticInvoker>;
    using ProcessCpuUsage = py::winrt_wrapper<winrt::Windows::System::Diagnostics::ProcessCpuUsage>;
    using ProcessCpuUsageReport = py::winrt_wrapper<winrt::Windows::System::Diagnostics::ProcessCpuUsageReport>;
    using ProcessDiagnosticInfo = py::winrt_wrapper<winrt::Windows::System::Diagnostics::ProcessDiagnosticInfo>;
    using ProcessDiskUsage = py::winrt_wrapper<winrt::Windows::System::Diagnostics::ProcessDiskUsage>;
    using ProcessDiskUsageReport = py::winrt_wrapper<winrt::Windows::System::Diagnostics::ProcessDiskUsageReport>;
    using ProcessMemoryUsage = py::winrt_wrapper<winrt::Windows::System::Diagnostics::ProcessMemoryUsage>;
    using ProcessMemoryUsageReport = py::winrt_wrapper<winrt::Windows::System::Diagnostics::ProcessMemoryUsageReport>;
    using SystemCpuUsage = py::winrt_wrapper<winrt::Windows::System::Diagnostics::SystemCpuUsage>;
    using SystemCpuUsageReport = py::winrt_wrapper<winrt::Windows::System::Diagnostics::SystemCpuUsageReport>;
    using SystemDiagnosticInfo = py::winrt_wrapper<winrt::Windows::System::Diagnostics::SystemDiagnosticInfo>;
    using SystemMemoryUsage = py::winrt_wrapper<winrt::Windows::System::Diagnostics::SystemMemoryUsage>;
    using SystemMemoryUsageReport = py::winrt_wrapper<winrt::Windows::System::Diagnostics::SystemMemoryUsageReport>;
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::System::Diagnostics::DiagnosticActionState> = "i";


    template<>
    struct py_type<winrt::Windows::System::Diagnostics::DiagnosticActionState>
    {
        static constexpr const char* module_name = "winrt.windows.system.diagnostics";
        static constexpr const char* type_name = "DiagnosticActionState";
    };

    template<>
    struct py_type<winrt::Windows::System::Diagnostics::DiagnosticActionResult>
    {
        static constexpr const char* module_name = "winrt.windows.system.diagnostics";
        static constexpr const char* type_name = "DiagnosticActionResult";
    };

    template<>
    struct py_type<winrt::Windows::System::Diagnostics::DiagnosticInvoker>
    {
        static constexpr const char* module_name = "winrt.windows.system.diagnostics";
        static constexpr const char* type_name = "DiagnosticInvoker";
    };

    template<>
    struct py_type<winrt::Windows::System::Diagnostics::ProcessCpuUsage>
    {
        static constexpr const char* module_name = "winrt.windows.system.diagnostics";
        static constexpr const char* type_name = "ProcessCpuUsage";
    };

    template<>
    struct py_type<winrt::Windows::System::Diagnostics::ProcessCpuUsageReport>
    {
        static constexpr const char* module_name = "winrt.windows.system.diagnostics";
        static constexpr const char* type_name = "ProcessCpuUsageReport";
    };

    template<>
    struct py_type<winrt::Windows::System::Diagnostics::ProcessDiagnosticInfo>
    {
        static constexpr const char* module_name = "winrt.windows.system.diagnostics";
        static constexpr const char* type_name = "ProcessDiagnosticInfo";
    };

    template<>
    struct py_type<winrt::Windows::System::Diagnostics::ProcessDiskUsage>
    {
        static constexpr const char* module_name = "winrt.windows.system.diagnostics";
        static constexpr const char* type_name = "ProcessDiskUsage";
    };

    template<>
    struct py_type<winrt::Windows::System::Diagnostics::ProcessDiskUsageReport>
    {
        static constexpr const char* module_name = "winrt.windows.system.diagnostics";
        static constexpr const char* type_name = "ProcessDiskUsageReport";
    };

    template<>
    struct py_type<winrt::Windows::System::Diagnostics::ProcessMemoryUsage>
    {
        static constexpr const char* module_name = "winrt.windows.system.diagnostics";
        static constexpr const char* type_name = "ProcessMemoryUsage";
    };

    template<>
    struct py_type<winrt::Windows::System::Diagnostics::ProcessMemoryUsageReport>
    {
        static constexpr const char* module_name = "winrt.windows.system.diagnostics";
        static constexpr const char* type_name = "ProcessMemoryUsageReport";
    };

    template<>
    struct py_type<winrt::Windows::System::Diagnostics::SystemCpuUsage>
    {
        static constexpr const char* module_name = "winrt.windows.system.diagnostics";
        static constexpr const char* type_name = "SystemCpuUsage";
    };

    template<>
    struct py_type<winrt::Windows::System::Diagnostics::SystemCpuUsageReport>
    {
        static constexpr const char* module_name = "winrt.windows.system.diagnostics";
        static constexpr const char* type_name = "SystemCpuUsageReport";
    };

    template<>
    struct py_type<winrt::Windows::System::Diagnostics::SystemDiagnosticInfo>
    {
        static constexpr const char* module_name = "winrt.windows.system.diagnostics";
        static constexpr const char* type_name = "SystemDiagnosticInfo";
    };

    template<>
    struct py_type<winrt::Windows::System::Diagnostics::SystemMemoryUsage>
    {
        static constexpr const char* module_name = "winrt.windows.system.diagnostics";
        static constexpr const char* type_name = "SystemMemoryUsage";
    };

    template<>
    struct py_type<winrt::Windows::System::Diagnostics::SystemMemoryUsageReport>
    {
        static constexpr const char* module_name = "winrt.windows.system.diagnostics";
        static constexpr const char* type_name = "SystemMemoryUsageReport";
    };
}