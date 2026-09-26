// A WinRT array, as a Python object and as a pair of ABI arguments.
//
// WinRT spells an array as two arguments - a count and a pointer to the
// elements - and says in three different ways who owns them: the caller passes
// elements to be read, lends storage to be filled, or receives storage the
// callee allocated. A table records which of the three a parameter is, so the
// interpreter has to be able to do all three knowing only the element's type
// code.
//
// That is what is here. Elements a caller passes or lends are borrowed from
// any Python object that exports a buffer of the right shape, which is what
// the generated projection did too, except that elements holding references
// are only taken from a winrt.system.Array of the same element type; elements
// a call hands back become a winrt.system.Array whose element type is a type
// code rather than a C++ type, which is the one thing py::ComArray<T> in
// <pywinrt/array.h> cannot be.
//
// A parameter's own descriptor is what describes its elements - the type code
// and type of an array parameter are the element's - so everything below takes
// the arg_desc that interp.h already built.

#pragma once

#include <Python.h>

#include "interp.h"

namespace py::interp
{
    /**
     * Where the count and the elements of an array output are received.
     *
     * A call lays one of these out in the block beside its arguments and
     * points the parameter's two ABI slots at the members, which is what a
     * C++ caller does with a winrt::com_array.
     */
    struct array_out
    {
        uint32_t count;
        void* data;
    };

    bool borrow_array(
        projection& owner,
        arg_desc& element,
        PyObject* obj,
        bool writable,
        Py_buffer* view) noexcept;

    bool borrow_array_argument(
        projection& owner,
        arg_desc& arg,
        PyObject* value,
        Py_buffer* view,
        uint8_t* args) noexcept;

    void point_at_array_output(
        arg_desc const& arg, array_out& received, uint8_t* args) noexcept;

    PyObject* array_take_python(
        projection& owner, arg_desc& element, array_out& received) noexcept;

    PyObject* array_to_python(
        projection& owner,
        arg_desc& element,
        uint32_t count,
        void const* data) noexcept;

    /**
     * The array behind a winrt.system.Array that the interpreter made.
     *
     * Opaque here because only a lent array has to be named again: it points
     * at memory a WinRT caller owns, so it has to be taken back before the
     * call it was lent for returns.
     */
    struct table_array;

    PyObject* array_lend_python(
        projection& owner,
        arg_desc& element,
        uint32_t count,
        void* data,
        table_array*& lent) noexcept;

    void take_back_array(table_array& lent) noexcept;

    PyObject* array_argument_to_python(
        projection& owner,
        arg_desc& arg,
        uint8_t const* args,
        table_array*& lent) noexcept;

    void array_result_to_abi(
        projection& owner, arg_desc& arg, PyObject* value, uint8_t* args);

    void array_from_python(
        projection& owner, arg_desc& element, PyObject* value, array_out& out);

    void release_array(arg_desc const& element, array_out& received) noexcept;

    PyObject* new_array(projection& owner, arg_desc& element, uint32_t count) noexcept;

    PyObject* type_assign_array(PyObject* cls, PyObject* arg) noexcept;
} // namespace py::interp
