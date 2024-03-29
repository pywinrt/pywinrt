# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.foundation
import winrt.windows.foundation.collections

from winrt.windows.security.authentication.web import TokenBindingKeyType, WebAuthenticationOptions, WebAuthenticationStatus

Self = typing.TypeVar('Self')

@typing.final
class WebAuthenticationBroker_Static(type):
    @typing.overload
    def authenticate_and_continue(cls, request_uri: typing.Optional[winrt.windows.foundation.Uri], /) -> None: ...
    @typing.overload
    def authenticate_and_continue(cls, request_uri: typing.Optional[winrt.windows.foundation.Uri], callback_uri: typing.Optional[winrt.windows.foundation.Uri], /) -> None: ...
    @typing.overload
    def authenticate_and_continue(cls, request_uri: typing.Optional[winrt.windows.foundation.Uri], callback_uri: typing.Optional[winrt.windows.foundation.Uri], continuation_data: typing.Optional[winrt.windows.foundation.collections.ValueSet], options: WebAuthenticationOptions, /) -> None: ...
    @typing.overload
    def authenticate_async(cls, options: WebAuthenticationOptions, request_uri: typing.Optional[winrt.windows.foundation.Uri], /) -> winrt.windows.foundation.IAsyncOperation[WebAuthenticationResult]: ...
    @typing.overload
    def authenticate_async(cls, options: WebAuthenticationOptions, request_uri: typing.Optional[winrt.windows.foundation.Uri], callback_uri: typing.Optional[winrt.windows.foundation.Uri], /) -> winrt.windows.foundation.IAsyncOperation[WebAuthenticationResult]: ...
    @typing.overload
    def authenticate_silently_async(cls, request_uri: typing.Optional[winrt.windows.foundation.Uri], /) -> winrt.windows.foundation.IAsyncOperation[WebAuthenticationResult]: ...
    @typing.overload
    def authenticate_silently_async(cls, request_uri: typing.Optional[winrt.windows.foundation.Uri], options: WebAuthenticationOptions, /) -> winrt.windows.foundation.IAsyncOperation[WebAuthenticationResult]: ...
    def get_current_application_callback_uri(cls) -> typing.Optional[winrt.windows.foundation.Uri]: ...

@typing.final
class WebAuthenticationBroker(winrt.system.Object, metaclass=WebAuthenticationBroker_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> WebAuthenticationBroker: ...

@typing.final
class WebAuthenticationResult(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> WebAuthenticationResult: ...
    @_property
    def response_data(self) -> str: ...
    @_property
    def response_error_detail(self) -> winrt.system.UInt32: ...
    @_property
    def response_status(self) -> WebAuthenticationStatus: ...

