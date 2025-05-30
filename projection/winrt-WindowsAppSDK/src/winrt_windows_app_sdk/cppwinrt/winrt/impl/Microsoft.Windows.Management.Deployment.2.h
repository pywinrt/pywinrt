// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Microsoft_Windows_Management_Deployment_2_H
#define WINRT_Microsoft_Windows_Management_Deployment_2_H
#include "winrt/impl/Microsoft.Windows.Management.Deployment.1.h"
WINRT_EXPORT namespace winrt::Microsoft::Windows::Management::Deployment
{
    struct PackageDeploymentProgress
    {
        winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentProgressStatus Status {};
        double Progress {};
    };
    inline bool operator==(PackageDeploymentProgress const& left, PackageDeploymentProgress const& right) noexcept
    {
        return left.Status == right.Status && left.Progress == right.Progress;
    }
    inline bool operator!=(PackageDeploymentProgress const& left, PackageDeploymentProgress const& right) noexcept
    {
        return !(left == right);
    }
    struct WINRT_IMPL_EMPTY_BASES AddPackageOptions : winrt::Microsoft::Windows::Management::Deployment::IAddPackageOptions
    {
        AddPackageOptions(std::nullptr_t) noexcept {}
        AddPackageOptions(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::Management::Deployment::IAddPackageOptions(ptr, take_ownership_from_abi) {}
        AddPackageOptions();
    };
    struct WINRT_IMPL_EMPTY_BASES EnsureReadyOptions : winrt::Microsoft::Windows::Management::Deployment::IEnsureReadyOptions,
        impl::require<EnsureReadyOptions, winrt::Microsoft::Windows::Management::Deployment::IEnsureReadyOptions2>
    {
        EnsureReadyOptions(std::nullptr_t) noexcept {}
        EnsureReadyOptions(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::Management::Deployment::IEnsureReadyOptions(ptr, take_ownership_from_abi) {}
        EnsureReadyOptions();
    };
    struct WINRT_IMPL_EMPTY_BASES PackageDeploymentManager : winrt::Microsoft::Windows::Management::Deployment::IPackageDeploymentManager,
        impl::require<PackageDeploymentManager, winrt::Microsoft::Windows::Management::Deployment::IPackageDeploymentManager2>
    {
        PackageDeploymentManager(std::nullptr_t) noexcept {}
        PackageDeploymentManager(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::Management::Deployment::IPackageDeploymentManager(ptr, take_ownership_from_abi) {}
        static auto GetDefault();
        static auto IsPackageDeploymentFeatureSupported(winrt::Microsoft::Windows::Management::Deployment::PackageDeploymentFeature const& feature);
    };
    struct WINRT_IMPL_EMPTY_BASES PackageDeploymentResult : winrt::Microsoft::Windows::Management::Deployment::IPackageDeploymentResult
    {
        PackageDeploymentResult(std::nullptr_t) noexcept {}
        PackageDeploymentResult(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::Management::Deployment::IPackageDeploymentResult(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES PackageRuntimeManager : winrt::Microsoft::Windows::Management::Deployment::IPackageRuntimeManager
    {
        PackageRuntimeManager(std::nullptr_t) noexcept {}
        PackageRuntimeManager(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::Management::Deployment::IPackageRuntimeManager(ptr, take_ownership_from_abi) {}
        static auto GetDefault();
    };
    struct WINRT_IMPL_EMPTY_BASES PackageSet : winrt::Microsoft::Windows::Management::Deployment::IPackageSet
    {
        PackageSet(std::nullptr_t) noexcept {}
        PackageSet(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::Management::Deployment::IPackageSet(ptr, take_ownership_from_abi) {}
        PackageSet();
    };
    struct WINRT_IMPL_EMPTY_BASES PackageSetItem : winrt::Microsoft::Windows::Management::Deployment::IPackageSetItem
    {
        PackageSetItem(std::nullptr_t) noexcept {}
        PackageSetItem(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::Management::Deployment::IPackageSetItem(ptr, take_ownership_from_abi) {}
        PackageSetItem();
    };
    struct WINRT_IMPL_EMPTY_BASES PackageSetItemRuntimeDisposition : winrt::Microsoft::Windows::Management::Deployment::IPackageSetItemRuntimeDisposition
    {
        PackageSetItemRuntimeDisposition(std::nullptr_t) noexcept {}
        PackageSetItemRuntimeDisposition(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::Management::Deployment::IPackageSetItemRuntimeDisposition(ptr, take_ownership_from_abi) {}
        PackageSetItemRuntimeDisposition();
    };
    struct WINRT_IMPL_EMPTY_BASES PackageSetRuntimeDisposition : winrt::Microsoft::Windows::Management::Deployment::IPackageSetRuntimeDisposition
    {
        PackageSetRuntimeDisposition(std::nullptr_t) noexcept {}
        PackageSetRuntimeDisposition(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::Management::Deployment::IPackageSetRuntimeDisposition(ptr, take_ownership_from_abi) {}
        PackageSetRuntimeDisposition();
    };
    struct WINRT_IMPL_EMPTY_BASES PackageVolume : winrt::Microsoft::Windows::Management::Deployment::IPackageVolume
    {
        PackageVolume(std::nullptr_t) noexcept {}
        PackageVolume(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::Management::Deployment::IPackageVolume(ptr, take_ownership_from_abi) {}
        static auto FindPackageVolumes();
        static auto FindPackageVolumeByPath(param::hstring const& packageStorePath);
        static auto FindPackageVolumeByName(param::hstring const& name);
    };
    struct WINRT_IMPL_EMPTY_BASES ProvisionPackageOptions : winrt::Microsoft::Windows::Management::Deployment::IProvisionPackageOptions
    {
        ProvisionPackageOptions(std::nullptr_t) noexcept {}
        ProvisionPackageOptions(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::Management::Deployment::IProvisionPackageOptions(ptr, take_ownership_from_abi) {}
        ProvisionPackageOptions();
    };
    struct WINRT_IMPL_EMPTY_BASES RegisterPackageOptions : winrt::Microsoft::Windows::Management::Deployment::IRegisterPackageOptions
    {
        RegisterPackageOptions(std::nullptr_t) noexcept {}
        RegisterPackageOptions(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::Management::Deployment::IRegisterPackageOptions(ptr, take_ownership_from_abi) {}
        RegisterPackageOptions();
    };
    struct WINRT_IMPL_EMPTY_BASES RemovePackageOptions : winrt::Microsoft::Windows::Management::Deployment::IRemovePackageOptions
    {
        RemovePackageOptions(std::nullptr_t) noexcept {}
        RemovePackageOptions(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::Management::Deployment::IRemovePackageOptions(ptr, take_ownership_from_abi) {}
        RemovePackageOptions();
    };
    struct WINRT_IMPL_EMPTY_BASES StagePackageOptions : winrt::Microsoft::Windows::Management::Deployment::IStagePackageOptions
    {
        StagePackageOptions(std::nullptr_t) noexcept {}
        StagePackageOptions(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::Windows::Management::Deployment::IStagePackageOptions(ptr, take_ownership_from_abi) {}
        StagePackageOptions();
    };
}
#endif
