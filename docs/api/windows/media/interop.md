# `winrt.windows.media.interop` module

APIs for desktop interop with the
[Windows.Media](https://learn.microsoft.com/uwp/api/windows.media) namespace.

## `get_for_window`

```python
get_for_window(hwnd, /)
```

Gets the system media transport controls of a window.

| Parameter | Type | Description |
|---|---|---|
| `hwnd` | `int` | The handle (`HWND`) of the top-level app window to get the controls of. |

**Returns:** The system media transport controls of the window.

**Return type:** `winrt.windows.media.SystemMediaTransportControls`

!!! version-added "Added in version 3.1"

!!! seealso "See also"

    <https://learn.microsoft.com/en-us/windows/win32/api/systemmediatransportcontrolsinterop/nf-systemmediatransportcontrolsinterop-isystemmediatransportcontrolsinterop-getforwindow>
