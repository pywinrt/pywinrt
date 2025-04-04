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

from winrt.windows.applicationmodel.extendedexecution import ExtendedExecutionReason, ExtendedExecutionResult, ExtendedExecutionRevokedReason

Self = typing.TypeVar('Self')

@typing.final
class ExtendedExecutionRevokedEventArgs(winrt.system.Object):
    # Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionRevokedReason Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionRevokedEventArgs::get_Reason()
    @_property
    def reason(self) -> ExtendedExecutionRevokedReason: ...

@typing.final
class ExtendedExecutionSession(winrt.system.Object, windows_foundation.IClosable):
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, exc_type: typing.Optional[typing.Type[BaseException]], exc_value: typing.Optional[BaseException], traceback: typing.Optional[types.TracebackType]) -> None: ...
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # System.Void Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionSession::Close()
    def close(self) -> None: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionResult> Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionSession::RequestExtensionAsync()
    def request_extension_async(self) -> windows_foundation.IAsyncOperation[ExtendedExecutionResult]: ...
    # Windows.Foundation.EventRegistrationToken Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionSession::add_Revoked(Windows.Foundation.TypedEventHandler`2<System.Object,Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionRevokedEventArgs>)
    def add_revoked(self, handler: windows_foundation.TypedEventHandler[winrt.system.Object, ExtendedExecutionRevokedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionSession::remove_Revoked(Windows.Foundation.EventRegistrationToken)
    def remove_revoked(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionReason Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionSession::get_Reason()
    @_property
    def reason(self) -> ExtendedExecutionReason: ...
    # System.Void Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionSession::put_Reason(Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionReason)
    @reason.setter
    def reason(self, value: ExtendedExecutionReason) -> None: ...
    # System.UInt32 Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionSession::get_PercentProgress()
    @_property
    def percent_progress(self) -> winrt.system.UInt32: ...
    # System.Void Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionSession::put_PercentProgress(System.UInt32)
    @percent_progress.setter
    def percent_progress(self, value: winrt.system.UInt32) -> None: ...
    # System.String Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionSession::get_Description()
    @_property
    def description(self) -> str: ...
    # System.Void Windows.ApplicationModel.ExtendedExecution.ExtendedExecutionSession::put_Description(System.String)
    @description.setter
    def description(self, value: str) -> None: ...

