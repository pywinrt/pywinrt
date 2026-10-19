// keeps <windows.h> from defining min and max as macros
#ifndef NOMINMAX
#define NOMINMAX
#endif

// must be included before cppwinrt to enable FILETIME conversions
#include <Windows.h>

#include <Python.h>
#include <datetime.h>

#include <pywinrt/base.h>
#include "metaclass.h"
#include "module_state.h"
#include "runtime.h"
#include "types.h"

/**
 * Adds a Python type to a Python module.
 *
 * @param module The module to add the type to.
 * @param type_spec The Python type spec.
 * @param base_type The base type, a tuple of base types or nullptr to use the base
 * slot.
 * @param metaclass Optional metaclass for the type.
 * @returns New reference on success or nullptr on error.
 */

PyTypeObject* py::register_python_type(
    PyObject* module,
    PyType_Spec* type_spec,
    PyObject* base_type,
    PyTypeObject* metaclass) noexcept
{
    py::pytype_handle type_object{reinterpret_cast<PyTypeObject*>(
        PyType_FromMetaclass(metaclass, module, type_spec, base_type))};

    if (!type_object)
    {
        return nullptr;
    }

    if (PyModule_AddType(module, type_object.get()) == -1)
    {
        return nullptr;
    }

    return type_object.detach();
}

namespace
{
    /**
     * Records the Python type that @p qualified_name resolved to, and hands
     * back the one the cache holds.
     *
     * That is not always the one passed in. The cache owns the only reference
     * that keeps a type alive and every caller gets a borrowed pointer out of
     * it, so when two threads resolve the same name at once, one of the two
     * answers has to be the answer for both, and it is decided here rather
     * than by whichever thread wrote last.
     */
    PyTypeObject* remember_python_type(
        py::cpp::_winrt::module_state* state,
        std::string_view qualified_name,
        PyObject* type) noexcept
    {
        try
        {
            py::cpp::_winrt::state_guard guard{state->cache_lock};

            auto const [it, inserted]
                = state->type_cache.try_emplace(std::string{qualified_name});

            if (inserted)
            {
                it->second.attach(reinterpret_cast<PyTypeObject*>(Py_NewRef(type)));
            }

            return it->second.get();
        }
        catch (...)
        {
            py::to_PyErr();
            return nullptr;
        }
    }

    /**
     * Whether @p module is a namespace package, which no file of its own
     * defines.
     *
     * @returns -1 with a Python error set.
     */
    int is_namespace_package(PyObject* module) noexcept
    {
        py::pyobj_handle spec{};
        if (PyObject_GetOptionalAttrString(module, "__spec__", spec.put()) < 0)
        {
            return -1;
        }

        if (!spec || Py_IsNone(spec.get()))
        {
            return 0;
        }

        py::pyobj_handle origin{};
        if (PyObject_GetOptionalAttrString(spec.get(), "origin", origin.put()) < 0)
        {
            return -1;
        }

        return !origin || Py_IsNone(origin.get());
    }
} // namespace

PyTypeObject* py::get_python_type(std::string_view qualified_name) noexcept
{
    auto state = py::cpp::_winrt::get_module_state();
    if (!state)
    {
        return nullptr;
    }

    {
        py::cpp::_winrt::state_guard guard{state->cache_lock};

        auto it = state->type_cache.find(qualified_name);
        if (it != state->type_cache.end())
        {
            return it->second.get();
        }
    }

    // A projection package's types are built from its table rather than
    // imported from an extension module, so the table is asked first. It is
    // registered before a single type is built, which is what lets a namespace
    // that two packages refer to each other through resolve while one of them
    // is still executing its __init__.py.
    if (auto from_table = py::interp::find_registered_type(qualified_name))
    {
        return remember_python_type(
            state, qualified_name, reinterpret_cast<PyObject*>(from_table));
    }

    if (PyErr_Occurred())
    {
        return nullptr;
    }

    auto pos = qualified_name.find_last_of('.');
    if (pos == std::string_view::npos)
    {
        PyErr_Format(
            PyExc_ValueError, "invalid qualified name: %s", qualified_name.data());
        return nullptr;
    }

    std::string module_name{qualified_name.substr(0, pos)};
    std::string type_name{qualified_name.substr(pos + 1)};

    pyobj_handle module{PyImport_ImportModule(module_name.c_str())};
    if (!module)
    {
        return nullptr;
    }

    // Importing it may have registered a table, which is where a projected
    // type comes from.
    if (auto from_table = py::interp::find_registered_type(qualified_name))
    {
        return remember_python_type(
            state, qualified_name, reinterpret_cast<PyObject*>(from_table));
    }

    if (PyErr_Occurred())
    {
        return nullptr;
    }

    pyobj_handle type{};
    if (PyObject_GetOptionalAttrString(module.get(), type_name.c_str(), type.put()) < 0)
    {
        return nullptr;
    }

    if (!type)
    {
        // Every package installs into the same directories, so a namespace
        // whose package is missing is still a namespace package when a package
        // of a namespace below it is installed. It has no types, and the
        // package to install is the one named after it, so it is missing as a
        // module would be.
        auto const is_namespace = is_namespace_package(module.get());
        if (is_namespace < 0)
        {
            return nullptr;
        }

        if (is_namespace)
        {
            pyobj_handle name{PyUnicode_FromString(module_name.c_str())};
            if (!name)
            {
                return nullptr;
            }

            pyobj_handle message{
                PyUnicode_FromFormat("No module named '%s'", module_name.c_str())};
            if (!message)
            {
                return nullptr;
            }

            PyErr_SetImportErrorSubclass(
                PyExc_ModuleNotFoundError, message.get(), name.get(), nullptr);
            return nullptr;
        }

        // Asked again for the module's own AttributeError.
        type.attach(PyObject_GetAttrString(module.get(), type_name.c_str()));
        if (!type)
        {
            return nullptr;
        }
    }

    if (!PyType_Check(type.get()))
    {
        PyErr_Format(
            PyExc_TypeError,
            "'%s.%s' is not a type",
            module_name.c_str(),
            type_name.c_str());
        return nullptr;
    }

    // A projected type comes from its package's table above, so what reaches
    // here is a Python type the runtime did not build, such as one of the
    // winrt.system names. Only that the name is bound to a type is checked; a
    // caller that needs a type built from a table looks it up with
    // py::interp::find_type_entry() and raises if there is none.

    return remember_python_type(state, qualified_name, type.get());
}

/**
 * Wraps a WinRT KeyValuePair iterator in a Python type that iterates the
 * keys only to be consistent with the Python mapping protocol.
 * @param iter The mapping iterator returned from the First() method.
 * @return A new reference to a new object that wraps @p iter.
 */
PyObject* py::wrap_mapping_iter(PyObject* iter) noexcept
{
    auto state = py::cpp::_winrt::get_module_state();
    if (!state)
    {
        return nullptr;
    }

    py::pyobj_handle wrapper{PyObject_CallOneArg(
        reinterpret_cast<PyObject*>(state->mapping_iter_type.get()), iter)};

    if (!wrapper)
    {
        return nullptr;
    }

    return wrapper.detach();
}

/**
 * Tests if @p view is compatible with @p itemsize and @p format
 * @param [in]  view        A Python buffer view.
 * @param [in]  itemsize    The required element size.
 * @param [in]  format      The required element format.
 * @returns @c true if the buffer is valid, otherwise sets Python error and returns
 * @c false
 */
bool py::is_buffer_compatible(
    Py_buffer const& view, Py_ssize_t itemsize, const char* format) noexcept
{
    if (view.itemsize != itemsize)
    {
        PyErr_Format(
            PyExc_BufferError,
            "requires buffer with itemsize == %zd, have %zd",
            itemsize,
            view.itemsize);
        return false;
    }

    WINRT_ASSERT(format);

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
            PyExc_BufferError, "requires buffer with ndim == 1, have %d", view.ndim);
        return false;
    }

    if (!view.strides || view.strides[0] != itemsize)
    {
        PyErr_Format(
            PyExc_BufferError,
            "requires buffer with strides[0] == %zd, have %zd",
            itemsize,
            view.strides ? view.strides[0] : 0);
        return false;
    }

    return true;
}

namespace
{
    // A DateTime counts 100 ns ticks from here, as a FILETIME does.
    constexpr std::chrono::sys_days datetime_epoch{
        std::chrono::year{1601} / std::chrono::January / 1};

    /**
     * Raises the OverflowError for a DateTime in a year a Python datetime cannot
     * hold.
     * @param [in]  year    The DateTime's year.
     */
    [[noreturn]] void throw_year_out_of_range(int year)
    {
        PyErr_Format(
            PyExc_OverflowError,
            "a DateTime in the year %d is out of range for a datetime, which holds "
            "the years 1 to 9999",
            year);
        throw py::python_exception();
    }

    /**
     * Reads a DateTime before 1601 on the proleptic Gregorian calendar, as
     * Python's datetime does.
     * @param [in]  value   The DateTime, with a negative tick count.
     * @returns             Its date and time, to the millisecond.
     */
    SYSTEMTIME calendar_system_time(winrt::Windows::Foundation::DateTime value)
    {
        auto const time
            = std::chrono::sys_time<std::chrono::milliseconds>{datetime_epoch}
              + std::chrono::floor<std::chrono::milliseconds>(value.time_since_epoch());
        auto const date = std::chrono::floor<std::chrono::days>(time);
        std::chrono::year_month_day const ymd{date};

        if (static_cast<int>(ymd.year()) < 1)
        {
            throw_year_out_of_range(static_cast<int>(ymd.year()));
        }

        std::chrono::hh_mm_ss const hms{time - date};

        SYSTEMTIME st{};
        st.wYear = static_cast<WORD>(static_cast<int>(ymd.year()));
        st.wMonth = static_cast<WORD>(static_cast<unsigned>(ymd.month()));
        st.wDay = static_cast<WORD>(static_cast<unsigned>(ymd.day()));
        st.wHour = static_cast<WORD>(hms.hours().count());
        st.wMinute = static_cast<WORD>(hms.minutes().count());
        st.wSecond = static_cast<WORD>(hms.seconds().count());
        st.wMilliseconds = static_cast<WORD>(hms.subseconds().count());

        return st;
    }

    /**
     * Counts the ticks of a date and time before 1601 on the proleptic Gregorian
     * calendar, as Python's datetime does.
     * @param [in]  st      The date and time, to the millisecond.
     * @returns             The DateTime, with a negative tick count.
     */
    winrt::Windows::Foundation::DateTime calendar_date_time(SYSTEMTIME const& st)
    {
        auto const date = std::chrono::sys_days{
            std::chrono::year{st.wYear} / std::chrono::month{st.wMonth}
            / std::chrono::day{st.wDay}};
        auto const since_epoch = date - datetime_epoch + std::chrono::hours{st.wHour}
                                 + std::chrono::minutes{st.wMinute}
                                 + std::chrono::seconds{st.wSecond}
                                 + std::chrono::milliseconds{st.wMilliseconds};

        return winrt::Windows::Foundation::DateTime{
            std::chrono::duration_cast<winrt::Windows::Foundation::TimeSpan>(
                since_epoch)};
    }
} // namespace

/**
 * Converts a winrt Windows.Foundation.DateTime to a Python datetime.datetime.
 *
 * Windows converts a DateTime from 1601 on, since it knows the leap seconds a
 * tick count includes from 2018 on. It refuses an earlier one, which is read on
 * the calendar instead. There is no leap second before 1972 for the two to
 * disagree on.
 *
 * @param [in]  value   The winrt Windows.Foundation.DateTime.
 * @returns             The Python datetime.datetime or nullptr on error.
 */
PyObject* py::convert_datetime(winrt::Windows::Foundation::DateTime value) noexcept
{
    try
    {
        SYSTEMTIME st;

        if (value.time_since_epoch().count() < 0)
        {
            st = calendar_system_time(value);
        }
        else
        {
            FILETIME ft = winrt::clock::to_FILETIME(value);

            if (!FileTimeToSystemTime(&ft, &st))
            {
                winrt::throw_last_error();
            }
        }

        if (st.wYear > 9999)
        {
            throw_year_out_of_range(st.wYear);
        }

        auto const since_epoch = value.time_since_epoch();
        auto const microseconds = static_cast<int>(
            (std::chrono::floor<std::chrono::microseconds>(since_epoch)
             - std::chrono::floor<std::chrono::milliseconds>(since_epoch))
                .count());

        if (!PyDateTimeAPI)
        {
            PyDateTime_IMPORT;

            if (!PyDateTimeAPI)
            {
                return nullptr;
            }
        }

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

/**
 * Converts a Python datetime.datetime to a winrt Windows.Foundation.DateTime.
 *
 * Windows converts a date from 1601 on and the calendar an earlier one, as in
 * py::convert_datetime().
 *
 * @param [in]  obj     The Python datetime.datetime object.
 * @returns             The winrt Windows.Foundation.DateTime value.
 */
winrt::Windows::Foundation::DateTime py::convert_to_datetime(PyObject* obj)
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

    winrt::Windows::Foundation::DateTime value;

    if (st.wYear < 1601)
    {
        value = calendar_date_time(st);
    }
    else
    {
        FILETIME ft;

        if (!SystemTimeToFileTime(&st, &ft))
        {
            winrt::throw_last_error();
        }

        value = winrt::clock::from_FILETIME(ft);
    }

    auto microseconds = PyDateTime_DATE_GET_MICROSECOND(utc.get()) % 1000;

    value += std::chrono::duration_cast<winrt::Windows::Foundation::DateTime::duration>(
        std::chrono::microseconds{microseconds});

    return value;
}

PyObject* py::convert_guid(winrt::guid value) noexcept
{
    auto state = py::cpp::_winrt::get_module_state();
    if (!state)
    {
        return nullptr;
    }

    // It is faster to swap in C++ rather than using bytes_le in Python since
    // Python stores the bytes in a different order.
    alignas(4) char buffer[sizeof(winrt::guid)];
    *reinterpret_cast<uint32_t*>(buffer) = _byteswap_ulong(value.Data1);
    *reinterpret_cast<uint16_t*>(buffer + 4) = _byteswap_ushort(value.Data2);
    *reinterpret_cast<uint16_t*>(buffer + 6) = _byteswap_ushort(value.Data3);
    std::memcpy(buffer + 8, value.Data4, sizeof(value.Data4));

    pyobj_handle value_as_bytes{PyBytes_FromStringAndSize(buffer, sizeof(buffer))};
    if (!value_as_bytes)
    {
        return nullptr;
    }

    return PyObject_CallOneArg(state->to_uuid_func.get(), value_as_bytes.get());
}

winrt::guid py::convert_to_guid(PyObject* obj)
{
    throw_if_pyobj_null(obj);

    try
    {
        pyobj_handle bytes{PyObject_GetAttrString(obj, "bytes")};
        if (!bytes)
        {
            throw python_exception();
        }

        char* buffer;
        Py_ssize_t size;
        if (PyBytes_AsStringAndSize(bytes.get(), &buffer, &size) == -1)
        {
            throw python_exception();
        }

        if (size != sizeof(winrt::guid))
        {
            PyErr_SetString(PyExc_ValueError, "bytes is wrong size");
            throw python_exception();
        }

        // It is faster to swap in C++ rather than using bytes_le in Python since
        // Python stores the bytes in a different order.
        winrt::guid result;
        result.Data1 = _byteswap_ulong(*reinterpret_cast<uint32_t*>(buffer));
        result.Data2 = _byteswap_ushort(*reinterpret_cast<uint16_t*>(buffer + 4));
        result.Data3 = _byteswap_ushort(*reinterpret_cast<uint16_t*>(buffer + 6));
        std::memcpy(result.Data4, buffer + 8, sizeof(result.Data4));

        return result;
    }
    catch (python_exception)
    {
        // Convert any Python exception to a TypeError with cause set to the
        // original exception.

        pyobj_handle old_value{take_raised_exception()};

        PyErr_SetString(PyExc_TypeError, "requires uuid.UUID object");
        pyobj_handle new_value{take_raised_exception()};

        // steals reference to cause
        PyException_SetCause(new_value.get(), old_value.detach());

        restore_raised_exception(new_value.detach());

        throw python_exception();
    }
}
