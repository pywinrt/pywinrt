from enum import IntEnum

import winrt._winrt_windows_system_interop as _interop

__all__ = ["create_dispatcher_queue_controller"]

class DispatcherQueueThreadApartmentType(IntEnum):
    NONE = 0
    ASTA = 1
    STA = 2

class DispatcherQueueThreadType(IntEnum):
    DEDICATED = 1
    CURRENT = 2

create_dispatcher_queue_controller = _interop.create_dispatcher_queue_controller
