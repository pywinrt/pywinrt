from typing import Any, NoReturn, Self, final
from uuid import UUID

import winrt.runtime.interop as _runtime
import winrt._winrt_microsoft_ui_xaml_media_dxinterop as _native
from winrt.system import Object
from winrt.system.hresult import E_NOINTERFACE

__all__ = ["SwapChainPanelNative"]

_IID_IUNKNOWN = UUID("00000000-0000-0000-C000-000000000046")
_IID_ISWAPCHAINPANELNATIVE = UUID("63aad0b8-7c24-40ff-85a8-640d944cc325")
_IID_ISWAPCHAINPANELNATIVE2 = UUID("88fd8248-10da-4810-bb4c-010dd27faea9")


@final
class SwapChainPanelNative:
    """
    The native interface of a WinUI 3 SwapChainPanel, which
    ``panel.as_(SwapChainPanelNative)`` is the only way to get.

    It holds *ISwapChainPanelNative2* if the panel supports it. Otherwise, it
    holds *ISwapChainPanelNative*, the base interface of
    *ISwapChainPanelNative2*.
    """

    # Nothing but the interface, so that nothing can come to rely on an
    # instance dictionary.
    __slots__ = ("_native",)

    _native: _runtime.InterfaceCapsule

    def __new__(cls, *args: Any, **kwargs: Any) -> Self:
        raise TypeError(f"cannot create '{__name__}.{cls.__qualname__}' instances")

    def __init_subclass__(cls, **kwargs: Any) -> NoReturn:
        raise TypeError(
            f"type '{__name__}.SwapChainPanelNative' is not an acceptable base type"
        )

    @classmethod
    def _query_interface_(cls, obj: Object) -> Self:
        self = super().__new__(cls)

        try:
            self._native = _runtime.as_interface(obj, _IID_ISWAPCHAINPANELNATIVE2)
            return self
        except OSError as e:
            if e.winerror != E_NOINTERFACE:
                raise

        # outside the except block, so that a failure here is not shown as
        # happening while handling the first one
        self._native = _runtime.as_interface(obj, _IID_ISWAPCHAINPANELNATIVE)
        return self

    def _interface_capsule_(self) -> _runtime.InterfaceCapsule:
        return self._native

    def __eq__(self, other: object) -> bool:
        # Equal to anything that is the same COM object, as a WinRT object is.
        if isinstance(other, SwapChainPanelNative):
            other_native = other._native
        elif isinstance(other, Object):
            other_native = _runtime.as_interface(other, _IID_IUNKNOWN)
        else:
            return NotImplemented

        return _native.get_identity(self._native) == _native.get_identity(other_native)

    def __hash__(self) -> int:
        return hash(_native.get_identity(self._native))

    def set_swap_chain(self, swap_chain: _runtime.InterfaceCapsule | None, /) -> None:
        _native.set_swap_chain(self._native, swap_chain)

    def set_swap_chain_handle(self, handle: int, /) -> None:
        _native.set_swap_chain_handle(self._native, handle)
