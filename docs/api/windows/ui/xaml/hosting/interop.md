# `winrt.windows.ui.xaml.hosting.interop` module

APIs for desktop interop with the
[Windows.UI.Xaml.Hosting](https://learn.microsoft.com/uwp/api/windows.ui.xaml.hosting)
namespace, which hosts UWP XAML in a window of a desktop app (XAML Islands).

## `DesktopWindowXamlSourceNative`

```python
@final
class DesktopWindowXamlSourceNative
```

The native interfaces of a
[`DesktopWindowXamlSource`](https://learn.microsoft.com/uwp/api/windows.ui.xaml.hosting.desktopwindowxamlsource),
which attach it to a window and hand it the window's messages.

Call `as_()` on a source to get one. The class cannot be created or subclassed.
It is equal to, and hashes like, any WinRT object that is the same COM object.

```python
from winrt.windows.ui.xaml.hosting import DesktopWindowXamlSource
from winrt.windows.ui.xaml.hosting.interop import DesktopWindowXamlSourceNative

with DesktopWindowXamlSource() as source:
    native = source.as_(DesktopWindowXamlSourceNative)
    native.attach_to_window(hwnd)
    ...
```

!!! seealso "See also"

    <https://learn.microsoft.com/windows/win32/api/windows.ui.xaml.hosting.desktopwindowxamlsource/nn-windows-ui-xaml-hosting-desktopwindowxamlsource-idesktopwindowxamlsourcenative>

### `attach_to_window`

```python
attach_to_window(hwnd: int, /) -> None
```

Attaches the XAML content to a window of the app, as a child window of it.

| Parameter | Type | Description |
|---|---|---|
| `hwnd` | `int` | The handle (`HWND`) of the window to attach to. |

**Raises:** [`OSError`][OSError] on failure.

### `window_handle`

```python
window_handle: int
```

The handle (`HWND`) of the child window that holds the XAML content, which the
app sizes and positions within its own window. It is `0` before
[`attach_to_window()`](#attach_to_window).

### `pretranslate_message`

```python
pretranslate_message(msg: Buffer, /) -> bool
```

Gives the XAML content the first look at a message of the app's message loop,
which is what makes keyboard navigation and accelerators work inside it.

| Parameter | Type | Description |
|---|---|---|
| `msg` | `Buffer` | A Win32 `MSG`, such as a [`ctypes.wintypes.MSG`](https://docs.python.org/3/library/ctypes.html#module-ctypes.wintypes). |

**Returns:** `True` if the XAML content handled the message, in which case the
app does not translate or dispatch it.

**Raises:**

- [`BufferError`][BufferError] if `msg` is not the size of a `MSG`.
- [`OSError`][OSError] on failure.

```python
msg = wintypes.MSG()

while user32.GetMessageW(ctypes.byref(msg), None, 0, 0) > 0:
    if native.pretranslate_message(msg):
        continue

    user32.TranslateMessage(ctypes.byref(msg))
    user32.DispatchMessageW(ctypes.byref(msg))
```

!!! version-added "Added in version 3.1"

[OSError]: https://docs.python.org/3/builtins/exceptions.html#OSError
[BufferError]: https://docs.python.org/3/builtins/exceptions.html#BufferError
