# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum
import datetime
import sys
import types
import typing

import winrt.system
import winrt.windows.foundation

class ExtendedExecutionReason(enum.IntEnum):
    UNSPECIFIED = 0
    LOCATION_TRACKING = 1
    SAVING_DATA = 2

class ExtendedExecutionResult(enum.IntEnum):
    ALLOWED = 0
    DENIED = 1

class ExtendedExecutionRevokedReason(enum.IntEnum):
    RESUMED = 0
    SYSTEM_POLICY = 1

Self = typing.TypeVar('Self')

class ExtendedExecutionRevokedEventArgs(winrt.system.Object):
    reason: ExtendedExecutionRevokedReason
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ExtendedExecutionRevokedEventArgs: ...

class ExtendedExecutionSession(winrt.system.Object):
    reason: ExtendedExecutionReason
    percent_progress: winrt.system.UInt32
    description: str
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, *args) -> None: ...
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ExtendedExecutionSession: ...
    def __new__(cls: typing.Type[ExtendedExecutionSession]) -> ExtendedExecutionSession:...
    def close(self) -> None: ...
    def request_extension_async(self) -> winrt.windows.foundation.IAsyncOperation[ExtendedExecutionResult]: ...
    def add_revoked(self, handler: winrt.windows.foundation.TypedEventHandler[winrt.system.Object, ExtendedExecutionRevokedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_revoked(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
