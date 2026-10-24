// The part of handles.h that is not inline: the thread states of threads
// Python has not seen.
//
// WinRT calls back on threads of its own: the thread pool, the thread of a
// DispatcherQueue made with CreateOnDedicatedThread(), an AudioGraph's audio
// thread. Attaching from such a thread creates a thread state for it, and the
// matching release deletes it again, so on its own whatever Python keeps per
// thread state would last for one callback: a threading.local, the running
// asyncio loop, the contextvars context. It would also cost a thread state
// made and cleared on every callback.
//
// So the first attach on such a thread makes the thread state with
// PyGILState_Ensure() and keeps that count of it, and every attach after finds
// it and reuses it. The count is given back when the thread exits, from a
// fiber-local storage callback, and the last release clears and deletes the
// thread state as it would have: nothing is left behind. Unlike a C++
// thread_local destructor or DLL_THREAD_DETACH, Windows calls FLS callbacks on
// the exiting thread before it takes the loader lock, so clearing the thread
// state there, which can run any finalizer, is safe.
//
// The thread state is made by PyGILState_Ensure() rather than by
// PyThreadState_Ensure*() even from 3.15, because a thread state made that way
// is deleted when its count runs out, and PyThreadState_EnsureFromView()
// would hold an interpreter guard for as long as the count stayed up, which
// keeps the interpreter from ever finalizing. PyThreadState_Ensure*() reuses
// the thread state PyGILState_Ensure() bound to the thread, and lets go of
// its guard each time.

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>

#include <pywinrt/handles.h>

namespace
{
    /**
     * Gives back the count keep_thread_state() kept, on the thread that is
     * exiting; the release that takes the count to zero clears and deletes
     * the thread state.
     */
    void NTAPI on_thread_exit(void*) noexcept
    {
#if PY_VERSION_HEX >= 0x030F0000
        PyInterpreterView* const view = py::main_interpreter_view();
        PyInterpreterGuard* const guard
            = view ? PyInterpreterGuard_FromView(view) : nullptr;

        // Without a guard, the interpreter is finalizing or gone, and deletes
        // the thread states of all its threads itself.
        if (!guard)
        {
            return;
        }
#else
        if (!Py_IsInitialized() || Py_IsFinalizing())
        {
            return;
        }
#endif

        // An interpreter initialized again since is not the one the thread
        // state was kept in, and this thread has none in it.
        if (PyGILState_GetThisThreadState())
        {
            PyGILState_STATE const state = PyGILState_Ensure();
            // The kept count; the GIL stays held for the release below.
            PyGILState_Release(PyGILState_LOCKED);
            PyGILState_Release(state);
        }

#if PY_VERSION_HEX >= 0x030F0000
        PyInterpreterGuard_Close(guard);
#endif
    }

    /**
     * The FLS slot whose callback gives the thread state back, or
     * FLS_OUT_OF_INDEXES. It is never freed: FlsFree() would call the callback
     * for every thread at once, on the thread that frees it, and the module is
     * never unloaded.
     */
    DWORD fls_index() noexcept
    {
        static DWORD const index = FlsAlloc(on_thread_exit);
        return index;
    }

    /**
     * Any value but null, which FLS takes to mean "nothing to clean up".
     */
    void* const kept = reinterpret_cast<void*>(1);
} // namespace

namespace py
{
#if PY_VERSION_HEX >= 0x030F0000
    /**
     * The main interpreter, for attaching to it from a thread Python has not
     * seen; @c nullptr if out of memory.
     */
    PyInterpreterView* main_interpreter_view() noexcept
    {
        // A view does not keep the interpreter from finalizing, so the one
        // taken on first use serves for as long as the process runs.
        static PyInterpreterView* const view = PyInterpreterView_FromMain();
        return view;
    }
#endif

    /**
     * Gives the calling thread, which has no Python thread state, one that
     * lasts until the thread exits, so that what Python keeps per thread -
     * threading.local, the running asyncio loop, the contextvars context -
     * carries over from one WinRT callback to the next. Without it, the thread
     * state that attaching creates is deleted again when the callback returns.
     * Call it without the GIL and before attaching.
     */
    void keep_thread_state() noexcept
    {
        DWORD const index = fls_index();

        if (index == FLS_OUT_OF_INDEXES)
        {
            return; // each callback makes its own thread state
        }

#if PY_VERSION_HEX >= 0x030F0000
        PyInterpreterView* const view = main_interpreter_view();
        PyInterpreterGuard* const guard
            = view ? PyInterpreterGuard_FromView(view) : nullptr;

        if (!guard)
        {
            return; // finalizing: the attach that follows fails too
        }
#endif

        // Makes the thread state, binds it to the thread and attaches it.
        PyGILState_STATE const state = PyGILState_Ensure();

        if (FlsSetValue(index, kept))
        {
            // Detaches, keeping the count.
            PyEval_SaveThread();
        }
        else
        {
            // Deletes the thread state again.
            PyGILState_Release(state);
        }

#if PY_VERSION_HEX >= 0x030F0000
        PyInterpreterGuard_Close(guard);
#endif
    }
} // namespace py
