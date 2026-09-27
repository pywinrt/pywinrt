# `winrt.runtime.interop` module

Various functions for interoperating with Win32 and COM.

## `initialize_with_window`

```python
initialize_with_window(obj: Object, hwnd: int) -> None
```

Provide an owner window to a Windows Runtime (WinRT) object used in a desktop application.

| Parameter | Type | Description |
|---|---|---|
| `obj` | [`Object`](system.md#object) | A WinRT object that implements *IInitializeWithWindow* interface. |
| `hwnd` | `int` | The handle of the window to be used as the owner window. |

!!! version-added "Added in version 3.0"

!!! seealso "See also"

    [Microsoft docs](https://learn.microsoft.com/en-us/windows/win32/api/shobjidl_core/nf-shobjidl_core-iinitializewithwindow-initialize)
