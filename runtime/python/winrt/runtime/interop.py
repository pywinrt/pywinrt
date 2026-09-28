from typing import NewType

from typing_extensions import CapsuleType

from winrt._winrt import (
    as_interface,
    hresult_error,
    initialize_with_window,
    wrap_interface,
)

__all__ = [
    "InterfaceCapsule",
    "as_interface",
    "hresult_error",
    "initialize_with_window",
    "wrap_interface",
]

# A capsule named "winrt.interface" that holds one reference to a COM interface,
# which its destructor releases. It is how a WinRT object crosses between the
# runtime and a compiled interop module, which share no C ABI.
InterfaceCapsule = NewType("InterfaceCapsule", CapsuleType)
