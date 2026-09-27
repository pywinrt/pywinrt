# `winrt.windows.media.interop` module

APIs for desktop interop with the
[Windows.Media](https://learn.microsoft.com/uwp/api/windows.media) namespace.

## `get_for_window`

```python
get_for_window(hwnd)
```

Targets a single window for the creation of a graphics capture item.

| Parameter | Type | Description |
|---|---|---|
| `hwnd` | `int` | The top-level app window (`HWND`) for which the `ISystemMediaTransportControls` interface is retrieved. |

**Returns:** Receives the `ISystemMediaTransportControls` that corresponds to the appWindow window.

**Return type:** `winrt.windows.media.SystemMediaTransportControls`

!!! version-added "Added in version 3.1"

!!! seealso "See also"

    <https://learn.microsoft.com/en-us/windows/win32/api/systemmediatransportcontrolsinterop/nf-systemmediatransportcontrolsinterop-isystemmediatransportcontrolsinterop-getforwindow>
