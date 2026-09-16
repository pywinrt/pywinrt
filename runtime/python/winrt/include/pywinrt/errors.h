// Failures, in both directions: the errors the projection raises itself, and
// the C++ and WinRT exceptions it has to turn into a Python exception on the way
// out of a method call or a callback.
//
// py::python_exception is the marker thrown once a Python error is already set,
// so the unwinding path does not have to carry the error with it, and
// py::to_PyErr() is the catch-all that every projected method ends with.

#pragma once

#include <pywinrt/abi.h>
#include <pywinrt/handles.h>
#include <pywinrt/prelude.h>

namespace py
{
    const winrt::hresult unraisable_python_exception{static_cast<int32_t>(0xA0EE4005)};

    /**
     * Thrown when a Python exception is pending (i.e. PyErr_Occurred() returns
     * non-NULL).
     */
    struct python_exception
    {
    };

    inline __declspec(noinline) void set_invalid_activation_error(
        const char* const type_name)
    {
        PyErr_Format(PyExc_TypeError, "cannot create '%s' instances", type_name);
    }

    inline __declspec(noinline) void set_arg_count_version_error(
        Py_ssize_t arg_count) noexcept
    {
        PyErr_Format(
            PyExc_AttributeError,
            "method overload with %zd arg(s) is not available in this version of Windows",
            arg_count);
    }

    inline __declspec(noinline) void set_invalid_arg_count_error(
        Py_ssize_t arg_count) noexcept
    {
        if (arg_count != -1)
        {
            PyErr_SetString(PyExc_TypeError, "Invalid parameter count");
        }
    }

    inline __declspec(noinline) void set_invalid_kwd_args_error() noexcept
    {
        PyErr_SetString(PyExc_TypeError, "keyword arguments not supported");
    }

    [[noreturn]] inline __declspec(noinline) void throw_member_not_available(
        member_kind kind,
        const char* type_name,
        const char* member_name,
        const char* interface_name,
        uint32_t arg_count)
    {
        throw member_not_available{
            kind, arg_count, type_name, member_name, interface_name};
    }

    /**
     * Queries @p obj for the interface @p I that declares a member.
     *
     * C++/WinRT reaches a member of a non-default interface through an implicit
     * conversion that is @c noexcept and yields a *null* interface when the
     * object does not implement it, which the call then dereferences. Asking for
     * the interface here costs the same query and turns that crash into a Python
     * exception.
     *
     * @throws member_not_available if @p obj does not implement @p I.
     */
    template<typename I, typename T>
    [[nodiscard]] I require(
        T const& obj,
        member_kind kind,
        const char* type_name,
        const char* member_name,
        const char* interface_name,
        uint32_t arg_count = 0)
    {
        auto iface = obj.template try_as<I>();

        if (!iface)
        {
            throw_member_not_available(
                kind, type_name, member_name, interface_name, arg_count);
        }

        return iface;
    }

    inline __declspec(noinline) void to_PyErr() noexcept
    {
        if (PyErr_Occurred())
        {
            // A Python exception is already pending - don't overwrite it
            return;
        }

        // otherwise convert C++ exception to Python exception
        try
        {
            throw;
        }
        catch (member_not_available const& e)
        {
            set_member_not_available_error(e);
        }
        catch (winrt::hresult_error const& e)
        {
            pyobj_handle exc{PyObject_CallFunction(
                PyExc_WindowsError,
                "iuui",
                0,                   // errno
                e.message().c_str(), // strerror
                nullptr,             // filename
                e.code().value)};    // winerror

            if (!exc)
            {
                // REVISIT: should we print something here so we don't loose the
                // info? Like: while raising an exception another error occurred...
                return;
            }

            PyErr_SetObject(reinterpret_cast<PyObject*>(Py_TYPE(exc.get())), exc.get());
        }
        catch (std::bad_alloc const& e)
        {
            PyErr_SetString(PyExc_MemoryError, e.what());
        }
        catch (std::out_of_range const& e)
        {
            PyErr_SetString(PyExc_IndexError, e.what());
        }
        catch (std::invalid_argument const& e)
        {
            PyErr_SetString(PyExc_TypeError, e.what());
        }
        catch (std::exception const& e)
        {
            PyErr_SetString(PyExc_RuntimeError, e.what());
        }
    }

    inline void throw_if_pyobj_null(PyObject* obj)
    {
        if (!obj)
        {
            if (!PyErr_Occurred())
            {
                PyErr_SetString(PyExc_SystemError, "object is null");
            }
            throw python_exception();
        }
    }

    [[noreturn]] inline void write_unraisable_and_throw()
    {
        PyErr_WriteUnraisable(nullptr);
        throw winrt::hresult_error(
            unraisable_python_exception, L"Unraisable Python exception");
    }
} // namespace py
