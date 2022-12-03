#include "pybase.h"
#include <Shobjidl.h>
#include <winrt/base.h>

namespace py::cpp::_winrt
{
    struct module_state
    {
        PyTypeObject* Object_type;
        PyTypeObject* Array_type;
        PyTypeObject* MappingIter_type;
    };

    // BEGIN: class _winrt.Object:

    constexpr const char* const _type_name_Object = "Object";

    PyDoc_STRVAR(Object_doc, "base class for wrapped WinRT object instances.");

    static PyObject* Object_new(
        PyTypeObject* /* unused */,
        PyObject* /* unused */,
        PyObject* /* unused */) noexcept
    {
        py::set_invalid_activation_error(_type_name_Object);
        return nullptr;
    }

    static void Object_dealloc(
        py::winrt_wrapper<winrt::Windows::Foundation::IInspectable>* self)
    {
        auto tp = Py_TYPE(self);
        self->obj = nullptr;
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyType_Slot Object_type_slots[] = {
        {Py_tp_new, Object_new},
        {Py_tp_dealloc, Object_dealloc},
        {Py_tp_doc, const_cast<char*>(Object_doc)},
        {},
    };

    static PyType_Spec Object_type_spec
        = {"_winrt.Object",
           sizeof(py::winrt_wrapper<winrt::Windows::Foundation::IInspectable>),
           0,
           Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
           Object_type_slots};

    // END: class _winrt.Object:

    // BEGIN: class _winrt.Array:

    extern PyType_Spec Array_type_spec;
#if PY_VERSION_HEX < 0x03090000
    extern PyBufferProcs Array_buffer_procs;
#endif

    // END: class _winrt.Array:

    // BEGIN: class _winrt.MappingIter:

    // This class is used to wrap the iterator returned by IMap/IMapView so
    // that it returns only the key instead of a KeyValuePair. This is done
    // to be consistent with the Python mapping protocol.
    //
    // In Python it would look something like this:
    //
    //  class MappingIter:
    //      def  __init__(self, base_iter):
    //          self._iter = iter(base_iter)
    //
    //      def __iter__(self):
    //          return self
    //
    //      def __next__(self):
    //          return next(self._iter)
    //

    struct MappingIter_object
    {
        PyObject_HEAD;
        PyObject* _iter;
    };

    static PyMemberDef MappingIter_members[]
        = {{"_iter",
            T_OBJECT_EX,
            offsetof(MappingIter_object, _iter),
            0,
            PyDoc_STR("base KeyValuePair iterator")},
           {}};

    static int MappingIter_init(PyObject* self, PyObject* args, PyObject* kwds) noexcept
    {
        // borrowed ref
        auto base_iter = PyTuple_GetItem(args, 0);

        if (!base_iter)
        {
            return -1;
        }

        if (!PyIter_Check(base_iter))
        {
            PyErr_SetString(PyExc_TypeError, "expecting an iterator");
            return -1;
        }

        if (PyObject_SetAttrString(self, "_iter", base_iter) == -1)
        {
            return -1;
        }

        return 0;
    }

    static PyObject* MappingIter_iternext(PyObject* self) noexcept
    {
        // new reference
        py::pyobj_handle base_iter{PyObject_GetAttrString(self, "_iter")};

        if (!base_iter)
        {
            return nullptr;
        }

        // new reference
        py::pyobj_handle next{PyIter_Next(base_iter.get())};

        if (!next)
        {
            return nullptr;
        }

        // new reference
        py::pyobj_handle key{PyObject_GetAttrString(next.get(), "key")};

        if (!key)
        {
            return nullptr;
        }

        return key.detach();
    }

    PyDoc_STRVAR(MappingIter_doc, "Utility class for wrapping KeyValuePair iterators.");

    static PyType_Slot MappingIter_type_slots[] = {
        {Py_tp_members, MappingIter_members},
        {Py_tp_init, MappingIter_init},
        {Py_tp_iter, PyObject_SelfIter},
        {Py_tp_iternext, MappingIter_iternext},
        {Py_tp_doc, const_cast<char*>(MappingIter_doc)},
        {},
    };

    static PyType_Spec MappingIter_type_spec
        = {"_winrt.MappingIter",
           sizeof(MappingIter_object),
           0,
           Py_TPFLAGS_DEFAULT,
           MappingIter_type_slots};

    // END: class _winrt.MappingIter:

    static PyObject* init_apartment(PyObject* /*unused*/, PyObject* type_obj) noexcept
    {
        auto type = PyLong_AsLong(type_obj);

        if (type == -1 && PyErr_Occurred())
        {
            return nullptr;
        }

        try
        {
            winrt::init_apartment(static_cast<winrt::apartment_type>(type));
            Py_RETURN_NONE;
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* uninit_apartment(
        PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        winrt::uninit_apartment();
        Py_RETURN_NONE;
    }

    static PyObject* initialize_with_window(
        PyObject* /*unused*/, PyObject* args) noexcept
    {
        PyObject* obj;
        Py_ssize_t hwnd;

        if (!PyArg_ParseTuple(args, "On", &obj, &hwnd))
        {
            return nullptr;
        }

        try
        {
            auto winrt_obj
                = py::convert_to<winrt::Windows::Foundation::IInspectable>(obj);
            auto result = winrt_obj.as<IInitializeWithWindow>()->Initialize(
                reinterpret_cast<HWND>(hwnd));

            if (result != S_OK)
            {
                winrt::throw_hresult(result);
            }
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }

        Py_RETURN_NONE;
    }

    PyDoc_STRVAR(module_doc, "_winrt");

    static PyMethodDef module_methods[]{
        {"init_apartment", init_apartment, METH_O, "initialize the apartment"},
        {"uninit_apartment",
         uninit_apartment,
         METH_NOARGS,
         "uninitialize the apartment"},
        {"initialize_with_window",
         initialize_with_window,
         METH_VARARGS,
         "interop function to invoke IInitializeWithWindow::Initialize on an object"},
        {}};

    static int module_traverse(PyObject* module, visitproc visit, void* arg) noexcept
    {
        auto state = reinterpret_cast<module_state*>(PyModule_GetState(module));
        assert(state);

        Py_VISIT(state->Object_type);
        Py_VISIT(state->Array_type);
        Py_VISIT(state->MappingIter_type);

        return 0;
    }

    static int module_clear(PyObject* module) noexcept
    {
        auto state = reinterpret_cast<module_state*>(PyModule_GetState(module));
        assert(state);

        Py_CLEAR(state->Object_type);
        Py_CLEAR(state->Array_type);
        Py_CLEAR(state->MappingIter_type);

        return 0;
    }

    static PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt",
           module_doc,
           sizeof(module_state),
           module_methods,
           nullptr,
           module_traverse,
           module_clear,
           nullptr};

    static PyObject* module_init() noexcept
    {
        static const auto kMTA
            = static_cast<long>(winrt::apartment_type::multi_threaded);
        static const auto kSTA
            = static_cast<long>(winrt::apartment_type::single_threaded);

        py::pyobj_handle module{PyModule_Create(&module_def)};

        if (!module)
        {
            return nullptr;
        }

        auto state = reinterpret_cast<module_state*>(PyModule_GetState(module.get()));
        assert(state);

        state->Object_type = py::register_python_type(
            module.get(), _type_name_Object, &Object_type_spec, nullptr);

        if (!state->Object_type)
        {
            return nullptr;
        }

        Py_INCREF(state->Object_type);

        state->Array_type = py::register_python_type(
            module.get(), "Array", &Array_type_spec, nullptr);

        if (!state->Array_type)
        {
            return nullptr;
        }

        Py_INCREF(state->Array_type);

#if PY_VERSION_HEX < 0x03090000
        state->Array_type->tp_as_buffer = &Array_buffer_procs;
#endif

        state->MappingIter_type = py::register_python_type(
            module.get(), "MappingIter", &MappingIter_type_spec, nullptr);

        if (!state->MappingIter_type)
        {
            return nullptr;
        }

        Py_INCREF(state->MappingIter_type);

        if (PyModule_AddIntConstant(module.get(), "MTA", kMTA) == -1)
        {
            return nullptr;
        }

        if (PyModule_AddIntConstant(module.get(), "STA", kSTA) == -1)
        {
            return nullptr;
        }

        return module.detach();
    }
} // namespace py::cpp::_winrt

PyMODINIT_FUNC PyInit__winrt(void) noexcept
{
    return py::cpp::_winrt::module_init();
}

PyTypeObject* py::winrt_type<py::Object>::get_python_type() noexcept
{
    // borrowed ref
    auto module = PyState_FindModule(&py::cpp::_winrt::module_def);

    if (!module)
    {
        PyErr_SetString(PyExc_RuntimeError, "could not find _winrt module");
        return nullptr;
    }

    auto state
        = reinterpret_cast<py::cpp::_winrt::module_state*>(PyModule_GetState(module));
    assert(state);

    return state->Object_type;
}

PyTypeObject* py::winrt_type<py::Array>::get_python_type() noexcept
{
    // borrowed ref
    auto module = PyState_FindModule(&py::cpp::_winrt::module_def);

    if (!module)
    {
        PyErr_SetString(PyExc_RuntimeError, "could not find _winrt module");
        return nullptr;
    }

    auto state
        = reinterpret_cast<py::cpp::_winrt::module_state*>(PyModule_GetState(module));
    assert(state);

    return state->Array_type;
}

PyTypeObject* py::winrt_type<py::MappingIter>::get_python_type() noexcept
{
    // borrowed ref
    auto module = PyState_FindModule(&py::cpp::_winrt::module_def);

    if (!module)
    {
        PyErr_SetString(PyExc_RuntimeError, "could not find _winrt module");
        return nullptr;
    }

    auto state
        = reinterpret_cast<py::cpp::_winrt::module_state*>(PyModule_GetState(module));
    assert(state);

    return state->MappingIter_type;
}
