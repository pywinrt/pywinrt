#pragma once

#include <Python.h>

#include "interp.h"

#include <atomic>
#include <cstdint>

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

#ifdef Py_GIL_DISABLED
    /**
     * A lock that the thread holding it can take again.
     */
    struct build_mutex
    {
        PyMutex mutex;
        std::atomic<unsigned long> owner;
        uint32_t depth;
    };
#else
    /**
     * Nothing, on a build where only one thread runs Python at a time.
     */
    struct build_mutex
    {
    };
#endif

    /**
     * Holds the lock over building what a projection builds on first use,
     * for a scope.
     *
     * The types a namespace binds are built while its package is imported,
     * which the import system does on one thread at a time. What is built
     * later - a delegate, the instance of a parameterized interface, the
     * vtable Python implements an interface behind, the layout of a call's
     * outputs - is built by whichever thread needs it first, and this is what
     * keeps two of them from building the same thing into the same
     * projection at once.
     *
     * Two rules keep it from deadlocking. A build that needs a type from
     * another package resolves it before taking the lock, so the lock is never
     * held across an import, which would wait for the thread importing that
     * package while that thread waited for the lock. And the thread holding
     * it can take it again, because building a type allocates Python objects,
     * and a collection that runs a finalizer can come back into the runtime.
     *
     * Under the GIL it is nothing, as the state lock is.
     */
    class build_guard
    {
      public:
        build_guard(build_guard const&) = delete;
        build_guard& operator=(build_guard const&) = delete;

#ifdef Py_GIL_DISABLED
        explicit build_guard(build_mutex& mutex) noexcept : mutex_(&mutex)
        {
            auto const self = PyThread_get_thread_ident();

            if (mutex_->owner.load(std::memory_order_relaxed) == self)
            {
                mutex_->depth++;
                return;
            }

            PyMutex_Lock(&mutex_->mutex);
            mutex_->owner.store(self, std::memory_order_relaxed);
            mutex_->depth = 1;
        }

        ~build_guard()
        {
            if (--mutex_->depth != 0)
            {
                return;
            }

            mutex_->owner.store(0, std::memory_order_relaxed);
            PyMutex_Unlock(&mutex_->mutex);
        }

      private:
        build_mutex* mutex_;
#else
        explicit build_guard(build_mutex&) noexcept
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
        /// Guards what the projections build after their packages have been
        /// imported. Taken before the cache lock when both are.
        build_mutex build_lock;
        PyObject* to_uuid_func;
        /// uuid.UUID, which is what a Guid is in Python.
        PyTypeObject* uuid_type;
        PyObject* wrap_async_func;
    };

    module_state* get_module_state() noexcept;
    module_state* try_get_module_state() noexcept;
} // namespace py::cpp::_winrt

namespace py
{
    PyTypeObject* get_inspectable_meta_type() noexcept;
} // namespace py
