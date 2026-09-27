# `winrt.windows.graphics.capture.interop` module

APIs for desktop interop with the
[Windows.Graphics.Capture](https://learn.microsoft.com/uwp/api/windows.graphics.capture) namespace.

## `create_for_monitor`

```python
create_for_monitor(monitor)
```

Targets a monitor(s) for the creation of a graphics capture item.

| Parameter | Type | Description |
|---|---|---|
| `monitor` | `int` | The monitor handle (`HMONITOR`) that represents the monitor to capture. |

**Returns:** A graphics capture item.

**Return type:** `winrt.windows.graphics.capture.GraphicsCaptureItem`

!!! seealso "See also"

    <https://learn.microsoft.com/windows/win32/api/windows.graphics.capture.interop/nf-windows-graphics-capture-interop-igraphicscaptureiteminterop-createformonitor>

## `create_for_window`

```python
create_for_window(window)
```

Targets a single window for the creation of a graphics capture item.

| Parameter | Type | Description |
|---|---|---|
| `window` | `int` | The window handle (`HWND`) that represents the window to capture. |

**Returns:** A graphics capture item.

**Return type:** `winrt.windows.graphics.capture.GraphicsCaptureItem`

!!! seealso "See also"

    <https://learn.microsoft.com/windows/win32/api/windows.graphics.capture.interop/nf-windows-graphics-capture-interop-igraphicscaptureiteminterop-createforwindow>
