// Loading a projection table and making the Python types it describes.
//
// This is the entry point a projection package's __init__.py reaches through
// winrt.runtime._internals.load_projection(), and the small set of things the
// three files that build one kind of type each - objects.cpp, structs.cpp and
// members.cpp - need from the one that keeps track of them all.

#pragma once

#include <Python.h>

#include "interp.h"

#include <string>
#include <string_view>
#include <vector>

namespace py::interp
{
    PyObject* load_projection(PyObject* self, PyObject* args) noexcept;

    PyTypeObject* find_registered_type(std::string_view qualified_name) noexcept;

    type_entry* find_registered_entry(std::string_view qualified_name) noexcept;

    type_entry* find_defining_entry(std::string_view qualified_name) noexcept;

    type_entry* get_type_entry(PyTypeObject* type) noexcept;

    type_entry* ensure_entry(projection& proj, uint32_t index) noexcept;

    type_entry* ensure_named_entry(projection& proj, uint32_t index) noexcept;

    PyTypeObject* ensure_type(projection& proj, uint32_t index) noexcept;

    PyTypeObject* ensure_referenced_type(projection& proj, uint32_t index) noexcept;

    bool remember(type_entry& entry, PyTypeObject* type);

    uint32_t align_up(uint32_t value, uint32_t align) noexcept;

    char const* keep(projection& proj, std::string value);

    std::string qualified(table::type_view const& type);

    PyGetSetDef* keep_getsets(projection& proj, std::vector<PyGetSetDef>& defs);

    PyMethodDef* keep_methods(projection& proj, std::vector<PyMethodDef>& defs);
} // namespace py::interp
