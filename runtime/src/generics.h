// The parameterized interfaces, closed over the types a namespace uses them
// with.
//
// IVector<T> is one WinRT interface with one set of members, but there is no
// such thing as a value of it: what a member passes is IVector<String> or
// IVector<Uri>, each with an IID of its own and an ABI of its own, because T is
// passed by value. The generated projection compiled a C++ class template per
// instance for that reason. A table says the same thing as data - an instance
// is a type record whose members are the definition's with every type argument
// filled in - so what is left here is to make a Python type of one, and to do
// it once per instance rather than once per package that names it.
//
// IReference<T> is the exception that proves it: it is a parameterized
// interface the projection never shows anyone, because a Python value either is
// the value it boxes or is None.

#pragma once

#include <Python.h>

#include "interp.h"

#include <string_view>

namespace py::interp
{
    bool ensure_instance_type(
        projection& proj, type_entry& entry, table::type_view const& record);

    PyObject* reference_to_python(type_entry& info, void* abi) noexcept;

    void* reference_from_python(type_entry& info, PyObject* value);

    PyTypeObject* find_type_by_signature(std::string_view signature) noexcept;
} // namespace py::interp
