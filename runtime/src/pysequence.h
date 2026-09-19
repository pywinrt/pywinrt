// A Python sequence, seen from WinRT as an IVector<T> or an IVectorView<T>.
//
// Which of the two it is asked to be makes no difference here: a vector view
// is a vector with fewer members, and neither of them is a copy of the Python
// object - the sequence protocol is run on the object itself, so a WinRT
// caller reading the collection reads whatever the Python side has put in it
// by then. Any Python iterable at all is also an IIterable<T> this way, whose
// one member hands back the iterator below.
//
// pycollections.h has the object all of these are carried in and what one call
// comes down to; pymapping.h is the same for a Python mapping.

#pragma once

#include <Python.h>

#include "pycollections.h"

#include <memory>

namespace py::interp
{
    std::unique_ptr<collection_calls> make_sequence_calls(PyObject* sequence);

    std::unique_ptr<collection_calls> make_sequence_iterator_calls(PyObject* iterator);
} // namespace py::interp
