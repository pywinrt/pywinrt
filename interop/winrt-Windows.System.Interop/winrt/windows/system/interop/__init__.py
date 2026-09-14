from enum import IntEnum

from winrt._winrt_windows_system_interop import (
    create_dispatcher_queue_controller,
)

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
