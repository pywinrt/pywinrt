// The WinRT collection interfaces, implemented on top of Python objects.
//
// When a projected method takes an IVector<T>, IMap<K, V> or one of their
// relatives, the Python list, dict or iterable passed in is wrapped in one of
// the implementations here rather than copied, so that both sides see the same
// collection. Each of them holds a reference to the Python object and takes the
// GIL before touching it, because WinRT may call in - or release the collection
// - on any thread.
//
// The py::converter<T> specializations at the bottom are what select them.

#pragma once

#include <pywinrt/convert.h>
#include <pywinrt/errors.h>
#include <pywinrt/handles.h>
#include <pywinrt/prelude.h>
#include <pywinrt/traits.h>

namespace py
{
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

        static std::optional<T> get_next(pyobj_handle const& iterator)
        {
            if (!iterator)
            {
                if (!PyErr_Occurred())
                {
                    PyErr_SetString(PyExc_SystemError, "iterator is null");
                }

                throw python_exception();
            }

            pyobj_handle next{PyIter_Next(iterator.get())};
            if (!next)
            {
                if (PyErr_Occurred())
                {
                    throw python_exception();
                }
                else
                {
                    return std::nullopt;
                }
            }

            return converter<T>::convert_to(next.get());
        }

        python_iterator(PyObject* i) : _iterator(i)
        {
            if (!_iterator)
            {
                if (!PyErr_Occurred())
                {
                    PyErr_SetString(PyExc_SystemError, "iterator is null");
                }

                throw python_exception();
            }

            _current_value = get_next(_iterator);
        }

        auto Current() const
        {
            auto gil = ensure_gil();

            try
            {
                return _current_value.value();
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        bool HasCurrent() const
        {
            auto gil = ensure_gil();

            try
            {
                return _current_value.has_value();
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        bool MoveNext()
        {
            auto gil = ensure_gil();

            try
            {
                _current_value = get_next(_iterator);
                return _current_value.has_value();
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        uint32_t GetMany(winrt::array_view<T> /*unused*/)
        {
            auto gil = ensure_gil();

            try
            {
                // TODO: implement GetMany
                PyErr_Format(
                    PyExc_NotImplementedError,
                    "py::python_iterator<%s>::GetMany() is not implemented",
                    type_name<T>());
                throw python_exception();
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
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

            try
            {
                return winrt::make<python_iterator<T>>(
                    PyObject_GetIter(_iterable.get()));
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }
    };

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

            try
            {
                pyobj_handle item{PySequence_GetItem(_sequence.get(), index)};

                if (!item)
                {
                    throw python_exception();
                }

                return converter<T>::convert_to(item.get());
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        uint32_t GetMany(uint32_t /*unused*/, winrt::array_view<T> /*unused*/)
        {
            auto gil = ensure_gil();

            try
            {
                // TODO: implement GetMany
                PyErr_Format(
                    PyExc_NotImplementedError,
                    "py::python_vector<%s>::GetMany() is not implemented",
                    type_name<T>());
                throw python_exception();
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        bool IndexOf(T const& value, uint32_t& index) const
        {
            auto gil = ensure_gil();

            try
            {
                pyobj_handle py_value{converter<T>::convert(value)};

                if (!py_value)
                {
                    throw python_exception();
                }

                auto py_index = PySequence_Index(_sequence.get(), py_value.get());

                if (py_index == -1)
                {
                    if (PyErr_ExceptionMatches(PyExc_ValueError))
                    {
                        PyErr_Clear();
                        return false;
                    }

                    throw python_exception();
                }

                index = static_cast<uint32_t>(py_index);

                return true;
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        uint32_t Size() const
        {
            auto gil = ensure_gil();

            try
            {
                auto size = PySequence_Size(_sequence.get());
                if (size == -1 && PyErr_Occurred())
                {
                    throw python_exception();
                }

                return static_cast<uint32_t>(size);
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        auto First() const
        {
            auto gil = ensure_gil();

            try
            {
                return winrt::make<python_iterator<T>>(
                    PyObject_GetIter(_sequence.get()));
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
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

            try
            {
                pyobj_handle py_value{converter<T>::convert(value)};

                if (!py_value)
                {
                    throw python_exception();
                }

                pyobj_handle result{PyObject_CallMethod(
                    _sequence.get(), "append", "O", py_value.get())};

                if (!result)
                {
                    throw python_exception();
                }
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        void Clear() const
        {
            auto gil = ensure_gil();

            try
            {
                if (PySequence_SetSlice(_sequence.get(), 0, PY_SSIZE_T_MAX, NULL) == -1)
                {
                    throw python_exception();
                }
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        T GetAt(uint32_t index) const
        {
            auto gil = ensure_gil();

            try
            {
                pyobj_handle item{PySequence_GetItem(_sequence.get(), index)};

                if (!item)
                {
                    throw python_exception();
                }

                return converter<T>::convert_to(item.get());
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        uint32_t GetMany(uint32_t /*unused*/, winrt::array_view<T> /*unused*/) const
        {
            auto gil = ensure_gil();

            try
            {
                // TODO: implement GetMany
                PyErr_Format(
                    PyExc_NotImplementedError,
                    "py::python_vector<%s>::GetMany() is not implemented",
                    type_name<T>());
                throw python_exception();
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        winrt::Windows::Foundation::Collections::IVectorView<T> GetView() const
        {
            auto gil = ensure_gil();

            try
            {
                return winrt::make<python_vector_view<T>>(_sequence.get());
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        bool IndexOf(T const& value, uint32_t& index) const
        {
            auto gil = ensure_gil();

            try
            {
                pyobj_handle py_value{converter<T>::convert(value)};

                if (!py_value)
                {
                    throw python_exception();
                }

                auto py_index = PySequence_Index(_sequence.get(), py_value.get());

                if (py_index == -1)
                {
                    if (PyErr_ExceptionMatches(PyExc_ValueError))
                    {
                        PyErr_Clear();
                        return false;
                    }

                    throw python_exception();
                }

                index = static_cast<uint32_t>(py_index);

                return true;
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        void InsertAt(uint32_t index, T const& value) const
        {
            auto gil = ensure_gil();

            try
            {
                pyobj_handle py_value{converter<T>::convert(value)};

                if (!py_value)
                {
                    throw python_exception();
                }

                pyobj_handle result{PyObject_CallMethod(
                    _sequence.get(), "insert", "IO", index, py_value.get())};

                if (!result)
                {
                    throw python_exception();
                }
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        void RemoveAt(uint32_t index) const
        {
            auto gil = ensure_gil();

            try
            {
                if (PySequence_DelItem(_sequence.get(), index) == -1)
                {
                    throw python_exception();
                }
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        void RemoveAtEnd() const
        {
            auto gil = ensure_gil();

            try
            {
                RemoveAt(Size() - 1);
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        void ReplaceAll(winrt::array_view<T const> items)
        {
            auto gil = ensure_gil();

            try
            {
                Clear();

                for (auto const& item : items)
                {
                    Append(item);
                }
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        void SetAt(uint32_t index, T const& value) const
        {
            auto gil = ensure_gil();

            try
            {
                pyobj_handle py_value{converter<T>::convert(value)};

                if (!py_value)
                {
                    throw python_exception();
                }

                if (PyList_SetItem(_sequence.get(), index, py_value.get()) == -1)
                {
                    if (PyErr_ExceptionMatches(PyExc_IndexError))
                    {
                        PyErr_Clear();
                        throw winrt::hresult_out_of_bounds();
                    }

                    throw python_exception();
                }
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        uint32_t Size() const
        {
            auto gil = ensure_gil();

            try
            {
                auto size = PySequence_Size(_sequence.get());
                if (size == -1 && PyErr_Occurred())
                {
                    throw python_exception();
                }

                return static_cast<uint32_t>(size);
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        auto First() const
        {
            auto gil = ensure_gil();

            try
            {
                return winrt::make<python_iterator<T>>(
                    PyObject_GetIter(_sequence.get()));
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
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

        static std::optional<KVPair> get_next(
            pyobj_handle const& mapping, pyobj_handle const& iterator)
        {
            pyobj_handle next_key{PyIter_Next(iterator.get())};

            if (!next_key)
            {
                if (PyErr_Occurred())
                {
                    throw python_exception();
                }

                return std::nullopt;
            }

            pyobj_handle next_value{PyObject_GetItem(mapping.get(), next_key.get())};

            if (!next_value)
            {
                throw python_exception();
            }

            auto key = converter<K>::convert_to(next_key.get());
            auto value = converter<V>::convert_to(next_value.get());

            return winrt::make<winrt::impl::key_value_pair<
                winrt::Windows::Foundation::Collections::IKeyValuePair<K, V>>>(
                key, value);
        }

        python_mapping_iterator(PyObject* mapping) : _mapping(mapping)
        {
            Py_INCREF(_mapping.get());

            _iterator = pyobj_handle{PyObject_GetIter(_mapping.get())};

            if (!_iterator)
            {
                throw python_exception();
            }

            _current_value = get_next(_mapping, _iterator);
        }

        KVPair Current() const
        {
            auto gil = ensure_gil();

            try
            {
                return _current_value.value();
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        bool HasCurrent() const
        {
            auto gil = ensure_gil();

            try
            {
                return _current_value.has_value();
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        bool MoveNext()
        {
            auto gil = ensure_gil();

            try
            {
                _current_value = get_next(_mapping, _iterator);
                return _current_value.has_value();
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        uint32_t GetMany(winrt::array_view<KVPair> /*unused*/) const
        {
            auto gil = ensure_gil();

            try
            {
                // TODO: implement GetMany
                PyErr_Format(
                    PyExc_NotImplementedError,
                    "py::python_mapping_iterator<%s, %s>::GetMany() is not implemented",
                    type_name<K>(),
                    type_name<V>());
                throw python_exception();
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
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

            try
            {
                pyobj_handle py_key{converter<K>::convert(key)};

                if (!py_key)
                {
                    throw python_exception();
                }

                auto ret = PyMapping_HasKeyWithError(_mapping.get(), py_key.get());

                if (ret == -1)
                {
                    throw python_exception();
                }

                return static_cast<bool>(ret);
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        V Lookup(K const& key) const
        {
            auto gil = ensure_gil();

            try
            {
                pyobj_handle py_key{converter<K>::convert(key)};

                if (!py_key)
                {
                    throw python_exception();
                }

                pyobj_handle item{PyObject_GetItem(_mapping.get(), py_key.get())};

                if (!item)
                {
                    if (PyErr_ExceptionMatches(PyExc_KeyError))
                    {
                        PyErr_Clear();
                        throw winrt::hresult_out_of_bounds();
                    }

                    throw python_exception();
                }

                return converter<V>::convert_to(item.get());
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        void Split(
            winrt::Windows::Foundation::Collections::IMapView<K, V>& first,
            winrt::Windows::Foundation::Collections::IMapView<K, V>& second) const
        {
            auto gil = ensure_gil();

            try
            {
                // null return indicates map cannot be split
                first = nullptr;
                second = nullptr;
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        uint32_t Size() const
        {
            auto gil = ensure_gil();

            try
            {
                auto size = PyMapping_Size(_mapping.get());

                if (size == -1 && PyErr_Occurred())
                {
                    throw python_exception();
                }

                return static_cast<uint32_t>(size);
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        winrt::Windows::Foundation::Collections::IIterator<
            winrt::Windows::Foundation::Collections::IKeyValuePair<K, V>>
        First() const
        {
            auto gil = ensure_gil();

            try
            {
                return winrt::make<python_mapping_iterator<K, V>>(_mapping.get());
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
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

            try
            {
                pyobj_handle result{
                    PyObject_CallMethod(_mapping.get(), "clear", nullptr)};

                if (!result)
                {
                    throw python_exception();
                }
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        winrt::Windows::Foundation::Collections::IMapView<K, V> GetView() const
        {
            auto gil = ensure_gil();

            try
            {
                return winrt::make<python_map_view<K, V>>(_mapping.get());
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        bool HasKey(K const& key) const
        {
            auto gil = ensure_gil();

            try
            {
                pyobj_handle py_key{converter<K>::convert(key)};

                if (!py_key)
                {
                    throw python_exception();
                }

                auto ret = PyMapping_HasKeyWithError(_mapping.get(), py_key.get());

                if (ret == -1)
                {
                    throw python_exception();
                }

                return static_cast<bool>(ret);
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        bool Insert(K const& key, V const& value) const
        {
            auto gil = ensure_gil();

            try
            {
                pyobj_handle py_key{converter<K>::convert(key)};

                if (!py_key)
                {
                    throw python_exception();
                }

                pyobj_handle py_value{converter<V>::convert(value)};

                if (!py_value)
                {
                    throw python_exception();
                }

                auto result = HasKey(key);

                if (PyObject_SetItem(_mapping.get(), py_key.get(), py_value.get())
                    == -1)
                {
                    throw python_exception();
                }

                return result;
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        V Lookup(K const& key) const
        {
            auto gil = ensure_gil();

            try
            {
                pyobj_handle py_key{converter<K>::convert(key)};

                if (!py_key)
                {
                    throw python_exception();
                }

                pyobj_handle item{PyObject_GetItem(_mapping.get(), py_key.get())};

                if (!item)
                {
                    if (PyErr_ExceptionMatches(PyExc_KeyError))
                    {
                        PyErr_Clear();
                        throw winrt::hresult_out_of_bounds();
                    }

                    throw python_exception();
                }

                return converter<V>::convert_to(item.get());
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        void Remove(K const& key) const
        {
            auto gil = ensure_gil();

            try
            {
                pyobj_handle py_key{converter<K>::convert(key)};

                if (!py_key)
                {
                    throw python_exception();
                }

                if (PyObject_DelItem(_mapping.get(), py_key.get()) == -1)
                {
                    if (PyErr_ExceptionMatches(PyExc_KeyError))
                    {
                        PyErr_Clear();
                        throw winrt::hresult_out_of_bounds();
                    }

                    throw python_exception();
                }
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        uint32_t Size() const
        {
            auto gil = ensure_gil();

            try
            {
                auto size = PyMapping_Size(_mapping.get());

                if (size == -1 && PyErr_Occurred())
                {
                    throw python_exception();
                }

                return static_cast<uint32_t>(size);
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
        }

        winrt::Windows::Foundation::Collections::IIterator<
            winrt::Windows::Foundation::Collections::IKeyValuePair<K, V>>
        First() const
        {
            auto gil = ensure_gil();

            try
            {
                return winrt::make<python_mapping_iterator<K, V>>(_mapping.get());
            }
            catch (python_exception)
            {
                write_unraisable_and_throw();
            }
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
