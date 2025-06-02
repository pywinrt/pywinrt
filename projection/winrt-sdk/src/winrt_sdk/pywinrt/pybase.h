#pragma once

// These two headers are part of the Python C Extension API
#include <Python.h>
#include <datetime.h>
#include <structmember.h>
#include "pythoncapi_compat.h"
#include "pywinrt_version.h"

#define COM_NO_WINDOWS_H
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <unknwn.h>
#undef GetCurrentTime
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.Metadata.h>
#include <winrt/Windows.Storage.Streams.h>

static_assert(PY_VERSION_HEX >= 0x03090000, "Python 3.9 or later is required");

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
    struct empty_instance<T, std::void_t<decltype(T(nullptr))>>
    {
        static T get()
        {
            return T(nullptr);
        }
    };

    /**
     * Traits for use with winrt::handle_type.
     */
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

    /**
     * Type alias for Python GIL state handle.
     */
    using gil_handle = winrt::handle_type<gil_state_traits>;

    /**
     * Helper function for ensuring a block of code runs with the Python GIL held.
     */
    static inline auto ensure_gil()
    {
        return gil_handle{PyGILState_Ensure()};
    }

    /**
     * Traits for use with winrt::handle_type.
     */
    struct thread_state_traits
    {
        using type = PyThreadState*;

        static void close(type value) noexcept
        {
            PyEval_RestoreThread(value);
        }

        static constexpr type invalid() noexcept
        {
            return nullptr;
        }
    };

    /**
     * Type alias for Python thread state handle.
     */
    using thread_state_handle = winrt::handle_type<thread_state_traits>;

    /**
     * Helper function for ensuring a block of code runs with the Python GIL released.
     */
    static inline auto release_gil()
    {
        return thread_state_handle{PyEval_SaveThread()};
    }

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
    struct is_struct_category
    {
        static constexpr bool value = false;
    };

    template<typename... Fields>
    struct is_struct_category<winrt::impl::struct_category<Fields...>>
    {
        static constexpr bool value = true;
    };

    template<typename T>
    constexpr bool is_struct_category_v
        = is_struct_category<typename winrt::impl::category<T>::type>::value;

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
            auto gil = ensure_gil();
            Py_CLEAR(_callable);
        }

        PyObject* callable() const noexcept
        {
            return _callable;
        }

      private:
        PyObject* _callable{};
    };

    /**
     * Python PyObject struct for wrapping WinRT structs.
     */
    template<typename T>
    struct winrt_struct_wrapper
    {
        PyObject_HEAD;
        T obj{};
    };

    /**
     * Python PyObject struct for wrapping WinRT objects and interfaces.
     *
     * @tparam T   The WinRT type to wrap (e.g. `winrt::Windows::...`).
     */
    template<typename T>
    struct winrt_wrapper
    {
        PyObject_HEAD;
        /** The winrt object instance. */
        T obj{};
        static_assert(std::is_base_of_v<winrt::Windows::Foundation::IUnknown, T>);
    };

    /**
     * Python PyObject struct for wrapping WinRT parameterized interfaces.
     *
     * @tparam T   The abstract pinterface wrapper type to wrap (e.g.
     * `py::proj::Windows::...`).
     */
    template<typename T>
    struct winrt_pinterface_wrapper
        : winrt_wrapper<winrt::Windows::Foundation::IUnknown>
    {
        /** The PyWinRT member implementation for the concrete generic type. */
        std::unique_ptr<T> impl{};
    };

    /**
     * Generic WinRT array type (System.Array) implementation.
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
     * Type registration for pure Python type info.
     *
     * @tparam T    A winrt type name like `winrt::Windows::Foundation::PropertyType`.
     */
    template<typename T>
    struct py_type
    {
        static constexpr std::string_view qualified_name = {};
        static constexpr const char* module_name = 0;
        static constexpr const char* type_name = 0;
    };

    struct
#if WINRT_IMPL_HAS_DECLSPEC_UUID
        __declspec(uuid("9c3654bc-4adc-463a-b392-d6bc9289c925"))
#endif
        IPywinrtObject : ::IUnknown
    {
        virtual int32_t __stdcall GetPyObject(PyObject*&) = 0;
        virtual int32_t __stdcall GetComposableInner(
            winrt::Windows::Foundation::IInspectable&)
            = 0;
    };

    /**
     * Get the inner object if the object is a subclass of a composable type,
     * otherwise return the object itself.
     *
     * This is only used for calling overridable methods. When we have a Python
     * subclass of a composable type and we call super().overridable_method(),
     * we would get infinite recursion if we didn't get the inner object and use
     * that to call the method.
     */
    static inline winrt::Windows::Foundation::IInspectable get_inner_or_self(
        winrt::Windows::Foundation::IInspectable const& self) noexcept
    {
        if (auto pyobj = self.try_as<IPywinrtObject>())
        {
            winrt::Windows::Foundation::IInspectable inner{};
            if (auto ret = pyobj->GetComposableInner(inner))
            {
                winrt::throw_hresult(ret);
            }

            return inner;
        }

        return self;
    }

    struct ImplementsInterface;
    static inline void copy_to_abi(ImplementsInterface* obj, void*& abi) noexcept;

    /**
     * Base class for WinRT type wrapping Python subclass of composable type.
     */
    struct py_obj_ref
    {
      private:
        PyObject* py_obj;
        std::map<winrt::guid, std::unique_ptr<ImplementsInterface>> interfaces;
        std::vector<winrt::guid> interface_guids{};

      public:
        void add_interface(
            winrt::guid const& guid, std::unique_ptr<ImplementsInterface> iface)
        {
            interfaces.emplace(guid, std::move(iface));
            interface_guids.push_back(guid);
        }

      protected:
        py_obj_ref() = delete;

        py_obj_ref(PyObject* py_obj) : py_obj{py_obj}
        {
        }

        PyObject* get_py_obj() noexcept
        {
            return Py_NewRef(py_obj);
        }

        static void toggle_reference(py_obj_ref* obj, bool is_last_reference) noexcept
        {
            auto state = PyGILState_Ensure();

            if (is_last_reference)
            {
                // We hold the only WinRT reference - allow the Python object
                // to be GC'd
                PyObject_GC_Track(obj->py_obj);
                // This might be the last reference to the Python object, so
                // obj may be destroyed after this call and no longer valid!
                Py_DECREF(obj->py_obj);
            }
            else
            {
                // external WinRT code has a reference - don't allow Python
                // object to be GC'd
                Py_INCREF(obj->py_obj);
                PyObject_GC_UnTrack(obj->py_obj);
            }

            PyGILState_Release(state);
        }

        int32_t query_interface_tearoff(
            winrt::guid const& guid, void** obj) const noexcept
        {

            auto match = interfaces.find(guid);
            if (match != interfaces.end())
            {
                copy_to_abi(match->second.get(), *obj);
                return 0;
            }

            return winrt::impl::error_no_interface;
        }

        std::vector<winrt::guid> get_iids_tearoff() const noexcept
        {
            return interface_guids;
        }
    };

    struct ImplementsInterface
    {
      private:
        PyObject* py_obj;
        winrt::impl::inspectable_abi* runtime_class;
        void* interface_abi;

      public:
        ImplementsInterface() = delete;

        ImplementsInterface(
            PyObject* py_obj,
            winrt::impl::inspectable_abi* runtime_class,
            void* interface_abi)
            : py_obj{py_obj}, runtime_class(runtime_class), interface_abi(interface_abi)
        {
        }

        void* get_abi() const noexcept
        {
            return interface_abi;
        }

        int32_t __stdcall QueryInterface(winrt::guid const& id, void** object) noexcept
        {
            return runtime_class->QueryInterface(id, object);
        }

        uint32_t __stdcall AddRef() noexcept
        {
            return runtime_class->AddRef();
        }

        uint32_t __stdcall Release() noexcept
        {
            return runtime_class->Release();
        }

        int32_t __stdcall GetIids(
            winrt::impl::count_type* count, winrt::impl::guid_type** array) noexcept
        {
            return runtime_class->GetIids(
                reinterpret_cast<uint32_t*>(count),
                reinterpret_cast<winrt::guid**>(array));
        }

        int32_t __stdcall abi_GetRuntimeClassName(void** name) noexcept
        {
            return runtime_class->GetRuntimeClassName(name);
        }

        int32_t __stdcall abi_GetTrustLevel(
            winrt::Windows::Foundation::TrustLevel* trustLevel) noexcept
        {
            return runtime_class->GetTrustLevel(trustLevel);
        }

      protected:
        PyObject* get_py_obj() noexcept
        {
            return Py_NewRef(py_obj);
        }
    };

    static inline void copy_to_abi(ImplementsInterface* obj, void*& abi) noexcept
    {
        abi = obj->get_abi();

        if (abi)
        {
            obj->AddRef();
        }
    }

    template<typename D, typename I>
    struct ImplementsInterfaceT : ImplementsInterface, winrt::impl::producers<D, I>
    {
        struct abi_guard
        {
            PyGILState_STATE state;

            abi_guard(D& /*unused*/)
            {
                state = PyGILState_Ensure();
            }

            ~abi_guard()
            {
                PyGILState_Release(state);
            }
        };

        ImplementsInterfaceT() = delete;

        ImplementsInterfaceT(
            PyObject* py_obj, winrt::impl::inspectable_abi* runtime_class)
            : ImplementsInterface(py_obj, runtime_class, winrt::to_abi(this))
        {
        }
    };

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

    struct pytype_ptr_traits
    {
        using type = PyTypeObject*;

        static void close(type value) noexcept
        {
            Py_CLEAR(value);
        }

        static constexpr type invalid() noexcept
        {
            return nullptr;
        }
    };

    using pytype_handle = winrt::handle_type<pytype_ptr_traits>;

    // Runtime API shared with other modules

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
    const uint16_t runtime_abi_version_minor = 2;

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

    const winrt::hresult unraisable_python_exception{static_cast<int32_t>(0xA0EE4005)};

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
        PyErr_Format(PyExc_TypeError, "cannot create '%s' instances", type_name);
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
            // A Python exception is already pending - don't overwrite it
            return;
        }

        // otherwise convert C++ exception to Python exception
        try
        {
            throw;
        }
        catch (winrt::hresult_error const& e)
        {
            pyobj_handle exc{PyObject_CallFunction(
                PyExc_WindowsError,
                "iuui",
                0,                   // errno
                e.message().c_str(), // strerror
                nullptr,             // filename
                e.code().value)};    // winerror

            if (!exc)
            {
                // REVISIT: should we print something here so we don't loose the
                // info? Like: while raising an exception another error occurred...
                return;
            }

            PyErr_SetObject(reinterpret_cast<PyObject*>(Py_TYPE(exc.get())), exc.get());
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

        return get_python_type(py_type<winrt_type>::qualified_name);
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

        return reinterpret_cast<func_t>(
            get_struct_from_tuple_func(py_type<T>::from_tuple));
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
                typeid(T).name());
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
                typeid(T).name());
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

    template<typename T, typename = void>
    struct buffer
    {
        static bool is_compatible(Py_buffer const& /*unused*/) noexcept
        {
            PyErr_Format(
                PyExc_NotImplementedError,
                "py::buffer<%s>::is_compatible() is not implemented",
                typeid(T).name());
            return false;
        } // namespace py
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
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::IInspectable>
        = "P";

    template<typename T>
    inline constexpr const char*
        buffer_format<T, std::enable_if_t<is_class_category_v<T>>>
        = "P";

    template<typename T>
    inline constexpr const char*
        buffer_format<T, std::enable_if_t<is_interface_category_v<T>>>
        = "P";

    template<typename T>
    constexpr const char*
        buffer_format<T, std::enable_if_t<is_pinterface_category_v<T>>>
        = "P";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::DateTime>
        = "q";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Foundation::TimeSpan>
        = "q";

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

    [[noreturn]] inline void write_unraisable_and_throw()
    {
        PyErr_WriteUnraisable(nullptr);
        throw winrt::hresult_error(
            unraisable_python_exception, L"Unraisable Python exception");
    }

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
                    typeid(T).name());
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
                    typeid(T).name());
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
                    typeid(T).name());
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
                    typeid(K).name(),
                    typeid(V).name());
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
    inline constexpr bool is_key_value_pair_v
        = winrt::impl::is_key_value_pair<T>::value;

    template<typename T>
    struct key_value_pair_type;

    template<typename K, typename V>
    struct key_value_pair_type<
        winrt::Windows::Foundation::Collections::IKeyValuePair<K, V>>
    {
        using key_type = K;
        using value_type = V;
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

    template<typename TItem>
    struct is_specialized_interface<
        winrt::Windows::Foundation::Collections::IVectorView<TItem>> : std::true_type
    {
    };

    template<typename TItem>
    struct is_specialized_interface<
        winrt::Windows::Foundation::Collections::IVector<TItem>> : std::true_type
    {
    };

    template<typename TKey, typename TValue>
    struct is_specialized_interface<
        winrt::Windows::Foundation::Collections::IMapView<TKey, TValue>>
        : std::true_type
    {
    };

    template<typename TKey, typename TValue>
    struct is_specialized_interface<
        winrt::Windows::Foundation::Collections::IMap<TKey, TValue>> : std::true_type
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
        using typename winrt::array_view<T>::size_type;

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
            this->m_size = static_cast<size_type>(view.shape[0]);
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
        static PyObject* convert(pybuf_view<T, writeable> const& /*unused*/) noexcept
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
    struct converter<winrt::array_view<T>>
    {
        static PyObject* convert(winrt::array_view<T> const& instance) noexcept
        {
            // there is no way to know the scope of the lifetime of the
            // underlying data of the array_view so we need to make a copy
            // before sending to Python to ensure we don't introduce use after
            // free memory bugs

            using copy_t = std::remove_const_t<T>;
            auto copy = std::make_unique<ComArray<copy_t>>();
            copy->array = winrt::com_array<copy_t>(instance.begin(), instance.end());

            return cpp::_winrt::Array_New(std::move(copy));
        }

        static winrt::array_view<T> convert_to(PyObject* obj)
        {
            throw_if_pyobj_null(obj);

            PyErr_Format(
                PyExc_NotImplementedError,
                "py::converter<%s>::convert_to() is not implemented",
                typeid(T).name());
            throw python_exception();
        }
    };

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

            if (index < 0 || static_cast<uint32_t>(index) >= array.size())
            {
                PyErr_SetString(PyExc_IndexError, "index out of range");
                return false;
            }

            try
            {
                array[static_cast<winrt::array_view<T>::size_type>(index)]
                    = convert_to<T>(item);
                return true;
            }
            catch (...)
            {
                py::to_PyErr();
                return false;
            }
        }
    };

    template<typename T>
    static inline T::composable make_py_obj(
        winrt::com_ptr<T> const& obj_impl, PyTypeObject* py_type, PyObject* py_obj)
    {
        auto obj = obj_impl.template as<typename T::composable>();

        pyobj_handle bases_iter{PyObject_GetIter(py_type->tp_bases)};
        if (!bases_iter)
        {
            return nullptr;
        }

        for (;;)
        {
            pyobj_handle base{PyIter_Next(bases_iter.get())};
            if (!base)
            {
                if (PyErr_Occurred())
                {
                    return nullptr;
                }

                break;
            }

            pyobj_handle make{};
            auto ret = PyObject_GetOptionalAttrString(base.get(), "_make_", make.put());
            if (ret == -1)
            {
                return nullptr;
            }
            if (ret == 0)
            {
                continue;
            }

            pyobj_handle iface_obj{PyObject_CallFunction(
                make.get(), "On", py_obj, (*(winrt::impl::inspectable_abi**)&obj))};
            if (!iface_obj)
            {
                return nullptr;
            }

            std::unique_ptr<ImplementsInterface> iface{
                reinterpret_cast<ImplementsInterface*>(
                    PyLong_AsVoidPtr(iface_obj.get()))};
            if (!iface && PyErr_Occurred())
            {
                return nullptr;
            }

            pyobj_handle guid_obj{PyObject_CallMethod(base.get(), "_guid_", nullptr)};
            if (!guid_obj)
            {
                return nullptr;
            }

            auto guid = py::convert_to<winrt::guid>(guid_obj.get());

            obj_impl->add_interface(guid, std::move(iface));
        }

        return std::move(obj);
    }
} // namespace py

#if !WINRT_IMPL_HAS_DECLSPEC_UUID
__CRT_UUID_DECL(
    py::IPywinrtObject,
    // clang-format off
    0x9c3654bc, 0x4adc, 0x463a, 0xb3, 0x92, 0xd6, 0xbc, 0x92, 0x89, 0xc9, 0x25
);
#endif
