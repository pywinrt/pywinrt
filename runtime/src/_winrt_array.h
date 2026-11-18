// winrt.system.Array, the Python type of a WinRT array whose elements the
// array owns.

#pragma once

#include <Python.h>

#include <pywinrt/base.h>

#include <memory>

namespace py::cpp::_winrt
{
    extern PyType_Spec Array_type_spec;

    bool Array_Assign(PyObject* obj, std::unique_ptr<py::Array> array) noexcept;

    py::Array* Array_Get(PyObject* obj) noexcept;
} // namespace py::cpp::_winrt
