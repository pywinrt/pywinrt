// What runtime.cpp provides to the rest of the runtime.

#pragma once

#include "prelude.h"

#include <string_view>

namespace py
{
    PyTypeObject* register_python_type(
        PyObject* module,
        PyType_Spec* type_spec,
        PyObject* base_type,
        PyTypeObject* metaclass) noexcept;
    PyTypeObject* get_python_type(std::string_view qualified_name) noexcept;
    bool is_buffer_compatible(
        Py_buffer const& view, Py_ssize_t itemsize, const char* format) noexcept;
    PyObject* convert_datetime(winrt::Windows::Foundation::DateTime value) noexcept;
    winrt::Windows::Foundation::DateTime convert_to_datetime(PyObject* obj);
    PyObject* convert_guid(winrt::guid value) noexcept;
    winrt::guid convert_to_guid(PyObject* obj);

    PyObject* wrap_mapping_iter(PyObject* iter) noexcept;

    bool raise_if_namespace_package(PyObject* module, char const* module_name) noexcept;
} // namespace py
