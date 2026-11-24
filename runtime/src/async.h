// The blocking wait of obj.get() and obj.wait(timeout), as the interpreter asks
// for it, and what surrounds it: the check that the thread may block at all,
// the conversion of a Python timeout, and turning what the waiter returns into
// the exception a failed call raises.
//
// The interpreter knows the async interface only as a descriptor, so it needs
// that descriptor back when the waiter in async.cpp calls in. Nothing here is
// part of the C ABI: the interpreter and the waiter are compiled into the same
// module.
//
// The hand-off of an async object to asyncio, which is what __await__ is, is
// here too.

#pragma once

#include <Python.h>

#include <pywinrt/prelude.h>

#include <cstdint>

namespace py
{
    PyObject* await_async(PyObject* obj) noexcept;

    /// The timeout that waits for as long as it takes.
    inline constexpr uint32_t async_wait_forever = 0xFFFFFFFF;

    /**
     * Sets a Python error and returns @c true when the calling thread must not
     * block, which is any thread in a single-threaded apartment: the operation
     * being waited for may need that very thread to dispatch a call before it
     * can finish, so waiting for it there deadlocks.
     *
     * @p operation is the name the error gives the blocking call, such as
     * @c "get()".
     *
     * The GIL must be held.
     */
    inline bool set_sta_blocking_wait_error(const char* operation) noexcept
    {
        if (!winrt::impl::is_sta_thread())
        {
            return false;
        }

        PyErr_Format(
            PyExc_RuntimeError,
            "%s cannot block a single-threaded apartment",
            operation);

        return true;
    }

    /**
     * Converts the timeout of @c wait() to the milliseconds the waiter takes.
     *
     * A timeout is a duration, so anything that is not a positive number of
     * seconds - including a NaN, which fails every comparison - asks for the
     * status as it stands rather than for a wait, and anything too large to
     * count in milliseconds is as good as no timeout at all.
     */
    inline uint32_t async_timeout_ms(double timeout_seconds) noexcept
    {
        if (!(timeout_seconds > 0.0))
        {
            return 0;
        }

        constexpr double max_seconds = (async_wait_forever - 1) / 1000.0;

        return timeout_seconds >= max_seconds
                   ? async_wait_forever - 1
                   : static_cast<uint32_t>(timeout_seconds * 1000.0);
    }

    /**
     * Throws if the wait itself failed, which is what a negative return of
     * py::interp::async_wait_for() means: it is the @c HRESULT of the failure,
     * since the four @c AsyncStatus values are not negative.
     *
     * @throws winrt::hresult_error
     */
    inline void check_async_wait(int32_t status)
    {
        if (status < 0)
        {
            winrt::check_hresult(status);
        }
    }

    /**
     * The same check for @c get(), which unlike @c wait() also treats a
     * canceled operation as a failure rather than as a status to report.
     *
     * @throws winrt::hresult_error, winrt::hresult_canceled
     */
    inline void check_async_get(int32_t status)
    {
        check_async_wait(status);

        // Reported from here rather than by GetResults(), which is not
        // required to fail for a canceled operation. This is C++/WinRT's own
        // helper, so get() raises what its get() does.
        winrt::impl::check_status_canceled(
            static_cast<winrt::Windows::Foundation::AsyncStatus>(status));
    }
} // namespace py

namespace py::interp
{
    /**
     * Hands @p async its completed handler.
     *
     * @param context Whatever was passed alongside, which for the interpreter
     * is the descriptor of the member to call.
     * @param async The async object, as an @c IInspectable.
     * @param handler The completed handler, as an @c IUnknown.
     * @returns Zero, or the @c HRESULT of the failure.
     */
    using set_completed_fn
        = int32_t (*)(void* context, void* async, void* handler) noexcept;

    int32_t async_wait_for(
        void* async,
        uint32_t timeout_ms,
        void const* handler_iid,
        set_completed_fn set_completed,
        void* context) noexcept;
} // namespace py::interp
