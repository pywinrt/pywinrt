// A WinRT object as a Python object.
//
// Every projected class and interface is a subclass of _winrt.Object whose
// whole state is one ABI pointer, so getting at that pointer is what this
// header is mostly for. The two make_*_type() functions below are what
// types.cpp calls when a table record turns out to be a class or an interface.

#pragma once

#include <Python.h>

#include "interp.h"

namespace py::interp
{
    /**
     * Where the ABI pointer sits in a wrapper, which is directly after the
     * object header. objects.cpp asserts this against the layout in
     * <pywinrt/wrappers.h>, which is the one that is ABI.
     */
    inline constexpr size_t abi_offset = sizeof(PyObject);

    /**
     * The ABI pointer a wrapped WinRT object holds. The object owns the
     * reference, so a caller that keeps the pointer takes one of its own.
     */
    inline void*& abi_of(PyObject* self) noexcept
    {
        return *reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(self) + abi_offset);
    }

    PyObject* wrap_abi(PyTypeObject* type, void* abi) noexcept;

    void* unwrap_abi(PyObject* obj, void const* iid, type_entry* info = nullptr);

    bool make_interface_type(
        projection& proj, type_entry& entry, table::type_view const& record);

    bool make_class_type(
        projection& proj, type_entry& entry, table::type_view const& record);
} // namespace py::interp
