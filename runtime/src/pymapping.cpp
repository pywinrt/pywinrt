// A Python mapping, seen from WinRT as an IMap<K, V> or an IMapView<K, V>.
//
// Every member below is one of the py::pymap_* entries, chosen by the protocol
// role the table gives the member that WinRT called, with the call's own
// conversions on either side of it. What a mapping needs beyond that is a way
// to be iterated as pairs: Python iterates a mapping as keys, so the iterator
// looks each key's value up as it moves onto it, and the pair it hands back is
// a third object of the same kind with the two of them in it.

#include <Python.h>

#include <pywinrt/base.h>

#include "callbacks.h"
#include "collections.h"
#include "interp.h"
#include "objects.h"
#include "pycollections.h"
#include "pymapping.h"

#include <memory>

namespace py::interp
{
    namespace
    {
        /**
         * A Python mapping as an IMap<K, V> or an IMapView<K, V>, and as the
         * IIterable<IKeyValuePair<K, V>> that both of them require.
         */
        struct mapping_calls final : collection_calls
        {
            using collection_calls::collection_calls;

            PyObject* operate(
                reverse_slot const& slot,
                python_args const& in,
                uint8_t const* args) override;

            sub_collection make_collection(reverse_slot const& slot) override;
        };

        /**
         * One entry of a Python mapping as an IKeyValuePair<K, V>.
         *
         * WinRT iterates a map as pairs and Python iterates one as keys, so
         * the pair is something the runtime makes: a two-item tuple of the
         * key and the value, behind the two members the table gives the roles
         * pair_key and pair_value.
         */
        struct pair_calls final : collection_calls
        {
            pair_calls(PyObject* key, PyObject* value) noexcept
                : collection_calls{nullptr}, key{Py_NewRef(key)},
                  value{Py_NewRef(value)}
            {
            }

            PyObject* operate(
                reverse_slot const& slot,
                python_args const& in,
                uint8_t const* args) override;

            pyobj_handle key;
            pyobj_handle value;
        };

        /**
         * The entries of a Python mapping as an
         * IIterator<IKeyValuePair<K, V>>.
         *
         * Python iterates the keys, so each step looks the value up as it
         * goes, and both are held for the same reason a sequence iterator
         * holds its item: WinRT asks whether there is an entry before it asks
         * what the entry is.
         */
        struct mapping_iterator_calls final : collection_calls
        {
            explicit mapping_iterator_calls(PyObject* mapping)
                : collection_calls{mapping}
            {
                check_python_result(pyiter_first(mapping, keys.put()));

                advance();
            }

            void advance();

            PyObject* operate(
                reverse_slot const& slot,
                python_args const& in,
                uint8_t const* args) override;

            sub_collection make_collection(reverse_slot const& slot) override;

            /**
             * The pair the iterator is on.
             *
             * @throws winrt::hresult_error once the keys are exhausted, which
             * is what WinRT calls reading an iterator past its end.
             */
            sub_collection pair(reverse_slot const& slot);

            /// The mapping's own iterator, which yields its keys.
            pyobj_handle keys;
            pyobj_handle current_key;
            pyobj_handle current_value;
        };

        PyObject* mapping_calls::operate(
            reverse_slot const& slot, python_args const& in, uint8_t const*)
        {
            auto* const mapping = obj.get();

            switch (slot.overload->role)
            {
            case table::member_role::size:
            {
                uint32_t size{};

                check_python_result(pymap_size(mapping, &size));

                return from_count(size);
            }
            case table::member_role::lookup:
            {
                pyobj_handle value;

                check_python_result(pymap_lookup(mapping, in.values[0], value.put()));

                return value.detach();
            }
            case table::member_role::has_key:
            {
                bool has_key{};

                check_python_result(pymap_has_key(mapping, in.values[0], &has_key));

                return from_flag(has_key);
            }
            case table::member_role::insert:
            {
                bool replaced{};

                check_python_result(
                    pymap_insert(mapping, in.values[0], in.values[1], &replaced));

                return from_flag(replaced);
            }
            case table::member_role::remove:
                check_python_result(pymap_remove(mapping, in.values[0]));

                Py_RETURN_NONE;
            case table::member_role::clear:
                check_python_result(pymap_clear(mapping));

                Py_RETURN_NONE;
            case table::member_role::split:
                // A null pair is how IMapView<K, V>::Split() says that the
                // map cannot be split, which is the only answer a Python
                // mapping has.
                return PyTuple_Pack(2, Py_None, Py_None);
            default:
                PyErr_Format(
                    PyExc_NotImplementedError,
                    "a Python mapping has nothing to answer '%s' with",
                    slot.overload->winrt_name);
                throw python_exception();
            }
        }

        sub_collection mapping_calls::make_collection(reverse_slot const& slot)
        {
            switch (slot.overload->role)
            {
            case table::member_role::get_view:
                return {
                    &output_type(*slot.member, *slot.overload, 0),
                    std::make_unique<mapping_calls>(obj.get())};
            case table::member_role::first:
                return {
                    &output_type(*slot.member, *slot.overload, 0),
                    std::make_unique<mapping_iterator_calls>(obj.get())};
            default:
                return {};
            }
        }

        void mapping_iterator_calls::advance()
        {
            PyObject* key{};
            PyObject* value{};

            check_python_result(pymap_iter_next(obj.get(), keys.get(), &key, &value));

            current_key.attach(key);
            current_value.attach(value);
        }

        sub_collection mapping_iterator_calls::pair(reverse_slot const& slot)
        {
            if (!current_key)
            {
                throw winrt::hresult_out_of_bounds();
            }

            return {
                &output_type(*slot.member, *slot.overload, 0),
                std::make_unique<pair_calls>(current_key.get(), current_value.get())};
        }

        sub_collection mapping_iterator_calls::make_collection(reverse_slot const& slot)
        {
            if (slot.overload->role != table::member_role::current)
            {
                return {};
            }

            return pair(slot);
        }

        PyObject* mapping_iterator_calls::operate(
            reverse_slot const& slot, python_args const& in, uint8_t const*)
        {
            switch (slot.overload->role)
            {
            case table::member_role::has_current:
                return from_flag(static_cast<bool>(current_key));
            case table::member_role::move_next:
                advance();

                return from_flag(static_cast<bool>(current_key));
            case table::member_role::get_many:
            {
                auto* const items = in.values[0];

                auto const capacity = array_capacity(items);

                uint32_t count = 0;

                while (count < capacity && current_key)
                {
                    auto entry = pair(slot);

                    pyobj_handle wrapper{wrap_abi(
                        entry.info->py_type,
                        build_collection(
                            *entry.info, std::move(entry.calls), entry.info->guid))};

                    if (!wrapper)
                    {
                        throw python_exception();
                    }

                    if (PySequence_SetItem(items, count, wrapper.get()) < 0)
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
                    "a Python mapping's iterator has nothing to answer '%s' "
                    "with",
                    slot.overload->winrt_name);
                throw python_exception();
            }
        }

        PyObject* pair_calls::operate(
            reverse_slot const& slot, python_args const&, uint8_t const*)
        {
            switch (slot.overload->role)
            {
            case table::member_role::pair_key:
                return Py_NewRef(key.get());
            case table::member_role::pair_value:
                return Py_NewRef(value.get());
            default:
                PyErr_Format(
                    PyExc_NotImplementedError,
                    "a mapping entry has nothing to answer '%s' with",
                    slot.overload->winrt_name);
                throw python_exception();
            }
        }
    } // namespace

    /**
     * What WinRT reads @p mapping as a map through.
     */
    std::unique_ptr<collection_calls> make_mapping_calls(PyObject* mapping)
    {
        return std::make_unique<mapping_calls>(mapping);
    }

    /**
     * What WinRT reads the entries of @p mapping as an
     * IIterator<IKeyValuePair<K, V>> through.
     */
    std::unique_ptr<collection_calls> make_mapping_iterator_calls(PyObject* mapping)
    {
        return std::make_unique<mapping_iterator_calls>(mapping);
    }
} // namespace py::interp
