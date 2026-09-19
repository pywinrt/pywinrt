// A call that arrives from WinRT, made into a Python one.
//
// This is interp.cpp read backwards. There, a Python call is turned into a
// WinRT one from a member's descriptor; here the same descriptor turns a WinRT
// call into a Python one: the arguments a reverse trampoline spilled into a
// buffer are converted the other way round, a Python operation is run, and
// what it hands back is stored where the outputs point.
//
// Which operation that is depends on what is behind the vtable. A delegate
// calls the object it holds; an interface a Python object implements calls one
// of its methods, or reads or writes one of its attributes - which is the only
// thing delegates.cpp and implements.cpp differ in once the call has arrived.
//
// The two halves either side of that operation - reading the arguments and
// writing the outputs - are the same whatever it is, so they are declared here
// as well: pycollections.cpp runs a Python sequence or mapping operation
// between them rather than a call on the object itself.

#pragma once

#include <Python.h>

#include "interp.h"

namespace py::interp
{
    /**
     * How many Python arguments one call from WinRT can have. The census says
     * the widest member of any metadata the tree projects takes seventeen.
     */
    inline constexpr uint16_t max_args = 20;

    /**
     * How many arrays one call from WinRT can lend a Python implementation.
     * The census says the widest member of any metadata the tree projects
     * passes or lends three.
     */
    inline constexpr uint16_t max_arrays = 4;

    struct table_array;

    /**
     * The Python arguments of one call, given back however it ends.
     */
    struct python_args
    {
        python_args() = default;
        python_args(python_args const&) = delete;
        python_args& operator=(python_args const&) = delete;

        ~python_args();

        void add_lent(table_array* array);

        PyObject* values[max_args]{};
        uint16_t count{};
        /// The arrays a WinRT caller lent, which stop being anything the
        /// moment this call returns. Filled in as they are lent, so only the
        /// count needs initialising.
        table_array* lent[max_arrays];
        uint16_t lent_count{};
    };

    PyObject* convert_borrowed(
        projection& owner, arg_desc& arg, void* storage) noexcept;

    void convert_to_abi(
        projection& owner, arg_desc& arg, PyObject* value, void* storage);

    void read_inputs(
        member_desc const& member,
        overload_desc& overload,
        uint8_t* buffer,
        python_args& in);

    void write_outputs(
        member_desc const& member,
        overload_desc& overload,
        PyObject* result,
        uint8_t* buffer);

    int32_t call_python(
        member_desc const& member,
        overload_desc& overload,
        PyObject* target,
        python_op op,
        void* args) noexcept;
} // namespace py::interp
