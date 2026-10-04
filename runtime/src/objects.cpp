// A WinRT object as a Python object: the wrapper, and the types it has.
//
// A class or an interface is projected as a subclass of _winrt.Object holding
// one ABI pointer - the interface itself, or a class's default interface - and
// that pointer is all a wrapper is. Everything else about one of these types is
// here: how it is created from an activation factory, how it is seen as another
// interface, and the handful of attributes every one of them carries.

#include <Python.h>

#include <pywinrt/base.h>

#include "_winrt_buffer.h"
#include "arrays.h"
#include "compose.h"
#include "implements.h"
#include "interp.h"
#include "members.h"
#include "module_state.h"
#include "objects.h"
#include "protocols.h"
#include "pycollections.h"
#include "types.h"

#include <vector>

namespace py::interp
{
    // The one thing objects.h assumes, asserted where the type it is about is
    // in scope.
    static_assert(
        abi_offset
        == offsetof(winrt_wrapper<winrt::Windows::Foundation::IInspectable>, obj));

    /**
     * Wraps an ABI pointer that the caller owns in @p type, which must be a
     * wrapper type built from a table. Ownership moves to the wrapper.
     */
    PyObject* wrap_abi(PyTypeObject* type, void* abi) noexcept
    {
        if (!abi)
        {
            Py_RETURN_NONE;
        }

        if (auto* const started = python_object_of(abi))
        {
            static_cast<::IUnknown*>(abi)->Release();

            return started;
        }

        auto const self = type->tp_alloc(type, 0);
        if (!self)
        {
            static_cast<::IUnknown*>(abi)->Release();
            return nullptr;
        }

        abi_of(self) = abi;

        return self;
    }

    /**
     * The ABI pointer an @c _winrt.Object holds, queried for @p iid.
     *
     * @param iid The interface to query for, or @c nullptr for IInspectable.
     * @param info What the table says @p iid is, when the caller has it. It
     * is what a Python object that is no WinRT object at all is measured
     * against: a list is an IVector<T> and a dict is an IMap<K, V>, and only
     * the type record says which of those the caller is asking for.
     * @returns A pointer the caller owns a reference to, or @c nullptr when
     * @p obj is None.
     * @throws python_exception if @p obj is not a wrapped WinRT object or does
     * not implement @p iid.
     */
    void* unwrap_abi(PyObject* obj, void const* iid, type_entry* info)
    {
        throw_if_pyobj_null(obj);

        if (Py_IsNone(obj))
        {
            return nullptr;
        }

        auto const object_type = get_object_type();
        if (!object_type)
        {
            throw python_exception();
        }

        if (!PyObject_TypeCheck(obj, object_type))
        {
            // A Python class that derives from the public name of a projected
            // interface is saying it implements it, so what WinRT is given is
            // a COM object that stands for the Python one.
            if (implements_interfaces(Py_TYPE(obj)))
            {
                return make_implements_object(obj, iid);
            }

            if (info)
            {
                // A Python list, dict or iterable, which WinRT reads through
                // a collection the runtime assembles over it.
                if (auto* const collection = make_python_collection(*info, obj, iid))
                {
                    return collection;
                }
            }

            if (iid
                && *static_cast<winrt::guid const*>(iid)
                       == winrt::guid_of<winrt::Windows::Storage::Streams::IBuffer>())
            {
                // Anything that exports a Python buffer stands in for an
                // IBuffer, which is what DataWriter.write_buffer(b"...")
                // passes. The IBuffer holds the buffer for as long as WinRT
                // holds the IBuffer.
                auto buffer = convert_to_ibuffer(obj);
                return winrt::detach_abi(buffer);
            }

            PyErr_Format(
                PyExc_TypeError,
                "expected a WinRT object, not '%s'",
                Py_TYPE(obj)->tp_name);
            throw python_exception();
        }

        auto const abi = abi_of(obj);
        if (!abi)
        {
            PyErr_SetString(PyExc_RuntimeError, "the object holds nothing");
            throw python_exception();
        }

        // A wrapper of exactly the type asked for already holds the interface
        // it is asked for, which for a class is its default one.
        if (info && Py_IS_TYPE(obj, info->py_type))
        {
            static_cast<::IUnknown*>(abi)->AddRef();
            return abi;
        }

        void* result{};
        int32_t hr{};

        {
            // The object may be a proxy, which a query for an interface it
            // has not been asked for before is a call into the apartment of.
            auto _gil = release_gil();
            hr = static_cast<::IUnknown*>(abi)->QueryInterface(
                iid ? *static_cast<winrt::guid const*>(iid)
                    : winrt::guid_of<winrt::Windows::Foundation::IInspectable>(),
                &result);
        }

        if (hr != 0)
        {
            winrt::check_hresult(hr);
        }

        return result;
    }

    namespace
    {
        void release_interface_capsule(PyObject* capsule) noexcept
        {
            auto const abi = PyCapsule_GetPointer(capsule, interface_capsule_name);
            if (!abi)
            {
                PyErr_WriteUnraisable(capsule);
                return;
            }

            // The capsule may outlive the wrapper it was taken from, and then
            // this is the last reference, which may be to a proxy.
            auto _gil = release_gil();
            static_cast<::IUnknown*>(abi)->Release();
        }
    } // namespace

    /**
     * Hands @p abi, and the reference the caller owns to it, to a new
     * interface pointer capsule. The reference is released either way.
     */
    PyObject* new_interface_capsule(void* abi) noexcept
    {
        auto const capsule
            = PyCapsule_New(abi, interface_capsule_name, release_interface_capsule);
        if (!capsule)
        {
            static_cast<::IUnknown*>(abi)->Release();
        }

        return capsule;
    }

    /**
     * Wraps @p abi in @p type after querying it for @p iid, which is what an
     * argument marked @c query_interface asks for. Ownership of @p abi moves
     * to the wrapper either way.
     */
    PyObject* wrap_activated_abi(PyTypeObject* type, void const* iid, void* abi)
    {
        void* queried{};
        auto const hr = static_cast<::IUnknown*>(abi)->QueryInterface(
            *static_cast<winrt::guid const*>(iid), &queried);
        static_cast<::IUnknown*>(abi)->Release();
        if (hr != 0)
        {
            winrt::check_hresult(hr);
        }

        return wrap_abi(type, queried);
    }

    namespace
    {
        /**
         * The Python arguments of a call, as a fastcall array.
         */
        PyObject* const* tuple_items(PyObject* args) noexcept
        {
            return PyTuple_GET_SIZE(args) == 0 ? nullptr : &PyTuple_GET_ITEM(args, 0);
        }

        /**
         * The class @p type is, which for a Python class derived from a
         * composable one is the class it derives from.
         *
         * A derived class has a table entry of its own for nothing: what it
         * activates, and every member it inherits, is the WinRT class's.
         */
        type_entry* class_entry_of(PyTypeObject* type) noexcept
        {
            for (auto* base = type; base; base = base->tp_base)
            {
                if (auto* const entry = find_type_entry(base))
                {
                    return entry;
                }
            }

            return nullptr;
        }

        PyObject* class_new(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
        {
            if (kwds && PyDict_GET_SIZE(kwds) != 0)
            {
                set_invalid_kwd_args_error();
                return nullptr;
            }

            auto const info = class_entry_of(type);
            if (!info || !info->constructor)
            {
                set_invalid_activation_error(info ? info->winrt_name : type->tp_name);
                return nullptr;
            }

            auto const overload
                = select_overload(*info->constructor, PyTuple_GET_SIZE(args));

            if (!overload)
            {
                return nullptr;
            }

            if (type != info->py_type)
            {
                return make_composed_object(
                    type,
                    *info,
                    *info->constructor,
                    *overload,
                    tuple_items(args),
                    PyTuple_GET_SIZE(args));
            }

            return call_member(
                *info->constructor,
                *overload,
                nullptr,
                tuple_items(args),
                PyTuple_GET_SIZE(args));
        }

        /**
         * _from_(): the object seen as this type, which is what as_() calls.
         */
        PyObject* type_from(PyObject* cls, PyObject* arg) noexcept
        {
            auto const info = find_type_entry(reinterpret_cast<PyTypeObject*>(cls));
            if (!info || !info->guid)
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "'%s' is not an interface a WinRT object can be seen as",
                    reinterpret_cast<PyTypeObject*>(cls)->tp_name);
                return nullptr;
            }

            try
            {
                return wrap_abi(info->py_type, unwrap_abi(arg, info->guid));
            }
            catch (...)
            {
                to_PyErr();
                return nullptr;
            }
        }

        PyObject* type_guid(PyObject* cls, PyObject* /*unused*/) noexcept
        {
            auto const info = find_type_entry(reinterpret_cast<PyTypeObject*>(cls));
            if (!info || !info->guid)
            {
                PyErr_Format(
                    PyExc_AttributeError,
                    "'%s' has no IID",
                    reinterpret_cast<PyTypeObject*>(cls)->tp_name);
                return nullptr;
            }

            return convert_guid(*static_cast<winrt::guid const*>(info->guid));
        }

        /**
         * The abstract type an interface's public name is bound to stands for
         * an implementation it does not have, so only a subclass of it can be
         * instantiated. What a subclass gets is what object() would give it.
         */
        PyObject* implements_new(
            PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
        {
            // The abstract type is made with no bases, so its base is object,
            // and a class deriving from it has the abstract type or something
            // below it as its base instead: object before it in the bases is
            // an MRO conflict.
            if (type->tp_base == &PyBaseObject_Type)
            {
                WINRT_ASSERT(
                    find_type_entry(type) && find_type_entry(type)->implements == type);

                pyobj_handle name{PyType_GetName(type)};
                if (!name)
                {
                    return nullptr;
                }

                PyErr_Format(
                    PyExc_TypeError, "Can't instantiate abstract class %U", name.get());
                return nullptr;
            }

            // object.__new__() refuses arguments only for a type whose
            // __new__ it is, so the check it makes for a type that has no
            // __init__ of its own is made here instead.
            if (type->tp_init == PyBaseObject_Type.tp_init)
            {
                if (PyTuple_GET_SIZE(args) != 0)
                {
                    PyErr_Format(
                        PyExc_TypeError, "%s() takes no arguments", type->tp_name);
                    return nullptr;
                }

                if (kwds && PyDict_GET_SIZE(kwds) != 0)
                {
                    PyErr_Format(
                        PyExc_TypeError, "%s() takes no arguments", type->tp_name);
                    return nullptr;
                }
            }

            pyobj_handle no_args{PyTuple_New(0)};
            if (!no_args)
            {
                return nullptr;
            }

            return PyBaseObject_Type.tp_new(type, no_args.get(), nullptr);
        }

        PyMethodDef class_methods[]
            = {{"_from_", type_from, METH_O | METH_CLASS, nullptr},
               {"_from", deprecated_from, METH_O | METH_CLASS, nullptr},
               {"_assign_array_", type_assign_array, METH_O | METH_CLASS, nullptr},
               {}};

        PyMethodDef implements_methods[]
            = {{"_from_", type_from, METH_O | METH_CLASS, nullptr},
               {"_from", deprecated_from, METH_O | METH_CLASS, nullptr},
               {"_assign_array_", type_assign_array, METH_O | METH_CLASS, nullptr},
               {"_guid_", type_guid, METH_NOARGS | METH_CLASS, nullptr},
               {}};

        // A parameterized interface is only ever named with its type arguments
        // - IAsyncOperation[int] - so both the type the projection binds it to
        // and the one a Python implementation would derive from have to be
        // subscriptable. PEP 585 says what that means.
        PyMethodDef generic_wrapper_methods[]
            = {{"__class_getitem__",
                Py_GenericAlias,
                METH_O | METH_CLASS,
                PyDoc_STR("See PEP 585")},
               {}};

        PyMethodDef generic_implements_methods[]
            = {{"_from_", type_from, METH_O | METH_CLASS, nullptr},
               {"_from", deprecated_from, METH_O | METH_CLASS, nullptr},
               {"_assign_array_", type_assign_array, METH_O | METH_CLASS, nullptr},
               {"_guid_", type_guid, METH_NOARGS | METH_CLASS, nullptr},
               {"__class_getitem__",
                Py_GenericAlias,
                METH_O | METH_CLASS,
                PyDoc_STR("See PEP 585")},
               {}};
    } // namespace

    /**
     * Creates the Python type of an interface, and the abstract type its public
     * name is bound to.
     */
    bool make_interface_type(
        projection& proj, type_entry& entry, table::type_view const& record)
    {
        auto const generic = (record.flags() & table::type_flags::parameterized) != 0;

        // The IID of a parameterized interface is not an IID at all: it is
        // the seed a concrete instance's IID is hashed from, so it must not
        // be handed to QueryInterface.
        if (!generic)
        {
            entry.guid = record.guid();
        }

        type_members collected;

        if (!collect_members(proj, entry, record, collected))
        {
            return false;
        }

        auto const object_type = get_object_type();
        if (!object_type)
        {
            return false;
        }

        pyobj_handle bases{PyTuple_Pack(1, object_type)};
        if (!bases)
        {
            return false;
        }

        std::vector<PyType_Slot> slots;

        if (generic)
        {
            slots.push_back(
                {Py_tp_methods, reinterpret_cast<void*>(generic_wrapper_methods)});
        }

        slots.push_back(
            {Py_tp_getset,
             reinterpret_cast<void*>(keep_getsets(proj, collected.instance_getsets))});

        add_protocol_slots(record, slots);

        slots.push_back({});

        // The Python type of a parameterized interface is what each instance of
        // it derives from, so unlike every other wrapper this one is a base.
        PyType_Spec spec{
            entry.tp_name.c_str(),
            static_cast<int>(object_basicsize),
            0,
            Py_TPFLAGS_DEFAULT | (generic ? Py_TPFLAGS_BASETYPE : 0),
            slots.data()};

        pytype_handle type{
            register_python_type(proj.module, &spec, bases.get(), nullptr)};
        if (!type)
        {
            return false;
        }

        entry.py_type = type.detach();

        if (!remember(entry, entry.py_type))
        {
            return false;
        }

        // The public name of an interface is the abstract type a Python
        // implementation of it derives from, which also carries the IID
        // that isinstance() asks for.
        auto const implements_name
            = keep(proj, proj.module_name + "." + std::string{record.name()});

        PyType_Slot implements_slots[]
            = {{Py_tp_new, reinterpret_cast<void*>(implements_new)},
               {Py_tp_methods,
                reinterpret_cast<void*>(
                    generic ? generic_implements_methods : implements_methods)},
               {}};

        PyType_Spec implements_spec{
            implements_name,
            0,
            0,
            Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
            implements_slots};

        auto const metaclass = get_inspectable_meta_type();
        if (!metaclass)
        {
            return false;
        }

        pytype_handle implements{
            register_python_type(proj.module, &implements_spec, nullptr, metaclass)};
        if (!implements)
        {
            return false;
        }

        entry.implements = implements.detach();

        if (!remember(entry, entry.implements))
        {
            return false;
        }

        if (!bind_methods(collected, entry.py_type, nullptr))
        {
            return false;
        }

        return bind_protocol_methods(record, entry.py_type);
    }

    /**
     * Creates the Python type of a class, with the metaclass its statics sit on
     * when it has any.
     */
    bool make_class_type(
        projection& proj, type_entry& entry, table::type_view const& record)
    {
        if (auto const default_interface = record.default_interface();
            default_interface != table::no_ref)
        {
            entry.guid = proj.table->type(default_interface).guid();
        }

        entry.class_name = winrt::to_hstring(qualified(record));

        PyTypeObject* base_type{};

        if (auto const base = record.base_type(); base != table::no_ref)
        {
            base_type = ensure_referenced_type(proj, base);
            if (!base_type)
            {
                return false;
            }
        }

        type_members collected;

        if (!collect_members(proj, entry, record, collected))
        {
            return false;
        }

        // A class's statics live on a metaclass of its own, which has to
        // derive from the metaclass of its base for Python to accept the
        // pair. A class with no statics simply reuses one.
        auto* metaclass = base_type ? Py_TYPE(base_type) : get_inspectable_meta_type();
        if (!metaclass)
        {
            return false;
        }

        pytype_handle statics{};

        if (!collected.static_getsets.empty()
            || std::any_of(
                collected.methods.begin(),
                collected.methods.end(),
                [](auto const& entry)
                {
                    return entry.second;
                }))
        {
            auto const statics_name = keep(proj, entry.tp_name + "_Static");

            PyType_Slot statics_slots[] = {
                {Py_tp_getset,
                 reinterpret_cast<void*>(keep_getsets(proj, collected.static_getsets))},
                {}};

            PyType_Spec statics_spec{
                statics_name,
                static_cast<int>(PyType_Type.tp_basicsize),
                static_cast<int>(PyType_Type.tp_itemsize),
                Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
                statics_slots};

            pyobj_handle statics_bases{
                PyTuple_Pack(1, reinterpret_cast<PyObject*>(metaclass))};
            if (!statics_bases)
            {
                return false;
            }

            statics.attach(
                reinterpret_cast<PyTypeObject*>(
                    PyType_FromSpecWithBases(&statics_spec, statics_bases.get())));

            if (!statics)
            {
                return false;
            }

            metaclass = statics.get();
        }

        auto const object_type = get_object_type();
        if (!object_type)
        {
            return false;
        }

        pyobj_handle bases{PyTuple_Pack(
            1,
            base_type ? reinterpret_cast<PyObject*>(base_type)
                      : reinterpret_cast<PyObject*>(object_type))};
        if (!bases)
        {
            return false;
        }

        auto const composable = (record.flags() & table::type_flags::composable) != 0;
        auto const is_static = (record.flags() & table::type_flags::static_class) != 0;

        std::vector<PyType_Slot> slots;

        // _from_() and _assign_array_() say what to do with an instance, so a
        // static class, which has none, does not get them.
        if (entry.guid)
        {
            slots.push_back({Py_tp_methods, reinterpret_cast<void*>(class_methods)});
        }

        slots.push_back({Py_tp_new, reinterpret_cast<void*>(class_new)});
        slots.push_back(
            {Py_tp_getset,
             reinterpret_cast<void*>(keep_getsets(proj, collected.instance_getsets))});

        add_protocol_slots(record, slots);

        slots.push_back({});

        // A static class has no instances at all, and a composable one has
        // to be derivable because the projection derives from it.
        PyType_Spec spec{
            entry.tp_name.c_str(),
            is_static ? 0 : static_cast<int>(object_basicsize),
            0,
            Py_TPFLAGS_DEFAULT | (composable ? Py_TPFLAGS_BASETYPE : 0),
            slots.data()};

        pytype_handle type{register_python_type(
            proj.module, &spec, is_static ? nullptr : bases.get(), metaclass)};
        if (!type)
        {
            return false;
        }

        entry.py_type = type.detach();
        entry.statics = statics.detach();

        if (!remember(entry, entry.py_type))
        {
            return false;
        }

        if (entry.statics && !remember(entry, entry.statics))
        {
            return false;
        }

        if (!bind_methods(collected, entry.py_type, metaclass))
        {
            return false;
        }

        return bind_protocol_methods(record, entry.py_type);
    }

    /**
     * _from(): what _from_() was called until 4.0, kept so that code written
     * before as_() existed goes on working. It calls _from_() rather than
     * doing the same thing itself, so that it cannot come to differ.
     */
    PyObject* deprecated_from(PyObject* cls, PyObject* arg) noexcept
    {
        if (PyErr_WarnEx(
                PyExc_DeprecationWarning,
                "_from() is deprecated: use obj.as_(type) instead",
                1)
            < 0)
        {
            return nullptr;
        }

        pyobj_handle from{PyUnicode_InternFromString("_from_")};
        if (!from)
        {
            return nullptr;
        }

        return PyObject_CallMethodOneArg(cls, from.get(), arg);
    }

    /**
     * Refuses @p name, a parameterized interface named without its type
     * arguments, where one interface is meant.
     */
    void set_parameterized_type_error(char const* name) noexcept
    {
        PyErr_Format(
            PyExc_TypeError,
            "'%s' takes type arguments, so it names no one interface",
            name);
    }
} // namespace py::interp

namespace
{
    /**
     * Refuses @p name as a type whose values are wrapped WinRT objects.
     * @returns nullptr, for the caller to return.
     */
    py::interp::type_entry* not_an_object_type(char const* name) noexcept
    {
        PyErr_Format(
            PyExc_TypeError,
            "'%s' is not a type a projection table gives values of",
            name);
        return nullptr;
    }

    /**
     * The type entry of @p type when its values are wrapped WinRT objects: a
     * runtime class, or an interface by either of the types its names are
     * bound to. @p name is what the caller named the type by.
     *
     * @returns @c nullptr with a @c TypeError set for any other type.
     */
    py::interp::type_entry* find_object_entry(
        PyTypeObject* type, char const* name) noexcept
    {
        auto const info = py::interp::find_type_entry(type);
        if (!info)
        {
            return not_an_object_type(name);
        }

        if (info->parameterized)
        {
            py::interp::set_parameterized_type_error(name);
            return nullptr;
        }

        if (!info->guid)
        {
            return not_an_object_type(name);
        }

        // The metaclass that carries a class's statics is remembered with the
        // class's entry, and no value is of it.
        if (type == info->statics)
        {
            return not_an_object_type(name);
        }

        return info;
    }

    /**
     * Wraps @p value as the type of @p info, queried for the interface that
     * an instance of that type holds.
     */
    PyObject* wrap_as(
        winrt::Windows::Foundation::IInspectable const& value,
        py::interp::type_entry const& info) noexcept
    {
        void* abi{};

        auto const hr
            = static_cast<::IUnknown*>(winrt::get_abi(value))
                  ->QueryInterface(*static_cast<winrt::guid const*>(info.guid), &abi);
        if (hr != 0)
        {
            try
            {
                winrt::check_hresult(hr);
            }
            catch (...)
            {
                py::to_PyErr();
                return nullptr;
            }
        }

        return py::interp::wrap_abi(info.py_type, abi);
    }

    /**
     * The IID that @p obj names: a @c uuid.UUID, or a projected class or
     * interface, which names the interface an instance of it holds.
     *
     * @throws python_exception if @p obj is neither.
     */
    winrt::guid iid_named_by(PyObject* obj)
    {
        if (!PyType_Check(obj))
        {
            return py::convert_to_guid(obj);
        }

        auto const type = reinterpret_cast<PyTypeObject*>(obj);

        auto const info = find_object_entry(type, type->tp_name);
        if (!info)
        {
            throw py::python_exception();
        }

        return *static_cast<winrt::guid const*>(info->guid);
    }
} // namespace

/**
 * Wraps @p value as the type that @p qualified_name is bound to.
 *
 * This and py::unwrap_object() are how a compiled module hands a WinRT object
 * to Python and takes one back now that it shares no generated code with the
 * package that projects the type: it names the type and the runtime, which
 * built that type from a table, does the rest.
 */
PyObject* py::wrap_object(
    winrt::Windows::Foundation::IInspectable const& value,
    char const* qualified_name) noexcept
{
    if (!value)
    {
        Py_RETURN_NONE;
    }

    auto const type = get_python_type(qualified_name);
    if (!type)
    {
        return nullptr;
    }

    auto const info = find_object_entry(type, qualified_name);
    if (!info)
    {
        return nullptr;
    }

    return wrap_as(value, *info);
}

/**
 * The @p iid interface of the WinRT object @p obj wraps.
 *
 * @returns @c false with a Python error set. A null @p result with a @c true
 * return is None, which is the empty interface.
 */
bool py::unwrap_object(PyObject* obj, winrt::guid const& iid, void** result) noexcept
{
    try
    {
        *result = py::interp::unwrap_abi(obj, &iid);

        return true;
    }
    catch (...)
    {
        to_PyErr();
        return false;
    }
}

namespace py::cpp::_winrt
{
    /**
     * as_interface(obj, iid) - the @p iid interface of the WinRT object @p obj
     * wraps, as an interface pointer capsule, or None for None. @p iid is a
     * uuid.UUID, or a projected class or interface, which stands for the
     * interface an instance of it holds.
     *
     * This and wrap_interface() are how compiled code outside the runtime
     * exchanges WinRT objects with it: through Python objects, so that nothing
     * but the capsule's name and these two signatures is shared.
     */
    PyObject* as_interface(PyObject* /*unused*/, PyObject* args) noexcept
    {
        PyObject* obj;
        PyObject* iid_obj;

        if (!PyArg_ParseTuple(args, "OO:as_interface", &obj, &iid_obj))
        {
            return nullptr;
        }

        try
        {
            auto const iid = iid_named_by(iid_obj);
            auto const abi = py::interp::unwrap_abi(obj, &iid);

            if (!abi)
            {
                Py_RETURN_NONE;
            }

            return py::interp::new_interface_capsule(abi);
        }
        catch (...)
        {
            to_PyErr();
            return nullptr;
        }
    }

    /**
     * wrap_interface(capsule, type) - the WinRT object an interface pointer
     * capsule holds, as @p type, or None for None. @p type is a projected class
     * or interface, or the qualified name it is bound to. The capsule keeps its
     * own reference.
     */
    PyObject* wrap_interface(PyObject* /*unused*/, PyObject* args) noexcept
    {
        PyObject* capsule;
        PyObject* type_obj;

        if (!PyArg_ParseTuple(args, "OO:wrap_interface", &capsule, &type_obj))
        {
            return nullptr;
        }

        char const* qualified_name{};

        if (PyUnicode_Check(type_obj))
        {
            qualified_name = PyUnicode_AsUTF8(type_obj);
            if (!qualified_name)
            {
                return nullptr;
            }
        }
        else if (!PyType_Check(type_obj))
        {
            PyErr_Format(
                PyExc_TypeError,
                "wrap_interface() argument 2 must be a type or str, not %s",
                Py_TYPE(type_obj)->tp_name);
            return nullptr;
        }

        if (Py_IsNone(capsule))
        {
            Py_RETURN_NONE;
        }

        auto const abi
            = PyCapsule_GetPointer(capsule, py::interp::interface_capsule_name);
        if (!abi)
        {
            return nullptr;
        }

        winrt::Windows::Foundation::IInspectable value{nullptr};
        winrt::copy_from_abi(value, abi);

        if (qualified_name)
        {
            return wrap_object(value, qualified_name);
        }

        auto const type = reinterpret_cast<PyTypeObject*>(type_obj);

        auto const info = find_object_entry(type, type->tp_name);
        if (!info)
        {
            return nullptr;
        }

        return wrap_as(value, *info);
    }
} // namespace py::cpp::_winrt
