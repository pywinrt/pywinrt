# `winrt.microsoft.ui.interop` module

Exposes interop functions from the `Microsoft.UI.Interop` namespace.

See: <https://learn.microsoft.com/en-us/windows/windows-app-sdk/api/win32/winrt-microsoft.ui.interop/>

!!! version-changed "Changed in version 3.0"

    The top-level package name has been changed from `winrt` to `winui3`.

!!! version-changed "Changed in version 4.0"

    The top-level package name has been changed back from `winui3` to
    `winrt`.

## `get_window_id_from_window`

```python
get_window_id_from_window(int hwnd) -> winrt.microsoft.ui.WindowId
```

Gets the WindowId that corresponds to the specified hwnd, if the provided HWND is valid.

| Parameter | Type | Description |
|---|---|---|
| `version` | | The handle of the window for which to get the WindowId. |

**Returns:** The identifier that corresponds to the specified hwnd.

**Raises:** `OSError` – if the the OS call failed.

!!! version-added "Added in version 2.1"

## `get_window_from_window_id`

```python
get_window_from_window_id(winrt.microsoft.ui.WindowId windowId) -> int
```

Gets the window handle that corresponds to the specified windowId, if the
provided windowId is valid and the system has an HWND that represents the
window.

| Parameter | Type | Description |
|---|---|---|
| `windowId` | `winrt.microsoft.ui.WindowId` | The identifier for the window for which to get the HWND. |

**Returns:** The handle of the window that corresponds to the specified windowId.

**Raises:** `OSError` – if the the OS call failed.

!!! version-added "Added in version 2.1"

## `get_display_id_from_monitor`

```python
get_display_id_from_monitor(int hmonitor) -> winrt.microsoft.ui.DisplayId
```

Gets the DisplayId that corresponds to the specified hmonitor, if the
provided HMONITOR is valid.

| Parameter | Type | Description |
|---|---|---|
| `hmonitor` | `int` | The handle of the display monitor for which to get the DisplayId. |

**Returns:** The identifier that corresponds to the specified hmonitor.

**Raises:** `OSError` – if the the OS call failed.

!!! version-added "Added in version 2.1"

## `get_monitor_from_display_id`

```python
get_monitor_from_display_id(winrt.microsoft.ui.DisplayId displayId) -> int
```

Gets the display monitor handle that corresponds to the specified displayId,
if the provided displayId is valid and the system has an HMONITOR that
represents the display monitor.

| Parameter | Type | Description |
|---|---|---|
| `displayId` | `winrt.microsoft.ui.DisplayId` | The identifier for the display monitor for which to get the HMONITOR. |

**Returns:** The handle of the display monitor that corresponds to the specified
displayId.

**Raises:** `OSError` – if the the OS call failed.

!!! version-added "Added in version 2.1"

## `get_icon_id_from_icon`

```python
get_icon_id_from_icon(int hicon) -> winrt.microsoft.ui.IconId
```

Gets the IconId that corresponds to the specified hicon, if the provided
HICON is valid.

| Parameter | Type | Description |
|---|---|---|
| `hicon` | `int` | The handle of the icon for which to get the IconId. |

**Returns:** The identifier that corresponds to the specified hicon.

**Raises:** `OSError` – if the the OS call failed.

!!! version-added "Added in version 2.1"

## `get_icon_from_icon_id`

```python
get_icon_from_icon_id(winrt.microsoft.ui.IconId iconId) -> int
```

Gets the icon handle that corresponds to the specified iconId, if the
provided iconId is valid and the system has an HICON that represents the
icon.

| Parameter | Type | Description |
|---|---|---|
| `iconId` | `winrt.microsoft.ui.IconId` | The identifier for the icon for which to get the HICON. |

**Returns:** The handle of the icon that corresponds to the specified iconId.

**Raises:** `OSError` – if the the OS call failed.

!!! version-added "Added in version 2.1"
