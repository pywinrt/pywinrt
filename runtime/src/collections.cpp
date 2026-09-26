// The runtime side of the collections that are backed by a Python object: the
// Python C API calls a wrapped list, dict or iterable makes when WinRT asks it
// for something, and what a Python exception raised during one of them means to
// WinRT.
//
// A Python list or dict passed to a WinRT method that takes an IVector<T> or an
// IMap<K, V> is wrapped rather than copied, so WinRT calls back into Python for
// every element operation it makes. pysequence.cpp and pymapping.cpp convert
// the elements; what is here is everything that does not name T, which is all
// of the Python work and all of the error policy, one function per operation.
//
// All of them:
//
//  - are called with the GIL held. The caller takes it once, because WinRT may
//    call on any thread, and nothing here takes it again.
//  - borrow the PyObject* they are given and return new references through
//    their out parameters, which are written on success only.
//  - return an HRESULT, because they are called from COM methods that cannot
//    throw. A Python IndexError or KeyError means the collection has no such
//    index or key and becomes E_BOUNDS, which is what WinRT expects from
//    GetAt() or Lookup(). Nothing on the WinRT side could catch any other
//    Python exception, so it is reported with report_unraisable() and returned
//    as that HRESULT.

#include <pywinrt/base.h>

#include "collections.h"

namespace
{
    /// The HRESULT of a member that did what it was asked.
    constexpr int32_t ok = 0;

    /**
     * Turns the pending Python exception into the HRESULT to return.
     *
     * @p not_found_type is the exception the Python object raises when it does
     * not have the index or the key that was asked for - IndexError for a
     * sequence, KeyError for a mapping - which is the one failure WinRT has a
     * name for. Anything else is a genuine error in Python code that WinRT
     * cannot be told about, so it goes to sys.unraisablehook instead.
     */
    int32_t python_error(PyObject* not_found_type) noexcept
    {
        if (not_found_type && PyErr_ExceptionMatches(not_found_type))
        {
            PyErr_Clear();

            return winrt::impl::error_out_of_bounds;
        }

        return py::report_unraisable();
    }

    /**
     * Converts a WinRT index to a Python one.
     *
     * Py_ssize_t is signed, and a negative index means one counted from the
     * end of the sequence, so an index that does not fit has to be refused
     * rather than passed on. This only happens on a 32-bit build, where
     * Py_ssize_t is narrower than the uint32_t of the WinRT index; no sequence
     * that large can exist either way.
     */
    bool to_index(uint32_t index, Py_ssize_t& result) noexcept
    {
        result = static_cast<Py_ssize_t>(index);

        return result >= 0;
    }
} // namespace

/// Number of items in @p sequence, for IVector<T>::Size().
int32_t py::pyseq_size(PyObject* sequence, uint32_t* size) noexcept
{
    const auto length = PySequence_Size(sequence);

    if (length < 0)
    {
        return python_error(nullptr);
    }

    *size = static_cast<uint32_t>(length);

    return ok;
}

/// The item at @p index, for IVector<T>::GetAt().
int32_t py::pyseq_get_at(PyObject* sequence, uint32_t index, PyObject** item) noexcept
{
    Py_ssize_t i;

    if (!to_index(index, i))
    {
        return winrt::impl::error_out_of_bounds;
    }

    auto value = PySequence_GetItem(sequence, i);

    if (!value)
    {
        return python_error(PyExc_IndexError);
    }

    *item = value;

    return ok;
}

/// Replaces the item at @p index, for IVector<T>::SetAt().
int32_t py::pyseq_set_at(PyObject* sequence, uint32_t index, PyObject* item) noexcept
{
    Py_ssize_t i;

    if (!to_index(index, i))
    {
        return winrt::impl::error_out_of_bounds;
    }

    if (PySequence_SetItem(sequence, i, item) < 0)
    {
        return python_error(PyExc_IndexError);
    }

    return ok;
}

/// Inserts @p item before @p index, for IVector<T>::InsertAt().
int32_t py::pyseq_insert_at(PyObject* sequence, uint32_t index, PyObject* item) noexcept
{
    Py_ssize_t i;

    if (!to_index(index, i))
    {
        return winrt::impl::error_out_of_bounds;
    }

    // Not PySequence_SetItem(), which replaces. insert() is the sequence
    // protocol's only way to make room, and it clamps an index past the end
    // rather than failing, which is what a Python caller of the same list
    // would see.
    pyobj_handle result{PyObject_CallMethod(sequence, "insert", "nO", i, item)};

    if (!result)
    {
        return python_error(PyExc_IndexError);
    }

    return ok;
}

/// Removes the item at @p index, for IVector<T>::RemoveAt().
int32_t py::pyseq_remove_at(PyObject* sequence, uint32_t index) noexcept
{
    Py_ssize_t i;

    if (!to_index(index, i))
    {
        return winrt::impl::error_out_of_bounds;
    }

    if (PySequence_DelItem(sequence, i) < 0)
    {
        return python_error(PyExc_IndexError);
    }

    return ok;
}

/// Adds @p item to the end, for IVector<T>::Append().
int32_t py::pyseq_append(PyObject* sequence, PyObject* item) noexcept
{
    pyobj_handle result{PyObject_CallMethod(sequence, "append", "O", item)};

    if (!result)
    {
        return python_error(nullptr);
    }

    return ok;
}

/// Removes the last item, for IVector<T>::RemoveAtEnd().
int32_t py::pyseq_remove_at_end(PyObject* sequence) noexcept
{
    const auto length = PySequence_Size(sequence);

    if (length < 0)
    {
        return python_error(nullptr);
    }

    if (length == 0)
    {
        // IVector<T>::RemoveAtEnd() on an empty vector is out of bounds, and
        // asking the sequence to delete item -1 would take one off the end
        // instead of saying so.
        return winrt::impl::error_out_of_bounds;
    }

    if (PySequence_DelItem(sequence, length - 1) < 0)
    {
        return python_error(PyExc_IndexError);
    }

    return ok;
}

/// Finds @p item, for IVector<T>::IndexOf(). Not finding it is a success
/// with @p found false, since a WinRT IndexOf() reports it that way.
int32_t py::pyseq_index_of(
    PyObject* sequence, PyObject* item, uint32_t* index, bool* found) noexcept
{
    const auto i = PySequence_Index(sequence, item);

    if (i < 0)
    {
        if (PyErr_ExceptionMatches(PyExc_ValueError))
        {
            // Not being there is an answer, not a failure: IVector<T>::IndexOf()
            // reports it by returning false.
            PyErr_Clear();

            *found = false;

            return ok;
        }

        return python_error(nullptr);
    }

    *index = static_cast<uint32_t>(i);
    *found = true;

    return ok;
}

/// Removes every item, for IVector<T>::Clear().
int32_t py::pyseq_clear(PyObject* sequence) noexcept
{
    if (PySequence_SetSlice(sequence, 0, PY_SSIZE_T_MAX, nullptr) < 0)
    {
        return python_error(nullptr);
    }

    return ok;
}

/// Starts an iteration of @p iterable, for IIterable<T>::First().
int32_t py::pyiter_first(PyObject* iterable, PyObject** iterator) noexcept
{
    auto iter = PyObject_GetIter(iterable);

    if (!iter)
    {
        return python_error(nullptr);
    }

    *iterator = iter;

    return ok;
}

/// Advances @p iterator, for IIterator<T>::MoveNext(). The end of the
/// iteration is a success with a null @p item.
int32_t py::pyiter_next(PyObject* iterator, PyObject** item) noexcept
{
    auto next = PyIter_Next(iterator);

    if (!next)
    {
        if (PyErr_Occurred())
        {
            return python_error(nullptr);
        }

        // The end of the iteration, which the caller sees as a null item.
        return ok;
    }

    *item = next;

    return ok;
}

/// Number of entries in @p mapping, for IMap<K, V>::Size().
int32_t py::pymap_size(PyObject* mapping, uint32_t* size) noexcept
{
    const auto length = PyMapping_Size(mapping);

    if (length < 0)
    {
        return python_error(nullptr);
    }

    *size = static_cast<uint32_t>(length);

    return ok;
}

/// The value @p key maps to, for IMap<K, V>::Lookup().
int32_t py::pymap_lookup(PyObject* mapping, PyObject* key, PyObject** value) noexcept
{
    auto item = PyObject_GetItem(mapping, key);

    if (!item)
    {
        return python_error(PyExc_KeyError);
    }

    *value = item;

    return ok;
}

/// Whether @p key is in @p mapping, for IMap<K, V>::HasKey().
int32_t py::pymap_has_key(PyObject* mapping, PyObject* key, bool* has_key) noexcept
{
    const auto result = PyMapping_HasKeyWithError(mapping, key);

    if (result < 0)
    {
        return python_error(nullptr);
    }

    *has_key = result != 0;

    return ok;
}

/// Maps @p key to @p value, for IMap<K, V>::Insert(), which reports
/// through @p replaced whether the key was already there.
int32_t py::pymap_insert(
    PyObject* mapping, PyObject* key, PyObject* value, bool* replaced) noexcept
{
    // IMap<K, V>::Insert() reports whether it replaced an entry, which the
    // mapping protocol has no way to say, so it has to be asked first.
    const auto had_key = PyMapping_HasKeyWithError(mapping, key);

    if (had_key < 0)
    {
        return python_error(nullptr);
    }

    if (PyObject_SetItem(mapping, key, value) < 0)
    {
        return python_error(nullptr);
    }

    *replaced = had_key != 0;

    return ok;
}

/// Removes @p key, for IMap<K, V>::Remove().
int32_t py::pymap_remove(PyObject* mapping, PyObject* key) noexcept
{
    if (PyObject_DelItem(mapping, key) < 0)
    {
        return python_error(PyExc_KeyError);
    }

    return ok;
}

/// Removes every entry, for IMap<K, V>::Clear().
int32_t py::pymap_clear(PyObject* mapping) noexcept
{
    pyobj_handle result{PyObject_CallMethod(mapping, "clear", nullptr)};

    if (!result)
    {
        return python_error(nullptr);
    }

    return ok;
}

/// Advances @p iterator over the keys of @p mapping and looks the value
/// up, for the IKeyValuePair<K, V> iterator of a mapping. The end of the
/// iteration is a success with a null @p key and @p value.
int32_t py::pymap_iter_next(
    PyObject* mapping, PyObject* iterator, PyObject** key, PyObject** value) noexcept
{
    pyobj_handle next_key{PyIter_Next(iterator)};

    if (!next_key)
    {
        if (PyErr_Occurred())
        {
            return python_error(nullptr);
        }

        // The end of the iteration, which the caller sees as a null key.
        return ok;
    }

    // A key the mapping's own iterator yielded but does not have is a broken
    // mapping rather than a lookup that missed, so this is not E_BOUNDS.
    pyobj_handle next_value{PyObject_GetItem(mapping, next_key.get())};

    if (!next_value)
    {
        return python_error(nullptr);
    }

    *key = next_key.detach();
    *value = next_value.detach();

    return ok;
}
