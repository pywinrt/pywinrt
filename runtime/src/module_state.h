#pragma once

#include <Python.h>

// internal implementation details for the winrt-runtime module

namespace py::cpp::_winrt
{
    struct module_state
    {
        PyTypeObject* inspectable_meta_type;
        PyTypeObject* object_type;
        PyTypeObject* array_type;
        PyTypeObject* mapping_iter_type;
        std::unordered_map<std::string_view, PyTypeObject*> type_cache;
        std::unordered_map<std::string_view, void*> struct_from_tuple_cache;
        PyObject* to_uuid_func;
        PyObject* wrap_async_func;
    };

    module_state* get_module_state() noexcept;

    /**
     * The counter behind py::get_type_registry_epoch().
     *
     * It is not part of the module state on purpose: what it tells a module is
     * that the state it memoized something out of is gone, so it has to
     * outlive every state, and it is bumped whenever one is created or torn
     * down.
     */
    extern uint64_t type_registry_epoch;
} // namespace py::cpp::_winrt
