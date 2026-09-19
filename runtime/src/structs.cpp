// A WinRT struct, as a value and as a Python type.
//
// A struct is the one value the runtime lays out itself. The table says what
// the fields are and never where they sit, because a pointer is not the same
// width on every architecture one table serves, so the offsets are computed by
// the reader and everything here works from them: reading a field, writing one,
// and giving back the strings and interface pointers a struct holds when the
// last copy of it goes away.
//
// The Python side is immutable, as it has always been: the fields are read
// only and __replace__() makes a new value.
//
// Two structs are not laid out at all. An HRESULT and an event token hold one
// integer each and are projected as subclasses of int, so the value is the
// integer and all that is built for them is the type.

#include <Python.h>

#define PYWINRT_RUNTIME_MODULE
#include <pywinrt/base.h>

#include "arrays.h"
#include "generics.h"
#include "interp.h"
#include "numerics-statics.h"
#include "numerics.h"
#include "objects.h"
#include "structs.h"
#include "types.h"

namespace py::interp
{
    /**
     * Releases an HSTRING, spelled as the opaque handle a value holds so that
     * the header does not have to name the Windows string API.
     */
    void release_string(void* abi) noexcept
    {
        winrt::hstring value;
        winrt::attach_abi(value, abi);
    }

    /**
     * A second reference to the same HSTRING.
     */
    void* duplicate_string(void* abi)
    {
        return winrt::impl::duplicate_hstring(
            static_cast<winrt::impl::hstring_header*>(abi));
    }

    namespace
    {
        /**
         * A field seen as the one output of a call, which is what the value
         * conversions in interp.cpp take.
         */
        arg_desc as_arg(field_desc const& field) noexcept
        {
            arg_desc arg{};
            arg.code = field.code;
            arg.category = table::param_category::out;
            arg.type = field.type;
            arg.info = field.info;

            return arg;
        }

        /**
         * Reads one field of a struct and converts it to Python, leaving what
         * the struct holds untouched.
         */
        PyObject* load_field(
            projection& owner, field_desc& field, uint8_t const* blob) noexcept
        {
            auto* const value = const_cast<uint8_t*>(blob) + field.offset;

            switch (field.code)
            {
            case table::type_code::string:
            {
                winrt::hstring text;
                winrt::attach_abi(text, *reinterpret_cast<void* const*>(value));
                auto const result = convert(text);
                winrt::detach_abi(text);
                return result;
            }
            case table::type_code::object:
            case table::type_code::interface_:
            case table::type_code::class_:
            case table::type_code::delegate:
            case table::type_code::generic:
            case table::type_code::reference:
            {
                auto const abi = *reinterpret_cast<void* const*>(value);
                if (abi)
                {
                    static_cast<::IUnknown*>(abi)->AddRef();
                }

                auto arg = as_arg(field);
                auto const result = convert_out(owner, arg, value);
                field.info = arg.info;
                return result;
            }
            case table::type_code::struct_:
            {
                auto const info = resolve(owner, field.type, field.info);
                if (!info)
                {
                    return nullptr;
                }

                return struct_to_python(*info, value);
            }
            default:
            {
                auto arg = as_arg(field);
                auto const result = convert_out(owner, arg, value);
                field.info = arg.info;
                return result;
            }
            }
        }

        /**
         * Converts a Python value and stores it in one field, which then owns
         * whatever it holds.
         *
         * @throws python_exception on a conversion failure.
         */
        void store_field(
            projection& owner, field_desc& field, PyObject* value, uint8_t* blob)
        {
            auto* const slot = blob + field.offset;

            switch (field.code)
            {
            case table::type_code::boolean:
                *reinterpret_cast<bool*>(slot) = convert_to<bool>(value);
                return;
            case table::type_code::int8:
                *reinterpret_cast<int8_t*>(slot) = convert_to<int8_t>(value);
                return;
            case table::type_code::uint8:
                *reinterpret_cast<uint8_t*>(slot) = convert_to<uint8_t>(value);
                return;
            case table::type_code::int16:
                *reinterpret_cast<int16_t*>(slot) = convert_to<int16_t>(value);
                return;
            case table::type_code::uint16:
                *reinterpret_cast<uint16_t*>(slot) = convert_to<uint16_t>(value);
                return;
            case table::type_code::char16:
                *reinterpret_cast<char16_t*>(slot) = convert_to<char16_t>(value);
                return;
            case table::type_code::int32:
            case table::type_code::hresult:
            case table::type_code::enum32:
                *reinterpret_cast<int32_t*>(slot) = convert_to<int32_t>(value);
                return;
            case table::type_code::uint32:
            case table::type_code::enum_u32:
                *reinterpret_cast<uint32_t*>(slot) = convert_to<uint32_t>(value);
                return;
            case table::type_code::int64:
                *reinterpret_cast<int64_t*>(slot) = convert_to<int64_t>(value);
                return;
            case table::type_code::uint64:
                *reinterpret_cast<uint64_t*>(slot) = convert_to<uint64_t>(value);
                return;
            case table::type_code::single:
                *reinterpret_cast<float*>(slot) = convert_to<float>(value);
                return;
            case table::type_code::double_:
                *reinterpret_cast<double*>(slot) = convert_to<double>(value);
                return;
            case table::type_code::datetime:
                *reinterpret_cast<int64_t*>(slot)
                    = convert_to<winrt::Windows::Foundation::DateTime>(value)
                          .time_since_epoch()
                          .count();
                return;
            case table::type_code::timespan:
                *reinterpret_cast<int64_t*>(slot)
                    = convert_to<winrt::Windows::Foundation::TimeSpan>(value).count();
                return;
            case table::type_code::event_token:
                *reinterpret_cast<int64_t*>(slot) = convert_to<int64_t>(value);
                return;
            case table::type_code::guid:
            {
                auto const value_guid = convert_to<winrt::guid>(value);
                std::memcpy(slot, &value_guid, sizeof(value_guid));
                return;
            }
            case table::type_code::string:
            {
                auto text = convert_to<winrt::hstring>(value);
                *reinterpret_cast<void**>(slot) = winrt::detach_abi(text);
                return;
            }
            case table::type_code::object:
                *reinterpret_cast<void**>(slot) = unwrap_abi(value, nullptr);
                return;
            case table::type_code::interface_:
            case table::type_code::class_:
            case table::type_code::generic:
            {
                auto const info = resolve(owner, field.type, field.info);
                if (!info || !info->guid)
                {
                    if (!PyErr_Occurred())
                    {
                        PyErr_SetString(
                            PyExc_NotImplementedError,
                            "no interface is known for this field");
                    }

                    throw python_exception();
                }

                *reinterpret_cast<void**>(slot) = unwrap_abi(value, info->guid);
                return;
            }
            case table::type_code::reference:
            {
                auto const info = resolve(owner, field.type, field.info);
                if (!info)
                {
                    throw python_exception();
                }

                *reinterpret_cast<void**>(slot) = reference_from_python(*info, value);
                return;
            }
            case table::type_code::struct_:
            {
                auto const info = resolve(owner, field.type, field.info);
                if (!info)
                {
                    throw python_exception();
                }

                struct_from_python(*info, value, slot);

                if (Py_IS_TYPE(value, info->py_type))
                {
                    // A nested struct copied out of a wrapper is borrowed, and
                    // this field has to own what it holds.
                    copy_struct_resources(*info, slot);
                }

                return;
            }
            default:
                PyErr_SetString(
                    PyExc_NotImplementedError, "a delegate is not interpreted yet");
                throw python_exception();
            }
        }
    } // namespace

    /**
     * Releases the strings and interface pointers a struct value holds.
     */
    void release_struct(type_entry const& info, void* blob) noexcept
    {
        if (!info.owns_resources)
        {
            return;
        }

        auto* const bytes = static_cast<uint8_t*>(blob);

        for (auto const& field : info.fields)
        {
            auto* const slot = bytes + field.offset;

            switch (field.code)
            {
            case table::type_code::string:
                release_string(*reinterpret_cast<void**>(slot));
                *reinterpret_cast<void**>(slot) = nullptr;
                break;
            case table::type_code::object:
            case table::type_code::interface_:
            case table::type_code::class_:
            case table::type_code::delegate:
            case table::type_code::generic:
            case table::type_code::reference:
                if (auto const abi = *reinterpret_cast<void**>(slot))
                {
                    static_cast<::IUnknown*>(abi)->Release();
                    *reinterpret_cast<void**>(slot) = nullptr;
                }
                break;
            case table::type_code::struct_:
                if (field.info)
                {
                    release_struct(*field.info, slot);
                }
                break;
            default:
                break;
            }
        }
    }

    /**
     * Duplicates the strings and adds a reference to the interface pointers of
     * the struct value at @p blob, which has already been copied byte for byte.
     */
    void copy_struct_resources(type_entry const& info, void* blob)
    {
        if (!info.owns_resources)
        {
            return;
        }

        auto* const bytes = static_cast<uint8_t*>(blob);

        for (auto const& field : info.fields)
        {
            auto* const slot = bytes + field.offset;

            switch (field.code)
            {
            case table::type_code::string:
                *reinterpret_cast<void**>(slot)
                    = duplicate_string(*reinterpret_cast<void**>(slot));
                break;
            case table::type_code::object:
            case table::type_code::interface_:
            case table::type_code::class_:
            case table::type_code::delegate:
            case table::type_code::generic:
            case table::type_code::reference:
                if (auto const abi = *reinterpret_cast<void**>(slot))
                {
                    static_cast<::IUnknown*>(abi)->AddRef();
                }
                break;
            case table::type_code::struct_:
                if (field.info)
                {
                    copy_struct_resources(*field.info, slot);
                }
                break;
            default:
                break;
            }
        }
    }

    /**
     * Copies the value of a struct out of @p obj into @p out, which is
     * @c info.size bytes of storage the caller owns.
     *
     * A value copied out of a wrapper is borrowed and needs no release; one
     * built from a tuple owns what is in it and must be released once the call
     * it was made for is over.
     *
     * @throws python_exception if @p obj is neither an instance of the struct's
     * type nor a tuple of its fields.
     */
    void struct_from_python(type_entry& info, PyObject* obj, void* out)
    {
        throw_if_pyobj_null(obj);

        if (Py_IS_TYPE(obj, info.py_type))
        {
            std::memcpy(
                out,
                reinterpret_cast<uint8_t const*>(obj) + info.blob_offset,
                info.size);
            return;
        }

        if (!PyTuple_Check(obj))
        {
            PyErr_Format(
                PyExc_TypeError,
                "expected '%s' or a tuple of its fields, not '%s'",
                info.py_type ? info.py_type->tp_name : info.winrt_name,
                Py_TYPE(obj)->tp_name);
            throw python_exception();
        }

        if (PyTuple_GET_SIZE(obj) != static_cast<Py_ssize_t>(info.fields.size()))
        {
            PyErr_Format(
                PyExc_TypeError,
                "'%s' has %zd fields, not %zd",
                info.py_type ? info.py_type->tp_name : info.winrt_name,
                static_cast<Py_ssize_t>(info.fields.size()),
                PyTuple_GET_SIZE(obj));
            throw python_exception();
        }

        std::memset(out, 0, info.size);

        auto* const bytes = static_cast<uint8_t*>(out);

        for (size_t i = 0; i < info.fields.size(); i++)
        {
            try
            {
                store_field(
                    *info.owner, info.fields[i], PyTuple_GET_ITEM(obj, i), bytes);
            }
            catch (...)
            {
                release_struct(info, out);
                throw;
            }
        }
    }

    /**
     * Wraps the struct at @p blob in its Python type and takes what it holds,
     * leaving @p blob empty. This is what a value WinRT has just handed over
     * wants: nothing is duplicated and nothing is left to release.
     */
    PyObject* struct_take_python(type_entry& info, void* blob) noexcept
    {
        auto const self = info.py_type->tp_alloc(info.py_type, 0);
        if (!self)
        {
            return nullptr;
        }

        std::memcpy(
            reinterpret_cast<uint8_t*>(self) + info.blob_offset, blob, info.size);
        std::memset(blob, 0, info.size);

        return self;
    }

    /**
     * Wraps a copy of the struct at @p blob in its Python type. Ownership of
     * @p blob stays with the caller, so the strings and interface pointers in
     * it are duplicated.
     */
    PyObject* struct_to_python(type_entry& info, void const* blob) noexcept
    {
        auto const self = info.py_type->tp_alloc(info.py_type, 0);
        if (!self)
        {
            return nullptr;
        }

        auto* const bytes = reinterpret_cast<uint8_t*>(self) + info.blob_offset;
        std::memcpy(bytes, blob, info.size);

        try
        {
            copy_struct_resources(info, bytes);
        }
        catch (...)
        {
            std::memset(bytes, 0, info.size);
            Py_DECREF(self);
            to_PyErr();
            return nullptr;
        }

        return self;
    }

    // ----- the Python type ---------------------------------------------------

    namespace
    {
        PyObject* field_get(PyObject* self, void* closure) noexcept
        {
            auto const field = static_cast<field_desc*>(closure);
            auto const info = field->declaring;

            return load_field(
                *info->owner,
                *field,
                reinterpret_cast<uint8_t const*>(self) + info->blob_offset);
        }

        /**
         * Fills a struct from the positional and keyword arguments its fields
         * are named by, leaving the ones that were not given at zero.
         */
        bool fill_struct(
            type_entry& info, uint8_t* blob, PyObject* args, PyObject* kwds)
        {
            auto const positional = PyTuple_GET_SIZE(args);
            if (positional > static_cast<Py_ssize_t>(info.fields.size()))
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "'%s' takes at most %zd arguments",
                    info.py_type->tp_name,
                    static_cast<Py_ssize_t>(info.fields.size()));
                return false;
            }

            for (Py_ssize_t i = 0; i < positional; i++)
            {
                store_field(
                    *info.owner, info.fields[i], PyTuple_GET_ITEM(args, i), blob);
            }

            if (!kwds)
            {
                return true;
            }

            Py_ssize_t position = 0;
            PyObject* key{};
            PyObject* value{};

            while (PyDict_Next(kwds, &position, &key, &value))
            {
                bool found = false;

                for (size_t i = 0; i < info.fields.size(); i++)
                {
                    auto const equal
                        = PyObject_RichCompareBool(key, info.fields[i].name, Py_EQ);

                    if (equal == -1)
                    {
                        throw python_exception();
                    }

                    if (!equal)
                    {
                        continue;
                    }

                    if (static_cast<Py_ssize_t>(i) < positional)
                    {
                        PyErr_Format(
                            PyExc_TypeError,
                            "'%s' got two values for '%U'",
                            info.py_type->tp_name,
                            key);
                        return false;
                    }

                    store_field(*info.owner, info.fields[i], value, blob);
                    found = true;
                    break;
                }

                if (!found)
                {
                    PyErr_Format(
                        PyExc_TypeError,
                        "'%s' has no field '%U'",
                        info.py_type->tp_name,
                        key);
                    return false;
                }
            }

            return true;
        }

        PyObject* struct_new(
            PyTypeObject* type, PyObject* args, PyObject* kwds) noexcept
        {
            auto const info = get_type_entry(type);
            if (!info)
            {
                PyErr_Format(PyExc_TypeError, "'%s' cannot be created", type->tp_name);
                return nullptr;
            }

            pyobj_handle self{type->tp_alloc(type, 0)};
            if (!self)
            {
                return nullptr;
            }

            auto* const blob
                = reinterpret_cast<uint8_t*>(self.get()) + info->blob_offset;
            std::memset(blob, 0, info->size);

            try
            {
                if (!fill_struct(*info, blob, args, kwds))
                {
                    return nullptr;
                }
            }
            catch (...)
            {
                to_PyErr();
                return nullptr;
            }

            return self.detach();
        }

        void struct_dealloc(PyObject* self) noexcept
        {
            auto const tp = Py_TYPE(self);

            tp->tp_free(self);
            Py_DECREF(tp);
        }

        void struct_dealloc_release(PyObject* self) noexcept
        {
            auto const tp = Py_TYPE(self);

            if (auto const info = get_type_entry(tp))
            {
                release_struct(
                    *info, reinterpret_cast<uint8_t*>(self) + info->blob_offset);
            }

            tp->tp_free(self);
            Py_DECREF(tp);
        }

        /**
         * The fields of a struct as a tuple, which is what comparing two of
         * them and printing one are both written in terms of.
         */
        pyobj_handle struct_as_tuple(PyObject* self, type_entry& info) noexcept
        {
            pyobj_handle values{
                PyTuple_New(static_cast<Py_ssize_t>(info.fields.size()))};
            if (!values)
            {
                return values;
            }

            auto* const blob
                = reinterpret_cast<uint8_t const*>(self) + info.blob_offset;

            for (size_t i = 0; i < info.fields.size(); i++)
            {
                auto const value = load_field(*info.owner, info.fields[i], blob);
                if (!value)
                {
                    return pyobj_handle{};
                }

                PyTuple_SET_ITEM(values.get(), static_cast<Py_ssize_t>(i), value);
            }

            return values;
        }

        /**
         * A type's name without its namespace, which is what a struct's repr
         * has always shown.
         */
        char const* short_name(type_entry const& info) noexcept
        {
            auto const dot = std::strrchr(info.winrt_name, '.');

            return dot ? dot + 1 : info.winrt_name;
        }

        PyObject* struct_repr(PyObject* self) noexcept
        {
            auto const info = get_type_entry(Py_TYPE(self));
            if (!info)
            {
                return PyObject_Repr(reinterpret_cast<PyObject*>(Py_TYPE(self)));
            }

            pyobj_handle parts{PyList_New(0)};
            if (!parts)
            {
                return nullptr;
            }

            auto* const blob
                = reinterpret_cast<uint8_t const*>(self) + info->blob_offset;

            for (auto& field : info->fields)
            {
                pyobj_handle value{load_field(*info->owner, field, blob)};
                if (!value)
                {
                    return nullptr;
                }

                pyobj_handle part{
                    PyUnicode_FromFormat("%U=%R", field.name, value.get())};
                if (!part)
                {
                    return nullptr;
                }

                if (PyList_Append(parts.get(), part.get()) == -1)
                {
                    return nullptr;
                }
            }

            pyobj_handle separator{PyUnicode_FromString(", ")};
            if (!separator)
            {
                return nullptr;
            }

            pyobj_handle joined{PyUnicode_Join(separator.get(), parts.get())};
            if (!joined)
            {
                return nullptr;
            }

            return PyUnicode_FromFormat("%s(%U)", short_name(*info), joined.get());
        }

        PyObject* struct_richcompare(PyObject* self, PyObject* other, int op) noexcept
        {
            if (op != Py_EQ && op != Py_NE)
            {
                Py_RETURN_NOTIMPLEMENTED;
            }

            auto const info = get_type_entry(Py_TYPE(self));
            if (!info)
            {
                Py_RETURN_NOTIMPLEMENTED;
            }

            pyobj_handle mine{struct_as_tuple(self, *info)};
            if (!mine)
            {
                return nullptr;
            }

            pyobj_handle theirs{};

            if (Py_IS_TYPE(other, info->py_type))
            {
                theirs = struct_as_tuple(other, *info);
                if (!theirs)
                {
                    return nullptr;
                }
            }
            else if (PyTuple_Check(other))
            {
                theirs.attach(Py_NewRef(other));
            }
            else
            {
                Py_RETURN_NOTIMPLEMENTED;
            }

            return PyObject_RichCompare(mine.get(), theirs.get(), op);
        }

        PyObject* struct_replace(
            PyObject* self, PyObject* args, PyObject* kwds) noexcept
        {
            if (PyTuple_GET_SIZE(args) != 0)
            {
                PyErr_SetString(
                    PyExc_TypeError, "__replace__() takes no positional arguments");
                return nullptr;
            }

            auto const info = get_type_entry(Py_TYPE(self));
            if (!info)
            {
                PyErr_SetString(PyExc_TypeError, "not a projected struct");
                return nullptr;
            }

            pyobj_handle copy{info->py_type->tp_alloc(info->py_type, 0)};
            if (!copy)
            {
                return nullptr;
            }

            auto* const blob
                = reinterpret_cast<uint8_t*>(copy.get()) + info->blob_offset;
            std::memcpy(
                blob,
                reinterpret_cast<uint8_t const*>(self) + info->blob_offset,
                info->size);

            try
            {
                copy_struct_resources(*info, blob);

                if (!kwds)
                {
                    return copy.detach();
                }

                Py_ssize_t position = 0;
                PyObject* key{};
                PyObject* value{};

                while (PyDict_Next(kwds, &position, &key, &value))
                {
                    bool found = false;

                    for (auto& field : info->fields)
                    {
                        auto const equal
                            = PyObject_RichCompareBool(key, field.name, Py_EQ);

                        if (equal == -1)
                        {
                            return nullptr;
                        }

                        if (!equal)
                        {
                            continue;
                        }

                        // The field is about to be overwritten, so what it
                        // holds now has to go back first.
                        auto const old = field.code;
                        if (old == table::type_code::string)
                        {
                            release_string(
                                *reinterpret_cast<void**>(blob + field.offset));
                            *reinterpret_cast<void**>(blob + field.offset) = nullptr;
                        }
                        else if (
                            old == table::type_code::object
                            || old == table::type_code::interface_
                            || old == table::type_code::class_
                            || old == table::type_code::delegate
                            || old == table::type_code::generic
                            || old == table::type_code::reference)
                        {
                            if (auto const abi
                                = *reinterpret_cast<void**>(blob + field.offset))
                            {
                                static_cast<::IUnknown*>(abi)->Release();
                                *reinterpret_cast<void**>(blob + field.offset)
                                    = nullptr;
                            }
                        }
                        else if (old == table::type_code::struct_ && field.info)
                        {
                            release_struct(*field.info, blob + field.offset);
                        }

                        store_field(*info->owner, field, value, blob);
                        found = true;
                        break;
                    }

                    if (!found)
                    {
                        PyErr_Format(
                            PyExc_TypeError,
                            "'%s' has no field '%U'",
                            info->py_type->tp_name,
                            key);
                        return nullptr;
                    }
                }

                return copy.detach();
            }
            catch (...)
            {
                to_PyErr();
                return nullptr;
            }
        }

        /**
         * The fields of one struct value as a tuple, with a struct field
         * unpacked in turn.
         *
         * Recursing here rather than calling unpack() on a wrapper for the
         * inner value is what keeps it to one Python object per tuple: the
         * nested field's entry was resolved when the type was built, so the
         * bytes of it are right there.
         */
        PyObject* unpack_fields(type_entry& info, uint8_t const* blob) noexcept
        {
            pyobj_handle values{
                PyTuple_New(static_cast<Py_ssize_t>(info.fields.size()))};
            if (!values)
            {
                return nullptr;
            }

            for (size_t i = 0; i < info.fields.size(); i++)
            {
                auto& field = info.fields[i];

                pyobj_handle value{
                    field.code == table::type_code::struct_
                        ? unpack_fields(*field.info, blob + field.offset)
                        : load_field(*info.owner, field, blob)};

                if (!value)
                {
                    return nullptr;
                }

                PyTuple_SET_ITEM(
                    values.get(), static_cast<Py_ssize_t>(i), value.detach());
            }

            return values.detach();
        }

        /**
         * unpack(): the value as a tuple of its fields, which is also what a
         * struct can be built from.
         */
        PyObject* struct_unpack(PyObject* self, PyObject* /*unused*/) noexcept
        {
            auto const info = get_type_entry(Py_TYPE(self));
            if (!info)
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "'%s' is not a struct this runtime built",
                    Py_TYPE(self)->tp_name);
                return nullptr;
            }

            return unpack_fields(
                *info, reinterpret_cast<uint8_t const*>(self) + info->blob_offset);
        }

        PyMethodDef struct_methods[]
            = {{"__replace__",
                reinterpret_cast<PyCFunction>(reinterpret_cast<void*>(struct_replace)),
                METH_VARARGS | METH_KEYWORDS,
                nullptr},
               {"unpack", struct_unpack, METH_NOARGS, nullptr},
               {"_assign_array_", type_assign_array, METH_O | METH_CLASS, nullptr},
               {}};
    } // namespace

    /**
     * Creates the Python type of a struct, with a read only attribute per
     * field.
     */
    bool make_struct_type(
        projection& proj, type_entry& entry, table::type_view const& record)
    {
        auto const layout = proj.table->get_struct_layout(record);

        entry.numerics_kind = numerics::classify(entry.winrt_name);

        if (!numerics::check_layout(entry.numerics_kind, layout.size, layout.align))
        {
            PyErr_Format(
                PyExc_ImportError,
                "'%s' is not laid out the way C++/WinRT lays it out",
                entry.tp_name.c_str());
            return false;
        }

        entry.size = layout.size;
        entry.align = layout.align;
        entry.blob_offset
            = align_up(static_cast<uint32_t>(sizeof(PyObject)), layout.align);
        entry.fields.resize(record.field_count());

        std::vector<PyGetSetDef> getsets;

        for (uint32_t i = 0; i < record.field_count(); i++)
        {
            auto const record_field = record.field(i);
            auto& field = entry.fields[i];

            field.code = record_field.code();
            field.offset = layout.offsets[i];
            field.type = record_field.type();
            field.declaring = &entry;

            pyobj_handle interned{
                PyUnicode_InternFromString(record_field.py_name().data())};
            if (!interned)
            {
                return false;
            }

            proj.names.push_back(interned.get());
            field.name = interned.detach();

            switch (field.code)
            {
            case table::type_code::string:
            case table::type_code::object:
            case table::type_code::interface_:
            case table::type_code::class_:
            case table::type_code::delegate:
            case table::type_code::generic:
            case table::type_code::reference:
                entry.owns_resources = true;
                break;
            case table::type_code::struct_:
                // A struct in a struct is resolved into the field now rather
                // than on first use, because releasing one has to know what
                // it holds and is not a place that can import a package.
                if (!resolve(proj, field.type, field.info))
                {
                    return false;
                }

                if (field.info->owns_resources)
                {
                    entry.owns_resources = true;
                }

                break;
            default:
                break;
            }

            PyGetSetDef def{};
            def.name = record_field.py_name().data();
            def.get = field_get;
            def.closure = &field;

            getsets.push_back(def);
        }

        // A Windows.Foundation.Numerics struct has arithmetic and methods
        // beyond its fields, so the two arrays every struct type is made with
        // are the start of its own rather than the whole of them.
        std::vector<PyMethodDef> methods;

        for (auto* def = struct_methods; def->ml_name; def++)
        {
            methods.push_back(*def);
        }

        numerics::add_methods(entry.numerics_kind, methods);
        methods.push_back({});

        std::vector<PyType_Slot> slots{
            {Py_tp_new, reinterpret_cast<void*>(struct_new)},
            {Py_tp_dealloc,
             reinterpret_cast<void*>(
                 entry.owns_resources ? struct_dealloc_release : struct_dealloc)},
            {Py_tp_repr, reinterpret_cast<void*>(struct_repr)},
            {Py_tp_richcompare, reinterpret_cast<void*>(struct_richcompare)},
            {Py_tp_methods, reinterpret_cast<void*>(keep_methods(proj, methods))},
            {Py_tp_getset, reinterpret_cast<void*>(keep_getsets(proj, getsets))}};

        numerics::add_slots(entry.numerics_kind, slots);
        slots.push_back({});

        PyType_Spec spec{
            entry.tp_name.c_str(),
            static_cast<int>(entry.blob_offset + entry.size),
            0,
            Py_TPFLAGS_DEFAULT,
            slots.data()};

        // The constants and the factory functions of a numerics struct are
        // members of the type rather than of a value, so they sit on a
        // metaclass the way a class's statics do.
        pytype_handle statics{};

        if (numerics::has_statics(entry.numerics_kind))
        {
            statics.attach(numerics::make_statics(proj, entry, entry.numerics_kind));
            if (!statics)
            {
                return false;
            }
        }

        pytype_handle type{
            register_python_type(proj.module, &spec, nullptr, statics.get())};
        if (!type)
        {
            return false;
        }

        entry.py_type = type.detach();
        entry.statics = statics.detach();

        return remember(entry, entry.py_type);
    }

    /**
     * Creates the Python type of a WinRT struct that is one integer, which is
     * a subclass of int rather than a wrapper with a field in it.
     *
     * Nothing else about the record is read: the value is the integer, so
     * there is no layout to work out, no field to read and nothing to give
     * back. What the type is for is what a stub and isinstance() need - an
     * HRESULT told apart from a count - and the name its field had, which
     * __init__.py keeps as a deprecated property of it.
     */
    bool make_integer_type(projection& proj, type_entry& entry)
    {
        PyType_Slot slots[] = {{}};

        PyType_Spec spec{entry.tp_name.c_str(), 0, 0, Py_TPFLAGS_DEFAULT, slots};

        pyobj_handle bases{PyTuple_Pack(1, reinterpret_cast<PyObject*>(&PyLong_Type))};
        if (!bases)
        {
            return false;
        }

        pytype_handle type{
            register_python_type(proj.module, &spec, bases.get(), nullptr)};
        if (!type)
        {
            return false;
        }

        entry.py_type = type.detach();

        return remember(entry, entry.py_type);
    }
} // namespace py::interp

namespace
{
    /**
     * What the table says about the struct type @p type, or @c nullptr with a
     * Python error set.
     */
    py::interp::type_entry* struct_entry(PyTypeObject* type) noexcept
    {
        auto const info = py::interp::get_type_entry(type);
        if (!info)
        {
            PyErr_Format(
                PyExc_TypeError,
                "'%s' is not a type a projection table built",
                type->tp_name);
            return nullptr;
        }

        if (info->category != py::table::category::struct_)
        {
            PyErr_Format(PyExc_TypeError, "'%s' is not a struct", type->tp_name);
            return nullptr;
        }

        return info;
    }
} // namespace

/**
 * The Python object for the struct value at @p value, which must have the
 * layout that the runtime computed for @p type.
 *
 * This and py::struct_from_python() replace the capsule of per-type conversion
 * functions that a projection module used to publish. A struct is passed by
 * value, so the old protocol handed one back as a C++ type and could not be
 * written generically at all.
 */
PyObject* py::struct_to_python(PyTypeObject* type, void const* value) noexcept
{
    auto const info = struct_entry(type);
    if (!info)
    {
        return nullptr;
    }

    return py::interp::struct_to_python(*info, value);
}

/**
 * Reads a Python value of the struct type @p type into @p out, which must be
 * as large as the runtime computed that type to be.
 *
 * @returns @c false with a Python error set. On success @p out owns whatever
 * is in it, which for a struct with a string or an interface field is a
 * reference the caller has to give back.
 */
bool py::struct_from_python(PyTypeObject* type, PyObject* obj, void* out) noexcept
{
    auto const info = struct_entry(type);
    if (!info)
    {
        return false;
    }

    try
    {
        py::interp::struct_from_python(*info, obj, out);

        return true;
    }
    catch (...)
    {
        to_PyErr();
        return false;
    }
}
