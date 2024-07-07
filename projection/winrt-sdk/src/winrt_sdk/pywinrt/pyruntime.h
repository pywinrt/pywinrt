#pragma once

#include <Python.h>

// internal implementation details for the winrt-runtime module

namespace py::cpp::_winrt
{
    struct module_state
    {
        PyTypeObject* object_type;
        PyTypeObject* array_type;
        PyTypeObject* mapping_iter_type;
    };

    module_state* get_module_state() noexcept;
} // namespace py::cpp::_winrt
