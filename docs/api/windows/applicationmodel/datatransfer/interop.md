# `winrt.windows.applicationmodel.datatransfer.interop` module

APIs for desktop interop with the
[Windows.ApplicationModel.DataTransfer](https://learn.microsoft.com/uwp/api/windows.applicationmodel.datatransfer)
namespace: sharing from a desktop app, which has no view for
`DataTransferManager.get_for_current_view()` to use.

```python
from winrt.windows.applicationmodel.datatransfer.interop import (
    get_for_window,
    show_share_ui_for_window,
)

manager = get_for_window(hwnd)
manager.add_data_requested(on_data_requested)  # fills in what to share
show_share_ui_for_window(hwnd)
```

## `get_for_window`

```python
get_for_window(hwnd, /)
```

Gets the `DataTransferManager` of a window. Its `data_requested` event asks for
what to share when the share sheet is shown for the window.

| Parameter | Type | Description |
|---|---|---|
| `hwnd` | `int` | The handle (`HWND`) of the top-level app window. |

**Returns:** The `DataTransferManager` of the window.

**Return type:** `winrt.windows.applicationmodel.datatransfer.DataTransferManager`

!!! version-added "Added in version 4.1"

!!! seealso "See also"

    <https://learn.microsoft.com/en-us/windows/win32/api/shobjidl_core/nf-shobjidl_core-idatatransfermanagerinterop-getforwindow>

## `show_share_ui_for_window`

```python
show_share_ui_for_window(hwnd, /)
```

Shows the share sheet for a window. The window's `DataTransferManager` raises
`data_requested` to fill it. A handler that has to await something, such as
getting a `StorageFile`, takes a deferral from the request with
`get_deferral()` and completes it once it's done.

| Parameter | Type | Description |
|---|---|---|
| `hwnd` | `int` | The handle (`HWND`) of the top-level app window to show the share sheet for. |

!!! version-added "Added in version 4.1"

!!! seealso "See also"

    <https://learn.microsoft.com/en-us/windows/win32/api/shobjidl_core/nf-shobjidl_core-idatatransfermanagerinterop-showshareuiforwindow>
