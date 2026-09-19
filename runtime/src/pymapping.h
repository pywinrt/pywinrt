// A Python mapping, seen from WinRT as an IMap<K, V> or an IMapView<K, V>.
//
// WinRT iterates a map as key-value pairs where Python iterates one as keys,
// so a mapping needs what a sequence does not: an iterator that looks each
// key's value up as it goes, and an IKeyValuePair<K, V> of its own to hand
// back. Both are here, and both read the mapping itself rather than a copy of
// it.
//
// pycollections.h has the object all of these are carried in and what one call
// comes down to; pysequence.h is the same for a Python sequence.

#pragma once

#include <Python.h>

#include "pycollections.h"

#include <memory>

namespace py::interp
{
    std::unique_ptr<collection_calls> make_mapping_calls(PyObject* mapping);

    std::unique_ptr<collection_calls> make_mapping_iterator_calls(PyObject* mapping);
} // namespace py::interp
