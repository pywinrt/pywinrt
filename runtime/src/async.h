// The blocking wait, as the interpreter asks for it.
//
// py::async_wait() in <pywinrt/abi.h> is what a compiled module calls: the
// module knows the async interface at compile time, so the callback that hands
// the object its completed handler is a captureless lambda and needs nothing
// passed to it. The interpreter knows the interface only as a descriptor, so it
// needs that descriptor back when the waiter calls in, which is the one thing
// the capsule entry cannot carry.
//
// Both go through the same waiter in async.cpp. Nothing here is part of the C
// ABI: the interpreter and the waiter are compiled into the same module.

#pragma once

#include <cstdint>

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
