# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum
import datetime
import sys
import types
import typing

import winrt.system

class ApplicationProfileModes(enum.IntFlag):
    DEFAULT = 0
    ALTERNATE = 0x1

Self = typing.TypeVar('Self')

class ApplicationProfile(winrt.system.Object):
    modes: typing.ClassVar[ApplicationProfileModes]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ApplicationProfile: ...
