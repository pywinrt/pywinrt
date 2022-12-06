
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
 * @returns A new reference to the type object or nullptr on error.
 */
PyTypeObject* py::register_python_type(
    PyObject* module,
    const char* const type_name,
    PyType_Spec* type_spec,
    PyObject* base_type) noexcept
{
    py::pyobj_handle type_object
    {
#if PY_HEX_VERSION >= 0x03090000
        PyType_FromModuleAndSpec(module, type_spec, base_type)
#else
        PyType_FromSpecWithBases(type_spec, base_type)
#endif
    };

    if (!type_object)
    {
        return nullptr;
    }

#if PY_HEX_VERSION >= 0x03100000
    if (PyModule_AddObjectRef(module, type_name, type_object.get()) == -1)
    {
        return nullptr;
    }
#else
    // steals ref to type_object on success!
    Py_INCREF(type_object.get());
    if (PyModule_AddObject(module, type_name, type_object.get()) == -1)
    {
        Py_DECREF(type_object.get());
        return nullptr;
    }
#endif

    return reinterpret_cast<PyTypeObject*>(type_object.detach());
}

/**
 * Wraps a WinRT KeyValuePair iterator in a Python type that iterates the
 * keys only to be consistent with the Python mapping protocol.
 * @param iter The mapping iterator returned from the First() method.
 * @return A new reference to a new object that wraps @p iter.
 */
PyObject* py::wrap_mapping_iter(PyObject* iter) noexcept
{
    auto mapping_iter_type = py::get_python_type<py::MappingIter>();

    if (!mapping_iter_type)
    {
        return nullptr;
    }

#if PY_VERSION_HEX >= 0x03090000
    py::pyobj_handle wrapper{
        PyObject_CallOneArg(reinterpret_cast<PyObject*>(mapping_iter_type), iter)};
#else
    py::pyobj_handle wrapper{PyObject_CallFunctionObjArgs(
        reinterpret_cast<PyObject*>(mapping_iter_type), iter, nullptr)};
#endif

    if (!wrapper)
    {
        return nullptr;
    }

    return wrapper.detach();
}
