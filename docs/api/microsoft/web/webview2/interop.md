# `winrt.microsoft.web.webview2.interop` module

What a window that shows a WebView2 through visual hosting needs from the COM
interop of `CoreWebView2CompositionController`, in the
`winrt-Microsoft.Web.WebView2.Interop` package: an OLE drop target for files
dragged onto the window. A WebView2 in a window of its own, from
`create_core_webview2_controller_async()`, does not need it.

See: <https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/winrt/interop/>

## `register_drop_target`

```python
register_drop_target(controller: CoreWebView2CompositionController, hwnd: int, /) -> DropTarget
```

Registers the window as an OLE drop target that hands what is dragged over it
to the WebView, as `RegisterDragDrop()` does. The WebView is taken to be drawn
from the top left corner of the window's client area, which is where the
points it is given start from.

It is called on the thread that owns the window, and initializes OLE on that
thread until the registration is revoked. That thread has to be a
single-threaded apartment, or not yet initialized for COM, which OLE makes it
one. The WebView takes a drop only while the controller's
`allow_external_drop` is `True`.

```python
controller.allow_external_drop = True
drop_target = register_drop_target(controller, hwnd)
...
# on WM_DESTROY
drop_target.close()
```

| Parameter | Type | Description |
|---|---|---|
| `controller` | `winrt.microsoft.web.webview2.core.CoreWebView2CompositionController` | The controller of the WebView. |
| `hwnd` | `int` | The window that shows the WebView. |

**Returns:** The registration, which holds the controller until it is
closed.

**Raises:**

- [`ValueError`][ValueError] if `hwnd` is not a window of the calling thread.
- [`OSError`][OSError] if OLE cannot be initialized on the thread, or the
  window already has a drop target.

## `DropTarget`

A window registered by [`register_drop_target()`](#register_drop_target). It
is not created directly.

### `close`

```python
close() -> None
```

Revokes the registration, as `RevokeDragDrop()` does. It is called on the
thread that owns the window, before the window is destroyed. Calling it again
does nothing. A `DropTarget` is also a context manager that closes it.

**Raises:** [`ValueError`][ValueError] if the window is not a window of the
calling thread, which includes a window that was already destroyed.

[OSError]: https://docs.python.org/3/library/exceptions.html#OSError
[ValueError]: https://docs.python.org/3/library/exceptions.html#ValueError
