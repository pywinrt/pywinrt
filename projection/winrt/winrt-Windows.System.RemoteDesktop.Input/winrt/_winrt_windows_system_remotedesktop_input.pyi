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

from winrt.windows.system.remotedesktop.input import RemoteKeyEventAttributes, RemoteTextConnectionOptions
from winrt.windows.system.remotedesktop.input import RemoteTextConnectionDataHandler

Self = typing.TypeVar('Self')

@typing.final
class RemoteTextConnection(winrt.system.Object, windows_foundation.IClosable):
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, exc_type: typing.Optional[typing.Type[BaseException]], exc_value: typing.Optional[BaseException], traceback: typing.Optional[types.TracebackType]) -> None: ...
    @typing.overload
    def __new__(cls: typing.Type[Self], connection_id: _uuid.UUID, pdu_forwarder: RemoteTextConnectionDataHandler, options: RemoteTextConnectionOptions) -> Self: ...
    @typing.overload
    def __new__(cls: typing.Type[Self], connection_id: _uuid.UUID, pdu_forwarder: RemoteTextConnectionDataHandler) -> Self: ...
    # System.Void Windows.System.RemoteDesktop.Input.RemoteTextConnection::Close()
    def close(self) -> None: ...
    # System.Void Windows.System.RemoteDesktop.Input.RemoteTextConnection::RegisterThread(System.UInt32)
    def register_thread(self, thread_id: winrt.system.UInt32, /) -> None: ...
    # System.Void Windows.System.RemoteDesktop.Input.RemoteTextConnection::ReportDataReceived(System.Byte[])
    def report_data_received(self, pdu_data: typing.Union[winrt.system.Array[winrt.system.UInt8], winrt.system.ReadableBuffer], /) -> None: ...
    # System.Void Windows.System.RemoteDesktop.Input.RemoteTextConnection::ReportPredictedKeyEvent(System.UInt16,Windows.System.RemoteDesktop.Input.RemoteKeyEventAttributes)
    def report_predicted_key_event(self, scan_code: winrt.system.UInt16, attributes: RemoteKeyEventAttributes, /) -> None: ...
    # System.Void Windows.System.RemoteDesktop.Input.RemoteTextConnection::UnregisterThread(System.UInt32)
    def unregister_thread(self, thread_id: winrt.system.UInt32, /) -> None: ...
    # System.Boolean Windows.System.RemoteDesktop.Input.RemoteTextConnection::get_IsEnabled()
    @_property
    def is_enabled(self) -> bool: ...
    # System.Void Windows.System.RemoteDesktop.Input.RemoteTextConnection::put_IsEnabled(System.Boolean)
    @is_enabled.setter
    def is_enabled(self, value: bool) -> None: ...

