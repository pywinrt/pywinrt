// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Networking.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>

#include <winrt/Windows.Security.EnterpriseData.h>

namespace py::proj::Windows::Security::EnterpriseData
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Security::EnterpriseData::DataProtectionStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Security::EnterpriseData::EnforcementLevel> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Security::EnterpriseData::FileProtectionStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Security::EnterpriseData::ProtectedImportExportStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Security::EnterpriseData::ProtectionPolicyAuditAction> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Security::EnterpriseData::ProtectionPolicyEvaluationResult> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Security::EnterpriseData::ProtectionPolicyRequestAccessBehavior> = "i";


    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::DataProtectionStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.DataProtectionStatus";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "DataProtectionStatus";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::EnforcementLevel>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.EnforcementLevel";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "EnforcementLevel";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::FileProtectionStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.FileProtectionStatus";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "FileProtectionStatus";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::ProtectedImportExportStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.ProtectedImportExportStatus";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "ProtectedImportExportStatus";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::ProtectionPolicyAuditAction>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.ProtectionPolicyAuditAction";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "ProtectionPolicyAuditAction";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::ProtectionPolicyEvaluationResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.ProtectionPolicyEvaluationResult";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "ProtectionPolicyEvaluationResult";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::ProtectionPolicyRequestAccessBehavior>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.ProtectionPolicyRequestAccessBehavior";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "ProtectionPolicyRequestAccessBehavior";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::BufferProtectUnprotectResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.BufferProtectUnprotectResult";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "BufferProtectUnprotectResult";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::DataProtectionInfo>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.DataProtectionInfo";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "DataProtectionInfo";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::DataProtectionManager>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.DataProtectionManager";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "DataProtectionManager";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::FileProtectionInfo>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.FileProtectionInfo";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "FileProtectionInfo";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::FileProtectionManager>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.FileProtectionManager";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "FileProtectionManager";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::FileRevocationManager>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.FileRevocationManager";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "FileRevocationManager";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::FileUnprotectOptions>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.FileUnprotectOptions";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "FileUnprotectOptions";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::ProtectedAccessResumedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.ProtectedAccessResumedEventArgs";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "ProtectedAccessResumedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::ProtectedAccessSuspendingEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.ProtectedAccessSuspendingEventArgs";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "ProtectedAccessSuspendingEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::ProtectedContainerExportResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.ProtectedContainerExportResult";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "ProtectedContainerExportResult";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::ProtectedContainerImportResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.ProtectedContainerImportResult";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "ProtectedContainerImportResult";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::ProtectedContentRevokedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.ProtectedContentRevokedEventArgs";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "ProtectedContentRevokedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::ProtectedFileCreateResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.ProtectedFileCreateResult";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "ProtectedFileCreateResult";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::ProtectionPolicyAuditInfo>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.ProtectionPolicyAuditInfo";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "ProtectionPolicyAuditInfo";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::ProtectionPolicyManager>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.ProtectionPolicyManager";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "ProtectionPolicyManager";
    };

    template<>
    struct py_type<winrt::Windows::Security::EnterpriseData::ThreadNetworkContext>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.enterprisedata.ThreadNetworkContext";
        static constexpr const char* module_name = "winrt.windows.security.enterprisedata";
        static constexpr const char* type_name = "ThreadNetworkContext";
    };
}

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.Networking.h")
#include "py.Windows.Networking.h"
#endif

#if __has_include("py.Windows.Storage.h")
#include "py.Windows.Storage.h"
#endif

#if __has_include("py.Windows.Storage.Streams.h")
#include "py.Windows.Storage.Streams.h"
#endif

namespace py::impl::Windows::Security::EnterpriseData
{
}

namespace py::wrapper::Windows::Security::EnterpriseData
{
    using BufferProtectUnprotectResult = py::winrt_wrapper<winrt::Windows::Security::EnterpriseData::BufferProtectUnprotectResult>;
    using DataProtectionInfo = py::winrt_wrapper<winrt::Windows::Security::EnterpriseData::DataProtectionInfo>;
    using DataProtectionManager = py::winrt_wrapper<winrt::Windows::Security::EnterpriseData::DataProtectionManager>;
    using FileProtectionInfo = py::winrt_wrapper<winrt::Windows::Security::EnterpriseData::FileProtectionInfo>;
    using FileProtectionManager = py::winrt_wrapper<winrt::Windows::Security::EnterpriseData::FileProtectionManager>;
    using FileRevocationManager = py::winrt_wrapper<winrt::Windows::Security::EnterpriseData::FileRevocationManager>;
    using FileUnprotectOptions = py::winrt_wrapper<winrt::Windows::Security::EnterpriseData::FileUnprotectOptions>;
    using ProtectedAccessResumedEventArgs = py::winrt_wrapper<winrt::Windows::Security::EnterpriseData::ProtectedAccessResumedEventArgs>;
    using ProtectedAccessSuspendingEventArgs = py::winrt_wrapper<winrt::Windows::Security::EnterpriseData::ProtectedAccessSuspendingEventArgs>;
    using ProtectedContainerExportResult = py::winrt_wrapper<winrt::Windows::Security::EnterpriseData::ProtectedContainerExportResult>;
    using ProtectedContainerImportResult = py::winrt_wrapper<winrt::Windows::Security::EnterpriseData::ProtectedContainerImportResult>;
    using ProtectedContentRevokedEventArgs = py::winrt_wrapper<winrt::Windows::Security::EnterpriseData::ProtectedContentRevokedEventArgs>;
    using ProtectedFileCreateResult = py::winrt_wrapper<winrt::Windows::Security::EnterpriseData::ProtectedFileCreateResult>;
    using ProtectionPolicyAuditInfo = py::winrt_wrapper<winrt::Windows::Security::EnterpriseData::ProtectionPolicyAuditInfo>;
    using ProtectionPolicyManager = py::winrt_wrapper<winrt::Windows::Security::EnterpriseData::ProtectionPolicyManager>;
    using ThreadNetworkContext = py::winrt_wrapper<winrt::Windows::Security::EnterpriseData::ThreadNetworkContext>;
}

namespace py
{
}
