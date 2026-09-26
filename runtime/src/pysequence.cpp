// A Python sequence, seen from WinRT as an IVector<T> or an IVectorView<T>.
//
// Every member below is one of the py::pyseq_* entries, chosen by the protocol
// role the table gives the member that WinRT called, with the call's own
// conversions on either side of it. The iterator is the exception that needs
// state of its own: WinRT reads one item ahead of Python, so the item the
// iterator is on is held here.

#include <Python.h>

#define PYWINRT_RUNTIME_MODULE
#include <pywinrt/base.h>

#include "callbacks.h"
#include "collections.h"
#include "interp.h"
#include "pycollections.h"
#include "pysequence.h"

#include <memory>

namespace py::interp
{
    namespace
    {
        /**
         * A Python sequence as an IVector<T> or an IVectorView<T>, and any
         * Python iterable at all as an IIterable<T>, whose one member is the
         * iterator protocol rather than the sequence one.
         */
        struct sequence_calls final : collection_calls
        {
            using collection_calls::collection_calls;

            PyObject* operate(
                reverse_slot const& slot,
                python_args const& in,
                uint8_t const* args) override;

            sub_collection make_collection(reverse_slot const& slot) override;
        };

        /**
         * A Python iterator as an IIterator<T>.
         *
         * WinRT reads an iterator one item ahead of Python: HasCurrent says
         * whether there is an item before Current is asked for it, and Python
         * only finds out by asking. So the item the iterator is on is held
         * here, and MoveNext is what asks for the next one.
         */
        struct sequence_iterator_calls final : collection_calls
        {
            explicit sequence_iterator_calls(PyObject* iterator)
                : collection_calls{iterator}
            {
                advance();
            }

            void advance();

            PyObject* operate(
                reverse_slot const& slot,
                python_args const& in,
                uint8_t const* args) override;

            /// The item the iterator is on, or nothing once it is exhausted.
            pyobj_handle current;
        };

        PyObject* sequence_calls::operate(
            reverse_slot const& slot, python_args const& in, uint8_t const* args)
        {
            auto* const sequence = obj.get();

            switch (slot.overload->role)
            {
            case table::member_role::size:
            {
                uint32_t size{};

                check_python_result(pyseq_size(sequence, &size));

                return from_count(size);
            }
            case table::member_role::get_at:
            {
                pyobj_handle item;

                check_python_result(pyseq_get_at(
                    sequence, raw_index(*slot.overload, args), item.put()));

                return item.detach();
            }
            case table::member_role::set_at:
                check_python_result(
                    pyseq_set_at(sequence, to_index(in.values[0]), in.values[1]));

                Py_RETURN_NONE;
            case table::member_role::insert_at:
                check_python_result(
                    pyseq_insert_at(sequence, to_index(in.values[0]), in.values[1]));

                Py_RETURN_NONE;
            case table::member_role::remove_at:
                check_python_result(pyseq_remove_at(sequence, to_index(in.values[0])));

                Py_RETURN_NONE;
            case table::member_role::append:
                check_python_result(pyseq_append(sequence, in.values[0]));

                Py_RETURN_NONE;
            case table::member_role::remove_at_end:
                check_python_result(pyseq_remove_at_end(sequence));

                Py_RETURN_NONE;
            case table::member_role::clear:
                check_python_result(pyseq_clear(sequence));

                Py_RETURN_NONE;
            case table::member_role::index_of:
            {
                uint32_t index{};
                bool found{};

                check_python_result(
                    pyseq_index_of(sequence, in.values[0], &index, &found));

                // The return value first and the declared outputs after it,
                // which is the order the projection hands a call's values
                // back in.
                pyobj_handle flag{from_flag(found)};
                pyobj_handle at{from_count(index)};

                return PyTuple_Pack(2, flag.get(), at.get());
            }
            case table::member_role::get_many:
            {
                auto const start = to_index(in.values[0]);
                auto* const items = in.values[1];

                uint32_t size{};

                check_python_result(pyseq_size(sequence, &size));

                if (start > size)
                {
                    throw winrt::hresult_out_of_bounds();
                }

                auto const available = size - start;
                auto count = array_capacity(items);

                if (count > available)
                {
                    count = available;
                }

                for (uint32_t i = 0; i < count; i++)
                {
                    pyobj_handle item;

                    check_python_result(pyseq_get_at(sequence, start + i, item.put()));

                    if (PySequence_SetItem(items, i, item.get()) < 0)
                    {
                        throw python_exception();
                    }
                }

                return from_count(count);
            }
            case table::member_role::replace_all:
            {
                auto* const items = in.values[0];

                auto const count = PySequence_Size(items);

                if (count < 0)
                {
                    throw python_exception();
                }

                check_python_result(pyseq_clear(sequence));

                for (Py_ssize_t i = 0; i < count; i++)
                {
                    pyobj_handle item{PySequence_GetItem(items, i)};

                    if (!item)
                    {
                        throw python_exception();
                    }

                    check_python_result(pyseq_append(sequence, item.get()));
                }

                Py_RETURN_NONE;
            }
            default:
                PyErr_Format(
                    PyExc_NotImplementedError,
                    "a Python sequence has nothing to answer '%s' with",
                    slot.overload->winrt_name);
                throw python_exception();
            }
        }

        sub_collection sequence_calls::make_collection(reverse_slot const& slot)
        {
            switch (slot.overload->role)
            {
            case table::member_role::get_view:
                // The same sequence again, behind the interface that only
                // reads it: a view of a Python list is the list, as it was
                // when the projection compiled a class template per element
                // type.
                return {
                    &output_type(*slot.member, *slot.overload, 0),
                    std::make_unique<sequence_calls>(obj.get())};
            case table::member_role::first:
            {
                pyobj_handle iterator;

                check_python_result(pyiter_first(obj.get(), iterator.put()));

                return {
                    &output_type(*slot.member, *slot.overload, 0),
                    std::make_unique<sequence_iterator_calls>(iterator.get())};
            }
            default:
                return {};
            }
        }

        void sequence_iterator_calls::advance()
        {
            PyObject* item{};

            check_python_result(pyiter_next(obj.get(), &item));

            current.attach(item);
        }

        PyObject* sequence_iterator_calls::operate(
            reverse_slot const& slot, python_args const& in, uint8_t const*)
        {
            switch (slot.overload->role)
            {
            case table::member_role::current:
                if (!current)
                {
                    throw winrt::hresult_out_of_bounds();
                }

                return Py_NewRef(current.get());
            case table::member_role::has_current:
                return from_flag(static_cast<bool>(current));
            case table::member_role::move_next:
                advance();

                return from_flag(static_cast<bool>(current));
            case table::member_role::get_many:
            {
                auto* const items = in.values[0];

                auto const capacity = array_capacity(items);

                uint32_t count = 0;

                while (count < capacity && current)
                {
                    if (PySequence_SetItem(items, count, current.get()) < 0)
                    {
                        throw python_exception();
                    }

                    count++;

                    advance();
                }

                return from_count(count);
            }
            default:
                PyErr_Format(
                    PyExc_NotImplementedError,
                    "a Python iterator has nothing to answer '%s' with",
                    slot.overload->winrt_name);
                throw python_exception();
            }
        }
    } // namespace

    /**
     * What WinRT reads @p sequence as a vector through.
     */
    std::unique_ptr<collection_calls> make_sequence_calls(PyObject* sequence)
    {
        return std::make_unique<sequence_calls>(sequence);
    }

    /**
     * What WinRT reads @p iterator, a Python iterator, as an IIterator<T>
     * through.
     */
    std::unique_ptr<collection_calls> make_sequence_iterator_calls(PyObject* iterator)
    {
        return std::make_unique<sequence_iterator_calls>(iterator);
    }
} // namespace py::interp
