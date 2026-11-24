// The runtime side of the async projection: the blocking wait behind obj.get()
// and obj.wait(timeout), and the hand-off of an async object to asyncio.
//
// C++/WinRT implements a blocking wait as a template over the async interface:
// a whole waiter - an event, a delegate object with its own vtable,
// QueryInterface, AddRef and Release, and the wait itself - for each async
// interface it is used with. The interpreter knows an async interface only
// from a table, so it has no C++ type to instantiate that template with.
//
// Nothing in that waiter depends on the type arguments. Every WinRT completed
// handler passes the async object and an AsyncStatus as two machine words, so
// they all share one ABI and differ only in the IID that identifies them. So
// the waiter is written here once, without templates, and the caller passes in
// the two things that do depend on the type arguments: the IID of its completed
// handler and a function that hands the handler to the object.
//
// py::interp::async_wait_for() touches no Python at all - it reports a failure
// as an HRESULT and leaves the raising to its caller - so the caller can
// release the GIL around the wait.

#include "base.h"

#include "async.h"
#include "module_state.h"

namespace
{
    /**
     * The ABI of a WinRT async completed handler.
     *
     * @c AsyncActionCompletedHandler, @c AsyncOperationCompletedHandler<T> and
     * their with-progress relatives all look like this and differ only in the
     * IID that identifies them, which is what lets one implementation stand in
     * for every one of them.
     */
    struct WINRT_IMPL_NOVTABLE async_completed_abi : winrt::impl::unknown_abi
    {
        virtual int32_t __stdcall Invoke(void* async, int32_t status) noexcept = 0;
    };

    /**
     * An async completed handler that sets an event, for a blocking wait.
     *
     * It answers @c QueryInterface for the IID it is constructed with. The rest
     * of the COM identity - @c IUnknown, @c IAgileObject and @c IMarshal - is
     * C++/WinRT's own, so this is as agile as the delegate it replaces.
     */
    struct async_completed_event final : async_completed_abi,
                                         winrt::impl::implements_delegate_base,
                                         winrt::impl::update_module_lock
    {
        explicit async_completed_event(winrt::guid const& iid) : m_iid(iid)
        {
        }

        int32_t __stdcall QueryInterface(
            winrt::guid const& id, void** result) noexcept final
        {
            return static_cast<int32_t>(query_interface(
                id, result, static_cast<async_completed_abi*>(this), m_iid));
        }

        uint32_t __stdcall AddRef() noexcept final
        {
            return increment_reference();
        }

        uint32_t __stdcall Release() noexcept final
        {
            auto const remaining = decrement_reference();

            if (remaining == 0)
            {
                delete this;
            }

            return remaining;
        }

        int32_t __stdcall Invoke(void* /*async*/, int32_t status) noexcept final
        {
            m_status.store(status, std::memory_order_release);
            WINRT_VERIFY(::WINRT_IMPL_SetEvent(m_event.get()));
            return 0;
        }

        /**
         * Waits up to @p timeout_ms milliseconds for the handler to be invoked
         * and returns the status it saw, which is still
         * @c AsyncStatus::Started if the wait timed out.
         *
         * The GIL must not be held.
         */
        winrt::Windows::Foundation::AsyncStatus wait(uint32_t timeout_ms) noexcept
        {
            ::WINRT_IMPL_WaitForSingleObject(m_event.get(), timeout_ms);

            return static_cast<winrt::Windows::Foundation::AsyncStatus>(
                m_status.load(std::memory_order_acquire));
        }

      private:
        winrt::guid m_iid;
        winrt::handle m_event{winrt::check_pointer(
            ::WINRT_IMPL_CreateEventW(nullptr, true, false, nullptr))};
        std::atomic<int32_t> m_status{
            static_cast<int32_t>(winrt::Windows::Foundation::AsyncStatus::Started)};
    };

} // namespace

/**
 * Waits up to @p timeout_ms milliseconds for @p async to finish.
 *
 * The GIL must not be held: this blocks, and the completed handler is invoked
 * on whichever thread the operation finishes on.
 *
 * @returns The @c AsyncStatus the wait saw, which is @c AsyncStatus::Started
 * if it timed out, or a failed @c HRESULT if the wait itself could not be set
 * up. The four status values are not negative, so the two cannot be confused.
 */
int32_t py::interp::async_wait_for(
    void* async,
    uint32_t timeout_ms,
    void const* handler_iid,
    set_completed_fn set_completed,
    void* context) noexcept
{
    try
    {
        winrt::Windows::Foundation::IInspectable object;
        winrt::copy_from_abi(object, async);

        auto const info = object.as<winrt::Windows::Foundation::IAsyncInfo>();
        auto const status = info.Status();

        if (status != winrt::Windows::Foundation::AsyncStatus::Started)
        {
            // Nothing to wait for, so no event and no handler either. This is
            // the whole of get() on an operation that has already finished.
            return static_cast<int32_t>(status);
        }

        winrt::com_ptr<async_completed_event> handler;
        handler.attach(
            new async_completed_event(*static_cast<winrt::guid const*>(handler_iid)));

        // The async object takes its own reference, so the handler outlives a
        // wait that returns before the operation is done.
        winrt::check_hresult(set_completed(
            context, async, static_cast<async_completed_abi*>(handler.get())));

        return static_cast<int32_t>(handler->wait(timeout_ms));
    }
    catch (...)
    {
        // The four AsyncStatus values are not negative, so a failed HRESULT
        // cannot be mistaken for one. to_hresult() also leaves the error info
        // on the thread for the caller's check_hresult() to build a message
        // from, which is why this does not need the GIL to report anything.
        return winrt::to_hresult();
    }
}

/**
 * What __await__ of an async operation returns: the iterator of the coroutine
 * winrt.runtime._internals.wrap_async() makes for it, which is imported the
 * first time an operation is awaited.
 */
PyObject* py::await_async(PyObject* obj) noexcept
{
    auto state = py::cpp::_winrt::get_module_state();
    if (!state)
    {
        return nullptr;
    }

    // lazy import to avoid circular import issues
    auto wrap_async = py::interp::load_published(state->wrap_async_func);

    if (!wrap_async)
    {
        pyobj_handle internals{PyImport_ImportModule("winrt.runtime._internals")};
        if (!internals)
        {
            return nullptr;
        }

        pyobj_handle wrap_async_func{
            PyObject_GetAttrString(internals.get(), "wrap_async")};
        if (!wrap_async_func)
        {
            return nullptr;
        }

        // Two threads can both have imported it, and the state keeps whichever
        // got there first.
        PyObject* expected{};

        if (std::atomic_ref<PyObject*>{state->wrap_async_func}.compare_exchange_strong(
                expected, wrap_async_func.get(), std::memory_order_acq_rel))
        {
            wrap_async = wrap_async_func.detach();
        }
        else
        {
            wrap_async = expected;
        }
    }

    pyobj_handle coroutine{PyObject_CallOneArg(wrap_async, obj)};
    if (!coroutine)
    {
        return nullptr;
    }

    pyobj_handle await_str{PyUnicode_InternFromString("__await__")};
    if (!await_str)
    {
        return nullptr;
    }

    return PyObject_CallMethodNoArgs(coroutine.get(), await_str.get());
}
