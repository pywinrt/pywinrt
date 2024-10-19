===============================================================
:mod:`winrt.windows.graphics.directx.direct3d11.interop` module
===============================================================

APIs for desktop interop with the `Windows.Graphics.DirectX.Direct3D11
<https://learn.microsoft.com/uwp/api/windows.graphics.directx.direct3d11>`_
namespace.

.. function:: create_direct3d11_device_from_dxgi_device(dxgi_device)

    Creates a WinRT Direct3D 11 device from a DXGI device.

    :param int dxgi_device: Pointer to an IDXGIDevice to create the Direct3D 11 device from.

    :return: A Direct3D 11 device.
    :rtype: :obj:`winrt.windows.graphics.directx.direct3d11.Direct3D11Device`

    :raises: :exc:`OSError` on failure.

    .. versionadded:: 2.3

    .. seealso:: https://learn.microsoft.com/en-us/windows/win32/api/windows.graphics.directx.direct3d11.interop/nf-windows-graphics-directx-direct3d11-interop-createdirect3d11devicefromdxgidevice

.. function:: create_direct3d11_surface_from_dxgi_surface(dxgi_surface)

    Creates a WinRT Direct3D 11 surface from a DXGI surface.

    :param int dxgi_surface: Pointer to an IDXGISurface to create the Direct3D 11 surface from.

    :return: A Direct3D 11 surface.
    :rtype: :obj:`winrt.windows.graphics.directx.direct3d11.Direct3D11Surface`

    :raises: :exc:`OSError` on failure.

    .. versionadded:: 2.3

    .. seealso:: https://learn.microsoft.com/en-us/windows/win32/api/windows.graphics.directx.direct3d11.interop/nf-windows-graphics-directx-direct3d11-interop-createdirect3d11surfacefromdxgisurface

.. function:: get_dxgi_device_from_object(device)

    Retrieves the IDXGIDevice from a Direct3D object.

    :param device: The WinRT object to retrieve the IDXGIDevice from.
    :type device: :class:`winrt.windows.graphics.directx.direct3d11.Direct3D11Device`

    :return: Pointer to IDXGIDevice interface of the WinRT object.
    :rtype: int

    .. versionadded:: 2.3

    .. seealso:: https://learn.microsoft.com/en-us/windows/win32/api/windows.graphics.directx.direct3d11.interop/nf-windows-graphics-directx-direct3d11-interop-getdxgiinterface

.. function:: get_dxgi_surface_from_object(surface)

    Retrieves the IDXGISurface from a Direct3D object.

    :param surface: The WinRT object to retrieve the IDXGISurface from.
    :type surface: :class:`winrt.windows.graphics.directx.direct3d11.Direct3D11Surface`

    :return: Pointer to IDXGISurface interface of the WinRT object.
    :rtype: int

    .. versionadded:: 2.3

    .. seealso:: https://learn.microsoft.com/en-us/windows/win32/api/windows.graphics.directx.direct3d11.interop/nf-windows-graphics-directx-direct3d11-interop-getdxgiinterface-r1
