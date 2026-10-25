# `winrt.windows.graphics.directx.direct3d11.interop` module

APIs for desktop interop with the
[Windows.Graphics.DirectX.Direct3D11](https://learn.microsoft.com/uwp/api/windows.graphics.directx.direct3d11)
namespace.

## `create_direct3d11_device`

```python
create_direct3d11_device(*, driver_type=D3dDriverType.HARDWARE, flags=D3d11CreateDeviceFlag.BGRA_SUPPORT)
```

Creates a Direct3D 11 device on the default adapter, as the WinRT Direct3D 11
device that APIs such as
[Windows.Graphics.Capture](https://learn.microsoft.com/uwp/api/windows.graphics.capture)
take.

| Parameter | Type | Description |
|---|---|---|
| `driver_type` | [`D3dDriverType`](#d3ddrivertype) | The driver to create the device with. |
| `flags` | [`D3d11CreateDeviceFlag`](#d3d11createdeviceflag) | The flags to create the device with. Windows.Graphics.Capture needs `BGRA_SUPPORT`. |

**Returns:** A Direct3D 11 device.

**Return type:** `winrt.windows.graphics.directx.direct3d11.IDirect3DDevice`

**Raises:**

- [`ValueError`][ValueError] if `driver_type` is not a `D3D_DRIVER_TYPE` value
  or `flags` is negative.
- [`OverflowError`][OverflowError] if `flags` does not fit in 32 bits.
- [`OSError`][OSError] if the device cannot be created.

!!! version-added "Added in version 4.1"

!!! seealso "See also"

    <https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-d3d11createdevice>

## `D3dDriverType`

```python
class D3dDriverType(enum.IntEnum)
```

The driver that [`create_direct3d11_device`](#create_direct3d11_device) creates
the device with: `HARDWARE`, `REFERENCE`, `NULL` or `WARP`. `UNKNOWN` and
`SOFTWARE` are left out, since they need an adapter or a software rasterizer
that the function does not take. `REFERENCE` needs the reference rasterizer,
`d3d11ref.dll`, which only the Windows SDK installs.

!!! version-added "Added in version 4.1"

!!! seealso "See also"

    <https://learn.microsoft.com/en-us/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_driver_type>

## `D3d11CreateDeviceFlag`

```python
class D3d11CreateDeviceFlag(enum.IntFlag)
```

The flags that [`create_direct3d11_device`](#create_direct3d11_device) creates
the device with: `SINGLE_THREADED`, `DEBUG`,
`PREVENT_INTERNAL_THREADING_OPTIMIZATIONS`, `BGRA_SUPPORT`, `DEBUGGABLE`,
`PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY`, `DISABLE_GPU_TIMEOUT` and
`VIDEO_SUPPORT`. `SWITCH_TO_REF` is left out, since Direct3D 11 does not
support it. `DEBUG` needs the debug layer, which the Graphics Tools optional
feature of Windows installs.

!!! version-added "Added in version 4.1"

!!! seealso "See also"

    <https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_create_device_flag>

## `create_direct3d11_device_from_dxgi_device`

```python
create_direct3d11_device_from_dxgi_device(dxgi_device, /)
```

Creates a WinRT Direct3D 11 device from a DXGI device.

| Parameter | Type | Description |
|---|---|---|
| `dxgi_device` | `int` | Pointer to an IDXGIDevice to create the Direct3D 11 device from. |

**Returns:** A Direct3D 11 device.

**Return type:** `winrt.windows.graphics.directx.direct3d11.IDirect3DDevice`

**Raises:** [`OSError`][OSError] on failure.

!!! version-added "Added in version 2.3"

!!! seealso "See also"

    <https://learn.microsoft.com/en-us/windows/win32/api/windows.graphics.directx.direct3d11.interop/nf-windows-graphics-directx-direct3d11-interop-createdirect3d11devicefromdxgidevice>

## `create_direct3d11_surface_from_dxgi_surface`

```python
create_direct3d11_surface_from_dxgi_surface(dxgi_surface, /)
```

Creates a WinRT Direct3D 11 surface from a DXGI surface.

| Parameter | Type | Description |
|---|---|---|
| `dxgi_surface` | `int` | Pointer to an IDXGISurface to create the Direct3D 11 surface from. |

**Returns:** A Direct3D 11 surface.

**Return type:** `winrt.windows.graphics.directx.direct3d11.IDirect3DSurface`

**Raises:** [`OSError`][OSError] on failure.

!!! version-added "Added in version 2.3"

!!! seealso "See also"

    <https://learn.microsoft.com/en-us/windows/win32/api/windows.graphics.directx.direct3d11.interop/nf-windows-graphics-directx-direct3d11-interop-createdirect3d11surfacefromdxgisurface>

## `get_dxgi_device_from_object`

```python
get_dxgi_device_from_object(device, /)
```

Retrieves the IDXGIDevice from a Direct3D object.

| Parameter | Type | Description |
|---|---|---|
| `device` | `winrt.windows.graphics.directx.direct3d11.IDirect3DDevice` | The WinRT object to retrieve the IDXGIDevice from. |

**Returns:** Pointer to IDXGIDevice interface of the WinRT object.

**Return type:** `int`

!!! version-added "Added in version 2.3"

!!! seealso "See also"

    <https://learn.microsoft.com/en-us/windows/win32/api/windows.graphics.directx.direct3d11.interop/nf-windows-graphics-directx-direct3d11-interop-getdxgiinterface>

## `get_dxgi_surface_from_object`

```python
get_dxgi_surface_from_object(surface, /)
```

Retrieves the IDXGISurface from a Direct3D object.

| Parameter | Type | Description |
|---|---|---|
| `surface` | `winrt.windows.graphics.directx.direct3d11.IDirect3DSurface` | The WinRT object to retrieve the IDXGISurface from. |

**Returns:** Pointer to IDXGISurface interface of the WinRT object.

**Return type:** `int`

!!! version-added "Added in version 2.3"

!!! seealso "See also"

    <https://learn.microsoft.com/en-us/windows/win32/api/windows.graphics.directx.direct3d11.interop/nf-windows-graphics-directx-direct3d11-interop-getdxgiinterface-r1>

[OSError]: https://docs.python.org/3/builtins/exceptions.html#OSError
[OverflowError]: https://docs.python.org/3/builtins/exceptions.html#OverflowError
[ValueError]: https://docs.python.org/3/builtins/exceptions.html#ValueError
