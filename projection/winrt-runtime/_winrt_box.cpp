#define PYWINRT_RUNTIME_MODULE
#include "pybase.h"

#include <winrt/base.h>
#include <Python.h>

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
        try
        {
            auto value = py::convert_to<winrt::Windows::Foundation::IInspectable>(obj);
            return py::convert(winrt::unbox_value<bool>(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* unbox_int8(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<winrt::Windows::Foundation::IInspectable>(obj);
            return py::convert(winrt::unbox_value<int8_t>(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* unbox_uint8(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<winrt::Windows::Foundation::IInspectable>(obj);
            return py::convert(winrt::unbox_value<uint8_t>(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* unbox_int16(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<winrt::Windows::Foundation::IInspectable>(obj);
            return py::convert(winrt::unbox_value<int16_t>(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* unbox_uint16(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<winrt::Windows::Foundation::IInspectable>(obj);
            return py::convert(winrt::unbox_value<uint16_t>(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* unbox_int32(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<winrt::Windows::Foundation::IInspectable>(obj);
            return py::convert(winrt::unbox_value<int32_t>(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* unbox_uint32(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<winrt::Windows::Foundation::IInspectable>(obj);
            return py::convert(winrt::unbox_value<uint32_t>(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* unbox_int64(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<winrt::Windows::Foundation::IInspectable>(obj);
            return py::convert(winrt::unbox_value<int64_t>(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* unbox_uint64(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<winrt::Windows::Foundation::IInspectable>(obj);
            return py::convert(winrt::unbox_value<uint64_t>(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* unbox_single(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<winrt::Windows::Foundation::IInspectable>(obj);
            return py::convert(winrt::unbox_value<float>(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* unbox_double(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<winrt::Windows::Foundation::IInspectable>(obj);
            return py::convert(winrt::unbox_value<double>(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* unbox_char16(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<winrt::Windows::Foundation::IInspectable>(obj);
            return py::convert(winrt::unbox_value<char16_t>(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* unbox_string(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<winrt::Windows::Foundation::IInspectable>(obj);
            return py::convert(winrt::unbox_value<winrt::hstring>(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* unbox_guid(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<winrt::Windows::Foundation::IInspectable>(obj);
            return py::convert(winrt::unbox_value<winrt::guid>(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* unbox_date_time(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<winrt::Windows::Foundation::IInspectable>(obj);
            return py::convert(
                winrt::unbox_value<winrt::Windows::Foundation::DateTime>(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    PyObject* unbox_time_span(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        try
        {
            auto value = py::convert_to<winrt::Windows::Foundation::IInspectable>(obj);
            return py::convert(
                winrt::unbox_value<winrt::Windows::Foundation::TimeSpan>(value));
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }
} // namespace py::cpp::_winrt
