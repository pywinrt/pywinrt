// A WinRT struct, as a value and as a Python type.
//
// A struct is the only value whose layout the runtime works out for itself,
// because the table says what the fields are and never where they sit. What
// that costs is that a struct value is not a blit: the strings and interface
// pointers in one have to be given back when the last copy of it goes away and
// duplicated when a copy is made, which is what the pairs below are for.
// structs.cpp says which of them to reach for when.

#pragma once

#include <Python.h>

#include "interp.h"

namespace py::interp
{
    void release_string(void* abi) noexcept;

    void* duplicate_string(void* abi);

    void release_struct(type_entry const& info, void* blob) noexcept;

    void copy_struct_resources(type_entry const& info, void* blob);

    void struct_from_python(type_entry& info, PyObject* obj, void* out);

    PyObject* struct_to_python(type_entry& info, void const* blob) noexcept;

    PyObject* struct_take_python(type_entry& info, void* blob) noexcept;

    bool make_struct_type(
        projection& proj, type_entry& entry, table::type_view const& record);
} // namespace py::interp
