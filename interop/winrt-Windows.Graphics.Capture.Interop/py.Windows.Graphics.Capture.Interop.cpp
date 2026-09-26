#include "interop.h"

#if __has_include("windows.graphics.capture.interop.h")
#include <windows.graphics.capture.interop.h>
#else
// some environments like MSYS2 don't have this header
#include <unknwn.h>

DEFINE_GUID(
    IID_IGraphicsCaptureItemInterop,
    // clang-format off
    0x3628E81B, 0x3CAC, 0x4C60, 0xB7, 0xF4, 0x23, 0xCE, 0x0E, 0x0C, 0x33, 0x56
    // clang-format on
);

MIDL_INTERFACE("3628E81B-3CAC-4C60-B7F4-23CE0E0C3356")
IGraphicsCaptureItemInterop : ::IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE CreateForWindow(
        HWND window, REFIID riid, _COM_Outptr_ void** result) noexcept = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateForMonitor(
        HMONITOR monitor, REFIID riid, _COM_Outptr_ void** result) noexcept = 0;
};

#ifdef __CRT_UUID_DECL
__CRT_UUID_DECL(
    IGraphicsCaptureItemInterop,
    // clang-format off
    0x3628E81B, 0x3CAC, 0x4C60, 0xB7, 0xF4, 0x23, 0xCE, 0x0E, 0x0C, 0x33, 0x56
    // clang-format on
);
#endif
#endif

// https://docs.microsoft.com/en-us/windows/win32/api/windows.graphics.capture.interop/

namespace
{
    /// ABI::Windows::Graphics::Capture::IGraphicsCaptureItem
    constexpr IID IID_IGraphicsCaptureItem{
        0x79C3F95B, 0x31F7, 0x4EC2, {0xA4, 0x64, 0x63, 0x2E, 0xF5, 0xD3, 0x07, 0x60}};

    /// The interop factory of GraphicsCaptureItem.
    HRESULT get_interop_factory(IGraphicsCaptureItemInterop** factory) noexcept
    {
        return interop::get_activation_factory(
            L"Windows.Graphics.Capture.GraphicsCaptureItem", factory);
    }

    PyObject* create_for_monitor(PyObject* /*unused*/, PyObject* monitor_obj) noexcept
    {
        auto const hmonitor = PyLong_AsVoidPtr(monitor_obj);
        if (!hmonitor && PyErr_Occurred())
        {
            return nullptr;
        }

        IGraphicsCaptureItemInterop* factory{};

        auto hr = get_interop_factory(&factory);
        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr);
        }

        IUnknown* item{};

        hr = factory->CreateForMonitor(
            static_cast<HMONITOR>(hmonitor),
            IID_IGraphicsCaptureItem,
            reinterpret_cast<void**>(&item));
        factory->Release();
        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr);
        }

        return interop::new_interface_capsule(item);
    }

    PyObject* create_for_window(PyObject* /*unused*/, PyObject* window_obj) noexcept
    {
        auto const hwnd = PyLong_AsVoidPtr(window_obj);
        if (!hwnd && PyErr_Occurred())
        {
            return nullptr;
        }

        // https://github.com/microsoft/Windows.UI.Composition-Win32-Samples/blob/a59e7586c0bd1a967e1e25f6ca0363e20151afe5/cpp/ScreenCaptureforHWND/ScreenCaptureforHWND/capture.interop.h#L11
        IGraphicsCaptureItemInterop* factory{};

        auto hr = get_interop_factory(&factory);
        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr);
        }

        IUnknown* item{};

        hr = factory->CreateForWindow(
            static_cast<HWND>(hwnd),
            IID_IGraphicsCaptureItem,
            reinterpret_cast<void**>(&item));
        factory->Release();
        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr);
        }

        return interop::new_interface_capsule(item);
    }

    PyMethodDef module_methods[]{
        {"create_for_monitor", create_for_monitor, METH_O, nullptr},
        {"create_for_window", create_for_window, METH_O, nullptr},
        {}};

    PyDoc_STRVAR(
        module_doc,
        "APIs for desktop interop with the Windows.Graphics.Capture namespace.");

    PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_windows_graphics_capture_interop",
           module_doc,
           0,
           module_methods,
           interop::module_slots,
           nullptr,
           nullptr,
           nullptr};
} // namespace

PyMODINIT_FUNC PyInit__winrt_windows_graphics_capture_interop(void) noexcept
{
    return PyModuleDef_Init(&module_def);
}
