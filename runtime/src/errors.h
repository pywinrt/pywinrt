// Failures, in both directions: the errors the projection raises itself, and
// the C++ and WinRT exceptions it has to turn into a Python exception on the way
// out of a method call or a callback.
//
// py::python_exception is the marker thrown once a Python error is already set,
// so the unwinding path does not have to carry the error with it, and
// py::to_PyErr() is the catch-all that every projected method ends with.
//
// Which Python exception any of this becomes is decided in errors.cpp rather
// than here: this header classifies what it caught into one of the PODs below
// and hands it to py::set_error() or py::set_member_not_available_error().

#pragma once

#include "prelude.h"

namespace py
{
    /**
     * The kind of member a Python attribute is projected from.
     */
    enum class member_kind
    {
        method,
        property,
        event,
    };

    /**
     * Thrown when an object does not implement the interface that declares the
     * member that is being called.
     *
     * The names point at strings that the loaded projection keeps, so they
     * outlive the exception without having to be copied.
     */
    struct member_not_available
    {
        member_kind kind;
        uint32_t arg_count;
        /// Metadata name of the type that declares the member or @c nullptr for
        /// a parameterized interface, which has no name in the metadata.
        const char* type_name;
        const char* member_name;
        /// Metadata name of the interface that was queried for.
        const char* interface_name;
    };

    /// The member is reached without an instance, so a failure to reach it
    /// cannot be blamed on the object.
    inline constexpr uint32_t site_is_static = 1;
    /// The member is a constructor.
    inline constexpr uint32_t site_is_constructor = 2;

    /**
     * Identifies the projected member a call was made through, so that the
     * error can say which one failed.
     *
     * Every field is either a scalar or a pointer to a string that outlives
     * the call. An instance of this is made where the call fails and passed
     * by pointer.
     */
    struct member_site
    {
        member_kind kind;
        /// Number of WinRT input parameters, which is what distinguishes the
        /// overloads of a method from each other.
        uint32_t arg_count;
        /// Metadata name of the type that declares the member or @c nullptr for
        /// a parameterized interface, which has no name in the metadata.
        const char* type_name;
        const char* member_name;
        /// Metadata name of the interface that declares the member, or
        /// @c nullptr when it is not reached through one.
        const char* interface_name;
        /// Zero or more of the @c site_is_* flags above.
        uint32_t flags;
    };

    /**
     * What py::to_PyErr() caught, which decides which Python exception
     * py::set_error() raises.
     */
    enum class error_kind
    {
        /// A C++/WinRT @c hresult_error. Only this kind fills in @c hresult
        /// and @c message.
        hresult,
        bad_alloc,
        out_of_range,
        invalid_argument,
        /// Any other @c std::exception.
        std_exception,
        /// Something that is not a @c std::exception at all.
        unknown,
    };

    /**
     * A C++ exception, flattened by py::to_PyErr() inside its catch block and
     * read by py::set_error() for the duration of the call.
     *
     * @c message is a local of the catch block that fills it in, and @c what
     * belongs to the exception being handled, so neither outlives the call.
     * py::set_error() copies what it keeps.
     */
    struct error_info
    {
        error_kind kind;
        /// The @c HRESULT, for @c error_kind::hresult.
        int32_t hresult;
        /// The error message, not necessarily null-terminated.
        const wchar_t* message;
        /// Length of @c message in characters.
        uint32_t message_length;
        /// @c std::exception::what(), or @c nullptr for @c error_kind::unknown.
        const char* what;
        /// The member that was being called, or @c nullptr when it is not known.
        member_site const* site;
    };

    /**
     * A call that never reached WinRT at all, because of how it was made.
     */
    enum class call_error
    {
        /// No overload takes this many arguments.
        invalid_arg_count,
        /// Keyword arguments were passed to a member that takes none.
        keyword_arguments,
        /// The type cannot be constructed from Python.
        cannot_instantiate,
    };

    void set_member_not_available_error(member_not_available const& info) noexcept;
    void set_error(error_info const& info) noexcept;
    PyObject* take_raised_exception() noexcept;
    void restore_raised_exception(PyObject* exception) noexcept;
    void set_call_error(call_error error, member_site const* site) noexcept;
    int32_t report_unraisable(PyObject* source) noexcept;

    /**
     * The @c HRESULT that py::report_unraisable() returns, and therefore the
     * one that a Python exception escaping a WinRT callback is reported to
     * WinRT as.
     */
    const winrt::hresult unraisable_python_exception{static_cast<int32_t>(0xA0EE4005)};

    /**
     * Thrown when a Python exception is pending (i.e. PyErr_Occurred() returns
     * non-NULL).
     */
    struct python_exception
    {
    };

    void set_out_of_range_error(PyObject* obj, const char* type) noexcept;

    inline WINRT_IMPL_NOINLINE void set_invalid_activation_error(
        const char* const type_name) noexcept
    {
        const member_site site{
            member_kind::method, 0, type_name, nullptr, nullptr, site_is_constructor};

        set_call_error(call_error::cannot_instantiate, &site);
    }

    inline WINRT_IMPL_NOINLINE void set_invalid_arg_count_error() noexcept
    {
        set_call_error(call_error::invalid_arg_count, nullptr);
    }

    inline WINRT_IMPL_NOINLINE void set_invalid_kwd_args_error() noexcept
    {
        set_call_error(call_error::keyword_arguments, nullptr);
    }

    [[noreturn]] inline WINRT_IMPL_NOINLINE void throw_member_not_available(
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
     * Sets a Python exception for the C++ exception currently being handled.
     *
     * Must only be called from a catch block. @p site names the member that was
     * being called, when the caller knows it.
     */
    inline WINRT_IMPL_NOINLINE void to_PyErr(member_site const* site = nullptr) noexcept
    {
        if (PyErr_Occurred())
        {
            // A Python exception is already pending - don't overwrite it
            return;
        }

        // An error_info's message is a local of the catch block that fills it
        // in, and its what() belongs to the exception being handled, so each
        // of these makes the call from inside its own catch block rather than
        // after the try.
        auto set_std_error = [site](error_kind kind, std::exception const& e) noexcept
        {
            error_info info{};
            info.kind = kind;
            info.what = e.what();
            info.site = site;

            set_error(info);
        };

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
            const auto message = e.message();

            error_info info{};
            info.kind = error_kind::hresult;
            info.hresult = e.code().value;
            info.message = message.c_str();
            info.message_length = message.size();
            info.site = site;

            set_error(info);
        }
        catch (std::bad_alloc const& e)
        {
            set_std_error(error_kind::bad_alloc, e);
        }
        catch (std::out_of_range const& e)
        {
            set_std_error(error_kind::out_of_range, e);
        }
        catch (std::invalid_argument const& e)
        {
            set_std_error(error_kind::invalid_argument, e);
        }
        catch (std::exception const& e)
        {
            set_std_error(error_kind::std_exception, e);
        }
        catch (...)
        {
            // Whatever it is, it must not leave this noexcept function: a
            // rethrow with nothing left to catch it terminates the process.
            error_info info{};
            info.kind = error_kind::unknown;
            info.site = site;

            set_error(info);
        }
    }

    /**
     * Throws if @p obj is null, and otherwise hands it back, so that the check
     * can stand in front of something that takes the object - a member
     * initializer, say - rather than only being a statement of its own.
     */
    inline PyObject* throw_if_pyobj_null(PyObject* obj)
    {
        if (!obj)
        {
            if (!PyErr_Occurred())
            {
                PyErr_SetString(PyExc_SystemError, "object is null");
            }
            throw python_exception();
        }

        return obj;
    }

    /**
     * Throws what WinRT sees in place of a Python exception that has already
     * been reported as unraisable, @p hr being what py::report_unraisable()
     * returned for it.
     *
     * The message says no more than that there was one, because the exception
     * itself has gone to sys.unraisablehook by the time this is thrown.
     */
    [[noreturn]] inline void throw_unraisable(winrt::hresult hr)
    {
        throw winrt::hresult_error(hr, L"Unraisable Python exception");
    }

    /**
     * Reports the pending Python exception as unraisable and throws, for a
     * callback that WinRT called and that therefore has nowhere to raise it.
     * @p source is the Python object WinRT was calling, which the report names.
     */
    [[noreturn]] inline void write_unraisable_and_throw(PyObject* source)
    {
        throw_unraisable(winrt::hresult{report_unraisable(source)});
    }

    /**
     * Throws if one of the Python collection entries in collections.h failed.
     *
     * @throws winrt::hresult_error
     */
    inline void check_python_result(int32_t hr)
    {
        if (hr < 0)
        {
            if (hr == unraisable_python_exception)
            {
                // The runtime has already reported it, so the only thing left
                // to do with it is to name it.
                throw_unraisable(winrt::hresult{hr});
            }

            winrt::throw_hresult(hr);
        }
    }
} // namespace py

namespace py::cpp::_winrt
{
    PyObject* hresult_error(PyObject* /*unused*/, PyObject* args) noexcept;
} // namespace py::cpp::_winrt
