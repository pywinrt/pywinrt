# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing

import winrt.system
import winrt.windows.foundation
import winrt.windows.foundation.collections
import winrt.windows.storage.streams

Self = typing.TypeVar('Self')

class LockApplicationHost(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> LockApplicationHost: ...
    @staticmethod
    def get_for_current_view() -> typing.Optional[LockApplicationHost]: ...
    def request_unlock(self) -> None: ...
    def add_unlocking(self, handler: winrt.windows.foundation.TypedEventHandler[LockApplicationHost, LockScreenUnlockingEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_unlocking(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class LockScreenBadge(winrt.system.Object):
    automation_name: str
    glyph: typing.Optional[winrt.windows.storage.streams.IRandomAccessStream]
    logo: typing.Optional[winrt.windows.storage.streams.IRandomAccessStream]
    number: typing.Optional[typing.Optional[winrt.system.UInt32]]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> LockScreenBadge: ...
    def launch_app(self) -> None: ...

class LockScreenInfo(winrt.system.Object):
    alarm_icon: typing.Optional[winrt.windows.storage.streams.IRandomAccessStream]
    badges: typing.Optional[winrt.windows.foundation.collections.IVectorView[LockScreenBadge]]
    detail_text: typing.Optional[winrt.windows.foundation.collections.IVectorView[str]]
    lock_screen_image: typing.Optional[winrt.windows.storage.streams.IRandomAccessStream]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> LockScreenInfo: ...
    def add_alarm_icon_changed(self, handler: winrt.windows.foundation.TypedEventHandler[LockScreenInfo, winrt.system.Object], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_alarm_icon_changed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_badges_changed(self, handler: winrt.windows.foundation.TypedEventHandler[LockScreenInfo, winrt.system.Object], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_badges_changed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_detail_text_changed(self, handler: winrt.windows.foundation.TypedEventHandler[LockScreenInfo, winrt.system.Object], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_detail_text_changed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_lock_screen_image_changed(self, handler: winrt.windows.foundation.TypedEventHandler[LockScreenInfo, winrt.system.Object], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_lock_screen_image_changed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class LockScreenUnlockingDeferral(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> LockScreenUnlockingDeferral: ...
    def complete(self) -> None: ...

class LockScreenUnlockingEventArgs(winrt.system.Object):
    deadline: datetime.datetime
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> LockScreenUnlockingEventArgs: ...
    def get_deferral(self) -> typing.Optional[LockScreenUnlockingDeferral]: ...
