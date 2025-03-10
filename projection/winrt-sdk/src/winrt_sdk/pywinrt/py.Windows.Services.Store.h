// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.Web.Http.h>

#include <winrt/Windows.Services.Store.h>

namespace py::proj::Windows::Services::Store
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Services::Store::StoreCanLicenseStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Services::Store::StoreConsumableStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Services::Store::StoreDurationUnit> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Services::Store::StorePackageUpdateState> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Services::Store::StorePurchaseStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Services::Store::StoreQueueItemExtendedState> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Services::Store::StoreQueueItemKind> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Services::Store::StoreQueueItemState> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Services::Store::StoreRateAndReviewStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Services::Store::StoreUninstallStorePackageStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Services::Store::StorePackageUpdateStatus> = "T{P:package_family_name:Q:package_download_size_in_bytes:Q:package_bytes_downloaded:d:package_download_progress:d:total_download_progress:i:package_update_state:}";


    template<>
    struct py_type<winrt::Windows::Services::Store::StoreCanLicenseStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreCanLicenseStatus";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreCanLicenseStatus";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreConsumableStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreConsumableStatus";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreConsumableStatus";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreDurationUnit>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreDurationUnit";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreDurationUnit";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StorePackageUpdateState>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StorePackageUpdateState";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StorePackageUpdateState";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StorePurchaseStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StorePurchaseStatus";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StorePurchaseStatus";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreQueueItemExtendedState>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreQueueItemExtendedState";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreQueueItemExtendedState";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreQueueItemKind>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreQueueItemKind";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreQueueItemKind";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreQueueItemState>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreQueueItemState";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreQueueItemState";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreRateAndReviewStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreRateAndReviewStatus";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreRateAndReviewStatus";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreUninstallStorePackageStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreUninstallStorePackageStatus";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreUninstallStorePackageStatus";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreAcquireLicenseResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreAcquireLicenseResult";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreAcquireLicenseResult";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreAppLicense>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreAppLicense";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreAppLicense";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreAvailability>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreAvailability";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreAvailability";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreCanAcquireLicenseResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreCanAcquireLicenseResult";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreCanAcquireLicenseResult";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreCollectionData>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreCollectionData";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreCollectionData";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreConsumableResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreConsumableResult";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreConsumableResult";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreContext>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreContext";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreContext";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreImage>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreImage";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreImage";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreLicense>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreLicense";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreLicense";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StorePackageInstallOptions>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StorePackageInstallOptions";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StorePackageInstallOptions";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StorePackageLicense>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StorePackageLicense";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StorePackageLicense";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StorePackageUpdate>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StorePackageUpdate";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StorePackageUpdate";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StorePackageUpdateResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StorePackageUpdateResult";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StorePackageUpdateResult";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StorePrice>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StorePrice";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StorePrice";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreProduct>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreProduct";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreProduct";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreProductOptions>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreProductOptions";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreProductOptions";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreProductPagedQueryResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreProductPagedQueryResult";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreProductPagedQueryResult";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreProductQueryResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreProductQueryResult";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreProductQueryResult";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreProductResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreProductResult";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreProductResult";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StorePurchaseProperties>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StorePurchaseProperties";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StorePurchaseProperties";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StorePurchaseResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StorePurchaseResult";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StorePurchaseResult";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreQueueItem>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreQueueItem";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreQueueItem";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreQueueItemCompletedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreQueueItemCompletedEventArgs";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreQueueItemCompletedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreQueueItemStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreQueueItemStatus";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreQueueItemStatus";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreRateAndReviewResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreRateAndReviewResult";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreRateAndReviewResult";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreRequestHelper>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreRequestHelper";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreRequestHelper";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreSendRequestResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreSendRequestResult";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreSendRequestResult";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreSku>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreSku";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreSku";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreSubscriptionInfo>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreSubscriptionInfo";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreSubscriptionInfo";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreUninstallStorePackageResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreUninstallStorePackageResult";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreUninstallStorePackageResult";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StoreVideo>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StoreVideo";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StoreVideo";
    };

    template<>
    struct py_type<winrt::Windows::Services::Store::StorePackageUpdateStatus>
    {
        static constexpr std::string_view from_tuple = "winrt._winrt_windows_services_store.StorePackageUpdateStatus_from_tuple";
        static constexpr std::string_view qualified_name = "winrt.windows.services.store.StorePackageUpdateStatus";
        static constexpr const char* module_name = "winrt.windows.services.store";
        static constexpr const char* type_name = "StorePackageUpdateStatus";
    };
}

#if __has_include("py.Windows.ApplicationModel.h")
#include "py.Windows.ApplicationModel.h"
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

#if __has_include("py.Windows.Web.Http.h")
#include "py.Windows.Web.Http.h"
#endif

namespace py::impl::Windows::Services::Store
{
}

namespace py::wrapper::Windows::Services::Store
{
    using StoreAcquireLicenseResult = py::winrt_wrapper<winrt::Windows::Services::Store::StoreAcquireLicenseResult>;
    using StoreAppLicense = py::winrt_wrapper<winrt::Windows::Services::Store::StoreAppLicense>;
    using StoreAvailability = py::winrt_wrapper<winrt::Windows::Services::Store::StoreAvailability>;
    using StoreCanAcquireLicenseResult = py::winrt_wrapper<winrt::Windows::Services::Store::StoreCanAcquireLicenseResult>;
    using StoreCollectionData = py::winrt_wrapper<winrt::Windows::Services::Store::StoreCollectionData>;
    using StoreConsumableResult = py::winrt_wrapper<winrt::Windows::Services::Store::StoreConsumableResult>;
    using StoreContext = py::winrt_wrapper<winrt::Windows::Services::Store::StoreContext>;
    using StoreImage = py::winrt_wrapper<winrt::Windows::Services::Store::StoreImage>;
    using StoreLicense = py::winrt_wrapper<winrt::Windows::Services::Store::StoreLicense>;
    using StorePackageInstallOptions = py::winrt_wrapper<winrt::Windows::Services::Store::StorePackageInstallOptions>;
    using StorePackageLicense = py::winrt_wrapper<winrt::Windows::Services::Store::StorePackageLicense>;
    using StorePackageUpdate = py::winrt_wrapper<winrt::Windows::Services::Store::StorePackageUpdate>;
    using StorePackageUpdateResult = py::winrt_wrapper<winrt::Windows::Services::Store::StorePackageUpdateResult>;
    using StorePrice = py::winrt_wrapper<winrt::Windows::Services::Store::StorePrice>;
    using StoreProduct = py::winrt_wrapper<winrt::Windows::Services::Store::StoreProduct>;
    using StoreProductOptions = py::winrt_wrapper<winrt::Windows::Services::Store::StoreProductOptions>;
    using StoreProductPagedQueryResult = py::winrt_wrapper<winrt::Windows::Services::Store::StoreProductPagedQueryResult>;
    using StoreProductQueryResult = py::winrt_wrapper<winrt::Windows::Services::Store::StoreProductQueryResult>;
    using StoreProductResult = py::winrt_wrapper<winrt::Windows::Services::Store::StoreProductResult>;
    using StorePurchaseProperties = py::winrt_wrapper<winrt::Windows::Services::Store::StorePurchaseProperties>;
    using StorePurchaseResult = py::winrt_wrapper<winrt::Windows::Services::Store::StorePurchaseResult>;
    using StoreQueueItem = py::winrt_wrapper<winrt::Windows::Services::Store::StoreQueueItem>;
    using StoreQueueItemCompletedEventArgs = py::winrt_wrapper<winrt::Windows::Services::Store::StoreQueueItemCompletedEventArgs>;
    using StoreQueueItemStatus = py::winrt_wrapper<winrt::Windows::Services::Store::StoreQueueItemStatus>;
    using StoreRateAndReviewResult = py::winrt_wrapper<winrt::Windows::Services::Store::StoreRateAndReviewResult>;
    using StoreRequestHelper = py::winrt_wrapper<winrt::Windows::Services::Store::StoreRequestHelper>;
    using StoreSendRequestResult = py::winrt_wrapper<winrt::Windows::Services::Store::StoreSendRequestResult>;
    using StoreSku = py::winrt_wrapper<winrt::Windows::Services::Store::StoreSku>;
    using StoreSubscriptionInfo = py::winrt_wrapper<winrt::Windows::Services::Store::StoreSubscriptionInfo>;
    using StoreUninstallStorePackageResult = py::winrt_wrapper<winrt::Windows::Services::Store::StoreUninstallStorePackageResult>;
    using StoreVideo = py::winrt_wrapper<winrt::Windows::Services::Store::StoreVideo>;
    using StorePackageUpdateStatus = py::winrt_struct_wrapper<winrt::Windows::Services::Store::StorePackageUpdateStatus>;
}

namespace py
{
}
