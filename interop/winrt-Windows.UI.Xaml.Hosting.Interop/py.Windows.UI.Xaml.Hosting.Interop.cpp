#include "interop.h"

#include <windows.ui.xaml.hosting.desktopwindowxamlsource.h>

// https://learn.microsoft.com/en-us/windows/win32/api/windows.ui.xaml.hosting.desktopwindowxamlsource/

namespace
{
    PyObject* attach_to_window(PyObject* /*unused*/, PyObject* args) noexcept
    {
        PyObject* capsule;
        PyObject* hwnd_obj;

        if (!PyArg_ParseTuple(args, "OO", &capsule, &hwnd_obj))
        {
            return nullptr;
        }

        auto const hwnd = PyLong_AsVoidPtr(hwnd_obj);
        if (!hwnd && PyErr_Occurred())
        {
            return nullptr;
        }

        IDesktopWindowXamlSourceNative* native{};
        if (!interop::query_capsule(capsule, &native))
        {
            return nullptr;
        }

        auto const hr = native->AttachToWindow(static_cast<HWND>(hwnd));
        native->Release();
        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr);
        }

        Py_RETURN_NONE;
    }

    PyObject* get_window_handle(PyObject* /*unused*/, PyObject* capsule) noexcept
    {
        IDesktopWindowXamlSourceNative* native{};
        if (!interop::query_capsule(capsule, &native))
        {
            return nullptr;
        }

        HWND hwnd{};

        auto const hr = native->get_WindowHandle(&hwnd);
        native->Release();
        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr);
        }

        return PyLong_FromVoidPtr(hwnd);
    }

    /// Sets BufferError unless @p view is the size of a MSG. Its format is not
    /// checked, because the field names in it are the exporter's own choice, so
    /// a ctypes.wintypes.MSG and a structure of the same layout both pass.
    bool check_msg_buffer(Py_buffer const& view) noexcept
    {
        if (view.len != static_cast<Py_ssize_t>(sizeof(MSG)))
        {
            PyErr_Format(
                PyExc_BufferError,
                "requires buffer of %zu bytes (a MSG), have %zd",
                sizeof(MSG),
                view.len);
            return false;
        }

        return true;
    }

    PyObject* pretranslate_message(PyObject* /*unused*/, PyObject* args) noexcept
    {
        PyObject* capsule;
        PyObject* msg;

        if (!PyArg_ParseTuple(args, "OO", &capsule, &msg))
        {
            return nullptr;
        }

        Py_buffer view;
        if (PyObject_GetBuffer(msg, &view, PyBUF_SIMPLE) == -1)
        {
            return nullptr;
        }

        if (!check_msg_buffer(view))
        {
            PyBuffer_Release(&view);
            return nullptr;
        }

        IDesktopWindowXamlSourceNative2* native2{};
        if (!interop::query_capsule(capsule, &native2))
        {
            PyBuffer_Release(&view);
            return nullptr;
        }

        BOOL handled{};

        auto const hr
            = native2->PreTranslateMessage(static_cast<MSG const*>(view.buf), &handled);
        native2->Release();
        PyBuffer_Release(&view);
        if (FAILED(hr))
        {
            return interop::set_hresult_error(hr);
        }

        return PyBool_FromLong(handled);
    }

    /// The identity of the COM object @p capsule holds, which is the address of
    /// its IUnknown, for as long as something holds a reference to it.
    PyObject* get_identity(PyObject* /*unused*/, PyObject* capsule) noexcept
    {
        IUnknown* identity{};
        if (!interop::query_capsule(capsule, &identity))
        {
            return nullptr;
        }

        identity->Release();

        return PyLong_FromVoidPtr(identity);
    }

    PyMethodDef module_methods[]{
        {"attach_to_window", attach_to_window, METH_VARARGS, nullptr},
        {"get_window_handle", get_window_handle, METH_O, nullptr},
        {"pretranslate_message", pretranslate_message, METH_VARARGS, nullptr},
        {"get_identity", get_identity, METH_O, nullptr},
        {}};

    PyDoc_STRVAR(
        module_doc,
        "APIs for desktop interop with the Windows.UI.Xaml.Hosting namespace.");

    PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt_windows_ui_xaml_hosting_interop",
           module_doc,
           0,
           module_methods,
           interop::module_slots,
           nullptr,
           nullptr,
           nullptr};
} // namespace

PyMODINIT_FUNC PyInit__winrt_windows_ui_xaml_hosting_interop(void) noexcept
{
    return PyModuleDef_Init(&module_def);
}
