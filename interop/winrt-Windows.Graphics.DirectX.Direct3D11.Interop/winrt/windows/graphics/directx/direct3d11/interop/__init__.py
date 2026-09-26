from uuid import UUID

import winrt._winrt as _runtime
import winrt._winrt_windows_graphics_directx_direct3d11_interop as _native
from winrt.system import Object
from winrt.windows.graphics.directx.direct3d11 import IDirect3DDevice, IDirect3DSurface

__all__ = [
    "create_direct3d11_device_from_dxgi_device",
    "create_direct3d11_surface_from_dxgi_surface",
    "get_dxgi_device_from_object",
    "get_dxgi_surface_from_object",
]

_IID_IINSPECTABLE = UUID("AF86E2E0-B12D-4C6A-9C5A-D7AA65101E90")


def create_direct3d11_device_from_dxgi_device(dxgi_device: int) -> IDirect3DDevice:
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
        "winrt.windows.graphics.directx.direct3d11.IDirect3DDevice",
    )


def create_direct3d11_surface_from_dxgi_surface(dxgi_surface: int) -> IDirect3DSurface:
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
        "winrt.windows.graphics.directx.direct3d11.IDirect3DSurface",
    )


def get_dxgi_device_from_object(object: Object) -> int:
    """
    Retrieves the IDXGIDevice from a Direct3D object.

    Args:
        object: The WinRT object to retrieve the IDXGIDevice from.

    Returns:
        Pointer to IDXGIDevice interface of the WinRT object.
    """
    return _native.get_dxgi_device_from_object(
        _runtime.as_interface(object, _IID_IINSPECTABLE)
    )


def get_dxgi_surface_from_object(object: Object) -> int:
    """
    Retrieves the IDXGISurface from a Direct3D object.

    Args:
        object: The WinRT object to retrieve the IDXGISurface from.

    Returns:
        Pointer to IDXGISurface interface of the WinRT object.
    """
    return _native.get_dxgi_surface_from_object(
        _runtime.as_interface(object, _IID_IINSPECTABLE)
    )
