// A WinRT object as a Python object: the wrapper, and the types it has.
//
// A class or an interface is projected as a subclass of _winrt.Object holding
// one ABI pointer - the interface itself, or a class's default interface - and
// that pointer is all a wrapper is. Everything else about one of these types is
// here: how it is created from an activation factory, how it is seen as another
// interface, and the handful of attributes every one of them carries.

#include <Python.h>

#include "_winrt.h"
#include "_winrt_buffer.h"
#include "com_array.h"
#include "compose.h"
#include "convert.h"
#include "errors.h"
#include "handles.h"
#include "implements.h"
#include "interp.h"
#include "members.h"
#include "objects.h"
#include "prelude.h"
#include "protocols.h"
#include "pycollections.h"
#include "runtime.h"
#include "table.h"
#include "types.h"
#include "wrappers.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
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

    namespace
    {
        /**
         * The winrt.system function that boxes @p obj, for a Python value of a
         * type WinRT boxes, or @c nullptr. An int is named for the width it
         * is most often meant as.
         */
        const char* box_function_for(PyObject* obj) noexcept
        {
            if (PyBool_Check(obj))
            {
                return "box_boolean()";
            }

            if (PyLong_Check(obj))
            {
                return "box_int32()";
            }

            if (PyFloat_Check(obj))
            {
                return "box_double()";
            }

            if (PyUnicode_Check(obj))
            {
                return "box_string()";
            }

            return nullptr;
        }
    } // namespace

    /**
     * The ABI pointer an @c _winrt.Object holds, queried for @p iid.
     *
     * @param iid The interface to query for, or @c nullptr for IInspectable.
     * @param info What the table says @p iid is, when the caller has it. It
     * is what a Python object that is no WinRT object at all is measured
     * against: a list is an IVector<T> and a dict is an IMap<K, V>, and only
     * the type record says which of those the caller is asking for.
     * @param fill WinRT writes into the IBuffer @p obj is passed as, so a
     * Python buffer has to be writable.
     * @returns A pointer the caller owns a reference to, or @c nullptr when
     * @p obj is None.
     * @throws python_exception if @p obj is not a wrapped WinRT object or does
     * not implement @p iid.
     */
    void* unwrap_abi(PyObject* obj, void const* iid, type_entry* info, bool fill)
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
                auto buffer = convert_to_ibuffer(obj, fill);
                return winrt::detach_abi(buffer);
            }

            // A number or a str where any object will do is a value that has
            // to be boxed first, which is not something to guess at.
            if (!iid)
            {
                if (auto const box = box_function_for(obj))
                {
                    PyErr_Format(
                        PyExc_TypeError,
                        "expected a WinRT object, not '%s': box it with "
                        "winrt.system.%s or another box function",
                        Py_TYPE(obj)->tp_name,
                        box);
                    throw python_exception();
                }
            }

            PyErr_Format(
                PyExc_TypeError,
                "expected a WinRT object, not '%s'",
                Py_TYPE(obj)->tp_name);
            throw python_exception();
        }

        auto const abi = instance_abi(obj);
        if (!abi)
        {
            throw python_exception();
        }

        // A wrapper of exactly the type asked for already holds the interface
        // it is asked for, which for a class is its default one.
        if (info && Py_IS_TYPE(obj, info->py_type.get()))
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

    /**
     * Says that @p self holds no WinRT object, as an instance of a Python
     * class derived from a composable one does until its __init__() has
     * composed it.
     */
    void set_not_composed_error(PyObject* self) noexcept
    {
        PyErr_Format(
            PyExc_RuntimeError,
            "'%s' object holds no WinRT object until its __init__() calls "
            "super().__init__()",
            Py_TYPE(self)->tp_name);
    }

    /**
     * Says that @p self is composed already, or is being composed on another
     * thread, so cannot be composed again.
     */
    void set_already_composed_error(PyObject* self) noexcept
    {
        PyErr_Format(
            PyExc_TypeError,
            "'%s' object is already composed into a WinRT object, so it cannot "
            "be initialized again",
            Py_TYPE(self)->tp_name);
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

    /**
     * The WinRT class @p type is, or the one it derives from when it is a
     * Python class derived from a composable one, or @c nullptr when it is
     * neither.
     *
     * A derived class has a table entry of its own for nothing: what it
     * activates, and every member it inherits, is the WinRT class's.
     *
     * The first type with an entry decides. The abstract type of an interface
     * has the layout of object, which a WinRT class's extends, so in a class
     * derived from both, tp_base is in the WinRT class's chain of bases. A
     * chain that reaches an interface's type, as a Python implementation's
     * does, reaches no class after it.
     */
    type_entry* class_of(PyTypeObject* type) noexcept
    {
        for (auto* base = type; base; base = base->tp_base)
        {
            auto* const entry = find_type_entry(base);
            if (!entry)
            {
                continue;
            }

            if (entry->category != table::category::class_)
            {
                return nullptr;
            }

            return entry;
        }

        return nullptr;
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

        int class_init(PyObject* self, PyObject* args, PyObject* kwds) noexcept;

        /**
         * Which call to a constructor a call is, which the errors for one
         * that no constructor takes say.
         */
        enum class constructor_call
        {
            // The call to the class, or the one a derived class's __init__()
            // makes to super().__init__(), whose arguments are for the
            // constructor.
            direct,

            // The call to a derived class, once its __init__() has returned
            // without calling super().__init__(), whose arguments are
            // __init__()'s.
            after_init,
        };

        /**
         * Refuses a call to @p type, which is or derives from the class
         * @p info, that no constructor of it takes: one with keyword
         * arguments, or any call to a class without a constructor. @p call
         * says which call it is.
         *
         * @p info can be @c nullptr only with constructor_call::direct, and
         * is refused then as a class that cannot be created.
         *
         * @returns @c true, or @c false with a Python error set.
         */
        bool check_call(
            PyTypeObject* type,
            type_entry const* info,
            PyObject* kwds,
            constructor_call call) noexcept
        {
            WINRT_ASSERT(info || call == constructor_call::direct);

            if (kwds && PyDict_GET_SIZE(kwds) != 0)
            {
                if (call == constructor_call::direct)
                {
                    set_invalid_kwd_args_error();
                    return false;
                }

                PyErr_Format(
                    PyExc_TypeError,
                    "%s.__init__() did not call super().__init__(), and a call "
                    "with keyword arguments fits no constructor of '%s'",
                    type->tp_name,
                    info->winrt_name);
                return false;
            }

            if (!info)
            {
                set_invalid_activation_error(type->tp_name);
                return false;
            }

            if (!info->constructor)
            {
                set_invalid_activation_error(info->winrt_name);
                return false;
            }

            return true;
        }

        /**
         * Composes @p self, an instance of a Python class derived from the
         * composable class @p info that holds nothing yet, with the
         * constructor overload that a call with @p args and @p kwds takes.
         * class_new(), class_init() and compose_after_init() each compose an
         * object with this, and @p call says which call it is.
         *
         * @p info can be @c nullptr only with constructor_call::direct, and
         * is refused then as a class that cannot be created.
         *
         * @returns @c true, or @c false with a Python error set.
         */
        bool compose_from_call(
            PyObject* self,
            type_entry* info,
            PyObject* args,
            PyObject* kwds,
            constructor_call call) noexcept
        {
            if (!check_call(Py_TYPE(self), info, kwds, call))
            {
                return false;
            }

            auto const overload
                = select_overload(*info->constructor, PyTuple_GET_SIZE(args));

            if (!overload)
            {
                if (call == constructor_call::direct)
                {
                    return false;
                }

                PyErr_Clear();
                PyErr_Format(
                    PyExc_TypeError,
                    "%s.__init__() did not call super().__init__(), and a call "
                    "with %zd arguments fits no constructor of '%s'",
                    Py_TYPE(self)->tp_name,
                    PyTuple_GET_SIZE(args),
                    info->winrt_name);
                return false;
            }

            return compose_into(
                self,
                *info,
                *info->constructor,
                *overload,
                tuple_items(args),
                PyTuple_GET_SIZE(args));
        }

        PyObject* class_new(PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
        {
            auto const info = class_of(type);

            // A Python class with an __init__ of its own takes the arguments
            // that __init__ declares, as any Python class does, so it is
            // class_init(), which its __init__ reaches through super(), that
            // composes it, with the arguments given there, or else
            // compose_after_init(), with the arguments of the call, once
            // __init__ has returned without doing so.
            if (info && type != info->py_type.get() && type->tp_init != class_init)
            {
                return type->tp_alloc(type, 0);
            }

            // Checked before an instance of a derived class is allocated, and
            // checked again by compose_from_call(), which composes it.
            if (!check_call(type, info, kwds, constructor_call::direct))
            {
                return nullptr;
            }

            if (type != info->py_type.get())
            {
                pyobj_handle self{type->tp_alloc(type, 0)};
                if (!self)
                {
                    return nullptr;
                }

                if (!compose_from_call(
                        self.get(), info, args, kwds, constructor_call::direct))
                {
                    return nullptr;
                }

                return self.detach();
            }

            auto const overload
                = select_overload(*info->constructor, PyTuple_GET_SIZE(args));

            if (!overload)
            {
                return nullptr;
            }

            if (overload->is_protected)
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "'%s' has a protected constructor, so only a class derived "
                    "from it can be created",
                    info->winrt_name);
                return nullptr;
            }

            return call_member(
                *info->constructor,
                *overload,
                nullptr,
                tuple_items(args),
                PyTuple_GET_SIZE(args));
        }

        /**
         * __init__() of a composable class. The arguments of a call to the
         * class went to the constructor already, in class_new(), unless the
         * class called is a Python one with an __init__ of its own, which
         * calls this one through super() with the arguments for the
         * constructor instead.
         */
        int class_init(PyObject* self, PyObject* args, PyObject* kwds) noexcept
        {
            auto* const type = Py_TYPE(self);

            if (type->tp_init == class_init)
            {
                return 0;
            }

            if (abi_of(self))
            {
                set_already_composed_error(self);
                return -1;
            }

            auto const info = class_of(type);

            return compose_from_call(self, info, args, kwds, constructor_call::direct)
                       ? 0
                       : -1;
        }

        /**
         * Refuses @p cls as a type a WinRT object can be seen as.
         * @returns nullptr, for the caller to return.
         */
        PyObject* not_seen_as(PyObject* cls) noexcept
        {
            PyErr_Format(
                PyExc_TypeError,
                "'%s' is not an interface a WinRT object can be seen as",
                reinterpret_cast<PyTypeObject*>(cls)->tp_name);
            return nullptr;
        }

        /**
         * _query_interface_(): the object seen as this type, which is what as_()
         * calls.
         */
        PyObject* type_query_interface(PyObject* cls, PyObject* arg) noexcept
        {
            auto const info = find_type_entry(reinterpret_cast<PyTypeObject*>(cls));
            if (!info)
            {
                return not_seen_as(cls);
            }

            // isinstance() refuses it with the same words
            if (info->parameterized)
            {
                set_parameterized_type_error(
                    reinterpret_cast<PyTypeObject*>(cls)->tp_name);
                return nullptr;
            }

            if (!info->guid)
            {
                return not_seen_as(cls);
            }

            try
            {
                return wrap_abi(info->py_type.get(), unwrap_abi(arg, info->guid));
            }
            catch (...)
            {
                to_PyErr();
                return nullptr;
            }
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
                    find_type_entry(type)
                    && find_type_entry(type)->implements.get() == type);

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

        /**
         * The object WinRT is given for @p self, a Python implementation of an
         * interface, which is what it answers WinRT's questions with.
         */
        winrt::Windows::Foundation::IInspectable identity_of(PyObject* self)
        {
            return {unwrap_abi(self, nullptr), winrt::take_ownership_from_abi};
        }

        // An implementation holds no WinRT object of its own, unlike a wrapper,
        // so it answers what Object's members ask through the one WinRT is given.

        PyObject* implements_iids_get(PyObject* self, void* /*unused*/) noexcept
        {
            try
            {
                return iids_of(identity_of(self));
            }
            catch (...)
            {
                to_PyErr();
                return nullptr;
            }
        }

        PyObject* implements_runtime_class_name_get(
            PyObject* self, void* /*unused*/) noexcept
        {
            try
            {
                return runtime_class_name_of(identity_of(self));
            }
            catch (...)
            {
                to_PyErr();
                return nullptr;
            }
        }

        PyGetSetDef implements_getset[]
            = {{"_iids_", implements_iids_get, nullptr, nullptr, nullptr},
               {"_runtime_class_name_",
                implements_runtime_class_name_get,
                nullptr,
                nullptr,
                nullptr},
               {}};

        PyMethodDef class_methods[] = {
            {"_query_interface_", type_query_interface, METH_O | METH_CLASS, nullptr},
            {"_from", deprecated_from, METH_O | METH_CLASS, nullptr},
            {}};

        // A composable class only derives from another, so the first in a
        // hierarchy takes the keywords of a Python class derived from it.
        PyMethodDef composable_root_methods[] = {
            {"_query_interface_", type_query_interface, METH_O | METH_CLASS, nullptr},
            {"_from", deprecated_from, METH_O | METH_CLASS, nullptr},
            {"__init_subclass__",
             reinterpret_cast<PyCFunction>(reinterpret_cast<void*>(init_subclass)),
             METH_VARARGS | METH_KEYWORDS | METH_CLASS,
             nullptr},
            {}};

        PyMethodDef implements_methods[] = {
            {"as_", object_as, METH_O, nullptr},
            {"_query_interface_", type_query_interface, METH_O | METH_CLASS, nullptr},
            {"_from", deprecated_from, METH_O | METH_CLASS, nullptr},
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

        PyMethodDef generic_implements_methods[] = {
            {"as_", object_as, METH_O, nullptr},
            {"_query_interface_", type_query_interface, METH_O | METH_CLASS, nullptr},
            {"_from", deprecated_from, METH_O | METH_CLASS, nullptr},
            {"__class_getitem__",
             Py_GenericAlias,
             METH_O | METH_CLASS,
             PyDoc_STR("See PEP 585")},
            {}};
    } // namespace

    /**
     * Composes @p self, an instance of a Python class derived from a
     * composable one whose __init__() has returned without composing it, with
     * the arguments of the call, @p args and @p kwds, as class_new() composes
     * an instance of a class without an __init__ of its own. An object that
     * is composed already, or that is no such instance, is left as it is.
     *
     * @returns @c true, or @c false with a Python error set.
     */
    bool compose_after_init(PyObject* self, PyObject* args, PyObject* kwds) noexcept
    {
        auto* const type = Py_TYPE(self);

        auto const info = class_of(type);
        if (!info)
        {
            return true;
        }

        if (type == info->py_type.get())
        {
            return true;
        }

        if (abi_of(self))
        {
            return true;
        }

        return compose_from_call(self, info, args, kwds, constructor_call::after_init);
    }

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

        entry.py_type = std::move(type);

        if (!remember(entry, entry.py_type.get()))
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
               {Py_tp_getset, reinterpret_cast<void*>(implements_getset)},
               {}};

        // Which instance of a parameterized interface a class implements would
        // be named by its base's type arguments, which only the class's
        // __orig_bases__ keep and nothing reads, so Python refuses to derive
        // from one.
        PyType_Spec implements_spec{
            implements_name,
            0,
            0,
            Py_TPFLAGS_DEFAULT | (generic ? 0 : Py_TPFLAGS_BASETYPE),
            implements_slots};

        auto const metaclass = get_object_meta_type();
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

        entry.implements = std::move(implements);

        if (!remember(entry, entry.implements.get()))
        {
            return false;
        }

        if (!bind_methods(collected, entry.py_type.get(), nullptr))
        {
            return false;
        }

        return bind_protocol_methods(record, entry.py_type.get());
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
        auto* metaclass = base_type ? Py_TYPE(base_type) : get_object_meta_type();
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

        // _query_interface_() says what to do with an instance, so a static
        // class, which has none, does not get it.
        if (entry.guid)
        {
            auto* const methods
                = composable && !base_type ? composable_root_methods : class_methods;

            slots.push_back({Py_tp_methods, reinterpret_cast<void*>(methods)});
        }

        slots.push_back({Py_tp_new, reinterpret_cast<void*>(class_new)});

        if (composable)
        {
            slots.push_back({Py_tp_init, reinterpret_cast<void*>(class_init)});
        }
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

        entry.py_type = std::move(type);
        entry.statics = std::move(statics);

        if (!remember(entry, entry.py_type.get()))
        {
            return false;
        }

        if (entry.statics && !remember(entry, entry.statics.get()))
        {
            return false;
        }

        if (!bind_methods(collected, entry.py_type.get(), metaclass))
        {
            return false;
        }

        return bind_protocol_methods(record, entry.py_type.get());
    }

    /**
     * _from(): what _query_interface_() was called before 4.0, kept so that
     * code written before as_() existed goes on working. It calls
     * _query_interface_() rather than doing the same thing itself, so that it
     * cannot come to differ.
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

        pyobj_handle query_interface{PyUnicode_InternFromString("_query_interface_")};
        if (!query_interface)
        {
            return nullptr;
        }

        return PyObject_CallMethodOneArg(cls, query_interface.get(), arg);
    }

    /**
     * The name @p type is written with in Python, qualified by its module
     * unless that is builtins or __main__, as
     * PyType_GetFullyQualifiedName() gives it from 3.13.
     */
    PyObject* qualified_name_of(PyTypeObject* type) noexcept
    {
#if PY_VERSION_HEX >= 0x030D0000
        return PyType_GetFullyQualifiedName(type);
#else
        pyobj_handle qualname{PyType_GetQualName(type)};
        if (!qualname)
        {
            return nullptr;
        }

        pyobj_handle module{
            PyObject_GetAttrString(reinterpret_cast<PyObject*>(type), "__module__")};
        if (!module)
        {
            return nullptr;
        }

        if (!PyUnicode_Check(module.get()))
        {
            return qualname.detach();
        }

        if (PyUnicode_CompareWithASCIIString(module.get(), "builtins") == 0)
        {
            return qualname.detach();
        }

        if (PyUnicode_CompareWithASCIIString(module.get(), "__main__") == 0)
        {
            return qualname.detach();
        }

        return PyUnicode_FromFormat("%U.%U", module.get(), qualname.get());
#endif
    }

    /**
     * as_(): @p self seen as @p arg, which is what arg._query_interface_()
     * makes of it. A wrapper answers it, and so does a Python implementation of
     * an interface.
     */
    PyObject* object_as(PyObject* self, PyObject* arg) noexcept
    {
        if (reinterpret_cast<PyObject*>(Py_TYPE(self)) == arg)
        {
            return Py_NewRef(self);
        }

        // A parameterized interface named with its type arguments, such as
        // IVector[int], is a types.GenericAlias, which looks attributes up on
        // the interface it parameterizes.
        if (PyObject_TypeCheck(arg, &Py_GenericAliasType))
        {
            pyobj_handle origin_query_interface;

            if (PyObject_GetOptionalAttrString(
                    arg, "_query_interface_", origin_query_interface.put())
                < 0)
            {
                return nullptr;
            }

            if (origin_query_interface)
            {
                PyErr_SetString(
                    PyExc_TypeError, "as_() cannot take a parameterized interface");
                return nullptr;
            }
        }

        // as_(type) is type._query_interface_(self). Only a type is asked for
        // it, since an instance would find its class's.
        if (!PyType_Check(arg))
        {
            pyobj_handle name{qualified_name_of(Py_TYPE(arg))};
            if (!name)
            {
                return nullptr;
            }

            PyErr_Format(
                PyExc_TypeError,
                "as_() takes a WinRT class or interface, not a '%U'",
                name.get());
            return nullptr;
        }

        // Every projected class with instances, projected interface and
        // interop interface has _query_interface_(), and a struct, an enum or
        // a static class has none, so a type without it is refused for what it
        // is rather than for the attribute it lacks.
        pyobj_handle query_interface;

        if (PyObject_GetOptionalAttrString(
                arg, "_query_interface_", query_interface.put())
            < 0)
        {
            return nullptr;
        }

        if (!query_interface)
        {
            pyobj_handle name{qualified_name_of(reinterpret_cast<PyTypeObject*>(arg))};
            if (!name)
            {
                return nullptr;
            }

            PyErr_Format(
                PyExc_TypeError,
                "as_() takes a WinRT class or interface, not '%U'",
                name.get());
            return nullptr;
        }

        return PyObject_CallOneArg(query_interface.get(), self);
    }

    /**
     * _iids_: the interfaces @p object says it implements, as GetIids answers.
     * The object may be a proxy, which this asks across apartments.
     */
    PyObject* iids_of(winrt::Windows::Foundation::IInspectable object) noexcept
    {
        try
        {
            winrt::com_array<winrt::guid> iids;

            {
                auto _gil = release_gil();
                iids = winrt::get_interfaces(object);
            }

            return convert(iids);
        }
        catch (...)
        {
            to_PyErr();
            return nullptr;
        }
    }

    /**
     * _runtime_class_name_: what @p object answers GetRuntimeClassName with.
     * The object may be a proxy, which this asks across apartments.
     */
    PyObject* runtime_class_name_of(
        winrt::Windows::Foundation::IInspectable object) noexcept
    {
        try
        {
            winrt::hstring name;

            {
                auto _gil = release_gil();
                name = winrt::get_class_name(object);
            }

            return convert(name);
        }
        catch (...)
        {
            to_PyErr();
            return nullptr;
        }
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
