# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system

Self = typing.TypeVar('Self')

@typing.final
class SystemProtection_Static(winrt._winrt.IInspectable_Static):
    # System.Void Windows.Phone.System.SystemProtection::RequestScreenUnlock()
    def request_screen_unlock(cls) -> None: ...
    # System.Boolean Windows.Phone.System.SystemProtection::get_ScreenLocked()
    @_property
    def screen_locked(cls) -> bool: ...

@typing.final
class SystemProtection(winrt.system.Object, metaclass=SystemProtection_Static):
    pass

