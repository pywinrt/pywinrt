// Converting values between Python and WinRT.
//
// py::convert<T>() and py::convert_to<T>() dispatch to py::converter<T>,
// specialized here for the types the runtime API itself mentions: the
// fundamental types, strings, GUIDs, DateTime, TimeSpan, IInspectable and
// IReference<T>. Arrays are specialized in <pywinrt/array.h> and buffers in
// <pywinrt/buffer.h>; <pywinrt/base.h> pulls in all of them, which is what
// makes every specialization visible before a module instantiates any of them.
//
// A WinRT class, interface, struct, enum or delegate is not here. Naming one
// in C++ takes the header C++/WinRT generates for its namespace, and knowing
// which Python type projects it took a second generated header beside that;
// nothing generates either any more, because a projection is a table the
// runtime interprets. A module that holds such a value hands it over by name
// instead - py::wrap_object(), py::unwrap_object(), py::struct_to_python() and
// py::struct_from_python() in <pywinrt/abi.h>.

#pragma once

#include <pywinrt/abi.h>
#include <pywinrt/errors.h>
#include <pywinrt/handles.h>
#include <pywinrt/prelude.h>
#include <pywinrt/traits.h>
#include <pywinrt/wrappers.h>

namespace py
{
    /**
     * Allocates the py::winrt_wrapper<T> layout of @p type_object and puts
     * @p instance in it.
     *
     * @p type_object is the Python type that projects @p T, which the caller
     * has because it asked the runtime for it by name; None is what an empty
     * @p instance becomes.
     */
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
                // Not a wrapper, but a Python class that derives from the
                // public name of a projected interface is an implementation
                // of it, and the runtime knows how to stand one up. Anything
                // else fails there with a message of its own.
                void* abi{};

                if (!unwrap_object(
                        obj,
                        winrt::guid_of<winrt::Windows::Foundation::IInspectable>(),
                        &abi))
                {
                    throw python_exception();
                }

                winrt::Windows::Foundation::IInspectable value;
                winrt::attach_abi(value, abi);

                return value;
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
