from winrt.windows.graphics.directx.direct3d11 import IDirect3DDevice
from winrt.system import Object

def create_direct3d11_device_from_dxgi_device(dxgi_device: int) -> IDirect3DDevice:
    """
    Creates an instance of IDirect3DDevice from an IDXGIDevice.

    Args:
        dxgi_device (IDXGIDevice*): The IDXGIDevice to create the Direct3DDevice from.

    Returns:
        A Direct3DDevice instance that wraps the DXGIDevice.
    """

def create_direct3d11_surface_from_dxgi_surface(dxgi_surface: int) -> int:
    """
    Creates an instance of IDirect3DSurface from an IDXGISurface.

    Args:
        dxgi_surface (IDXGISurface*): The IDXGISurface to create the Direct3DSurface from.

    Returns:
        Pointer to IDirect3DSurface interface of the IDXGISurface.
    """

def get_dxgi_device_from_object(object: Object) -> int:
    """
    Retrieves the IDXGIDevice from a Direct3D object.

    Args:
        object: The WinRT object to retrieve the IDXGIDevice from.

    Returns:
        Pointer to IDXGIDevice interface of the WinRT object.
    """

def get_dxgi_surface_from_object(object: Object) -> int:
    """
    Retrieves the IDXGISurface from a Direct3D object.

    Args:
        object: The WinRT object to retrieve the IDXGISurface from.

    Returns:
        Pointer to IDXGISurface interface of the WinRT object.
    """
