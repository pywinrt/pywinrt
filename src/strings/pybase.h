#pragma once

// These two headers are part of the Python C Extension API
#include <Python.h>
#include <datetime.h>
#include <structmember.h>

#include <windows.h>
#undef GetCurrentTime // breaks winrt API of the same name

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Metadata.h>

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
        = !std::is_base_of_v<winrt::Windows::Foundation::IInspectable, T>
          && std::is_base_of_v<winrt::Windows::Foundation::IUnknown, T>;

    template<typename T>
    constexpr bool is_enum_category_v
        = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::enum_category>;

    template<typename T>
    constexpr bool is_interface_category_v
        = std::is_same_v<winrt::impl::category_t<T>, winrt::impl::interface_category>;

    template<typename T>
    constexpr bool is_pinterface_category_v
        = std::is_base_of_v<winrt::Windows::Foundation::IInspectable, T>
          && pinterface_checker<typename winrt::impl::category<T>::type>::value;

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
        return wrapper->get_unknown(wrapper).template as<To>();
    }

    /**
     * Type registration for binary extension types.
     */
    template<typename T>
    struct winrt_type
    {
        static PyTypeObject* get_python_type() noexcept
        {
            PyErr_Format(
                PyExc_NotImplementedError,
                "py::winrt_type<%s>::get_python_type() is not implemented",
                typeid(T).name());
            return nullptr;
        }
    };

    /**
     * Empty type for getting `_winrt.Object` Python type via
     * `get_python_type<py::Object>()`.
     */
    struct Object
    {
    };

    /**
     * Type registration for the base `_winrt.Object` Python type.
     */
    template<>
    struct winrt_type<Object>
    {
        static PyTypeObject* get_python_type() noexcept;
    };

    /**
     * Type for getting `_winrt.Array` Python type via
     * `get_python_type<py::Array>()`.
     */
    struct Array
    {
        /**
         * Allocates a new array.
         * @param [in]  size    The number of elements in the array.
         * @returns @c true on success, otherwise sets Python error and returns
         * @c false.
         */
        virtual bool Alloc(uint32_t size) noexcept = 0;

        /**
         * Gets the WinRT name of the element type of the array.
         */
        virtual std::wstring_view WinrtElementTypeName() noexcept = 0;

        /**
         * Gets the Py_buffer format string for this array type.
         *
         * The format string must be compatible with the struct module.
         */
        virtual std::string_view Format() noexcept = 0;

        /**
         * Gets the number of elements in the array.
         */
        virtual uint32_t Size() noexcept = 0;

        /**
         * Gets the size of a single element in bytes for this array type.
         */
        virtual size_t ValueSize() noexcept = 0;

        /**
         * Gets a pointer to the array buffer.
         */
        virtual void* Data() noexcept = 0;

        /**
         * Gets the item a @p index and converts it to a Python object.
         * @param [in]  index   The index of the item in the array.
         * @returns A new reference to a Python object or sets Python error and returns
         * @c nullptr on failure.
         */
        virtual PyObject* At(uint32_t index) noexcept = 0;

        /**
         * Converts @p item to a WinRT object and stores it in the array.
         * @param [in]  index   The index of the item in the array.
         * @param [in]  item    The Python object to convert and store.
         * @returns @c true on success, otherwise sets Python error and returns
         * @c false.
         */
        virtual bool Set(Py_ssize_t index, PyObject* item) noexcept = 0;

        // needed to avoid leaks with derived types when used with std::unique_ptr
        virtual ~Array() = default;
    };

    /**
     * Type registration for the base `_winrt.Array` Python type.
     */
    template<>
    struct winrt_type<Array>
    {
        static PyTypeObject* get_python_type() noexcept;
    };

    namespace cpp::_winrt
    {
        PyObject* Array_New(std::unique_ptr<py::Array> array) noexcept;
        bool Array_Assign(PyObject* obj, std::unique_ptr<py::Array> array) noexcept;
    } // namespace cpp::_winrt

    /**
     * A type that always evaluates to false.
     *
     * This is useful for static_assert() in template functions to catch
     * unsupported types.
     */
    template<typename T>
    struct false_type : std::false_type
    {
    };

    /**
     * Empty type for getting `_winrt.MappingIter` Python type via
     * `get_python_type<py::MappingIter>()`.
     */
    struct MappingIter
    {
    };

    /**
     * Type registration for the base `_winrt.MappingIter` Python type.
     */
    template<>
    struct winrt_type<MappingIter>
    {
        static PyTypeObject* get_python_type() noexcept;
    };

    /**
     * Type registration for pure Python types.
     */
    template<typename T>
    struct py_type
    {
        static PyObject* get_python_type() noexcept
        {
            PyErr_Format(
                PyExc_NotImplementedError,
                "py::py_type<%s>::get_python_type() is not implemented",
                typeid(T).name());
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
    PyObject* get_py_type() noexcept
    {
        return py_type<T>::get_python_type();
    }

    /**
     * Gets the registered binary extension type for @p T.
     *
     * @returns A borrowed reference to the type or nullptr if the type was not
     * registered.
     */
    template<typename T>
    PyTypeObject* get_python_type() noexcept
    {
        if constexpr (is_pinterface_category_v<T>)
        {
            return winrt_type<
                typename pinterface_python_type<T>::abstract>::get_python_type();
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

    // BEGIN: methods defined in runtime.cpp

    PyTypeObject* register_python_type(
        PyObject* module,
        const char* const type_name,
        PyType_Spec* type_spec,
        PyObject* base_type,
        PyTypeObject* metaclass) noexcept;

    PyObject* wrap_mapping_iter(PyObject* iter) noexcept;

    // END: methods defined in runtime.cpp

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
        PyErr_Format(PyExc_TypeError, "type '%s' is not activatable", type_name);
    }

    inline __declspec(noinline) void set_arg_count_version_error(
        Py_ssize_t arg_count) noexcept
    {
        PyErr_Format(
            PyExc_AttributeError,
            "method overload with %zd arg(s) is not available in this version of Windows",
            arg_count);
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
            PyErr_Format(
                PyExc_NotImplementedError,
                "py::wrap_struct(%s instance, PyTypeObject* type_object == nullptr) is not implemented",
                typeid(T).name());
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

#if PY_VERSION_HEX < 0x03080000
        Py_INCREF(type_object);
#endif

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
            PyErr_Format(
                PyExc_NotImplementedError,
                "py::wrap(%s instance, PyTypeObject* type_object == nullptr) is not implemented",
                typeid(T).name());
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

#if PY_VERSION_HEX < 0x03080000
        Py_INCREF(type_object);
#endif

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

        auto type = get_python_type<T>();

        if (!type)
        {
            return nullptr;
        }

        auto py_instance = PyObject_New(
            py::winrt_pinterface_wrapper<typename ptype::abstract>, type);

        if (!py_instance)
        {
            return nullptr;
        }

        // PyObject_New doesn't call type's constructor, so manually initialize the
        // wrapper's fields
        py_instance->get_unknown
            = &winrt_pinterface_wrapper<typename ptype::abstract>::fetch_unknown;
        std::memset(&(py_instance->obj), 0, sizeof(py_instance->obj));
        py_instance->obj = std::make_unique<typename ptype::concrete>(instance);

#if PY_VERSION_HEX < 0x03080000
        Py_INCREF(type);
#endif

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
            auto type = get_python_type<T>();

            if (!type)
            {
                return nullptr;
            }

            return wrap<T>(instance, type);
        }
        else
        {
            if constexpr (std::is_same_v<
                              typename pinterface_python_type<T>::abstract,
                              void>)
            {
                PyErr_Format(
                    PyExc_NotImplementedError,
                    "py::wrap(%s instance) is not implemented",
                    typeid(T).name());
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

    /**
     * Tests if @p view is compatible with @p itemsize and @p format
     * @param [in]  view        A Python buffer view.
     * @param [in]  itemsize    The required element size.
     * @param [in]  format      The required element format.
     * @returns @c true if the buffer is valid, otherwise sets Python error and returns
     * @c false
     */
    static bool is_buffer_compatible(
        Py_buffer const& view, Py_ssize_t itemsize, const char* format) noexcept
    {
        if (view.itemsize != itemsize)
        {
            PyErr_Format(
                PyExc_BufferError,
                "requires buffer with itemsize == %zd, have %zd",
                itemsize,
                view.ndim);
            return false;
        }

        assert(format);

        if (!view.format || std::strcmp(view.format, format))
        {
            PyErr_Format(
                PyExc_BufferError,
                "requires buffer with format == \"%s\", have \"%s\"",
                format,
                view.format);
            return false;
        }

        if (view.ndim != 1)
        {
            PyErr_Format(
                PyExc_BufferError,
                "requires buffer with ndim == 1, have %d",
                view.ndim);
            return false;
        }

        if (!view.strides || view.strides[0] != itemsize)
        {
            PyErr_Format(
                PyExc_BufferError,
                "requires buffer with strides[0] == %zd, have %zd",
                view.strides ? view.strides[0] : 0,
                itemsize);
            return false;
        }

        return true;
    }

    template<typename T, typename = void>
    struct buffer
    {
        static bool is_compatible(Py_buffer const& view) noexcept
        {
            PyErr_Format(
                PyExc_NotImplementedError,
                "py::buffer<%s>::is_compatible() is not implemented",
                typeid(T).name());
            return false;
        }
    };

    // PEP 3118 struct formats

    template<typename T, typename = void>
    inline constexpr const char* buffer_format = 0;

    template<>
    inline constexpr const char* buffer_format<bool> = "?";

    template<>
    inline constexpr const char* buffer_format<int8_t> = "b";

    template<>
    inline constexpr const char* buffer_format<uint8_t> = "B";

    template<>
    inline constexpr const char* buffer_format<int16_t> = "h";

    template<>
    inline constexpr const char* buffer_format<uint16_t> = "H";

    template<>
    inline constexpr const char* buffer_format<int32_t> = "i";

    template<>
    inline constexpr const char* buffer_format<uint32_t> = "I";

    template<>
    inline constexpr const char* buffer_format<int64_t> = "q";

    template<>
    inline constexpr const char* buffer_format<uint64_t> = "Q";

    template<>
    inline constexpr const char* buffer_format<float> = "f";

    template<>
    inline constexpr const char* buffer_format<double> = "d";

    template<>
    inline constexpr const char* buffer_format<char16_t> = "u";

    template<>
    inline constexpr const char* buffer_format<winrt::guid> = "T{I2H8B}";

    template<>
    inline constexpr const char* buffer_format<winrt::hstring> = "P";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::IInspectable> = "P";

    template<typename T>
    inline constexpr const char* buffer_format<T, std::enable_if_t<is_class_category_v<T>>>
        = "P";

    template<typename T>
    inline constexpr const char* buffer_format<T, std::enable_if_t<is_interface_category_v<T>>>
        = "P";

    template<typename T>
    constexpr const char*
        buffer_format<T, std::enable_if_t<is_pinterface_category_v<T>>>
        = "P";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::DateTime> = "q";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::TimeSpan> = "q";

    template<typename T>
    struct buffer<T>
    {
        static bool is_compatible(Py_buffer const& view) noexcept
        {
            static_assert(buffer_format<T>);
            return is_buffer_compatible(view, sizeof(T), buffer_format<T>);
        }
    };

    template<typename T, typename = void>
    struct converter
    {
        static PyObject* convert(T value) noexcept
        {
            PyErr_Format(
                PyExc_NotImplementedError,
                "py::converter<%s>::convert() is not implemented",
                typeid(T).name());
            return nullptr;
        }

        static T convert_to(PyObject* obj)
        {
            PyErr_Format(
                PyExc_NotImplementedError,
                "py::converter<%s>::convert_to() is not implemented",
                typeid(T).name());
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

    /**
     * winrt::Windows::Foundation::DateTime is a specialized type that is an
     * alias for std::chrono::time_point in C++/WinRT and is converted to/from
     * datetime.datetime in Python.
     */
    template<>
    struct converter<winrt::Windows::Foundation::DateTime>
    {
        static PyObject* convert(winrt::Windows::Foundation::DateTime value) noexcept
        {
            try
            {
                FILETIME ft = winrt::clock::to_FILETIME(value);
                SYSTEMTIME st;

                if (!FileTimeToSystemTime(&ft, &st))
                {
                    winrt::throw_last_error();
                }

                auto microseconds
                    = std::chrono::time_point_cast<std::chrono::microseconds>(value)
                          .time_since_epoch()
                          .count()
                      % 1000;

                // FIXME: where can we put this so it only imports once?
                PyDateTime_IMPORT;

                // new reference
                return PyDateTimeAPI->DateTime_FromDateAndTime(
                    st.wYear,
                    st.wMonth,
                    st.wDay,
                    st.wHour,
                    st.wMinute,
                    st.wSecond,
                    st.wMilliseconds * 1000 + microseconds,
                    PyDateTime_TimeZone_UTC,
                    PyDateTimeAPI->DateTimeType);
            }
            catch (...)
            {
                py::to_PyErr();
                return nullptr;
            }
        }

        static winrt::Windows::Foundation::DateTime convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            // FIXME: where can we put this so it only imports once?
            PyDateTime_IMPORT;

            if (!PyDateTime_Check(obj))
            {
                PyErr_SetString(PyExc_TypeError, "requires datetime.datetime object");
                throw python_exception();
            }

            // WinRT works in UTC, so ensure correct time zone. Also works
            // for "naive" datetime.

            // new reference
            pyobj_handle utc{
                PyObject_CallMethod(obj, "astimezone", "O", PyDateTime_TimeZone_UTC)};
            if (!utc)
            {
                throw python_exception();
            }

            SYSTEMTIME st;
            st.wYear = PyDateTime_GET_YEAR(utc.get());
            st.wMonth = PyDateTime_GET_MONTH(utc.get());
            st.wDay = PyDateTime_GET_DAY(utc.get());
            st.wHour = PyDateTime_DATE_GET_HOUR(utc.get());
            st.wMinute = PyDateTime_DATE_GET_MINUTE(utc.get());
            st.wSecond = PyDateTime_DATE_GET_SECOND(utc.get());
            st.wMilliseconds = PyDateTime_DATE_GET_MICROSECOND(utc.get()) / 1000;

            FILETIME ft;

            if (!SystemTimeToFileTime(&st, &ft))
            {
                winrt::throw_last_error();
            }

            auto value = winrt::clock::from_FILETIME(ft);

            auto microseconds = PyDateTime_DATE_GET_MICROSECOND(utc.get()) % 1000;

            value += std::chrono::duration_cast<
                winrt::Windows::Foundation::DateTime::duration>(
                std::chrono::microseconds{microseconds});

            return value;
        }
    };

    /**
     * winrt::Windows::Foundation::TimeSpan is a specialized type that is an
     * alias for std::chrono::duration in C++/WinRT and is converted to/from
     * datetime.timedelta in Python.
     */
    template<>
    struct converter<winrt::Windows::Foundation::TimeSpan>
    {
        static PyObject* convert(winrt::Windows::Foundation::TimeSpan value) noexcept
        {
            try
            {
                // FIXME: where can we put this so it only imports once?
                PyDateTime_IMPORT;

                // REVISIT: the commented code can be used in c++20
                // auto days = std::chrono::duration_cast<std::chrono::days>(value);
                auto days = std::chrono::duration_cast<
                    std::chrono::duration<int64_t, std::ratio<86400>>>(value);
                auto seconds
                    = std::chrono::duration_cast<std::chrono::seconds>(value - days);
                auto microseconds
                    = std::chrono::duration_cast<std::chrono::microseconds>(
                        value - seconds - days);

                return PyDelta_FromDSU(
                    static_cast<int>(days.count()),
                    static_cast<int>(seconds.count()),
                    static_cast<int>(microseconds.count()));
            }
            catch (...)
            {
                py::to_PyErr();
                return nullptr;
            }
        }

        static winrt::Windows::Foundation::TimeSpan convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            // FIXME: where can we put this so it only imports once?
            PyDateTime_IMPORT;

            if (!PyDelta_Check(obj))
            {
                PyErr_SetString(PyExc_TypeError, "requires datetime.timedelta object");
                throw python_exception();
            }

            return std::chrono::duration_cast<winrt::Windows::Foundation::TimeSpan>(
                // REVISIT: the commented code can be used in c++20
                // std::chrono::days(PyDateTime_DELTA_GET_DAYS(obj))
                std::chrono::duration<int64_t, std::ratio<86400>>(
                    PyDateTime_DELTA_GET_DAYS(obj))
                + std::chrono::seconds(PyDateTime_DELTA_GET_SECONDS(obj))
                + std::chrono::microseconds(PyDateTime_DELTA_GET_MICROSECONDS(obj)));
        }
    };

    template<>
    struct converter<winrt::Windows::Foundation::IInspectable>
    {
        static PyObject* convert(
            winrt::Windows::Foundation::IInspectable const& value) noexcept
        {
            auto object_type = get_python_type<Object>();

            if (!object_type)
            {
                return nullptr;
            }

            return wrap<winrt::Windows::Foundation::IInspectable>(value, object_type);
        }

        static winrt::Windows::Foundation::IInspectable convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            auto object_type = get_python_type<Object>();

            if (!object_type)
            {
                throw python_exception();
            }

            auto result
                = PyObject_IsInstance(obj, reinterpret_cast<PyObject*>(object_type));

            if (result == -1)
            {
                throw python_exception();
            }

            if (result == 0)
            {
                PyErr_SetString(PyExc_TypeError, "not a System.Object");
                throw python_exception();
            }

            auto wrapper = reinterpret_cast<winrt_wrapper_base*>(obj);
            return as<winrt::Windows::Foundation::IInspectable>(wrapper);
        }
    };

    template<>
    struct converter<winrt::hstring>
    {
        static PyObject* convert(winrt::hstring const& value) noexcept
        {
            return PyUnicode_FromWideChar(value.c_str(), value.size());
        }

        static winrt::hstring convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            Py_ssize_t py_size;
            auto buffer = PyUnicode_AsWideCharString(obj, &py_size);

            if (!buffer)
            {
                throw python_exception();
            }

            auto size = static_cast<winrt::hstring::size_type>(py_size);

            winrt::hstring str{buffer, size};

            return str;
        }
    };

    template<>
    struct converter<char16_t>
    {
        static PyObject* convert(char16_t value) noexcept
        {
            return PyUnicode_FromWideChar(reinterpret_cast<const wchar_t*>(&value), 1);
        }

        static char16_t convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            Py_ssize_t size;
            auto buffer = PyUnicode_AsWideCharString(obj, &size);

            if (!buffer)
            {
                throw python_exception();
            }

            if (size != 1)
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "expected a character, but string of length %zd found",
                    size);
                throw python_exception();
            }

            return buffer[0];
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

            auto type = get_python_type<T>();

            if (!type)
            {
                throw python_exception();
            }

            if (Py_TYPE(obj) != type)
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
                PyErr_Format(
                    PyExc_NotImplementedError,
                    "py::python_iterable<%s>::iter::GetMany() is not implemented",
                    typeid(T).name());
                throw python_exception();
            }
        };
    };

    template<typename T>
    std::optional<T> convert_interface_to(PyObject* obj)
    {
        throw_if_pyobj_null(obj);

        auto type = get_python_type<T>();

        if (!type)
        {
            throw python_exception();
        }

        if (Py_TYPE(obj) == type)
        {
            return reinterpret_cast<winrt_wrapper<T>*>(obj)->obj;
        }

        auto object_type = get_python_type<Object>();

        if (!object_type)
        {
            throw python_exception();
        }

        auto result
            = PyObject_IsInstance(obj, reinterpret_cast<PyObject*>(object_type));

        if (result == -1)
        {
            throw python_exception();
        }

        if (result == 0)
        {
            return {};
        }

        auto wrapper = reinterpret_cast<winrt_wrapper_base*>(obj);
        return as<T>(wrapper);
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
    struct is_specialized_interface : std::false_type
    {
    };

    template<typename T>
    inline constexpr bool is_specialized_interface_v
        = is_specialized_interface<T>::value;

    template<typename TItem>
    struct is_specialized_interface<
        winrt::Windows::Foundation::Collections::IIterable<TItem>> : std::true_type
    {
    };

    template<typename T>
    struct is_specialized_interface<winrt::Windows::Foundation::IReference<T>>
        : std::true_type
    {
    };

    template<typename T>
    struct converter<
        T,
        typename std::enable_if_t<(
            is_interface_category_v<T>
            || is_pinterface_category_v<T>)&&!is_specialized_interface_v<T>>>
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
            PyErr_Format(
                PyExc_NotImplementedError,
                "py::converter<%s>::convert() is not implemented for delegates",
                typeid(T).name());
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
    template<typename T, bool writeable>
    struct pybuf_view : winrt::array_view<T>
    {
        using typename winrt::array_view<T>::value_type;
        using typename winrt::array_view<T>::pointer;

        pybuf_view(pybuf_view const&) = delete;
        pybuf_view& operator=(pybuf_view const&) = delete;

        pybuf_view() = delete;

        pybuf_view(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            // this is assuming pybuf_view is always treated as read-only
            if (PyObject_GetBuffer(
                    obj,
                    &view,
                    PyBUF_C_CONTIGUOUS | PyBUF_FORMAT
                        | (writeable ? PyBUF_WRITABLE : 0))
                == -1)
            {
                throw python_exception();
            }

            // TODO: if view.format == "P", we should try to verify the pointer type

            if (!buffer<T>::is_compatible(view))
            {
                PyBuffer_Release(&view);

                throw python_exception();
            }

            this->m_data = reinterpret_cast<pointer>(view.buf);
            this->m_size = view.shape[0];
        }

        ~pybuf_view()
        {
            PyBuffer_Release(&view);
        }

      private:
        Py_buffer view;
    };

    template<typename T, bool writeable>
    struct converter<pybuf_view<T, writeable>>
    {
        static PyObject* convert(pybuf_view<T, writeable> const& instance) noexcept
        {
            PyErr_Format(
                PyExc_NotImplementedError,
                "py::converter<%s>::convert() is not implemented for py::pybuf_view",
                typeid(T).name());
            return nullptr;
        }

        static pybuf_view<T, writeable> convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            return pybuf_view<T, writeable>{obj};
        }
    };

    template<typename T>
    struct ComArray;

    template<typename T>
    struct converter<winrt::com_array<T>>
    {
        static PyObject* convert(winrt::com_array<T> const& instance) noexcept
        {
            auto array = std::make_unique<ComArray<T>>();
            // HACK: casting to rvalue reference to steal data
            // std::move doesn't work because of const&
            array->array = const_cast<winrt::com_array<T>&&>(instance);
            return py::cpp::_winrt::Array_New(std::move(array));
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

            return items;
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
    struct converter<std::optional<T>>
    {
        static PyObject* convert(std::optional<T> const& reference) noexcept
        {
            if (!reference)
            {
                Py_RETURN_NONE;
            }

            return converter<T>::convert(reference.value());
        }

        static std::optional<T> convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            if (obj == Py_None)
            {
                return {};
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
                [cb = std::move(cb)](auto const& operation, auto status) mutable
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

    template<typename T>
    struct ComArray : Array
    {
        winrt::com_array<T> array;

        bool Alloc(uint32_t size) noexcept override
        {
            try
            {
                array = winrt::com_array<T>(size, empty_instance<T>::get());
                return true;
            }
            catch (...)
            {
                py::to_PyErr();
                return false;
            }
        }

        virtual std::wstring_view WinrtElementTypeName() noexcept override
        {
            return winrt::name_of<T>();
        }

        std::string_view Format() noexcept override
        {
            static_assert(buffer_format<T>);
            return buffer_format<T>;
        }

        uint32_t Size() noexcept override
        {
            return array.size();
        }

        size_t ValueSize() noexcept override
        {
            return sizeof(typename winrt::com_array<T>::value_type);
        }

        void* Data() noexcept override
        {
            return array.data();
        }

        PyObject* At(uint32_t index) noexcept override
        {
            try
            {
                return convert<T>(array.at(index));
            }
            catch (...)
            {
                to_PyErr();
                return nullptr;
            }
        }

        bool Set(Py_ssize_t index, PyObject* item) noexcept override
        {
            if (index < 0)
            {
                index = array.size() + index;
            }

            if (index < 0 || index >= array.size())
            {
                PyErr_SetString(PyExc_IndexError, "index out of range");
                return false;
            }

            try
            {
                array[index] = convert_to<T>(item);
                return true;
            }
            catch (...)
            {
                py::to_PyErr();
                return false;
            }
        }
    };
} // namespace py
