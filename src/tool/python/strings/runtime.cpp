
#include <Python.h>
#include "pybase.h"

/**
 * Adds a Python type to a Python module.
 *
 * @param module The module to add the type to.
 * @param type_name A valid Python identifier.
 * @param type_spec The Python type spec.
 * @param base_type The base type, a tuple of base types or nullptr to use the base
 * slot.
 * @returns A borrowed reference to the type object or nullptr on error.
 */
PyTypeObject* py::register_python_type(
    PyObject* module,
    const char* const type_name,
    PyType_Spec* type_spec,
    PyObject* base_type) noexcept
{
    py::pyobj_handle type_object{PyType_FromSpecWithBases(type_spec, base_type)};

    if (!type_object)
    {
        return nullptr;
    }

    // steals ref to type_object on success!
    if (PyModule_AddObject(module, type_name, type_object.get()) == -1)
    {
        return nullptr;
    }

    return reinterpret_cast<PyTypeObject*>(type_object.detach());
}
