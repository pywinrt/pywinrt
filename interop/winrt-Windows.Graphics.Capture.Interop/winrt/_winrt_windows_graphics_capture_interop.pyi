from winrt.runtime.interop import InterfaceCapsule

def create_for_monitor(
    factory: InterfaceCapsule, monitor: int, /
) -> InterfaceCapsule: ...
def create_for_window(
    factory: InterfaceCapsule, window: int, /
) -> InterfaceCapsule: ...
