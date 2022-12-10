#include "pybase.h"
#include <winrt/base.h>

namespace py::cpp::_winrt
{
    struct Array
    {
        PyObject_HEAD;
        std::unique_ptr<py::Array> array;
    };

    static constexpr const char* const Array_type_name = "Array";

    PyDoc_STRVAR(Array_doc, "class for wrapped COM array instances");

    static PyObject* Array_tp_new(
        PyTypeObject* subtype, PyObject* args, PyObject* kwds) noexcept
    {
        pyobj_handle self{subtype->tp_alloc(subtype, 0)};

        if (!self)
        {
            return nullptr;
        }

        // call C++ constructors on memory allocated from CPython heap
        new (&reinterpret_cast<Array*>(self.get())->array) std::unique_ptr<py::Array>{};

        PyObject* arg0;
        PyObject* arg1 = nullptr;

        if (PyArg_UnpackTuple(args, "Array", 1, 2, &arg0, &arg1) < 0)
        {
            return nullptr;
        }

        std::unique_ptr<py::Array> array;

        if (PyUnicode_Check(arg0))
        {
            if (PyUnicode_CompareWithASCIIString(arg0, "?") == 0)
            {
                reinterpret_cast<Array*>(self.get())->array
                    = std::make_unique<py::ComArray<bool>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "b") == 0)
            {
                reinterpret_cast<Array*>(self.get())->array
                    = std::make_unique<py::ComArray<int8_t>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "B") == 0)
            {
                reinterpret_cast<Array*>(self.get())->array
                    = std::make_unique<py::ComArray<uint8_t>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "h") == 0)
            {
                reinterpret_cast<Array*>(self.get())->array
                    = std::make_unique<py::ComArray<int16_t>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "H") == 0)
            {
                reinterpret_cast<Array*>(self.get())->array
                    = std::make_unique<py::ComArray<uint16_t>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "i") == 0)
            {
                reinterpret_cast<Array*>(self.get())->array
                    = std::make_unique<py::ComArray<int32_t>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "I") == 0)
            {
                reinterpret_cast<Array*>(self.get())->array
                    = std::make_unique<py::ComArray<uint32_t>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "q") == 0)
            {
                reinterpret_cast<Array*>(self.get())->array
                    = std::make_unique<py::ComArray<int64_t>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "Q") == 0)
            {
                reinterpret_cast<Array*>(self.get())->array
                    = std::make_unique<py::ComArray<uint64_t>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "f") == 0)
            {
                reinterpret_cast<Array*>(self.get())->array
                    = std::make_unique<py::ComArray<float>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "d") == 0)
            {
                reinterpret_cast<Array*>(self.get())->array
                    = std::make_unique<py::ComArray<double>>();
            }
            else if (PyUnicode_CompareWithASCIIString(arg0, "u") == 0)
            {
                reinterpret_cast<Array*>(self.get())->array
                    = std::make_unique<py::ComArray<char16_t>>();
            }
            else
            {
                PyErr_SetString(PyExc_ValueError, "unsupported format string");
                return nullptr;
            }
        }
        else if (PyType_Check(arg0))
        {
            // FIXME: where can we put this so it only imports once?
            PyDateTime_IMPORT;

            auto type = reinterpret_cast<PyTypeObject*>(arg0);

            if (type == &PyUnicode_Type)
            {
                reinterpret_cast<Array*>(self.get())->array
                    = std::make_unique<py::ComArray<winrt::hstring>>();
            }
            else if (std::strcmp(type->tp_name, "UUID") == 0) // TODO: stricter check
            {
                reinterpret_cast<Array*>(self.get())->array
                    = std::make_unique<py::ComArray<winrt::guid>>();
            }
            else if (type == PyDateTimeAPI->DateTimeType)
            {
                reinterpret_cast<Array*>(self.get())->array = std::make_unique<
                    py::ComArray<winrt::Windows::Foundation::DateTime>>();
            }
            else if (type == PyDateTimeAPI->DeltaType)
            {
                reinterpret_cast<Array*>(self.get())->array = std::make_unique<
                    py::ComArray<winrt::Windows::Foundation::TimeSpan>>();
            }
            else if (type == py::get_python_type<winrt::Windows::Foundation::Point>())
            {
                reinterpret_cast<Array*>(self.get())->array = std::make_unique<
                    py::ComArray<winrt::Windows::Foundation::Point>>();
            }
            else if (type == py::get_python_type<winrt::Windows::Foundation::Size>())
            {
                reinterpret_cast<Array*>(self.get())->array = std::make_unique<
                    py::ComArray<winrt::Windows::Foundation::Size>>();
            }
            else if (type == py::get_python_type<winrt::Windows::Foundation::Rect>())
            {
                reinterpret_cast<Array*>(self.get())->array = std::make_unique<
                    py::ComArray<winrt::Windows::Foundation::Rect>>();
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
            return self.detach();
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

                uint32_t size = count;

                if (size != count)
                {
                    PyErr_SetString(PyExc_OverflowError, "count exceeds max size");
                    return nullptr;
                }

                if (!reinterpret_cast<Array*>(self.get())->array->Alloc(size))
                {
                    return nullptr;
                }

                return self.detach();
            }
        }

        if (PyObject_CheckBuffer(arg1))
        {
            Py_buffer view;

            if (PyObject_GetBuffer(arg1, &view, PyBUF_FULL_RO) < 0)
            {
                return nullptr;
            }

            using py_buffer_ptr
                = std::unique_ptr<Py_buffer, decltype(&PyBuffer_Release)>;
            py_buffer_ptr{&view, &PyBuffer_Release};

            if (view.itemsize
                != reinterpret_cast<Array*>(self.get())->array->ValueSize())
            {
                PyErr_SetString(PyExc_TypeError, "itemsize is incorrect");
                return nullptr;
            }

            if (view.format == nullptr
                || std::strcmp(
                       view.format,
                       reinterpret_cast<Array*>(self.get())->array->Format())
                       != 0)
            {
                PyErr_SetString(PyExc_TypeError, "format is incorrect");
                return nullptr;
            }

            uint32_t const size = view.len / view.itemsize;

            if (size != view.len / view.itemsize)
            {
                PyErr_SetString(PyExc_OverflowError, "count exceeds max size");
                return nullptr;
            }

            if (!reinterpret_cast<Array*>(self.get())->array->Alloc(size))
            {
                return nullptr;
            }

            if (PyBuffer_ToContiguous(
                    reinterpret_cast<Array*>(self.get())->array->Data(),
                    &view,
                    reinterpret_cast<Array*>(self.get())->array->Size()
                        * reinterpret_cast<Array*>(self.get())->array->ValueSize(),
                    'C')
                < 0)
            {
                return nullptr;
            }

            return self.detach();
        }

        if (PyList_CheckExact(arg1) || PyTuple_CheckExact(arg1))
        {
            Py_ssize_t count = PySequence_Fast_GET_SIZE(arg1);

            uint32_t size = count;

            if (size != count)
            {
                PyErr_SetString(PyExc_OverflowError, "count exceeds max size");
                return nullptr;
            }

            if (!reinterpret_cast<Array*>(self.get())->array->Alloc(size))
            {
                return nullptr;
            }

            PyObject** items = PySequence_Fast_ITEMS(arg1);

            for (uint32_t i = 0;
                 i < reinterpret_cast<Array*>(self.get())->array->Size();
                 i++)
            {
                if (!reinterpret_cast<Array*>(self.get())->array->Set(i, items[i]))
                {
                    return nullptr;
                }
            }

            return self.detach();
        }

        PyErr_Format(
            PyExc_TypeError,
            "cannot convert '%.200s' object to Array",
            Py_TYPE(arg1)->tp_name);

        return nullptr;
    }

    static void Array_tp_dealloc(PyObject* self) noexcept
    {
        PyTypeObject* tp = Py_TYPE(self);
        reinterpret_cast<Array*>(self)->array.~unique_ptr();
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyMethodDef Array_tp_methods[] = {
#if PY_VERSION_HEX >= 0x03090000
        {"__class_getitem__",
         Py_GenericAlias,
         METH_O | METH_CLASS,
         PyDoc_STR("See PEP 585")},
#endif
        {}};

    static Py_ssize_t Array_sq_length(PyObject* self) noexcept
    {
        return reinterpret_cast<Array*>(self)->array->Size();
    }

    PyObject* Array_sq_item(PyObject* self, Py_ssize_t index) noexcept
    {
        return reinterpret_cast<Array*>(self)->array->At(index);
    }

    static int Array_bf_getbuffer(PyObject* self, Py_buffer* view, int flags) noexcept
    {
        view->obj = nullptr;

        if (((flags & PyBUF_WRITABLE) == PyBUF_WRITABLE))
        {
            PyErr_SetString(PyExc_BufferError, "Array is not writable.");
            return -1;
        }

        view->readonly = 1;

        // required fields
        Py_INCREF(self);
        view->obj = self;
        view->buf
            = reinterpret_cast<void*>(reinterpret_cast<Array*>(self)->array->Data());
        view->len
            = static_cast<Py_ssize_t>(reinterpret_cast<Array*>(self)->array->Size())
              * reinterpret_cast<Array*>(self)->array->ValueSize();
        view->itemsize = reinterpret_cast<Array*>(self)->array->ValueSize();
        view->ndim = 1;

        if ((flags & PyBUF_FORMAT) == PyBUF_FORMAT)
        {
            view->format
                = const_cast<char*>(reinterpret_cast<Array*>(self)->array->Format());
        }
        else
        {
            view->format = nullptr;
        }

        if ((flags & PyBUF_ND) == PyBUF_ND)
        {
            // HACK: using internal as a Py_ssize_t value so that we don't have
            // to allocate/free the extra info
            view->internal = reinterpret_cast<void*>(
                static_cast<uintptr_t>(reinterpret_cast<Array*>(self)->array->Size()));
            view->shape = reinterpret_cast<Py_ssize_t*>(&view->internal);
        }
        else
        {
            view->internal = NULL;
            view->shape = NULL;
        }

        if ((flags & PyBUF_STRIDES) == PyBUF_STRIDES)
        {
            view->strides = &(view->itemsize);
        }
        else
        {
            view->strides = NULL;
        }

        view->suboffsets = NULL;

        return 0;
    }

#if PY_VERSION_HEX < 0x03090000
    PyBufferProcs Array_buffer_procs = {Array_bf_getbuffer, nullptr};
#endif

    static PyType_Slot Array_type_slots[] = {
        {Py_tp_doc, const_cast<char*>(Array_doc)},
        {Py_tp_new, Array_tp_new},
        {Py_tp_dealloc, Array_tp_dealloc},
        {Py_tp_methods, Array_tp_methods},
        {Py_sq_length, Array_sq_length},
        {Py_sq_item, Array_sq_item},
#if PY_VERSION_HEX >= 0x03090000
        {Py_bf_getbuffer, Array_bf_getbuffer},
#endif
        {},
    };

    PyType_Spec Array_type_spec
        = {"_winrt.Array", sizeof(Array), 0, Py_TPFLAGS_DEFAULT, Array_type_slots};
} // namespace py::cpp::_winrt
