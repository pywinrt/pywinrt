// What a projected member is as an attribute of the type it belongs to.
//
// Building a type is two passes, because CPython wants a type's getset array
// before the type exists and its methods only after: collect_members() turns a
// table's groups into descriptors and hands back what each pass needs, and
// bind_methods() runs the second one.

#pragma once

#include <Python.h>

#include "interp.h"

#include <utility>
#include <vector>

namespace py::interp
{
    extern PyType_Spec projected_method_type_spec;

    /**
     * What one type's members come to before any of it exists as a Python type:
     * the getset arrays a type spec needs up front, and the methods that are
     * set as attributes once it does.
     */
    struct type_members
    {
        std::vector<PyGetSetDef> instance_getsets;
        std::vector<PyGetSetDef> static_getsets;
        /// Each method and whether it belongs on the metaclass.
        std::vector<std::pair<member_desc*, bool>> methods;
    };

    bool collect_members(
        projection& proj,
        type_entry& entry,
        table::type_view const& record,
        type_members& collected);

    bool bind_methods(
        type_members& collected, PyTypeObject* instance_type, PyTypeObject* statics);
} // namespace py::interp
