// The blocking half of the async projection - obj.get() and obj.wait(timeout) -
// as the calling module sees it.
//
// The wait itself is in winrt-runtime, behind py::async_wait() in
// <pywinrt/abi.h>: an event, a completed handler with its own vtable and the
// wait, none of which depends on the type arguments of the async interface, and
// all of which C++/WinRT would otherwise inline into every caller.
//
// What is left here is the part that has to stay with the caller: the check
// that the thread may block at all, the conversion of a Python timeout, and
// turning what the waiter returns into the exception a failed call raises. The
// runtime's waiter touches no Python at all, which is what lets the generated
// code release the GIL once around both the wait and the GetResults() that
// follows it.

#pragma once

#include <pywinrt/abi.h>
#include <pywinrt/errors.h>
#include <pywinrt/prelude.h>

namespace py
{
    /// The timeout that waits for as long as it takes.
    inline constexpr uint32_t async_wait_forever = 0xFFFFFFFF;

    /**
     * Sets a Python error and returns @c true when the calling thread must not
     * block, which is any thread in a single-threaded apartment: the operation
     * being waited for may need that very thread to dispatch a call before it
     * can finish, so waiting for it there deadlocks.
     *
     * The GIL must be held.
     */
    inline bool set_sta_blocking_wait_error() noexcept
    {
        if (!winrt::impl::is_sta_thread())
        {
            return false;
        }

        PyErr_SetString(
            PyExc_RuntimeError,
            "Cannot call blocking method from single-threaded apartment.");

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
     * py::async_wait() means: it is the @c HRESULT of the failure, since the
     * four @c AsyncStatus values are not negative.
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
