// Making a Python type whose metaclass is not `type`.
//
// CPython grew PyType_FromMetaclass() in 3.12; the three files here that build
// a type with a metaclass - runtime.cpp, delegates.cpp and generics.cpp - call
// it whatever they are compiled against, so below 3.12 they get the one here.

#pragma once

#include <Python.h>

#include <pywinrt/base.h>

#if PY_VERSION_HEX < 0x030C0000
/**
 * Creates a type from @p spec whose metaclass is @p metaclass.
 *
 * A backport of the CPython 3.12 function of the same name. PyWinRT is not
 * built against the stable ABI, so it can do what nanobind's nb_type does:
 * make a tentative type with PyType_FromModuleAndSpec(), copy it into a larger
 * one allocated with the metaclass, and let the tentative one expire.
 *
 * @param metaclass The metaclass, or nullptr for the default.
 * @param module The module the type belongs to.
 * @param spec The type spec.
 * @param bases The base type, a tuple of base types, or nullptr.
 * @returns New reference on success or nullptr on error.
 */
inline PyObject* PyType_FromMetaclass(
    PyTypeObject* metaclass, PyObject* module, PyType_Spec* spec, PyObject* bases)
{
    py::pyobj_handle temp{PyType_FromModuleAndSpec(module, spec, bases)};

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
    Py_XINCREF(temp_ht->ht_module);

    ht->ht_name = temp_ht->ht_name;
    ht->ht_qualname = temp_ht->ht_qualname;
    ht->ht_slots = temp_ht->ht_slots;
    ht->ht_module = temp_ht->ht_module;

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

    // PyType_FromModuleAndSpec() put __module__ in the tentative type's dict,
    // from the part of the spec name before the last dot, and tp_dict is one
    // of the fields deliberately not copied above. Nothing else rebuilds it:
    // the methods, getsets and members come back from the slots, but a type
    // that does not say which module it belongs to reports the one its base
    // came from.
    py::pyobj_handle module_name{PyObject_GetAttrString(temp.get(), "__module__")};

    if (!module_name)
    {
        return nullptr;
    }

    if (PyObject_SetAttrString(result.get(), "__module__", module_name.get()) < 0)
    {
        return nullptr;
    }

    return result.detach();
}
#endif
