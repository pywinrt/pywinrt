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

    /**
     * The noun for a member of kind @p kind in a message.
     */
    const char* member_kind_name(py::member_kind kind) noexcept
    {
        switch (kind)
        {
        case py::member_kind::method:
            return "method";
        case py::member_kind::property:
            return "property";
        case py::member_kind::event:
            return "event";
        }

        // The kind is read from another module, so it can hold a value this
        // one does not know about, and calling that an event would be a
        // guess rather than an answer.
        return "member";
    }

    /**
     * Asks the metadata whether this version of Windows has the named member
     * at all.
     *
     * This is the only thing on either failure path that knows about
     * @c ApiInformation. It is a cold query - it reads the @c .winmd files
     * through @c WinTypes.dll - which is why it is made here, to choose the
     * wording of an error, rather than before every call as the generated
     * probes used to do.
     *
     * @returns whether the member is in the metadata, or no value when the
     * question cannot be answered, which is not the same as "no". Metadata is
     * resolved through the package graph and the app directory as well as the
     * Windows metadata directory, so a type outside the Windows namespace is
     * only found when the framework package that declares it is registered for
     * this process. If the type itself is not found, the member being missing
     * from the metadata says nothing about which versions have it.
     */
    std::optional<bool> is_in_metadata(
        py::member_kind kind,
        const char* type_name,
        const char* member_name,
        uint32_t arg_count) noexcept
    {
        if (!type_name || !member_name)
        {
            return {};
        }

        try
        {
            using winrt::Windows::Foundation::Metadata::ApiInformation;

            const auto type = winrt::to_hstring(type_name);

            if (!ApiInformation::IsTypePresent(type))
            {
                return {};
            }

            const auto member = winrt::to_hstring(member_name);

            switch (kind)
            {
            case py::member_kind::method:
                return ApiInformation::IsMethodPresent(type, member, arg_count);
            case py::member_kind::property:
                return ApiInformation::IsPropertyPresent(type, member);
            case py::member_kind::event:
                return ApiInformation::IsEventPresent(type, member);
            }

            return {};
        }
        catch (...)
        {
            // an unusable metadata resolver is not worth reporting instead
            // of the error we were called to report
            return {};
        }
    }

    /**
     * Raises the @c AttributeError that says this version of Windows does not
     * have the named member.
     */
    void set_not_in_this_version_error(
        py::member_kind kind, const char* type_name, const char* member_name) noexcept
    {
        PyErr_Format(
            PyExc_AttributeError,
            "%s '%s.%s' is not available in this version of Windows",
            member_kind_name(kind),
            type_name,
            member_name);
    }

    /**
     * Whether @p hresult is how activation reports that the class or the
     * factory interface that declares a static member does not exist here.
     *
     * These are what C++/WinRT's static path throws when the member is not on
     * this machine, and the only failures that are worth asking the metadata
     * about: anything else came from inside the member and is a real error.
     */
    bool is_activation_failure(int32_t hresult) noexcept
    {
        return hresult == winrt::impl::error_no_interface
               || hresult == winrt::impl::error_class_not_registered
               || hresult == winrt::impl::error_class_not_available;
    }
} // namespace

void py::set_error(py::error_info const& info) noexcept
{
    switch (info.kind)
    {
    case error_kind::hresult:
    {
        // A static member is reached through the activation factory, which is
        // where a member this version of Windows does not have fails. The
        // generated code used to ask ApiInformation before every such call to
        // get a better message than the HRESULT; asking here instead keeps the
        // message and costs nothing until something has already gone wrong.
        if (info.site && (info.site->flags & site_is_static)
            && is_activation_failure(info.hresult))
        {
            const auto in_metadata = is_in_metadata(
                info.site->kind,
                info.site->type_name,
                info.site->member_name,
                info.site->arg_count);

            if (in_metadata.has_value() && !in_metadata.value())
            {
                set_not_in_this_version_error(
                    info.site->kind, info.site->type_name, info.site->member_name);

                return;
            }
        }

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

void py::set_member_not_available_error(py::member_not_available const& info) noexcept
{
    // The interface query answers whether this object implements the member.
    // ApiInformation answers whether this version of Windows has it at all,
    // which is the more useful thing to say when it does not.
    const auto in_metadata
        = is_in_metadata(info.kind, info.type_name, info.member_name, info.arg_count);

    if (in_metadata.has_value() && !in_metadata.value())
    {
        set_not_in_this_version_error(info.kind, info.type_name, info.member_name);

        return;
    }

    PyErr_Format(
        PyExc_AttributeError,
        "%s '%s' requires '%s', which this object does not implement",
        member_kind_name(info.kind),
        info.member_name,
        info.interface_name);
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
