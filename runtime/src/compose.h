// A Python subclass of a composable WinRT class.
//
// A composable class is one WinRT allows to be derived from, and deriving from
// it in Python makes a pair of objects: the Python one, and a WinRT one that
// answers on its behalf. That WinRT one is the outer object - it is what the
// class is composed into, it holds the inner object the class was composed
// over, and it answers the overridable interfaces from Python and everything
// else from the inner.
//
// implements.cpp is the other half of this. A Python object that merely
// implements interfaces is the same COM object with the same tearoffs, no
// inner, and a reference that only goes one way; here the two objects refer to
// each other, so the reference between them toggles and the pair stays
// collectable as a cycle while WinRT is not holding on to it.

#pragma once

#include <Python.h>

#include "interp.h"

namespace py::interp
{
    PyObject* make_composed_object(
        PyTypeObject* type,
        type_entry& entry,
        member_desc const& constructor,
        overload_desc& overload,
        PyObject* const* args,
        Py_ssize_t nargs) noexcept;

    void* composable_inner(void* abi) noexcept;
} // namespace py::interp
