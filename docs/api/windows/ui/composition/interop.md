# `winrt.windows.ui.composition.interop` module

APIs for desktop interop with the
[Windows.UI.Composition](https://learn.microsoft.com/uwp/api/windows.ui.composition) namespace.

## `create_desktop_window_target`

```python
create_desktop_window_target(compositor, hwnd_target, *, is_topmost=False)
```

Creates a visual layer on a window to host Composition content.

| Parameter | Type | Description |
|---|---|---|
| `compositor` | `winrt.windows.ui.composition.Compositor` | The Compositor object that is used to create the `DesktopWindowTarget`. |
| `hwnd_target` | `int` | The window handle (HWND) that the visual layer will be attached to. |
| `is_topmost` | `bool` | |

**Returns:** The created `DesktopWindowTarget`.

**Return type:** `winrt.windows.ui.composition.DesktopWindowTarget`

**Raises:** [`OSError`](https://docs.python.org/3/builtins/exceptions.html#OSError) on failure

!!! version-added "Added in version 2.2"
