// Converting values between Python and WinRT.
//
// py::convert<T>() and py::convert_to<T>() are what the generated code calls;
// both dispatch to py::converter<T>, specialized here for the fundamental
// types, strings, enums, structs, classes, interfaces, delegates and
// IReference<T>. The collection interfaces are specialized in
// <pywinrt/collections.h>, arrays in <pywinrt/array.h> and buffers in
// <pywinrt/buffer.h>; <pywinrt/base.h> pulls in all of them, which is what makes
// every specialization visible before a module instantiates any of them.
//
// The py::wrap() family above the converters is the Python side of the same job:
// it allocates the PyObject layouts from <pywinrt/wrappers.h> and fills them in.
//
// This is also where the per-module template instantiations are concentrated, so
// it is the file to look at when a module is larger or slower to compile than it
// should be.

#pragma once

#include <pywinrt/abi.h>
#include <pywinrt/errors.h>
#include <pywinrt/handles.h>
#include <pywinrt/prelude.h>
#include <pywinrt/traits.h>
#include <pywinrt/wrappers.h>

namespace py
{
    namespace impl
    {
        /**
         * One answer from the type registry, remembered by the template that
         * asked for it so that the same @c T is only looked up once.
         *
         * A memo is good for the registry that gave it and no other, which is
         * what the interpreter and the epoch say; py::get_type_registry_epoch()
         * in <pywinrt/abi.h> explains why it takes both. Static storage makes
         * a memo that has never been filled in zero, and a null interpreter is
         * not one that any thread runs in, so it never matches.
         */
        struct registry_memo
        {
            void* value;
            PyInterpreterState* interpreter;
            uint64_t epoch;

            /// The remembered answer, or nullptr if it has to be looked up
            /// again.
            void* get() const noexcept
            {
                return epoch == get_type_registry_epoch()
                               && interpreter == PyInterpreterState_Get()
                           ? value
                           : nullptr;
            }

            /// Remembers @p answer for the registry that is live now. A
            /// lookup that failed is not remembered: it left a Python error
            /// set, which the next caller has to be given as well.
            void set(void* answer) noexcept
            {
                if (!answer)
                {
                    return;
                }

                value = answer;
                interpreter = PyInterpreterState_Get();
                epoch = get_type_registry_epoch();
            }
        };

// The miss path of a memo, which every instantiation of the templates below
// shares rather than carrying a copy of. It runs once per type per process, so
// what matters about it is that it is not inlined into the hundreds of
// get_python_type_for<T>() that the compiler emits per module: leaving it out
// of them is 0.5 % of a projection build.
#ifdef _MSC_VER
#define PYWINRT_MEMO_MISS __declspec(noinline)
#else
#define PYWINRT_MEMO_MISS [[gnu::noinline]]
#endif

        PYWINRT_MEMO_MISS inline void* fill_type_memo(
            registry_memo& memo, std::string_view qualified_name) noexcept
        {
            auto type = get_python_type(qualified_name);
            memo.set(type);

            return type;
        }

        PYWINRT_MEMO_MISS inline void* fill_func_memo(
            registry_memo& memo, std::string_view capsule_name) noexcept
        {
            auto func = get_struct_from_tuple_func(capsule_name);
            memo.set(func);

            return func;
        }

#undef PYWINRT_MEMO_MISS
    } // namespace impl

    /**
     * Gets the Python wrapper type object for @p T.
     *
     * @tparam T The winrt type to get the wrapper type for.
     * @returns A borrowed reference to the type or nullptr if the type was not
     * registered.
     */
    template<typename T>
    PyTypeObject* get_python_type_for() noexcept
    {
        using winrt_type = std::conditional_t<
            is_pinterface_category_v<T>,
            typename pinterface_python_type<T>::abstract,
            T>;

        static_assert(!std::empty(py_type<winrt_type>::qualified_name));

        // The registry looks the name up by string, and the generated code
        // asks for the same type on every conversion - four times per struct
        // argument that makes a round trip - so the answer is memoized.
        static impl::registry_memo memo{};

        auto type = memo.get();

        if (!type)
        {
            type = impl::fill_type_memo(memo, py_type<winrt_type>::qualified_name);
        }

        return static_cast<PyTypeObject*>(type);
    }

    /**
     * Gets the Python wrapper type object for @p T.
     *
     * @tparam T The winrt type to get the wrapper type for.
     * @returns A borrowed reference to the type or nullptr if the type was not
     * registered.
     */
    template<typename T>
    auto get_struct_from_tuple_func_for() noexcept
    {
        using func_t = T (*)(PyObject*);
        static_assert(!std::empty(py_type<T>::from_tuple));

        static impl::registry_memo memo{};

        auto func = memo.get();

        if (!func)
        {
            func = impl::fill_func_memo(memo, py_type<T>::from_tuple);
        }

        return reinterpret_cast<func_t>(func);
    }

    /**
     * Converts a Python integer object to a Python Enum object.
     *
     * This is the equivelent of calling `Enum(value)` in Python.
     *
     * @param value The value object.
     * @tparam T A winrt enum type.
     * @returns A new reference to the Enum object or nullptr on error.
     */
    template<typename T>
    PyObject* wrap_enum(PyObject* value) noexcept
    {
        static_assert(is_enum_category_v<T>);

        auto type_object = get_python_type_for<T>();
        if (!type_object)
        {
            return nullptr;
        }

        pyobj_handle obj{
            PyObject_CallOneArg(reinterpret_cast<PyObject*>(type_object), value)};
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
                type_name<T>());
            return nullptr;
        }

        auto py_instance = reinterpret_cast<py::winrt_struct_wrapper<T>*>(
            type_object->tp_alloc(type_object, 0));

        if (!py_instance)
        {
            return nullptr;
        }

        // call C++ constructors on memory allocated from CPython heap
        std::construct_at(&py_instance->obj, instance);

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
                type_name<T>());
            return nullptr;
        }

        auto py_instance = reinterpret_cast<py::winrt_wrapper<T>*>(
            type_object->tp_alloc(type_object, 0));

        if (!py_instance)
        {
            return nullptr;
        }

        // call C++ constructors on memory allocated from CPython heap
        std::construct_at(&py_instance->obj, instance);

        return reinterpret_cast<PyObject*>(py_instance);
    }

    template<typename T>
    PyObject* wrap_pinterface(T instance) noexcept
    {
        if (!instance)
        {
            Py_RETURN_NONE;
        }

        using ptype = pinterface_python_type<T>;

        auto type = get_python_type_for<T>();
        if (!type)
        {
            return nullptr;
        }

        auto py_instance
            = reinterpret_cast<py::winrt_pinterface_wrapper<typename ptype::abstract>*>(
                type->tp_alloc(type, 0));

        if (!py_instance)
        {
            return nullptr;
        }

        // call C++ constructors on memory allocated from CPython heap
        std::construct_at(&py_instance->obj, instance);
        std::construct_at(&py_instance->impl);
        py_instance->impl = std::make_unique<typename ptype::concrete>(instance);

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
            auto type = get_python_type_for<T>();
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
                    type_name<T>());
                return nullptr;
            }
            else
            {
                return wrap_pinterface<T>(instance);
            }
        }
    }

    template<typename T, typename = void>
    struct converter
    {
        static_assert(
            std::is_same_v<T, void>,
            "specialization of py::converter<T> is not implemented");
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

            if (result == static_cast<unsigned long>(-1) && PyErr_Occurred())
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

            if (result == static_cast<unsigned long long>(-1) && PyErr_Occurred())
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
            return convert_guid(value);
        }

        static winrt::guid convert_to(PyObject* obj)
        {
            return convert_to_guid(obj);
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
            return convert_datetime(value);
        }

        static winrt::Windows::Foundation::DateTime convert_to(PyObject* obj)
        {
            return convert_to_datetime(obj);
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
                if (!PyDateTimeAPI)
                {
                    PyDateTime_IMPORT;

                    if (!PyDateTimeAPI)
                    {
                        return nullptr;
                    }
                }

                auto days = std::chrono::duration_cast<std::chrono::days>(value);
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

            if (!PyDateTimeAPI)
            {
                PyDateTime_IMPORT;

                if (!PyDateTimeAPI)
                {
                    throw python_exception();
                }
            }

            if (!PyDelta_Check(obj))
            {
                PyErr_SetString(PyExc_TypeError, "requires datetime.timedelta object");
                throw python_exception();
            }

            return std::chrono::duration_cast<winrt::Windows::Foundation::TimeSpan>(
                std::chrono::days(PyDateTime_DELTA_GET_DAYS(obj))
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
            if (!value)
            {
                Py_RETURN_NONE;
            }

            try
            {
                winrt::com_ptr<IPywinrtObject> obj{};
                if (value.try_as(obj))
                {
                    PyObject* pyobj;
                    winrt::check_hresult(obj->GetPyObject(pyobj));
                    return pyobj;
                }
            }
            catch (...)
            {
                py::to_PyErr();
                return nullptr;
            }

            auto object_type = get_object_type();

            if (!object_type)
            {
                return nullptr;
            }

            return wrap<winrt::Windows::Foundation::IInspectable>(value, object_type);
        }

        static winrt::Windows::Foundation::IInspectable convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            if (Py_IsNone(obj))
            {
                return nullptr;
            }

            auto object_type = get_object_type();

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

            return reinterpret_cast<
                       winrt_wrapper<winrt::Windows::Foundation::IUnknown>*>(obj)
                ->obj.as<winrt::Windows::Foundation::IInspectable>();
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
            std::unique_ptr<wchar_t, decltype(&PyMem_Free)> buffer{
                PyUnicode_AsWideCharString(obj, &py_size), &PyMem_Free};

            if (!buffer)
            {
                throw python_exception();
            }

            auto size = static_cast<winrt::hstring::size_type>(py_size);

            winrt::hstring str{buffer.get(), size};

            return str;
        }
    };

    template<>
    struct converter<winrt::param::hstring>
    {
        static PyObject* convert(winrt::param::hstring const& value) noexcept
        {
            return PyUnicode_FromWideChar(
                static_cast<winrt::hstring const&>(value).c_str(),
                static_cast<winrt::hstring const&>(value).size());
        }

        static winrt::param::hstring convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            // It is expected that this is only used to convert delegate parameters
            // to Python objects, so we shouldn't need to go the other way around.
            throw winrt::hresult_not_implemented{
                L"winrt::param::hstring cannot be converted from Python object"};
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
            std::unique_ptr<wchar_t, decltype(&PyMem_Free)> buffer{
                PyUnicode_AsWideCharString(obj, &size), &PyMem_Free};

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

            return *buffer;
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

            if (!value)
            {
                return nullptr;
            }

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
    struct converter<T, typename std::enable_if_t<is_struct_category_v<T>>>
    {
        static PyObject* convert(T instance) noexcept
        {
            auto type = get_python_type_for<T>();
            if (!type)
            {
                return nullptr;
            }

            return py::wrap_struct(instance, type);
        }

        static auto convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            if (PyTuple_Check(obj))
            {
                auto func = get_struct_from_tuple_func_for<T>();
                if (!func)
                {
                    throw python_exception();
                }

                return func(obj);
            }

            auto type = get_python_type_for<T>();
            if (!type)
            {
                throw python_exception();
            }

            if (!Py_IS_TYPE(obj, type))
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "must be %.200s, not %.200s",
                    type->tp_name,
                    Py_TYPE(obj)->tp_name);
                throw python_exception();
            }

            return reinterpret_cast<py::winrt_struct_wrapper<T>*>(obj)->obj;
        }
    };

    template<typename T>
    std::optional<T> convert_interface_to(PyObject* obj)
    {
        throw_if_pyobj_null(obj);

        auto type = get_python_type_for<T>();
        if (!type)
        {
            throw python_exception();
        }

        if (Py_IS_TYPE(obj, type))
        {
            return reinterpret_cast<winrt_wrapper<T>*>(obj)->obj;
        }

        auto object_type = get_object_type();

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
            if constexpr (std::is_same_v<winrt::Windows::Storage::Streams::IBuffer, T>)
            {
                return py::convert_to_ibuffer(obj);
            }
            else
            {
                return std::nullopt;
            }
        }

        return reinterpret_cast<winrt_wrapper<winrt::Windows::Foundation::IUnknown>*>(
                   obj)
            ->obj.as<T>();
    }

    template<typename T>
    struct converter<
        T,
        typename std::enable_if_t<
            (is_class_category_v<T> || is_interface_category_v<T>
             || is_pinterface_category_v<T>)
            && !is_specialized_interface_v<T>>>
    {
        static PyObject* convert(T const& instance) noexcept
        {
            try
            {
                winrt::com_ptr<IPywinrtObject> obj{};
                if (instance.try_as(obj))
                {
                    PyObject* pyobj;
                    winrt::check_hresult(obj->GetPyObject(pyobj));
                    return pyobj;
                }
            }
            catch (...)
            {
                py::to_PyErr();
                return nullptr;
            }

            return wrap(instance);
        }

        static auto convert_to(PyObject* obj)
        {
            if (Py_IsNone(obj))
            {
                return empty_instance<T>::get();
            }

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
        static PyObject* convert(T const& /*unused*/) noexcept
        {
            // TODO: support converting delegates
            PyErr_Format(
                PyExc_NotImplementedError,
                "py::converter<%s>::convert() is not implemented for delegates",
                type_name<T>());
            return nullptr;
        }

        static auto convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);
            return delegate_python_type<T>::type::get(obj);
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

            if (Py_IsNone(obj))
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

            if (Py_IsNone(obj))
            {
                return std::nullopt;
            }

            return converter<T>::convert_to(obj);
        }
    };

    template<typename T>
    PyObject* convert(T const& instance) noexcept
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
        return convert_to<T>(PyTuple_GET_ITEM(args, index));
    }
} // namespace py
