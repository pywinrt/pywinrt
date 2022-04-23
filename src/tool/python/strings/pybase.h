#pragma once

// These two headers are part of the Python C Extension API
#include <Python.h>
#include <structmember.h>

#include <winrt/Windows.Foundation.h>

namespace py
{
    template<typename T, typename = std::void_t<>>
    struct empty_instance
    {
        static T get()
        {
            return T{};
        }
    };

    template<typename T>
    struct empty_instance<T, std::void_t<decltype(T{nullptr})>>
    {
        static T get()
        {
            return T{nullptr};
        }
    };

    struct gil_state_traits
    {
        using type = PyGILState_STATE;

        static void close(type value) noexcept
        {
            PyGILState_Release(value);
        }

        static constexpr type invalid() noexcept
        {
            return static_cast<PyGILState_STATE>(0);
        }
    };

    template<typename Category>
    struct pinterface_checker
    {
        static constexpr bool value = false;
    };

    template<template<typename... TArgs> typename T, typename... Args>
    struct pinterface_checker<T<Args...>>
    {
        static constexpr bool value = true;
    };

    template<typename T>
    struct pinterface_python_type
    {
        using abstract = void;
        using concrete = void;
    };

    template<typename T>
    struct delegate_python_type
    {
        using type = void;
    };

    template<typename T>
    constexpr bool is_basic_category_v
        = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::basic_category>;

    template<typename T>
    constexpr bool is_class_category_v
        = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::class_category>;

    template<typename T>
    constexpr bool is_delegate_category_v
        = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::delegate_category>;

    template<typename T>
    constexpr bool is_pdelegate_category_v
        = !std::is_base_of_v<
              winrt::Windows::Foundation::IInspectable,
              T> && std::is_base_of_v<winrt::Windows::Foundation::IUnknown, T>;

    template<typename T>
    constexpr bool is_enum_category_v
        = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::enum_category>;

    template<typename T>
    constexpr bool is_interface_category_v
        = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::interface_category>;

    template<typename T>
    constexpr bool is_pinterface_category_v
        = std::is_base_of_v<winrt::Windows::Foundation::IInspectable, T>&&
            pinterface_checker<typename winrt::impl::category<T>::type>::value;

    struct delegate_callable
    {
        delegate_callable() noexcept = default;

        explicit delegate_callable(PyObject* callable) : _callable(callable)
        {
            Py_INCREF(_callable);
        }

        delegate_callable(delegate_callable&& other) noexcept
        {
            std::swap(_callable, other._callable);
        }

        ~delegate_callable()
        {
            winrt::handle_type<py::gil_state_traits> gil_state{PyGILState_Ensure()};
            Py_CLEAR(_callable);
        }

        PyObject* callable() const noexcept
        {
            return _callable;
        }

      private:
        PyObject* _callable{};
    };

    struct winrt_wrapper_base
    {
        PyObject_HEAD;

        // PyObject_New doesn't call type's constructor, so manually manage the
        // "virtual" get_unknown function
        winrt::Windows::Foundation::IUnknown const& (*get_unknown)(
            winrt_wrapper_base* self);
    };

    template<typename T>
    struct winrt_struct_wrapper
    {
        PyObject_HEAD;
        T obj{};
    };

    template<typename T>
    struct winrt_wrapper : winrt_wrapper_base
    {
        T obj{nullptr};

        static winrt::Windows::Foundation::IUnknown const& fetch_unknown(
            winrt_wrapper_base* self)
        {
            return reinterpret_cast<winrt_wrapper<T>*>(self)->obj;
        }
    };

    template<typename T>
    struct winrt_pinterface_wrapper : winrt_wrapper_base
    {
        std::unique_ptr<T> obj{nullptr};

        static winrt::Windows::Foundation::IUnknown const& fetch_unknown(
            winrt_wrapper_base* self)
        {
            return reinterpret_cast<winrt_pinterface_wrapper<T>*>(self)
                ->obj->get_unknown();
        }
    };

    template<typename To>
    To as(winrt_wrapper_base* wrapper)
    {
        return wrapper->get_unknown(wrapper).as<To>();
    }

    struct Object;

    /**
     * Type registration for pure Python types.
     */
    template<typename T>
    struct py_type
    {
        static PyObject* get_python_type()
        {
            return nullptr;
        }
    };

    /**
     * Gets the registered pure Python type for @p T.
     *
     * @returns A borrowed reference to the type or nullptr if the type was not
     * registered.
     */
    template<typename T>
    PyObject* get_py_type()
    {
        return py_type<T>::get_python_type();
    }

    /**
     * Type registration for binary extension types.
     */
    template<typename T>
    struct winrt_type
    {
        static PyTypeObject* get_python_type()
        {
            return nullptr;
        }
    };

    /**
     * Type registration for the base `_winrt.Object` Python type.
     */
    template<>
    struct winrt_type<Object>
    {
        static PyTypeObject* python_type;
    };

    /**
     * Gets the registered binary extension type for @p T.
     *
     * @returns A borrowed reference to the type or nullptr if the type was not
     * registered.
     */
    template<typename T>
    PyTypeObject* get_python_type()
    {
        if constexpr (is_pinterface_category_v<T>)
        {
            return winrt_type<pinterface_python_type<T>::abstract>::get_python_type();
        }
        else
        {
            return winrt_type<T>::get_python_type();
        }
    }

    struct pyobj_ptr_traits
    {
        using type = PyObject*;

        static void close(type value) noexcept
        {
            Py_CLEAR(value);
        }

        static constexpr type invalid() noexcept
        {
            return nullptr;
        }
    };

    using pyobj_handle = winrt::handle_type<pyobj_ptr_traits>;

    PyTypeObject* register_python_type(
        PyObject* module,
        const char* const type_name,
        PyType_Spec* type_spec,
        PyObject* base_type) noexcept;

    /**
     * Thrown when a Python exception is pending (i.e. PyErr_Occurred() returns
     * non-NULL).
     */
    struct python_exception
    {
    };

    inline __declspec(noinline) void set_invalid_activation_error(
        const char* const type_name)
    {
        std::string msg{type_name};
        msg.append(" is not activatable");
        PyErr_SetString(PyExc_TypeError, msg.c_str());
    }

    inline __declspec(noinline) void set_invalid_arg_count_error(
        Py_ssize_t arg_count) noexcept
    {
        if (arg_count != -1)
        {
            PyErr_SetString(PyExc_TypeError, "Invalid parameter count");
        }
    }

    inline __declspec(noinline) void set_invalid_kwd_args_error() noexcept
    {
        PyErr_SetString(PyExc_TypeError, "keyword arguments not supported");
    }

    inline __declspec(noinline) void to_PyErr() noexcept
    {
        if (PyErr_Occurred())
        {
            // A Python excpetion is already pending - don't overwrite it
            return;
        }

        // otherwise convert C++ exception to Python exception
        try
        {
            throw;
        }
        catch (winrt::hresult_error const& e)
        {
            PyErr_SetFromWindowsErr(e.code());
        }
        catch (std::bad_alloc const& e)
        {
            PyErr_SetString(PyExc_MemoryError, e.what());
        }
        catch (std::out_of_range const& e)
        {
            PyErr_SetString(PyExc_IndexError, e.what());
        }
        catch (std::invalid_argument const& e)
        {
            PyErr_SetString(PyExc_TypeError, e.what());
        }
        catch (std::exception const& e)
        {
            PyErr_SetString(PyExc_RuntimeError, e.what());
        }
    }

    void wrapped_instance(std::size_t key, PyObject* obj);
    PyObject* wrapped_instance(std::size_t key);

    template<typename T>
    inline auto get_instance_hash(T const& instance)
    {
        return std::hash<winrt::Windows::Foundation::IUnknown>{}(instance);
    }

    /**
     * Converts a Python integer object to a Python Enum object.
     *
     * This is the equivelent of calling `Enum(value)` in Python.
     *
     * @param value The value object.
     * @returns A new reference to the Enum object or nullptr on error.
     */
    template<typename T>
    PyObject* wrap_enum(PyObject* value) noexcept
    {
        PyObject* type_object = get_py_type<T>();

        if (!type_object)
        {
            PyErr_SetString(PyExc_RuntimeError, "enum type has not been registered");
            return nullptr;
        }

        pyobj_handle args{PyTuple_Pack(1, value)};

        if (!args)
        {
            return nullptr;
        }

        pyobj_handle obj{PyObject_Call(type_object, args.get(), nullptr)};

        if (!obj)
        {
            return nullptr;
        }

        return obj.detach();
    }

    template<typename T>
    PyObject* wrap_struct(T instance, PyTypeObject* type_object)
    {
        if (!type_object)
        {
            PyErr_SetNone(PyExc_NotImplementedError);
            return nullptr;
        }

        auto py_instance = PyObject_New(py::winrt_struct_wrapper<T>, type_object);

        if (!py_instance)
        {
            return nullptr;
        }

        // PyObject_New doesn't call type's constructor, so manually initialize the
        // wrapper's fields
        std::memset(&(py_instance->obj), 0, sizeof(py_instance->obj));
        py_instance->obj = instance;

        return reinterpret_cast<PyObject*>(py_instance);
    }

    template<typename T>
    PyObject* wrap(T instance, PyTypeObject* type_object)
    {
        if (!instance)
        {
            Py_RETURN_NONE;
        }

        if (!type_object)
        {
            PyErr_SetNone(PyExc_NotImplementedError);
            return nullptr;
        }

        auto py_instance = PyObject_New(py::winrt_wrapper<T>, type_object);

        if (!py_instance)
        {
            return nullptr;
        }

        // PyObject_New doesn't call type's constructor, so manually initialize the
        // wrapper's fields
        py_instance->get_unknown = &winrt_wrapper<T>::fetch_unknown;
        std::memset(&(py_instance->obj), 0, sizeof(py_instance->obj));
        py_instance->obj = instance;

        wrapped_instance(
            get_instance_hash(instance), reinterpret_cast<PyObject*>(py_instance));

        return reinterpret_cast<PyObject*>(py_instance);
    }

    template<typename T>
    PyObject* wrap_pinterface(T instance)
    {
        if (!instance)
        {
            Py_RETURN_NONE;
        }

        using ptype = pinterface_python_type<T>;

        PyTypeObject* type_object = get_python_type<T>();

        if (!type_object)
        {
            PyErr_SetNone(PyExc_NotImplementedError);
            return nullptr;
        }

        auto py_instance
            = PyObject_New(py::winrt_pinterface_wrapper<ptype::abstract>, type_object);

        if (!py_instance)
        {
            return nullptr;
        }

        // PyObject_New doesn't call type's constructor, so manually initialize the
        // wrapper's fields
        py_instance->get_unknown
            = &winrt_pinterface_wrapper<ptype::abstract>::fetch_unknown;
        std::memset(&(py_instance->obj), 0, sizeof(py_instance->obj));
        py_instance->obj = std::make_unique<ptype::concrete>(instance);

        wrapped_instance(
            get_instance_hash(instance), reinterpret_cast<PyObject*>(py_instance));

        return reinterpret_cast<PyObject*>(py_instance);
    }

    template<typename T>
    PyObject* wrap(T instance) noexcept
    {
        if (!instance)
        {
            Py_RETURN_NONE;
        }

        if constexpr (is_class_category_v<T> || is_interface_category_v<T>)
        {
            return wrap<T>(instance, get_python_type<T>());
        }
        else
        {
            if constexpr (std::is_same_v<pinterface_python_type<T>::abstract, void>)
            {
                PyErr_SetNone(PyExc_NotImplementedError);
                return nullptr;
            }
            else
            {
                return wrap_pinterface<T>(instance);
            }
        }
    }

    inline void throw_if_pyobj_null(PyObject* obj)
    {
        if (!obj)
        {
            if (!PyErr_Occurred())
            {
                PyErr_SetString(PyExc_SystemError, "object is null");
            }
            throw python_exception();
        }
    }

    template<typename T, typename = void>
    struct converter
    {
        static PyObject* convert(T value) noexcept
        {
            PyErr_SetNone(PyExc_NotImplementedError);
            return nullptr;
        }

        static T convert_to(PyObject* obj)
        {
            PyErr_SetNone(PyExc_NotImplementedError);
            throw python_exception();
        }
    };

    template<>
    struct converter<bool>
    {
        static PyObject* convert(bool value) noexcept
        {
            return PyBool_FromLong(value ? 1 : 0);
        }

        static bool convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            auto result = PyObject_IsTrue(obj);

            if (result == -1)
            {
                throw python_exception();
            }

            return result > 0;
        }
    };

    template<>
    struct converter<int8_t>
    {
        static PyObject* convert(int8_t value) noexcept
        {
            return PyLong_FromLong(static_cast<int32_t>(value));
        }

        static int8_t convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            int32_t result = PyLong_AsLong(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw python_exception();
            }

            if (result < INT8_MIN || result > INT8_MAX)
            {
                PyErr_SetString(PyExc_OverflowError, "does not fit in int8_t");
                throw python_exception();
            }

            return static_cast<int8_t>(result);
        }
    };

    template<>
    struct converter<uint8_t>
    {
        static PyObject* convert(uint8_t value) noexcept
        {
            return PyLong_FromLong(static_cast<int32_t>(value));
        }

        static uint8_t convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            int32_t result = PyLong_AsLong(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw python_exception();
            }

            if (result < 0 || result > UINT8_MAX)
            {
                PyErr_SetString(PyExc_OverflowError, "does not fit in uint8_t");
                throw python_exception();
            }

            return static_cast<uint8_t>(result);
        }
    };

    template<>
    struct converter<int16_t>
    {
        static PyObject* convert(int16_t value) noexcept
        {
            return PyLong_FromLong(static_cast<int32_t>(value));
        }

        static int16_t convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            int32_t result = PyLong_AsLong(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw python_exception();
            }

            if (result < INT16_MIN || result > INT16_MAX)
            {
                PyErr_SetString(PyExc_OverflowError, "does not fit in int16_t");
                throw python_exception();
            }

            return static_cast<int16_t>(result);
        }
    };

    template<>
    struct converter<uint16_t>
    {
        static PyObject* convert(uint16_t value) noexcept
        {
            return PyLong_FromLong(static_cast<int32_t>(value));
        }

        static uint16_t convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            int32_t result = PyLong_AsLong(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw python_exception();
            }

            if (result < 0 || result > UINT16_MAX)
            {
                PyErr_SetString(PyExc_OverflowError, "does not fit in uint16_t");
                throw python_exception();
            }

            return static_cast<uint16_t>(result);
        }
    };

    template<>
    struct converter<int32_t>
    {
        static PyObject* convert(int32_t value) noexcept
        {
            return PyLong_FromLong(value);
        }

        static int32_t convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            auto result = PyLong_AsLong(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw python_exception();
            }

            return result;
        }
    };

    template<>
    struct converter<uint32_t>
    {
        static PyObject* convert(uint32_t value) noexcept
        {
            return PyLong_FromUnsignedLong(value);
        }

        static uint32_t convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            auto result = PyLong_AsUnsignedLong(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw python_exception();
            }

            return result;
        }
    };

    template<>
    struct converter<int64_t>
    {
        static PyObject* convert(int64_t value) noexcept
        {
            return PyLong_FromLongLong(value);
        }

        static int64_t convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            auto result = PyLong_AsLongLong(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw python_exception();
            }

            return result;
        }
    };

    template<>
    struct converter<uint64_t>
    {
        static PyObject* convert(uint64_t value) noexcept
        {
            return PyLong_FromUnsignedLongLong(value);
        }

        static uint64_t convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            auto result = PyLong_AsUnsignedLongLong(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw python_exception();
            }

            return result;
        }
    };

    template<>
    struct converter<float>
    {
        static PyObject* convert(float value) noexcept
        {
            return PyFloat_FromDouble(value);
        }

        static float convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            auto result = PyFloat_AsDouble(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw python_exception();
            }

            return static_cast<float>(result);
        }
    };

    template<>
    struct converter<double>
    {
        static PyObject* convert(double value) noexcept
        {
            return PyFloat_FromDouble(value);
        }

        static double convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            auto result = PyFloat_AsDouble(obj);

            if (result == -1 && PyErr_Occurred())
            {
                throw python_exception();
            }

            return result;
        }
    };

    template<>
    struct converter<winrt::guid>
    {
        static PyObject* convert(winrt::guid value) noexcept
        {
            pyobj_handle valueAsBytes{
                PyBytes_FromStringAndSize((char*)&value, sizeof(value))};
            if (!valueAsBytes)
            {
                return nullptr;
            }
            pyobj_handle uuidModule{PyImport_ImportModule("uuid")};
            if (!uuidModule)
            {
                return nullptr;
            }
            pyobj_handle uuidClass{PyObject_GetAttrString(uuidModule.get(), "UUID")};
            if (!uuidClass)
            {
                return nullptr;
            }
            pyobj_handle args{PyTuple_New(0)};
            if (!args)
            {
                return nullptr;
            }
            pyobj_handle kwargs{PyDict_New()};
            if (!kwargs)
            {
                return nullptr;
            }

            auto result
                = PyDict_SetItemString(kwargs.get(), "bytes_le", valueAsBytes.get());
            if (result == -1)
            {
                return nullptr;
            }

            return PyObject_Call(uuidClass.get(), args.get(), kwargs.get());
        }

        static winrt::guid convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            pyobj_handle bytes{PyObject_GetAttrString(obj, "bytes_le")};
            if (!bytes)
            {
                throw python_exception();
            }

            winrt::guid result;
            char* buffer;
            Py_ssize_t size;
            if (PyBytes_AsStringAndSize(bytes.get(), &buffer, &size) == -1)
            {
                throw python_exception();
            }

            if (size != sizeof(result))
            {
                PyErr_SetString(PyExc_ValueError, "bytes_le is wrong size");
                throw python_exception();
            }

            memcpy(&result, buffer, size);

            return result;
        }
    };

    template<>
    struct converter<winrt::Windows::Foundation::IInspectable>
    {
        static PyObject* convert(
            winrt::Windows::Foundation::IInspectable const& value) noexcept
        {
            return wrap<winrt::Windows::Foundation::IInspectable>(
                value, winrt_type<Object>::python_type);
        }

        static winrt::Windows::Foundation::IInspectable convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            int result = PyObject_IsInstance(
                obj, reinterpret_cast<PyObject*>(winrt_type<Object>::python_type));

            if (result == -1)
            {
                throw python_exception();
            }

            if (result == 0)
            {
                PyErr_SetString(PyExc_TypeError, "not a _winrt.Object");
                throw python_exception();
            }

            auto wrapper = reinterpret_cast<winrt_wrapper_base*>(obj);
            return as<winrt::Windows::Foundation::IInspectable>(wrapper);
        }
    };

    struct pystring
    {
        wchar_t* buffer{nullptr};
        std::wstring_view::size_type size{};

        explicit pystring(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            Py_ssize_t py_size;
            buffer = PyUnicode_AsWideCharString(obj, &py_size);

            if (!buffer)
            {
                throw python_exception();
            }

            size = static_cast<std::wstring_view::size_type>(py_size);
        }

        pystring(pystring& other) = delete;
        pystring& operator=(pystring const&) = delete;

        pystring(pystring&& other) noexcept : buffer(other.buffer), size(other.size)
        {
            other.buffer = nullptr;
        }

        pystring& operator=(pystring&& rhs)
        {
            std::swap(buffer, rhs.buffer);
            size = rhs.size;
        }

        operator bool() const noexcept
        {
            return buffer != nullptr;
        }

        ~pystring()
        {
            PyMem_Free(buffer);
        }
    };

    struct pystringview : public pystring, public std::wstring_view
    {
        explicit pystringview(PyObject* obj)
            : pystring(obj), std::wstring_view(pystring::buffer, pystring::size)
        {
        }
    };

    template<>
    struct converter<winrt::hstring>
    {
        static PyObject* convert(winrt::hstring const& value) noexcept
        {
            return PyUnicode_FromWideChar(value.c_str(), value.size());
        }

        static pystringview convert_to(PyObject* obj)
        {
            pystringview str{obj};

            return str;
        }
    };

    template<typename T>
    struct converter<T, typename std::enable_if_t<is_enum_category_v<T>>>
    {
        static PyObject* convert(T instance) noexcept
        {
            using enum_type = std::underlying_type_t<T>;
            pyobj_handle value{
                converter<enum_type>::convert(static_cast<enum_type>(instance))};
            return wrap_enum<T>(value.get());
        }

        static auto convert_to(PyObject* obj)
        {
            using enum_type = std::underlying_type_t<T>;
            throw_if_pyobj_null(obj);
            return static_cast<T>(converter<enum_type>::convert_to(obj));
        }
    };

    template<typename T>
    struct converter<T, typename std::enable_if_t<is_class_category_v<T>>>
    {
        static PyObject* convert(T const& instance) noexcept
        {
            return wrap(instance);
        }

        static auto convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            if (Py_TYPE(obj) != get_python_type<T>())
            {
                PyErr_SetString(PyExc_TypeError, "wrong type");
                throw python_exception();
            }

            return reinterpret_cast<winrt_wrapper<T>*>(obj)->obj;
        }
    };

    template<typename T>
    struct python_iterable : winrt::implements<
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
            return winrt::make<iterator>(PyObject_GetIter(_iterable.get()));
        }

      private:
        struct iterator : winrt::implements<
                              iterator,
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
                        return {};
                    }
                }

                return std::move(
                    std::optional<T>{converter<T>::convert_to(next.get())});
            }

            iterator(PyObject* i) : _iterator(i)
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
                return _current_value.value();
            }

            bool HasCurrent() const
            {
                return _current_value.has_value();
            }

            bool MoveNext()
            {
                _current_value = get_next(_iterator);
                return _current_value.has_value();
            }

            uint32_t GetMany(winrt::array_view<T> values)
            {
                // TODO: implement GetMany
                PyErr_SetNone(PyExc_NotImplementedError);
                throw python_exception();
            }
        };
    };

    template<typename T>
    std::optional<T> convert_interface_to(PyObject* obj)
    {
        throw_if_pyobj_null(obj);

        if (Py_TYPE(obj) == get_python_type<T>())
        {
            return reinterpret_cast<winrt_wrapper<T>*>(obj)->obj;
        }

        if (PyObject_IsInstance(
                obj, reinterpret_cast<PyObject*>(winrt_type<Object>::python_type)))
        {
            auto wrapper = reinterpret_cast<winrt_wrapper_base*>(obj);
            return as<T>(wrapper);
        }

        return {};
    }

    // TODO: specialization for Python Sequence Protocol -> IVector[View]
    // TODO: specialization for Python Mapping Protocol -> IMap[View]

    template<typename TItem>
    struct converter<winrt::Windows::Foundation::Collections::IIterable<TItem>>
    {
        using TCollection = winrt::Windows::Foundation::Collections::IIterable<TItem>;

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

            pyobj_handle iterator{PyObject_GetIter(obj)};

            if (!iterator)
            {
                throw python_exception();
            }

            return winrt::make<python_iterable<TItem>>(obj);
        }
    };

    template<typename T>
    struct is_specalized_interface : std::false_type
    {
    };

    template<typename T>
    inline constexpr bool is_specalized_interface_v = is_specalized_interface<T>::value;

    template<typename TItem>
    struct is_specalized_interface<
        winrt::Windows::Foundation::Collections::IIterable<TItem>> : std::true_type
    {
    };

    template<typename T>
    struct is_specalized_interface<winrt::Windows::Foundation::IReference<T>>
        : std::true_type
    {
    };

    template<typename T>
    struct converter<
        T,
        typename std::enable_if_t<(
            is_interface_category_v<
                T> || is_pinterface_category_v<T>)&&!is_specalized_interface_v<T>>>
    {
        static PyObject* convert(T const& instance) noexcept
        {
            return wrap(instance);
        }

        static auto convert_to(PyObject* obj)
        {
            auto result = convert_interface_to<T>(obj);

            if (!result)
            {
                if (!PyErr_Occurred())
                {
                    PyErr_SetString(PyExc_TypeError, "convert_to returned null");
                }

                throw python_exception();
            }

            return result.value();
        }
    };

    template<typename T>
    struct converter<
        T,
        typename std::enable_if_t<
            is_delegate_category_v<T> || is_pdelegate_category_v<T>>>
    {
        static PyObject* convert(T const& instance) noexcept
        {
            // TODO: support converting delegates
            PyErr_SetNone(PyExc_NotImplementedError);
            return nullptr;
        }

        static auto convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);
            return delegate_python_type<T>::type::get(obj);
        }
    };

    /**
     * A wrapper around the Python buffer protocol that implements winrt::array_view.
     */
    template<typename T>
    struct pybuf_view : winrt::array_view<T>
    {
        using typename winrt::array_view<T>::value_type;
        using typename winrt::array_view<T>::pointer;

        pybuf_view(pybuf_view const&) = delete;
        pybuf_view& operator=(pybuf_view const&) = delete;

        pybuf_view() = delete;

        pybuf_view(PyObject* obj)
        {
            // this is assuming array_view is always treated as read-only
            if (PyObject_GetBuffer(obj, &view, PyBUF_SIMPLE) == -1)
            {
                throw python_exception();
            }

            // we don't allow data with the wrong item size
            if (view.itemsize != sizeof(value_type))
            {
                pyobj_handle msg{PyUnicode_FromFormat(
                    "itemsize is incorrect, expecting %zu but was %zi",
                    sizeof(value_type),
                    view.itemsize)};
                PyErr_SetObject(PyExc_TypeError, msg.get());
                PyBuffer_Release(&view);

                throw python_exception();
            }

            this->m_data = reinterpret_cast<pointer>(view.buf);
            this->m_size = view.len / view.itemsize;
        }

        ~pybuf_view()
        {
            PyBuffer_Release(&view);
        }

      private:
        Py_buffer view;
    };

    template<typename T>
    struct converter<winrt::array_view<T>>
    {
        static PyObject* convert(winrt::array_view<T> const& instance) noexcept
        {
            PyErr_SetNone(PyExc_NotImplementedError);
            return nullptr;
        }

        static winrt::array_view<T> convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            // For fundamental C++ types, use buffer protocol, i.e. bytearray,
            // array, etc. are used without copying data.
            if (std::is_fundamental_v<T> && PyObject_CheckBuffer(obj))
            {
                return std::move(pybuf_view<T>{obj});
            }

            // Other types require marshalling from Python to COM array.
            return converter<winrt::com_array<T>>::convert_to(obj);
        }
    };

    template<typename T>
    struct converter<winrt::com_array<T>>
    {
        static PyObject* convert(winrt::com_array<T> const& instance) noexcept
        {
            pyobj_handle list{PyList_New(instance.size())};
            if (!list)
            {
                return nullptr;
            }

            for (uint32_t index = 0; index < instance.size(); index++)
            {
                pyobj_handle item{converter<T>::convert(instance[index])};
                if (!item)
                {
                    return nullptr;
                }

                if (PyList_SetItem(list.get(), index, item.get()) == -1)
                {
                    return nullptr;
                }

                // PyList_SetItem steals the reference to item
                item.detach();
            }

            return list.detach();
        }

        static auto convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            Py_ssize_t list_size = PySequence_Size(obj);

            if (list_size == -1)
            {
                throw python_exception();
            }

            winrt::com_array<T> items(
                static_cast<uint32_t>(list_size), empty_instance<T>::get());

            for (Py_ssize_t index = 0; index < list_size; index++)
            {
                pyobj_handle item{PySequence_GetItem(obj, index)};

                if (!item)
                {
                    throw python_exception();
                }

                items[static_cast<uint32_t>(index)]
                    = converter<T>::convert_to(item.get());
            }

            return std::move(items);
        }
    };

    template<typename T>
    struct converter<winrt::Windows::Foundation::IReference<T>>
    {
        static PyObject* convert(
            winrt::Windows::Foundation::IReference<T> const& reference) noexcept
        {
            if (reference == nullptr)
            {
                Py_RETURN_NONE;
            }

            return converter<T>::convert(reference.Value());
        }

        static winrt::Windows::Foundation::IReference<T> convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            if (obj == Py_None)
            {
                return nullptr;
            }

            return converter<T>::convert_to(obj);
        }
    };

    template<typename T>
    PyObject* convert(T const& instance)
    {
        return converter<T>::convert(instance);
    }

    template<typename T>
    auto convert_to(PyObject* value)
    {
        return converter<T>::convert_to(value);
    }

    template<typename T>
    auto convert_to(PyObject* args, int index)
    {
        return convert_to<T>(PyTuple_GetItem(args, index));
    }

    template<typename Async>
    PyObject* get_results(Async const& operation) noexcept
    {
        try
        {
            if constexpr (std::is_void_v<decltype(operation.GetResults())>)
            {
                operation.GetResults();
                Py_RETURN_NONE;
            }
            else
            {
                return convert(operation.GetResults());
            }
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    template<typename Async>
    PyObject* get_error(Async const& operation) noexcept
    {
        try
        {
            // Set error to get same behavior as non-async
            PyErr_SetFromWindowsErr(operation.ErrorCode());
            pyobj_handle type, value, trace;
            PyErr_Fetch(type.put(), value.put(), trace.put());
            return value.detach();
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    struct completion_callback
    {
        completion_callback() noexcept = default;

        explicit completion_callback(pyobj_handle& loop, pyobj_handle& future)
            : _loop(loop.detach()), _future(future.detach())
        {
        }

        completion_callback(completion_callback&& other) noexcept
        {
            std::swap(_loop, other._loop);
            std::swap(_future, other._future);
        }

        ~completion_callback()
        {
            winrt::handle_type<py::gil_state_traits> gil_state{PyGILState_Ensure()};
            Py_CLEAR(_loop);
            Py_CLEAR(_future);
        }

        PyObject* loop() const noexcept
        {
            return _loop;
        }

        PyObject* future() const noexcept
        {
            return _future;
        }

        PyObject* future_type() const noexcept
        {
            return reinterpret_cast<PyObject*>(Py_TYPE(_future));
        }

      private:
        PyObject* _loop{};
        PyObject* _future{};
    };

    template<typename Async>
    PyObject* dunder_await(Async const& async) noexcept
    {
        pyobj_handle asyncio{PyImport_ImportModule("asyncio")};
        if (!asyncio)
        {
            return nullptr;
        }

        pyobj_handle loop{
            PyObject_CallMethod(asyncio.get(), "get_running_loop", nullptr)};
        if (!loop)
        {
            return nullptr;
        }

        pyobj_handle future{PyObject_CallMethod(loop.get(), "create_future", nullptr)};
        if (!future)
        {
            return nullptr;
        }

        // make a copy of future to pass into completed lambda
        pyobj_handle future_copy{future.get()};
        Py_INCREF(future_copy.get());

        completion_callback cb{loop, future_copy};

        try
        {
            async.Completed(
                [cb = std::move(cb)](
                    Async const& operation,
                    winrt::Windows::Foundation::AsyncStatus status) mutable
                {
                    winrt::handle_type<py::gil_state_traits> gil_state{
                        PyGILState_Ensure()};

                    if (status == winrt::Windows::Foundation::AsyncStatus::Completed)
                    {
                        pyobj_handle results{get_results(operation)};

                        pyobj_handle set_result{
                            PyObject_GetAttrString(cb.future_type(), "set_result")};
                        pyobj_handle handle{PyObject_CallMethod(
                            cb.loop(),
                            "call_soon_threadsafe",
                            "OOO",
                            set_result.get(),
                            cb.future(),
                            results.get())};
                    }
                    else if (
                        status == winrt::Windows::Foundation::AsyncStatus::Canceled)
                    {
                        pyobj_handle cancel{
                            PyObject_GetAttrString(cb.future_type(), "cancel")};
                        pyobj_handle handle{PyObject_CallMethod(
                            cb.loop(),
                            "call_soon_threadsafe",
                            "OO",
                            cancel.get(),
                            cb.future())};
                    }
                    else
                    {
                        pyobj_handle exc{get_error(operation)};
                        pyobj_handle set_exception{
                            PyObject_GetAttrString(cb.future_type(), "set_exception")};
                        pyobj_handle handle{PyObject_CallMethod(
                            cb.loop(),
                            "call_soon_threadsafe",
                            "OOO",
                            set_exception.get(),
                            cb.future(),
                            exc.get())};
                    }
                });
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }

        return PyObject_GetIter(future.get());
    }

    template<>
    struct converter<winrt::event_token>
    {
        static PyObject* convert(winrt::event_token instance) noexcept;
        static winrt::event_token convert_to(PyObject* obj);
    };
} // namespace py
