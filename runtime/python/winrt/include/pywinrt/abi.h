// The ABI contract between a projection module and winrt-runtime.
//
// A projection module does not link against the runtime. It imports the
// winrt._winrt._C_API capsule at module init, checks the version, and reaches
// every runtime entry point through the py::runtime_api struct below, which the
// inline wrappers at the bottom of this file hide behind ordinary function
// names. The runtime module itself is compiled with PYWINRT_RUNTIME_MODULE
// defined and provides the real definitions instead.
//
// This is the one file where "does this change the ABI?" is answered, so it is
// deliberately the smallest one here:
//
//   - The struct is append-only. A new entry point goes at the end and bumps
//     runtime_abi_version_minor; changing, reordering or removing one bumps
//     runtime_abi_version_major, which every module built against an older
//     header refuses to load against.
//   - The types that cross the boundary are part of the contract too, even the
//     ones declared elsewhere: py::Array and its vtable (<pywinrt/wrappers.h>),
//     py::member_not_available below, and the C++/WinRT and Python types in the
//     signatures.
//   - Nothing else in pywinrt/ is part of it. The rest is inline in the calling
//     module, which carries its own copy, so it may change freely.

#pragma once

#include <pywinrt/handles.h>
#include <pywinrt/prelude.h>
#include <pywinrt/wrappers.h>

namespace py
{
    /**
     * The kind of member a Python attribute is projected from.
     */
    enum class member_kind
    {
        method,
        property,
        event,
    };

    /**
     * Thrown when an object does not implement the interface that declares the
     * member that is being called.
     *
     * The names point at string literals in the generated code, so they outlive
     * the exception without having to be copied.
     */
    struct member_not_available
    {
        member_kind kind;
        uint32_t arg_count;
        /// Metadata name of the type that declares the member or @c nullptr for
        /// a parameterized interface, which has no name in the metadata.
        const char* type_name;
        const char* member_name;
        /// Metadata name of the interface that was queried for.
        const char* interface_name;
    };

    /** Unique identifier for validating runtime API struct pointer. */
    const winrt::guid runtime_api_guid{"B6C6659B-8458-4D05-AC29-A3886597E7D2"};

    /**
     * ABI version for runtime verification.
     *
     * This must be changed if the runtime API changes in a way that breaks
     * binary compatibility.
     */
    const uint16_t runtime_abi_version_major = 4;

    /**
     * ABI version for runtime verification.
     *
     * This must be changed if the runtime API changes in a way that adds new
     * APIs but otherwise doesn't break binary compatibility.
     */
    const uint16_t runtime_abi_version_minor = 3;

    PyTypeObject* register_python_type(
        PyObject* module,
        PyType_Spec* type_spec,
        PyObject* base_type,
        PyTypeObject* metaclass) noexcept;
    PyTypeObject* get_python_type(std::string_view qualified_name) noexcept;
    void* get_struct_from_tuple_func(std::string_view capsule_name) noexcept;
    PyObject* wrap_mapping_iter(PyObject* iter) noexcept;
    bool is_buffer_compatible(
        Py_buffer const& view, Py_ssize_t itemsize, const char* format) noexcept;
    PyObject* convert_datetime(winrt::Windows::Foundation::DateTime value) noexcept;
    winrt::Windows::Foundation::DateTime convert_to_datetime(PyObject* obj);
    PyObject* convert_guid(winrt::guid value) noexcept;
    winrt::guid convert_to_guid(PyObject* obj);
    PyTypeObject* get_inspectable_meta_type() noexcept;
    PyTypeObject* get_object_type() noexcept;
    PyObject* await_async(PyObject*) noexcept;
    winrt::Windows::Storage::Streams::IBuffer convert_to_ibuffer(PyObject* obj);
    void set_member_not_available_error(member_not_available const& info) noexcept;

    namespace cpp::_winrt
    {
        PyObject* Array_New(std::unique_ptr<py::Array> array) noexcept;
        bool Array_Assign(PyObject* obj, std::unique_ptr<py::Array> array) noexcept;
    } // namespace cpp::_winrt

    struct runtime_api
    {
        winrt::guid runtime_api_guid;
        uint16_t abi_version_major;
        uint16_t abi_version_minor;
        decltype(register_python_type)* register_python_type;
        decltype(get_python_type)* get_python_type;
        decltype(get_struct_from_tuple_func)* get_struct_from_tuple_func;
        decltype(wrap_mapping_iter)* wrap_mapping_iter;
        decltype(is_buffer_compatible)* is_buffer_compatible;
        decltype(convert_datetime)* convert_datetime;
        decltype(convert_to_datetime)* convert_to_datetime;
        decltype(convert_guid)* convert_guid;
        decltype(convert_to_guid)* convert_to_guid;
        decltype(get_inspectable_meta_type)* get_inspectable_meta_type;
        decltype(get_object_type)* get_object_type;
        decltype(cpp::_winrt::Array_New)* array_new;
        decltype(cpp::_winrt::Array_Assign)* array_assign;
        decltype(await_async)* await_async;
        decltype(convert_to_ibuffer)* convert_to_ibuffer;
        decltype(set_member_not_available_error)* set_member_not_available_error;
    };

#ifndef PYWINRT_RUNTIME_MODULE
    // this section is used when compiling other modules that use the
    // winrt-runtime module

    [[maybe_unused]] static const runtime_api* PyWinRT_API;

    /* Return -1 on error, 0 on success.
     * PyCapsule_Import will set an exception if there's an error.
     */
    [[maybe_unused]] static int import_winrt_runtime()
    {
        // Have to make sure winrt._winrt is imported, otherwise the capsule
        // lookup could fail if winrt was imported but winrt._winrt wasn't.
        pyobj_handle module{PyImport_ImportModule("winrt._winrt")};
        if (!module)
        {
            return -1;
        }

        PyWinRT_API = reinterpret_cast<runtime_api*>(
            PyCapsule_Import("winrt._winrt._C_API", 0));

        if (!PyWinRT_API)
        {
            return -1;
        }

        if (PyWinRT_API->runtime_api_guid != runtime_api_guid)
        {
            PyErr_SetString(
                PyExc_RuntimeError, "winrt._winrt._C_API capsule has invalid data");
            return -1;
        }

        if (PyWinRT_API->abi_version_major != runtime_abi_version_major)
        {
            PyErr_Format(
                PyExc_RuntimeError,
                "winrt._winrt._C_API ABI major version mismatch: expected %d, got %d",
                runtime_abi_version_major,
                PyWinRT_API->abi_version_major);
            return -1;
        }

        if (PyWinRT_API->abi_version_minor < runtime_abi_version_minor)
        {
            PyErr_Format(
                PyExc_RuntimeError,
                "_winrt._C_API ABI minor version mismatch: expected >= %d, got %d",
                runtime_abi_version_minor,
                PyWinRT_API->abi_version_minor);
            return -1;
        }

        return 0;
    }

    inline PyTypeObject* register_python_type(
        PyObject* module,
        PyType_Spec* type_spec,
        PyObject* base_type,
        PyTypeObject* metaclass) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->register_python_type);
        return (*PyWinRT_API->register_python_type)(
            module, type_spec, base_type, metaclass);
    }

    inline PyTypeObject* get_python_type(std::string_view qualified_name) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->get_python_type);
        return (*PyWinRT_API->get_python_type)(qualified_name);
    }

    inline void* get_struct_from_tuple_func(
        const std::string_view capsule_name) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->get_struct_from_tuple_func);
        return (*PyWinRT_API->get_struct_from_tuple_func)(capsule_name);
    }

    inline PyObject* wrap_mapping_iter(PyObject* iter) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->wrap_mapping_iter);
        return (*PyWinRT_API->wrap_mapping_iter)(iter);
    }

    inline void set_member_not_available_error(
        member_not_available const& info) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->set_member_not_available_error);
        (*PyWinRT_API->set_member_not_available_error)(info);
    }

    inline bool is_buffer_compatible(
        Py_buffer const& view, Py_ssize_t itemsize, const char* format) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->is_buffer_compatible);
        return (*PyWinRT_API->is_buffer_compatible)(view, itemsize, format);
    }

    inline PyObject* convert_datetime(
        winrt::Windows::Foundation::DateTime value) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->convert_datetime);
        return (*PyWinRT_API->convert_datetime)(value);
    }

    inline winrt::Windows::Foundation::DateTime convert_to_datetime(PyObject* obj)
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->convert_to_datetime);
        return (*PyWinRT_API->convert_to_datetime)(obj);
    }

    inline PyObject* convert_guid(winrt::guid value) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->convert_guid);
        return (*PyWinRT_API->convert_guid)(value);
    }

    inline winrt::guid convert_to_guid(PyObject* obj)
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->convert_to_guid);
        return (*PyWinRT_API->convert_to_guid)(obj);
    }

    inline PyTypeObject* get_inspectable_meta_type() noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->get_inspectable_meta_type);
        return (*PyWinRT_API->get_inspectable_meta_type)();
    }

    inline PyTypeObject* get_object_type() noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->get_object_type);
        return (*PyWinRT_API->get_object_type)();
    }

    inline PyObject* await_async(PyObject* obj) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->await_async);
        return (*PyWinRT_API->await_async)(obj);
    }

    inline winrt::Windows::Storage::Streams::IBuffer convert_to_ibuffer(PyObject* obj)
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->convert_to_ibuffer);
        return (*PyWinRT_API->convert_to_ibuffer)(obj);
    }

    namespace cpp::_winrt
    {
        inline PyObject* Array_New(std::unique_ptr<py::Array> array) noexcept
        {
            WINRT_ASSERT(PyWinRT_API && PyWinRT_API->array_new);
            return (*PyWinRT_API->array_new)(std::move(array));
        }

        inline bool Array_Assign(
            PyObject* obj, std::unique_ptr<py::Array> array) noexcept
        {
            WINRT_ASSERT(PyWinRT_API && PyWinRT_API->array_assign);
            return (*PyWinRT_API->array_assign)(obj, std::move(array));
        }
    } // namespace cpp::_winrt
#endif
} // namespace py
