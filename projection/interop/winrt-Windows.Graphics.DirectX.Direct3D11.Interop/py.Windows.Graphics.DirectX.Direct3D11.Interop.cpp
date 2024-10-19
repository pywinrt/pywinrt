#if __has_include("windows.graphics.directx.direct3d11.interop.h")
#include <windows.graphics.directx.direct3d11.interop.h>
#else
// some environments like MSYS2 don't have the interop header
#include <dxgi.h>
#include <inspectable.h>
#include <unknwn.h>

// https://gist.github.com/kennykerr/15a62c8218254bc908de672e5ed405fa

extern "C"
{
    HRESULT __stdcall CreateDirect3D11DeviceFromDXGIDevice(
        ::IDXGIDevice* dxgiDevice, ::IInspectable** graphicsDevice);

    HRESULT __stdcall CreateDirect3D11SurfaceFromDXGISurface(
        ::IDXGISurface* dgxiSurface, ::IInspectable** graphicsSurface);
}

DEFINE_GUID(
    IID__x_Windows_Graphics_DirectX_Direct3D11_IDirect3DDxgiInterfaceAccess,
    // clang-format off
    0xA9B3D012, 0x3DF2, 0x4EE3, 0xB8, 0xD1, 0x86, 0x95, 0xF4, 0x57, 0xD3, 0xC1
    // clang-format on
);

namespace Windows::Graphics::DirectX::Direct3D11
{
    MIDL_INTERFACE("A9B3D012-3DF2-4EE3-B8D1-8695F457D3C1")
    IDirect3DDxgiInterfaceAccess : ::IUnknown
    {
        virtual HRESULT STDMETHODCALLTYPE GetInterface(
            REFIID riid, _COM_Outptr_ void** p) noexcept
            = 0;
    };
} // namespace Windows::Graphics::DirectX::Direct3D11

#ifdef __CRT_UUID_DECL
__CRT_UUID_DECL(
    Windows::Graphics::DirectX::Direct3D11::IDirect3DDxgiInterfaceAccess,
    // clang-format off
    0xA9B3D012, 0x3DF2, 0x4EE3, 0xB8, 0xD1, 0x86, 0x95, 0xF4, 0x57, 0xD3, 0xC1
    // clang-format on
);
#endif
#endif

#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include "pybase.h"
#include "py.Windows.Graphics.DirectX.Direct3D11.h"

// https://learn.microsoft.com/en-us/windows/win32/api/windows.graphics.directx.direct3d11.interop/

namespace py::cpp::Windows::Graphics::DirectX::Direct3D11::Interop
{
    static PyObject* create_direct3d11_device_from_dxgi_device(
        PyObject* /*unused*/, PyObject* dxgi_device_obj) noexcept
    {
        try
        {
            auto dxgi_device
                = reinterpret_cast<IDXGIDevice*>(PyLong_AsVoidPtr(dxgi_device_obj));

            if (!dxgi_device && PyErr_Occurred())
            {
                return nullptr;
            }

            winrt::com_ptr<::IInspectable> d3d_device;
            winrt::check_hresult(
                CreateDirect3D11DeviceFromDXGIDevice(dxgi_device, d3d_device.put()));

            return py::convert(
                d3d_device.as<
                    winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyDoc_STRVAR(
        create_direct3d11_device_from_dxgi_device_doc,
        R"(Creates an instance of IDirect3DDevice from an IDXGIDevice.

Args:
    dxgi_device (IDXGIDevice*): The IDXGIDevice to create the Direct3DDevice from.

Returns:
    A Direct3DDevice instance that wraps the DXGIDevice.
)");

    static PyObject* create_direct3d11_surface_from_dxgi_surface(
        PyObject* /*unused*/, PyObject* dxgi_surface_obj) noexcept
    {
        try
        {
            auto dxgi_surface
                = reinterpret_cast<IDXGISurface*>(PyLong_AsVoidPtr(dxgi_surface_obj));

            if (!dxgi_surface && PyErr_Occurred())
            {
                return nullptr;
            }

            winrt::com_ptr<::IInspectable> d3d_surface;
            winrt::check_hresult(CreateDirect3D11SurfaceFromDXGISurface(
                dxgi_surface, d3d_surface.put()));

            return py::convert(
                d3d_surface.as<
                    winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface>());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyDoc_STRVAR(
        create_direct3d11_surface_from_dxgi_surface_doc,
        R"(Creates an instance of IDirect3DSurface from an IDXGISurface.

Args:
    dxgi_surface (IDXGISurface*): The IDXGISurface to create the Direct3DSurface from.

Returns:
    A Direct3DSurface instance that wraps the DXGISurface.
)");

    static PyObject* get_dxgi_device_from_object(
        PyObject* /*unused*/, PyObject* py_obj) noexcept
    {
        try
        {
            auto object
                = py::convert_to<winrt::Windows::Foundation::IInspectable>(py_obj);
            auto access = object.as<::Windows::Graphics::DirectX::Direct3D11::
                                        IDirect3DDxgiInterfaceAccess>();
            winrt::com_ptr<IDXGIDevice> result;
            winrt::check_hresult(
                access->GetInterface(winrt::guid_of<IDXGIDevice>(), result.put_void()));

            return PyLong_FromVoidPtr(result.detach());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* get_dxgi_surface_from_object(
        PyObject* /*unused*/, PyObject* py_obj) noexcept
    {
        try
        {
            auto object
                = py::convert_to<winrt::Windows::Foundation::IInspectable>(py_obj);
            auto access = object.as<::Windows::Graphics::DirectX::Direct3D11::
                                        IDirect3DDxgiInterfaceAccess>();
            winrt::com_ptr<IDXGISurface> result;
            winrt::check_hresult(access->GetInterface(
                winrt::guid_of<IDXGISurface>(), result.put_void()));

            return PyLong_FromVoidPtr(result.detach());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef module_methods[]{
        {"create_direct3d11_device_from_dxgi_device",
         create_direct3d11_device_from_dxgi_device,
         METH_O,
         create_direct3d11_device_from_dxgi_device_doc},
        {"create_direct3d11_surface_from_dxgi_surface",
         create_direct3d11_surface_from_dxgi_surface,
         METH_O,
         create_direct3d11_surface_from_dxgi_surface_doc},
        {"get_dxgi_device_from_object", get_dxgi_device_from_object, METH_O, nullptr},
        {"get_dxgi_surface_from_object", get_dxgi_surface_from_object, METH_O, nullptr},
        {}};

    PyDoc_STRVAR(
        module_doc,
        "Interop functions for use with IDirect3DDevice and IDirect3DSurface.");

    static PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_windows_graphics_directx_direct3d11_interop",
           module_doc,
           0,
           module_methods,
           nullptr,
           nullptr,
           nullptr,
           nullptr};
} // namespace py::cpp::Windows::Graphics::DirectX::Direct3D11::Interop

PyMODINIT_FUNC PyInit__winrt_windows_graphics_directx_direct3d11_interop(void) noexcept
{
    using namespace py::cpp::Windows::Graphics::DirectX::Direct3D11::Interop;

    if (py::import_winrt_runtime() == -1)
    {
        return nullptr;
    }

    return PyModule_Create(&module_def);
}
