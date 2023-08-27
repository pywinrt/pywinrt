// must be included before cppwinrt to enable FILETIME conversions
#include <Windows.h>

#include <Python.h>
#include <datetime.h>

#define PYWINRT_RUNTIME_MODULE
#include "pybase.h"

// "backport" of Python 3.12 function.
#if PY_VERSION_HEX < 0x030C0000
static PyObject* PyType_FromMetaclass(
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

    return result.detach();
}
#endif

/**
 * Adds a Python type to a Python module.
 *
 * @param module The module to add the type to.
 * @param type_spec The Python type spec.
 * @param base_type The base type, a tuple of base types or nullptr to use the base
 * slot.
 * @param metaclass Optional metaclass for the type.
 * @returns 0 on success or -1 on error.
 */

int py::register_python_type(
    PyObject* module,
    PyType_Spec* type_spec,
    PyObject* base_type,
    PyTypeObject* metaclass) noexcept
{
    py::pyobj_handle type_object{
        PyType_FromMetaclass(metaclass, module, type_spec, base_type)};

    if (!type_object)
    {
        return -1;
    }

    if (PyModule_AddType(module, reinterpret_cast<PyTypeObject*>(type_object.get()))
        == -1)
    {
        return -1;
    }

    return 0;
}

/**
 * Wraps a WinRT KeyValuePair iterator in a Python type that iterates the
 * keys only to be consistent with the Python mapping protocol.
 * @param iter The mapping iterator returned from the First() method.
 * @return A new reference to a new object that wraps @p iter.
 */
PyObject* py::wrap_mapping_iter(PyObject* iter) noexcept
{
    py::pyobj_handle winrt_module{PyImport_ImportModule("winrt._winrt")};

    if (!winrt_module)
    {
        return nullptr;
    }

    py::pyobj_handle mapping_iter_type{
        PyObject_GetAttrString(winrt_module.get(), "MappingIter")};

    if (!mapping_iter_type)
    {
        return nullptr;
    }

    py::pyobj_handle wrapper{PyObject_CallOneArg(mapping_iter_type.get(), iter)};

    if (!wrapper)
    {
        return nullptr;
    }

    return wrapper.detach();
}

/**
 * Tests if @p view is compatible with @p itemsize and @p format
 * @param [in]  view        A Python buffer view.
 * @param [in]  itemsize    The required element size.
 * @param [in]  format      The required element format.
 * @returns @c true if the buffer is valid, otherwise sets Python error and returns
 * @c false
 */
bool py::is_buffer_compatible(
    Py_buffer const& view, Py_ssize_t itemsize, const char* format) noexcept
{
    if (view.itemsize != itemsize)
    {
        PyErr_Format(
            PyExc_BufferError,
            "requires buffer with itemsize == %zd, have %zd",
            itemsize,
            view.ndim);
        return false;
    }

    WINRT_ASSERT(format);

    if (!view.format || std::strcmp(view.format, format))
    {
        PyErr_Format(
            PyExc_BufferError,
            "requires buffer with format == \"%s\", have \"%s\"",
            format,
            view.format);
        return false;
    }

    if (view.ndim != 1)
    {
        PyErr_Format(
            PyExc_BufferError, "requires buffer with ndim == 1, have %d", view.ndim);
        return false;
    }

    if (!view.strides || view.strides[0] != itemsize)
    {
        PyErr_Format(
            PyExc_BufferError,
            "requires buffer with strides[0] == %zd, have %zd",
            view.strides ? view.strides[0] : 0,
            itemsize);
        return false;
    }

    return true;
}

/**
 * Converts a winrt Windows.Foundation.DateTime to a Python datetime.datetime.
 * @param [in]  value   The winrt Windows.Foundation.DateTime.
 * @returns             The Python datetime.datetime or nullptr on error.
 */
PyObject* py::convert_datetime(winrt::Windows::Foundation::DateTime value) noexcept
{
    try
    {
        FILETIME ft = winrt::clock::to_FILETIME(value);
        SYSTEMTIME st;

        if (!FileTimeToSystemTime(&ft, &st))
        {
            winrt::throw_last_error();
        }

        auto microseconds = static_cast<int>(
            std::chrono::time_point_cast<std::chrono::microseconds>(value)
                .time_since_epoch()
                .count()
            % 1000);

        if (!PyDateTimeAPI)
        {
            PyDateTime_IMPORT;

            if (!PyDateTimeAPI)
            {
                return nullptr;
            }
        }

        // new reference
        return PyDateTimeAPI->DateTime_FromDateAndTime(
            st.wYear,
            st.wMonth,
            st.wDay,
            st.wHour,
            st.wMinute,
            st.wSecond,
            st.wMilliseconds * 1000 + microseconds,
            PyDateTime_TimeZone_UTC,
            PyDateTimeAPI->DateTimeType);
    }
    catch (...)
    {
        py::to_PyErr();
        return nullptr;
    }
}

/**
 * Converts a Python datetime.datetime to a winrt Windows.Foundation.DateTime.
 * @param [in]  obj     The Python datetime.datetime object.
 * @returns             The winrt Windows.Foundation.DateTime value.
 */
winrt::Windows::Foundation::DateTime py::convert_to_datetime(PyObject* obj)
{
    throw_if_pyobj_null(obj);

    if (!PyDateTimeAPI)
    {
        PyDateTime_IMPORT;

        if (!PyDateTimeAPI)
        {
            throw python_exception();
        }
    }

    if (!PyDateTime_Check(obj))
    {
        PyErr_SetString(PyExc_TypeError, "requires datetime.datetime object");
        throw python_exception();
    }

    // WinRT works in UTC, so ensure correct time zone. Also works
    // for "naive" datetime.

    // new reference
    pyobj_handle utc{
        PyObject_CallMethod(obj, "astimezone", "O", PyDateTime_TimeZone_UTC)};
    if (!utc)
    {
        throw python_exception();
    }

    SYSTEMTIME st;
    st.wYear = PyDateTime_GET_YEAR(utc.get());
    st.wMonth = PyDateTime_GET_MONTH(utc.get());
    st.wDay = PyDateTime_GET_DAY(utc.get());
    st.wHour = PyDateTime_DATE_GET_HOUR(utc.get());
    st.wMinute = PyDateTime_DATE_GET_MINUTE(utc.get());
    st.wSecond = PyDateTime_DATE_GET_SECOND(utc.get());
    st.wMilliseconds = PyDateTime_DATE_GET_MICROSECOND(utc.get()) / 1000;

    FILETIME ft;

    if (!SystemTimeToFileTime(&st, &ft))
    {
        winrt::throw_last_error();
    }

    auto value = winrt::clock::from_FILETIME(ft);

    auto microseconds = PyDateTime_DATE_GET_MICROSECOND(utc.get()) % 1000;

    value += std::chrono::duration_cast<winrt::Windows::Foundation::DateTime::duration>(
        std::chrono::microseconds{microseconds});

    return value;
}
