#define PYWINRT_RUNTIME_MODULE
#include <pywinrt/base.h>
#include "module_state.h"
#include <winrt/base.h>

namespace py::cpp::_winrt
{
    /**
     * Python wrapper around System.Array type.
     */
    struct Array
    {
        PyObject_HEAD
        std::unique_ptr<py::Array> array;
    };

    PyDoc_STRVAR(Array_doc, "class for wrapped COM array instances");

    static PyTypeObject* get_array_type() noexcept;

    /**
     * Allocates memory for a new System.Array object.
     * @param [in]  subtype The type to use for allocation.
     */
    static Array* Array_Alloc(PyTypeObject* subtype)
    {
        auto self = reinterpret_cast<Array*>(subtype->tp_alloc(subtype, 0));

        if (!self)
        {
            return nullptr;
        }

        // call C++ constructors on memory allocated from CPython heap
        std::construct_at(&self->array);

        return self;
    }

    /**
     * Creates a new System.Array object from @p array.
     * @param [in]  array   A WinRT array such as a COM array.
     * @returns The new Python wrapper object or @c nullptr on error.
     */
    PyObject* Array_New(std::unique_ptr<py::Array> array) noexcept
    {
        auto type = get_array_type();

        if (!type)
        {
            return nullptr;
        }

        auto self = Array_Alloc(type);

        if (!self)
        {
            return nullptr;
        }

        self->array = std::move(array);

        return reinterpret_cast<PyObject*>(self);
    }

    /**
     * Assigns @p array to @p obj if @p obj is a System.Array.
     * @param [in]  obj     The Python System.Array wrapper object.
     * @param [in]  array   The WinRT array wrapper.
     * @returns @c true on success, otherwise returns @c false and sets Python
     * exception.
     */
    bool Array_Assign(PyObject* obj, std::unique_ptr<py::Array> array) noexcept
    {
        if (!Py_IS_TYPE(obj, get_array_type()))
        {
            {
                PyErr_SetString(PyExc_TypeError, "argument must be System.Array");
                return false;
            }
        }

        reinterpret_cast<Array*>(obj)->array = std::move(array);

        return true;
    }

    /**
     * The PEP 3118 buffer format among @p annotations, which are the
     * __metadata__ of an Annotated alias, or a null handle when there is none.
     *
     * Each annotation of a winrt.system scalar says which kind of format it is
     * in _annotation_kind_, and reading that rather than taking the first
     * string is what tells the buffer format from the struct format: Char16 is
     * "u" as one and "H" as the other, and both are formats of their own that
     * name different types.
     *
     * @returns @c false with a Python error set.
     */
    static bool find_buffer_format(PyObject* annotations, pyobj_handle& format) noexcept
    {
        if (!PyTuple_Check(annotations))
        {
            return true;
        }

        for (Py_ssize_t i = 0; i < PyTuple_GET_SIZE(annotations); i++)
        {
            auto const annotation = PyTuple_GET_ITEM(annotations, i);

            pyobj_handle kind;

            if (PyObject_GetOptionalAttrString(
                    annotation, "_annotation_kind_", kind.put())
                < 0)
            {
                return false;
            }

            if (!kind)
            {
                continue;
            }

            if (!PyUnicode_Check(kind.get()))
            {
                continue;
            }

            if (PyUnicode_CompareWithASCIIString(kind.get(), "buffer_format") != 0)
            {
                continue;
            }

            if (!PyUnicode_Check(annotation))
            {
                continue;
            }

            format = pyobj_handle{Py_NewRef(annotation)};

            return true;
        }

        return true;
    }

    static PyObject* Array_tp_new(
        PyTypeObject* subtype, PyObject* args, PyObject* kwds) noexcept
    {
        if (kwds)
        {
            PyErr_SetString(PyExc_TypeError, "keyword arguments are not supported");
            return nullptr;
        }

        auto self = Array_Alloc(subtype);

        if (!self)
        {
            return nullptr;
        }

        pyobj_handle self_handle{reinterpret_cast<PyObject*>(self)};

        PyObject* arg0;
        PyObject* arg1 = nullptr;

        if (!PyArg_UnpackTuple(args, "Array", 1, 2, &arg0, &arg1))
        {
            return nullptr;
        }

        std::unique_ptr<py::Array> array;

        // A winrt.system scalar alias - Annotated[int, BufferFormat("i"), ...]
        // - names the same element type as the buffer format it carries, so
        // that format stands in for it here.
        pyobj_handle annotations;

        if (PyObject_GetOptionalAttrString(arg0, "__metadata__", annotations.put()) < 0)
        {
            return nullptr;
        }

        pyobj_handle buffer_format;

        if (annotations)
        {
            if (!find_buffer_format(annotations.get(), buffer_format))
            {
                return nullptr;
            }
        }

        if (buffer_format)
        {
            arg0 = buffer_format.get();
        }

        if (PyUnicode_Check(arg0))
        {
            // Every format string has a type that names the same thing, and
            // two spellings of one thing is where a format being a buffer
            // format and a struct format at once came from. A format unwrapped
            // from an alias just above is not the spelling being deprecated.
            if (!buffer_format)
            {
                if (PyErr_WarnEx(
                        PyExc_DeprecationWarning,
                        "passing a format string to Array is deprecated: pass "
                        "the type instead, such as winrt.system.Int32, bool, "
                        "str or a projected enum",
                        1)
                    < 0)
                {
                    return nullptr;
                }
            }

            if (PyUnicode_CompareWithASCIIString(arg0, "?") == 0)
            {
                self->array = std::make_unique<py::ComArray<bool>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "b") == 0)
            {
                self->array = std::make_unique<py::ComArray<int8_t>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "B") == 0)
            {
                self->array = std::make_unique<py::ComArray<uint8_t>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "h") == 0)
            {
                self->array = std::make_unique<py::ComArray<int16_t>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "H") == 0)
            {
                self->array = std::make_unique<py::ComArray<uint16_t>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "i") == 0)
            {
                self->array = std::make_unique<py::ComArray<int32_t>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "I") == 0)
            {
                self->array = std::make_unique<py::ComArray<uint32_t>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "q") == 0)
            {
                self->array = std::make_unique<py::ComArray<int64_t>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "Q") == 0)
            {
                self->array = std::make_unique<py::ComArray<uint64_t>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "f") == 0)
            {
                self->array = std::make_unique<py::ComArray<float>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "d") == 0)
            {
                self->array = std::make_unique<py::ComArray<double>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "u") == 0)
            {
                self->array = std::make_unique<py::ComArray<char16_t>>();
            }
            else
            {
                PyErr_SetString(PyExc_ValueError, "unsupported format string");
                return nullptr;
            }
        }
        else if (PyType_Check(arg0))
        {
            if (!PyDateTimeAPI)
            {
                PyDateTime_IMPORT;

                if (!PyDateTimeAPI)
                {
                    return nullptr;
                }
            }

            auto type = reinterpret_cast<PyTypeObject*>(arg0);

            // if a type has an _assign_array_ special method, use that to create
            // the py::Array and assign it to self->array.
            pyobj_handle assign_array{PyObject_GetAttrString(arg0, "_assign_array_")};

            if (!assign_array)
            {
                if (PyErr_ExceptionMatches(PyExc_AttributeError))
                {
                    PyErr_Clear();
                }
                else
                {
                    return nullptr;
                }
            }

            if (assign_array)
            {
                pyobj_handle result{
                    PyObject_CallFunction(assign_array.get(), "O", self)};

                if (!result)
                {
                    return nullptr;
                }
            }
            else if (type == &PyBool_Type)
            {
                self->array = std::make_unique<py::ComArray<bool>>();
            }
            else if (type == &PyUnicode_Type)
            {
                self->array = std::make_unique<py::ComArray<winrt::hstring>>();
            }
            else if (std::strcmp(type->tp_name, "UUID") == 0) // TODO: stricter
                                                              // check
            {
                self->array = std::make_unique<py::ComArray<winrt::guid>>();
            }
            else if (type == PyDateTimeAPI->DateTimeType)
            {
                self->array = std::make_unique<
                    py::ComArray<winrt::Windows::Foundation::DateTime>>();
            }
            else if (type == PyDateTimeAPI->DeltaType)
            {
                self->array = std::make_unique<
                    py::ComArray<winrt::Windows::Foundation::TimeSpan>>();
            }
            else if (type == py::get_object_type())
            {
                self->array = std::make_unique<
                    py::ComArray<winrt::Windows::Foundation::IInspectable>>();
            }
            else if (type == &PyLong_Type)
            {
                PyErr_SetString(
                    PyExc_TypeError,
                    "int does not say which WinRT integer type: name one with "
                    "a winrt.system alias such as winrt.system.Int32");
                return nullptr;
            }
            else if (type == &PyFloat_Type)
            {
                PyErr_SetString(
                    PyExc_TypeError,
                    "float does not say which WinRT floating point type: name "
                    "one with winrt.system.Single or winrt.system.Double");
                return nullptr;
            }
            else
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "cannot use %.100s as type for WinRT array",
                    type->tp_name);
                return nullptr;
            }
        }
        else
        {
            PyErr_SetString(PyExc_TypeError, "first argument must be str or type");
            return nullptr;
        }

        if (!arg1)
        {
            // no args means empty array
            return self_handle.detach();
        }

        if (PyIndex_Check(arg1))
        {
            Py_ssize_t count = PyNumber_AsSsize_t(arg1, PyExc_OverflowError);

            if (count == -1 && PyErr_Occurred())
            {
                if (!PyErr_ExceptionMatches(PyExc_TypeError))
                {
                    return nullptr;
                }

                PyErr_Clear();
            }
            else
            {
                if (count < 0)
                {
                    PyErr_SetString(PyExc_ValueError, "negative count");
                    return nullptr;
                }

                auto size = static_cast<uint32_t>(count);

                if (static_cast<Py_ssize_t>(size) != count)
                {
                    PyErr_SetString(PyExc_OverflowError, "count exceeds max size");
                    return nullptr;
                }

                if (!self->array->Alloc(size))
                {
                    return nullptr;
                }

                return self_handle.detach();
            }
        }

        if (PyObject_CheckBuffer(arg1))
        {
            py::buffer_view buffer{arg1, PyBUF_FULL_RO};
            if (!buffer)
            {
                return nullptr;
            }

            auto const& view = buffer.view();

            if (view.ndim != 1)
            {
                PyErr_SetString(PyExc_TypeError, "ndim must be 1");
                return nullptr;
            }

            if (view.itemsize != static_cast<Py_ssize_t>(self->array->ValueSize()))
            {
                PyErr_SetString(PyExc_TypeError, "itemsize is incorrect");
                return nullptr;
            }

            auto format = view.format ? std::basic_string_view(view.format)
                                      : std::basic_string_view("B");

            if (format != self->array->Format())
            {
                PyErr_SetString(PyExc_TypeError, "format is incorrect");
                return nullptr;
            }

            auto const size = static_cast<uint32_t>(view.len / view.itemsize);

            if (static_cast<Py_ssize_t>(size) != view.len / view.itemsize)
            {
                PyErr_SetString(PyExc_OverflowError, "count exceeds max size");
                return nullptr;
            }

            // TODO: need better parsing since 'P' could be in an identifier
            // however, since we are only using lower case identifiers, this
            // shouldn't be a problem in practice.
            if (format.find('P') != std::string_view::npos)
            {
                // TODO: need a way to validate pointer types before we can do this
                PyErr_SetString(
                    PyExc_NotImplementedError,
                    "copying arrays containing pointers is not implemented");
                return nullptr;
            }

            // Fast path for bilitable types.

            if (!self->array->Alloc(size))
            {
                return nullptr;
            }

            if (PyBuffer_ToContiguous(
                    self->array->Data(),
                    &view,
                    self->array->Size() * self->array->ValueSize(),
                    'C')
                < 0)
            {
                return nullptr;
            }

            return self_handle.detach();
        }

        if (PyList_CheckExact(arg1) || PyTuple_CheckExact(arg1))
        {
            auto count = PySequence_Fast_GET_SIZE(arg1);

            auto size = static_cast<uint32_t>(count);

            if (static_cast<Py_ssize_t>(size) != count)
            {
                PyErr_SetString(PyExc_OverflowError, "count exceeds max size");
                return nullptr;
            }

            if (!self->array->Alloc(size))
            {
                return nullptr;
            }

            PyObject** items = PySequence_Fast_ITEMS(arg1);

            for (uint32_t i = 0; i < self->array->Size(); i++)
            {
                if (!self->array->Set(i, items[i]))
                {
                    return nullptr;
                }
            }

            return self_handle.detach();
        }

        PyErr_Format(
            PyExc_TypeError,
            "cannot convert '%.200s' object to Array",
            Py_TYPE(arg1)->tp_name);

        return nullptr;
    }

    static void Array_tp_dealloc(Array* self) noexcept
    {
        PyTypeObject* tp = Py_TYPE(self);

        std::destroy_at(&self->array);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyObject* Array_get_winrt_element_type_name(
        Array* self, void* /*unused*/) noexcept
    {
        auto type = self->array->WinrtElementTypeName();
        return PyUnicode_FromWideChar(type.data(), type.size());
    }

    static PyGetSetDef Array_tp_getset[]
        = {{"_winrt_element_type_name_",
            reinterpret_cast<getter>(Array_get_winrt_element_type_name),
            nullptr,
            PyDoc_STR("Gets the WinRT name of the element type of this array."),
            nullptr},
           {}};

    // needed for collections.abc.MutableSequence
    static PyObject* Array_insert(PyObject* /*unused*/, PyObject* /*unused*/)
    {
        PyErr_SetString(PyExc_TypeError, "'Array' type does not support inserting");
        return nullptr;
    }

    static PyMethodDef Array_tp_methods[] = {
        {"__class_getitem__",
         Py_GenericAlias,
         METH_O | METH_CLASS,
         PyDoc_STR("See PEP 585")},
        {"insert", Array_insert, METH_VARARGS, PyDoc_STR("inserting is not supported")},
        {}};

    static Py_ssize_t Array_sq_length(Array* self) noexcept
    {
        return self->array->Size();
    }

    PyObject* Array_sq_item(Array* self, Py_ssize_t index) noexcept
    {
        WINRT_ASSERT(index >= 0);
        return self->array->At(static_cast<uint32_t>(index));
    }

    int Array_sq_ass_item(Array* self, Py_ssize_t index, PyObject* value) noexcept
    {
        if (!value)
        {
            PyErr_SetString(
                PyExc_TypeError, "'Array' object doesn't support item deletion");
            return -1;
        }

        if (!self->array->Set(index, value))
        {
            return -1;
        }

        return 0;
    }

    static int Array_bf_getbuffer(Array* self, Py_buffer* view, int flags) noexcept
    {
        view->readonly = 0;

        // required fields
        view->obj = Py_NewRef(self);
        view->buf = reinterpret_cast<void*>(self->array->Data());
        view->len
            = static_cast<Py_ssize_t>(self->array->Size()) * self->array->ValueSize();
        view->itemsize = self->array->ValueSize();
        view->ndim = 1;

        if ((flags & PyBUF_FORMAT) == PyBUF_FORMAT)
        {
            view->format = const_cast<char*>(self->array->Format().data());
        }
        else
        {
            view->format = nullptr;
        }

        if ((flags & PyBUF_ND) == PyBUF_ND)
        {
            // HACK: using internal as a Py_ssize_t value so that we don't have
            // to allocate/free the extra info
            view->internal
                = reinterpret_cast<void*>(static_cast<uintptr_t>(self->array->Size()));
            view->shape = reinterpret_cast<Py_ssize_t*>(&view->internal);
        }
        else
        {
            view->internal = nullptr;
            view->shape = nullptr;
        }

        if ((flags & PyBUF_STRIDES) == PyBUF_STRIDES)
        {
            view->strides = &(view->itemsize);
        }
        else
        {
            view->strides = nullptr;
        }

        view->suboffsets = nullptr;

        return 0;
    }

    static PyType_Slot Array_type_slots[] = {
        {Py_tp_doc, const_cast<char*>(Array_doc)},
        {Py_tp_new, reinterpret_cast<void*>(Array_tp_new)},
        {Py_tp_dealloc, reinterpret_cast<void*>(Array_tp_dealloc)},
        {Py_tp_getset, reinterpret_cast<void*>(Array_tp_getset)},
        {Py_tp_methods, reinterpret_cast<void*>(Array_tp_methods)},
        {Py_sq_length, reinterpret_cast<void*>(Array_sq_length)},
        {Py_sq_item, reinterpret_cast<void*>(Array_sq_item)},
        {Py_sq_ass_item, reinterpret_cast<void*>(Array_sq_ass_item)},
        {Py_bf_getbuffer, reinterpret_cast<void*>(Array_bf_getbuffer)},
        {},
    };

    PyType_Spec Array_type_spec
        = {"_winrt.Array", sizeof(Array), 0, Py_TPFLAGS_DEFAULT, Array_type_slots};

    static PyTypeObject* get_array_type() noexcept
    {
        auto state = py::cpp::_winrt::get_module_state();
        if (!state)
        {
            return nullptr;
        }

        return state->array_type;
    }

} // namespace py::cpp::_winrt
