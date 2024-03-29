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

from winrt.microsoft.ui.dispatching import DispatcherQueuePriority, DispatcherRunOptions
from winrt.microsoft.ui.dispatching import DispatcherQueueHandler

Self = typing.TypeVar('Self')

@typing.final
class DispatcherExitDeferral(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> DispatcherExitDeferral: ...
    def __new__(cls: typing.Type[DispatcherExitDeferral]) -> DispatcherExitDeferral:...
    def complete(self) -> None: ...

@typing.final
class DispatcherQueue_Static(type):
    def get_for_current_thread(cls) -> typing.Optional[DispatcherQueue]: ...

@typing.final
class DispatcherQueue(winrt.system.Object, metaclass=DispatcherQueue_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> DispatcherQueue: ...
    def create_timer(self) -> typing.Optional[DispatcherQueueTimer]: ...
    def enqueue_event_loop_exit(self) -> None: ...
    def ensure_system_dispatcher_queue(self) -> None: ...
    @typing.overload
    def run_event_loop(self) -> None: ...
    @typing.overload
    def run_event_loop(self, options: DispatcherRunOptions, deferral: typing.Optional[DispatcherExitDeferral], /) -> None: ...
    @typing.overload
    def try_enqueue(self, callback: typing.Optional[DispatcherQueueHandler], /) -> bool: ...
    @typing.overload
    def try_enqueue(self, priority: DispatcherQueuePriority, callback: typing.Optional[DispatcherQueueHandler], /) -> bool: ...
    def add_shutdown_completed(self, handler: winrt.windows.foundation.TypedEventHandler[DispatcherQueue, winrt.system.Object], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_shutdown_completed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_shutdown_starting(self, handler: winrt.windows.foundation.TypedEventHandler[DispatcherQueue, DispatcherQueueShutdownStartingEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_shutdown_starting(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_framework_shutdown_completed(self, handler: winrt.windows.foundation.TypedEventHandler[DispatcherQueue, winrt.system.Object], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_framework_shutdown_completed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_framework_shutdown_starting(self, handler: winrt.windows.foundation.TypedEventHandler[DispatcherQueue, DispatcherQueueShutdownStartingEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_framework_shutdown_starting(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def has_thread_access(self) -> bool: ...

@typing.final
class DispatcherQueueController_Static(type):
    def create_on_current_thread(cls) -> typing.Optional[DispatcherQueueController]: ...
    def create_on_dedicated_thread(cls) -> typing.Optional[DispatcherQueueController]: ...

@typing.final
class DispatcherQueueController(winrt.system.Object, metaclass=DispatcherQueueController_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> DispatcherQueueController: ...
    def shutdown_queue(self) -> None: ...
    def shutdown_queue_async(self) -> winrt.windows.foundation.IAsyncAction: ...
    @_property
    def dispatcher_queue(self) -> typing.Optional[DispatcherQueue]: ...

@typing.final
class DispatcherQueueShutdownStartingEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> DispatcherQueueShutdownStartingEventArgs: ...
    def get_deferral(self) -> typing.Optional[winrt.windows.foundation.Deferral]: ...

@typing.final
class DispatcherQueueTimer(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> DispatcherQueueTimer: ...
    def start(self) -> None: ...
    def stop(self) -> None: ...
    def add_tick(self, handler: winrt.windows.foundation.TypedEventHandler[DispatcherQueueTimer, winrt.system.Object], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_tick(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def is_repeating(self) -> bool: ...
    @is_repeating.setter
    def is_repeating(self, value: bool) -> None: ...
    @_property
    def interval(self) -> datetime.timedelta: ...
    @interval.setter
    def interval(self, value: datetime.timedelta) -> None: ...
    @_property
    def is_running(self) -> bool: ...

