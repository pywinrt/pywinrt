// The Python C API calls behind a collection that is backed by a Python object.
//
// Each of these is one IVector<T>, IIterable<T> or IMap<K, V> operation, done
// on the Python list, dict or iterable a WinRT collection wraps, with what a
// Python exception raised during it means to WinRT. pysequence.cpp and
// pymapping.cpp call them; collections.cpp implements them and states the
// contract they share.

#pragma once

#include <Python.h>

#include <cstdint>

namespace py
{
    int32_t pyseq_size(PyObject* sequence, uint32_t* size) noexcept;
    int32_t pyseq_get_at(PyObject* sequence, uint32_t index, PyObject** item) noexcept;
    int32_t pyseq_set_at(PyObject* sequence, uint32_t index, PyObject* item) noexcept;
    int32_t pyseq_insert_at(
        PyObject* sequence, uint32_t index, PyObject* item) noexcept;
    int32_t pyseq_remove_at(PyObject* sequence, uint32_t index) noexcept;
    int32_t pyseq_append(PyObject* sequence, PyObject* item) noexcept;
    int32_t pyseq_remove_at_end(PyObject* sequence) noexcept;
    int32_t pyseq_index_of(
        PyObject* sequence, PyObject* item, uint32_t* index, bool* found) noexcept;
    int32_t pyseq_clear(PyObject* sequence) noexcept;
    int32_t pyiter_first(PyObject* iterable, PyObject** iterator) noexcept;
    int32_t pyiter_next(PyObject* iterator, PyObject** item) noexcept;
    int32_t pymap_size(PyObject* mapping, uint32_t* size) noexcept;
    int32_t pymap_lookup(PyObject* mapping, PyObject* key, PyObject** value) noexcept;
    int32_t pymap_has_key(PyObject* mapping, PyObject* key, bool* has_key) noexcept;
    int32_t pymap_insert(
        PyObject* mapping, PyObject* key, PyObject* value, bool* replaced) noexcept;
    int32_t pymap_remove(PyObject* mapping, PyObject* key) noexcept;
    int32_t pymap_clear(PyObject* mapping) noexcept;
    int32_t pymap_iter_next(
        PyObject* mapping,
        PyObject* iterator,
        PyObject** key,
        PyObject** value) noexcept;
} // namespace py
