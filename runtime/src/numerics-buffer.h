// A Windows.Foundation.Numerics struct as the block of floats it is: the
// buffer of a value and of an array of values, a buffer read as either, and
// the indexing that reads one of the floats. numerics-buffer.cpp says more.

#pragma once

#include <Python.h>

#include "interp.h"
#include "numerics.h"

#include <span>
#include <string_view>
#include <vector>

namespace py::interp::numerics
{
    void add_buffer_slots(kind which, std::vector<PyType_Slot>& slots);

    std::span<Py_ssize_t const> element_shape(kind which) noexcept;

    Py_ssize_t block_count(
        Py_buffer const& view, std::span<Py_ssize_t const> element) noexcept;

    void set_block_error(
        Py_buffer const& view,
        std::wstring_view element,
        std::span<Py_ssize_t const> shape) noexcept;

    bool check_array_source(std::wstring_view element, PyObject* obj) noexcept;

    bool read_buffer(type_entry& info, PyObject* obj, void* out);
} // namespace py::interp::numerics
