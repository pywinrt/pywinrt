# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from . import _winrt_windows_graphics_printing_workflow

class PdlConversionHostBasedProcessingOperations(enum.IntFlag):
    NONE = 0
    PAGE_ROTATION = 0x1
    PAGE_ORDERING = 0x2
    COPIES = 0x4
    BLANK_PAGE_INSERTION = 0x8
    ALL = 0xffffffff

class PrintWorkflowAttributesMergePolicy(enum.IntEnum):
    MERGE_PREFER_PRINT_TICKET_ON_CONFLICT = 0
    MERGE_PREFER_PSA_ON_CONFLICT = 1
    DO_NOT_MERGE_WITH_PRINT_TICKET = 2

class PrintWorkflowJobAbortReason(enum.IntEnum):
    JOB_FAILED = 0
    USER_CANCELED = 1

class PrintWorkflowPdlConversionType(enum.IntEnum):
    XPS_TO_PDF = 0
    XPS_TO_PWGR = 1
    XPS_TO_PCLM = 2

class PrintWorkflowPrinterJobStatus(enum.IntEnum):
    ERROR = 0
    ABORTED = 1
    IN_PROGRESS = 2
    COMPLETED = 3

class PrintWorkflowSessionStatus(enum.IntEnum):
    STARTED = 0
    COMPLETED = 1
    ABORTED = 2
    CLOSED = 3
    PDL_DATA_AVAILABLE_FOR_MODIFICATION = 4

class PrintWorkflowSubmittedStatus(enum.IntEnum):
    SUCCEEDED = 0
    CANCELED = 1
    FAILED = 2

class PrintWorkflowUICompletionStatus(enum.IntEnum):
    COMPLETED = 0
    LAUNCH_FAILED = 1
    JOB_FAILED = 2
    USER_CANCELED = 3

PrintWorkflowBackgroundSession = _winrt_windows_graphics_printing_workflow.PrintWorkflowBackgroundSession
PrintWorkflowBackgroundSetupRequestedEventArgs = _winrt_windows_graphics_printing_workflow.PrintWorkflowBackgroundSetupRequestedEventArgs
PrintWorkflowConfiguration = _winrt_windows_graphics_printing_workflow.PrintWorkflowConfiguration
PrintWorkflowForegroundSession = _winrt_windows_graphics_printing_workflow.PrintWorkflowForegroundSession
PrintWorkflowForegroundSetupRequestedEventArgs = _winrt_windows_graphics_printing_workflow.PrintWorkflowForegroundSetupRequestedEventArgs
PrintWorkflowJobActivatedEventArgs = _winrt_windows_graphics_printing_workflow.PrintWorkflowJobActivatedEventArgs
PrintWorkflowJobBackgroundSession = _winrt_windows_graphics_printing_workflow.PrintWorkflowJobBackgroundSession
PrintWorkflowJobNotificationEventArgs = _winrt_windows_graphics_printing_workflow.PrintWorkflowJobNotificationEventArgs
PrintWorkflowJobStartingEventArgs = _winrt_windows_graphics_printing_workflow.PrintWorkflowJobStartingEventArgs
PrintWorkflowJobTriggerDetails = _winrt_windows_graphics_printing_workflow.PrintWorkflowJobTriggerDetails
PrintWorkflowJobUISession = _winrt_windows_graphics_printing_workflow.PrintWorkflowJobUISession
PrintWorkflowObjectModelSourceFileContent = _winrt_windows_graphics_printing_workflow.PrintWorkflowObjectModelSourceFileContent
PrintWorkflowObjectModelTargetPackage = _winrt_windows_graphics_printing_workflow.PrintWorkflowObjectModelTargetPackage
PrintWorkflowPdlConverter = _winrt_windows_graphics_printing_workflow.PrintWorkflowPdlConverter
PrintWorkflowPdlDataAvailableEventArgs = _winrt_windows_graphics_printing_workflow.PrintWorkflowPdlDataAvailableEventArgs
PrintWorkflowPdlModificationRequestedEventArgs = _winrt_windows_graphics_printing_workflow.PrintWorkflowPdlModificationRequestedEventArgs
PrintWorkflowPdlSourceContent = _winrt_windows_graphics_printing_workflow.PrintWorkflowPdlSourceContent
PrintWorkflowPdlTargetStream = _winrt_windows_graphics_printing_workflow.PrintWorkflowPdlTargetStream
PrintWorkflowPrinterJob = _winrt_windows_graphics_printing_workflow.PrintWorkflowPrinterJob
PrintWorkflowSourceContent = _winrt_windows_graphics_printing_workflow.PrintWorkflowSourceContent
PrintWorkflowSpoolStreamContent = _winrt_windows_graphics_printing_workflow.PrintWorkflowSpoolStreamContent
PrintWorkflowStreamTarget = _winrt_windows_graphics_printing_workflow.PrintWorkflowStreamTarget
PrintWorkflowSubmittedEventArgs = _winrt_windows_graphics_printing_workflow.PrintWorkflowSubmittedEventArgs
PrintWorkflowSubmittedOperation = _winrt_windows_graphics_printing_workflow.PrintWorkflowSubmittedOperation
PrintWorkflowTarget = _winrt_windows_graphics_printing_workflow.PrintWorkflowTarget
PrintWorkflowTriggerDetails = _winrt_windows_graphics_printing_workflow.PrintWorkflowTriggerDetails
PrintWorkflowUIActivatedEventArgs = _winrt_windows_graphics_printing_workflow.PrintWorkflowUIActivatedEventArgs
PrintWorkflowUILauncher = _winrt_windows_graphics_printing_workflow.PrintWorkflowUILauncher
PrintWorkflowXpsDataAvailableEventArgs = _winrt_windows_graphics_printing_workflow.PrintWorkflowXpsDataAvailableEventArgs