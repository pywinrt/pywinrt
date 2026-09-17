// The WinRT collection interfaces, implemented on top of Python objects.
//
// When a projected method takes an IVector<T>, IMap<K, V> or one of their
// relatives, the Python list, dict or iterable passed in is wrapped in one of
// the implementations here rather than copied, so that both sides see the same
// collection. Each of them holds a reference to the Python object and takes the
// GIL before touching it, because WinRT may call in - or release the collection
// - on any thread.
//
// What each member does to the Python object is not here, though. The WinRT ABI
// of IVector<T> passes T by value, so the class templates have to be compiled
// into the calling module, but everything inside them that does not name T is
// one of the py::py{seq,iter,map}_* entries in <pywinrt/abi.h>, which is also
// where their shared contract - GIL held by the caller, borrowed references in,
// HRESULT out - is written down. So a member here takes the GIL, calls one
// entry, and converts what comes back; the Python C API call and the policy for
// what a Python exception means to WinRT belong to winrt-runtime.
//
// The py::converter<T> specializations at the bottom are what select them.

#pragma once

#include <pywinrt/abi.h>
#include <pywinrt/convert.h>
#include <pywinrt/errors.h>
#include <pywinrt/handles.h>
#include <pywinrt/prelude.h>
#include <pywinrt/traits.h>

namespace py
{
    /**
     * Throws if one of the collection entries in <pywinrt/abi.h> failed.
     *
     * @throws winrt::hresult_error
     */
    inline void check_python_result(int32_t hr)
    {
        if (hr < 0)
        {
            if (hr == unraisable_python_exception)
            {
                // The runtime has already reported it, so the only thing left
                // to do with it is to name it.
                throw_unraisable(winrt::hresult{hr});
            }

            winrt::throw_hresult(hr);
        }
    }

    /**
     * Converts @p value to the Python object that goes into the collection.
     *
     * @throws winrt::hresult_error if the conversion fails. There is no Python
     * caller to raise it to - WinRT is the one calling - so the Python error is
     * reported as unraisable, as it is for a failure inside the entries.
     */
    template<typename T>
    [[nodiscard]] pyobj_handle convert_or_unraisable(T const& value)
    {
        pyobj_handle obj{converter<T>::convert(value)};

        if (!obj)
        {
            write_unraisable_and_throw();
        }

        return obj;
    }

    /**
     * Converts @p obj, which came out of the collection, to T.
     *
     * @throws winrt::hresult_error if the conversion fails, for the same reason
     * as py::convert_or_unraisable().
     */
    template<typename T>
    [[nodiscard]] T convert_to_or_unraisable(PyObject* obj)
    {
        try
        {
            return converter<T>::convert_to(obj);
        }
        catch (python_exception const&)
        {
            write_unraisable_and_throw();
        }
    }

    template<typename D, typename... I>
    struct python_implements : winrt::implements<D, I...>
    {
        // WinRT may free these objects without holding the GIL, so we need to
        // make sure that the GIL is held when we release the object so that
        // the Python finalizers can be called safely, if needed.
        static void final_release(std::unique_ptr<D> self)
        {
            auto gil = ensure_gil();
            self.reset();
        }
    };

    template<typename T>
    struct python_iterator : python_implements<
                                 python_iterator<T>,
                                 winrt::Windows::Foundation::Collections::IIterator<T>>
    {
        pyobj_handle _iterator;
        std::optional<T> _current_value;

        /**
         * Takes ownership of @p iterator, which must be a Python iterator, and
         * moves onto its first item. The GIL must be held.
         */
        explicit python_iterator(pyobj_handle&& iterator)
            : _iterator(std::move(iterator))
        {
            _current_value = get_next();
        }

        /**
         * Advances the Python iterator and converts what it yields, or returns
         * no value once it is exhausted. The GIL must be held.
         */
        std::optional<T> get_next()
        {
            pyobj_handle item;

            check_python_result(pyiter_next(_iterator.get(), item.put()));

            if (!item)
            {
                return std::nullopt;
            }

            return convert_to_or_unraisable<T>(item.get());
        }

        auto Current() const
        {
            // The item was converted when the iterator moved onto it, so this
            // touches no Python object and does not need the GIL.
            return _current_value.value();
        }

        bool HasCurrent() const
        {
            return _current_value.has_value();
        }

        bool MoveNext()
        {
            auto gil = ensure_gil();

            _current_value = get_next();

            return _current_value.has_value();
        }

        uint32_t GetMany(winrt::array_view<T> items)
        {
            auto gil = ensure_gil();

            uint32_t count{};

            while (count < items.size() && _current_value.has_value())
            {
                items[count++] = _current_value.value();
                _current_value = get_next();
            }

            return count;
        }
    };

    template<typename T>
    struct python_iterable : python_implements<
                                 python_iterable<T>,
                                 winrt::Windows::Foundation::Collections::IIterable<T>>
    {
        pyobj_handle _iterable;

        explicit python_iterable(PyObject* iterable) : _iterable(iterable)
        {
            Py_INCREF(_iterable.get());
        }

        auto First() const
        {
            auto gil = ensure_gil();

            pyobj_handle iterator;

            check_python_result(pyiter_first(_iterable.get(), iterator.put()));

            return winrt::make<python_iterator<T>>(std::move(iterator));
        }
    };

    /**
     * The GetMany() of both vector types, which are the same function with
     * different const-ness. The GIL must be held.
     */
    template<typename T>
    uint32_t get_many_from(
        PyObject* sequence, uint32_t start_index, winrt::array_view<T> items)
    {
        uint32_t size{};

        check_python_result(pyseq_size(sequence, &size));

        if (start_index > size)
        {
            throw winrt::hresult_out_of_bounds();
        }

        const auto available = size - start_index;
        const auto count = items.size() < available ? items.size() : available;

        for (uint32_t i = 0; i < count; i++)
        {
            pyobj_handle item;

            check_python_result(pyseq_get_at(sequence, start_index + i, item.put()));

            items[i] = convert_to_or_unraisable<T>(item.get());
        }

        return count;
    }

    template<typename T>
    struct python_vector_view
        : python_implements<
              python_vector_view<T>,
              winrt::Windows::Foundation::Collections::IVectorView<T>,
              winrt::Windows::Foundation::Collections::IIterable<T>>
    {
        pyobj_handle _sequence;

        // It is up to the caller to ensure that this is a sequence object!
        explicit python_vector_view(PyObject* sequence) : _sequence(sequence)
        {
            Py_INCREF(_sequence.get());
        }

        T GetAt(uint32_t index) const
        {
            auto gil = ensure_gil();

            pyobj_handle item;

            check_python_result(pyseq_get_at(_sequence.get(), index, item.put()));

            return convert_to_or_unraisable<T>(item.get());
        }

        uint32_t GetMany(uint32_t start_index, winrt::array_view<T> items) const
        {
            auto gil = ensure_gil();

            return get_many_from(_sequence.get(), start_index, items);
        }

        bool IndexOf(T const& value, uint32_t& index) const
        {
            auto gil = ensure_gil();

            auto py_value = convert_or_unraisable<T>(value);

            bool found{};

            check_python_result(
                pyseq_index_of(_sequence.get(), py_value.get(), &index, &found));

            return found;
        }

        uint32_t Size() const
        {
            auto gil = ensure_gil();

            uint32_t size{};

            check_python_result(pyseq_size(_sequence.get(), &size));

            return size;
        }

        auto First() const
        {
            auto gil = ensure_gil();

            pyobj_handle iterator;

            check_python_result(pyiter_first(_sequence.get(), iterator.put()));

            return winrt::make<python_iterator<T>>(std::move(iterator));
        }
    };

    template<typename T>
    struct python_vector : python_implements<
                               python_vector<T>,
                               winrt::Windows::Foundation::Collections::IVector<T>,
                               winrt::Windows::Foundation::Collections::IIterable<T>>
    {
        pyobj_handle _sequence;

        // It is up to the caller to ensure that this is a sequence object!
        explicit python_vector(PyObject* sequence) : _sequence(sequence)
        {
            Py_INCREF(_sequence.get());
        }

        void Append(T const& value)
        {
            auto gil = ensure_gil();

            auto py_value = convert_or_unraisable<T>(value);

            check_python_result(pyseq_append(_sequence.get(), py_value.get()));
        }

        void Clear() const
        {
            auto gil = ensure_gil();

            check_python_result(pyseq_clear(_sequence.get()));
        }

        T GetAt(uint32_t index) const
        {
            auto gil = ensure_gil();

            pyobj_handle item;

            check_python_result(pyseq_get_at(_sequence.get(), index, item.put()));

            return convert_to_or_unraisable<T>(item.get());
        }

        uint32_t GetMany(uint32_t start_index, winrt::array_view<T> items) const
        {
            auto gil = ensure_gil();

            return get_many_from(_sequence.get(), start_index, items);
        }

        winrt::Windows::Foundation::Collections::IVectorView<T> GetView() const
        {
            auto gil = ensure_gil();

            return winrt::make<python_vector_view<T>>(_sequence.get());
        }

        bool IndexOf(T const& value, uint32_t& index) const
        {
            auto gil = ensure_gil();

            auto py_value = convert_or_unraisable<T>(value);

            bool found{};

            check_python_result(
                pyseq_index_of(_sequence.get(), py_value.get(), &index, &found));

            return found;
        }

        void InsertAt(uint32_t index, T const& value) const
        {
            auto gil = ensure_gil();

            auto py_value = convert_or_unraisable<T>(value);

            check_python_result(
                pyseq_insert_at(_sequence.get(), index, py_value.get()));
        }

        void RemoveAt(uint32_t index) const
        {
            auto gil = ensure_gil();

            check_python_result(pyseq_remove_at(_sequence.get(), index));
        }

        void RemoveAtEnd() const
        {
            auto gil = ensure_gil();

            check_python_result(pyseq_remove_at_end(_sequence.get()));
        }

        void ReplaceAll(winrt::array_view<T const> items)
        {
            auto gil = ensure_gil();

            check_python_result(pyseq_clear(_sequence.get()));

            for (auto const& item : items)
            {
                auto py_item = convert_or_unraisable<T>(item);

                check_python_result(pyseq_append(_sequence.get(), py_item.get()));
            }
        }

        void SetAt(uint32_t index, T const& value) const
        {
            auto gil = ensure_gil();

            auto py_value = convert_or_unraisable<T>(value);

            check_python_result(pyseq_set_at(_sequence.get(), index, py_value.get()));
        }

        uint32_t Size() const
        {
            auto gil = ensure_gil();

            uint32_t size{};

            check_python_result(pyseq_size(_sequence.get(), &size));

            return size;
        }

        auto First() const
        {
            auto gil = ensure_gil();

            pyobj_handle iterator;

            check_python_result(pyiter_first(_sequence.get(), iterator.put()));

            return winrt::make<python_iterator<T>>(std::move(iterator));
        }
    };

    template<typename K, typename V>
    struct python_mapping_iterator
        : python_implements<
              python_mapping_iterator<K, V>,
              winrt::Windows::Foundation::Collections::IIterator<
                  winrt::Windows::Foundation::Collections::IKeyValuePair<K, V>>>
    {
        using KVPair = winrt::Windows::Foundation::Collections::IKeyValuePair<K, V>;

        pyobj_handle _mapping;
        pyobj_handle _iterator;
        std::optional<KVPair> _current_value;

        /**
         * Iterates the keys of @p mapping, looking each value up as it goes.
         * The GIL must be held.
         */
        explicit python_mapping_iterator(PyObject* mapping) : _mapping(mapping)
        {
            Py_INCREF(_mapping.get());

            check_python_result(pyiter_first(_mapping.get(), _iterator.put()));

            _current_value = get_next();
        }

        /**
         * Advances to the next entry of the mapping, or returns no value once
         * the keys are exhausted. The GIL must be held.
         */
        std::optional<KVPair> get_next()
        {
            pyobj_handle key;
            pyobj_handle value;

            check_python_result(pymap_iter_next(
                _mapping.get(), _iterator.get(), key.put(), value.put()));

            if (!key)
            {
                return std::nullopt;
            }

            auto pair_key = convert_to_or_unraisable<K>(key.get());
            auto pair_value = convert_to_or_unraisable<V>(value.get());

            return winrt::make<winrt::impl::key_value_pair<KVPair>>(
                pair_key, pair_value);
        }

        KVPair Current() const
        {
            // The entry was converted when the iterator moved onto it, so this
            // touches no Python object and does not need the GIL.
            return _current_value.value();
        }

        bool HasCurrent() const
        {
            return _current_value.has_value();
        }

        bool MoveNext()
        {
            auto gil = ensure_gil();

            _current_value = get_next();

            return _current_value.has_value();
        }

        uint32_t GetMany(winrt::array_view<KVPair> items)
        {
            auto gil = ensure_gil();

            uint32_t count{};

            while (count < items.size() && _current_value.has_value())
            {
                items[count++] = _current_value.value();
                _current_value = get_next();
            }

            return count;
        }
    };

    template<typename K, typename V>
    struct python_map_view
        : python_implements<
              python_map_view<K, V>,
              winrt::Windows::Foundation::Collections::IMapView<K, V>,
              winrt::Windows::Foundation::Collections::IIterable<
                  winrt::Windows::Foundation::Collections::IKeyValuePair<K, V>>>
    {
        pyobj_handle _mapping;

        // It is up to the caller to ensure that this is a mapping object!
        explicit python_map_view(PyObject* mapping) : _mapping(mapping)
        {
            Py_INCREF(_mapping.get());
        }

        bool HasKey(K const& key) const
        {
            auto gil = ensure_gil();

            auto py_key = convert_or_unraisable<K>(key);

            bool has_key{};

            check_python_result(
                pymap_has_key(_mapping.get(), py_key.get(), &has_key));

            return has_key;
        }

        V Lookup(K const& key) const
        {
            auto gil = ensure_gil();

            auto py_key = convert_or_unraisable<K>(key);

            pyobj_handle item;

            check_python_result(
                pymap_lookup(_mapping.get(), py_key.get(), item.put()));

            return convert_to_or_unraisable<V>(item.get());
        }

        void Split(
            winrt::Windows::Foundation::Collections::IMapView<K, V>& first,
            winrt::Windows::Foundation::Collections::IMapView<K, V>& second) const
        {
            // null return indicates map cannot be split
            first = nullptr;
            second = nullptr;
        }

        uint32_t Size() const
        {
            auto gil = ensure_gil();

            uint32_t size{};

            check_python_result(pymap_size(_mapping.get(), &size));

            return size;
        }

        winrt::Windows::Foundation::Collections::IIterator<
            winrt::Windows::Foundation::Collections::IKeyValuePair<K, V>>
        First() const
        {
            auto gil = ensure_gil();

            return winrt::make<python_mapping_iterator<K, V>>(_mapping.get());
        }
    };

    template<typename K, typename V>
    struct python_map
        : python_implements<
              python_map<K, V>,
              winrt::Windows::Foundation::Collections::IMap<K, V>,
              winrt::Windows::Foundation::Collections::IIterable<
                  winrt::Windows::Foundation::Collections::IKeyValuePair<K, V>>>
    {
        pyobj_handle _mapping;

        // It is up to the caller to ensure that this is a mapping object!
        explicit python_map(PyObject* mapping) : _mapping(mapping)
        {
            Py_INCREF(_mapping.get());
        }

        void Clear() const
        {
            auto gil = ensure_gil();

            check_python_result(pymap_clear(_mapping.get()));
        }

        winrt::Windows::Foundation::Collections::IMapView<K, V> GetView() const
        {
            auto gil = ensure_gil();

            return winrt::make<python_map_view<K, V>>(_mapping.get());
        }

        bool HasKey(K const& key) const
        {
            auto gil = ensure_gil();

            auto py_key = convert_or_unraisable<K>(key);

            bool has_key{};

            check_python_result(
                pymap_has_key(_mapping.get(), py_key.get(), &has_key));

            return has_key;
        }

        bool Insert(K const& key, V const& value) const
        {
            auto gil = ensure_gil();

            auto py_key = convert_or_unraisable<K>(key);
            auto py_value = convert_or_unraisable<V>(value);

            bool replaced{};

            check_python_result(pymap_insert(
                _mapping.get(), py_key.get(), py_value.get(), &replaced));

            return replaced;
        }

        V Lookup(K const& key) const
        {
            auto gil = ensure_gil();

            auto py_key = convert_or_unraisable<K>(key);

            pyobj_handle item;

            check_python_result(
                pymap_lookup(_mapping.get(), py_key.get(), item.put()));

            return convert_to_or_unraisable<V>(item.get());
        }

        void Remove(K const& key) const
        {
            auto gil = ensure_gil();

            auto py_key = convert_or_unraisable<K>(key);

            check_python_result(pymap_remove(_mapping.get(), py_key.get()));
        }

        uint32_t Size() const
        {
            auto gil = ensure_gil();

            uint32_t size{};

            check_python_result(pymap_size(_mapping.get(), &size));

            return size;
        }

        winrt::Windows::Foundation::Collections::IIterator<
            winrt::Windows::Foundation::Collections::IKeyValuePair<K, V>>
        First() const
        {
            auto gil = ensure_gil();

            return winrt::make<python_mapping_iterator<K, V>>(_mapping.get());
        }
    };

    template<typename TItem>
    struct converter<winrt::Windows::Foundation::Collections::IIterable<TItem>>
    {
        using TCollection = winrt::Windows::Foundation::Collections::IIterable<TItem>;

        static PyObject* convert(TCollection const& instance) noexcept
        {
            return wrap(instance);
        }

        static TCollection convert_to(PyObject* obj)
        {
            if (auto result = convert_interface_to<TCollection>(obj))
            {
                return result.value();
            }

            if constexpr (is_key_value_pair_v<TItem>)
            {
                if (PyMapping_Check(obj))
                {
                    using K = key_value_pair_type<TItem>::key_type;
                    using V = key_value_pair_type<TItem>::value_type;
                    return winrt::make<python_map_view<K, V>>(obj);
                }
            }

            pyobj_handle iterator{PyObject_GetIter(obj)};

            if (!iterator)
            {
                throw python_exception();
            }

            return winrt::make<python_iterable<TItem>>(obj);
        }
    };

    template<typename TItem>
    struct converter<winrt::Windows::Foundation::Collections::IVectorView<TItem>>
    {
        using TCollection = winrt::Windows::Foundation::Collections::IVectorView<TItem>;
        using TMutableCollection
            = winrt::Windows::Foundation::Collections::IVector<TItem>;

        static PyObject* convert(TCollection const& instance) noexcept
        {
            return wrap(instance);
        }

        static auto convert_to(PyObject* obj)
        {
            if (auto result = convert_interface_to<TCollection>(obj))
            {
                return result.value();
            }

            if (auto result = convert_interface_to<TMutableCollection>(obj))
            {
                return result.value().GetView();
            }

            if (!PySequence_Check(obj))
            {
                PyErr_SetString(PyExc_TypeError, "expected a sequence");
                throw python_exception();
            }

            return winrt::make<python_vector_view<TItem>>(obj);
        }
    };

    template<typename TItem>
    struct converter<winrt::Windows::Foundation::Collections::IVector<TItem>>
    {
        using TCollection = winrt::Windows::Foundation::Collections::IVector<TItem>;

        static PyObject* convert(TCollection const& instance) noexcept
        {
            return wrap(instance);
        }

        static auto convert_to(PyObject* obj)
        {
            if (auto result = convert_interface_to<TCollection>(obj))
            {
                return result.value();
            }

            if (!PySequence_Check(obj))
            {
                PyErr_SetString(PyExc_TypeError, "expected a sequence");
                throw python_exception();
            }

            return winrt::make<python_vector<TItem>>(obj);
        }
    };

    template<typename TKey, typename TValue>
    struct converter<winrt::Windows::Foundation::Collections::IMapView<TKey, TValue>>
    {
        using TMap = winrt::Windows::Foundation::Collections::IMapView<TKey, TValue>;

        static PyObject* convert(TMap const& instance) noexcept
        {
            return wrap(instance);
        }

        static auto convert_to(PyObject* obj)
        {
            if (auto result = convert_interface_to<TMap>(obj))
            {
                return result.value();
            }

            if (!PyMapping_Check(obj))
            {
                PyErr_SetString(PyExc_TypeError, "expected a mapping");
                throw python_exception();
            }

            return winrt::make<python_map_view<TKey, TValue>>(obj);
        }
    };

    template<typename TKey, typename TValue>
    struct converter<winrt::Windows::Foundation::Collections::IMap<TKey, TValue>>
    {
        using TMap = winrt::Windows::Foundation::Collections::IMap<TKey, TValue>;

        static PyObject* convert(TMap const& instance) noexcept
        {
            return wrap(instance);
        }

        static auto convert_to(PyObject* obj)
        {
            if (auto result = convert_interface_to<TMap>(obj))
            {
                return result.value();
            }

            if (!PyMapping_Check(obj))
            {
                PyErr_SetString(PyExc_TypeError, "expected a mapping");
                throw python_exception();
            }

            return winrt::make<python_map<TKey, TValue>>(obj);
        }
    };
} // namespace py
