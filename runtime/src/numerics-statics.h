// The constants and the factory functions of the Windows.Foundation.Numerics
// structs.
//
// Vector3.zero and Matrix4x4.make_look_at() are members of the type rather
// than of a value, which in Python means members of the type's type. A type
// cannot be given a metaclass once it exists, so this is what structs.cpp
// makes one with while it is building the type. numerics.h says where the
// members themselves come from.

#pragma once

#include <Python.h>

#include "interp.h"
#include "numerics.h"

namespace py::interp::numerics
{
    bool has_statics(kind which) noexcept;

    PyTypeObject* make_statics(projection& proj, type_entry& entry, kind which);
} // namespace py::interp::numerics
