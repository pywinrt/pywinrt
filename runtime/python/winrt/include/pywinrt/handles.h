// RAII handles for the things the Python C API hands out in pairs: the GIL, the
// thread state, and references to objects and types.
//
// They are winrt::handle_type specializations, so they follow the same shape as
// the C++/WinRT handles the rest of the projection uses.

#pragma once

#include <pywinrt/prelude.h>

namespace py
{
    /**
     * Traits for use with winrt::handle_type.
     */
    struct gil_state_traits
    {
        using type = PyGILState_STATE;

        static void close(type value) noexcept
        {
            PyGILState_Release(value);
        }

        static constexpr type invalid() noexcept
        {
            return static_cast<PyGILState_STATE>(0);
        }
    };

    /**
     * Type alias for Python GIL state handle.
     */
    using gil_handle = winrt::handle_type<gil_state_traits>;

    /**
     * Helper function for ensuring a block of code runs with the Python GIL held.
     */
    static inline auto ensure_gil()
    {
        return gil_handle{PyGILState_Ensure()};
    }

    /**
     * Traits for use with winrt::handle_type.
     */
    struct thread_state_traits
    {
        using type = PyThreadState*;

        static void close(type value) noexcept
        {
            PyEval_RestoreThread(value);
        }

        static constexpr type invalid() noexcept
        {
            return nullptr;
        }
    };

    /**
     * Type alias for Python thread state handle.
     */
    using thread_state_handle = winrt::handle_type<thread_state_traits>;

    /**
     * Helper function for ensuring a block of code runs with the Python GIL released.
     */
    static inline auto release_gil()
    {
        return thread_state_handle{PyEval_SaveThread()};
    }

    struct pyobj_ptr_traits
    {
        using type = PyObject*;

        static void close(type value) noexcept
        {
            Py_CLEAR(value);
        }

        static constexpr type invalid() noexcept
        {
            return nullptr;
        }
    };

    using pyobj_handle = winrt::handle_type<pyobj_ptr_traits>;

    struct pytype_ptr_traits
    {
        using type = PyTypeObject*;

        static void close(type value) noexcept
        {
            Py_CLEAR(value);
        }

        static constexpr type invalid() noexcept
        {
            return nullptr;
        }
    };

    using pytype_handle = winrt::handle_type<pytype_ptr_traits>;
} // namespace py
