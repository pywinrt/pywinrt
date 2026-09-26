#include "interop.h"

#if __has_include("windows.graphics.directx.direct3d11.interop.h")
#include <windows.graphics.directx.direct3d11.interop.h>

#ifdef __MINGW32__
// mingw-w64 generates this header from an .idl that is missing this
// function, so declare it here. Everything else the header needs is
// there, including the C++ IDirect3DDxgiInterfaceAccess and its uuid.
extern "C" HRESULT __stdcall CreateDirect3D11SurfaceFromDXGISurface(
    ::IDXGISurface* dgxiSurface, ::IInspectable** graphicsSurface);
#endif

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
            REFIID riid, _COM_Outptr_ void** p) noexcept = 0;
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

// https://learn.microsoft.com/en-us/windows/win32/api/windows.graphics.directx.direct3d11.interop/

namespace
{
    using ::Windows::Graphics::DirectX::Direct3D11::IDirect3DDxgiInterfaceAccess;

    PyObject* create_direct3d11_device_from_dxgi_device(
        PyObject* /*unused*/, PyObject* dxgi_device_obj) noexcept
    {
        auto const dxgi_device
            = static_cast<IDXGIDevice*>(PyLong_AsVoidPtr(dxgi_device_obj));
        if (!dxgi_device && PyErr_Occurred())
        {
            return nullptr;
        }

        ::IInspectable* d3d_device{};

        auto const hr = CreateDirect3D11DeviceFromDXGIDevice(dxgi_device, &d3d_device);
        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr);
        }

        return interop::new_interface_capsule(d3d_device);
    }

    PyObject* create_direct3d11_surface_from_dxgi_surface(
        PyObject* /*unused*/, PyObject* dxgi_surface_obj) noexcept
    {
        auto const dxgi_surface
            = static_cast<IDXGISurface*>(PyLong_AsVoidPtr(dxgi_surface_obj));
        if (!dxgi_surface && PyErr_Occurred())
        {
            return nullptr;
        }

        ::IInspectable* d3d_surface{};

        auto const hr
            = CreateDirect3D11SurfaceFromDXGISurface(dxgi_surface, &d3d_surface);
        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr);
        }

        return interop::new_interface_capsule(d3d_surface);
    }

    /// The @p I interface that the Direct3D object in @p capsule wraps, as an
    /// integer that owns a reference to it.
    template<typename I>
    PyObject* get_dxgi_interface(PyObject* capsule) noexcept
    {
        IDirect3DDxgiInterfaceAccess* access{};
        if (!interop::query_capsule(capsule, &access))
        {
            return nullptr;
        }

        I* result{};

        auto const hr
            = access->GetInterface(__uuidof(I), reinterpret_cast<void**>(&result));
        access->Release();
        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr);
        }

        auto const value = PyLong_FromVoidPtr(result);
        if (!value)
        {
            result->Release();
        }

        return value;
    }

    PyObject* get_dxgi_device_from_object(
        PyObject* /*unused*/, PyObject* capsule) noexcept
    {
        return get_dxgi_interface<IDXGIDevice>(capsule);
    }

    PyObject* get_dxgi_surface_from_object(
        PyObject* /*unused*/, PyObject* capsule) noexcept
    {
        return get_dxgi_interface<IDXGISurface>(capsule);
    }

    PyMethodDef module_methods[]{
        {"create_direct3d11_device_from_dxgi_device",
         create_direct3d11_device_from_dxgi_device,
         METH_O,
         nullptr},
        {"create_direct3d11_surface_from_dxgi_surface",
         create_direct3d11_surface_from_dxgi_surface,
         METH_O,
         nullptr},
        {"get_dxgi_device_from_object", get_dxgi_device_from_object, METH_O, nullptr},
        {"get_dxgi_surface_from_object", get_dxgi_surface_from_object, METH_O, nullptr},
        {}};

    PyDoc_STRVAR(
        module_doc,
        "Interop functions for use with IDirect3DDevice and IDirect3DSurface.");

    PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_windows_graphics_directx_direct3d11_interop",
           module_doc,
           0,
           module_methods,
           nullptr,
           nullptr,
           nullptr,
           nullptr};
} // namespace

PyMODINIT_FUNC PyInit__winrt_windows_graphics_directx_direct3d11_interop(void) noexcept
{
    return PyModule_Create(&module_def);
}
