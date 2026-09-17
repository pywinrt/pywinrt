// The runtime side of the collections that are backed by a Python object: the
// Python C API calls a wrapped list, dict or iterable makes when WinRT asks it
// for something, and what a Python exception raised during one of them means to
// WinRT.
//
// The class templates themselves stay in the calling module
// (<pywinrt/collections.h>), because the WinRT ABI of IVector<T> passes T by
// value and a runtime that does not know T cannot pass it. What is here is
// everything inside them that does not name T, which is all of the Python work
// and all of the error policy: the module converts one element and calls one
// entry per operation.
//
// The contract these share - the GIL is held by the caller, references are
// borrowed in and new out, failure is an HRESULT - is in <pywinrt/abi.h> with
// the declarations.

#define PYWINRT_RUNTIME_MODULE
#include <pywinrt/base.h>

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

int32_t py::pyseq_insert_at(
    PyObject* sequence, uint32_t index, PyObject* item) noexcept
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

int32_t py::pyseq_append(PyObject* sequence, PyObject* item) noexcept
{
    pyobj_handle result{PyObject_CallMethod(sequence, "append", "O", item)};

    if (!result)
    {
        return python_error(nullptr);
    }

    return ok;
}

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

int32_t py::pyseq_clear(PyObject* sequence) noexcept
{
    if (PySequence_SetSlice(sequence, 0, PY_SSIZE_T_MAX, nullptr) < 0)
    {
        return python_error(nullptr);
    }

    return ok;
}

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

int32_t py::pymap_remove(PyObject* mapping, PyObject* key) noexcept
{
    if (PyObject_DelItem(mapping, key) < 0)
    {
        return python_error(PyExc_KeyError);
    }

    return ok;
}

int32_t py::pymap_clear(PyObject* mapping) noexcept
{
    pyobj_handle result{PyObject_CallMethod(mapping, "clear", nullptr)};

    if (!result)
    {
        return python_error(nullptr);
    }

    return ok;
}

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
