# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.applicationmodel.background
import winrt.windows.foundation

from winrt.microsoft.windows.pushnotifications import PushNotificationChannelStatus

Self = typing.TypeVar('Self')

@typing.final
class PushNotificationCreateChannelStatus:
    status: PushNotificationChannelStatus
    extended_error: winrt.windows.foundation.HResult
    retry_count: winrt.system.UInt32
    def __init__(self, status: PushNotificationChannelStatus, extended_error: winrt.windows.foundation.HResult, retry_count: winrt.system.UInt32) -> None: ...

@typing.final
class PushNotificationChannel(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> PushNotificationChannel: ...
    def close(self) -> None: ...
    @_property
    def expiration_time(self) -> datetime.datetime: ...
    @_property
    def uri(self) -> typing.Optional[winrt.windows.foundation.Uri]: ...

@typing.final
class PushNotificationCreateChannelResult(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> PushNotificationCreateChannelResult: ...
    @_property
    def channel(self) -> typing.Optional[PushNotificationChannel]: ...
    @_property
    def extended_error(self) -> winrt.windows.foundation.HResult: ...
    @_property
    def status(self) -> PushNotificationChannelStatus: ...

@typing.final
class PushNotificationManager_Static(type):
    def is_supported(cls) -> bool: ...
    @_property
    def default(cls) -> typing.Optional[PushNotificationManager]: ...

@typing.final
class PushNotificationManager(winrt.system.Object, metaclass=PushNotificationManager_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> PushNotificationManager: ...
    def create_channel_async(self, remote_id: _uuid.UUID, /) -> winrt.windows.foundation.IAsyncOperationWithProgress[PushNotificationCreateChannelResult, PushNotificationCreateChannelStatus]: ...
    def register(self) -> None: ...
    def unregister(self) -> None: ...
    def unregister_all(self) -> None: ...
    def add_push_received(self, handler: winrt.windows.foundation.TypedEventHandler[PushNotificationManager, PushNotificationReceivedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_push_received(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

@typing.final
class PushNotificationReceivedEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> PushNotificationReceivedEventArgs: ...
    def get_deferral(self) -> typing.Optional[winrt.windows.applicationmodel.background.BackgroundTaskDeferral]: ...
    def add_canceled(self, handler: typing.Optional[winrt.windows.applicationmodel.background.BackgroundTaskCanceledEventHandler], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_canceled(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def payload(self) -> winrt.system.UInt8: ...

