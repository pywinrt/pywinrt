// The error policy of the projection: which Python exception a failed call
// raises, and what it says.
//
// A projection module does none of this itself. It catches its own C++
// exception in py::to_PyErr(), flattens it into one of the PODs in
// <pywinrt/abi.h>, and calls in here, so the wording, the exception type and
// the HRESULT mapping belong to winrt-runtime and can improve in a runtime
// release without rebuilding any of the projection packages.

#define PYWINRT_RUNTIME_MODULE
#include <pywinrt/base.h>

namespace
{
    /**
     * @p what, or @p fallback when the module had nothing to report.
     */
    const char* what_or(const char* what, const char* fallback) noexcept
    {
        return what && *what ? what : fallback;
    }
} // namespace

void py::set_error(py::error_info const& info) noexcept
{
    switch (info.kind)
    {
    case error_kind::hresult:
    {
        // The message is an hstring owned by the exception the caller is
        // still handling, so it is not necessarily null-terminated and must
        // be copied before this returns.
        pyobj_handle message{PyUnicode_FromWideChar(
            info.message, static_cast<Py_ssize_t>(info.message_length))};

        if (!message)
        {
            return;
        }

        pyobj_handle exc{PyObject_CallFunction(
            PyExc_WindowsError,
            "iOOi",
            0,              // errno
            message.get(),  // strerror
            Py_None,        // filename
            info.hresult)}; // winerror

        if (!exc)
        {
            // REVISIT: should we print something here so we don't loose the
            // info? Like: while raising an exception another error occurred...
            return;
        }

        PyErr_SetObject(reinterpret_cast<PyObject*>(Py_TYPE(exc.get())), exc.get());
        break;
    }

    case error_kind::bad_alloc:
        PyErr_SetString(PyExc_MemoryError, what_or(info.what, "bad allocation"));
        break;

    case error_kind::out_of_range:
        PyErr_SetString(PyExc_IndexError, what_or(info.what, "out of range"));
        break;

    case error_kind::invalid_argument:
        PyErr_SetString(PyExc_TypeError, what_or(info.what, "invalid argument"));
        break;

    case error_kind::std_exception:
        PyErr_SetString(PyExc_RuntimeError, what_or(info.what, "unknown error"));
        break;

    case error_kind::unknown:
    default:
        // Nothing that is thrown on purpose lands here. Reaching it means an
        // exception of a type the projection does not know about escaped a
        // WinRT call, which used to terminate the process on the way out of
        // the noexcept function that caught it.
        PyErr_SetString(
            PyExc_RuntimeError, "a C++ exception of an unknown type was thrown");
        break;
    }
}

void py::set_call_error(
    py::call_error error, py::member_site const* site, Py_ssize_t arg_count) noexcept
{
    switch (error)
    {
    case call_error::invalid_arg_count:
        // -1 is how the generated code reports an argument tuple it could not
        // even count, which it only fails to do with a Python error already set
        if (arg_count != -1)
        {
            PyErr_SetString(PyExc_TypeError, "Invalid parameter count");
        }
        break;

    case call_error::keyword_arguments:
        PyErr_SetString(PyExc_TypeError, "keyword arguments not supported");
        break;

    case call_error::cannot_instantiate:
    default:
        PyErr_Format(
            PyExc_TypeError,
            "cannot create '%s' instances",
            site && site->type_name ? site->type_name : "?");
        break;
    }
}

int32_t py::report_unraisable() noexcept
{
    PyErr_WriteUnraisable(nullptr);

    return unraisable_python_exception.value;
}
