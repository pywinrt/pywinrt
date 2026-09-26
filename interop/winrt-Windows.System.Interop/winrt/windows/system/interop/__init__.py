from enum import IntEnum

import winrt._winrt as _runtime
import winrt._winrt_windows_system_interop as _native
from winrt.windows.system import DispatcherQueueController

__all__ = [
    "create_dispatcher_queue_controller",
    "DispatcherQueueThreadApartmentType",
    "DispatcherQueueThreadType",
]


class DispatcherQueueThreadApartmentType(IntEnum):
    NONE = 0
    ASTA = 1
    STA = 2


class DispatcherQueueThreadType(IntEnum):
    DEDICATED = 1
    CURRENT = 2


def create_dispatcher_queue_controller(
    thread_type: DispatcherQueueThreadType = DispatcherQueueThreadType.CURRENT,
    apartment_type: DispatcherQueueThreadApartmentType = DispatcherQueueThreadApartmentType.NONE,
) -> DispatcherQueueController:
    return _runtime.wrap_interface(
        _native.create_dispatcher_queue_controller(thread_type, apartment_type),
        "winrt.windows.system.DispatcherQueueController",
    )
