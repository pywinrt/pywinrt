// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Networking.h>
#include <winrt/Windows.Storage.Streams.h>

#include <winrt/Windows.Security.Cryptography.Certificates.h>

namespace py::proj::Windows::Security::Cryptography::Certificates
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Security::Cryptography::Certificates::CertificateChainPolicy> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Security::Cryptography::Certificates::ChainValidationResult> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Security::Cryptography::Certificates::EnrollKeyUsages> = "I";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Security::Cryptography::Certificates::ExportOption> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Security::Cryptography::Certificates::InstallOptions> = "I";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Security::Cryptography::Certificates::KeyProtectionLevel> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Security::Cryptography::Certificates::KeySize> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Security::Cryptography::Certificates::SignatureValidationResult> = "i";


    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::CertificateChainPolicy>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.CertificateChainPolicy";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "CertificateChainPolicy";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::ChainValidationResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.ChainValidationResult";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "ChainValidationResult";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::EnrollKeyUsages>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.EnrollKeyUsages";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "EnrollKeyUsages";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::ExportOption>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.ExportOption";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "ExportOption";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::InstallOptions>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.InstallOptions";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "InstallOptions";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::KeyProtectionLevel>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.KeyProtectionLevel";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "KeyProtectionLevel";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::KeySize>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.KeySize";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "KeySize";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::SignatureValidationResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.SignatureValidationResult";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "SignatureValidationResult";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::Certificate>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.Certificate";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "Certificate";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::CertificateChain>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.CertificateChain";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "CertificateChain";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::CertificateEnrollmentManager>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.CertificateEnrollmentManager";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "CertificateEnrollmentManager";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::CertificateExtension>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.CertificateExtension";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "CertificateExtension";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::CertificateKeyUsages>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.CertificateKeyUsages";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "CertificateKeyUsages";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::CertificateQuery>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.CertificateQuery";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "CertificateQuery";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::CertificateRequestProperties>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.CertificateRequestProperties";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "CertificateRequestProperties";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::CertificateStore>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.CertificateStore";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "CertificateStore";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::CertificateStores>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.CertificateStores";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "CertificateStores";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::ChainBuildingParameters>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.ChainBuildingParameters";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "ChainBuildingParameters";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::ChainValidationParameters>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.ChainValidationParameters";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "ChainValidationParameters";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::CmsAttachedSignature>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.CmsAttachedSignature";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "CmsAttachedSignature";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::CmsDetachedSignature>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.CmsDetachedSignature";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "CmsDetachedSignature";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::CmsSignerInfo>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.CmsSignerInfo";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "CmsSignerInfo";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::CmsTimestampInfo>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.CmsTimestampInfo";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "CmsTimestampInfo";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::KeyAlgorithmNames>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.KeyAlgorithmNames";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "KeyAlgorithmNames";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::KeyAttestationHelper>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.KeyAttestationHelper";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "KeyAttestationHelper";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::KeyStorageProviderNames>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.KeyStorageProviderNames";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "KeyStorageProviderNames";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::PfxImportParameters>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.PfxImportParameters";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "PfxImportParameters";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::StandardCertificateStoreNames>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.StandardCertificateStoreNames";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "StandardCertificateStoreNames";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::SubjectAlternativeNameInfo>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.SubjectAlternativeNameInfo";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "SubjectAlternativeNameInfo";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::UserCertificateEnrollmentManager>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.UserCertificateEnrollmentManager";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "UserCertificateEnrollmentManager";
    };

    template<>
    struct py_type<winrt::Windows::Security::Cryptography::Certificates::UserCertificateStore>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.security.cryptography.certificates.UserCertificateStore";
        static constexpr const char* module_name = "winrt.windows.security.cryptography.certificates";
        static constexpr const char* type_name = "UserCertificateStore";
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

#if __has_include("py.Windows.Storage.Streams.h")
#include "py.Windows.Storage.Streams.h"
#endif

namespace py::impl::Windows::Security::Cryptography::Certificates
{
}

namespace py::wrapper::Windows::Security::Cryptography::Certificates
{
    using Certificate = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::Certificate>;
    using CertificateChain = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::CertificateChain>;
    using CertificateEnrollmentManager = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::CertificateEnrollmentManager>;
    using CertificateExtension = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::CertificateExtension>;
    using CertificateKeyUsages = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::CertificateKeyUsages>;
    using CertificateQuery = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::CertificateQuery>;
    using CertificateRequestProperties = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::CertificateRequestProperties>;
    using CertificateStore = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::CertificateStore>;
    using CertificateStores = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::CertificateStores>;
    using ChainBuildingParameters = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::ChainBuildingParameters>;
    using ChainValidationParameters = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::ChainValidationParameters>;
    using CmsAttachedSignature = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::CmsAttachedSignature>;
    using CmsDetachedSignature = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::CmsDetachedSignature>;
    using CmsSignerInfo = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::CmsSignerInfo>;
    using CmsTimestampInfo = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::CmsTimestampInfo>;
    using KeyAlgorithmNames = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::KeyAlgorithmNames>;
    using KeyAttestationHelper = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::KeyAttestationHelper>;
    using KeyStorageProviderNames = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::KeyStorageProviderNames>;
    using PfxImportParameters = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::PfxImportParameters>;
    using StandardCertificateStoreNames = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::StandardCertificateStoreNames>;
    using SubjectAlternativeNameInfo = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::SubjectAlternativeNameInfo>;
    using UserCertificateEnrollmentManager = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::UserCertificateEnrollmentManager>;
    using UserCertificateStore = py::winrt_wrapper<winrt::Windows::Security::Cryptography::Certificates::UserCertificateStore>;
}

namespace py
{
}
