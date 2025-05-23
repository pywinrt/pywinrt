# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.foundation as windows_foundation
import winrt.windows.foundation.collections as windows_foundation_collections

from winrt.windows.management.update import WindowsUpdateAdministratorOptions, WindowsUpdateAdministratorStatus, WindowsUpdateAttentionRequiredReason

Self = typing.TypeVar('Self')

@typing.final
class PreviewBuildsManager_Static(winrt._winrt.IInspectable_Static):
    # Windows.Management.Update.PreviewBuildsManager Windows.Management.Update.PreviewBuildsManager::GetDefault()
    def get_default(cls) -> PreviewBuildsManager: ...
    # System.Boolean Windows.Management.Update.PreviewBuildsManager::IsSupported()
    def is_supported(cls) -> bool: ...

@typing.final
class PreviewBuildsManager(winrt.system.Object, metaclass=PreviewBuildsManager_Static):
    # Windows.Management.Update.PreviewBuildsState Windows.Management.Update.PreviewBuildsManager::GetCurrentState()
    def get_current_state(self) -> PreviewBuildsState: ...
    # Windows.Foundation.IAsyncOperation`1<System.Boolean> Windows.Management.Update.PreviewBuildsManager::SyncAsync()
    def sync_async(self) -> windows_foundation.IAsyncOperation[bool]: ...
    # System.Boolean Windows.Management.Update.PreviewBuildsManager::get_ArePreviewBuildsAllowed()
    @_property
    def are_preview_builds_allowed(self) -> bool: ...
    # System.Void Windows.Management.Update.PreviewBuildsManager::put_ArePreviewBuildsAllowed(System.Boolean)
    @are_preview_builds_allowed.setter
    def are_preview_builds_allowed(self, value: bool) -> None: ...

@typing.final
class PreviewBuildsState(winrt.system.Object):
    # Windows.Foundation.Collections.ValueSet Windows.Management.Update.PreviewBuildsState::get_Properties()
    @_property
    def properties(self) -> windows_foundation_collections.ValueSet: ...

@typing.final
class WindowsUpdate(winrt.system.Object):
    # System.Void Windows.Management.Update.WindowsUpdate::AcceptEula()
    def accept_eula(self) -> None: ...
    # System.Object Windows.Management.Update.WindowsUpdate::GetPropertyValue(System.String)
    def get_property_value(self, property_name: str, /) -> winrt.system.Object: ...
    # Windows.Management.Update.WindowsUpdateActionProgress Windows.Management.Update.WindowsUpdate::get_ActionProgress()
    @_property
    def action_progress(self) -> WindowsUpdateActionProgress: ...
    # Windows.Management.Update.WindowsUpdateActionResult Windows.Management.Update.WindowsUpdate::get_ActionResult()
    @_property
    def action_result(self) -> WindowsUpdateActionResult: ...
    # Windows.Management.Update.WindowsUpdateAttentionRequiredInfo Windows.Management.Update.WindowsUpdate::get_AttentionRequiredInfo()
    @_property
    def attention_required_info(self) -> WindowsUpdateAttentionRequiredInfo: ...
    # System.String Windows.Management.Update.WindowsUpdate::get_CurrentAction()
    @_property
    def current_action(self) -> str: ...
    # Windows.Foundation.IReference`1<Windows.Foundation.DateTime> Windows.Management.Update.WindowsUpdate::get_Deadline()
    @_property
    def deadline(self) -> typing.Optional[datetime.datetime]: ...
    # System.String Windows.Management.Update.WindowsUpdate::get_Description()
    @_property
    def description(self) -> str: ...
    # System.String Windows.Management.Update.WindowsUpdate::get_EulaText()
    @_property
    def eula_text(self) -> str: ...
    # System.Boolean Windows.Management.Update.WindowsUpdate::get_IsCritical()
    @_property
    def is_critical(self) -> bool: ...
    # System.Boolean Windows.Management.Update.WindowsUpdate::get_IsDriver()
    @_property
    def is_driver(self) -> bool: ...
    # System.Boolean Windows.Management.Update.WindowsUpdate::get_IsEulaAccepted()
    @_property
    def is_eula_accepted(self) -> bool: ...
    # System.Boolean Windows.Management.Update.WindowsUpdate::get_IsFeatureUpdate()
    @_property
    def is_feature_update(self) -> bool: ...
    # System.Boolean Windows.Management.Update.WindowsUpdate::get_IsForOS()
    @_property
    def is_for_os(self) -> bool: ...
    # System.Boolean Windows.Management.Update.WindowsUpdate::get_IsMandatory()
    @_property
    def is_mandatory(self) -> bool: ...
    # System.Boolean Windows.Management.Update.WindowsUpdate::get_IsMinorImpact()
    @_property
    def is_minor_impact(self) -> bool: ...
    # System.Boolean Windows.Management.Update.WindowsUpdate::get_IsSecurity()
    @_property
    def is_security(self) -> bool: ...
    # System.Boolean Windows.Management.Update.WindowsUpdate::get_IsSeeker()
    @_property
    def is_seeker(self) -> bool: ...
    # System.Boolean Windows.Management.Update.WindowsUpdate::get_IsUrgent()
    @_property
    def is_urgent(self) -> bool: ...
    # Windows.Foundation.Uri Windows.Management.Update.WindowsUpdate::get_MoreInfoUrl()
    @_property
    def more_info_url(self) -> windows_foundation.Uri: ...
    # System.String Windows.Management.Update.WindowsUpdate::get_ProviderId()
    @_property
    def provider_id(self) -> str: ...
    # Windows.Foundation.Uri Windows.Management.Update.WindowsUpdate::get_SupportUrl()
    @_property
    def support_url(self) -> windows_foundation.Uri: ...
    # System.String Windows.Management.Update.WindowsUpdate::get_Title()
    @_property
    def title(self) -> str: ...
    # System.String Windows.Management.Update.WindowsUpdate::get_UpdateId()
    @_property
    def update_id(self) -> str: ...

@typing.final
class WindowsUpdateActionCompletedEventArgs(winrt.system.Object):
    # System.String Windows.Management.Update.WindowsUpdateActionCompletedEventArgs::get_Action()
    @_property
    def action(self) -> str: ...
    # Windows.Foundation.HResult Windows.Management.Update.WindowsUpdateActionCompletedEventArgs::get_ExtendedError()
    @_property
    def extended_error(self) -> windows_foundation.HResult: ...
    # System.Boolean Windows.Management.Update.WindowsUpdateActionCompletedEventArgs::get_Succeeded()
    @_property
    def succeeded(self) -> bool: ...
    # Windows.Management.Update.WindowsUpdate Windows.Management.Update.WindowsUpdateActionCompletedEventArgs::get_Update()
    @_property
    def update(self) -> WindowsUpdate: ...

@typing.final
class WindowsUpdateActionProgress(winrt.system.Object):
    # System.String Windows.Management.Update.WindowsUpdateActionProgress::get_Action()
    @_property
    def action(self) -> str: ...
    # System.Double Windows.Management.Update.WindowsUpdateActionProgress::get_Progress()
    @_property
    def progress(self) -> winrt.system.Double: ...

@typing.final
class WindowsUpdateActionResult(winrt.system.Object):
    # System.String Windows.Management.Update.WindowsUpdateActionResult::get_Action()
    @_property
    def action(self) -> str: ...
    # Windows.Foundation.HResult Windows.Management.Update.WindowsUpdateActionResult::get_ExtendedError()
    @_property
    def extended_error(self) -> windows_foundation.HResult: ...
    # System.Boolean Windows.Management.Update.WindowsUpdateActionResult::get_Succeeded()
    @_property
    def succeeded(self) -> bool: ...
    # Windows.Foundation.DateTime Windows.Management.Update.WindowsUpdateActionResult::get_Timestamp()
    @_property
    def timestamp(self) -> datetime.datetime: ...

@typing.final
class WindowsUpdateAdministrator_Static(winrt._winrt.IInspectable_Static):
    # System.Void Windows.Management.Update.WindowsUpdateAdministrator::CancelRestartRequest(System.String)
    def cancel_restart_request(cls, request_restart_token: str, /) -> None: ...
    # Windows.Management.Update.WindowsUpdateGetAdministratorResult Windows.Management.Update.WindowsUpdateAdministrator::GetRegisteredAdministrator(System.String)
    def get_registered_administrator(cls, organization_name: str, /) -> WindowsUpdateGetAdministratorResult: ...
    # System.String Windows.Management.Update.WindowsUpdateAdministrator::GetRegisteredAdministratorName()
    def get_registered_administrator_name(cls) -> str: ...
    # Windows.Management.Update.WindowsUpdateAdministratorStatus Windows.Management.Update.WindowsUpdateAdministrator::RegisterForAdministration(System.String,Windows.Management.Update.WindowsUpdateAdministratorOptions)
    def register_for_administration(cls, organization_name: str, options: WindowsUpdateAdministratorOptions, /) -> WindowsUpdateAdministratorStatus: ...
    # System.String Windows.Management.Update.WindowsUpdateAdministrator::RequestRestart(Windows.Management.Update.WindowsUpdateRestartRequestOptions)
    def request_restart(cls, restart_options: WindowsUpdateRestartRequestOptions, /) -> str: ...
    # Windows.Management.Update.WindowsUpdateAdministratorStatus Windows.Management.Update.WindowsUpdateAdministrator::UnregisterForAdministration(System.String)
    def unregister_for_administration(cls, organization_name: str, /) -> WindowsUpdateAdministratorStatus: ...

@typing.final
class WindowsUpdateAdministrator(winrt.system.Object, metaclass=WindowsUpdateAdministrator_Static):
    # System.Void Windows.Management.Update.WindowsUpdateAdministrator::ApproveWindowsUpdate(System.String,Windows.Management.Update.WindowsUpdateApprovalData)
    def approve_windows_update(self, update_id: str, approval_data: WindowsUpdateApprovalData, /) -> None: ...
    # System.Void Windows.Management.Update.WindowsUpdateAdministrator::ApproveWindowsUpdateAction(System.String,System.String)
    def approve_windows_update_action(self, update_id: str, action: str, /) -> None: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Management.Update.WindowsUpdate> Windows.Management.Update.WindowsUpdateAdministrator::GetUpdates()
    def get_updates(self) -> typing.Sequence[WindowsUpdate]: ...
    # System.Void Windows.Management.Update.WindowsUpdateAdministrator::RevokeWindowsUpdateActionApproval(System.String,System.String)
    def revoke_windows_update_action_approval(self, update_id: str, action: str, /) -> None: ...
    # System.Void Windows.Management.Update.WindowsUpdateAdministrator::RevokeWindowsUpdateApproval(System.String)
    def revoke_windows_update_approval(self, update_id: str, /) -> None: ...
    # System.Void Windows.Management.Update.WindowsUpdateAdministrator::StartAdministratorScan()
    def start_administrator_scan(self) -> None: ...

@typing.final
class WindowsUpdateApprovalData(winrt.system.Object):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # Windows.Foundation.IReference`1<System.Boolean> Windows.Management.Update.WindowsUpdateApprovalData::get_Seeker()
    @_property
    def seeker(self) -> typing.Optional[bool]: ...
    # System.Void Windows.Management.Update.WindowsUpdateApprovalData::put_Seeker(Windows.Foundation.IReference`1<System.Boolean>)
    @seeker.setter
    def seeker(self, value: typing.Optional[bool]) -> None: ...
    # Windows.Foundation.IReference`1<System.Boolean> Windows.Management.Update.WindowsUpdateApprovalData::get_OptOutOfAutoReboot()
    @_property
    def opt_out_of_auto_reboot(self) -> typing.Optional[bool]: ...
    # System.Void Windows.Management.Update.WindowsUpdateApprovalData::put_OptOutOfAutoReboot(Windows.Foundation.IReference`1<System.Boolean>)
    @opt_out_of_auto_reboot.setter
    def opt_out_of_auto_reboot(self, value: typing.Optional[bool]) -> None: ...
    # Windows.Foundation.IReference`1<System.Int32> Windows.Management.Update.WindowsUpdateApprovalData::get_ComplianceGracePeriodInDays()
    @_property
    def compliance_grace_period_in_days(self) -> typing.Optional[winrt.system.Int32]: ...
    # System.Void Windows.Management.Update.WindowsUpdateApprovalData::put_ComplianceGracePeriodInDays(Windows.Foundation.IReference`1<System.Int32>)
    @compliance_grace_period_in_days.setter
    def compliance_grace_period_in_days(self, value: typing.Optional[winrt.system.Int32]) -> None: ...
    # Windows.Foundation.IReference`1<System.Int32> Windows.Management.Update.WindowsUpdateApprovalData::get_ComplianceDeadlineInDays()
    @_property
    def compliance_deadline_in_days(self) -> typing.Optional[winrt.system.Int32]: ...
    # System.Void Windows.Management.Update.WindowsUpdateApprovalData::put_ComplianceDeadlineInDays(Windows.Foundation.IReference`1<System.Int32>)
    @compliance_deadline_in_days.setter
    def compliance_deadline_in_days(self, value: typing.Optional[winrt.system.Int32]) -> None: ...
    # Windows.Foundation.IReference`1<System.Boolean> Windows.Management.Update.WindowsUpdateApprovalData::get_AllowDownloadOnMetered()
    @_property
    def allow_download_on_metered(self) -> typing.Optional[bool]: ...
    # System.Void Windows.Management.Update.WindowsUpdateApprovalData::put_AllowDownloadOnMetered(Windows.Foundation.IReference`1<System.Boolean>)
    @allow_download_on_metered.setter
    def allow_download_on_metered(self, value: typing.Optional[bool]) -> None: ...

@typing.final
class WindowsUpdateAttentionRequiredInfo(winrt.system.Object):
    # Windows.Management.Update.WindowsUpdateAttentionRequiredReason Windows.Management.Update.WindowsUpdateAttentionRequiredInfo::get_Reason()
    @_property
    def reason(self) -> WindowsUpdateAttentionRequiredReason: ...
    # Windows.Foundation.IReference`1<Windows.Foundation.DateTime> Windows.Management.Update.WindowsUpdateAttentionRequiredInfo::get_Timestamp()
    @_property
    def timestamp(self) -> typing.Optional[datetime.datetime]: ...

@typing.final
class WindowsUpdateAttentionRequiredReasonChangedEventArgs(winrt.system.Object):
    # Windows.Management.Update.WindowsUpdateAttentionRequiredReason Windows.Management.Update.WindowsUpdateAttentionRequiredReasonChangedEventArgs::get_Reason()
    @_property
    def reason(self) -> WindowsUpdateAttentionRequiredReason: ...
    # Windows.Management.Update.WindowsUpdate Windows.Management.Update.WindowsUpdateAttentionRequiredReasonChangedEventArgs::get_Update()
    @_property
    def update(self) -> WindowsUpdate: ...

@typing.final
class WindowsUpdateGetAdministratorResult(winrt.system.Object):
    # Windows.Management.Update.WindowsUpdateAdministrator Windows.Management.Update.WindowsUpdateGetAdministratorResult::get_Administrator()
    @_property
    def administrator(self) -> WindowsUpdateAdministrator: ...
    # Windows.Management.Update.WindowsUpdateAdministratorStatus Windows.Management.Update.WindowsUpdateGetAdministratorResult::get_Status()
    @_property
    def status(self) -> WindowsUpdateAdministratorStatus: ...

@typing.final
class WindowsUpdateItem(winrt.system.Object):
    # System.String Windows.Management.Update.WindowsUpdateItem::get_Category()
    @_property
    def category(self) -> str: ...
    # System.String Windows.Management.Update.WindowsUpdateItem::get_Description()
    @_property
    def description(self) -> str: ...
    # Windows.Foundation.Uri Windows.Management.Update.WindowsUpdateItem::get_MoreInfoUrl()
    @_property
    def more_info_url(self) -> windows_foundation.Uri: ...
    # System.String Windows.Management.Update.WindowsUpdateItem::get_Operation()
    @_property
    def operation(self) -> str: ...
    # System.String Windows.Management.Update.WindowsUpdateItem::get_ProviderId()
    @_property
    def provider_id(self) -> str: ...
    # Windows.Foundation.DateTime Windows.Management.Update.WindowsUpdateItem::get_Timestamp()
    @_property
    def timestamp(self) -> datetime.datetime: ...
    # System.String Windows.Management.Update.WindowsUpdateItem::get_Title()
    @_property
    def title(self) -> str: ...
    # System.String Windows.Management.Update.WindowsUpdateItem::get_UpdateId()
    @_property
    def update_id(self) -> str: ...

@typing.final
class WindowsUpdateManager(winrt.system.Object):
    def __new__(cls: typing.Type[Self], client_id: str) -> Self: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Management.Update.WindowsUpdate> Windows.Management.Update.WindowsUpdateManager::GetApplicableUpdates()
    def get_applicable_updates(self) -> typing.Sequence[WindowsUpdate]: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Management.Update.WindowsUpdateItem> Windows.Management.Update.WindowsUpdateManager::GetMostRecentCompletedUpdates(System.Int32)
    def get_most_recent_completed_updates(self, count: winrt.system.Int32, /) -> typing.Sequence[WindowsUpdateItem]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Foundation.Collections.IVectorView`1<Windows.Management.Update.WindowsUpdateItem>> Windows.Management.Update.WindowsUpdateManager::GetMostRecentCompletedUpdatesAsync(System.Int32)
    def get_most_recent_completed_updates_async(self, count: winrt.system.Int32, /) -> windows_foundation.IAsyncOperation[typing.Sequence[WindowsUpdateItem]]: ...
    # System.Void Windows.Management.Update.WindowsUpdateManager::StartScan(System.Boolean)
    def start_scan(self, user_initiated: bool, /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.Management.Update.WindowsUpdateManager::add_ActionCompleted(Windows.Foundation.TypedEventHandler`2<Windows.Management.Update.WindowsUpdateManager,Windows.Management.Update.WindowsUpdateActionCompletedEventArgs>)
    def add_action_completed(self, handler: windows_foundation.TypedEventHandler[WindowsUpdateManager, WindowsUpdateActionCompletedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.Management.Update.WindowsUpdateManager::remove_ActionCompleted(Windows.Foundation.EventRegistrationToken)
    def remove_action_completed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.Management.Update.WindowsUpdateManager::add_AttentionRequiredReasonChanged(Windows.Foundation.TypedEventHandler`2<Windows.Management.Update.WindowsUpdateManager,Windows.Management.Update.WindowsUpdateAttentionRequiredReasonChangedEventArgs>)
    def add_attention_required_reason_changed(self, handler: windows_foundation.TypedEventHandler[WindowsUpdateManager, WindowsUpdateAttentionRequiredReasonChangedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.Management.Update.WindowsUpdateManager::remove_AttentionRequiredReasonChanged(Windows.Foundation.EventRegistrationToken)
    def remove_attention_required_reason_changed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.Management.Update.WindowsUpdateManager::add_ProgressChanged(Windows.Foundation.TypedEventHandler`2<Windows.Management.Update.WindowsUpdateManager,Windows.Management.Update.WindowsUpdateProgressChangedEventArgs>)
    def add_progress_changed(self, handler: windows_foundation.TypedEventHandler[WindowsUpdateManager, WindowsUpdateProgressChangedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.Management.Update.WindowsUpdateManager::remove_ProgressChanged(Windows.Foundation.EventRegistrationToken)
    def remove_progress_changed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.Management.Update.WindowsUpdateManager::add_ScanCompleted(Windows.Foundation.TypedEventHandler`2<Windows.Management.Update.WindowsUpdateManager,Windows.Management.Update.WindowsUpdateScanCompletedEventArgs>)
    def add_scan_completed(self, handler: windows_foundation.TypedEventHandler[WindowsUpdateManager, WindowsUpdateScanCompletedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.Management.Update.WindowsUpdateManager::remove_ScanCompleted(Windows.Foundation.EventRegistrationToken)
    def remove_scan_completed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.Management.Update.WindowsUpdateManager::add_ScanningStateChanged(Windows.Foundation.TypedEventHandler`2<Windows.Management.Update.WindowsUpdateManager,System.Object>)
    def add_scanning_state_changed(self, handler: windows_foundation.TypedEventHandler[WindowsUpdateManager, winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.Management.Update.WindowsUpdateManager::remove_ScanningStateChanged(Windows.Foundation.EventRegistrationToken)
    def remove_scanning_state_changed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.Management.Update.WindowsUpdateManager::add_WorkingStateChanged(Windows.Foundation.TypedEventHandler`2<Windows.Management.Update.WindowsUpdateManager,System.Object>)
    def add_working_state_changed(self, handler: windows_foundation.TypedEventHandler[WindowsUpdateManager, winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.Management.Update.WindowsUpdateManager::remove_WorkingStateChanged(Windows.Foundation.EventRegistrationToken)
    def remove_working_state_changed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # System.Boolean Windows.Management.Update.WindowsUpdateManager::get_IsScanning()
    @_property
    def is_scanning(self) -> bool: ...
    # System.Boolean Windows.Management.Update.WindowsUpdateManager::get_IsWorking()
    @_property
    def is_working(self) -> bool: ...
    # Windows.Foundation.IReference`1<Windows.Foundation.DateTime> Windows.Management.Update.WindowsUpdateManager::get_LastSuccessfulScanTimestamp()
    @_property
    def last_successful_scan_timestamp(self) -> typing.Optional[datetime.datetime]: ...

@typing.final
class WindowsUpdateProgressChangedEventArgs(winrt.system.Object):
    # Windows.Management.Update.WindowsUpdateActionProgress Windows.Management.Update.WindowsUpdateProgressChangedEventArgs::get_ActionProgress()
    @_property
    def action_progress(self) -> WindowsUpdateActionProgress: ...
    # Windows.Management.Update.WindowsUpdate Windows.Management.Update.WindowsUpdateProgressChangedEventArgs::get_Update()
    @_property
    def update(self) -> WindowsUpdate: ...

@typing.final
class WindowsUpdateRestartRequestOptions(winrt.system.Object):
    @typing.overload
    def __new__(cls: typing.Type[Self], title: str, description: str, more_info_url: windows_foundation.Uri, compliance_deadline_in_days: winrt.system.Int32, compliance_grace_period_in_days: winrt.system.Int32) -> Self: ...
    @typing.overload
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # System.String Windows.Management.Update.WindowsUpdateRestartRequestOptions::get_Title()
    @_property
    def title(self) -> str: ...
    # System.Void Windows.Management.Update.WindowsUpdateRestartRequestOptions::put_Title(System.String)
    @title.setter
    def title(self, value: str) -> None: ...
    # System.String Windows.Management.Update.WindowsUpdateRestartRequestOptions::get_OrganizationName()
    @_property
    def organization_name(self) -> str: ...
    # System.Void Windows.Management.Update.WindowsUpdateRestartRequestOptions::put_OrganizationName(System.String)
    @organization_name.setter
    def organization_name(self, value: str) -> None: ...
    # System.Boolean Windows.Management.Update.WindowsUpdateRestartRequestOptions::get_OptOutOfAutoReboot()
    @_property
    def opt_out_of_auto_reboot(self) -> bool: ...
    # System.Void Windows.Management.Update.WindowsUpdateRestartRequestOptions::put_OptOutOfAutoReboot(System.Boolean)
    @opt_out_of_auto_reboot.setter
    def opt_out_of_auto_reboot(self, value: bool) -> None: ...
    # Windows.Foundation.Uri Windows.Management.Update.WindowsUpdateRestartRequestOptions::get_MoreInfoUrl()
    @_property
    def more_info_url(self) -> windows_foundation.Uri: ...
    # System.Void Windows.Management.Update.WindowsUpdateRestartRequestOptions::put_MoreInfoUrl(Windows.Foundation.Uri)
    @more_info_url.setter
    def more_info_url(self, value: windows_foundation.Uri) -> None: ...
    # System.String Windows.Management.Update.WindowsUpdateRestartRequestOptions::get_Description()
    @_property
    def description(self) -> str: ...
    # System.Void Windows.Management.Update.WindowsUpdateRestartRequestOptions::put_Description(System.String)
    @description.setter
    def description(self, value: str) -> None: ...
    # System.Int32 Windows.Management.Update.WindowsUpdateRestartRequestOptions::get_ComplianceGracePeriodInDays()
    @_property
    def compliance_grace_period_in_days(self) -> winrt.system.Int32: ...
    # System.Void Windows.Management.Update.WindowsUpdateRestartRequestOptions::put_ComplianceGracePeriodInDays(System.Int32)
    @compliance_grace_period_in_days.setter
    def compliance_grace_period_in_days(self, value: winrt.system.Int32) -> None: ...
    # System.Int32 Windows.Management.Update.WindowsUpdateRestartRequestOptions::get_ComplianceDeadlineInDays()
    @_property
    def compliance_deadline_in_days(self) -> winrt.system.Int32: ...
    # System.Void Windows.Management.Update.WindowsUpdateRestartRequestOptions::put_ComplianceDeadlineInDays(System.Int32)
    @compliance_deadline_in_days.setter
    def compliance_deadline_in_days(self, value: winrt.system.Int32) -> None: ...

@typing.final
class WindowsUpdateScanCompletedEventArgs(winrt.system.Object):
    # Windows.Foundation.HResult Windows.Management.Update.WindowsUpdateScanCompletedEventArgs::get_ExtendedError()
    @_property
    def extended_error(self) -> windows_foundation.HResult: ...
    # System.String Windows.Management.Update.WindowsUpdateScanCompletedEventArgs::get_ProviderId()
    @_property
    def provider_id(self) -> str: ...
    # System.Boolean Windows.Management.Update.WindowsUpdateScanCompletedEventArgs::get_Succeeded()
    @_property
    def succeeded(self) -> bool: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Management.Update.WindowsUpdate> Windows.Management.Update.WindowsUpdateScanCompletedEventArgs::get_Updates()
    @_property
    def updates(self) -> typing.Sequence[WindowsUpdate]: ...

