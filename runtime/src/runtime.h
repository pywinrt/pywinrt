// What runtime.cpp provides to the rest of the runtime beyond the entry points
// it implements for <pywinrt/abi.h>.

#pragma once

#include <Python.h>

namespace py
{
    PyObject* wrap_mapping_iter(PyObject* iter) noexcept;

    bool raise_if_namespace_package(PyObject* module, char const* module_name) noexcept;
} // namespace py
