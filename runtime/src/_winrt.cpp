// must be included before winrt to avoid compile errors
#include <Shobjidl.h>
#include <libloaderapi.h>

#include <pywinrt/base.h>
#include "module_state.h"
#include "implements.h"
#include "members.h"
#include "objects.h"
#include "types.h"
#include <winrt/base.h>

#include <atomic>

namespace py::cpp::_winrt
{
    // BEGIN: class _winrt.Object_Static:

    /**
     * Whether @p cls is the wrapper type of the interface @p info describes,
     * as opposed to the interface's public name or a class.
     */
    static bool is_interface_wrapper(
        py::interp::type_entry const& info, PyObject* cls) noexcept
    {
        return info.category == py::table::category::interface_
               && reinterpret_cast<PyTypeObject*>(cls) == info.py_type;
    }

    /**
     * Whether @p type is a projected class or interface that implements the
     * interface whose IID is @p iid, or derives from one that does. A class
     * lists every interface it implements, and an interface every one it
     * requires, so nothing past the table records of @p type's bases needs to
     * be read, and nothing needs to be imported.
     */
    static bool implements_interface(PyTypeObject* type, winrt::guid const& iid)
    {
        auto const matches = [&iid](void const* guid)
        {
            return guid && *static_cast<winrt::guid const*>(guid) == iid;
        };

        auto* const mro = type->tp_mro;
        if (!mro)
        {
            return false;
        }

        for (Py_ssize_t i = 0; i < PyTuple_GET_SIZE(mro); i++)
        {
            auto* const base = PyTuple_GET_ITEM(mro, i);
            if (!PyType_Check(base))
            {
                continue;
            }

            auto* const entry
                = py::interp::find_type_entry(reinterpret_cast<PyTypeObject*>(base));
            if (!entry)
            {
                continue;
            }

            // The metaclass that carries a class's statics is remembered with
            // the class's entry, and it implements nothing.
            if (reinterpret_cast<PyTypeObject*>(base) == entry->statics)
            {
                continue;
            }

            if (matches(entry->guid))
            {
                return true;
            }

            auto const& table = *entry->owner->table;
            auto const interfaces = table.type(entry->index).interfaces();

            for (uint32_t j = 0; j < interfaces.size(); j++)
            {
                auto const record = table.type(interfaces[j]);
                if (record.flags() & py::table::type_flags::parameterized)
                {
                    continue;
                }

                if (matches(record.guid()))
                {
                    return true;
                }
            }
        }

        return false;
    }

    static PyObject* Object_Static_instancecheck(PyObject* cls, PyObject* obj) noexcept
    {
        try
        {
            // Ordinary type checking is the answer for a Python class that
            // derives from this one, which is how an implementation of the
            // interface says so.
            py::pyobj_handle derived{PyObject_CallMethod(
                reinterpret_cast<PyObject*>(&PyType_Type),
                "__instancecheck__",
                "OO",
                cls,
                obj)};
            if (!derived)
            {
                return nullptr;
            }

            if (PyObject_IsTrue(derived.get()))
            {
                return derived.detach();
            }

            auto const object_type = py::get_object_type();
            if (!object_type)
            {
                return nullptr;
            }

            // A Python implementation of an interface is a WinRT object too,
            // though it does not derive from Object, whose instances hold a
            // WinRT object it has none of.
            if (cls == reinterpret_cast<PyObject*>(object_type))
            {
                return PyBool_FromLong(py::interp::implements_interfaces(Py_TYPE(obj)));
            }

            // A class with no entry of its own is a Python class, which only
            // its own instances are instances of.
            auto const info
                = py::interp::find_type_entry(reinterpret_cast<PyTypeObject*>(cls));
            if (!info)
            {
                return derived.detach();
            }

            // The wrapper type an interface's instances are returned as is an
            // ordinary type; the interface's public name is what answers by
            // asking the object.
            if (is_interface_wrapper(*info, cls))
            {
                return derived.detach();
            }

            if (info->parameterized)
            {
                py::interp::set_parameterized_type_error(
                    reinterpret_cast<PyTypeObject*>(cls)->tp_name);
                return nullptr;
            }

            if (!info->guid)
            {
                // The class says nothing about an interface, so there is
                // nothing else to ask.
                return derived.detach();
            }

            if (!PyObject_TypeCheck(obj, object_type))
            {
                // The query below asks a WinRT object what it holds, which
                // only something that holds a WinRT object can answer. A
                // Python implementation answers for the interfaces its class
                // implements, as issubclass() does, which as_() agrees with.
                if (info->category != py::table::category::interface_)
                {
                    return derived.detach();
                }

                return PyBool_FromLong(implements_interface(
                    Py_TYPE(obj), *static_cast<winrt::guid const*>(info->guid)));
            }

            auto const& guid = *static_cast<winrt::guid const*>(info->guid);
            auto const& instance
                = reinterpret_cast<
                      py::winrt_wrapper<winrt::Windows::Foundation::IUnknown>*>(obj)
                      ->obj;
            if (!instance)
            {
                Py_RETURN_FALSE;
            }

            // The question as_() asks, rather than GetIids, whose list an
            // implementation may leave incomplete. The object may be a proxy,
            // which is asked through its apartment.
            winrt::com_ptr<::IUnknown> queried;
            int32_t hr{};

            {
                auto _gil = py::release_gil();
                hr = instance.as(guid, queried.put_void());
                queried = nullptr;
            }

            if (hr == winrt::impl::error_no_interface)
            {
                Py_RETURN_FALSE;
            }

            winrt::check_hresult(hr);

            Py_RETURN_TRUE;
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyObject* Object_Static_subclasscheck(
        PyObject* cls, PyObject* subclass) noexcept
    {
        try
        {
            // A Python class that derives from this one, or this one itself.
            py::pyobj_handle derived{PyObject_CallMethod(
                reinterpret_cast<PyObject*>(&PyType_Type),
                "__subclasscheck__",
                "OO",
                cls,
                subclass)};
            if (!derived)
            {
                return nullptr;
            }

            if (PyObject_IsTrue(derived.get()))
            {
                return derived.detach();
            }

            auto const object_type = py::get_object_type();
            if (!object_type)
            {
                return nullptr;
            }

            // As for instances: a Python implementation of an interface.
            // type.__subclasscheck__() has refused anything that is not a
            // class, but a class need not be a type object.
            if (cls == reinterpret_cast<PyObject*>(object_type))
            {
                return PyBool_FromLong(
                    PyType_Check(subclass)
                    && py::interp::implements_interfaces(
                        reinterpret_cast<PyTypeObject*>(subclass)));
            }

            auto const info
                = py::interp::find_type_entry(reinterpret_cast<PyTypeObject*>(cls));
            if (!info)
            {
                return derived.detach();
            }

            // As for instances.
            if (is_interface_wrapper(*info, cls))
            {
                return derived.detach();
            }

            if (info->parameterized)
            {
                py::interp::set_parameterized_type_error(
                    reinterpret_cast<PyTypeObject*>(cls)->tp_name);
                return nullptr;
            }

            // A projected class does not derive from its interfaces in Python,
            // which is the one relation the table has to answer for. Between
            // classes, deriving is what a composable class's subclass does.
            if (info->category != py::table::category::interface_)
            {
                return derived.detach();
            }

            if (!info->guid)
            {
                return derived.detach();
            }

            // type.__subclasscheck__() has refused anything that is not a
            // class, but a class need not be a type object.
            if (!PyType_Check(subclass))
            {
                return derived.detach();
            }

            return PyBool_FromLong(implements_interface(
                reinterpret_cast<PyTypeObject*>(subclass),
                *static_cast<winrt::guid const*>(info->guid)));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef Object_Static_methods[]
        = {{"__instancecheck__",
            reinterpret_cast<PyCFunction>(Object_Static_instancecheck),
            METH_O,
            nullptr},
           {"__subclasscheck__",
            reinterpret_cast<PyCFunction>(Object_Static_subclasscheck),
            METH_O,
            nullptr},
           {}};

    static PyType_Slot Object_Static_type_slots[]
        = {{Py_tp_base, reinterpret_cast<void*>(&PyType_Type)},
           {Py_tp_methods, reinterpret_cast<void*>(Object_Static_methods)},
           {}};

    static PyType_Spec Object_Static_type_spec
        = {"winrt._winrt.Object_Static",
           0,
           0,
           Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
           Object_Static_type_slots};

    // END: class _winrt.Object_Static:

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

        // The object is often a proxy for one that lives in another
        // apartment, and releasing a proxy is a call into that apartment,
        // which may at that moment be waiting for the GIL to call back into
        // Python, such as to run the completed handler of an async operation.
        if (self->obj)
        {
            auto _gil = py::release_gil();
            self->obj = nullptr;
        }

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
        return py::interp::iids_of(self->obj);
    }

    PyDoc_STRVAR(
        Object_runtime_class_name_doc,
        "Gets the fully qualified name of the current Windows Runtime object.");

    static PyObject* Object_runtime_class_name_get(
        py::winrt_wrapper<winrt::Windows::Foundation::IInspectable>* self,
        void* /*unused*/) noexcept
    {
        return py::interp::runtime_class_name_of(self->obj);
    }

    /**
     * _from_(): the object seen as IInspectable, which is what as_(Object)
     * calls, as as_() calls _from_() of any other type it is given. Every
     * projected type has a _from_() of its own, so this one answers for
     * winrt.system.Object alone.
     */
    static PyObject* Object_from(PyObject* cls, PyObject* arg) noexcept
    {
        auto const object_type = py::get_object_type();
        if (!object_type)
        {
            return nullptr;
        }

        if (cls != reinterpret_cast<PyObject*>(object_type))
        {
            PyErr_Format(
                PyExc_TypeError,
                "'%s' is not an interface a WinRT object can be seen as",
                reinterpret_cast<PyTypeObject*>(cls)->tp_name);
            return nullptr;
        }

        try
        {
            return py::interp::wrap_abi(
                object_type, py::interp::unwrap_abi(arg, nullptr));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    static PyMethodDef Object_methods[]
        = {{"as_", py::interp::object_as, METH_O, nullptr},
           {"_from_", Object_from, METH_O | METH_CLASS, nullptr},
           {"_from", py::interp::deprecated_from, METH_O | METH_CLASS, nullptr},
           {}};

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

    static PyObject* Object_richcompare(
        py::winrt_wrapper<winrt::Windows::Foundation::IInspectable>* self,
        PyObject* other,
        int op) noexcept
    {
        if (op != Py_EQ && op != Py_NE)
        {
            Py_RETURN_NOTIMPLEMENTED;
        }

        auto const object_type = py::get_object_type();
        if (!object_type)
        {
            return nullptr;
        }

        // Only another wrapper can hold the same object: anything else would
        // be stood up as a new one to compare, so it is left to answer for
        // itself, and Python falls back on identity if it does not. What is
        // compared is identity, which a proxy answers itself.
        if (!PyObject_TypeCheck(other, object_type))
        {
            Py_RETURN_NOTIMPLEMENTED;
        }

        auto const equal
            = self->obj
              == reinterpret_cast<
                     py::winrt_wrapper<winrt::Windows::Foundation::IUnknown>*>(other)
                     ->obj;

        return PyBool_FromLong(op == Py_EQ ? equal : !equal);
    }

    static Py_hash_t Object_hash(
        py::winrt_wrapper<winrt::Windows::Foundation::IInspectable>* self) noexcept
    {
        return static_cast<Py_hash_t>(
            std::hash<winrt::Windows::Foundation::IInspectable>{}(self->obj));
    }

    static PyType_Slot Object_type_slots[]
        = {{Py_tp_doc, const_cast<char*>(Object_doc)},
           {Py_tp_new, reinterpret_cast<void*>(Object_new)},
           {Py_tp_dealloc, reinterpret_cast<void*>(Object_dealloc)},
           {Py_tp_methods, reinterpret_cast<void*>(Object_methods)},
           {Py_tp_getset, reinterpret_cast<void*>(Object_getset)},
           {Py_tp_richcompare, reinterpret_cast<void*>(Object_richcompare)},
           {Py_tp_hash, reinterpret_cast<void*>(Object_hash)},
           {}};

    // Every wrapper type the runtime builds from a table inherits this one and
    // is laid out as py::winrt_wrapper<T>. The basic size is taken from
    // <pywinrt/abi.h> rather than spelled again here so that the asserts that
    // guard the layout guard this too.
    static PyType_Spec Object_type_spec
        = {"winrt.system.Object",
           py::object_basicsize,
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
        PyObject_HEAD
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

        auto arg_count = PyTuple_GET_SIZE(args);
        if (arg_count != 1)
        {
            PyErr_SetString(PyExc_TypeError, "requires a single argument");
            return -1;
        }

        // borrowed ref
        auto base_iter = PyTuple_GET_ITEM(args, 0);

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
        = {"winrt._winrt.MappingIter",
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
     * uuid.UUID.
     */
    static PyTypeObject* import_uuid_type() noexcept
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

        if (!PyType_Check(uuid_type.get()))
        {
            PyErr_SetString(PyExc_TypeError, "uuid.UUID is not a type");
            return nullptr;
        }

        return reinterpret_cast<PyTypeObject*>(uuid_type.detach());
    }

    /**
     * Equivalent to functools.cache(functools.partial(uuid.UUID, None))
     *
     * This is a performance optimization since the UUID constructor is expensive.
     */
    static PyObject* wrap_uuid_constructor(PyTypeObject* uuid_type) noexcept
    {
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
            partial_func.get(),
            reinterpret_cast<PyObject*>(uuid_type),
            Py_None,
            nullptr)};
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

    /**
     * The state of the module, for code that is not handed the module.
     *
     * The runtime refuses to load into any interpreter but the main one, so
     * there is one state at a time. Importing the module again after it was
     * taken out of sys.modules executes a second one, which replaces the
     * first here, and freeing a state clears this only if it is still that
     * state.
     */
    static std::atomic<module_state*> main_state{};

    static int module_traverse(PyObject* module, visitproc visit, void* arg) noexcept
    {
        auto state = reinterpret_cast<module_state*>(PyModule_GetState(module));

        Py_VISIT(state->object_meta_type.get());
        Py_VISIT(state->object_type.get());
        Py_VISIT(state->array_type.get());
        Py_VISIT(state->mapping_iter_type.get());
        Py_VISIT(state->projected_method_type.get());
        Py_VISIT(state->to_uuid_func.get());
        Py_VISIT(state->uuid_type.get());
        Py_VISIT(state->wrap_async_func);

        for (const auto& [key, value] : state->type_cache)
        {
            Py_VISIT(value.get());
        }

        return 0;
    }

    static int module_clear(PyObject* module) noexcept
    {
        auto state = reinterpret_cast<module_state*>(PyModule_GetState(module));

        state->object_meta_type.close();
        state->object_type.close();
        state->array_type.close();
        state->mapping_iter_type.close();
        state->projected_method_type.close();
        state->to_uuid_func.close();
        state->uuid_type.close();
        Py_CLEAR(state->wrap_async_func);

        // Nothing here takes the cache lock, and traverse and free do not
        // either. The collector calls them with the world stopped on a
        // free-threaded build, so a thread paused holding the lock would never
        // let go of it.
        //
        // The types a projection built hold descriptors that point back at it,
        // so letting go of them here is what breaks the cycle. The descriptors
        // themselves are freed with the state, below, because a type that
        // outlives this call still has them bound.
        state->type_entries.clear();
        state->generic_types.clear();

        for (auto& [name, projection] : state->projections)
        {
            projection->release_types();
            projection->failure.close();
        }

        // Its references are released as it goes out of scope, with the map
        // in the state already empty.
        auto type_cache = std::move(state->type_cache);

        return 0;
    }

    static void module_free(PyObject* module) noexcept
    {
        auto state = reinterpret_cast<module_state*>(PyModule_GetState(module));

        auto expected = state;
        main_state.compare_exchange_strong(
            expected, nullptr, std::memory_order_acq_rel);

        std::destroy_at(&state->object_meta_type);
        std::destroy_at(&state->object_type);
        std::destroy_at(&state->array_type);
        std::destroy_at(&state->mapping_iter_type);
        std::destroy_at(&state->projected_method_type);
        std::destroy_at(&state->to_uuid_func);
        std::destroy_at(&state->uuid_type);
        Py_XDECREF(state->wrap_async_func);

        std::destroy_at(&state->type_cache);

        std::destroy_at(&state->type_entries);
        std::destroy_at(&state->generic_types);
        std::destroy_at(&state->buffer_exports);

        std::destroy_at(&state->projections);
    }

    // Not using a header file for thes because setuptools doesn't have a nice
    // way to pick up private header files.
    PyObject* box_boolean(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_int8(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_uint8(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_int16(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_uint16(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_int32(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_uint32(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_int64(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_uint64(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_single(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_double(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_char16(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_string(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_guid(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_date_time(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_time_span(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_boolean(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_int8(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_uint8(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_int16(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_uint16(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_int32(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_uint32(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_int64(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_uint64(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_single(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_double(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_char16(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_string(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_guid(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_date_time(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_time_span(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* read_table(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* as_interface(PyObject* /*unused*/, PyObject* args) noexcept;
    PyObject* wrap_interface(PyObject* /*unused*/, PyObject* args) noexcept;
    PyObject* hresult_error(PyObject* /*unused*/, PyObject* args) noexcept;

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
        {"add_dll_directory",
         add_dll_directory,
         METH_O,
         PyDoc_STR("Adds a directory to the DLL search path.")},
        {"remove_dll_directory",
         remove_dll_directory,
         METH_O,
         PyDoc_STR(
             "Removes a directory that was added to the process DLL search path by using add_dll_directory.")},
        {"box_boolean", box_boolean, METH_O, PyDoc_STR("Box a Boolean value")},
        {"box_char16", box_char16, METH_O, PyDoc_STR("Box a Char16 value")},
        {"box_string", box_string, METH_O, PyDoc_STR("Box a string value")},
        {"box_int8", box_int8, METH_O, PyDoc_STR("Box a Int8 value")},
        {"box_uint8", box_uint8, METH_O, PyDoc_STR("Box a UInt8 value")},
        {"box_int16", box_int16, METH_O, PyDoc_STR("Box a Int16 value")},
        {"box_uint16", box_uint16, METH_O, PyDoc_STR("Box a UInt16 value")},
        {"box_int32", box_int32, METH_O, PyDoc_STR("Box a Int32 value")},
        {"box_uint32", box_uint32, METH_O, PyDoc_STR("Box a UInt32 value")},
        {"box_int64", box_int64, METH_O, PyDoc_STR("Box a Int64 value")},
        {"box_uint64", box_uint64, METH_O, PyDoc_STR("Box a UInt64 value")},
        {"box_single", box_single, METH_O, PyDoc_STR("Box a Single value")},
        {"box_double", box_double, METH_O, PyDoc_STR("Box a Double value")},
        {"box_guid", box_guid, METH_O, PyDoc_STR("Box a GUID value")},
        {"box_date_time",
         box_date_time,
         METH_O,
         PyDoc_STR("Box a Windows.Foundation.DateTime value")},
        {"box_time_span",
         box_time_span,
         METH_O,
         PyDoc_STR("Box a Windows.Foundation.TimeSpan value")},
        {"unbox_boolean", unbox_boolean, METH_O, PyDoc_STR("Unbox a Boolean value")},
        {"unbox_char16", unbox_char16, METH_O, PyDoc_STR("Unbox a Char16 value")},
        {"unbox_string", unbox_string, METH_O, PyDoc_STR("Unbox a string value")},
        {"unbox_int8", unbox_int8, METH_O, PyDoc_STR("Unbox a Int8 value")},
        {"unbox_uint8", unbox_uint8, METH_O, PyDoc_STR("Unbox a UInt8 value")},
        {"unbox_int16", unbox_int16, METH_O, PyDoc_STR("Unbox a Int16 value")},
        {"unbox_uint16", unbox_uint16, METH_O, PyDoc_STR("Unbox a UInt16 value")},
        {"unbox_int32", unbox_int32, METH_O, PyDoc_STR("Unbox a Int32 value")},
        {"unbox_uint32", unbox_uint32, METH_O, PyDoc_STR("Unbox a UInt32 value")},
        {"unbox_int64", unbox_int64, METH_O, PyDoc_STR("Unbox a Int64 value")},
        {"unbox_uint64", unbox_uint64, METH_O, PyDoc_STR("Unbox a UInt64 value")},
        {"unbox_single", unbox_single, METH_O, PyDoc_STR("Unbox a Single value")},
        {"unbox_double", unbox_double, METH_O, PyDoc_STR("Unbox a Double value")},
        {"unbox_guid", unbox_guid, METH_O, PyDoc_STR("Unbox a GUID value")},
        {"unbox_date_time",
         unbox_date_time,
         METH_O,
         PyDoc_STR("Unbox a Windows.Foundation.DateTime value")},
        {"unbox_time_span",
         unbox_time_span,
         METH_O,
         PyDoc_STR("Unbox a Windows.Foundation.TimeSpan value")},
        {"load_projection",
         py::interp::load_projection,
         METH_VARARGS,
         PyDoc_STR(
             "Builds the Python types of one WinRT namespace from its projection table. "
             "Called by winrt.runtime._internals.load_projection(), which is the first "
             "statement of a projection package's __init__.py.")},
        {"read_table",
         read_table,
         METH_O,
         PyDoc_STR(
             "Reads a projection table and returns its contents as plain Python objects. "
             "This is how test/test_table.py checks that the generator's writer and the "
             "runtime's reader agree on the format.")},
        {"as_interface",
         as_interface,
         METH_VARARGS,
         PyDoc_STR(
             "An interface of a WinRT object, as an interface pointer capsule. "
             "This and wrap_interface() are how the interop packages hand WinRT "
             "objects to their compiled code and take them back.")},
        {"wrap_interface",
         wrap_interface,
         METH_VARARGS,
         PyDoc_STR("The WinRT object an interface pointer capsule holds.")},
        {"hresult_error",
         hresult_error,
         METH_VARARGS,
         PyDoc_STR(
             "The exception a WinRT call that failed with an HRESULT raises, for "
             "the interop packages to raise in turn.")},
        {}};

    static int module_exec(PyObject* module) noexcept
    {
        static const auto kMTA
            = static_cast<long>(winrt::apartment_type::multi_threaded);
        static const auto kSTA
            = static_cast<long>(winrt::apartment_type::single_threaded);

        // CPython allocates the state zeroed just before this runs, and does
        // not call traverse, clear or free before it has, so constructing the
        // handles and maps first is what lets those three assume them.
        auto state = reinterpret_cast<module_state*>(PyModule_GetState(module));
        std::construct_at(&state->object_meta_type);
        std::construct_at(&state->object_type);
        std::construct_at(&state->array_type);
        std::construct_at(&state->mapping_iter_type);
        std::construct_at(&state->projected_method_type);
        std::construct_at(&state->to_uuid_func);
        std::construct_at(&state->uuid_type);
        std::construct_at(&state->type_cache);
        std::construct_at(&state->projections);
        std::construct_at(&state->type_entries);
        std::construct_at(&state->generic_types);
        std::construct_at(&state->buffer_exports);

        // The slot below says the same, but CPython only enforces it in an
        // interpreter configured to check, which a legacy subinterpreter on a
        // build with the GIL is not.
        if (PyInterpreterState_Get() != PyInterpreterState_Main())
        {
            PyErr_SetString(
                PyExc_ImportError,
                "winrt._winrt can only be imported into the main interpreter");
            return -1;
        }

        py::pytype_handle object_meta_type{py::register_python_type(
            module, &Object_Static_type_spec, nullptr, nullptr)};
        if (!object_meta_type)
        {
            return -1;
        }

        // Object's metaclass is what makes a Python implementation of an
        // interface an instance of it, as the type hints say.
        py::pytype_handle object_type{py::register_python_type(
            module, &Object_type_spec, nullptr, object_meta_type.get())};
        if (!object_type)
        {
            return -1;
        }

        py::pytype_handle array_type{
            py::register_python_type(module, &Array_type_spec, nullptr, nullptr)};
        if (!array_type)
        {
            return -1;
        }

        py::pytype_handle mapping_iter_type{
            py::register_python_type(module, &MappingIter_type_spec, nullptr, nullptr)};
        if (!mapping_iter_type)
        {
            return -1;
        }

        // Not added to the module: it is what a projected method is bound as,
        // and nothing constructs one from Python.
        py::pytype_handle projected_method_type{reinterpret_cast<PyTypeObject*>(
            PyType_FromSpec(&py::interp::projected_method_type_spec))};
        if (!projected_method_type)
        {
            return -1;
        }

        if (PyModule_AddIntConstant(module, "MTA", kMTA) == -1)
        {
            return -1;
        }

        if (PyModule_AddIntConstant(module, "STA", kSTA) == -1)
        {
            return -1;
        }

        py::pytype_handle uuid_type{import_uuid_type()};
        if (!uuid_type)
        {
            return -1;
        }

        pyobj_handle to_uuid_func{wrap_uuid_constructor(uuid_type.get())};
        if (!to_uuid_func)
        {
            return -1;
        }

        state->object_meta_type = std::move(object_meta_type);
        state->object_type = std::move(object_type);
        state->array_type = std::move(array_type);
        state->mapping_iter_type = std::move(mapping_iter_type);
        state->projected_method_type = std::move(projected_method_type);
        state->to_uuid_func = std::move(to_uuid_func);
        state->uuid_type = std::move(uuid_type);
        state->wrap_async_func = nullptr; // lazy-initialized

        main_state.store(state, std::memory_order_release);

        return 0;
    }

    static PyModuleDef_Slot module_slots[]{
        {Py_mod_exec, reinterpret_cast<void*>(module_exec)},
#ifdef Py_mod_multiple_interpreters
        // A WinRT callback that arrives on a thread Python has not seen
        // attaches through PyGILState_Ensure(), which only knows the main
        // interpreter, so in any other interpreter it would run the Python
        // code of one interpreter in another.
        {Py_mod_multiple_interpreters, Py_MOD_MULTIPLE_INTERPRETERS_NOT_SUPPORTED},
#endif
#ifdef Py_mod_gil
        {Py_mod_gil, Py_MOD_GIL_NOT_USED},
#endif
        {}};

    PyDoc_STRVAR(module_doc, "_winrt");

    static PyModuleDef module_def
        = {PyModuleDef_HEAD_INIT,
           "_winrt",
           module_doc,
           sizeof(module_state),
           module_methods,
           module_slots,
           module_traverse,
           module_clear,
           reinterpret_cast<freefunc>(module_free)};
} // namespace py::cpp::_winrt

PyMODINIT_FUNC PyInit__winrt(void) noexcept
{
    return PyModuleDef_Init(&py::cpp::_winrt::module_def);
}

/**
 * The state of the module, or @c nullptr with a Python error set when
 * winrt._winrt is not loaded.
 */
py::cpp::_winrt::module_state* py::cpp::_winrt::get_module_state() noexcept
{
    auto const state = try_get_module_state();
    if (!state)
    {
        PyErr_SetString(PyExc_SystemError, "winrt-runtime is not loaded");
    }

    return state;
}

/**
 * The state of the module, or @c nullptr with no Python error set when
 * winrt._winrt is not loaded, for a caller whose own @c nullptr means that it
 * found nothing.
 */
py::cpp::_winrt::module_state* py::cpp::_winrt::try_get_module_state() noexcept
{
    return py::cpp::_winrt::main_state.load(std::memory_order_acquire);
}

/**
 * The metaclass of the projected types, or @c nullptr with a Python error set
 * when winrt._winrt is not loaded.
 */
PyTypeObject* py::get_object_meta_type() noexcept
{
    auto state = py::cpp::_winrt::get_module_state();
    if (!state)
    {
        return nullptr;
    }

    return state->object_meta_type.get();
}

/**
 * winrt.system.Object, or @c nullptr with a Python error set when winrt._winrt
 * is not loaded.
 */
PyTypeObject* py::get_object_type() noexcept
{
    auto state = py::cpp::_winrt::get_module_state();
    if (!state)
    {
        return nullptr;
    }

    return state->object_type.get();
}
