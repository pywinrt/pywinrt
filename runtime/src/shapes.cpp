// The one translation unit that instantiates the call trampolines.
//
// shapes-generated.h is a census of every ABI call shape in the metadata the
// tree projects, and including it emits one trampoline per shape. That is a
// few hundred small functions, so it is compiled once, here, and everything
// else reaches them through the two accessors below.

#include <Python.h>

#define PYWINRT_RUNTIME_MODULE
#include <pywinrt/base.h>

#include "interp.h"
#include "shapes-generated.h"

namespace py::interp
{
    /**
     * The trampoline and buffer layout of a forward call shape, or @c nullptr
     * with a Python error set if this runtime's census does not have it.
     */
    shapes::shape_desc const* get_forward_shape(uint32_t id) noexcept
    {
        if (id >= std::size(shapes::forward_shapes))
        {
            PyErr_Format(
                PyExc_NotImplementedError,
                "this winrt-runtime was built with %zu call shapes and the table asks for number %u; "
                "the projection package needs a newer winrt-runtime",
                std::size(shapes::forward_shapes),
                id);
            return nullptr;
        }

        return &shapes::forward_shapes[id];
    }

    /// One past the highest forward shape id this runtime was built with.
    uint32_t forward_shape_count() noexcept
    {
        return static_cast<uint32_t>(std::size(shapes::forward_shapes));
    }

    /// One past the highest reverse shape id this runtime was built with.
    uint32_t reverse_shape_count() noexcept
    {
        return static_cast<uint32_t>(std::size(shapes::reverse_shapes));
    }
} // namespace py::interp

namespace py::shapes
{
    /**
     * Where a call from WinRT into Python arrives.
     *
     * Nothing implements a WinRT interface in Python yet - delegates, events and
     * implemented interfaces are the next step - so every reverse trampoline
     * that the census instantiated lands here and says so. The entries exist
     * already because they are what makes a vtable, and a vtable is built
     * before anything can be called through it.
     */
    int32_t reverse_dispatch(void* /*self*/, uint16_t /*slot*/, void* /*args*/) noexcept
    {
        return winrt::impl::error_not_implemented;
    }
} // namespace py::shapes
