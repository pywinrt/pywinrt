// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Security_Cryptography_Certificates_2_H
#define WINRT_Windows_Security_Cryptography_Certificates_2_H
#include "winrt/impl/Windows.Foundation.Collections.1.h"
#include "winrt/impl/Windows.Storage.Streams.1.h"
#include "winrt/impl/Windows.Security.Cryptography.Certificates.1.h"
WINRT_EXPORT namespace winrt::Windows::Security::Cryptography::Certificates
{
    struct WINRT_IMPL_EMPTY_BASES Certificate : winrt::Windows::Security::Cryptography::Certificates::ICertificate,
        impl::require<Certificate, winrt::Windows::Security::Cryptography::Certificates::ICertificate2, winrt::Windows::Security::Cryptography::Certificates::ICertificate3>
    {
        Certificate(std::nullptr_t) noexcept {}
        Certificate(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Security::Cryptography::Certificates::ICertificate(ptr, take_ownership_from_abi) {}
        explicit Certificate(winrt::Windows::Storage::Streams::IBuffer const& certBlob);
    };
    struct WINRT_IMPL_EMPTY_BASES CertificateChain : winrt::Windows::Security::Cryptography::Certificates::ICertificateChain
    {
        CertificateChain(std::nullptr_t) noexcept {}
        CertificateChain(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Security::Cryptography::Certificates::ICertificateChain(ptr, take_ownership_from_abi) {}
    };
    struct CertificateEnrollmentManager
    {
        CertificateEnrollmentManager() = delete;
        static auto CreateRequestAsync(winrt::Windows::Security::Cryptography::Certificates::CertificateRequestProperties const& request);
        static auto InstallCertificateAsync(param::hstring const& certificate, winrt::Windows::Security::Cryptography::Certificates::InstallOptions const& installOption);
        static auto ImportPfxDataAsync(param::hstring const& pfxData, param::hstring const& password, winrt::Windows::Security::Cryptography::Certificates::ExportOption const& exportable, winrt::Windows::Security::Cryptography::Certificates::KeyProtectionLevel const& keyProtectionLevel, winrt::Windows::Security::Cryptography::Certificates::InstallOptions const& installOption, param::hstring const& friendlyName);
        [[nodiscard]] static auto UserCertificateEnrollmentManager();
        static auto ImportPfxDataAsync(param::hstring const& pfxData, param::hstring const& password, winrt::Windows::Security::Cryptography::Certificates::ExportOption const& exportable, winrt::Windows::Security::Cryptography::Certificates::KeyProtectionLevel const& keyProtectionLevel, winrt::Windows::Security::Cryptography::Certificates::InstallOptions const& installOption, param::hstring const& friendlyName, param::hstring const& keyStorageProvider);
        static auto ImportPfxDataAsync(param::hstring const& pfxData, param::hstring const& password, winrt::Windows::Security::Cryptography::Certificates::PfxImportParameters const& pfxImportParameters);
    };
    struct WINRT_IMPL_EMPTY_BASES CertificateExtension : winrt::Windows::Security::Cryptography::Certificates::ICertificateExtension
    {
        CertificateExtension(std::nullptr_t) noexcept {}
        CertificateExtension(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Security::Cryptography::Certificates::ICertificateExtension(ptr, take_ownership_from_abi) {}
        CertificateExtension();
    };
    struct WINRT_IMPL_EMPTY_BASES CertificateKeyUsages : winrt::Windows::Security::Cryptography::Certificates::ICertificateKeyUsages
    {
        CertificateKeyUsages(std::nullptr_t) noexcept {}
        CertificateKeyUsages(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Security::Cryptography::Certificates::ICertificateKeyUsages(ptr, take_ownership_from_abi) {}
        CertificateKeyUsages();
    };
    struct WINRT_IMPL_EMPTY_BASES CertificateQuery : winrt::Windows::Security::Cryptography::Certificates::ICertificateQuery,
        impl::require<CertificateQuery, winrt::Windows::Security::Cryptography::Certificates::ICertificateQuery2>
    {
        CertificateQuery(std::nullptr_t) noexcept {}
        CertificateQuery(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Security::Cryptography::Certificates::ICertificateQuery(ptr, take_ownership_from_abi) {}
        CertificateQuery();
    };
    struct WINRT_IMPL_EMPTY_BASES CertificateRequestProperties : winrt::Windows::Security::Cryptography::Certificates::ICertificateRequestProperties,
        impl::require<CertificateRequestProperties, winrt::Windows::Security::Cryptography::Certificates::ICertificateRequestProperties2, winrt::Windows::Security::Cryptography::Certificates::ICertificateRequestProperties3, winrt::Windows::Security::Cryptography::Certificates::ICertificateRequestProperties4>
    {
        CertificateRequestProperties(std::nullptr_t) noexcept {}
        CertificateRequestProperties(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Security::Cryptography::Certificates::ICertificateRequestProperties(ptr, take_ownership_from_abi) {}
        CertificateRequestProperties();
    };
    struct WINRT_IMPL_EMPTY_BASES CertificateStore : winrt::Windows::Security::Cryptography::Certificates::ICertificateStore,
        impl::require<CertificateStore, winrt::Windows::Security::Cryptography::Certificates::ICertificateStore2>
    {
        CertificateStore(std::nullptr_t) noexcept {}
        CertificateStore(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Security::Cryptography::Certificates::ICertificateStore(ptr, take_ownership_from_abi) {}
    };
    struct CertificateStores
    {
        CertificateStores() = delete;
        static auto FindAllAsync();
        static auto FindAllAsync(winrt::Windows::Security::Cryptography::Certificates::CertificateQuery const& query);
        [[nodiscard]] static auto TrustedRootCertificationAuthorities();
        [[nodiscard]] static auto IntermediateCertificationAuthorities();
        static auto GetStoreByName(param::hstring const& storeName);
        static auto GetUserStoreByName(param::hstring const& storeName);
    };
    struct WINRT_IMPL_EMPTY_BASES ChainBuildingParameters : winrt::Windows::Security::Cryptography::Certificates::IChainBuildingParameters
    {
        ChainBuildingParameters(std::nullptr_t) noexcept {}
        ChainBuildingParameters(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Security::Cryptography::Certificates::IChainBuildingParameters(ptr, take_ownership_from_abi) {}
        ChainBuildingParameters();
    };
    struct WINRT_IMPL_EMPTY_BASES ChainValidationParameters : winrt::Windows::Security::Cryptography::Certificates::IChainValidationParameters
    {
        ChainValidationParameters(std::nullptr_t) noexcept {}
        ChainValidationParameters(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Security::Cryptography::Certificates::IChainValidationParameters(ptr, take_ownership_from_abi) {}
        ChainValidationParameters();
    };
    struct WINRT_IMPL_EMPTY_BASES CmsAttachedSignature : winrt::Windows::Security::Cryptography::Certificates::ICmsAttachedSignature
    {
        CmsAttachedSignature(std::nullptr_t) noexcept {}
        CmsAttachedSignature(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Security::Cryptography::Certificates::ICmsAttachedSignature(ptr, take_ownership_from_abi) {}
        explicit CmsAttachedSignature(winrt::Windows::Storage::Streams::IBuffer const& inputBlob);
        static auto GenerateSignatureAsync(winrt::Windows::Storage::Streams::IBuffer const& data, param::async_iterable<winrt::Windows::Security::Cryptography::Certificates::CmsSignerInfo> const& signers, param::async_iterable<winrt::Windows::Security::Cryptography::Certificates::Certificate> const& certificates);
    };
    struct WINRT_IMPL_EMPTY_BASES CmsDetachedSignature : winrt::Windows::Security::Cryptography::Certificates::ICmsDetachedSignature
    {
        CmsDetachedSignature(std::nullptr_t) noexcept {}
        CmsDetachedSignature(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Security::Cryptography::Certificates::ICmsDetachedSignature(ptr, take_ownership_from_abi) {}
        explicit CmsDetachedSignature(winrt::Windows::Storage::Streams::IBuffer const& inputBlob);
        static auto GenerateSignatureAsync(winrt::Windows::Storage::Streams::IInputStream const& data, param::async_iterable<winrt::Windows::Security::Cryptography::Certificates::CmsSignerInfo> const& signers, param::async_iterable<winrt::Windows::Security::Cryptography::Certificates::Certificate> const& certificates);
    };
    struct WINRT_IMPL_EMPTY_BASES CmsSignerInfo : winrt::Windows::Security::Cryptography::Certificates::ICmsSignerInfo
    {
        CmsSignerInfo(std::nullptr_t) noexcept {}
        CmsSignerInfo(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Security::Cryptography::Certificates::ICmsSignerInfo(ptr, take_ownership_from_abi) {}
        CmsSignerInfo();
    };
    struct WINRT_IMPL_EMPTY_BASES CmsTimestampInfo : winrt::Windows::Security::Cryptography::Certificates::ICmsTimestampInfo
    {
        CmsTimestampInfo(std::nullptr_t) noexcept {}
        CmsTimestampInfo(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Security::Cryptography::Certificates::ICmsTimestampInfo(ptr, take_ownership_from_abi) {}
    };
    struct KeyAlgorithmNames
    {
        KeyAlgorithmNames() = delete;
        [[nodiscard]] static auto Rsa();
        [[nodiscard]] static auto Dsa();
        [[nodiscard]] static auto Ecdh256();
        [[nodiscard]] static auto Ecdh384();
        [[nodiscard]] static auto Ecdh521();
        [[nodiscard]] static auto Ecdsa256();
        [[nodiscard]] static auto Ecdsa384();
        [[nodiscard]] static auto Ecdsa521();
        [[nodiscard]] static auto Ecdsa();
        [[nodiscard]] static auto Ecdh();
    };
    struct KeyAttestationHelper
    {
        KeyAttestationHelper() = delete;
        static auto DecryptTpmAttestationCredentialAsync(param::hstring const& credential);
        static auto GetTpmAttestationCredentialId(param::hstring const& credential);
        static auto DecryptTpmAttestationCredentialAsync(param::hstring const& credential, param::hstring const& containerName);
    };
    struct KeyStorageProviderNames
    {
        KeyStorageProviderNames() = delete;
        [[nodiscard]] static auto SoftwareKeyStorageProvider();
        [[nodiscard]] static auto SmartcardKeyStorageProvider();
        [[nodiscard]] static auto PlatformKeyStorageProvider();
        [[nodiscard]] static auto PassportKeyStorageProvider();
    };
    struct WINRT_IMPL_EMPTY_BASES PfxImportParameters : winrt::Windows::Security::Cryptography::Certificates::IPfxImportParameters
    {
        PfxImportParameters(std::nullptr_t) noexcept {}
        PfxImportParameters(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Security::Cryptography::Certificates::IPfxImportParameters(ptr, take_ownership_from_abi) {}
        PfxImportParameters();
    };
    struct StandardCertificateStoreNames
    {
        StandardCertificateStoreNames() = delete;
        [[nodiscard]] static auto Personal();
        [[nodiscard]] static auto TrustedRootCertificationAuthorities();
        [[nodiscard]] static auto IntermediateCertificationAuthorities();
    };
    struct WINRT_IMPL_EMPTY_BASES SubjectAlternativeNameInfo : winrt::Windows::Security::Cryptography::Certificates::ISubjectAlternativeNameInfo,
        impl::require<SubjectAlternativeNameInfo, winrt::Windows::Security::Cryptography::Certificates::ISubjectAlternativeNameInfo2>
    {
        SubjectAlternativeNameInfo(std::nullptr_t) noexcept {}
        SubjectAlternativeNameInfo(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Security::Cryptography::Certificates::ISubjectAlternativeNameInfo(ptr, take_ownership_from_abi) {}
        SubjectAlternativeNameInfo();
    };
    struct WINRT_IMPL_EMPTY_BASES UserCertificateEnrollmentManager : winrt::Windows::Security::Cryptography::Certificates::IUserCertificateEnrollmentManager,
        impl::require<UserCertificateEnrollmentManager, winrt::Windows::Security::Cryptography::Certificates::IUserCertificateEnrollmentManager2>
    {
        UserCertificateEnrollmentManager(std::nullptr_t) noexcept {}
        UserCertificateEnrollmentManager(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Security::Cryptography::Certificates::IUserCertificateEnrollmentManager(ptr, take_ownership_from_abi) {}
        using winrt::Windows::Security::Cryptography::Certificates::IUserCertificateEnrollmentManager::ImportPfxDataAsync;
        using impl::consume_t<UserCertificateEnrollmentManager, winrt::Windows::Security::Cryptography::Certificates::IUserCertificateEnrollmentManager2>::ImportPfxDataAsync;
    };
    struct WINRT_IMPL_EMPTY_BASES UserCertificateStore : winrt::Windows::Security::Cryptography::Certificates::IUserCertificateStore
    {
        UserCertificateStore(std::nullptr_t) noexcept {}
        UserCertificateStore(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Security::Cryptography::Certificates::IUserCertificateStore(ptr, take_ownership_from_abi) {}
    };
}
#endif
