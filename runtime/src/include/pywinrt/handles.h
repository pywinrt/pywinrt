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
     * Holds the Python GIL for as long as it is in scope.
     *
     * This one is not a winrt::handle_type like the rest of the file. That
     * model needs one value of the handle type to stand for "holding nothing",
     * and PyGILState_STATE has none to spare: PyGILState_Ensure() returns
     * whether the calling thread already held the GIL, and both answers have to
     * be handed back to PyGILState_Release(). PyGILState_LOCKED does not unlock
     * anything, but it still balances the counter on the thread state, which is
     * what decides when the state of a thread Python did not create is
     * destroyed. Since PyGILState_LOCKED is zero, spelling it as the empty
     * value is also spelling it as the one case that must not be skipped.
     */
    struct gil_guard
    {
        gil_guard() noexcept : m_state{PyGILState_Ensure()}
        {
        }

        gil_guard(gil_guard const&) = delete;
        gil_guard& operator=(gil_guard const&) = delete;

        ~gil_guard() noexcept
        {
            PyGILState_Release(m_state);
        }

      private:
        PyGILState_STATE m_state;
    };

    /**
     * Helper function for ensuring a block of code runs with the Python GIL held.
     */
    [[nodiscard]] static inline gil_guard ensure_gil()
    {
        return {};
    }

    /**
     * Traits for use with winrt::handle_type.
     *
     * Unlike the GIL above, this one does fit the model: PyEval_SaveThread()
     * always returns the thread state it detached, never a null pointer, so
     * null is free to mean "holding nothing".
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

    /**
     * Holds a Py_buffer for as long as it is in scope.
     *
     * Like the GIL above, this one is not a winrt::handle_type: a Py_buffer is
     * a struct the caller owns rather than a pointer with a spare value to
     * stand for "holding nothing", and PyBuffer_Release() is given its address.
     *
     * The buffer is requested by the constructor, with the PyBUF_ @p flags, so
     * check the object before reading anything from it: when it is false, the
     * object could not supply one and a Python error is set.
     */
    struct buffer_view
    {
        buffer_view(PyObject* obj, int flags) noexcept
            : m_valid{PyObject_GetBuffer(obj, &m_view, flags) == 0}
        {
        }

        buffer_view(buffer_view const&) = delete;
        buffer_view& operator=(buffer_view const&) = delete;

        ~buffer_view() noexcept
        {
            // a no-op when the request failed, since the view is still zeroed
            PyBuffer_Release(&m_view);
        }

        explicit operator bool() const noexcept
        {
            return m_valid;
        }

        /** The view itself, for the fields the accessors below do not cover. */
        Py_buffer const& view() const noexcept
        {
            return m_view;
        }

        void* data() const noexcept
        {
            return m_view.buf;
        }

        size_t size() const noexcept
        {
            return static_cast<size_t>(m_view.len);
        }

      private:
        Py_buffer m_view{};
        bool m_valid;
    };

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
