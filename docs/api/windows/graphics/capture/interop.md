# `winrt.windows.graphics.capture.interop` module

APIs for desktop interop with the
[Windows.Graphics.Capture](https://learn.microsoft.com/uwp/api/windows.graphics.capture) namespace.

## `create_for_monitor`

```python
create_for_monitor(hmonitor, /)
```

Targets a monitor(s) for the creation of a graphics capture item.

| Parameter | Type | Description |
|---|---|---|
| `hmonitor` | `int` | The monitor handle (`HMONITOR`) that represents the monitor to capture. |

**Returns:** A graphics capture item.

**Return type:** `winrt.windows.graphics.capture.GraphicsCaptureItem`

!!! seealso "See also"

    <https://learn.microsoft.com/windows/win32/api/windows.graphics.capture.interop/nf-windows-graphics-capture-interop-igraphicscaptureiteminterop-createformonitor>

## `create_for_window`

```python
create_for_window(hwnd, /)
```

Targets a single window for the creation of a graphics capture item.

| Parameter | Type | Description |
|---|---|---|
| `hwnd` | `int` | The window handle (`HWND`) that represents the window to capture. |

**Returns:** A graphics capture item.

**Return type:** `winrt.windows.graphics.capture.GraphicsCaptureItem`

!!! seealso "See also"

    <https://learn.microsoft.com/windows/win32/api/windows.graphics.capture.interop/nf-windows-graphics-capture-interop-igraphicscaptureiteminterop-createforwindow>
