#include "interop.h"

#include <windows.ui.xaml.hosting.desktopwindowxamlsource.h>

#include <cstring>

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

    /// The format of a Python buffer that holds MSG structures.
    constexpr char msg_format[]
        = "T{P:hwnd:I:message:P:wParam:P:lParam:L:time:T{l:x:l:y:}:point:L:lPrivate:}";

    /// Sets BufferError unless @p view is a one-dimensional array of MSG.
    bool check_msg_buffer(Py_buffer const& view) noexcept
    {
        if (view.itemsize != sizeof(MSG))
        {
            PyErr_Format(
                PyExc_BufferError,
                "requires buffer with itemsize == %zu, have %zd",
                sizeof(MSG),
                view.itemsize);
            return false;
        }

        if (!view.format)
        {
            PyErr_Format(
                PyExc_BufferError, "requires buffer with format == \"%s\"", msg_format);
            return false;
        }

        if (std::strcmp(view.format, msg_format) != 0)
        {
            PyErr_Format(
                PyExc_BufferError,
                "requires buffer with format == \"%s\", have \"%s\"",
                msg_format,
                view.format);
            return false;
        }

        if (view.ndim != 1)
        {
            PyErr_Format(
                PyExc_BufferError,
                "requires buffer with ndim == 1, have %d",
                view.ndim);
            return false;
        }

        if (!view.strides)
        {
            PyErr_Format(
                PyExc_BufferError,
                "requires buffer with strides[0] == %zu",
                sizeof(MSG));
            return false;
        }

        if (view.strides[0] != static_cast<Py_ssize_t>(sizeof(MSG)))
        {
            PyErr_Format(
                PyExc_BufferError,
                "requires buffer with strides[0] == %zu, have %zd",
                sizeof(MSG),
                view.strides[0]);
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
        if (PyObject_GetBuffer(msg, &view, PyBUF_C_CONTIGUOUS | PyBUF_FORMAT) == -1)
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
