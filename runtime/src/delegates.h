// A Python callable as a WinRT delegate.
//
// A delegate is the reverse of every other call the interpreter makes: WinRT
// enters a vtable and Python answers. The trampoline that catches the call is
// in shapes.cpp and what it makes of the arguments is in callbacks.cpp; what
// is here is the COM object whose one vtable entry it is.
//
// A delegate is also the one record in a table that no Python type stands for.
// __init__.py binds a typing alias to its name - a delegate is a callable and
// nothing more, as far as Python is concerned - so the type registry has
// nothing to hand back for one, and the call path asks for the table entry
// instead.

#pragma once

#include <Python.h>

#include "interp.h"
#include "table.h"

namespace py::interp
{
    bool make_delegate_type(
        projection& proj, type_entry& entry, table::type_view const& record);

    void* make_delegate(type_entry& info, PyObject* callable) noexcept;
} // namespace py::interp
