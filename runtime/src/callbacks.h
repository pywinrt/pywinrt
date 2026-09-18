// A call that arrives from WinRT, made into a Python one.
//
// This is interp.cpp read backwards. There, a Python call is turned into a
// WinRT one from a member's descriptor; here the same descriptor turns a WinRT
// call into a Python one: the arguments a reverse trampoline spilled into a
// buffer are converted the other way round, a Python operation is run, and
// what it hands back is stored where the outputs point.
//
// Which operation that is depends on what is behind the vtable. A delegate
// calls the object it holds; an interface a Python object implements calls one
// of its methods, or reads or writes one of its attributes - which is the only
// thing delegates.cpp and implements.cpp differ in once the call has arrived.

#pragma once

#include <Python.h>

#include "interp.h"

namespace py::interp
{
    PyObject* convert_borrowed(
        projection& owner, arg_desc& arg, void* storage) noexcept;

    void convert_to_abi(
        projection& owner, arg_desc& arg, PyObject* value, void* storage);

    int32_t call_python(
        member_desc const& member,
        overload_desc& overload,
        PyObject* target,
        python_op op,
        void* args) noexcept;
} // namespace py::interp
