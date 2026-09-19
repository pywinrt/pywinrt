// A WinRT enum, as the Python enum class the table describes.
//
// An enum is the one projected type that is nothing but data: it has no
// interface, no members and no ABI of its own beyond the thirty-two bits its
// constants occupy. So there is no wrapper type to build here and no
// conversion to write - a value of one is an ordinary Python int, which the
// interpreter makes by calling the class with it - and all this does is read
// the constants out of the table and hand them to the standard library.

#pragma once

#include <Python.h>

#include "interp.h"

namespace py::interp
{
    bool make_enum_type(
        projection& proj, type_entry& entry, table::type_view const& record);
} // namespace py::interp
