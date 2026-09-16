// The runtime side of a Python subclass of a composable WinRT type.
//
// A composable object is a pair - the Python object and the WinRT object that
// answers calls on its behalf - and the reference between them is a toggle
// rather than a plain count, so that the pair stays collectable as a cycle
// while WinRT is not holding on to it. The bookkeeping is the same for every
// composable type in every module, so <pywinrt/compose.h> forwards it here
// instead of compiling a copy of it into each one.

#define PYWINRT_RUNTIME_MODULE
#include <pywinrt/base.h>

void py::toggle_python_reference(PyObject* obj, bool is_last_reference) noexcept
{
    auto state = PyGILState_Ensure();

    if (is_last_reference)
    {
        // We hold the only WinRT reference - allow the Python object to be GC'd
        PyObject_GC_Track(obj);
        // This might be the last reference to the Python object, so obj may be
        // destroyed after this call and no longer valid!
        Py_DECREF(obj);
    }
    else
    {
        // external WinRT code has a reference - don't allow Python object to be
        // GC'd
        Py_INCREF(obj);
        PyObject_GC_UnTrack(obj);
    }

    PyGILState_Release(state);
}
