# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winrt._winrt_windows_devices_printers import (
    IppAttributeError,
    IppAttributeValue,
    IppIntegerRange,
    IppPrintDevice,
    IppResolution,
    IppSetAttributesResult,
    IppTextWithLanguage,
    PageConfigurationSettings,
    PdlPassthroughProvider,
    PdlPassthroughTarget,
    Print3DDevice,
    PrintSchema,
    VirtualPrinterInstallationParameters,
    VirtualPrinterInstallationResult,
    VirtualPrinterManager,
    VirtualPrinterSupportedFormat,
)

__all__ = [
    "IppAttributeErrorReason",
    "IppAttributeValueKind",
    "IppPrintDeviceKind",
    "IppResolutionUnit",
    "PageConfigurationSource",
    "VirtualPrinterInstallationStatus",
    "VirtualPrinterPreferredInputFormat",
    "IppAttributeError",
    "IppAttributeValue",
    "IppIntegerRange",
    "IppPrintDevice",
    "IppResolution",
    "IppSetAttributesResult",
    "IppTextWithLanguage",
    "PageConfigurationSettings",
    "PdlPassthroughProvider",
    "PdlPassthroughTarget",
    "Print3DDevice",
    "PrintSchema",
    "VirtualPrinterInstallationParameters",
    "VirtualPrinterInstallationResult",
    "VirtualPrinterManager",
    "VirtualPrinterSupportedFormat",
]

class IppAttributeErrorReason(enum.IntEnum):
    REQUEST_ENTITY_TOO_LARGE = 0
    ATTRIBUTE_NOT_SUPPORTED = 1
    ATTRIBUTE_VALUES_NOT_SUPPORTED = 2
    ATTRIBUTE_NOT_SETTABLE = 3
    CONFLICTING_ATTRIBUTES = 4

class IppAttributeValueKind(enum.IntEnum):
    UNSUPPORTED = 0
    UNKNOWN = 1
    NO_VALUE = 2
    INTEGER = 3
    BOOLEAN = 4
    ENUM = 5
    OCTET_STRING = 6
    DATE_TIME = 7
    RESOLUTION = 8
    RANGE_OF_INTEGER = 9
    COLLECTION = 10
    TEXT_WITH_LANGUAGE = 11
    NAME_WITH_LANGUAGE = 12
    TEXT_WITHOUT_LANGUAGE = 13
    NAME_WITHOUT_LANGUAGE = 14
    KEYWORD = 15
    URI = 16
    URI_SCHEMA = 17
    CHARSET = 18
    NATURAL_LANGUAGE = 19
    MIME_MEDIA_TYPE = 20

class IppPrintDeviceKind(enum.IntEnum):
    PRINTER = 0
    FAX_OUT = 1
    VIRTUAL_PRINTER = 2

class IppResolutionUnit(enum.IntEnum):
    DOTS_PER_INCH = 0
    DOTS_PER_CENTIMETER = 1

class PageConfigurationSource(enum.IntEnum):
    PRINT_JOB_CONFIGURATION = 0
    PDL_CONTENT = 1

class VirtualPrinterInstallationStatus(enum.IntEnum):
    INSTALLATION_SUCCEEDED = 0
    PRINTER_ALREADY_INSTALLED = 1
    PRINTER_INSTALLATION_ACCESS_DENIED = 2
    PRINTER_INSTALLATION_FAILED = 3

class VirtualPrinterPreferredInputFormat(enum.IntEnum):
    OPEN_XPS = 0
    POST_SCRIPT = 1

