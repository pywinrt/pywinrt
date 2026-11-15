#include "interop.h"

#include <shobjidl_core.h>

// https://learn.microsoft.com/en-us/windows/win32/api/shobjidl_core/nn-shobjidl_core-idatatransfermanagerinterop

namespace
{
    /// ABI::Windows::ApplicationModel::DataTransfer::IDataTransferManager
    constexpr IID IID_IDataTransferManager{
        0xA5CAEE9B, 0x8708, 0x49D1, {0x8D, 0x36, 0x67, 0xD2, 0x5A, 0x8D, 0xA0, 0x0C}};

    /// Parses the (factory, hwnd) arguments both functions take.
    bool parse_args(
        PyObject* args, IDataTransferManagerInterop** factory, HWND* hwnd) noexcept
    {
        PyObject* factory_capsule;
        PyObject* hwnd_obj;

        if (!PyArg_ParseTuple(args, "OO", &factory_capsule, &hwnd_obj))
        {
            return false;
        }

        auto const handle = PyLong_AsVoidPtr(hwnd_obj);
        if (!handle && PyErr_Occurred())
        {
            return false;
        }

        if (!interop::query_capsule(factory_capsule, factory))
        {
            return false;
        }

        *hwnd = static_cast<HWND>(handle);
        return true;
    }

    PyObject* get_for_window(PyObject* /*unused*/, PyObject* args) noexcept
    {
        IDataTransferManagerInterop* factory{};
        HWND hwnd{};
        if (!parse_args(args, &factory, &hwnd))
        {
            return nullptr;
        }

        IUnknown* manager{};
        HRESULT hr;
        {
            interop::gil_released const guard;
            hr = factory->GetForWindow(
                hwnd, IID_IDataTransferManager, reinterpret_cast<void**>(&manager));
            factory->Release();
        }
        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr);
        }

        return interop::new_interface_capsule(manager);
    }

    PyObject* show_share_ui_for_window(PyObject* /*unused*/, PyObject* args) noexcept
    {
        IDataTransferManagerInterop* factory{};
        HWND hwnd{};
        if (!parse_args(args, &factory, &hwnd))
        {
            return nullptr;
        }

        HRESULT hr;
        {
            // The window's DataTransferManager raises DataRequested to fill the
            // sheet, and its handler may be Python on another thread.
            interop::gil_released const guard;
            hr = factory->ShowShareUIForWindow(hwnd);
            factory->Release();
        }
        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr);
        }

        Py_RETURN_NONE;
    }

    PyMethodDef module_methods[]{
        {"get_for_window", get_for_window, METH_VARARGS, nullptr},
        {"show_share_ui_for_window", show_share_ui_for_window, METH_VARARGS, nullptr},
        {}};

    PyDoc_STRVAR(
        module_doc,
        "APIs for desktop interop with the Windows.ApplicationModel.DataTransfer "
        "namespace.");

    PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_windows_applicationmodel_datatransfer_interop",
           module_doc,
           0,
           module_methods,
           interop::module_slots,
           nullptr,
           nullptr,
           nullptr};
} // namespace

PyMODINIT_FUNC PyInit__winrt_windows_applicationmodel_datatransfer_interop(
    void) noexcept
{
    return PyModuleDef_Init(&module_def);
}
