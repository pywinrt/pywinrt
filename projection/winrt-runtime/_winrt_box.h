
#include <Python.h>

namespace py::cpp::_winrt
{
    PyObject* box_boolean(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_int8(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_uint8(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_int16(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_uint16(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_int32(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_uint32(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_int64(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_uint64(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_single(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_double(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_char16(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_string(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_guid(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_date_time(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* box_time_span(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_boolean(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_int8(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_uint8(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_int16(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_uint16(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_int32(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_uint32(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_int64(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_uint64(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_single(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_double(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_char16(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_string(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_guid(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_date_time(PyObject* /*unused*/, PyObject* obj) noexcept;
    PyObject* unbox_time_span(PyObject* /*unused*/, PyObject* obj) noexcept;
} // namespace py::cpp::_winrt
