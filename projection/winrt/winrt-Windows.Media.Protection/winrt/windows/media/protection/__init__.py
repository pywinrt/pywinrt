# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum
import typing
import uuid as _uuid

import winrt.system
from winrt._winrt_windows_media_protection import (
    ComponentLoadFailedEventArgs,
    ComponentRenewal,
    HdcpSession,
    MediaProtectionManager,
    MediaProtectionPMPServer,
    MediaProtectionServiceCompletion,
    ProtectionCapabilities,
    RevocationAndRenewalInformation,
    RevocationAndRenewalItem,
    ServiceRequestedEventArgs,
    _IMediaProtectionServiceRequest,
    IMediaProtectionServiceRequest,
)

__all__ = [
    "GraphicsTrustStatus",
    "HdcpProtection",
    "HdcpSetProtectionResult",
    "ProtectionCapabilityResult",
    "RenewalStatus",
    "RevocationAndRenewalReasons",
    "ComponentLoadFailedEventArgs",
    "ComponentRenewal",
    "HdcpSession",
    "MediaProtectionManager",
    "MediaProtectionPMPServer",
    "MediaProtectionServiceCompletion",
    "ProtectionCapabilities",
    "RevocationAndRenewalInformation",
    "RevocationAndRenewalItem",
    "ServiceRequestedEventArgs",
    "IMediaProtectionServiceRequest",
    "ComponentLoadFailedEventHandler",
    "RebootNeededEventHandler",
    "ServiceRequestedEventHandler",
]

class GraphicsTrustStatus(enum.IntEnum):
    TRUST_NOT_REQUIRED = 0
    TRUST_ESTABLISHED = 1
    ENVIRONMENT_NOT_SUPPORTED = 2
    DRIVER_NOT_SUPPORTED = 3
    DRIVER_SIGNING_FAILURE = 4
    UNKNOWN_FAILURE = 5

class HdcpProtection(enum.IntEnum):
    OFF = 0
    ON = 1
    ON_WITH_TYPE_ENFORCEMENT = 2

class HdcpSetProtectionResult(enum.IntEnum):
    SUCCESS = 0
    TIMED_OUT = 1
    NOT_SUPPORTED = 2
    UNKNOWN_FAILURE = 3

class ProtectionCapabilityResult(enum.IntEnum):
    NOT_SUPPORTED = 0
    MAYBE = 1
    PROBABLY = 2

class RenewalStatus(enum.IntEnum):
    NOT_STARTED = 0
    UPDATES_IN_PROGRESS = 1
    USER_CANCELLED = 2
    APP_COMPONENTS_MAY_NEED_UPDATING = 3
    NO_COMPONENTS_FOUND = 4

class RevocationAndRenewalReasons(enum.IntFlag):
    USER_MODE_COMPONENT_LOAD = 0x1
    KERNEL_MODE_COMPONENT_LOAD = 0x2
    APP_COMPONENT = 0x4
    GLOBAL_REVOCATION_LIST_LOAD_FAILED = 0x10
    INVALID_GLOBAL_REVOCATION_LIST_SIGNATURE = 0x20
    GLOBAL_REVOCATION_LIST_ABSENT = 0x1000
    COMPONENT_REVOKED = 0x2000
    INVALID_COMPONENT_CERTIFICATE_EXTENDED_KEY_USE = 0x4000
    COMPONENT_CERTIFICATE_REVOKED = 0x8000
    INVALID_COMPONENT_CERTIFICATE_ROOT = 0x10000
    COMPONENT_HIGH_SECURITY_CERTIFICATE_REVOKED = 0x20000
    COMPONENT_LOW_SECURITY_CERTIFICATE_REVOKED = 0x40000
    BOOT_DRIVER_VERIFICATION_FAILED = 0x100000
    COMPONENT_SIGNED_WITH_TEST_CERTIFICATE = 0x1000000
    ENCRYPTION_FAILURE = 0x10000000

ComponentLoadFailedEventHandler = typing.Callable[[MediaProtectionManager, ComponentLoadFailedEventArgs], None]
RebootNeededEventHandler = typing.Callable[[MediaProtectionManager], None]
ServiceRequestedEventHandler = typing.Callable[[MediaProtectionManager, ServiceRequestedEventArgs], None]
