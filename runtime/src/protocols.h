// The Python protocols a projected type implements.
//
// A WinRT collection is a Python sequence or a Python mapping, an IIterator is
// a Python iterator, an IStringable has a str() and an async operation can be
// awaited and waited on. None of that is anything WinRT says: the table only
// records which of the well known interfaces a type implements, and every one
// of them comes down to calling ordinary members - len(v) is Size, v[i] is
// GetAt(i), iter(m) is First().
//
// So this is where the slots that CPython calls are, and where the members
// behind them are found once, when the type is built. It is the same code for a
// class that implements IMap<String, String> and for the IMap<String, String>
// interface itself, because by the time it runs both are a descriptor and a
// pointer.

#pragma once

#include <Python.h>

#include "interp.h"

#include <vector>

namespace py::interp
{
    void find_protocol_members(type_entry& entry) noexcept;

    void add_protocol_slots(
        table::type_view const& record, std::vector<PyType_Slot>& slots);

    bool bind_protocol_methods(table::type_view const& record, PyTypeObject* type);
} // namespace py::interp
