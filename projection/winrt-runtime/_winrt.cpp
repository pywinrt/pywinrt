// must be included before winrt to avoid compile errors
#include <Shobjidl.h>

#define PYWINRT_RUNTIME_MODULE
#include "pybase.h"
#include <winrt/base.h>

namespace py::cpp::_winrt
{
    // BEGIN: class _winrt.Object:

    static constexpr const char* const type_name_Object = "Object";

    PyDoc_STRVAR(Object_doc, "base class for wrapped WinRT object instances.");

    static PyObject* Object_new(
        PyTypeObject* /*unused*/, PyObject* /*unused*/, PyObject* /*unused*/) noexcept
    {
        py::set_invalid_activation_error(type_name_Object);
        return nullptr;
    }

    static void Object_dealloc(
        py::winrt_wrapper<winrt::Windows::Foundation::IInspectable>* self)
    {
        auto tp = Py_TYPE(self);

        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    static PyType_Slot Object_type_slots[] = {
        {Py_tp_new, reinterpret_cast<void*>(Object_new)},
        {Py_tp_dealloc, reinterpret_cast<void*>(Object_dealloc)},
        {Py_tp_doc, const_cast<char*>(Object_doc)},
        {},
    };

    static PyType_Spec Object_type_spec
        = {"_winrt.Object",
           sizeof(py::winrt_wrapper<winrt::Windows::Foundation::IUnknown>),
           0,
           Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
           Object_type_slots};

    // END: class _winrt.Object:

    // BEGIN: class _winrt.Array:

    extern PyType_Spec Array_type_spec;

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
    //          return next(self._iter).key
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
        if (kwds)
        {
            PyErr_SetString(PyExc_TypeError, "keyword arguments are not supported");
            return -1;
        }

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
        {Py_tp_members, reinterpret_cast<void*>(MappingIter_members)},
        {Py_tp_init, reinterpret_cast<void*>(MappingIter_init)},
        {Py_tp_iter, reinterpret_cast<void*>(PyObject_SelfIter)},
        {Py_tp_iternext, reinterpret_cast<void*>(MappingIter_iternext)},
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

    static const py::runtime_api runtime_api{
        .runtime_api_guid = py::runtime_api_guid,
        .abi_version_major = py::runtime_abi_version_major,
        .abi_version_minor = py::runtime_abi_version_minor,
        .register_python_type = py::register_python_type,
        .wrap_mapping_iter = py::wrap_mapping_iter,
        .is_buffer_compatible = py::is_buffer_compatible,
        .convert_datetime = py::convert_datetime,
        .convert_to_datetime = py::convert_to_datetime,
        .get_object_type = py::get_object_type,
        .array_new = py::cpp::_winrt::Array_New,
        .array_assign = &py::cpp::_winrt::Array_Assign,
    };

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

    static PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt",
           module_doc,
           0,
           module_methods,
           nullptr,
           nullptr,
           nullptr,
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

        if (py::register_python_type(module.get(), &Object_type_spec, nullptr, nullptr)
            == -1)
        {
            return nullptr;
        }

        if (py::register_python_type(module.get(), &Array_type_spec, nullptr, nullptr)
            == -1)
        {
            return nullptr;
        }

        if (py::register_python_type(
                module.get(), &MappingIter_type_spec, nullptr, nullptr)
            == -1)
        {
            return nullptr;
        }

        pyobj_handle runtime_api_capsule{PyCapsule_New(
            const_cast<py::runtime_api*>(&runtime_api),
            "winrt._winrt._C_API",
            nullptr)};

        if (!runtime_api_capsule)
        {
            return nullptr;
        }

        if (PyModule_AddObjectRef(module.get(), "_C_API", runtime_api_capsule.get())
            == -1)
        {
            return nullptr;
        }

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

PyTypeObject* py::get_object_type() noexcept
{
    py::pyobj_handle system_module{PyImport_ImportModule("winrt.system")};

    if (!system_module)
    {
        return nullptr;
    }

    py::pyobj_handle object_type{PyObject_GetAttrString(system_module.get(), "Object")};

    if (!object_type)
    {
        return nullptr;
    }

    if (!PyType_Check(object_type.get()))
    {
        PyErr_SetString(PyExc_TypeError, "winrt.system.Object is not a type object!");
        return nullptr;
    }

    if (reinterpret_cast<PyTypeObject*>(object_type.get())->tp_new
        != py::cpp::_winrt::Object_new)
    {
        PyErr_SetString(
            PyExc_TypeError, "winrt.system.Object is not the expected type!");
        return nullptr;
    }

    return reinterpret_cast<PyTypeObject*>(object_type.get());
}
