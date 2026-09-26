// The one translation unit that instantiates the call trampolines.
//
// shapes-generated.h is a census of every ABI call shape in the metadata the
// tree projects, and including it emits one trampoline per shape. That is a
// few hundred small functions, so it is compiled once, here, and everything
// else reaches them through the two accessors below.

#include <Python.h>

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

    /**
     * The vtable entry a WinRT caller enters for a (shape, slot) pair, or
     * @c nullptr with a Python error set if this runtime's census does not
     * have it.
     */
    shapes::reverse_desc const* get_reverse_shape(uint32_t id) noexcept
    {
        if (id >= std::size(shapes::reverse_shapes))
        {
            PyErr_Format(
                PyExc_NotImplementedError,
                "this winrt-runtime was built with %zu callback shapes and the table asks for number %u; "
                "the projection package needs a newer winrt-runtime",
                std::size(shapes::reverse_shapes),
                id);
            return nullptr;
        }

        return &shapes::reverse_shapes[id];
    }

    /// One past the highest reverse shape id this runtime was built with.
    uint32_t reverse_shape_count() noexcept
    {
        return static_cast<uint32_t>(std::size(shapes::reverse_shapes));
    }

    /// Which census these trampolines were instantiated from.
    std::string_view census_lineage() noexcept
    {
        return shapes::census_lineage;
    }

    /// How many times ids had been appended to that census.
    uint32_t census_revision() noexcept
    {
        return shapes::census_revision;
    }
} // namespace py::interp

namespace py::shapes
{
    /**
     * Where a call from WinRT into Python arrives.
     *
     * Every reverse trampoline the census instantiated lands here, whatever
     * its shape and whatever object it was called on, so all this does is find
     * that object and hand the call to it. What the object makes of the slot
     * and the spilled arguments is delegates.cpp's business.
     */
    int32_t reverse_dispatch(void* self, uint16_t slot, void* args) noexcept
    {
        return static_cast<com_head*>(self)->target->invoke(slot, args);
    }
} // namespace py::shapes
