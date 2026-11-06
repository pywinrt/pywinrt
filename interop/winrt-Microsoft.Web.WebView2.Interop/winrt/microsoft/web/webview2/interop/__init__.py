import typing as _typing

import winrt.runtime.interop as _runtime
import winrt._winrt_microsoft_web_webview2_interop as _native
import winrt.microsoft.web.webview2.core as _core

__all__ = ["DropTarget", "register_drop_target"]


@_typing.final
class DropTarget:
    """
    A window registered as the OLE drop target of a visually hosted WebView2,
    which register_drop_target() returns.
    """

    __slots__ = ("_hwnd",)

    _hwnd: int | None

    def __init__(self) -> None:
        raise TypeError("DropTarget is made by register_drop_target()")

    def close(self) -> None:
        """
        Revokes the registration, on the thread that owns the window and before
        the window is destroyed. Calling it again does nothing.
        """
        if self._hwnd is None:
            return

        _native.revoke_drop_target(self._hwnd)
        self._hwnd = None

    def __enter__(self) -> _typing.Self:
        return self

    def __exit__(self, *args: object) -> None:
        self.close()


def register_drop_target(
    controller: _core.CoreWebView2CompositionController, hwnd: int, /
) -> DropTarget:
    """
    Registers the window hwnd as an OLE drop target that hands what is dragged
    over it to the WebView that controller controls, which is drawn from the
    top left corner of the window's client area.

    It has to be called on the thread that owns the window, which it
    initializes OLE on. The WebView takes a drop only while the controller's
    allow_external_drop is true.
    """
    _native.register_drop_target(
        _runtime.as_interface(controller, _core.CoreWebView2CompositionController),
        hwnd,
    )

    target = object.__new__(DropTarget)
    target._hwnd = hwnd  # pyright: ignore[reportPrivateUsage]

    return target
