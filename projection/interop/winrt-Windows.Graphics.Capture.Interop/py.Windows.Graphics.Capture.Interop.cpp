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
        HWND window, REFIID riid, _COM_Outptr_ void** result) noexcept
        = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateForMonitor(
        HMONITOR monitor, REFIID riid, _COM_Outptr_ void** result) noexcept
        = 0;
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

#include "pybase.h"
#include "py.Windows.Graphics.Capture.h"
#include <winrt/Windows.Graphics.Capture.h>

// https://docs.microsoft.com/en-us/windows/win32/api/windows.graphics.capture.interop/

namespace py::cpp::Windows::Graphics::Capture::Interop
{
    static PyObject* create_for_monitor(
        PyObject* /*unused*/, PyObject* window_obj) noexcept
    {
        try
        {
            auto hmonitor = PyLong_AsVoidPtr(window_obj);

            if (!hmonitor && PyErr_Occurred())
            {
                return nullptr;
            }

            auto activation_factory = winrt::get_activation_factory<
                winrt::Windows::Graphics::Capture::GraphicsCaptureItem>();
            auto interop_factory = activation_factory.as<IGraphicsCaptureItemInterop>();
            winrt::Windows::Graphics::Capture::GraphicsCaptureItem item = {nullptr};
            auto result = interop_factory->CreateForMonitor(
                reinterpret_cast<HMONITOR>(hmonitor),
                winrt::guid_of<
                    winrt::Windows::Graphics::Capture::IGraphicsCaptureItem>(),
                reinterpret_cast<void**>(winrt::put_abi(item)));

            if (result < 0)
            {
                winrt::throw_hresult(result);
            }

            return py::convert(item);
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* create_for_window(
        PyObject* /*unused*/, PyObject* window_obj) noexcept
    {
        try
        {
            auto hwnd = PyLong_AsVoidPtr(window_obj);

            if (!hwnd && PyErr_Occurred())
            {
                return nullptr;
            }

            // https://github.com/microsoft/Windows.UI.Composition-Win32-Samples/blob/a59e7586c0bd1a967e1e25f6ca0363e20151afe5/cpp/ScreenCaptureforHWND/ScreenCaptureforHWND/capture.interop.h#L11
            auto activation_factory = winrt::get_activation_factory<
                winrt::Windows::Graphics::Capture::GraphicsCaptureItem>();
            auto interop_factory = activation_factory.as<IGraphicsCaptureItemInterop>();
            winrt::Windows::Graphics::Capture::GraphicsCaptureItem item = {nullptr};
            auto result = interop_factory->CreateForWindow(
                reinterpret_cast<HWND>(hwnd),
                winrt::guid_of<
                    winrt::Windows::Graphics::Capture::IGraphicsCaptureItem>(),
                reinterpret_cast<void**>(winrt::put_abi(item)));

            if (result < 0)
            {
                winrt::throw_hresult(result);
            }

            return py::convert(item);
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyDoc_STRVAR(
        create_for_monitor_doc,
        R"(Targets a monitor(s) for the creation of a graphics capture item.

Args:
    monitor (HMONITOR): The monitor handle that represents the monitor to capture.

Returns:
    A graphics capture item.

Requirements:
    Windows 10 1903 (build 18334)
)");

    PyDoc_STRVAR(
        create_for_window_doc,
        R"(Targets a single window for the creation of a graphics capture item.

Args:
    window (HWND): The window handle that represents the window to capture.

Returns:
    A graphics capture item.

Requirements:
    Windows 10 1903 (build 18334)
)");

    static PyMethodDef module_methods[]{
        {"create_for_monitor", create_for_monitor, METH_O, create_for_monitor_doc},
        {"create_for_window", create_for_window, METH_O, create_for_window_doc},
        {}};

    PyDoc_STRVAR(
        module_doc,
        "APIs for desktop interop with the Windows.Graphics.Capture namespace.");

    static PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_windows_graphics_capture_interop",
           module_doc,
           0,
           module_methods,
           nullptr,
           nullptr,
           nullptr,
           nullptr};
} // namespace py::cpp::Windows::Graphics::Capture::Interop

PyMODINIT_FUNC PyInit__winrt_windows_graphics_capture_interop(void) noexcept
{
    using namespace py::cpp::Windows::Graphics::Capture::Interop;

    if (py::import_winrt_runtime() == -1)
    {
        return nullptr;
    }

    return PyModule_Create(&module_def);
}
