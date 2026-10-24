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

    /**
     * What abi_of() holds while compose_into() is composing the object, which
     * is a call into WinRT that another thread can run during: not a pointer,
     * so that the thread that holds it is the only one composing, and the
     * object holds nothing yet to every other.
     */
    inline void* const composing_abi = reinterpret_cast<void*>(1);

    /**
     * The ABI pointer @p self holds, or @c nullptr when it holds none, which
     * an instance of a Python class derived from a composable one does until
     * its __init__() has called the WinRT class's, and while that call is
     * under way.
     */
    inline void* held_abi(PyObject* self) noexcept
    {
        auto const abi = abi_of(self);
        return abi == composing_abi ? nullptr : abi;
    }

    void set_not_composed_error(PyObject* self) noexcept;

    void set_already_composed_error(PyObject* self) noexcept;

    /**
     * The ABI pointer @p self holds, to call an instance member on, or
     * @c nullptr with a Python error set when it holds none.
     */
    inline void* instance_abi(PyObject* self) noexcept
    {
        auto const abi = held_abi(self);
        if (!abi)
        {
            set_not_composed_error(self);
        }

        return abi;
    }

    bool compose_after_init(PyObject* self, PyObject* args, PyObject* kwds) noexcept;

    /**
     * The name every interface pointer capsule carries. A capsule of this
     * name holds one reference to a COM interface, which its destructor
     * releases, and is how compiled code outside the runtime hands one over.
     */
    inline constexpr char interface_capsule_name[] = "winrt.interface";

    PyObject* new_interface_capsule(void* abi) noexcept;

    PyObject* wrap_abi(PyTypeObject* type, void* abi) noexcept;

    PyObject* wrap_activated_abi(PyTypeObject* type, void const* iid, void* abi);

    void* unwrap_abi(
        PyObject* obj, void const* iid, type_entry* info = nullptr, bool fill = false);

    bool make_interface_type(
        projection& proj, type_entry& entry, table::type_view const& record);

    bool make_class_type(
        projection& proj, type_entry& entry, table::type_view const& record);

    void set_parameterized_type_error(char const* name) noexcept;

    PyObject* deprecated_from(PyObject* cls, PyObject* arg) noexcept;

    PyObject* object_as(PyObject* self, PyObject* arg) noexcept;

    PyObject* iids_of(winrt::Windows::Foundation::IInspectable object) noexcept;

    PyObject* runtime_class_name_of(
        winrt::Windows::Foundation::IInspectable object) noexcept;
} // namespace py::interp
