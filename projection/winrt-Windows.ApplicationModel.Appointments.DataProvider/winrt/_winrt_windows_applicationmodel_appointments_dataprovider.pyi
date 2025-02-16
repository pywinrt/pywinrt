# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.applicationmodel.appointments as windows_applicationmodel_appointments
import winrt.windows.foundation as windows_foundation
import winrt.windows.foundation.collections as windows_foundation_collections

Self = typing.TypeVar('Self')

@typing.final
class AppointmentCalendarCancelMeetingRequest(winrt.system.Object):
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCancelMeetingRequest::ReportCompletedAsync()
    def report_completed_async(self) -> windows_foundation.IAsyncAction: ...
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCancelMeetingRequest::ReportFailedAsync()
    def report_failed_async(self) -> windows_foundation.IAsyncAction: ...
    # System.String Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCancelMeetingRequest::get_AppointmentCalendarLocalId()
    @_property
    def appointment_calendar_local_id(self) -> str: ...
    # System.String Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCancelMeetingRequest::get_AppointmentLocalId()
    @_property
    def appointment_local_id(self) -> str: ...
    # Windows.Foundation.IReference`1<Windows.Foundation.DateTime> Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCancelMeetingRequest::get_AppointmentOriginalStartTime()
    @_property
    def appointment_original_start_time(self) -> typing.Optional[datetime.datetime]: ...
    # System.String Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCancelMeetingRequest::get_Comment()
    @_property
    def comment(self) -> str: ...
    # System.Boolean Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCancelMeetingRequest::get_NotifyInvitees()
    @_property
    def notify_invitees(self) -> bool: ...
    # System.String Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCancelMeetingRequest::get_Subject()
    @_property
    def subject(self) -> str: ...

@typing.final
class AppointmentCalendarCancelMeetingRequestEventArgs(winrt.system.Object):
    # Windows.Foundation.Deferral Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCancelMeetingRequestEventArgs::GetDeferral()
    def get_deferral(self) -> windows_foundation.Deferral: ...
    # Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCancelMeetingRequest Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCancelMeetingRequestEventArgs::get_Request()
    @_property
    def request(self) -> AppointmentCalendarCancelMeetingRequest: ...

@typing.final
class AppointmentCalendarCreateOrUpdateAppointmentRequest(winrt.system.Object):
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCreateOrUpdateAppointmentRequest::ReportCompletedAsync(Windows.ApplicationModel.Appointments.Appointment)
    def report_completed_async(self, created_or_updated_appointment: windows_applicationmodel_appointments.Appointment, /) -> windows_foundation.IAsyncAction: ...
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCreateOrUpdateAppointmentRequest::ReportFailedAsync()
    def report_failed_async(self) -> windows_foundation.IAsyncAction: ...
    # Windows.ApplicationModel.Appointments.Appointment Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCreateOrUpdateAppointmentRequest::get_Appointment()
    @_property
    def appointment(self) -> windows_applicationmodel_appointments.Appointment: ...
    # System.String Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCreateOrUpdateAppointmentRequest::get_AppointmentCalendarLocalId()
    @_property
    def appointment_calendar_local_id(self) -> str: ...
    # Windows.Foundation.Collections.IVectorView`1<System.String> Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCreateOrUpdateAppointmentRequest::get_ChangedProperties()
    @_property
    def changed_properties(self) -> typing.Sequence[str]: ...
    # System.Boolean Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCreateOrUpdateAppointmentRequest::get_NotifyInvitees()
    @_property
    def notify_invitees(self) -> bool: ...

@typing.final
class AppointmentCalendarCreateOrUpdateAppointmentRequestEventArgs(winrt.system.Object):
    # Windows.Foundation.Deferral Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCreateOrUpdateAppointmentRequestEventArgs::GetDeferral()
    def get_deferral(self) -> windows_foundation.Deferral: ...
    # Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCreateOrUpdateAppointmentRequest Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCreateOrUpdateAppointmentRequestEventArgs::get_Request()
    @_property
    def request(self) -> AppointmentCalendarCreateOrUpdateAppointmentRequest: ...

@typing.final
class AppointmentCalendarForwardMeetingRequest(winrt.system.Object):
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarForwardMeetingRequest::ReportCompletedAsync()
    def report_completed_async(self) -> windows_foundation.IAsyncAction: ...
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarForwardMeetingRequest::ReportFailedAsync()
    def report_failed_async(self) -> windows_foundation.IAsyncAction: ...
    # System.String Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarForwardMeetingRequest::get_AppointmentCalendarLocalId()
    @_property
    def appointment_calendar_local_id(self) -> str: ...
    # System.String Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarForwardMeetingRequest::get_AppointmentLocalId()
    @_property
    def appointment_local_id(self) -> str: ...
    # Windows.Foundation.IReference`1<Windows.Foundation.DateTime> Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarForwardMeetingRequest::get_AppointmentOriginalStartTime()
    @_property
    def appointment_original_start_time(self) -> typing.Optional[datetime.datetime]: ...
    # System.String Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarForwardMeetingRequest::get_Comment()
    @_property
    def comment(self) -> str: ...
    # System.String Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarForwardMeetingRequest::get_ForwardHeader()
    @_property
    def forward_header(self) -> str: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.Appointments.AppointmentInvitee> Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarForwardMeetingRequest::get_Invitees()
    @_property
    def invitees(self) -> typing.Sequence[windows_applicationmodel_appointments.AppointmentInvitee]: ...
    # System.String Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarForwardMeetingRequest::get_Subject()
    @_property
    def subject(self) -> str: ...

@typing.final
class AppointmentCalendarForwardMeetingRequestEventArgs(winrt.system.Object):
    # Windows.Foundation.Deferral Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarForwardMeetingRequestEventArgs::GetDeferral()
    def get_deferral(self) -> windows_foundation.Deferral: ...
    # Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarForwardMeetingRequest Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarForwardMeetingRequestEventArgs::get_Request()
    @_property
    def request(self) -> AppointmentCalendarForwardMeetingRequest: ...

@typing.final
class AppointmentCalendarProposeNewTimeForMeetingRequest(winrt.system.Object):
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarProposeNewTimeForMeetingRequest::ReportCompletedAsync()
    def report_completed_async(self) -> windows_foundation.IAsyncAction: ...
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarProposeNewTimeForMeetingRequest::ReportFailedAsync()
    def report_failed_async(self) -> windows_foundation.IAsyncAction: ...
    # System.String Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarProposeNewTimeForMeetingRequest::get_AppointmentCalendarLocalId()
    @_property
    def appointment_calendar_local_id(self) -> str: ...
    # System.String Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarProposeNewTimeForMeetingRequest::get_AppointmentLocalId()
    @_property
    def appointment_local_id(self) -> str: ...
    # Windows.Foundation.IReference`1<Windows.Foundation.DateTime> Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarProposeNewTimeForMeetingRequest::get_AppointmentOriginalStartTime()
    @_property
    def appointment_original_start_time(self) -> typing.Optional[datetime.datetime]: ...
    # System.String Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarProposeNewTimeForMeetingRequest::get_Comment()
    @_property
    def comment(self) -> str: ...
    # Windows.Foundation.TimeSpan Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarProposeNewTimeForMeetingRequest::get_NewDuration()
    @_property
    def new_duration(self) -> datetime.timedelta: ...
    # Windows.Foundation.DateTime Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarProposeNewTimeForMeetingRequest::get_NewStartTime()
    @_property
    def new_start_time(self) -> datetime.datetime: ...
    # System.String Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarProposeNewTimeForMeetingRequest::get_Subject()
    @_property
    def subject(self) -> str: ...

@typing.final
class AppointmentCalendarProposeNewTimeForMeetingRequestEventArgs(winrt.system.Object):
    # Windows.Foundation.Deferral Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarProposeNewTimeForMeetingRequestEventArgs::GetDeferral()
    def get_deferral(self) -> windows_foundation.Deferral: ...
    # Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarProposeNewTimeForMeetingRequest Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarProposeNewTimeForMeetingRequestEventArgs::get_Request()
    @_property
    def request(self) -> AppointmentCalendarProposeNewTimeForMeetingRequest: ...

@typing.final
class AppointmentCalendarSyncManagerSyncRequest(winrt.system.Object):
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarSyncManagerSyncRequest::ReportCompletedAsync()
    def report_completed_async(self) -> windows_foundation.IAsyncAction: ...
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarSyncManagerSyncRequest::ReportFailedAsync()
    def report_failed_async(self) -> windows_foundation.IAsyncAction: ...
    # System.String Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarSyncManagerSyncRequest::get_AppointmentCalendarLocalId()
    @_property
    def appointment_calendar_local_id(self) -> str: ...

@typing.final
class AppointmentCalendarSyncManagerSyncRequestEventArgs(winrt.system.Object):
    # Windows.Foundation.Deferral Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarSyncManagerSyncRequestEventArgs::GetDeferral()
    def get_deferral(self) -> windows_foundation.Deferral: ...
    # Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarSyncManagerSyncRequest Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarSyncManagerSyncRequestEventArgs::get_Request()
    @_property
    def request(self) -> AppointmentCalendarSyncManagerSyncRequest: ...

@typing.final
class AppointmentCalendarUpdateMeetingResponseRequest(winrt.system.Object):
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarUpdateMeetingResponseRequest::ReportCompletedAsync()
    def report_completed_async(self) -> windows_foundation.IAsyncAction: ...
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarUpdateMeetingResponseRequest::ReportFailedAsync()
    def report_failed_async(self) -> windows_foundation.IAsyncAction: ...
    # System.String Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarUpdateMeetingResponseRequest::get_AppointmentCalendarLocalId()
    @_property
    def appointment_calendar_local_id(self) -> str: ...
    # System.String Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarUpdateMeetingResponseRequest::get_AppointmentLocalId()
    @_property
    def appointment_local_id(self) -> str: ...
    # Windows.Foundation.IReference`1<Windows.Foundation.DateTime> Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarUpdateMeetingResponseRequest::get_AppointmentOriginalStartTime()
    @_property
    def appointment_original_start_time(self) -> typing.Optional[datetime.datetime]: ...
    # System.String Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarUpdateMeetingResponseRequest::get_Comment()
    @_property
    def comment(self) -> str: ...
    # Windows.ApplicationModel.Appointments.AppointmentParticipantResponse Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarUpdateMeetingResponseRequest::get_Response()
    @_property
    def response(self) -> windows_applicationmodel_appointments.AppointmentParticipantResponse: ...
    # System.Boolean Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarUpdateMeetingResponseRequest::get_SendUpdate()
    @_property
    def send_update(self) -> bool: ...
    # System.String Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarUpdateMeetingResponseRequest::get_Subject()
    @_property
    def subject(self) -> str: ...

@typing.final
class AppointmentCalendarUpdateMeetingResponseRequestEventArgs(winrt.system.Object):
    # Windows.Foundation.Deferral Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarUpdateMeetingResponseRequestEventArgs::GetDeferral()
    def get_deferral(self) -> windows_foundation.Deferral: ...
    # Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarUpdateMeetingResponseRequest Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarUpdateMeetingResponseRequestEventArgs::get_Request()
    @_property
    def request(self) -> AppointmentCalendarUpdateMeetingResponseRequest: ...

@typing.final
class AppointmentDataProviderConnection(winrt.system.Object):
    # System.Void Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderConnection::Start()
    def start(self) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderConnection::add_CancelMeetingRequested(Windows.Foundation.TypedEventHandler`2<Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderConnection,Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCancelMeetingRequestEventArgs>)
    def add_cancel_meeting_requested(self, handler: windows_foundation.TypedEventHandler[AppointmentDataProviderConnection, AppointmentCalendarCancelMeetingRequestEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderConnection::remove_CancelMeetingRequested(Windows.Foundation.EventRegistrationToken)
    def remove_cancel_meeting_requested(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderConnection::add_CreateOrUpdateAppointmentRequested(Windows.Foundation.TypedEventHandler`2<Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderConnection,Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarCreateOrUpdateAppointmentRequestEventArgs>)
    def add_create_or_update_appointment_requested(self, handler: windows_foundation.TypedEventHandler[AppointmentDataProviderConnection, AppointmentCalendarCreateOrUpdateAppointmentRequestEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderConnection::remove_CreateOrUpdateAppointmentRequested(Windows.Foundation.EventRegistrationToken)
    def remove_create_or_update_appointment_requested(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderConnection::add_ForwardMeetingRequested(Windows.Foundation.TypedEventHandler`2<Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderConnection,Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarForwardMeetingRequestEventArgs>)
    def add_forward_meeting_requested(self, handler: windows_foundation.TypedEventHandler[AppointmentDataProviderConnection, AppointmentCalendarForwardMeetingRequestEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderConnection::remove_ForwardMeetingRequested(Windows.Foundation.EventRegistrationToken)
    def remove_forward_meeting_requested(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderConnection::add_ProposeNewTimeForMeetingRequested(Windows.Foundation.TypedEventHandler`2<Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderConnection,Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarProposeNewTimeForMeetingRequestEventArgs>)
    def add_propose_new_time_for_meeting_requested(self, handler: windows_foundation.TypedEventHandler[AppointmentDataProviderConnection, AppointmentCalendarProposeNewTimeForMeetingRequestEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderConnection::remove_ProposeNewTimeForMeetingRequested(Windows.Foundation.EventRegistrationToken)
    def remove_propose_new_time_for_meeting_requested(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderConnection::add_SyncRequested(Windows.Foundation.TypedEventHandler`2<Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderConnection,Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarSyncManagerSyncRequestEventArgs>)
    def add_sync_requested(self, handler: windows_foundation.TypedEventHandler[AppointmentDataProviderConnection, AppointmentCalendarSyncManagerSyncRequestEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderConnection::remove_SyncRequested(Windows.Foundation.EventRegistrationToken)
    def remove_sync_requested(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderConnection::add_UpdateMeetingResponseRequested(Windows.Foundation.TypedEventHandler`2<Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderConnection,Windows.ApplicationModel.Appointments.DataProvider.AppointmentCalendarUpdateMeetingResponseRequestEventArgs>)
    def add_update_meeting_response_requested(self, handler: windows_foundation.TypedEventHandler[AppointmentDataProviderConnection, AppointmentCalendarUpdateMeetingResponseRequestEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderConnection::remove_UpdateMeetingResponseRequested(Windows.Foundation.EventRegistrationToken)
    def remove_update_meeting_response_requested(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...

@typing.final
class AppointmentDataProviderTriggerDetails(winrt.system.Object):
    # Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderConnection Windows.ApplicationModel.Appointments.DataProvider.AppointmentDataProviderTriggerDetails::get_Connection()
    @_property
    def connection(self) -> AppointmentDataProviderConnection: ...

