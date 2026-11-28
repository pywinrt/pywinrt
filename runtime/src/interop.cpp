// Interface pointer capsules: winrt._winrt.as_interface() and the functions
// beside it, which winrt.runtime.interop exports. They are how compiled code
// outside the runtime exchanges WinRT objects with it, through Python objects,
// so that nothing but the capsule's name and their signatures is shared.

#include <Python.h>

#include "_winrt.h"
#include "convert.h"
#include "errors.h"
#include "handles.h"
#include "implements.h"
#include "interop.h"
#include "interp.h"
#include "objects.h"
#include "prelude.h"
#include "runtime.h"
#include "table.h"
#include "types.h"

#include <cstdint>

namespace py::interp
{
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
        if (type == info->statics.get())
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

        return py::interp::wrap_abi(info.py_type.get(), abi);
    }

    /**
     * Wraps @p value as winrt.system.Object, which is what a projected member
     * typed Object returns. @p value is queried for IInspectable first, since
     * the interface a capsule holds may be a COM interface that is not one.
     */
    PyObject* wrap_as_object(
        winrt::Windows::Foundation::IInspectable const& value) noexcept
    {
        try
        {
            winrt::Windows::Foundation::IInspectable object;
            int32_t hr{};

            {
                // The object may be a proxy, which a query for an interface
                // is a call into the apartment of.
                auto _gil = py::release_gil();
                hr = static_cast<::IUnknown*>(winrt::get_abi(value))
                         ->QueryInterface(
                             winrt::guid_of<winrt::Windows::Foundation::IInspectable>(),
                             winrt::put_abi(object));
            }

            winrt::check_hresult(hr);

            return py::convert(object);
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    /**
     * Wraps @p value as @p type, a projected class or interface, or
     * winrt.system.Object. @p name is what the caller named the type by.
     */
    PyObject* wrap_as_type(
        winrt::Windows::Foundation::IInspectable const& value,
        PyTypeObject* type,
        char const* name) noexcept
    {
        auto const object_type = py::get_object_type();
        if (!object_type)
        {
            return nullptr;
        }

        if (type == object_type)
        {
            return wrap_as_object(value);
        }

        auto const info = find_object_entry(type, name);
        if (!info)
        {
            return nullptr;
        }

        return wrap_as(value, *info);
    }

    /**
     * Whether @p obj is a parameterized interface given its type arguments,
     * such as IVector[int], which is a types.GenericAlias.
     *
     * @returns -1 with a Python error set.
     */
    int is_generic_instance(PyObject* obj) noexcept
    {
        if (!PyObject_TypeCheck(obj, &Py_GenericAliasType))
        {
            return 0;
        }

        py::pyobj_handle origin{PyObject_GetAttrString(obj, "__origin__")};
        if (!origin)
        {
            return -1;
        }

        if (!PyType_Check(origin.get()))
        {
            return 0;
        }

        auto const info = py::interp::find_type_entry(
            reinterpret_cast<PyTypeObject*>(origin.get()));
        if (!info)
        {
            return 0;
        }

        return info->parameterized;
    }

    /**
     * The @c TypeError for @p obj, which py::convert_to_guid() refused as an
     * IID, saying that @p function takes @p takes and naming what @p obj is.
     *
     * @returns @c nullptr with a Python error set.
     */
    PyObject* new_iid_type_error(
        PyObject* obj, char const* function, char const* takes) noexcept
    {
        auto const generic_instance = is_generic_instance(obj);
        if (generic_instance < 0)
        {
            return nullptr;
        }

        py::pyobj_handle message;

        if (generic_instance)
        {
            message.attach(PyUnicode_FromFormat(
                "%s() takes the parameterized interface '%R' only by its IID, "
                "as a uuid.UUID",
                function,
                obj));
        }
        else
        {
            py::pyobj_handle name{py::interp::qualified_name_of(Py_TYPE(obj))};
            if (!name)
            {
                return nullptr;
            }

            message.attach(PyUnicode_FromFormat(
                "%s() takes %s, not '%U'", function, takes, name.get()));
        }

        if (!message)
        {
            return nullptr;
        }

        return PyObject_CallOneArg(PyExc_TypeError, message.get());
    }

    /**
     * Replaces the @c TypeError that py::convert_to_guid() raised for @p obj
     * with one saying that @p function takes @p takes, and naming what @p obj
     * is. Its cause stays. An error while making it leaves the error as it
     * was.
     */
    void set_iid_type_error(
        PyObject* obj, char const* function, char const* takes) noexcept
    {
        py::pyobj_handle refused{py::take_raised_exception()};

        py::pyobj_handle error{new_iid_type_error(obj, function, takes)};
        if (!error)
        {
            PyErr_Clear();
            py::restore_raised_exception(refused.detach());
            return;
        }

        PyException_SetCause(error.get(), PyException_GetCause(refused.get()));
        py::restore_raised_exception(error.detach());
    }

    /**
     * The IID that @p obj names: a @c uuid.UUID, or a projected class or
     * interface, which names the interface an instance of it holds, or
     * winrt.system.Object, which names IInspectable.
     *
     * @throws python_exception if @p obj is none of these. For anything that is
     * not a type, the @c TypeError names @p function and says what it takes.
     */
    winrt::guid iid_named_by(PyObject* obj, char const* function, char const* takes)
    {
        if (!PyType_Check(obj))
        {
            try
            {
                return py::convert_to_guid(obj);
            }
            catch (py::python_exception const&)
            {
                set_iid_type_error(obj, function, takes);
                throw;
            }
        }

        auto const object_type = py::get_object_type();
        if (!object_type)
        {
            throw py::python_exception();
        }

        if (obj == reinterpret_cast<PyObject*>(object_type))
        {
            return winrt::guid_of<winrt::Windows::Foundation::IInspectable>();
        }

        auto const type = reinterpret_cast<PyTypeObject*>(obj);

        auto const info = find_object_entry(type, type->tp_name);
        if (!info)
        {
            throw py::python_exception();
        }

        return *static_cast<winrt::guid const*>(info->guid);
    }

    /**
     * Checks whether @p obj is a wrapped WinRT object, including an instance
     * of a Python subclass of a composable class.
     *
     * @returns -1 with a Python error set.
     */
    int is_winrt_object(PyObject* obj) noexcept
    {
        auto const object_type = py::get_object_type();
        if (!object_type)
        {
            return -1;
        }

        return PyObject_TypeCheck(obj, object_type);
    }

    /**
     * Gets the pointer from an interface pointer capsule, without adding a
     * reference. @p obj is either the capsule itself, or an interop class
     * instance whose _interface_capsule_() method returns the capsule.
     *
     * @param keep Set to the capsule returned by _interface_capsule_(), if
     * that was called.
     * @returns 1 with @p result set, 0 if @p obj is neither kind of object,
     * or -1 with a Python error set.
     */
    int capsule_interface(PyObject* obj, py::pyobj_handle& keep, void** result) noexcept
    {
        if (!PyCapsule_CheckExact(obj))
        {
            py::pyobj_handle method;
            if (PyObject_GetOptionalAttrString(obj, "_interface_capsule_", method.put())
                < 0)
            {
                return -1;
            }

            if (!method)
            {
                return 0;
            }

            keep.attach(PyObject_CallNoArgs(method.get()));
            if (!keep)
            {
                return -1;
            }

            if (!PyCapsule_CheckExact(keep.get()))
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "_interface_capsule_() must return an interface pointer "
                    "capsule, not '%s'",
                    Py_TYPE(keep.get())->tp_name);
                return -1;
            }

            obj = keep.get();
        }

        *result = PyCapsule_GetPointer(obj, py::interp::interface_capsule_name);
        if (!*result)
        {
            return -1;
        }

        return 1;
    }

    /**
     * Wraps @p value as the type that @p qualified_name is bound to, for
     * wrap_interface() when it is given a name.
     */
    PyObject* wrap_object(
        winrt::Windows::Foundation::IInspectable const& value,
        char const* qualified_name) noexcept
    {
        if (!value)
        {
            Py_RETURN_NONE;
        }

        auto const type = py::get_python_type(qualified_name);
        if (!type)
        {
            return nullptr;
        }

        return wrap_as_type(value, type, qualified_name);
    }
} // namespace

namespace py::cpp::_winrt
{
    /**
     * as_interface(obj, iid) - the @p iid interface of the WinRT object @p obj
     * wraps, as an interface pointer capsule, or None for None. @p iid is a
     * uuid.UUID, or a projected class or interface, which stands for the
     * interface an instance of it holds, or winrt.system.Object, which stands
     * for IInspectable.
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
            auto const iid = iid_named_by(
                iid_obj,
                "as_interface",
                "a uuid.UUID or a projected class or interface");
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
     * get_activation_factory(cls, iid) - the @p iid interface of the
     * activation factory of the runtime class @p cls, as an interface pointer
     * capsule. @p iid is a uuid.UUID, or a projected interface, or
     * winrt.system.Object, which stands for IInspectable.
     *
     * A class's statics, its constructors and the Windows interop interfaces
     * that act for it, such as IGraphicsCaptureItemInterop, are all reached
     * through its activation factory. The runtime gets it as it does for a
     * static member, so a call that comes first in a process works.
     */
    PyObject* get_activation_factory(PyObject* /*unused*/, PyObject* args) noexcept
    {
        PyObject* type_obj;
        PyObject* iid_obj;

        if (!PyArg_ParseTuple(
                args, "O!O:get_activation_factory", &PyType_Type, &type_obj, &iid_obj))
        {
            return nullptr;
        }

        auto const type = reinterpret_cast<PyTypeObject*>(type_obj);

        auto const info = py::interp::find_type_entry(type);
        if (!info)
        {
            PyErr_Format(PyExc_TypeError, "'%s' is not a runtime class", type->tp_name);
            return nullptr;
        }

        if (info->category != py::table::category::class_)
        {
            PyErr_Format(PyExc_TypeError, "'%s' is not a runtime class", type->tp_name);
            return nullptr;
        }

        // A class stands for its default interface, which no factory
        // implements, and so does a Python subclass of a composable one.
        auto const iid_class
            = PyType_Check(iid_obj)
                  ? py::interp::class_of(reinterpret_cast<PyTypeObject*>(iid_obj))
                  : nullptr;
        if (iid_class)
        {
            auto const iid_type = reinterpret_cast<PyTypeObject*>(iid_obj);
            if (iid_type == iid_class->py_type.get())
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "get_activation_factory() takes an interface or a uuid.UUID, "
                    "not the runtime class '%s'",
                    iid_type->tp_name);
                return nullptr;
            }

            PyErr_Format(
                PyExc_TypeError,
                "get_activation_factory() takes an interface or a uuid.UUID, "
                "not '%s', a subclass of the runtime class '%s'",
                iid_type->tp_name,
                iid_class->py_type.get()->tp_name);
            return nullptr;
        }

        try
        {
            auto const iid = iid_named_by(
                iid_obj, "get_activation_factory", "an interface or a uuid.UUID");

            void* factory{};
            int32_t hr{};

            {
                // Activation may start a server in another process.
                auto _gil = py::release_gil();

                hr = winrt::impl::get_runtime_activation_factory_impl<false>(
                    winrt::param::hstring{info->class_name}, iid, &factory);
            }

            winrt::check_hresult(hr);

            return py::interp::new_interface_capsule(factory);
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
     * or interface, or the qualified name it is bound to, or
     * winrt.system.Object. The capsule keeps its own reference.
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

        return wrap_as_type(value, type, type->tp_name);
    }

    /**
     * interface_pointer(obj) - the raw COM pointer that @p obj holds, as an
     * int. @p obj is one of:
     *
     * - an interface pointer capsule: the pointer in it;
     * - a WinRT object: its default interface (for an interface type, that
     *   interface);
     * - an interop class instance: the pointer in the capsule its
     *   _interface_capsule_() returns.
     *
     * The pointer is borrowed: no reference is added, and it is valid only
     * while @p obj is alive. A receiver that keeps it must AddRef() it.
     *
     * No QueryInterface() is done. A borrowed pointer to a queried interface
     * would have nothing keeping it alive, and a tear-off interface would be
     * freed at once. To get a specific interface, call as_interface() first
     * and pass the capsule it returns.
     */
    PyObject* interface_pointer(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        auto const is_object = is_winrt_object(obj);
        if (is_object < 0)
        {
            return nullptr;
        }

        if (is_object)
        {
            auto const abi = py::interp::instance_abi(obj);
            if (!abi)
            {
                return nullptr;
            }

            return PyLong_FromVoidPtr(abi);
        }

        py::pyobj_handle keep;
        void* held{};

        auto const found = capsule_interface(obj, keep, &held);
        if (found < 0)
        {
            return nullptr;
        }

        if (!found)
        {
            // A Python implementation of a WinRT interface holds no COM
            // pointer: as_interface() makes a COM object for it each time.
            if (py::interp::implements_interfaces(Py_TYPE(obj)))
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "interface_pointer() cannot take '%s', a Python "
                    "implementation of a WinRT interface, which holds no COM "
                    "pointer; pass the capsule that as_interface() returns for it",
                    Py_TYPE(obj)->tp_name);
                return nullptr;
            }

            PyErr_Format(
                PyExc_TypeError,
                "interface_pointer() takes a WinRT object, an interface pointer "
                "capsule or an object with _interface_capsule_(), not '%s'",
                Py_TYPE(obj)->tp_name);
            return nullptr;
        }

        return PyLong_FromVoidPtr(held);
    }
} // namespace py::cpp::_winrt
