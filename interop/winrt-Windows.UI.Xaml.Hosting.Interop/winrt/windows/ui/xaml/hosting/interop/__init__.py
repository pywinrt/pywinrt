from typing import Any, NoReturn, Self, final
from uuid import UUID

from typing_extensions import Buffer, CapsuleType

import winrt._winrt as _runtime
import winrt._winrt_windows_ui_xaml_hosting_interop as _native
from winrt.system import Object

__all__ = ["DesktopWindowXamlSourceNative"]

_IID_IUNKNOWN = UUID("00000000-0000-0000-C000-000000000046")
_IID_IDESKTOPWINDOWXAMLSOURCENATIVE = UUID("3CBCF1BF-2F76-4E9C-96AB-E84B37972554")


@final
class DesktopWindowXamlSourceNative:
    """
    The native interfaces of a DesktopWindowXamlSource, which
    ``source.as_(DesktopWindowXamlSourceNative)`` is the only way to get.
    """

    # Nothing but the interface, so that nothing can come to rely on an
    # instance dictionary.
    __slots__ = ("_native",)

    _native: CapsuleType

    def __new__(cls, *args: Any, **kwargs: Any) -> Self:
        raise TypeError(f"cannot create '{__name__}.{cls.__qualname__}' instances")

    def __init_subclass__(cls, **kwargs: Any) -> NoReturn:
        raise TypeError(
            f"type '{__name__}.DesktopWindowXamlSourceNative' is not an acceptable base type"
        )

    @classmethod
    def _from(cls, obj: Object) -> Self:
        self = super().__new__(cls)
        self._native = _runtime.as_interface(obj, _IID_IDESKTOPWINDOWXAMLSOURCENATIVE)
        return self

    def __eq__(self, other: object) -> bool:
        # Equal to anything that is the same COM object, as a WinRT object is.
        if isinstance(other, DesktopWindowXamlSourceNative):
            other_native = other._native
        elif isinstance(other, Object):
            other_native = _runtime.as_interface(other, _IID_IUNKNOWN)
        else:
            return NotImplemented

        return _native.get_identity(self._native) == _native.get_identity(other_native)

    def __hash__(self) -> int:
        return hash(_native.get_identity(self._native))

    def attach_to_window(self, hwnd: int) -> None:
        _native.attach_to_window(self._native, hwnd)

    def pretranslate_message(self, msg: Buffer) -> bool:
        return _native.pretranslate_message(self._native, msg)

    @property
    def window_handle(self) -> int:
        return _native.get_window_handle(self._native)
