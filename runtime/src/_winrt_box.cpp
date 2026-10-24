#include <pywinrt/base.h>

#include <winrt/base.h>
#include <Python.h>

namespace
{
    /**
     * Sets the TypeError of unboxing @p value, which is not a boxed @p type,
     * naming what it is instead when WinRT says.
     */
    void set_unbox_error(
        winrt::Windows::Foundation::IInspectable const& value,
        const char* type) noexcept
    {
        if (!value)
        {
            PyErr_Format(PyExc_TypeError, "expected a boxed %s, not None", type);
            return;
        }

        winrt::hstring name;

        try
        {
            name = winrt::get_class_name(value);
        }
        catch (...)
        {
            // an object that does not say what it is still is not one
        }

        if (name.empty())
        {
            PyErr_Format(PyExc_TypeError, "expected a boxed %s", type);
            return;
        }

        py::pyobj_handle actual{
            PyUnicode_FromWideChar(name.c_str(), static_cast<Py_ssize_t>(name.size()))};
        if (!actual)
        {
            return;
        }

        PyErr_Format(
            PyExc_TypeError, "expected a boxed %s, not %U", type, actual.get());
    }

    /**
     * The value boxed in @p obj, which has to be an IReference<T>: a value
     * boxed as @p type, the WinRT name of @p T.
     */
    template<typename T>
    PyObject* unbox(PyObject* obj, const char* type) noexcept
    {
        try
        {
            auto const value
                = py::convert_to<winrt::Windows::Foundation::IInspectable>(obj);

            auto const reference
                = value.try_as<winrt::Windows::Foundation::IReference<T>>();
            if (!reference)
            {
                set_unbox_error(value, type);
                return nullptr;
            }

            return py::convert(reference.Value());
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }
} // namespace

namespace py::cpp::_winrt
{
    PyObject* box_boolean(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<bool>(obj);
            return py::convert(winrt::box_value(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* box_int8(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<int8_t>(obj);
            return py::convert(winrt::box_value(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* box_uint8(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<uint8_t>(obj);
            return py::convert(winrt::box_value(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* box_int16(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<int16_t>(obj);
            return py::convert(winrt::box_value(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* box_uint16(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<uint16_t>(obj);
            return py::convert(winrt::box_value(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* box_int32(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<int32_t>(obj);
            return py::convert(winrt::box_value(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* box_uint32(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<uint32_t>(obj);
            return py::convert(winrt::box_value(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* box_int64(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<int64_t>(obj);
            return py::convert(winrt::box_value(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* box_uint64(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<uint64_t>(obj);
            return py::convert(winrt::box_value(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* box_single(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<float>(obj);
            return py::convert(winrt::box_value(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* box_double(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<double>(obj);
            return py::convert(winrt::box_value(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* box_char16(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<char16_t>(obj);
            return py::convert(winrt::box_value(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* box_string(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<winrt::hstring>(obj);
            return py::convert(winrt::box_value(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* box_guid(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<winrt::guid>(obj);
            return py::convert(winrt::box_value(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* box_date_time(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<winrt::Windows::Foundation::DateTime>(obj);
            return py::convert(winrt::box_value(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* box_time_span(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<winrt::Windows::Foundation::TimeSpan>(obj);
            return py::convert(winrt::box_value(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* unbox_boolean(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        return unbox<bool>(obj, "Boolean");
    }

    PyObject* unbox_int8(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        return unbox<int8_t>(obj, "Int8");
    }

    PyObject* unbox_uint8(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        return unbox<uint8_t>(obj, "UInt8");
    }

    PyObject* unbox_int16(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        return unbox<int16_t>(obj, "Int16");
    }

    PyObject* unbox_uint16(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        return unbox<uint16_t>(obj, "UInt16");
    }

    PyObject* unbox_int32(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        return unbox<int32_t>(obj, "Int32");
    }

    PyObject* unbox_uint32(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        return unbox<uint32_t>(obj, "UInt32");
    }

    PyObject* unbox_int64(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        return unbox<int64_t>(obj, "Int64");
    }

    PyObject* unbox_uint64(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        return unbox<uint64_t>(obj, "UInt64");
    }

    PyObject* unbox_single(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        return unbox<float>(obj, "Single");
    }

    PyObject* unbox_double(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        return unbox<double>(obj, "Double");
    }

    PyObject* unbox_char16(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        return unbox<char16_t>(obj, "Char16");
    }

    PyObject* unbox_string(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        return unbox<winrt::hstring>(obj, "String");
    }

    PyObject* unbox_guid(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        return unbox<winrt::guid>(obj, "Guid");
    }

    PyObject* unbox_date_time(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        return unbox<winrt::Windows::Foundation::DateTime>(obj, "DateTime");
    }

    PyObject* unbox_time_span(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        return unbox<winrt::Windows::Foundation::TimeSpan>(obj, "TimeSpan");
    }
} // namespace py::cpp::_winrt
