# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winrt._winrt_windows_applicationmodel import (
    PackageInstallProgress,
    PackageVersion,
    AppDisplayInfo,
    AppInfo,
    AppInstallerInfo,
    AppInstance,
    DesignMode,
    EnteredBackgroundEventArgs,
    FindRelatedPackagesOptions,
    FullTrustProcessLaunchResult,
    FullTrustProcessLauncher,
    LeavingBackgroundEventArgs,
    LimitedAccessFeatureRequestResult,
    LimitedAccessFeatures,
    Package,
    PackageCatalog,
    PackageCatalogAddOptionalPackageResult,
    PackageCatalogAddResourcePackageResult,
    PackageCatalogRemoveOptionalPackagesResult,
    PackageCatalogRemoveResourcePackagesResult,
    PackageContentGroup,
    PackageContentGroupStagingEventArgs,
    PackageId,
    PackageInstallingEventArgs,
    PackageStagingEventArgs,
    PackageStatus,
    PackageStatusChangedEventArgs,
    PackageUninstallingEventArgs,
    PackageUpdateAvailabilityResult,
    PackageUpdatingEventArgs,
    StartupTask,
    SuspendingDeferral,
    SuspendingEventArgs,
    SuspendingOperation,
    _IEnteredBackgroundEventArgs,
    IEnteredBackgroundEventArgs,
    _ILeavingBackgroundEventArgs,
    ILeavingBackgroundEventArgs,
    _IPackageCatalogStatics2,
    IPackageCatalogStatics2,
    _ISuspendingDeferral,
    ISuspendingDeferral,
    _ISuspendingEventArgs,
    ISuspendingEventArgs,
    _ISuspendingOperation,
    ISuspendingOperation,
)

__all__ = [
    "AddResourcePackageOptions",
    "AppExecutionContext",
    "AppInstallerPolicySource",
    "FullTrustLaunchResult",
    "LimitedAccessFeatureStatus",
    "PackageContentGroupState",
    "PackageRelationship",
    "PackageSignatureKind",
    "PackageUpdateAvailability",
    "StartupTaskState",
    "PackageInstallProgress",
    "PackageVersion",
    "AppDisplayInfo",
    "AppInfo",
    "AppInstallerInfo",
    "AppInstance",
    "DesignMode",
    "EnteredBackgroundEventArgs",
    "FindRelatedPackagesOptions",
    "FullTrustProcessLaunchResult",
    "FullTrustProcessLauncher",
    "LeavingBackgroundEventArgs",
    "LimitedAccessFeatureRequestResult",
    "LimitedAccessFeatures",
    "Package",
    "PackageCatalog",
    "PackageCatalogAddOptionalPackageResult",
    "PackageCatalogAddResourcePackageResult",
    "PackageCatalogRemoveOptionalPackagesResult",
    "PackageCatalogRemoveResourcePackagesResult",
    "PackageContentGroup",
    "PackageContentGroupStagingEventArgs",
    "PackageId",
    "PackageInstallingEventArgs",
    "PackageStagingEventArgs",
    "PackageStatus",
    "PackageStatusChangedEventArgs",
    "PackageUninstallingEventArgs",
    "PackageUpdateAvailabilityResult",
    "PackageUpdatingEventArgs",
    "StartupTask",
    "SuspendingDeferral",
    "SuspendingEventArgs",
    "SuspendingOperation",
    "IEnteredBackgroundEventArgs",
    "ILeavingBackgroundEventArgs",
    "IPackageCatalogStatics2",
    "ISuspendingDeferral",
    "ISuspendingEventArgs",
    "ISuspendingOperation",
]

class AddResourcePackageOptions(enum.IntFlag):
    NONE = 0x0
    FORCE_TARGET_APP_SHUTDOWN = 0x1
    APPLY_UPDATE_IF_AVAILABLE = 0x2

class AppExecutionContext(enum.IntEnum):
    UNKNOWN = 0
    HOST = 1
    GUEST = 2

class AppInstallerPolicySource(enum.IntEnum):
    DEFAULT = 0
    SYSTEM = 1

class FullTrustLaunchResult(enum.IntEnum):
    SUCCESS = 0
    ACCESS_DENIED = 1
    FILE_NOT_FOUND = 2
    UNKNOWN = 3

class LimitedAccessFeatureStatus(enum.IntEnum):
    UNAVAILABLE = 0
    AVAILABLE = 1
    AVAILABLE_WITHOUT_TOKEN = 2
    UNKNOWN = 3

class PackageContentGroupState(enum.IntEnum):
    NOT_STAGED = 0
    QUEUED = 1
    STAGING = 2
    STAGED = 3

class PackageRelationship(enum.IntEnum):
    DEPENDENCIES = 0
    DEPENDENTS = 1
    ALL = 2

class PackageSignatureKind(enum.IntEnum):
    NONE = 0
    DEVELOPER = 1
    ENTERPRISE = 2
    STORE = 3
    SYSTEM = 4

class PackageUpdateAvailability(enum.IntEnum):
    UNKNOWN = 0
    NO_UPDATES = 1
    AVAILABLE = 2
    REQUIRED = 3
    ERROR = 4

class StartupTaskState(enum.IntEnum):
    DISABLED = 0
    DISABLED_BY_USER = 1
    ENABLED = 2
    DISABLED_BY_POLICY = 3
    ENABLED_BY_POLICY = 4

