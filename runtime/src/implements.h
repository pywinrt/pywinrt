// A Python object that implements WinRT interfaces.
//
// The public name of every projected interface is bound to an abstract type
// rather than to the wrapper - winrt.windows.foundation.IStringable, not the
// _IStringable a WinRT value of it comes back as - and a Python class that
// derives from one is saying it implements that interface. What is here is
// what such an object is on the WinRT side: one COM object with a tearoff per
// interface, whose vtables are assembled from the same table records the
// forward path calls through.
//
// compose.cpp is the other half of this. A Python subclass of a composable
// WinRT class is one of these with a WinRT object of its own inside it, and
// the tearoffs are the same tearoffs.

#pragma once

#include <Python.h>

#include "interp.h"

namespace py::interp
{
    bool implements_interfaces(PyTypeObject* type) noexcept;

    void* make_implements_object(PyObject* obj, void const* iid);
} // namespace py::interp
