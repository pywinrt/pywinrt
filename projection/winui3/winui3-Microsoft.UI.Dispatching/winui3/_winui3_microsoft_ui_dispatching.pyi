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

from winui3.microsoft.ui.dispatching import DispatcherQueuePriority, DispatcherRunOptions
from winui3.microsoft.ui.dispatching import DispatcherQueueHandler

Self = typing.TypeVar('Self')

@typing.final
class DispatcherExitDeferral(winrt.system.Object):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # System.Void Microsoft.UI.Dispatching.DispatcherExitDeferral::Complete()
    def complete(self) -> None: ...

@typing.final
class DispatcherQueue_Static(winrt._winrt.IInspectable_Static):
    # Microsoft.UI.Dispatching.DispatcherQueue Microsoft.UI.Dispatching.DispatcherQueue::GetForCurrentThread()
    def get_for_current_thread(cls) -> DispatcherQueue: ...

@typing.final
class DispatcherQueue(winrt.system.Object, metaclass=DispatcherQueue_Static):
    # Microsoft.UI.Dispatching.DispatcherQueueTimer Microsoft.UI.Dispatching.DispatcherQueue::CreateTimer()
    def create_timer(self) -> DispatcherQueueTimer: ...
    # System.Void Microsoft.UI.Dispatching.DispatcherQueue::EnqueueEventLoopExit()
    def enqueue_event_loop_exit(self) -> None: ...
    # System.Void Microsoft.UI.Dispatching.DispatcherQueue::EnsureSystemDispatcherQueue()
    def ensure_system_dispatcher_queue(self) -> None: ...
    # System.Void Microsoft.UI.Dispatching.DispatcherQueue::RunEventLoop()
    def run_event_loop(self) -> None: ...
    # System.Void Microsoft.UI.Dispatching.DispatcherQueue::RunEventLoop(Microsoft.UI.Dispatching.DispatcherRunOptions,Microsoft.UI.Dispatching.DispatcherExitDeferral)
    def run_event_loop_with_options(self, options: DispatcherRunOptions, deferral: DispatcherExitDeferral, /) -> None: ...
    # System.Boolean Microsoft.UI.Dispatching.DispatcherQueue::TryEnqueue(Microsoft.UI.Dispatching.DispatcherQueueHandler)
    def try_enqueue(self, callback: DispatcherQueueHandler, /) -> bool: ...
    # System.Boolean Microsoft.UI.Dispatching.DispatcherQueue::TryEnqueue(Microsoft.UI.Dispatching.DispatcherQueuePriority,Microsoft.UI.Dispatching.DispatcherQueueHandler)
    def try_enqueue_with_priority(self, priority: DispatcherQueuePriority, callback: DispatcherQueueHandler, /) -> bool: ...
    # Windows.Foundation.EventRegistrationToken Microsoft.UI.Dispatching.DispatcherQueue::add_ShutdownCompleted(Windows.Foundation.TypedEventHandler`2<Microsoft.UI.Dispatching.DispatcherQueue,System.Object>)
    def add_shutdown_completed(self, handler: windows_foundation.TypedEventHandler[DispatcherQueue, winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Microsoft.UI.Dispatching.DispatcherQueue::remove_ShutdownCompleted(Windows.Foundation.EventRegistrationToken)
    def remove_shutdown_completed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Microsoft.UI.Dispatching.DispatcherQueue::add_ShutdownStarting(Windows.Foundation.TypedEventHandler`2<Microsoft.UI.Dispatching.DispatcherQueue,Microsoft.UI.Dispatching.DispatcherQueueShutdownStartingEventArgs>)
    def add_shutdown_starting(self, handler: windows_foundation.TypedEventHandler[DispatcherQueue, DispatcherQueueShutdownStartingEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Microsoft.UI.Dispatching.DispatcherQueue::remove_ShutdownStarting(Windows.Foundation.EventRegistrationToken)
    def remove_shutdown_starting(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Microsoft.UI.Dispatching.DispatcherQueue::add_FrameworkShutdownCompleted(Windows.Foundation.TypedEventHandler`2<Microsoft.UI.Dispatching.DispatcherQueue,System.Object>)
    def add_framework_shutdown_completed(self, handler: windows_foundation.TypedEventHandler[DispatcherQueue, winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Microsoft.UI.Dispatching.DispatcherQueue::remove_FrameworkShutdownCompleted(Windows.Foundation.EventRegistrationToken)
    def remove_framework_shutdown_completed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Microsoft.UI.Dispatching.DispatcherQueue::add_FrameworkShutdownStarting(Windows.Foundation.TypedEventHandler`2<Microsoft.UI.Dispatching.DispatcherQueue,Microsoft.UI.Dispatching.DispatcherQueueShutdownStartingEventArgs>)
    def add_framework_shutdown_starting(self, handler: windows_foundation.TypedEventHandler[DispatcherQueue, DispatcherQueueShutdownStartingEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Microsoft.UI.Dispatching.DispatcherQueue::remove_FrameworkShutdownStarting(Windows.Foundation.EventRegistrationToken)
    def remove_framework_shutdown_starting(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # System.Boolean Microsoft.UI.Dispatching.DispatcherQueue::get_HasThreadAccess()
    @_property
    def has_thread_access(self) -> bool: ...

@typing.final
class DispatcherQueueController_Static(winrt._winrt.IInspectable_Static):
    # Microsoft.UI.Dispatching.DispatcherQueueController Microsoft.UI.Dispatching.DispatcherQueueController::CreateOnCurrentThread()
    def create_on_current_thread(cls) -> DispatcherQueueController: ...
    # Microsoft.UI.Dispatching.DispatcherQueueController Microsoft.UI.Dispatching.DispatcherQueueController::CreateOnDedicatedThread()
    def create_on_dedicated_thread(cls) -> DispatcherQueueController: ...

@typing.final
class DispatcherQueueController(winrt.system.Object, metaclass=DispatcherQueueController_Static):
    # System.Void Microsoft.UI.Dispatching.DispatcherQueueController::ShutdownQueue()
    def shutdown_queue(self) -> None: ...
    # Windows.Foundation.IAsyncAction Microsoft.UI.Dispatching.DispatcherQueueController::ShutdownQueueAsync()
    def shutdown_queue_async(self) -> windows_foundation.IAsyncAction: ...
    # Microsoft.UI.Dispatching.DispatcherQueue Microsoft.UI.Dispatching.DispatcherQueueController::get_DispatcherQueue()
    @_property
    def dispatcher_queue(self) -> DispatcherQueue: ...

@typing.final
class DispatcherQueueShutdownStartingEventArgs(winrt.system.Object):
    # Windows.Foundation.Deferral Microsoft.UI.Dispatching.DispatcherQueueShutdownStartingEventArgs::GetDeferral()
    def get_deferral(self) -> windows_foundation.Deferral: ...

@typing.final
class DispatcherQueueTimer(winrt.system.Object):
    # System.Void Microsoft.UI.Dispatching.DispatcherQueueTimer::Start()
    def start(self) -> None: ...
    # System.Void Microsoft.UI.Dispatching.DispatcherQueueTimer::Stop()
    def stop(self) -> None: ...
    # Windows.Foundation.EventRegistrationToken Microsoft.UI.Dispatching.DispatcherQueueTimer::add_Tick(Windows.Foundation.TypedEventHandler`2<Microsoft.UI.Dispatching.DispatcherQueueTimer,System.Object>)
    def add_tick(self, handler: windows_foundation.TypedEventHandler[DispatcherQueueTimer, winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Microsoft.UI.Dispatching.DispatcherQueueTimer::remove_Tick(Windows.Foundation.EventRegistrationToken)
    def remove_tick(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # System.Boolean Microsoft.UI.Dispatching.DispatcherQueueTimer::get_IsRepeating()
    @_property
    def is_repeating(self) -> bool: ...
    # System.Void Microsoft.UI.Dispatching.DispatcherQueueTimer::put_IsRepeating(System.Boolean)
    @is_repeating.setter
    def is_repeating(self, value: bool) -> None: ...
    # Windows.Foundation.TimeSpan Microsoft.UI.Dispatching.DispatcherQueueTimer::get_Interval()
    @_property
    def interval(self) -> datetime.timedelta: ...
    # System.Void Microsoft.UI.Dispatching.DispatcherQueueTimer::put_Interval(Windows.Foundation.TimeSpan)
    @interval.setter
    def interval(self, value: datetime.timedelta) -> None: ...
    # System.Boolean Microsoft.UI.Dispatching.DispatcherQueueTimer::get_IsRunning()
    @_property
    def is_running(self) -> bool: ...

