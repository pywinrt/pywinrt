// What a Python list, dict or iterable is to WinRT, in the part that does not
// depend on which of the two Python protocols reads it.
//
// The object is implements.cpp's - one COM object with a tearoff per interface
// - and what is here is the reverse_call it carries: a call that arrives is
// converted the way any other call from WinRT is, the operation the member's
// protocol role stands for is run on the Python object, and what it hands back
// is converted the same way again. So nothing in the runtime matches a WinRT
// member name, and an element's type never appears: it is the type code in the
// member's own record, the same one the forward direction reads.
//
// Which Python protocol a collection is read through is decided at the bottom
// of this file, off the roles the interface's members play rather than off its
// name: a type whose members include a Lookup is a map however it is spelled.

#include <Python.h>

#include <pywinrt/base.h>

#include "callbacks.h"
#include "implements.h"
#include "interp.h"
#include "pycollections.h"
#include "pymapping.h"
#include "pysequence.h"

#include <algorithm>
#include <array>
#include <memory>

namespace py::interp
{
    namespace
    {
        /**
         * How many interfaces one of these answers: the collection interface
         * itself and everything it requires.
         */
        constexpr size_t max_interfaces = 4;

        /**
         * Hands @p abi over as the one output of @p overload, which is what
         * the call's own output conversion would do with a wrapped one.
         */
        void store_collection(
            overload_desc& overload, uint8_t* buffer, void* abi) noexcept
        {
            void* storage{};
            std::memcpy(
                &storage, buffer + output_arg(overload, 0).offset, sizeof(storage));

            if (!storage)
            {
                static_cast<::IUnknown*>(abi)->Release();
                return;
            }

            std::memcpy(storage, &abi, sizeof(abi));
        }
    } // namespace

    int32_t collection_calls::run(reverse_slot const& slot, void* args) noexcept
    {
        if (slot.overload->role == table::member_role::none)
        {
            // A member of an interface the collection interfaces require
            // that is none of their business: the VectorChanged event of
            // an IObservableVector<T>, which a plain Python list has no
            // way to raise.
            return winrt::impl::error_not_implemented;
        }

        auto* const buffer = static_cast<uint8_t*>(args);

        try
        {
            try
            {
                if (auto sub = make_collection(slot); sub.calls)
                {
                    store_collection(
                        *slot.overload,
                        buffer,
                        build_collection(
                            *sub.info, std::move(sub.calls), sub.info->guid));

                    return 0;
                }

                python_args in;

                // GetAt() is the exception that pays for itself: its one
                // argument is an index, it reads it out of the buffer
                // below, and it is the member a WinRT caller reading a
                // whole sequence spends its time in.
                if (slot.overload->role != table::member_role::get_at)
                {
                    read_inputs(*slot.member, *slot.overload, buffer, in);
                }

                pyobj_handle result{operate(slot, in, buffer)};

                if (!result)
                {
                    throw python_exception();
                }

                if (slot.overload->out_count == 0)
                {
                    return 0;
                }

                write_outputs(*slot.member, *slot.overload, result.get(), buffer);

                return 0;
            }
            catch (python_exception const&)
            {
                write_unraisable_and_throw();
            }
        }
        catch (...)
        {
            return winrt::to_hresult();
        }
    }

    void* build_collection(
        type_entry& info, std::unique_ptr<collection_calls> calls, void const* iid)
    {
        // On the stack because one of these is built for every entry of a
        // map that WinRT iterates, and none of the collection interfaces
        // requires more than a couple: an IVector<T> requires the
        // IIterable<T> behind it, an IKeyValuePair<K, V> requires nothing
        // at all.
        std::array<type_entry*, max_interfaces> interfaces{&info};
        size_t count = 1;

        // An interface lists what it requires transitively, and a WinRT
        // caller that holds an IVector<T> is entitled to ask it for any of
        // them.
        auto const required = info.owner->table->type(info.index).interfaces();

        for (uint32_t i = 0; i < required.size(); i++)
        {
            type_entry* cache{};

            auto* const other = resolve(*info.owner, required[i], cache);

            if (!other)
            {
                throw python_exception();
            }

            auto const first = interfaces.begin();

            if (std::find(first, first + count, other) != first + count)
            {
                continue;
            }

            if (count == interfaces.size())
            {
                PyErr_Format(
                    PyExc_NotImplementedError,
                    "'%s' requires more interfaces than a Python "
                    "collection stands in for",
                    info.winrt_name);
                throw python_exception();
            }

            interfaces[count++] = other;
        }

        for (size_t i = 0; i < count; i++)
        {
            if (!ensure_interface_vtable(*interfaces[i]))
            {
                throw python_exception();
            }
        }

        return make_reverse_object({interfaces.data(), count}, iid, std::move(calls));
    }

    namespace
    {
        /**
         * Whether @p info is an IKeyValuePair<K, V>.
         *
         * The member it gives the role pair_key to is what says so, since
         * there is nothing else about the record to tell it from any other
         * two-property interface.
         */
        bool is_mapping_entry(type_entry const& info) noexcept
        {
            for (uint16_t i = 0; i < info.member_count; i++)
            {
                auto const& member = info.members[i];

                for (uint16_t j = 0; j < member.count; j++)
                {
                    if (member.overloads[j].role == table::member_role::pair_key)
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        /**
         * Whether an iterable of @p info's type yields the entries of a map.
         *
         * It is read off the table the long way round - what First() hands
         * back, what that reads an item with, what the item is - because an
         * IIterable<IKeyValuePair<K, V>> is the one collection whose own
         * record says nothing about being a mapping.
         *
         * @throws python_exception if the table does not say what @p info
         * iterates.
         */
        bool iterates_mapping_entries(type_entry& info)
        {
            auto* const first = info.protocol.first;

            if (!first)
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "'%s' names no member to iterate it with",
                    info.winrt_name);
                throw python_exception();
            }

            auto& iterator = output_type(*first, first->overloads[0], 0);

            auto* const current = iterator.protocol.current;

            if (!current)
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "'%s' names no member to read an item with",
                    iterator.winrt_name);
                throw python_exception();
            }

            auto& item = output_arg(current->overloads[0], 0);

            // Only a parameterized instance can be one, and an item of any
            // other type code names no record to ask.
            if (item.code != table::type_code::generic)
            {
                return false;
            }

            auto* const element = resolve(*current->owner, item.type, item.info);

            if (!element)
            {
                throw python_exception();
            }

            return is_mapping_entry(*element);
        }

        /**
         * Refuses @p obj as a value of @p info, naming the Python protocol it
         * would have had to have.
         */
        [[noreturn]] void refuse(
            type_entry const& info, PyObject* obj, char const* protocol)
        {
            PyErr_Format(
                PyExc_TypeError,
                "'%s' takes a %s, not '%s'",
                info.winrt_name,
                protocol,
                Py_TYPE(obj)->tp_name);
            throw python_exception();
        }
    } // namespace

    /**
     * The WinRT collection that stands for @p obj as a value of @p info, with
     * one reference on its @p iid interface.
     *
     * Which collection @p info is, is read off the roles its members play
     * rather than off its name: a type whose members include a Lookup is a
     * map however it is spelled, and one that only knows how to iterate is an
     * IIterable<T>.
     *
     * @returns @c nullptr, with no exception set, when @p info is not a
     * collection at all.
     * @throws python_exception if it is one and @p obj has not got the Python
     * protocol it needs.
     */
    void* make_python_collection(type_entry& info, PyObject* obj, void const* iid)
    {
        if (info.protocol.lookup)
        {
            if (!PyMapping_Check(obj))
            {
                refuse(info, obj, "mapping");
            }

            return build_collection(info, make_mapping_calls(obj), iid);
        }

        if (info.protocol.get_at)
        {
            if (!PySequence_Check(obj))
            {
                refuse(info, obj, "sequence");
            }

            return build_collection(info, make_sequence_calls(obj), iid);
        }

        if (!info.protocol.first)
        {
            return nullptr;
        }

        // An iterable of pairs is how WinRT spells a mapping that is only
        // read in order, so a Python mapping stands in for one by looking
        // each key's value up as it goes rather than by yielding its keys.
        if (iterates_mapping_entries(info) && PyMapping_Check(obj))
        {
            return build_collection(info, make_mapping_calls(obj), iid);
        }

        pyobj_handle iterator{PyObject_GetIter(obj)};

        if (!iterator)
        {
            PyErr_Clear();

            refuse(info, obj, "iterable");
        }

        return build_collection(info, make_sequence_calls(obj), iid);
    }
} // namespace py::interp
