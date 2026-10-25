from enum import IntEnum, IntFlag
from uuid import UUID

import winrt.runtime.interop as _runtime
import winrt._winrt_windows_graphics_directx_direct3d11_interop as _native
from winrt.system import Object
from winrt.windows.graphics.directx.direct3d11 import IDirect3DDevice, IDirect3DSurface

__all__ = [
    "create_direct3d11_device",
    "create_direct3d11_device_from_dxgi_device",
    "create_direct3d11_surface_from_dxgi_surface",
    "get_dxgi_device_from_object",
    "get_dxgi_surface_from_object",
    "D3dDriverType",
    "D3d11CreateDeviceFlag",
]

_IID_IINSPECTABLE = UUID("AF86E2E0-B12D-4C6A-9C5A-D7AA65101E90")


# https://learn.microsoft.com/en-us/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_driver_type
# UNKNOWN and SOFTWARE are left out, since they need an adapter or a software
# rasterizer that create_direct3d11_device() does not take.
class D3dDriverType(IntEnum):
    HARDWARE = 1
    REFERENCE = 2
    NULL = 3
    WARP = 5


# https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_create_device_flag
# SWITCH_TO_REF is left out, since Direct3D 11 does not support it.
class D3d11CreateDeviceFlag(IntFlag):
    SINGLE_THREADED = 0x1
    DEBUG = 0x2
    PREVENT_INTERNAL_THREADING_OPTIMIZATIONS = 0x8
    BGRA_SUPPORT = 0x20
    DEBUGGABLE = 0x40
    PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY = 0x80
    DISABLE_GPU_TIMEOUT = 0x100
    VIDEO_SUPPORT = 0x800


def create_direct3d11_device(
    *,
    driver_type: D3dDriverType = D3dDriverType.HARDWARE,
    flags: D3d11CreateDeviceFlag = D3d11CreateDeviceFlag.BGRA_SUPPORT,
) -> IDirect3DDevice:
    """
    Creates a Direct3D 11 device on the default adapter and wraps it in an
    IDirect3DDevice.

    Args:
        driver_type: The driver to create the device with.
        flags: The flags to create the device with. Windows.Graphics.Capture
            needs BGRA_SUPPORT, which the default includes.

    Returns:
        A Direct3DDevice instance that wraps the new device.
    """
    return _runtime.wrap_interface(
        _native.create_direct3d11_device(driver_type, flags),
        IDirect3DDevice,
    )


def create_direct3d11_device_from_dxgi_device(dxgi_device: int, /) -> IDirect3DDevice:
    """
    Creates an instance of IDirect3DDevice from an IDXGIDevice.

    Args:
        dxgi_device (IDXGIDevice*): The IDXGIDevice to create the Direct3DDevice
            from.

    Returns:
        A Direct3DDevice instance that wraps the DXGIDevice.
    """
    return _runtime.wrap_interface(
        _native.create_direct3d11_device_from_dxgi_device(dxgi_device),
        IDirect3DDevice,
    )


def create_direct3d11_surface_from_dxgi_surface(
    dxgi_surface: int, /
) -> IDirect3DSurface:
    """
    Creates an instance of IDirect3DSurface from an IDXGISurface.

    Args:
        dxgi_surface (IDXGISurface*): The IDXGISurface to create the
            Direct3DSurface from.

    Returns:
        A Direct3DSurface instance that wraps the DXGISurface.
    """
    return _runtime.wrap_interface(
        _native.create_direct3d11_surface_from_dxgi_surface(dxgi_surface),
        IDirect3DSurface,
    )


def get_dxgi_device_from_object(device: Object, /) -> int:
    """
    Retrieves the IDXGIDevice from a Direct3D object.

    Args:
        device: The WinRT object to retrieve the IDXGIDevice from.

    Returns:
        Pointer to IDXGIDevice interface of the WinRT object.
    """
    return _native.get_dxgi_device_from_object(
        _runtime.as_interface(device, _IID_IINSPECTABLE)
    )


def get_dxgi_surface_from_object(surface: Object, /) -> int:
    """
    Retrieves the IDXGISurface from a Direct3D object.

    Args:
        surface: The WinRT object to retrieve the IDXGISurface from.

    Returns:
        Pointer to IDXGISurface interface of the WinRT object.
    """
    return _native.get_dxgi_surface_from_object(
        _runtime.as_interface(surface, _IID_IINSPECTABLE)
    )
