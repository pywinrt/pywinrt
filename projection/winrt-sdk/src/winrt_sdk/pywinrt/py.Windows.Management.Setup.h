// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>

#include <winrt/Windows.Management.Setup.h>

namespace py::proj::Windows::Management::Setup
{
}

namespace py::impl::Windows::Management::Setup
{
    struct DeploymentSessionHeartbeatRequested
    {
        static winrt::Windows::Management::Setup::DeploymentSessionHeartbeatRequested get(PyObject* callable)
        {
            py::delegate_callable _delegate{ callable };

            return [delegate = std::move(_delegate)](auto param0)
            {
                auto gil = py::ensure_gil();

                py::pyobj_handle py_param0{ py::convert(param0) };

                if (!py_param0) {
                    PyErr_WriteUnraisable(delegate.callable());
                    throw std::invalid_argument("param0");
                }

                py::pyobj_handle args{ PyTuple_Pack(1, py_param0.get()) };

                if (!args) {
                    PyErr_WriteUnraisable(delegate.callable());
                    throw winrt::hresult_error();
                }

                py::pyobj_handle return_value{ PyObject_CallObject(delegate.callable(), args.get()) };

                if (!return_value)
                {
                    PyErr_WriteUnraisable(delegate.callable());
                    throw winrt::hresult_error();
                }
            };
        };
    };
}

namespace py::wrapper::Windows::Management::Setup
{
    using AgentProvisioningProgressReport = py::winrt_wrapper<winrt::Windows::Management::Setup::AgentProvisioningProgressReport>;
    using DeploymentSessionConnectionChangedEventArgs = py::winrt_wrapper<winrt::Windows::Management::Setup::DeploymentSessionConnectionChangedEventArgs>;
    using DeploymentSessionHeartbeatRequestedEventArgs = py::winrt_wrapper<winrt::Windows::Management::Setup::DeploymentSessionHeartbeatRequestedEventArgs>;
    using DeploymentSessionStateChangedEventArgs = py::winrt_wrapper<winrt::Windows::Management::Setup::DeploymentSessionStateChangedEventArgs>;
    using DeploymentWorkload = py::winrt_wrapper<winrt::Windows::Management::Setup::DeploymentWorkload>;
    using DeploymentWorkloadBatch = py::winrt_wrapper<winrt::Windows::Management::Setup::DeploymentWorkloadBatch>;
    using DevicePreparationExecutionContext = py::winrt_wrapper<winrt::Windows::Management::Setup::DevicePreparationExecutionContext>;
    using MachineProvisioningProgressReporter = py::winrt_wrapper<winrt::Windows::Management::Setup::MachineProvisioningProgressReporter>;
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Management::Setup::DeploymentAgentProgressState> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Management::Setup::DeploymentSessionConnectionChange> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Management::Setup::DeploymentSessionStateChange> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Management::Setup::DeploymentWorkloadState> = "i";


    template<>
    struct py_type<winrt::Windows::Management::Setup::DeploymentAgentProgressState>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.management.setup.DeploymentAgentProgressState";
        static constexpr const char* module_name = "winrt.windows.management.setup";
        static constexpr const char* type_name = "DeploymentAgentProgressState";
    };

    template<>
    struct py_type<winrt::Windows::Management::Setup::DeploymentSessionConnectionChange>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.management.setup.DeploymentSessionConnectionChange";
        static constexpr const char* module_name = "winrt.windows.management.setup";
        static constexpr const char* type_name = "DeploymentSessionConnectionChange";
    };

    template<>
    struct py_type<winrt::Windows::Management::Setup::DeploymentSessionStateChange>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.management.setup.DeploymentSessionStateChange";
        static constexpr const char* module_name = "winrt.windows.management.setup";
        static constexpr const char* type_name = "DeploymentSessionStateChange";
    };

    template<>
    struct py_type<winrt::Windows::Management::Setup::DeploymentWorkloadState>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.management.setup.DeploymentWorkloadState";
        static constexpr const char* module_name = "winrt.windows.management.setup";
        static constexpr const char* type_name = "DeploymentWorkloadState";
    };

    template<>
    struct py_type<winrt::Windows::Management::Setup::AgentProvisioningProgressReport>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.management.setup.AgentProvisioningProgressReport";
        static constexpr const char* module_name = "winrt.windows.management.setup";
        static constexpr const char* type_name = "AgentProvisioningProgressReport";
    };

    template<>
    struct py_type<winrt::Windows::Management::Setup::DeploymentSessionConnectionChangedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.management.setup.DeploymentSessionConnectionChangedEventArgs";
        static constexpr const char* module_name = "winrt.windows.management.setup";
        static constexpr const char* type_name = "DeploymentSessionConnectionChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Management::Setup::DeploymentSessionHeartbeatRequestedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.management.setup.DeploymentSessionHeartbeatRequestedEventArgs";
        static constexpr const char* module_name = "winrt.windows.management.setup";
        static constexpr const char* type_name = "DeploymentSessionHeartbeatRequestedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Management::Setup::DeploymentSessionStateChangedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.management.setup.DeploymentSessionStateChangedEventArgs";
        static constexpr const char* module_name = "winrt.windows.management.setup";
        static constexpr const char* type_name = "DeploymentSessionStateChangedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Management::Setup::DeploymentWorkload>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.management.setup.DeploymentWorkload";
        static constexpr const char* module_name = "winrt.windows.management.setup";
        static constexpr const char* type_name = "DeploymentWorkload";
    };

    template<>
    struct py_type<winrt::Windows::Management::Setup::DeploymentWorkloadBatch>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.management.setup.DeploymentWorkloadBatch";
        static constexpr const char* module_name = "winrt.windows.management.setup";
        static constexpr const char* type_name = "DeploymentWorkloadBatch";
    };

    template<>
    struct py_type<winrt::Windows::Management::Setup::DevicePreparationExecutionContext>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.management.setup.DevicePreparationExecutionContext";
        static constexpr const char* module_name = "winrt.windows.management.setup";
        static constexpr const char* type_name = "DevicePreparationExecutionContext";
    };

    template<>
    struct py_type<winrt::Windows::Management::Setup::MachineProvisioningProgressReporter>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.management.setup.MachineProvisioningProgressReporter";
        static constexpr const char* module_name = "winrt.windows.management.setup";
        static constexpr const char* type_name = "MachineProvisioningProgressReporter";
    };
    template <>
    struct delegate_python_type<winrt::Windows::Management::Setup::DeploymentSessionHeartbeatRequested>
    {
        using type = py::impl::Windows::Management::Setup::DeploymentSessionHeartbeatRequested;
    };

}