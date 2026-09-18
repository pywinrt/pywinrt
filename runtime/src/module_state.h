#pragma once

#include <Python.h>

#include "interp.h"

// internal implementation details for the winrt-runtime module

namespace py::cpp::_winrt
{
#ifdef Py_GIL_DISABLED
    using state_mutex = PyMutex;
#else
    /**
     * Nothing, on a build where only one thread runs Python at a time.
     */
    struct state_mutex
    {
    };
#endif

    /**
     * Holds the lock over the caches in the module state for a scope.
     *
     * Free-threading is what this is for. Under the GIL the maps below are
     * already safe - every one of them is reached with the GIL held - so the
     * guard is an empty object that compiles away, and the lock an empty
     * struct that takes no room in the state. On a free-threaded build it is a
     * PyMutex, which is one byte and unlocked when zero, so a state that
     * CPython hands out zeroed needs no initialization.
     *
     * One lock covers every cache, and the rule that makes that safe is that a
     * critical section is one map operation and never a call that can run
     * Python: building a type imports modules and calls back in here, and a
     * PyMutex is not recursive. So a lookup that misses releases the lock,
     * does the work, and takes the lock again to record the answer - which
     * means two threads can do the same work, and whichever gets there second
     * drops its own result and takes the one already in the map.
     */
    class state_guard
    {
      public:
        state_guard(state_guard const&) = delete;
        state_guard& operator=(state_guard const&) = delete;

#ifdef Py_GIL_DISABLED
        explicit state_guard(state_mutex& mutex) noexcept : mutex_(&mutex)
        {
            PyMutex_Lock(mutex_);
        }

        ~state_guard()
        {
            PyMutex_Unlock(mutex_);
        }

      private:
        state_mutex* mutex_;
#else
        explicit state_guard(state_mutex&) noexcept
        {
        }
#endif
    };

    struct module_state
    {
        PyTypeObject* inspectable_meta_type;
        PyTypeObject* object_type;
        PyTypeObject* array_type;
        PyTypeObject* mapping_iter_type;
        /// The descriptor that a projected method is bound as.
        PyTypeObject* projected_method_type;
        /// Guards every map below, which are the only members that are written
        /// after the state is built.
        state_mutex cache_lock;
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
        /// The Python type of each concrete parameterized interface, by the
        /// WinRT signature of the instance. Borrowed: the entry that built one
        /// owns it, and they are let go of together.
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
     * down. Two interpreters starting on two threads is the one way that can
     * happen at once, so it is written through std::atomic_ref. It stays a
     * plain @c uint64_t because the capsule hands a module its address and the
     * module reads it directly, which an aligned 64-bit load does in one
     * instruction on every architecture this is built for.
     */
    extern uint64_t type_registry_epoch;
} // namespace py::cpp::_winrt
