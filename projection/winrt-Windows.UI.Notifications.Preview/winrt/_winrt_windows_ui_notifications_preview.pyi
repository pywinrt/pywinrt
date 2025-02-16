# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.ui as windows_ui

Self = typing.TypeVar('Self')

@typing.final
class ToastOcclusionManagerPreview_Static(winrt._winrt.IInspectable_Static):
    # System.Void Windows.UI.Notifications.Preview.ToastOcclusionManagerPreview::SetToastWindowMargin(Windows.UI.WindowId,System.Double)
    def set_toast_window_margin(cls, app_window_id: typing.Union[windows_ui.WindowId, typing.Tuple[winrt.system.UInt64]], margin: winrt.system.Double, /) -> None: ...

@typing.final
class ToastOcclusionManagerPreview(winrt.system.Object, metaclass=ToastOcclusionManagerPreview_Static):
    pass

