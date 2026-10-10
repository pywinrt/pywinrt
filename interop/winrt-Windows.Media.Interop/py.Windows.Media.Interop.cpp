#include "interop.h"

#include <systemmediatransportcontrolsinterop.h>

// https://learn.microsoft.com/en-us/windows/win32/api/systemmediatransportcontrolsinterop/

namespace
{
    /// ABI::Windows::Media::ISystemMediaTransportControls
    constexpr IID IID_ISystemMediaTransportControls{
        0x99FA3FF4, 0x1742, 0x42A6, {0x90, 0x2E, 0x08, 0x7D, 0x41, 0xF9, 0x65, 0xEC}};

    PyObject* get_for_window(PyObject* /*unused*/, PyObject* args) noexcept
    {
        PyObject* factory_capsule;
        PyObject* hwnd_obj;

        if (!PyArg_ParseTuple(args, "OO", &factory_capsule, &hwnd_obj))
        {
            return nullptr;
        }

        auto const hwnd = PyLong_AsVoidPtr(hwnd_obj);
        if (!hwnd && PyErr_Occurred())
        {
            return nullptr;
        }

        ISystemMediaTransportControlsInterop* factory{};
        if (!interop::query_capsule(factory_capsule, &factory))
        {
            return nullptr;
        }

        IUnknown* controls{};

        auto const hr = factory->GetForWindow(
            static_cast<HWND>(hwnd),
            IID_ISystemMediaTransportControls,
            reinterpret_cast<void**>(&controls));
        factory->Release();
        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr);
        }

        return interop::new_interface_capsule(controls);
    }

    PyMethodDef module_methods[]{
        {"get_for_window", get_for_window, METH_VARARGS, nullptr}, {}};

    PyDoc_STRVAR(
        module_doc, "APIs for desktop interop with the Windows.Media namespace.");

    PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_windows_media_interop",
           module_doc,
           0,
           module_methods,
           interop::module_slots,
           nullptr,
           nullptr,
           nullptr};
} // namespace

PyMODINIT_FUNC PyInit__winrt_windows_media_interop(void) noexcept
{
    return PyModuleDef_Init(&module_def);
}
