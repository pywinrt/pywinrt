// must be included before winrt to avoid compile errors
#include <Shobjidl.h>
#include <libloaderapi.h>

#define PYWINRT_RUNTIME_MODULE
#include "pybase.h"
#include "pyruntime.h"
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
        py::winrt_wrapper<winrt::Windows::Foundation::IInspectable>* self) noexcept
    {
        auto tp = Py_TYPE(self);

        std::destroy_at(&self->obj);
        tp->tp_free(self);
        Py_DECREF(tp);
    }

    PyDoc_STRVAR(
        Object_iids_doc,
        "Gets the interfaces that are implemented by the current Windows Runtime class.");

    static PyObject* Object_iids_get(
        py::winrt_wrapper<winrt::Windows::Foundation::IInspectable>* self,
        void* /*unused*/) noexcept
    {
        try
        {
            auto name = winrt::get_interfaces(self->obj);
            return convert(name);
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyDoc_STRVAR(
        Object_runtime_class_name_doc,
        "Gets the fully qualified name of the current Windows Runtime object.");

    static PyObject* Object_runtime_class_name_get(
        py::winrt_wrapper<winrt::Windows::Foundation::IInspectable>* self,
        void* /*unused*/) noexcept
    {
        try
        {
            auto name = winrt::get_class_name(self->obj);
            return convert(name);
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyGetSetDef Object_getset[]
        = {{"_iids_",
            reinterpret_cast<getter>(Object_iids_get),
            nullptr,
            Object_iids_doc,
            nullptr},
           {"_runtime_class_name_",
            reinterpret_cast<getter>(Object_runtime_class_name_get),
            nullptr,
            Object_runtime_class_name_doc,
            nullptr},
           {}};

    static PyType_Slot Object_type_slots[]
        = {{Py_tp_doc, const_cast<char*>(Object_doc)},
           {Py_tp_new, reinterpret_cast<void*>(Object_new)},
           {Py_tp_dealloc, reinterpret_cast<void*>(Object_dealloc)},
           {Py_tp_getset, reinterpret_cast<void*>(Object_getset)},
           {}};

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
        PyObject* _key;
    };

    static PyMemberDef MappingIter_members[]
        = {{"_iter",
            T_OBJECT_EX,
            offsetof(MappingIter_object, _iter),
            0,
            PyDoc_STR("base KeyValuePair iterator")},
           {"_key", T_OBJECT, offsetof(MappingIter_object, _key), 0, nullptr},
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

        py::pyobj_handle key{PyUnicode_FromString("key")};
        if (!key)
        {
            return -1;
        }

        if (PyObject_SetAttrString(self, "_key", key.get()) == -1)
        {
            return -1;
        }

        return 0;
    }

    static PyObject* MappingIter_iternext(MappingIter_object* self) noexcept
    {
        if (!self->_iter)
        {
            PyErr_SetString(PyExc_RuntimeError, "_iter was deleted");
            return nullptr;
        }

        py::pyobj_handle next{PyIter_Next(self->_iter)};
        if (!next)
        {
            return nullptr;
        }

        if (!self->_key)
        {
            PyErr_SetString(PyExc_RuntimeError, "_key was deleted");
            return nullptr;
        }

        py::pyobj_handle key{PyObject_GetAttr(next.get(), self->_key)};
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
        .get_python_type = py::get_python_type,
        .wrap_mapping_iter = py::wrap_mapping_iter,
        .is_buffer_compatible = py::is_buffer_compatible,
        .convert_datetime = py::convert_datetime,
        .convert_to_datetime = py::convert_to_datetime,
        .convert_guid = py::convert_guid,
        .convert_to_guid = py::convert_to_guid,
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

    static PyObject* add_dll_directory(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        std::unique_ptr<wchar_t, decltype(&PyMem_Free)> path{
            PyUnicode_AsWideCharString(obj, nullptr), &PyMem_Free};

        if (!path)
        {
            return nullptr;
        }

        auto cookie = AddDllDirectory(path.get());

        if (!cookie)
        {
            PyErr_SetFromWindowsErr(GetLastError());
            return nullptr;
        }

        return PyLong_FromVoidPtr(cookie);
    }

    static PyObject* remove_dll_directory(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        auto cookie = PyLong_AsVoidPtr(obj);
        if (!cookie && PyErr_Occurred())
        {
            return nullptr;
        }

        if (!RemoveDllDirectory(cookie))
        {
            PyErr_SetFromWindowsErr(GetLastError());
            return nullptr;
        }

        Py_RETURN_NONE;
    }

    /**
     * Equivalent to functools.cache(functools.partial(uuid.UUID, None))
     *
     * This is a performance optimization since the UUID constructor is expensive.
     */
    static PyObject* wrap_uuid_constructor() noexcept
    {
        pyobj_handle uuid_module{PyImport_ImportModule("uuid")};
        if (!uuid_module)
        {
            return nullptr;
        }

        pyobj_handle uuid_type{PyObject_GetAttrString(uuid_module.get(), "UUID")};
        if (!uuid_type)
        {
            return nullptr;
        }

        pyobj_handle functools_module{PyImport_ImportModule("functools")};
        if (!functools_module)
        {
            return nullptr;
        }

        pyobj_handle partial_func{
            PyObject_GetAttrString(functools_module.get(), "partial")};
        if (!partial_func)
        {
            return nullptr;
        }

        pyobj_handle partial_uuid_func{PyObject_CallFunctionObjArgs(
            partial_func.get(), uuid_type.get(), Py_None, nullptr)};
        if (!partial_uuid_func)
        {
            return nullptr;
        }

        // REVISIT: we could probably implement a cache function in C++ that
        // would even more performant
        pyobj_handle cache_func{
            PyObject_GetAttrString(functools_module.get(), "cache")};
        if (!cache_func)
        {
            return nullptr;
        }

        return PyObject_CallOneArg(cache_func.get(), partial_uuid_func.get());
    }

    static int module_traverse(PyObject* module, visitproc visit, void* arg) noexcept
    {
        auto state = reinterpret_cast<module_state*>(PyModule_GetState(module));

        Py_VISIT(state->object_type);
        Py_VISIT(state->array_type);
        Py_VISIT(state->mapping_iter_type);
        Py_VISIT(state->to_uuid_func);

        for (const auto& [key, value] : state->type_cache)
        {
            Py_VISIT(value);
        }

        return 0;
    }

    static int module_clear(PyObject* module) noexcept
    {
        auto state = reinterpret_cast<module_state*>(PyModule_GetState(module));

        Py_CLEAR(state->object_type);
        Py_CLEAR(state->array_type);
        Py_CLEAR(state->mapping_iter_type);
        Py_CLEAR(state->to_uuid_func);

        auto type_cache = std::move(state->type_cache);

        for (auto& [key, value] : type_cache)
        {
            Py_XDECREF(value);
        }

        return 0;
    }

    static void module_free(PyObject* module) noexcept
    {
        auto state = reinterpret_cast<module_state*>(PyModule_GetState(module));

        Py_XDECREF(state->object_type);
        Py_XDECREF(state->array_type);
        Py_XDECREF(state->mapping_iter_type);
        Py_XDECREF(state->to_uuid_func);

        for (auto& [key, value] : state->type_cache)
        {
            Py_XDECREF(value);
        }

        std::destroy_at(&state->type_cache);
    }

    PyDoc_STRVAR(module_doc, "_winrt");

    static PyMethodDef module_methods[]{
        {"init_apartment", init_apartment, METH_O, "initialize the apartment"},
        {"uninit_apartment",
         uninit_apartment,
         METH_NOARGS,
         PyDoc_STR("uninitialize the apartment")},
        {"initialize_with_window",
         initialize_with_window,
         METH_VARARGS,
         PyDoc_STR(
             "interop function to invoke IInitializeWithWindow::Initialize on an object")},
        {"_add_dll_directory",
         add_dll_directory,
         METH_O,
         PyDoc_STR("Adds a directory to the DLL search path.")},
        {"_remove_dll_directory",
         remove_dll_directory,
         METH_O,
         PyDoc_STR(
             "Removes a directory that was added to the process DLL search path by using _add_dll_directory.")},
        {}};

    static PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt",
           module_doc,
           sizeof(module_state),
           module_methods,
           nullptr,
           module_traverse,
           module_clear,
           reinterpret_cast<freefunc>(module_free)};

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
        std::construct_at(&state->type_cache);

        py::pytype_handle object_type{py::register_python_type(
            module.get(), &Object_type_spec, nullptr, nullptr)};
        if (!object_type)
        {
            return nullptr;
        }

        py::pytype_handle array_type{
            py::register_python_type(module.get(), &Array_type_spec, nullptr, nullptr)};
        if (!array_type)
        {
            return nullptr;
        }

        py::pytype_handle mapping_iter_type{py::register_python_type(
            module.get(), &MappingIter_type_spec, nullptr, nullptr)};
        if (!mapping_iter_type)
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

        pyobj_handle to_uuid_func{wrap_uuid_constructor()};
        if (!to_uuid_func)
        {
            return nullptr;
        }

        state->object_type = object_type.detach();
        state->array_type = array_type.detach();
        state->mapping_iter_type = mapping_iter_type.detach();
        state->to_uuid_func = to_uuid_func.detach();

        return module.detach();
    }
} // namespace py::cpp::_winrt

PyMODINIT_FUNC PyInit__winrt(void) noexcept
{
    return py::cpp::_winrt::module_init();
}

py::cpp::_winrt::module_state* py::cpp::_winrt::get_module_state() noexcept
{
    auto module = PyState_FindModule(&py::cpp::_winrt::module_def);
    if (!module)
    {
        return nullptr;
    }

    return reinterpret_cast<py::cpp::_winrt::module_state*>(PyModule_GetState(module));
}

PyTypeObject* py::get_object_type() noexcept
{
    auto state = py::cpp::_winrt::get_module_state();
    if (!state)
    {
        return nullptr;
    }

    return state->object_type;
}
