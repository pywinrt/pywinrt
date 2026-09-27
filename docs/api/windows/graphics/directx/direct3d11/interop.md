# `winrt.windows.graphics.directx.direct3d11.interop` module

APIs for desktop interop with the
[Windows.Graphics.DirectX.Direct3D11](https://learn.microsoft.com/uwp/api/windows.graphics.directx.direct3d11)
namespace.

## `create_direct3d11_device_from_dxgi_device`

```python
create_direct3d11_device_from_dxgi_device(dxgi_device)
```

Creates a WinRT Direct3D 11 device from a DXGI device.

| Parameter | Type | Description |
|---|---|---|
| `dxgi_device` | `int` | Pointer to an IDXGIDevice to create the Direct3D 11 device from. |

**Returns:** A Direct3D 11 device.

**Return type:** `winrt.windows.graphics.directx.direct3d11.Direct3D11Device`

**Raises:** [`OSError`][OSError] on failure.

!!! version-added "Added in version 2.3"

!!! seealso "See also"

    <https://learn.microsoft.com/en-us/windows/win32/api/windows.graphics.directx.direct3d11.interop/nf-windows-graphics-directx-direct3d11-interop-createdirect3d11devicefromdxgidevice>

## `create_direct3d11_surface_from_dxgi_surface`

```python
create_direct3d11_surface_from_dxgi_surface(dxgi_surface)
```

Creates a WinRT Direct3D 11 surface from a DXGI surface.

| Parameter | Type | Description |
|---|---|---|
| `dxgi_surface` | `int` | Pointer to an IDXGISurface to create the Direct3D 11 surface from. |

**Returns:** A Direct3D 11 surface.

**Return type:** `winrt.windows.graphics.directx.direct3d11.Direct3D11Surface`

**Raises:** [`OSError`][OSError] on failure.

!!! version-added "Added in version 2.3"

!!! seealso "See also"

    <https://learn.microsoft.com/en-us/windows/win32/api/windows.graphics.directx.direct3d11.interop/nf-windows-graphics-directx-direct3d11-interop-createdirect3d11surfacefromdxgisurface>

## `get_dxgi_device_from_object`

```python
get_dxgi_device_from_object(device)
```

Retrieves the IDXGIDevice from a Direct3D object.

| Parameter | Type | Description |
|---|---|---|
| `device` | `winrt.windows.graphics.directx.direct3d11.Direct3D11Device` | The WinRT object to retrieve the IDXGIDevice from. |

**Returns:** Pointer to IDXGIDevice interface of the WinRT object.

**Return type:** `int`

!!! version-added "Added in version 2.3"

!!! seealso "See also"

    <https://learn.microsoft.com/en-us/windows/win32/api/windows.graphics.directx.direct3d11.interop/nf-windows-graphics-directx-direct3d11-interop-getdxgiinterface>

## `get_dxgi_surface_from_object`

```python
get_dxgi_surface_from_object(surface)
```

Retrieves the IDXGISurface from a Direct3D object.

| Parameter | Type | Description |
|---|---|---|
| `surface` | `winrt.windows.graphics.directx.direct3d11.Direct3D11Surface` | The WinRT object to retrieve the IDXGISurface from. |

**Returns:** Pointer to IDXGISurface interface of the WinRT object.

**Return type:** `int`

!!! version-added "Added in version 2.3"

!!! seealso "See also"

    <https://learn.microsoft.com/en-us/windows/win32/api/windows.graphics.directx.direct3d11.interop/nf-windows-graphics-directx-direct3d11-interop-getdxgiinterface-r1>

[OSError]: https://docs.python.org/3/builtins/exceptions.html#OSError
