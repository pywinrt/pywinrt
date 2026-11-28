// Interface pointer capsules, which are how compiled code outside the runtime
// exchanges WinRT objects with it: winrt._winrt.as_interface() and the
// functions beside it, which winrt.runtime.interop exports.

#pragma once

#include <Python.h>

namespace py::interp
{
    /**
     * The name every interface pointer capsule carries. A capsule of this
     * name holds one reference to a COM interface, which its destructor
     * releases, and is how compiled code outside the runtime hands one over.
     */
    inline constexpr char interface_capsule_name[] = "winrt.interface";

    PyObject* new_interface_capsule(void* abi) noexcept;
} // namespace py::interp

namespace py::cpp::_winrt
{
    PyObject* as_interface(PyObject* /*unused*/, PyObject* args) noexcept;

    PyObject* get_activation_factory(PyObject* /*unused*/, PyObject* args) noexcept;

    PyObject* wrap_interface(PyObject* /*unused*/, PyObject* args) noexcept;

    PyObject* interface_pointer(PyObject* /*unused*/, PyObject* obj) noexcept;
} // namespace py::cpp::_winrt
