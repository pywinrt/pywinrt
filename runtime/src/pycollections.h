// A Python list, dict or iterable, seen from WinRT as a collection.
//
// A projected member that takes an IVector<T>, IMapView<K, V> or one of their
// relatives is given the Python object passed to it rather than a copy of it,
// so that both sides go on seeing the same collection. What WinRT gets is a
// COM object whose vtable comes from the table's record for that parameterized
// instance and whose members run the Python sequence or mapping protocol on
// the object behind it.
//
// This file has what the two protocols share: the object itself, which is
// implements.cpp's with a different answer to the calls that arrive at it, and
// what one call comes down to either way. pysequence.cpp reads a Python
// sequence through it and pymapping.cpp reads a Python mapping; which of them
// a value belongs to is read off the roles that the table gives the interface
// being asked for.
//
// The Python C API calls themselves, and what a Python exception raised during
// one of them means to WinRT, are the py::pyseq_*, py::pymap_* and py::pyiter_*
// entries in <pywinrt/abi.h>, implemented in collections.cpp.

#pragma once

#include <Python.h>

#include "callbacks.h"
#include "implements.h"
#include "interp.h"

#include <cstdint>
#include <cstring>
#include <memory>

namespace py::interp
{
    struct collection_calls;

    /**
     * A second collection over the same Python object, which is what First(),
     * GetView() and the Current() of a map's iterator hand back.
     */
    struct sub_collection
    {
        type_entry* info;
        std::unique_ptr<collection_calls> calls;
    };

    /**
     * What a WinRT caller reads a wrapped Python object through, and the
     * Python state that reading it needs.
     *
     * Each operation below is written as the Python one it is: it is
     * handed the call's inputs already converted and hands back one Python
     * value, and callbacks.cpp does both conversions. So an element's type
     * never appears here - it is the type code in the member's own record,
     * the same one the forward direction uses.
     */
    struct collection_calls : reverse_call
    {
        explicit collection_calls(PyObject* obj) noexcept : obj{Py_XNewRef(obj)}
        {
        }

        int32_t run(reverse_slot const& slot, void* args) noexcept final;

        PyObject* target() const noexcept override
        {
            return obj.get();
        }

        /**
         * Runs the operation that @p slot's role stands for on @p in, the
         * call's inputs, and hands back what its outputs are made of.
         *
         * @throws python_exception on a Python failure, and
         * winrt::hresult_error for one WinRT has a name for - an index the
         * object does not have.
         */
        virtual PyObject* operate(
            reverse_slot const& slot, python_args const& in, uint8_t const* args) = 0;

        /**
         * The collection @p slot's role hands WinRT, for the three
         * members whose output is another one rather than a value.
         *
         * They are apart from operate() because a COM object is not a
         * Python value: making a Python one of it only for the call's own
         * output conversion to take it apart again would cost an object
         * and a QueryInterface on every step of an iteration.
         */
        virtual sub_collection make_collection(reverse_slot const& slot)
        {
            (void)slot;

            return {};
        }

        pyobj_handle obj;
    };

    void* build_collection(
        type_entry& info, std::unique_ptr<collection_calls> calls, void const* iid);

    /**
     * One Python index, back from the value that the call's own input
     * conversion made of it.
     *
     * @throws python_exception if it is not a count, which a member whose
     * table record says @c uint32 cannot produce.
     */
    inline uint32_t to_index(PyObject* value)
    {
        auto const index = PyLong_AsUnsignedLong(value);

        if (index == static_cast<unsigned long>(-1) && PyErr_Occurred())
        {
            throw python_exception();
        }

        return static_cast<uint32_t>(index);
    }

    /**
     * The first input of @p overload, read straight out of the buffer the
     * trampoline spilled it into.
     *
     * Only GetAt() is read this way, because it is the member a WinRT
     * caller reading a whole sequence spends its time in and its one
     * argument is an index: making a Python integer of it, for the
     * operation to ask for the number back, cost a sixth of the call.
     * Every other member is worth the general path, and run() skips the
     * input conversion for this one alone.
     */
    inline uint32_t raw_index(overload_desc& overload, uint8_t const* args) noexcept
    {
        uint32_t index{};

        for (uint16_t i = 0; i < overload.arg_count; i++)
        {
            auto const& arg = overload.args[i];

            if (arg.category != table::param_category::in || arg.is_implicit)
            {
                continue;
            }

            std::memcpy(&index, args + arg.offset, sizeof(index));
            break;
        }

        return index;
    }

    /**
     * @p count as the value the call's own output conversion takes.
     *
     * @throws python_exception if the object cannot be made.
     */
    inline PyObject* from_count(uint32_t count)
    {
        auto* const value = PyLong_FromUnsignedLong(count);

        if (!value)
        {
            throw python_exception();
        }

        return value;
    }

    /**
     * @p value as the boolean the call's own output conversion takes.
     */
    inline PyObject* from_flag(bool value) noexcept
    {
        return PyBool_FromLong(value);
    }

    /**
     * How many elements the array a caller lent has room for.
     *
     * @throws python_exception if the array cannot say, which the one the
     * call's own input conversion made of a WinRT array always can.
     */
    inline uint32_t array_capacity(PyObject* items)
    {
        auto const capacity = PySequence_Size(items);

        if (capacity < 0)
        {
            throw python_exception();
        }

        return static_cast<uint32_t>(capacity);
    }

    /**
     * The output at @p index of @p overload.
     *
     * @throws python_exception if the member has no such output.
     */
    inline arg_desc& output_arg(overload_desc& overload, uint16_t index)
    {
        uint16_t seen = 0;

        for (uint16_t i = 0; i < overload.arg_count; i++)
        {
            auto& arg = overload.args[i];

            if (!is_output(arg))
            {
                continue;
            }

            if (seen++ == index)
            {
                return arg;
            }
        }

        PyErr_Format(
            PyExc_TypeError,
            "'%s' has no output %u",
            overload.winrt_name,
            static_cast<unsigned>(index));
        throw python_exception();
    }

    /**
     * The type of the output at @p index of @p overload, resolved.
     *
     * A member that hands WinRT another collection - First(), GetView(),
     * Split() - says which one in its own record, so nothing here has to
     * know that an IVector<T> iterates as an IIterator<T>.
     *
     * @throws python_exception if the table names no type there.
     */
    inline type_entry& output_type(
        member_desc const& member, overload_desc& overload, uint16_t index)
    {
        auto& arg = output_arg(overload, index);

        if (auto* const info = resolve(*member.owner, arg.type, arg.info))
        {
            return *info;
        }

        throw python_exception();
    }

    void* make_python_collection(type_entry& info, PyObject* obj, void const* iid);
} // namespace py::interp
