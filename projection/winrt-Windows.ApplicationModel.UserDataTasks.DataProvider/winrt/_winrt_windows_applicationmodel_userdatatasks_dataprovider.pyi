# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.applicationmodel.userdatatasks
import winrt.windows.foundation

Self = typing.TypeVar('Self')

class UserDataTaskDataProviderConnection(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> UserDataTaskDataProviderConnection: ...
    def start(self) -> None: ...
    def add_complete_task_requested(self, handler: winrt.windows.foundation.TypedEventHandler[UserDataTaskDataProviderConnection, UserDataTaskListCompleteTaskRequestEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_complete_task_requested(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_create_or_update_task_requested(self, handler: winrt.windows.foundation.TypedEventHandler[UserDataTaskDataProviderConnection, UserDataTaskListCreateOrUpdateTaskRequestEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_create_or_update_task_requested(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_delete_task_requested(self, handler: winrt.windows.foundation.TypedEventHandler[UserDataTaskDataProviderConnection, UserDataTaskListDeleteTaskRequestEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_delete_task_requested(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_skip_occurrence_requested(self, handler: winrt.windows.foundation.TypedEventHandler[UserDataTaskDataProviderConnection, UserDataTaskListSkipOccurrenceRequestEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_skip_occurrence_requested(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_sync_requested(self, handler: winrt.windows.foundation.TypedEventHandler[UserDataTaskDataProviderConnection, UserDataTaskListSyncManagerSyncRequestEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_sync_requested(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class UserDataTaskDataProviderTriggerDetails(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> UserDataTaskDataProviderTriggerDetails: ...
    @_property
    def connection(self) -> typing.Optional[UserDataTaskDataProviderConnection]: ...

class UserDataTaskListCompleteTaskRequest(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> UserDataTaskListCompleteTaskRequest: ...
    def report_completed_async(self, completed_task_id: str, /) -> winrt.windows.foundation.IAsyncAction: ...
    def report_failed_async(self) -> winrt.windows.foundation.IAsyncAction: ...
    @_property
    def task_id(self) -> str: ...
    @_property
    def task_list_id(self) -> str: ...

class UserDataTaskListCompleteTaskRequestEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> UserDataTaskListCompleteTaskRequestEventArgs: ...
    def get_deferral(self) -> typing.Optional[winrt.windows.foundation.Deferral]: ...
    @_property
    def request(self) -> typing.Optional[UserDataTaskListCompleteTaskRequest]: ...

class UserDataTaskListCreateOrUpdateTaskRequest(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> UserDataTaskListCreateOrUpdateTaskRequest: ...
    def report_completed_async(self, created_or_updated_user_data_task: typing.Optional[winrt.windows.applicationmodel.userdatatasks.UserDataTask], /) -> winrt.windows.foundation.IAsyncAction: ...
    def report_failed_async(self) -> winrt.windows.foundation.IAsyncAction: ...
    @_property
    def task(self) -> typing.Optional[winrt.windows.applicationmodel.userdatatasks.UserDataTask]: ...
    @_property
    def task_list_id(self) -> str: ...

class UserDataTaskListCreateOrUpdateTaskRequestEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> UserDataTaskListCreateOrUpdateTaskRequestEventArgs: ...
    def get_deferral(self) -> typing.Optional[winrt.windows.foundation.Deferral]: ...
    @_property
    def request(self) -> typing.Optional[UserDataTaskListCreateOrUpdateTaskRequest]: ...

class UserDataTaskListDeleteTaskRequest(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> UserDataTaskListDeleteTaskRequest: ...
    def report_completed_async(self) -> winrt.windows.foundation.IAsyncAction: ...
    def report_failed_async(self) -> winrt.windows.foundation.IAsyncAction: ...
    @_property
    def task_id(self) -> str: ...
    @_property
    def task_list_id(self) -> str: ...

class UserDataTaskListDeleteTaskRequestEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> UserDataTaskListDeleteTaskRequestEventArgs: ...
    def get_deferral(self) -> typing.Optional[winrt.windows.foundation.Deferral]: ...
    @_property
    def request(self) -> typing.Optional[UserDataTaskListDeleteTaskRequest]: ...

class UserDataTaskListSkipOccurrenceRequest(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> UserDataTaskListSkipOccurrenceRequest: ...
    def report_completed_async(self) -> winrt.windows.foundation.IAsyncAction: ...
    def report_failed_async(self) -> winrt.windows.foundation.IAsyncAction: ...
    @_property
    def task_id(self) -> str: ...
    @_property
    def task_list_id(self) -> str: ...

class UserDataTaskListSkipOccurrenceRequestEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> UserDataTaskListSkipOccurrenceRequestEventArgs: ...
    def get_deferral(self) -> typing.Optional[winrt.windows.foundation.Deferral]: ...
    @_property
    def request(self) -> typing.Optional[UserDataTaskListSkipOccurrenceRequest]: ...

class UserDataTaskListSyncManagerSyncRequest(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> UserDataTaskListSyncManagerSyncRequest: ...
    def report_completed_async(self) -> winrt.windows.foundation.IAsyncAction: ...
    def report_failed_async(self) -> winrt.windows.foundation.IAsyncAction: ...
    @_property
    def task_list_id(self) -> str: ...

class UserDataTaskListSyncManagerSyncRequestEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> UserDataTaskListSyncManagerSyncRequestEventArgs: ...
    def get_deferral(self) -> typing.Optional[winrt.windows.foundation.Deferral]: ...
    @_property
    def request(self) -> typing.Optional[UserDataTaskListSyncManagerSyncRequest]: ...
