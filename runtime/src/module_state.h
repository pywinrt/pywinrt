#pragma once

#include <Python.h>

#include "interp.h"

// internal implementation details for the winrt-runtime module

namespace py::cpp::_winrt
{
    struct module_state
    {
        PyTypeObject* inspectable_meta_type;
        PyTypeObject* object_type;
        PyTypeObject* array_type;
        PyTypeObject* mapping_iter_type;
        /// The descriptor that a projected method is bound as.
        PyTypeObject* projected_method_type;
        std::unordered_map<std::string_view, PyTypeObject*> type_cache;
        std::unordered_map<std::string_view, void*> struct_from_tuple_cache;
        /// The projection tables that have been loaded, by the name of the
        /// module each was loaded into.
        std::unordered_map<std::string, std::unique_ptr<py::interp::projection>>
            projections;
        /// The descriptor a projected type was built from, which is how the
        /// call path gets from the type of a value back to what the table said
        /// about it. One descriptor can be here more than once: an interface
        /// registers both its wrapper and its abstract type, and a class with
        /// statics registers its metaclass as well.
        std::unordered_map<PyTypeObject*, py::interp::type_entry*> type_entries;
        /// The Python type of each parameterized interface a projection has
        /// closed, by the WinRT signature of the instance. Borrowed: the
        /// entry that built one owns it, and they are let go of together.
        std::unordered_map<std::string_view, PyTypeObject*> generic_types;
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
