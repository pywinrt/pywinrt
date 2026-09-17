// Failures, in both directions: the errors the projection raises itself, and
// the C++ and WinRT exceptions it has to turn into a Python exception on the way
// out of a method call or a callback.
//
// py::python_exception is the marker thrown once a Python error is already set,
// so the unwinding path does not have to carry the error with it, and
// py::to_PyErr() is the catch-all that every projected method ends with.
//
// Which Python exception any of this becomes is decided by winrt-runtime rather
// than here: this header classifies what it caught into one of the PODs in
// <pywinrt/abi.h> and hands it over. A module carries its own copy of these
// headers, so anything it decides for itself is frozen into it at build time,
// and the wording of an error is the kind of thing that should be able to
// improve in a runtime release without rebuilding four hundred packages.

#pragma once

#include <pywinrt/abi.h>
#include <pywinrt/handles.h>
#include <pywinrt/prelude.h>

namespace py
{
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

    inline WINRT_IMPL_NOINLINE void set_invalid_activation_error(
        const char* const type_name) noexcept
    {
        const member_site site{
            member_kind::method, 0, type_name, nullptr, nullptr, site_is_constructor};

        set_call_error(call_error::cannot_instantiate, &site, 0);
    }

    inline WINRT_IMPL_NOINLINE void set_invalid_arg_count_error(
        Py_ssize_t arg_count) noexcept
    {
        set_call_error(call_error::invalid_arg_count, nullptr, arg_count);
    }

    inline WINRT_IMPL_NOINLINE void set_invalid_kwd_args_error() noexcept
    {
        set_call_error(call_error::keyword_arguments, nullptr, 0);
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

        // The exception object lives until its handler ends, and everything an
        // error_info points at belongs to it, so each of these has to make the
        // call from inside its own catch block rather than after the try.
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
            const auto restricted = e.try_as<winrt::impl::IRestrictedErrorInfo>();

            error_info info{};
            info.kind = error_kind::hresult;
            info.hresult = e.code().value;
            info.message = message.c_str();
            info.message_length = message.size();
            info.restricted_error_info = restricted.get();
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
        throw winrt::hresult_error(
            winrt::hresult{report_unraisable()}, L"Unraisable Python exception");
    }
} // namespace py
