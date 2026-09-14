from winrt.windows.system import DispatcherQueueController
from winrt.windows.system.interop import (
    DispatcherQueueThreadApartmentType,
    DispatcherQueueThreadType,
)

def create_dispatcher_queue_controller(
    thread_type: DispatcherQueueThreadType = DispatcherQueueThreadType.CURRENT,
    apartment_type: DispatcherQueueThreadApartmentType = DispatcherQueueThreadApartmentType.NONE,
) -> DispatcherQueueController: ...
