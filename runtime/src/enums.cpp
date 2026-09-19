// The Python enum class one table record describes. See enums.h.

#include <Python.h>

#define PYWINRT_RUNTIME_MODULE
#include <pywinrt/base.h>

#include "enums.h"
#include "types.h"

namespace py::interp
{
    namespace
    {
        /**
         * The standard library class the constants are handed to.
         *
         * A WinRT enum that carries FlagsAttribute is a set of bits and is
         * read unsigned, and every other one is a signed number, which is
         * exactly the difference between the two classes here.
         */
        pyobj_handle get_enum_base(bool is_flags) noexcept
        {
            pyobj_handle module{PyImport_ImportModule("enum")};
            if (!module)
            {
                return pyobj_handle{};
            }

            return pyobj_handle{
                PyObject_GetAttrString(module.get(), is_flags ? "IntFlag" : "IntEnum")};
        }

        /**
         * The (name, value) pairs of @p record, in the order it writes them,
         * which is the order the metadata declares them in. That order is what
         * decides which of two constants holding one value is the name and
         * which is the alias.
         */
        pyobj_handle get_constants(
            table::type_view const& record, bool is_flags) noexcept
        {
            pyobj_handle constants{PyList_New(record.constant_count())};
            if (!constants)
            {
                return pyobj_handle{};
            }

            for (uint32_t i = 0; i < record.constant_count(); i++)
            {
                auto const constant = record.constant(i);

                pyobj_handle name{PyUnicode_FromStringAndSize(
                    constant.py_name().data(),
                    static_cast<Py_ssize_t>(constant.py_name().size()))};
                if (!name)
                {
                    return pyobj_handle{};
                }

                pyobj_handle value{
                    is_flags ? PyLong_FromUnsignedLong(constant.value())
                             : PyLong_FromLong(static_cast<int32_t>(constant.value()))};
                if (!value)
                {
                    return pyobj_handle{};
                }

                pyobj_handle pair{PyTuple_Pack(2, name.get(), value.get())};
                if (!pair)
                {
                    return pyobj_handle{};
                }

                if (PyList_SetItem(constants.get(), i, pair.detach()) < 0)
                {
                    return pyobj_handle{};
                }
            }

            return constants;
        }
    } // namespace

    /**
     * Creates the Python enum class of a WinRT enum and binds it in its module.
     *
     * There is no type to assemble slot by slot here: an enum is a name and a
     * list of constants, and what makes a class of those is enum.IntEnum's own
     * call. Saying which module the class belongs to is part of that call,
     * because a Python class remembers where it was written rather than where
     * it ends up, and pickling a member follows that answer back.
     */
    bool make_enum_type(
        projection& proj, type_entry& entry, table::type_view const& record)
    {
        auto const is_flags = (record.flags() & table::type_flags::flags_enum) != 0;

        pyobj_handle base{get_enum_base(is_flags)};
        if (!base)
        {
            return false;
        }

        pyobj_handle constants{get_constants(record, is_flags)};
        if (!constants)
        {
            return false;
        }

        pyobj_handle name{PyUnicode_FromStringAndSize(
            record.py_name().data(), static_cast<Py_ssize_t>(record.py_name().size()))};
        if (!name)
        {
            return false;
        }

        pyobj_handle module_name{PyUnicode_FromString(proj.module_name.c_str())};
        if (!module_name)
        {
            return false;
        }

        pyobj_handle args{PyTuple_Pack(2, name.get(), constants.get())};
        if (!args)
        {
            return false;
        }

        pyobj_handle keywords{PyDict_New()};
        if (!keywords)
        {
            return false;
        }

        if (PyDict_SetItemString(keywords.get(), "module", module_name.get()) < 0)
        {
            return false;
        }

        // An enum is written at the top of its module, so what it is called
        // there and what it is called at all are the same name.
        if (PyDict_SetItemString(keywords.get(), "qualname", name.get()) < 0)
        {
            return false;
        }

        pyobj_handle type{PyObject_Call(base.get(), args.get(), keywords.get())};
        if (!type)
        {
            return false;
        }

        if (PyObject_SetAttr(proj.module, name.get(), type.get()) < 0)
        {
            return false;
        }

        entry.py_type = reinterpret_cast<PyTypeObject*>(type.detach());

        return true;
    }
} // namespace py::interp
