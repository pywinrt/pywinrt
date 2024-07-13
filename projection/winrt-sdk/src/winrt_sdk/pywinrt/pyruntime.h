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
        std::unordered_map<std::string_view, PyTypeObject*> type_cache;
        PyObject* to_uuid_func;
    };

    module_state* get_module_state() noexcept;
} // namespace py::cpp::_winrt
