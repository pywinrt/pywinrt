
#include <Python.h>
#include "pybase.h"

// "backport" of Python 3.12 function.
#if PY_VERSION_HEX < 0x030C0000
static PyObject* PyType_FromMetaclass(
    PyTypeObject* metaclass, PyObject* module, PyType_Spec* spec, PyObject* bases)
{
    py::pyobj_handle temp
    {
#if PY_VERSION_HEX >= 0x03090000
        PyType_FromModuleAndSpec(module, spec, bases)
#else
        PyType_FromSpecWithBases(spec, bases)
#endif
    };

    if (!temp)
    {
        return nullptr;
    }

    // if no metaclass, then no hacks needed
    if (!metaclass)
    {
        return temp.detach();
    }

    // https://github.com/wjakob/nanobind/blob/54b2e7e40fd36ff9c2923a933982b79c39b14b10/src/nb_type.cpp#L457

    /* The fallback code below is cursed. It provides an alternative when
        PyType_FromMetaclass() is not available we are furthermore *not*
        targeting the stable ABI interface. It calls PyType_FromSpec() to create
        a tentative type, copies its contents into a larger type with a
        different metaclass, then lets the original type expire. */

    auto temp_ht = reinterpret_cast<PyHeapTypeObject*>(temp.get());
    auto temp_tp = &temp_ht->ht_type;

    py::pyobj_handle result{PyType_GenericAlloc(metaclass, Py_SIZE(temp_tp))};

    if (!result)
    {
        return nullptr;
    }

    auto ht = reinterpret_cast<PyHeapTypeObject*>(result.get());
    auto tp = &ht->ht_type;

    Py_INCREF(temp_ht->ht_name);
    Py_INCREF(temp_ht->ht_qualname);
    Py_XINCREF(temp_ht->ht_slots);

    ht->ht_name = temp_ht->ht_name;
    ht->ht_qualname = temp_ht->ht_qualname;
    ht->ht_slots = temp_ht->ht_slots;

#if PY_VERSION_HEX >= 0x03090000
    Py_XINCREF(temp_ht->ht_module);
    ht->ht_module = temp_ht->ht_module;
#endif

    auto tp_name = PyUnicode_AsUTF8AndSize(temp_ht->ht_name, nullptr);

    if (!tp_name)
    {
        return nullptr;
    }

    tp->tp_name = tp_name;

    std::unique_ptr<char, decltype(&PyObject_Free)> tp_doc(
        (char*)nullptr, &PyObject_Free);

    if (temp_tp->tp_doc)
    {
        auto size = std::strlen(temp_tp->tp_doc) + 1;

        tp_doc.reset((char*)PyObject_Malloc(size));

        if (!tp_doc)
        {
            return nullptr;
        }

        std::memcpy(tp_doc.get(), temp_tp->tp_doc, size);
    }

    tp->tp_doc = tp_doc.release();

    tp->tp_flags = spec->flags | Py_TPFLAGS_HEAPTYPE;

    if (temp_tp->tp_flags & Py_TPFLAGS_HAVE_GC)
    {
        tp->tp_flags |= Py_TPFLAGS_HAVE_GC;
    }

    /* The following fields remain intentionally null-initialized
       following the call to PyType_GenericAlloc(): tp_dict, tp_bases, tp_mro,
       tp_cache, tp_subclasses, tp_weaklist. */

    /* not used in PyWinRT: tp_vectorcall, tp_weaklistoffset, tp_dictoffset,
     * tp_vectorcall_offset */

    Py_INCREF(temp_tp->tp_base);

    tp->tp_basicsize = temp_tp->tp_basicsize;
    tp->tp_itemsize = temp_tp->tp_itemsize;
    tp->tp_dealloc = temp_tp->tp_dealloc;
    tp->tp_getattr = temp_tp->tp_getattr;
    tp->tp_setattr = temp_tp->tp_setattr;
    tp->tp_repr = temp_tp->tp_repr;
    tp->tp_hash = temp_tp->tp_hash;
    tp->tp_call = temp_tp->tp_call;
    tp->tp_str = temp_tp->tp_str;
    tp->tp_getattro = temp_tp->tp_getattro;
    tp->tp_setattro = temp_tp->tp_setattro;
    tp->tp_traverse = temp_tp->tp_traverse;
    tp->tp_clear = temp_tp->tp_clear;
    tp->tp_richcompare = temp_tp->tp_richcompare;
    tp->tp_iter = temp_tp->tp_iter;
    tp->tp_iternext = temp_tp->tp_iternext;
    tp->tp_methods = temp_tp->tp_methods;
    tp->tp_getset = temp_tp->tp_getset;
    tp->tp_base = temp_tp->tp_base;
    tp->tp_descr_get = temp_tp->tp_descr_get;
    tp->tp_descr_set = temp_tp->tp_descr_set;
    tp->tp_init = temp_tp->tp_init;
    tp->tp_alloc = temp_tp->tp_alloc;
    tp->tp_new = temp_tp->tp_new;
    tp->tp_free = temp_tp->tp_free;
    tp->tp_is_gc = temp_tp->tp_is_gc;
    tp->tp_del = temp_tp->tp_del;
    tp->tp_finalize = temp_tp->tp_finalize;

    if (temp_tp->tp_members)
    {
        tp->tp_members = (PyMemberDef*)((char*)tp + Py_TYPE(tp)->tp_basicsize);
        std::memcpy(
            tp->tp_members, temp_tp->tp_members, tp->tp_itemsize * Py_SIZE(temp_tp));
    }

    ht->as_async = temp_ht->as_async;
    tp->tp_as_async = &ht->as_async;

    ht->as_number = temp_ht->as_number;
    tp->tp_as_number = &ht->as_number;

    ht->as_sequence = temp_ht->as_sequence;
    tp->tp_as_sequence = &ht->as_sequence;

    ht->as_mapping = temp_ht->as_mapping;
    tp->tp_as_mapping = &ht->as_mapping;

    ht->as_buffer = temp_ht->as_buffer;
    tp->tp_as_buffer = &ht->as_buffer;

    if (PyType_Ready(tp) < 0)
    {
        return nullptr;
    }

    return result.detach();
}
#endif

/**
 * Adds a Python type to a Python module.
 *
 * @param module The module to add the type to.
 * @param type_name A valid Python identifier.
 * @param type_spec The Python type spec.
 * @param base_type The base type, a tuple of base types or nullptr to use the base
 * slot.
 * @param metaclass Optional metaclass for the type.
 * @returns A new reference to the type object or nullptr on error.
 */
PyTypeObject* py::register_python_type(
    PyObject* module,
    const char* const type_name,
    PyType_Spec* type_spec,
    PyObject* base_type,
    PyTypeObject* metaclass) noexcept
{
    py::pyobj_handle type_object{
        PyType_FromMetaclass(metaclass, module, type_spec, base_type)};

    if (!type_object)
    {
        return nullptr;
    }

#if PY_VERSION_HEX >= 0x030A0000
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
