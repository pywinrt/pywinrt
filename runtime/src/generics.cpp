// The parameterized interfaces, closed over the types a namespace uses them
// with.
//
// A generic instance is built from a table record like any other type, so most
// of this file is the two things that make one different: it derives from the
// Python type its definition is bound to, which is where the collection
// protocol mixins live, and it is built once per instance rather than once per
// package, because two namespaces that both pass an IVector<String> have to
// agree about what one is.
//
// IReference<T> is at the bottom, and is the one parameterized interface with
// no Python type at all: a value of it is either the value it holds or None.

#include <Python.h>

#define PYWINRT_RUNTIME_MODULE
#include <pywinrt/base.h>

#include "generics.h"
#include "interp.h"
#include "members.h"
#include "module_state.h"
#include "objects.h"
#include "protocols.h"
#include "types.h"

namespace py::interp
{
    namespace
    {
        /**
         * The type that has already been built for this instance, or
         * @c nullptr.
         *
         * The key is the WinRT signature, which is what the IID is hashed from
         * and so names the instance and nothing else. Every package that passes
         * an IVector<String> carries a record for it, and they all have to come
         * out as the same Python type: an instance is not bound to a module, so
         * nothing else would make two of them equal.
         */
        PyTypeObject* find_instance_type(std::string_view signature) noexcept
        {
            auto const s = py::cpp::_winrt::get_module_state();
            if (!s)
            {
                return nullptr;
            }

            py::cpp::_winrt::state_guard guard{s->cache_lock};
            auto const found = s->generic_types.find(signature);

            return found == s->generic_types.end() ? nullptr : found->second;
        }

        bool remember_instance_type(
            std::string_view signature, PyTypeObject* type) noexcept
        {
            auto const s = py::cpp::_winrt::get_module_state();
            if (!s)
            {
                PyErr_SetString(PyExc_SystemError, "winrt-runtime is not loaded");
                return false;
            }

            try
            {
                py::cpp::_winrt::state_guard guard{s->cache_lock};
                s->generic_types[signature] = type;
            }
            catch (...)
            {
                to_PyErr();
                return false;
            }

            return true;
        }

        /**
         * Boxes @p value as the WinRT value of @p code.
         *
         * Only the types that PropertyValue can hold are here. Anything else -
         * a struct that is not one of the three PropertyValue knows, an enum, a
         * class - needs an IReference<T> implementation of the runtime's own,
         * which is a WinRT object implemented here rather than a call into
         * WinRT and so belongs with the rest of the reverse direction.
         */
        winrt::Windows::Foundation::IInspectable box_value(
            table::type_code code, PyObject* value)
        {
            switch (code)
            {
            case table::type_code::boolean:
                return winrt::box_value(convert_to<bool>(value));
            case table::type_code::int8:
                return winrt::box_value(convert_to<int8_t>(value));
            case table::type_code::uint8:
                return winrt::box_value(convert_to<uint8_t>(value));
            case table::type_code::int16:
                return winrt::box_value(convert_to<int16_t>(value));
            case table::type_code::uint16:
                return winrt::box_value(convert_to<uint16_t>(value));
            case table::type_code::int32:
                return winrt::box_value(convert_to<int32_t>(value));
            case table::type_code::uint32:
                return winrt::box_value(convert_to<uint32_t>(value));
            case table::type_code::int64:
                return winrt::box_value(convert_to<int64_t>(value));
            case table::type_code::uint64:
                return winrt::box_value(convert_to<uint64_t>(value));
            case table::type_code::single:
                return winrt::box_value(convert_to<float>(value));
            case table::type_code::double_:
                return winrt::box_value(convert_to<double>(value));
            case table::type_code::char16:
                return winrt::box_value(convert_to<char16_t>(value));
            case table::type_code::string:
                return winrt::box_value(convert_to<winrt::hstring>(value));
            case table::type_code::guid:
                return winrt::box_value(convert_to<winrt::guid>(value));
            case table::type_code::datetime:
                return winrt::box_value(
                    convert_to<winrt::Windows::Foundation::DateTime>(value));
            case table::type_code::timespan:
                return winrt::box_value(
                    convert_to<winrt::Windows::Foundation::TimeSpan>(value));
            default:
                return nullptr;
            }
        }
    } // namespace

    /**
     * Builds the Python type of one generic instance, or finds the one that
     * another package's table has already built.
     */
    bool ensure_instance_type(
        projection& proj, type_entry& entry, table::type_view const& record)
    {
        entry.owner = &proj;
        entry.index = record.index();
        entry.category = record.get_category();
        entry.guid = record.guid();
        entry.winrt_name = keep(proj, qualified(record));
        entry.tp_name = record.py_name();

        if (entry.category == table::category::delegate)
        {
            // A delegate is a callable rather than a wrapper, so an instance of
            // one has no Python type of its own. What the table says about it -
            // its IID - is all that anything needs until Python can implement
            // one.
            return true;
        }

        if (auto const built = find_instance_type(record.signature()))
        {
            entry.py_type = reinterpret_cast<PyTypeObject*>(Py_NewRef(built));

            return true;
        }

        auto const base = record.base_type();
        if (base == table::no_ref)
        {
            PyErr_Format(
                PyExc_ImportError,
                "the table names no definition for '%s'",
                entry.winrt_name);
            return false;
        }

        auto const base_type = ensure_referenced_type(proj, base);
        if (!base_type)
        {
            return false;
        }

        type_members collected;

        if (!collect_members(proj, entry, record, collected))
        {
            return false;
        }

        pyobj_handle bases{PyTuple_Pack(1, reinterpret_cast<PyObject*>(base_type))};
        if (!bases)
        {
            return false;
        }

        std::vector<PyType_Slot> slots;

        slots.push_back(
            {Py_tp_getset,
             reinterpret_cast<void*>(keep_getsets(proj, collected.instance_getsets))});

        add_protocol_slots(record, slots);

        slots.push_back({});

        PyType_Spec spec{
            entry.tp_name.c_str(),
            static_cast<int>(object_basicsize),
            0,
            Py_TPFLAGS_DEFAULT,
            slots.data()};

        // Not register_python_type(), which also binds the type in the module:
        // an instance belongs to the module that defines the parameterized
        // interface, and nothing there is named after one type argument.
        pytype_handle type{reinterpret_cast<PyTypeObject*>(
            PyType_FromMetaclass(nullptr, proj.module, &spec, bases.get()))};
        if (!type)
        {
            return false;
        }

        entry.py_type = type.detach();

        if (!remember(entry, entry.py_type))
        {
            return false;
        }

        if (!remember_instance_type(record.signature(), entry.py_type))
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
     * The Python value of an IReference<T>, which is the value it holds.
     *
     * Ownership of @p abi moves here: the reference is the projection's own
     * business and never reaches Python, so it is read and given back.
     */
    PyObject* reference_to_python(type_entry& info, void* abi) noexcept
    {
        if (!abi)
        {
            Py_RETURN_NONE;
        }

        winrt::com_ptr<::IUnknown> reference;
        reference.attach(static_cast<::IUnknown*>(abi));

        auto const value = info.protocol.value;
        if (!value)
        {
            PyErr_Format(PyExc_TypeError, "'%s' holds no value", info.winrt_name);
            return nullptr;
        }

        auto const overload = select_overload(*value, 0);
        if (!overload)
        {
            return nullptr;
        }

        return call_member(*value, *overload, reference.get(), nullptr, 0);
    }

    /**
     * An IReference<T> holding @p value, which the caller owns a reference to.
     *
     * @returns @c nullptr for None, which is the empty reference.
     * @throws python_exception if the value cannot be boxed.
     */
    void* reference_from_python(type_entry& info, PyObject* value)
    {
        throw_if_pyobj_null(value);

        if (Py_IsNone(value))
        {
            return nullptr;
        }

        auto const held = info.protocol.value;
        if (!held)
        {
            PyErr_Format(PyExc_TypeError, "'%s' holds no value", info.winrt_name);
            throw python_exception();
        }

        auto const boxed = box_value(held->overloads[0].args[0].code, value);
        if (!boxed)
        {
            PyErr_Format(
                PyExc_NotImplementedError,
                "'%s' cannot be made from a Python value yet",
                info.winrt_name);
            throw python_exception();
        }

        // What PropertyValue hands back answers the IReference<T> of the type
        // it boxed, which is the pointer the member takes.
        void* result{};

        winrt::check_hresult(
            static_cast<::IUnknown*>(winrt::get_abi(boxed))
                ->QueryInterface(*static_cast<winrt::guid const*>(info.guid), &result));

        return result;
    }

    PyTypeObject* find_type_by_signature(std::string_view signature) noexcept
    {
        return find_instance_type(signature);
    }
} // namespace py::interp

/**
 * Wraps @p value as the concrete parameterized interface that @p signature
 * names.
 *
 * A concrete parameterized interface is bound to nothing - there is no
 * IVector[str] attribute anywhere - so a compiled module names one by the
 * signature its IID is hashed from, and gets the type that the package which
 * passes that instance built.
 */
PyObject* py::wrap_by_signature(
    winrt::Windows::Foundation::IInspectable const& value,
    char const* signature) noexcept
{
    if (!value)
    {
        Py_RETURN_NONE;
    }

    auto const type = py::interp::find_type_by_signature(signature);
    if (!type)
    {
        PyErr_Format(
            PyExc_TypeError,
            "no loaded projection passes a '%s', so there is no type for one",
            signature);
        return nullptr;
    }

    auto const info = py::interp::get_type_entry(type);
    if (!info)
    {
        PyErr_SetString(PyExc_TypeError, "the type has no descriptor");
        return nullptr;
    }

    void* abi{};

    auto const hr
        = static_cast<::IUnknown*>(winrt::get_abi(value))
              ->QueryInterface(*static_cast<winrt::guid const*>(info->guid), &abi);
    if (hr != 0)
    {
        try
        {
            winrt::check_hresult(hr);
        }
        catch (...)
        {
            to_PyErr();
            return nullptr;
        }
    }

    return py::interp::wrap_abi(type, abi);
}
