# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.data.json as windows_data_json
import winrt.windows.foundation as windows_foundation
import winrt.windows.foundation.collections as windows_foundation_collections
import winrt.windows.system as windows_system

from winrt.windows.system.diagnostics import DiagnosticActionState

Self = typing.TypeVar('Self')

@typing.final
class DiagnosticActionResult(winrt.system.Object):
    # Windows.Foundation.HResult Windows.System.Diagnostics.DiagnosticActionResult::get_ExtendedError()
    @_property
    def extended_error(self) -> windows_foundation.HResult: ...
    # Windows.Foundation.Collections.ValueSet Windows.System.Diagnostics.DiagnosticActionResult::get_Results()
    @_property
    def results(self) -> windows_foundation_collections.ValueSet: ...

@typing.final
class DiagnosticInvoker_Static(winrt._winrt.IInspectable_Static):
    # Windows.System.Diagnostics.DiagnosticInvoker Windows.System.Diagnostics.DiagnosticInvoker::GetDefault()
    def get_default(cls) -> DiagnosticInvoker: ...
    # Windows.System.Diagnostics.DiagnosticInvoker Windows.System.Diagnostics.DiagnosticInvoker::GetForUser(Windows.System.User)
    def get_for_user(cls, user: windows_system.User, /) -> DiagnosticInvoker: ...
    # System.Boolean Windows.System.Diagnostics.DiagnosticInvoker::get_IsSupported()
    @_property
    def is_supported(cls) -> bool: ...

@typing.final
class DiagnosticInvoker(winrt.system.Object, metaclass=DiagnosticInvoker_Static):
    # Windows.Foundation.IAsyncOperationWithProgress`2<Windows.System.Diagnostics.DiagnosticActionResult,Windows.System.Diagnostics.DiagnosticActionState> Windows.System.Diagnostics.DiagnosticInvoker::RunDiagnosticActionAsync(Windows.Data.Json.JsonObject)
    def run_diagnostic_action_async(self, context: windows_data_json.JsonObject, /) -> windows_foundation.IAsyncOperationWithProgress[DiagnosticActionResult, DiagnosticActionState]: ...
    # Windows.Foundation.IAsyncOperationWithProgress`2<Windows.System.Diagnostics.DiagnosticActionResult,Windows.System.Diagnostics.DiagnosticActionState> Windows.System.Diagnostics.DiagnosticInvoker::RunDiagnosticActionFromStringAsync(System.String)
    def run_diagnostic_action_from_string_async(self, context: str, /) -> windows_foundation.IAsyncOperationWithProgress[DiagnosticActionResult, DiagnosticActionState]: ...

@typing.final
class ProcessCpuUsage(winrt.system.Object):
    # Windows.System.Diagnostics.ProcessCpuUsageReport Windows.System.Diagnostics.ProcessCpuUsage::GetReport()
    def get_report(self) -> ProcessCpuUsageReport: ...

@typing.final
class ProcessCpuUsageReport(winrt.system.Object):
    # Windows.Foundation.TimeSpan Windows.System.Diagnostics.ProcessCpuUsageReport::get_KernelTime()
    @_property
    def kernel_time(self) -> datetime.timedelta: ...
    # Windows.Foundation.TimeSpan Windows.System.Diagnostics.ProcessCpuUsageReport::get_UserTime()
    @_property
    def user_time(self) -> datetime.timedelta: ...

@typing.final
class ProcessDiagnosticInfo_Static(winrt._winrt.IInspectable_Static):
    # Windows.System.Diagnostics.ProcessDiagnosticInfo Windows.System.Diagnostics.ProcessDiagnosticInfo::GetForCurrentProcess()
    def get_for_current_process(cls) -> ProcessDiagnosticInfo: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.System.Diagnostics.ProcessDiagnosticInfo> Windows.System.Diagnostics.ProcessDiagnosticInfo::GetForProcesses()
    def get_for_processes(cls) -> typing.Sequence[ProcessDiagnosticInfo]: ...
    # Windows.System.Diagnostics.ProcessDiagnosticInfo Windows.System.Diagnostics.ProcessDiagnosticInfo::TryGetForProcessId(System.UInt32)
    def try_get_for_process_id(cls, process_id: winrt.system.UInt32, /) -> typing.Optional[ProcessDiagnosticInfo]: ...

@typing.final
class ProcessDiagnosticInfo(winrt.system.Object, metaclass=ProcessDiagnosticInfo_Static):
    # Windows.Foundation.Collections.IVector`1<Windows.System.AppDiagnosticInfo> Windows.System.Diagnostics.ProcessDiagnosticInfo::GetAppDiagnosticInfos()
    def get_app_diagnostic_infos(self) -> typing.MutableSequence[windows_system.AppDiagnosticInfo]: ...
    # Windows.System.Diagnostics.ProcessCpuUsage Windows.System.Diagnostics.ProcessDiagnosticInfo::get_CpuUsage()
    @_property
    def cpu_usage(self) -> ProcessCpuUsage: ...
    # Windows.System.Diagnostics.ProcessDiskUsage Windows.System.Diagnostics.ProcessDiagnosticInfo::get_DiskUsage()
    @_property
    def disk_usage(self) -> ProcessDiskUsage: ...
    # System.String Windows.System.Diagnostics.ProcessDiagnosticInfo::get_ExecutableFileName()
    @_property
    def executable_file_name(self) -> str: ...
    # Windows.System.Diagnostics.ProcessMemoryUsage Windows.System.Diagnostics.ProcessDiagnosticInfo::get_MemoryUsage()
    @_property
    def memory_usage(self) -> ProcessMemoryUsage: ...
    # Windows.System.Diagnostics.ProcessDiagnosticInfo Windows.System.Diagnostics.ProcessDiagnosticInfo::get_Parent()
    @_property
    def parent(self) -> ProcessDiagnosticInfo: ...
    # System.UInt32 Windows.System.Diagnostics.ProcessDiagnosticInfo::get_ProcessId()
    @_property
    def process_id(self) -> winrt.system.UInt32: ...
    # Windows.Foundation.DateTime Windows.System.Diagnostics.ProcessDiagnosticInfo::get_ProcessStartTime()
    @_property
    def process_start_time(self) -> datetime.datetime: ...
    # System.Boolean Windows.System.Diagnostics.ProcessDiagnosticInfo::get_IsPackaged()
    @_property
    def is_packaged(self) -> bool: ...

@typing.final
class ProcessDiskUsage(winrt.system.Object):
    # Windows.System.Diagnostics.ProcessDiskUsageReport Windows.System.Diagnostics.ProcessDiskUsage::GetReport()
    def get_report(self) -> ProcessDiskUsageReport: ...

@typing.final
class ProcessDiskUsageReport(winrt.system.Object):
    # System.Int64 Windows.System.Diagnostics.ProcessDiskUsageReport::get_BytesReadCount()
    @_property
    def bytes_read_count(self) -> winrt.system.Int64: ...
    # System.Int64 Windows.System.Diagnostics.ProcessDiskUsageReport::get_BytesWrittenCount()
    @_property
    def bytes_written_count(self) -> winrt.system.Int64: ...
    # System.Int64 Windows.System.Diagnostics.ProcessDiskUsageReport::get_OtherBytesCount()
    @_property
    def other_bytes_count(self) -> winrt.system.Int64: ...
    # System.Int64 Windows.System.Diagnostics.ProcessDiskUsageReport::get_OtherOperationCount()
    @_property
    def other_operation_count(self) -> winrt.system.Int64: ...
    # System.Int64 Windows.System.Diagnostics.ProcessDiskUsageReport::get_ReadOperationCount()
    @_property
    def read_operation_count(self) -> winrt.system.Int64: ...
    # System.Int64 Windows.System.Diagnostics.ProcessDiskUsageReport::get_WriteOperationCount()
    @_property
    def write_operation_count(self) -> winrt.system.Int64: ...

@typing.final
class ProcessMemoryUsage(winrt.system.Object):
    # Windows.System.Diagnostics.ProcessMemoryUsageReport Windows.System.Diagnostics.ProcessMemoryUsage::GetReport()
    def get_report(self) -> ProcessMemoryUsageReport: ...

@typing.final
class ProcessMemoryUsageReport(winrt.system.Object):
    # System.UInt64 Windows.System.Diagnostics.ProcessMemoryUsageReport::get_NonPagedPoolSizeInBytes()
    @_property
    def non_paged_pool_size_in_bytes(self) -> winrt.system.UInt64: ...
    # System.UInt32 Windows.System.Diagnostics.ProcessMemoryUsageReport::get_PageFaultCount()
    @_property
    def page_fault_count(self) -> winrt.system.UInt32: ...
    # System.UInt64 Windows.System.Diagnostics.ProcessMemoryUsageReport::get_PageFileSizeInBytes()
    @_property
    def page_file_size_in_bytes(self) -> winrt.system.UInt64: ...
    # System.UInt64 Windows.System.Diagnostics.ProcessMemoryUsageReport::get_PagedPoolSizeInBytes()
    @_property
    def paged_pool_size_in_bytes(self) -> winrt.system.UInt64: ...
    # System.UInt64 Windows.System.Diagnostics.ProcessMemoryUsageReport::get_PeakNonPagedPoolSizeInBytes()
    @_property
    def peak_non_paged_pool_size_in_bytes(self) -> winrt.system.UInt64: ...
    # System.UInt64 Windows.System.Diagnostics.ProcessMemoryUsageReport::get_PeakPageFileSizeInBytes()
    @_property
    def peak_page_file_size_in_bytes(self) -> winrt.system.UInt64: ...
    # System.UInt64 Windows.System.Diagnostics.ProcessMemoryUsageReport::get_PeakPagedPoolSizeInBytes()
    @_property
    def peak_paged_pool_size_in_bytes(self) -> winrt.system.UInt64: ...
    # System.UInt64 Windows.System.Diagnostics.ProcessMemoryUsageReport::get_PeakVirtualMemorySizeInBytes()
    @_property
    def peak_virtual_memory_size_in_bytes(self) -> winrt.system.UInt64: ...
    # System.UInt64 Windows.System.Diagnostics.ProcessMemoryUsageReport::get_PeakWorkingSetSizeInBytes()
    @_property
    def peak_working_set_size_in_bytes(self) -> winrt.system.UInt64: ...
    # System.UInt64 Windows.System.Diagnostics.ProcessMemoryUsageReport::get_PrivatePageCount()
    @_property
    def private_page_count(self) -> winrt.system.UInt64: ...
    # System.UInt64 Windows.System.Diagnostics.ProcessMemoryUsageReport::get_VirtualMemorySizeInBytes()
    @_property
    def virtual_memory_size_in_bytes(self) -> winrt.system.UInt64: ...
    # System.UInt64 Windows.System.Diagnostics.ProcessMemoryUsageReport::get_WorkingSetSizeInBytes()
    @_property
    def working_set_size_in_bytes(self) -> winrt.system.UInt64: ...

@typing.final
class SystemCpuUsage(winrt.system.Object):
    # Windows.System.Diagnostics.SystemCpuUsageReport Windows.System.Diagnostics.SystemCpuUsage::GetReport()
    def get_report(self) -> SystemCpuUsageReport: ...

@typing.final
class SystemCpuUsageReport(winrt.system.Object):
    # Windows.Foundation.TimeSpan Windows.System.Diagnostics.SystemCpuUsageReport::get_IdleTime()
    @_property
    def idle_time(self) -> datetime.timedelta: ...
    # Windows.Foundation.TimeSpan Windows.System.Diagnostics.SystemCpuUsageReport::get_KernelTime()
    @_property
    def kernel_time(self) -> datetime.timedelta: ...
    # Windows.Foundation.TimeSpan Windows.System.Diagnostics.SystemCpuUsageReport::get_UserTime()
    @_property
    def user_time(self) -> datetime.timedelta: ...

@typing.final
class SystemDiagnosticInfo_Static(winrt._winrt.IInspectable_Static):
    # Windows.System.Diagnostics.SystemDiagnosticInfo Windows.System.Diagnostics.SystemDiagnosticInfo::GetForCurrentSystem()
    def get_for_current_system(cls) -> SystemDiagnosticInfo: ...
    # System.Boolean Windows.System.Diagnostics.SystemDiagnosticInfo::IsArchitectureSupported(Windows.System.ProcessorArchitecture)
    def is_architecture_supported(cls, type: windows_system.ProcessorArchitecture, /) -> bool: ...
    # Windows.System.ProcessorArchitecture Windows.System.Diagnostics.SystemDiagnosticInfo::get_PreferredArchitecture()
    @_property
    def preferred_architecture(cls) -> windows_system.ProcessorArchitecture: ...

@typing.final
class SystemDiagnosticInfo(winrt.system.Object, metaclass=SystemDiagnosticInfo_Static):
    # Windows.System.Diagnostics.SystemCpuUsage Windows.System.Diagnostics.SystemDiagnosticInfo::get_CpuUsage()
    @_property
    def cpu_usage(self) -> SystemCpuUsage: ...
    # Windows.System.Diagnostics.SystemMemoryUsage Windows.System.Diagnostics.SystemDiagnosticInfo::get_MemoryUsage()
    @_property
    def memory_usage(self) -> SystemMemoryUsage: ...

@typing.final
class SystemMemoryUsage(winrt.system.Object):
    # Windows.System.Diagnostics.SystemMemoryUsageReport Windows.System.Diagnostics.SystemMemoryUsage::GetReport()
    def get_report(self) -> SystemMemoryUsageReport: ...

@typing.final
class SystemMemoryUsageReport(winrt.system.Object):
    # System.UInt64 Windows.System.Diagnostics.SystemMemoryUsageReport::get_AvailableSizeInBytes()
    @_property
    def available_size_in_bytes(self) -> winrt.system.UInt64: ...
    # System.UInt64 Windows.System.Diagnostics.SystemMemoryUsageReport::get_CommittedSizeInBytes()
    @_property
    def committed_size_in_bytes(self) -> winrt.system.UInt64: ...
    # System.UInt64 Windows.System.Diagnostics.SystemMemoryUsageReport::get_TotalPhysicalSizeInBytes()
    @_property
    def total_physical_size_in_bytes(self) -> winrt.system.UInt64: ...

