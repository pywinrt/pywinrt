# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system

from winrt.windows.phone.applicationmodel import ApplicationProfileModes

Self = typing.TypeVar('Self')

class ApplicationProfile_Static(type):
    @_property
    def modes(cls) -> ApplicationProfileModes: ...

class ApplicationProfile(winrt.system.Object, metaclass=ApplicationProfile_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ApplicationProfile: ...
