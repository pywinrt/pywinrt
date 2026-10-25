#include "interop.h"

#include <d3d11.h>

#include <cstdint>
#include <memory>

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

    struct release_com
    {
        void operator()(::IUnknown* p) const noexcept
        {
            p->Release();
        }
    };

    /// Owns a reference to a COM interface, which it releases.
    template<typename I>
    using com_owner = std::unique_ptr<I, release_com>;

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

    /// PyLong_AsInt32() as Python 3.14 has it, which is how winrt-runtime
    /// converts an enum that is not a flags enum.
    /// @returns false with a Python error set.
    bool as_int32(PyObject* obj, std::int32_t* value) noexcept
    {
#if PY_VERSION_HEX >= 0x030E0000
        return PyLong_AsInt32(obj, value) == 0;
#else
        // a long is 32 bits on Windows, so this raises OverflowError for what
        // does not fit, as PyLong_AsInt32() does
        auto const result = PyLong_AsLong(obj);
        if (result == -1 && PyErr_Occurred())
        {
            return false;
        }

        *value = result;
        return true;
#endif
    }

    /// PyLong_AsUInt32() as Python 3.14 has it, which is how winrt-runtime
    /// converts a flags enum: a negative number raises ValueError and one too
    /// large OverflowError.
    /// @returns false with a Python error set.
    bool as_uint32(PyObject* obj, std::uint32_t* value) noexcept
    {
#if PY_VERSION_HEX >= 0x030E0000
        return PyLong_AsUInt32(obj, value) == 0;
#else
        // This goes when the oldest Python supported is 3.14.
        auto const result = PyLong_AsLongLong(obj);
        if (result == -1 && PyErr_Occurred())
        {
            return false;
        }

        if (result < 0)
        {
            PyErr_SetString(PyExc_ValueError, "Cannot convert negative int");
            return false;
        }

        if (result > UINT32_MAX)
        {
            PyErr_SetString(
                PyExc_OverflowError, "Python int too large to convert to C uint32_t");
            return false;
        }

        *value = static_cast<std::uint32_t>(result);
        return true;
#endif
    }

    // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-d3d11createdevice
    PyObject* create_direct3d11_device(PyObject* /*unused*/, PyObject* args) noexcept
    {
        PyObject* driver_type_obj;
        PyObject* flags_obj;

        if (!PyArg_ParseTuple(args, "OO", &driver_type_obj, &flags_obj))
        {
            return nullptr;
        }

        std::int32_t driver_type;
        if (!as_int32(driver_type_obj, &driver_type))
        {
            return nullptr;
        }

        // Only a value of D3D_DRIVER_TYPE may be cast to it. UNKNOWN and
        // SOFTWARE are among them, and D3D11CreateDevice refuses them, since
        // there is neither an adapter nor a software rasterizer.
        if (driver_type < D3D_DRIVER_TYPE_UNKNOWN || driver_type > D3D_DRIVER_TYPE_WARP)
        {
            PyErr_Format(
                PyExc_ValueError,
                "%d is not a valid D3dDriverType",
                static_cast<int>(driver_type));
            return nullptr;
        }

        std::uint32_t flags;
        if (!as_uint32(flags_obj, &flags))
        {
            return nullptr;
        }

        ID3D11Device* d3d11_device_abi{};
        HRESULT hr;

        {
            // creating a hardware device can take tens of milliseconds
            interop::gil_released const guard;
            hr = D3D11CreateDevice(
                nullptr,
                static_cast<D3D_DRIVER_TYPE>(driver_type),
                nullptr,
                flags,
                nullptr,
                0,
                D3D11_SDK_VERSION,
                &d3d11_device_abi,
                nullptr,
                nullptr);
        }

        // Neither this nor QueryInterface() sets error info, so whatever is on
        // the thread was left by some earlier call and is not taken.
        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr, nullptr);
        }

        // An error is raised before these are released, when the function
        // returns, so a release cannot replace the error info it takes.
        com_owner<ID3D11Device> const d3d11_device{d3d11_device_abi};

        IDXGIDevice* dxgi_device_abi{};
        hr = d3d11_device->QueryInterface(
            __uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgi_device_abi));
        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr, nullptr);
        }

        com_owner<IDXGIDevice> const dxgi_device{dxgi_device_abi};

        ::IInspectable* d3d_device{};
        hr = CreateDirect3D11DeviceFromDXGIDevice(dxgi_device.get(), &d3d_device);
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
        {"create_direct3d11_device", create_direct3d11_device, METH_VARARGS, nullptr},
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
           interop::module_slots,
           nullptr,
           nullptr,
           nullptr};
} // namespace

PyMODINIT_FUNC PyInit__winrt_windows_graphics_directx_direct3d11_interop(void) noexcept
{
    return PyModuleDef_Init(&module_def);
}
