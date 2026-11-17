# `winrt.microsoft.ui.xaml.media.dxinterop` module

APIs for DirectX interop with the
[Microsoft.UI.Xaml.Media](https://learn.microsoft.com/windows/windows-app-sdk/api/winrt/microsoft.ui.xaml.media)
namespace of WinUI 3.

The module builds against the Windows SDK alone, so its version does not follow
the Windows App SDK's, and it works with any Windows App SDK release. A
`SwapChainPanel` still needs the Windows App Runtime, which
`winrt-Microsoft.WindowsAppSDK.WinUI` needs anyway.

## `SwapChainPanelNative`

```python
@final
class SwapChainPanelNative
```

The native interface of a WinUI 3
[`SwapChainPanel`](https://learn.microsoft.com/windows/windows-app-sdk/api/winrt/microsoft.ui.xaml.controls.swapchainpanel),
which gives it the DXGI swap chain it shows.

Call `as_()` on a panel to get one. It holds *ISwapChainPanelNative2* if the
panel supports it. Otherwise, it holds *ISwapChainPanelNative*, the base
interface of *ISwapChainPanelNative2*.
The class cannot be created or subclassed. It is equal to, and hashes like, any
WinRT object that is the same COM object.

A graphics library that presents to the panel itself takes the address of the
native interface, which
[`interface_pointer()`](../../../../runtime.interop.md#interface_pointer) gives:

```python
from winrt.microsoft.ui.xaml.controls import SwapChainPanel
from winrt.microsoft.ui.xaml.media.dxinterop import SwapChainPanelNative
from winrt.runtime.interop import interface_pointer

panel = SwapChainPanel()
native = panel.as_(SwapChainPanelNative)
pointer = interface_pointer(native)
```

Whichever of the two interfaces the instance holds, the pointer can be used as
an *ISwapChainPanelNative*, because *ISwapChainPanelNative2* inherits from it.
The pointer is valid only while `native` is alive, so a library that keeps it
must add a reference of its own.

!!! seealso "See also"

    <https://learn.microsoft.com/windows/windows-app-sdk/api/win32/microsoft.ui.xaml.media.dxinterop/nn-microsoft-ui-xaml-media-dxinterop-iswapchainpanelnative>

### `set_swap_chain`

```python
set_swap_chain(swap_chain: InterfaceCapsule | None, /) -> None
```

Sets the swap chain the panel shows.

| Parameter | Type | Description |
|---|---|---|
| `swap_chain` | [`InterfaceCapsule`](../../../../runtime.interop.md#interfacecapsule) | An interface pointer capsule holding an *IDXGISwapChain*, or `None` to remove the swap chain. |

**Raises:** [`OSError`][OSError] on failure.

### `set_swap_chain_handle`

```python
set_swap_chain_handle(handle: int, /) -> None
```

Sets the swap chain the panel shows by a composition surface handle, rather
than by the swap chain itself.

| Parameter | Type | Description |
|---|---|---|
| `handle` | `int` | A handle that *DCompositionCreateSurfaceHandle()* made, which the swap chain was created for with *IDXGIFactoryMedia::CreateSwapChainForCompositionSurfaceHandle()*. |

**Raises:** [`OSError`][OSError] on failure, and with `E_NOINTERFACE` if the
panel does not implement *ISwapChainPanelNative2*.

!!! seealso "See also"

    <https://learn.microsoft.com/windows/windows-app-sdk/api/win32/microsoft.ui.xaml.media.dxinterop/nn-microsoft-ui-xaml-media-dxinterop-iswapchainpanelnative2>

[OSError]: https://docs.python.org/3/builtins/exceptions.html#OSError
