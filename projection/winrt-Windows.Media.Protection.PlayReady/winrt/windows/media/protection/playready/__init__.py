# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winrt import _winrt_windows_media_protection_playready

__all__ = [
    "NDCertificateFeature",
    "NDCertificatePlatformID",
    "NDCertificateType",
    "NDClosedCaptionFormat",
    "NDContentIDType",
    "NDMediaStreamType",
    "NDProximityDetectionType",
    "NDStartAsyncOptions",
    "PlayReadyDecryptorSetup",
    "PlayReadyEncryptionAlgorithm",
    "PlayReadyHardwareDRMFeatures",
    "PlayReadyITADataFormat",
    "NDClient",
    "NDCustomData",
    "NDDownloadEngineNotifier",
    "NDLicenseFetchDescriptor",
    "NDStorageFileHelper",
    "NDStreamParserNotifier",
    "NDTCPMessenger",
    "PlayReadyContentHeader",
    "PlayReadyContentResolver",
    "PlayReadyDomain",
    "PlayReadyDomainIterable",
    "PlayReadyDomainIterator",
    "PlayReadyDomainJoinServiceRequest",
    "PlayReadyDomainLeaveServiceRequest",
    "PlayReadyITADataGenerator",
    "PlayReadyIndividualizationServiceRequest",
    "PlayReadyLicense",
    "PlayReadyLicenseAcquisitionServiceRequest",
    "PlayReadyLicenseIterable",
    "PlayReadyLicenseIterator",
    "PlayReadyLicenseManagement",
    "PlayReadyLicenseSession",
    "PlayReadyMeteringReportServiceRequest",
    "PlayReadyRevocationServiceRequest",
    "PlayReadySecureStopIterable",
    "PlayReadySecureStopIterator",
    "PlayReadySecureStopServiceRequest",
    "PlayReadySoapMessage",
    "PlayReadyStatics",
    "INDClosedCaptionDataReceivedEventArgs",
    "INDCustomData",
    "INDDownloadEngine",
    "INDDownloadEngineNotifier",
    "INDLicenseFetchCompletedEventArgs",
    "INDLicenseFetchDescriptor",
    "INDLicenseFetchResult",
    "INDMessenger",
    "INDProximityDetectionCompletedEventArgs",
    "INDRegistrationCompletedEventArgs",
    "INDSendResult",
    "INDStartResult",
    "INDStorageFileHelper",
    "INDStreamParser",
    "INDStreamParserNotifier",
    "INDTransmitterProperties",
    "IPlayReadyDomain",
    "IPlayReadyLicense",
    "IPlayReadyLicenseAcquisitionServiceRequest",
    "IPlayReadyLicenseSession",
    "IPlayReadyLicenseSession2",
    "IPlayReadySecureStopServiceRequest",
    "IPlayReadyServiceRequest",
]

class NDCertificateFeature(enum.IntEnum):
    TRANSMITTER = 1
    RECEIVER = 2
    SHARED_CERTIFICATE = 3
    SECURE_CLOCK = 4
    ANTI_ROLL_BACK_CLOCK = 5
    C_R_L_S = 9
    PLAY_READY3_FEATURES = 13

class NDCertificatePlatformID(enum.IntEnum):
    WINDOWS = 0
    O_S_X = 1
    WINDOWS_ON_A_R_M = 2
    WINDOWS_MOBILE7 = 5
    I_O_S_ON_A_R_M = 6
    X_BOX_ON_P_P_C = 7
    WINDOWS_PHONE8_ON_A_R_M = 8
    WINDOWS_PHONE8_ON_X86 = 9
    XBOX_ONE = 10
    ANDROID_ON_A_R_M = 11
    WINDOWS_PHONE81_ON_A_R_M = 12
    WINDOWS_PHONE81_ON_X86 = 13

class NDCertificateType(enum.IntEnum):
    UNKNOWN = 0
    P_C = 1
    DEVICE = 2
    DOMAIN = 3
    ISSUER = 4
    CRL_SIGNER = 5
    SERVICE = 6
    SILVERLIGHT = 7
    APPLICATION = 8
    METERING = 9
    KEY_FILE_SIGNER = 10
    SERVER = 11
    LICENSE_SIGNER = 12

class NDClosedCaptionFormat(enum.IntEnum):
    A_T_S_C = 0
    S_C_T_E20 = 1
    UNKNOWN = 2

class NDContentIDType(enum.IntEnum):
    KEY_I_D = 1
    PLAY_READY_OBJECT = 2
    CUSTOM = 3

class NDMediaStreamType(enum.IntEnum):
    AUDIO = 1
    VIDEO = 2

class NDProximityDetectionType(enum.IntEnum):
    U_D_P = 1
    T_C_P = 2
    TRANSPORT_AGNOSTIC = 4

class NDStartAsyncOptions(enum.IntEnum):
    MUTUAL_AUTHENTICATION = 1
    WAIT_FOR_LICENSE_DESCRIPTOR = 2

class PlayReadyDecryptorSetup(enum.IntEnum):
    UNINITIALIZED = 0
    ON_DEMAND = 1

class PlayReadyEncryptionAlgorithm(enum.IntEnum):
    UNPROTECTED = 0
    AES128_CTR = 1
    COCKTAIL = 4
    AES128_CBC = 5
    UNSPECIFIED = 65535
    UNINITIALIZED = 2147483647

class PlayReadyHardwareDRMFeatures(enum.IntEnum):
    HARDWARE_D_R_M = 1
    H_E_V_C = 2
    AES128_CBC = 3

class PlayReadyITADataFormat(enum.IntEnum):
    SERIALIZED_PROPERTIES = 0
    SERIALIZED_PROPERTIES_WITH_CONTENT_PROTECTION_WRAPPER = 1

NDClient = _winrt_windows_media_protection_playready.NDClient
NDCustomData = _winrt_windows_media_protection_playready.NDCustomData
NDDownloadEngineNotifier = _winrt_windows_media_protection_playready.NDDownloadEngineNotifier
NDLicenseFetchDescriptor = _winrt_windows_media_protection_playready.NDLicenseFetchDescriptor
NDStorageFileHelper = _winrt_windows_media_protection_playready.NDStorageFileHelper
NDStreamParserNotifier = _winrt_windows_media_protection_playready.NDStreamParserNotifier
NDTCPMessenger = _winrt_windows_media_protection_playready.NDTCPMessenger
PlayReadyContentHeader = _winrt_windows_media_protection_playready.PlayReadyContentHeader
PlayReadyContentResolver = _winrt_windows_media_protection_playready.PlayReadyContentResolver
PlayReadyDomain = _winrt_windows_media_protection_playready.PlayReadyDomain
PlayReadyDomainIterable = _winrt_windows_media_protection_playready.PlayReadyDomainIterable
PlayReadyDomainIterator = _winrt_windows_media_protection_playready.PlayReadyDomainIterator
PlayReadyDomainJoinServiceRequest = _winrt_windows_media_protection_playready.PlayReadyDomainJoinServiceRequest
PlayReadyDomainLeaveServiceRequest = _winrt_windows_media_protection_playready.PlayReadyDomainLeaveServiceRequest
PlayReadyITADataGenerator = _winrt_windows_media_protection_playready.PlayReadyITADataGenerator
PlayReadyIndividualizationServiceRequest = _winrt_windows_media_protection_playready.PlayReadyIndividualizationServiceRequest
PlayReadyLicense = _winrt_windows_media_protection_playready.PlayReadyLicense
PlayReadyLicenseAcquisitionServiceRequest = _winrt_windows_media_protection_playready.PlayReadyLicenseAcquisitionServiceRequest
PlayReadyLicenseIterable = _winrt_windows_media_protection_playready.PlayReadyLicenseIterable
PlayReadyLicenseIterator = _winrt_windows_media_protection_playready.PlayReadyLicenseIterator
PlayReadyLicenseManagement = _winrt_windows_media_protection_playready.PlayReadyLicenseManagement
PlayReadyLicenseSession = _winrt_windows_media_protection_playready.PlayReadyLicenseSession
PlayReadyMeteringReportServiceRequest = _winrt_windows_media_protection_playready.PlayReadyMeteringReportServiceRequest
PlayReadyRevocationServiceRequest = _winrt_windows_media_protection_playready.PlayReadyRevocationServiceRequest
PlayReadySecureStopIterable = _winrt_windows_media_protection_playready.PlayReadySecureStopIterable
PlayReadySecureStopIterator = _winrt_windows_media_protection_playready.PlayReadySecureStopIterator
PlayReadySecureStopServiceRequest = _winrt_windows_media_protection_playready.PlayReadySecureStopServiceRequest
PlayReadySoapMessage = _winrt_windows_media_protection_playready.PlayReadySoapMessage
PlayReadyStatics = _winrt_windows_media_protection_playready.PlayReadyStatics
INDClosedCaptionDataReceivedEventArgs = _winrt_windows_media_protection_playready.INDClosedCaptionDataReceivedEventArgs
INDCustomData = _winrt_windows_media_protection_playready.INDCustomData
INDDownloadEngine = _winrt_windows_media_protection_playready.INDDownloadEngine
INDDownloadEngineNotifier = _winrt_windows_media_protection_playready.INDDownloadEngineNotifier
INDLicenseFetchCompletedEventArgs = _winrt_windows_media_protection_playready.INDLicenseFetchCompletedEventArgs
INDLicenseFetchDescriptor = _winrt_windows_media_protection_playready.INDLicenseFetchDescriptor
INDLicenseFetchResult = _winrt_windows_media_protection_playready.INDLicenseFetchResult
INDMessenger = _winrt_windows_media_protection_playready.INDMessenger
INDProximityDetectionCompletedEventArgs = _winrt_windows_media_protection_playready.INDProximityDetectionCompletedEventArgs
INDRegistrationCompletedEventArgs = _winrt_windows_media_protection_playready.INDRegistrationCompletedEventArgs
INDSendResult = _winrt_windows_media_protection_playready.INDSendResult
INDStartResult = _winrt_windows_media_protection_playready.INDStartResult
INDStorageFileHelper = _winrt_windows_media_protection_playready.INDStorageFileHelper
INDStreamParser = _winrt_windows_media_protection_playready.INDStreamParser
INDStreamParserNotifier = _winrt_windows_media_protection_playready.INDStreamParserNotifier
INDTransmitterProperties = _winrt_windows_media_protection_playready.INDTransmitterProperties
IPlayReadyDomain = _winrt_windows_media_protection_playready.IPlayReadyDomain
IPlayReadyLicense = _winrt_windows_media_protection_playready.IPlayReadyLicense
IPlayReadyLicenseAcquisitionServiceRequest = _winrt_windows_media_protection_playready.IPlayReadyLicenseAcquisitionServiceRequest
IPlayReadyLicenseSession = _winrt_windows_media_protection_playready.IPlayReadyLicenseSession
IPlayReadyLicenseSession2 = _winrt_windows_media_protection_playready.IPlayReadyLicenseSession2
IPlayReadySecureStopServiceRequest = _winrt_windows_media_protection_playready.IPlayReadySecureStopServiceRequest
IPlayReadyServiceRequest = _winrt_windows_media_protection_playready.IPlayReadyServiceRequest
