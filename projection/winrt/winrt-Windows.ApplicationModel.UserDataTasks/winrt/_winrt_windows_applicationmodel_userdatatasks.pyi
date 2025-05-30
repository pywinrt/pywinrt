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
import winrt.windows.system as windows_system

from winrt.windows.applicationmodel.userdatatasks import UserDataTaskDaysOfWeek, UserDataTaskDetailsKind, UserDataTaskKind, UserDataTaskListOtherAppReadAccess, UserDataTaskListOtherAppWriteAccess, UserDataTaskListSyncStatus, UserDataTaskPriority, UserDataTaskQueryKind, UserDataTaskQuerySortProperty, UserDataTaskRecurrenceUnit, UserDataTaskRegenerationUnit, UserDataTaskSensitivity, UserDataTaskStoreAccessType, UserDataTaskWeekOfMonth

Self = typing.TypeVar('Self')

@typing.final
class UserDataTask(winrt.system.Object):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # Windows.ApplicationModel.UserDataTasks.UserDataTaskRegenerationProperties Windows.ApplicationModel.UserDataTasks.UserDataTask::get_RegenerationProperties()
    @_property
    def regeneration_properties(self) -> UserDataTaskRegenerationProperties: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTask::put_RegenerationProperties(Windows.ApplicationModel.UserDataTasks.UserDataTaskRegenerationProperties)
    @regeneration_properties.setter
    def regeneration_properties(self, value: UserDataTaskRegenerationProperties) -> None: ...
    # Windows.ApplicationModel.UserDataTasks.UserDataTaskPriority Windows.ApplicationModel.UserDataTasks.UserDataTask::get_Priority()
    @_property
    def priority(self) -> UserDataTaskPriority: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTask::put_Priority(Windows.ApplicationModel.UserDataTasks.UserDataTaskPriority)
    @priority.setter
    def priority(self, value: UserDataTaskPriority) -> None: ...
    # Windows.Foundation.IReference`1<Windows.Foundation.DateTime> Windows.ApplicationModel.UserDataTasks.UserDataTask::get_CompletedDate()
    @_property
    def completed_date(self) -> typing.Optional[datetime.datetime]: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTask::put_CompletedDate(Windows.Foundation.IReference`1<Windows.Foundation.DateTime>)
    @completed_date.setter
    def completed_date(self, value: typing.Optional[datetime.datetime]) -> None: ...
    # Windows.Foundation.IReference`1<Windows.Foundation.DateTime> Windows.ApplicationModel.UserDataTasks.UserDataTask::get_DueDate()
    @_property
    def due_date(self) -> typing.Optional[datetime.datetime]: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTask::put_DueDate(Windows.Foundation.IReference`1<Windows.Foundation.DateTime>)
    @due_date.setter
    def due_date(self, value: typing.Optional[datetime.datetime]) -> None: ...
    # Windows.ApplicationModel.UserDataTasks.UserDataTaskDetailsKind Windows.ApplicationModel.UserDataTasks.UserDataTask::get_DetailsKind()
    @_property
    def details_kind(self) -> UserDataTaskDetailsKind: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTask::put_DetailsKind(Windows.ApplicationModel.UserDataTasks.UserDataTaskDetailsKind)
    @details_kind.setter
    def details_kind(self, value: UserDataTaskDetailsKind) -> None: ...
    # Windows.Foundation.IReference`1<Windows.Foundation.DateTime> Windows.ApplicationModel.UserDataTasks.UserDataTask::get_StartDate()
    @_property
    def start_date(self) -> typing.Optional[datetime.datetime]: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTask::put_StartDate(Windows.Foundation.IReference`1<Windows.Foundation.DateTime>)
    @start_date.setter
    def start_date(self, value: typing.Optional[datetime.datetime]) -> None: ...
    # System.String Windows.ApplicationModel.UserDataTasks.UserDataTask::get_Details()
    @_property
    def details(self) -> str: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTask::put_Details(System.String)
    @details.setter
    def details(self, value: str) -> None: ...
    # System.String Windows.ApplicationModel.UserDataTasks.UserDataTask::get_Subject()
    @_property
    def subject(self) -> str: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTask::put_Subject(System.String)
    @subject.setter
    def subject(self, value: str) -> None: ...
    # Windows.ApplicationModel.UserDataTasks.UserDataTaskSensitivity Windows.ApplicationModel.UserDataTasks.UserDataTask::get_Sensitivity()
    @_property
    def sensitivity(self) -> UserDataTaskSensitivity: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTask::put_Sensitivity(Windows.ApplicationModel.UserDataTasks.UserDataTaskSensitivity)
    @sensitivity.setter
    def sensitivity(self, value: UserDataTaskSensitivity) -> None: ...
    # System.String Windows.ApplicationModel.UserDataTasks.UserDataTask::get_RemoteId()
    @_property
    def remote_id(self) -> str: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTask::put_RemoteId(System.String)
    @remote_id.setter
    def remote_id(self, value: str) -> None: ...
    # Windows.Foundation.IReference`1<Windows.Foundation.DateTime> Windows.ApplicationModel.UserDataTasks.UserDataTask::get_Reminder()
    @_property
    def reminder(self) -> typing.Optional[datetime.datetime]: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTask::put_Reminder(Windows.Foundation.IReference`1<Windows.Foundation.DateTime>)
    @reminder.setter
    def reminder(self, value: typing.Optional[datetime.datetime]) -> None: ...
    # Windows.ApplicationModel.UserDataTasks.UserDataTaskRecurrenceProperties Windows.ApplicationModel.UserDataTasks.UserDataTask::get_RecurrenceProperties()
    @_property
    def recurrence_properties(self) -> UserDataTaskRecurrenceProperties: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTask::put_RecurrenceProperties(Windows.ApplicationModel.UserDataTasks.UserDataTaskRecurrenceProperties)
    @recurrence_properties.setter
    def recurrence_properties(self, value: UserDataTaskRecurrenceProperties) -> None: ...
    # System.String Windows.ApplicationModel.UserDataTasks.UserDataTask::get_Id()
    @_property
    def id(self) -> str: ...
    # Windows.ApplicationModel.UserDataTasks.UserDataTaskKind Windows.ApplicationModel.UserDataTasks.UserDataTask::get_Kind()
    @_property
    def kind(self) -> UserDataTaskKind: ...
    # System.String Windows.ApplicationModel.UserDataTasks.UserDataTask::get_ListId()
    @_property
    def list_id(self) -> str: ...

@typing.final
class UserDataTaskBatch(winrt.system.Object):
    # Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.UserDataTasks.UserDataTask> Windows.ApplicationModel.UserDataTasks.UserDataTaskBatch::get_Tasks()
    @_property
    def tasks(self) -> typing.Sequence[UserDataTask]: ...

@typing.final
class UserDataTaskList(winrt.system.Object):
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.UserDataTasks.UserDataTaskList::DeleteAsync()
    def delete_async(self) -> windows_foundation.IAsyncAction: ...
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.UserDataTasks.UserDataTaskList::DeleteTaskAsync(System.String)
    def delete_task_async(self, user_data_task_id: str, /) -> windows_foundation.IAsyncAction: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.UserDataTasks.UserDataTask> Windows.ApplicationModel.UserDataTasks.UserDataTaskList::GetTaskAsync(System.String)
    def get_task_async(self, user_data_task: str, /) -> windows_foundation.IAsyncOperation[UserDataTask]: ...
    # Windows.ApplicationModel.UserDataTasks.UserDataTaskReader Windows.ApplicationModel.UserDataTasks.UserDataTaskList::GetTaskReader()
    def get_task_reader(self) -> UserDataTaskReader: ...
    # Windows.ApplicationModel.UserDataTasks.UserDataTaskReader Windows.ApplicationModel.UserDataTasks.UserDataTaskList::GetTaskReader(Windows.ApplicationModel.UserDataTasks.UserDataTaskQueryOptions)
    def get_task_reader_with_options(self, options: UserDataTaskQueryOptions, /) -> UserDataTaskReader: ...
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.UserDataTasks.UserDataTaskList::RegisterSyncManagerAsync()
    def register_sync_manager_async(self) -> windows_foundation.IAsyncAction: ...
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.UserDataTasks.UserDataTaskList::SaveAsync()
    def save_async(self) -> windows_foundation.IAsyncAction: ...
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.UserDataTasks.UserDataTaskList::SaveTaskAsync(Windows.ApplicationModel.UserDataTasks.UserDataTask)
    def save_task_async(self, user_data_task: UserDataTask, /) -> windows_foundation.IAsyncAction: ...
    # Windows.ApplicationModel.UserDataTasks.UserDataTaskListOtherAppWriteAccess Windows.ApplicationModel.UserDataTasks.UserDataTaskList::get_OtherAppWriteAccess()
    @_property
    def other_app_write_access(self) -> UserDataTaskListOtherAppWriteAccess: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskList::put_OtherAppWriteAccess(Windows.ApplicationModel.UserDataTasks.UserDataTaskListOtherAppWriteAccess)
    @other_app_write_access.setter
    def other_app_write_access(self, value: UserDataTaskListOtherAppWriteAccess) -> None: ...
    # Windows.ApplicationModel.UserDataTasks.UserDataTaskListOtherAppReadAccess Windows.ApplicationModel.UserDataTasks.UserDataTaskList::get_OtherAppReadAccess()
    @_property
    def other_app_read_access(self) -> UserDataTaskListOtherAppReadAccess: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskList::put_OtherAppReadAccess(Windows.ApplicationModel.UserDataTasks.UserDataTaskListOtherAppReadAccess)
    @other_app_read_access.setter
    def other_app_read_access(self, value: UserDataTaskListOtherAppReadAccess) -> None: ...
    # System.String Windows.ApplicationModel.UserDataTasks.UserDataTaskList::get_DisplayName()
    @_property
    def display_name(self) -> str: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskList::put_DisplayName(System.String)
    @display_name.setter
    def display_name(self, value: str) -> None: ...
    # System.String Windows.ApplicationModel.UserDataTasks.UserDataTaskList::get_Id()
    @_property
    def id(self) -> str: ...
    # Windows.ApplicationModel.UserDataTasks.UserDataTaskListLimitedWriteOperations Windows.ApplicationModel.UserDataTasks.UserDataTaskList::get_LimitedWriteOperations()
    @_property
    def limited_write_operations(self) -> UserDataTaskListLimitedWriteOperations: ...
    # System.String Windows.ApplicationModel.UserDataTasks.UserDataTaskList::get_SourceDisplayName()
    @_property
    def source_display_name(self) -> str: ...
    # Windows.ApplicationModel.UserDataTasks.UserDataTaskListSyncManager Windows.ApplicationModel.UserDataTasks.UserDataTaskList::get_SyncManager()
    @_property
    def sync_manager(self) -> UserDataTaskListSyncManager: ...
    # System.String Windows.ApplicationModel.UserDataTasks.UserDataTaskList::get_UserDataAccountId()
    @_property
    def user_data_account_id(self) -> str: ...

@typing.final
class UserDataTaskListLimitedWriteOperations(winrt.system.Object):
    # Windows.Foundation.IAsyncOperation`1<System.String> Windows.ApplicationModel.UserDataTasks.UserDataTaskListLimitedWriteOperations::TryCompleteTaskAsync(System.String)
    def try_complete_task_async(self, user_data_task_id: str, /) -> windows_foundation.IAsyncOperation[str]: ...
    # Windows.Foundation.IAsyncOperation`1<System.Boolean> Windows.ApplicationModel.UserDataTasks.UserDataTaskListLimitedWriteOperations::TryCreateOrUpdateTaskAsync(Windows.ApplicationModel.UserDataTasks.UserDataTask)
    def try_create_or_update_task_async(self, user_data_task: UserDataTask, /) -> windows_foundation.IAsyncOperation[bool]: ...
    # Windows.Foundation.IAsyncOperation`1<System.Boolean> Windows.ApplicationModel.UserDataTasks.UserDataTaskListLimitedWriteOperations::TryDeleteTaskAsync(System.String)
    def try_delete_task_async(self, user_data_task_id: str, /) -> windows_foundation.IAsyncOperation[bool]: ...
    # Windows.Foundation.IAsyncOperation`1<System.Boolean> Windows.ApplicationModel.UserDataTasks.UserDataTaskListLimitedWriteOperations::TrySkipOccurrenceAsync(System.String)
    def try_skip_occurrence_async(self, user_data_task_id: str, /) -> windows_foundation.IAsyncOperation[bool]: ...

@typing.final
class UserDataTaskListSyncManager(winrt.system.Object):
    # Windows.Foundation.IAsyncOperation`1<System.Boolean> Windows.ApplicationModel.UserDataTasks.UserDataTaskListSyncManager::SyncAsync()
    def sync_async(self) -> windows_foundation.IAsyncOperation[bool]: ...
    # Windows.Foundation.EventRegistrationToken Windows.ApplicationModel.UserDataTasks.UserDataTaskListSyncManager::add_SyncStatusChanged(Windows.Foundation.TypedEventHandler`2<Windows.ApplicationModel.UserDataTasks.UserDataTaskListSyncManager,System.Object>)
    def add_sync_status_changed(self, handler: windows_foundation.TypedEventHandler[UserDataTaskListSyncManager, winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskListSyncManager::remove_SyncStatusChanged(Windows.Foundation.EventRegistrationToken)
    def remove_sync_status_changed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.ApplicationModel.UserDataTasks.UserDataTaskListSyncStatus Windows.ApplicationModel.UserDataTasks.UserDataTaskListSyncManager::get_Status()
    @_property
    def status(self) -> UserDataTaskListSyncStatus: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskListSyncManager::put_Status(Windows.ApplicationModel.UserDataTasks.UserDataTaskListSyncStatus)
    @status.setter
    def status(self, value: UserDataTaskListSyncStatus) -> None: ...
    # Windows.Foundation.DateTime Windows.ApplicationModel.UserDataTasks.UserDataTaskListSyncManager::get_LastSuccessfulSyncTime()
    @_property
    def last_successful_sync_time(self) -> datetime.datetime: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskListSyncManager::put_LastSuccessfulSyncTime(Windows.Foundation.DateTime)
    @last_successful_sync_time.setter
    def last_successful_sync_time(self, value: datetime.datetime) -> None: ...
    # Windows.Foundation.DateTime Windows.ApplicationModel.UserDataTasks.UserDataTaskListSyncManager::get_LastAttemptedSyncTime()
    @_property
    def last_attempted_sync_time(self) -> datetime.datetime: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskListSyncManager::put_LastAttemptedSyncTime(Windows.Foundation.DateTime)
    @last_attempted_sync_time.setter
    def last_attempted_sync_time(self, value: datetime.datetime) -> None: ...

@typing.final
class UserDataTaskManager_Static(winrt._winrt.IInspectable_Static):
    # Windows.ApplicationModel.UserDataTasks.UserDataTaskManager Windows.ApplicationModel.UserDataTasks.UserDataTaskManager::GetDefault()
    def get_default(cls) -> UserDataTaskManager: ...
    # Windows.ApplicationModel.UserDataTasks.UserDataTaskManager Windows.ApplicationModel.UserDataTasks.UserDataTaskManager::GetForUser(Windows.System.User)
    def get_for_user(cls, user: windows_system.User, /) -> UserDataTaskManager: ...

@typing.final
class UserDataTaskManager(winrt.system.Object, metaclass=UserDataTaskManager_Static):
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.UserDataTasks.UserDataTaskStore> Windows.ApplicationModel.UserDataTasks.UserDataTaskManager::RequestStoreAsync(Windows.ApplicationModel.UserDataTasks.UserDataTaskStoreAccessType)
    def request_store_async(self, access_type: UserDataTaskStoreAccessType, /) -> windows_foundation.IAsyncOperation[UserDataTaskStore]: ...
    # Windows.System.User Windows.ApplicationModel.UserDataTasks.UserDataTaskManager::get_User()
    @_property
    def user(self) -> windows_system.User: ...

@typing.final
class UserDataTaskQueryOptions(winrt.system.Object):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # Windows.ApplicationModel.UserDataTasks.UserDataTaskQuerySortProperty Windows.ApplicationModel.UserDataTasks.UserDataTaskQueryOptions::get_SortProperty()
    @_property
    def sort_property(self) -> UserDataTaskQuerySortProperty: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskQueryOptions::put_SortProperty(Windows.ApplicationModel.UserDataTasks.UserDataTaskQuerySortProperty)
    @sort_property.setter
    def sort_property(self, value: UserDataTaskQuerySortProperty) -> None: ...
    # Windows.ApplicationModel.UserDataTasks.UserDataTaskQueryKind Windows.ApplicationModel.UserDataTasks.UserDataTaskQueryOptions::get_Kind()
    @_property
    def kind(self) -> UserDataTaskQueryKind: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskQueryOptions::put_Kind(Windows.ApplicationModel.UserDataTasks.UserDataTaskQueryKind)
    @kind.setter
    def kind(self, value: UserDataTaskQueryKind) -> None: ...

@typing.final
class UserDataTaskReader(winrt.system.Object):
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.UserDataTasks.UserDataTaskBatch> Windows.ApplicationModel.UserDataTasks.UserDataTaskReader::ReadBatchAsync()
    def read_batch_async(self) -> windows_foundation.IAsyncOperation[UserDataTaskBatch]: ...

@typing.final
class UserDataTaskRecurrenceProperties(winrt.system.Object):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # Windows.Foundation.IReference`1<Windows.ApplicationModel.UserDataTasks.UserDataTaskWeekOfMonth> Windows.ApplicationModel.UserDataTasks.UserDataTaskRecurrenceProperties::get_WeekOfMonth()
    @_property
    def week_of_month(self) -> typing.Optional[UserDataTaskWeekOfMonth]: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskRecurrenceProperties::put_WeekOfMonth(Windows.Foundation.IReference`1<Windows.ApplicationModel.UserDataTasks.UserDataTaskWeekOfMonth>)
    @week_of_month.setter
    def week_of_month(self, value: typing.Optional[UserDataTaskWeekOfMonth]) -> None: ...
    # Windows.Foundation.IReference`1<Windows.Foundation.DateTime> Windows.ApplicationModel.UserDataTasks.UserDataTaskRecurrenceProperties::get_Until()
    @_property
    def until(self) -> typing.Optional[datetime.datetime]: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskRecurrenceProperties::put_Until(Windows.Foundation.IReference`1<Windows.Foundation.DateTime>)
    @until.setter
    def until(self, value: typing.Optional[datetime.datetime]) -> None: ...
    # Windows.ApplicationModel.UserDataTasks.UserDataTaskRecurrenceUnit Windows.ApplicationModel.UserDataTasks.UserDataTaskRecurrenceProperties::get_Unit()
    @_property
    def unit(self) -> UserDataTaskRecurrenceUnit: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskRecurrenceProperties::put_Unit(Windows.ApplicationModel.UserDataTasks.UserDataTaskRecurrenceUnit)
    @unit.setter
    def unit(self, value: UserDataTaskRecurrenceUnit) -> None: ...
    # Windows.Foundation.IReference`1<System.Int32> Windows.ApplicationModel.UserDataTasks.UserDataTaskRecurrenceProperties::get_Occurrences()
    @_property
    def occurrences(self) -> typing.Optional[winrt.system.Int32]: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskRecurrenceProperties::put_Occurrences(Windows.Foundation.IReference`1<System.Int32>)
    @occurrences.setter
    def occurrences(self, value: typing.Optional[winrt.system.Int32]) -> None: ...
    # Windows.Foundation.IReference`1<System.Int32> Windows.ApplicationModel.UserDataTasks.UserDataTaskRecurrenceProperties::get_Month()
    @_property
    def month(self) -> typing.Optional[winrt.system.Int32]: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskRecurrenceProperties::put_Month(Windows.Foundation.IReference`1<System.Int32>)
    @month.setter
    def month(self, value: typing.Optional[winrt.system.Int32]) -> None: ...
    # System.Int32 Windows.ApplicationModel.UserDataTasks.UserDataTaskRecurrenceProperties::get_Interval()
    @_property
    def interval(self) -> winrt.system.Int32: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskRecurrenceProperties::put_Interval(System.Int32)
    @interval.setter
    def interval(self, value: winrt.system.Int32) -> None: ...
    # Windows.Foundation.IReference`1<Windows.ApplicationModel.UserDataTasks.UserDataTaskDaysOfWeek> Windows.ApplicationModel.UserDataTasks.UserDataTaskRecurrenceProperties::get_DaysOfWeek()
    @_property
    def days_of_week(self) -> typing.Optional[UserDataTaskDaysOfWeek]: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskRecurrenceProperties::put_DaysOfWeek(Windows.Foundation.IReference`1<Windows.ApplicationModel.UserDataTasks.UserDataTaskDaysOfWeek>)
    @days_of_week.setter
    def days_of_week(self, value: typing.Optional[UserDataTaskDaysOfWeek]) -> None: ...
    # Windows.Foundation.IReference`1<System.Int32> Windows.ApplicationModel.UserDataTasks.UserDataTaskRecurrenceProperties::get_Day()
    @_property
    def day(self) -> typing.Optional[winrt.system.Int32]: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskRecurrenceProperties::put_Day(Windows.Foundation.IReference`1<System.Int32>)
    @day.setter
    def day(self, value: typing.Optional[winrt.system.Int32]) -> None: ...

@typing.final
class UserDataTaskRegenerationProperties(winrt.system.Object):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # Windows.Foundation.IReference`1<Windows.Foundation.DateTime> Windows.ApplicationModel.UserDataTasks.UserDataTaskRegenerationProperties::get_Until()
    @_property
    def until(self) -> typing.Optional[datetime.datetime]: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskRegenerationProperties::put_Until(Windows.Foundation.IReference`1<Windows.Foundation.DateTime>)
    @until.setter
    def until(self, value: typing.Optional[datetime.datetime]) -> None: ...
    # Windows.ApplicationModel.UserDataTasks.UserDataTaskRegenerationUnit Windows.ApplicationModel.UserDataTasks.UserDataTaskRegenerationProperties::get_Unit()
    @_property
    def unit(self) -> UserDataTaskRegenerationUnit: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskRegenerationProperties::put_Unit(Windows.ApplicationModel.UserDataTasks.UserDataTaskRegenerationUnit)
    @unit.setter
    def unit(self, value: UserDataTaskRegenerationUnit) -> None: ...
    # Windows.Foundation.IReference`1<System.Int32> Windows.ApplicationModel.UserDataTasks.UserDataTaskRegenerationProperties::get_Occurrences()
    @_property
    def occurrences(self) -> typing.Optional[winrt.system.Int32]: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskRegenerationProperties::put_Occurrences(Windows.Foundation.IReference`1<System.Int32>)
    @occurrences.setter
    def occurrences(self, value: typing.Optional[winrt.system.Int32]) -> None: ...
    # System.Int32 Windows.ApplicationModel.UserDataTasks.UserDataTaskRegenerationProperties::get_Interval()
    @_property
    def interval(self) -> winrt.system.Int32: ...
    # System.Void Windows.ApplicationModel.UserDataTasks.UserDataTaskRegenerationProperties::put_Interval(System.Int32)
    @interval.setter
    def interval(self, value: winrt.system.Int32) -> None: ...

@typing.final
class UserDataTaskStore(winrt.system.Object):
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.UserDataTasks.UserDataTaskList> Windows.ApplicationModel.UserDataTasks.UserDataTaskStore::CreateListAsync(System.String)
    def create_list_async(self, name: str, /) -> windows_foundation.IAsyncOperation[UserDataTaskList]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.UserDataTasks.UserDataTaskList> Windows.ApplicationModel.UserDataTasks.UserDataTaskStore::CreateListAsync(System.String,System.String)
    def create_list_in_account_async(self, name: str, user_data_account_id: str, /) -> windows_foundation.IAsyncOperation[UserDataTaskList]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.UserDataTasks.UserDataTaskList>> Windows.ApplicationModel.UserDataTasks.UserDataTaskStore::FindListsAsync()
    def find_lists_async(self) -> windows_foundation.IAsyncOperation[typing.Sequence[UserDataTaskList]]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.UserDataTasks.UserDataTaskList> Windows.ApplicationModel.UserDataTasks.UserDataTaskStore::GetListAsync(System.String)
    def get_list_async(self, task_list_id: str, /) -> windows_foundation.IAsyncOperation[UserDataTaskList]: ...

