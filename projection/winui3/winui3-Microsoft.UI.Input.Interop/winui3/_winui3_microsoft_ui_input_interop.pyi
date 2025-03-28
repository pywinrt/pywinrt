# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.devices.input as windows_devices_input
import winui3.microsoft.ui.input as microsoft_ui_input

Self = typing.TypeVar('Self')

@typing.final
class PenDeviceInterop_Static(winrt._winrt.IInspectable_Static):
    # Windows.Devices.Input.PenDevice Microsoft.UI.Input.Interop.PenDeviceInterop::FromPointerPoint(Microsoft.UI.Input.PointerPoint)
    def from_pointer_point(cls, pointer_point: microsoft_ui_input.PointerPoint, /) -> windows_devices_input.PenDevice: ...

@typing.final
class PenDeviceInterop(winrt.system.Object, metaclass=PenDeviceInterop_Static):
    pass

