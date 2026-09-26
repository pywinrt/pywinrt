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
//     runtime_abi_version_major, resets the minor to 0 and replaces
//     runtime_api_guid, and every module built against an older header
//     refuses to load against the result.
//   - The types that cross the boundary are part of the contract too, even the
//     ones declared elsewhere. They are listed and, where the compiler can
//     check it, asserted in "the declared layouts" below.
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

    /// The member is reached without an instance, so a failure to reach it
    /// cannot be blamed on the object.
    inline constexpr uint32_t site_is_static = 1;
    /// The member is a constructor.
    inline constexpr uint32_t site_is_constructor = 2;

    /**
     * Identifies the projected member a call was made through, so that the
     * runtime can say which one failed without the calling module having to
     * work out the wording itself.
     *
     * Every field is either a scalar or a pointer to a string literal in the
     * calling module, so an instance of this is a @c static @c constexpr next
     * to the member it describes and costs nothing to pass.
     */
    struct member_site
    {
        member_kind kind;
        /// Number of WinRT input parameters, which is what distinguishes the
        /// overloads of a method from each other.
        uint32_t arg_count;
        /// Metadata name of the type that declares the member or @c nullptr for
        /// a parameterized interface, which has no name in the metadata.
        const char* type_name;
        const char* member_name;
        /// Metadata name of the interface that declares the member, or
        /// @c nullptr when it is not reached through one.
        const char* interface_name;
        /// Zero or more of the @c site_is_* flags above.
        uint32_t flags;
    };

    /**
     * What the calling module caught, which decides which Python exception the
     * runtime raises.
     */
    enum class error_kind
    {
        /// A C++/WinRT @c hresult_error. Only this kind fills in @c hresult,
        /// @c message and @c restricted_error_info.
        hresult,
        bad_alloc,
        out_of_range,
        invalid_argument,
        /// Any other @c std::exception.
        std_exception,
        /// Something that is not a @c std::exception at all.
        unknown,
    };

    /**
     * A C++ exception, flattened into something that can cross the boundary.
     *
     * The exception itself cannot: its layout comes from whichever C++/WinRT
     * and standard library headers the calling module was built with, and
     * matching exception types across modules is not something every supported
     * toolchain does the same way. So the module classifies its own exception
     * and hands over this instead, which is read only for the duration of the
     * call.
     */
    struct error_info
    {
        error_kind kind;
        /// The @c HRESULT, for @c error_kind::hresult.
        int32_t hresult;
        /// The error message, not necessarily null-terminated.
        const wchar_t* message;
        /// Length of @c message in characters.
        uint32_t message_length;
        /// Borrowed @c IRestrictedErrorInfo the error was originated with, or
        /// @c nullptr. Carried so that the runtime can report the details it
        /// holds without the modules having to be rebuilt to send them.
        void* restricted_error_info;
        /// @c std::exception::what(), or @c nullptr for @c error_kind::unknown.
        const char* what;
        /// The member that was being called, or @c nullptr when it is not known.
        member_site const* site;
    };

    /**
     * Hands @p handler to @p async as its completed handler.
     *
     * The implementation queries @p handler for the closed handler type of its
     * own async interface and calls @c Completed() with it, which is the only
     * part of a blocking wait that has to name the type arguments. It is a
     * captureless lambda in the calling module, so what crosses is an ordinary
     * function pointer.
     *
     * @returns an @c HRESULT, because nothing may throw across the boundary.
     * Called with the GIL released, like the wait that calls it.
     */
    using async_set_completed_fn = int32_t (*)(
        winrt::Windows::Foundation::IInspectable const& async,
        winrt::Windows::Foundation::IUnknown const& handler) noexcept;

    /**
     * A call that never reached WinRT at all, because of how it was made.
     */
    enum class call_error
    {
        /// No overload takes this many arguments. The count is passed
        /// alongside; -1 means a Python exception is already pending.
        invalid_arg_count,
        /// Keyword arguments were passed to a member that takes none.
        keyword_arguments,
        /// The type cannot be constructed from Python.
        cannot_instantiate,
    };

    // ----- the declared layouts ------------------------------------------
    //
    // The runtime_api struct below is only half of the contract. A projection
    // module lays out the objects it allocates with its own copy of these
    // headers, and both the runtime and the other projection modules read them
    // back, so the size and the field offsets are part of the major version
    // just as much as the entry points are. Everything that crosses is listed
    // here, and everything that can be checked at compile time is asserted, so
    // that a change to one of them is a build error rather than a mismatch that
    // shows up as a wrong answer at run time.
    //
    // What crosses:
    //
    //  - py::winrt_wrapper<T>, the layout of every wrapped class and interface
    //    instance. The runtime reads the obj field through _winrt.Object's
    //    slots (tp_dealloc, tp_hash, tp_richcompare, iids,
    //    runtime_class_name), and every module's wrapper types derive from
    //    _winrt.Object, which is why its tp_basicsize - py::object_basicsize
    //    below - is fixed. Modules also read each other's: converting any
    //    _winrt.Object to a C++/WinRT interface is a cast to
    //    winrt_wrapper<IUnknown>* whatever module allocated it.
    //  - py::winrt_struct_wrapper<T> and py::winrt_pinterface_wrapper<T>, for
    //    the same reason. A struct or a parameterized interface is owned by one
    //    module and unwrapped by all the others.
    //  - py::member_not_available, passed by reference to
    //    set_member_not_available_error(), and py::member_site and
    //    py::error_info, passed to set_call_error() and set_error(). The
    //    pointers in them are borrowed for the duration of the call: the names
    //    point at string literals in the calling module, and the message and
    //    the IRestrictedErrorInfo in an error_info belong to the exception
    //    object that the module is still standing in the catch block for.
    //  - The vtables: py::Array (Alloc, WinrtElementTypeName, Format, Size,
    //    ValueSize, Data, At, Set, then the destructor), which the module
    //    implements and the runtime calls; py::IPywinrtObject (IUnknown's
    //    three, then GetPyObject, GetComposableInner), which the module
    //    implements and both the runtime and the other modules query for; and
    //    the composable tearoffs in <pywinrt/compose.h>, which are ordinary
    //    WinRT interfaces and so are fixed by the metadata rather than by us.
    //    A vtable's slot order cannot be asserted, only kept.
    //  - The C++/WinRT types in the signatures below. They are passed by value,
    //    so their size is contract; the asserts pin the ones that are not
    //    obviously so.
    //  - py::async_set_completed_fn, the callback a blocking wait hands the
    //    runtime. It is a captureless lambda in the generated code, so what
    //    crosses is a plain function pointer, but its signature is contract
    //    like everything else here.
    //
    // What does not cross, although it looks like it might: py::delegate_callable
    // is held inside a module's own delegate implementations and never handed to
    // anyone, and py::py_obj_ref's interface map is private to the module that
    // built the composable object.

    /**
     * @c tp_basicsize of @c _winrt.Object, which every projection module's
     * wrapper types inherit.
     */
    inline constexpr size_t object_basicsize
        = sizeof(winrt_wrapper<winrt::Windows::Foundation::IUnknown>);

    static_assert(
        std::is_standard_layout_v<winrt_wrapper<winrt::Windows::Foundation::IUnknown>>);
    static_assert(object_basicsize == sizeof(PyObject) + sizeof(void*));
    static_assert(
        offsetof(winrt_wrapper<winrt::Windows::Foundation::IUnknown>, obj)
        == sizeof(PyObject));

    static_assert(std::is_standard_layout_v<
                  winrt_struct_wrapper<winrt::Windows::Foundation::DateTime>>);
    static_assert(
        offsetof(winrt_struct_wrapper<winrt::Windows::Foundation::DateTime>, obj)
        == sizeof(PyObject));

    namespace impl
    {
        /**
         * Stands in for the generated member implementation of a parameterized
         * interface in the layout assert below. Only the size of the
         * @c unique_ptr that holds it is contract, and that is the same for
         * every one of them.
         */
        struct pinterface_impl_probe
        {
        };
    } // namespace impl

    static_assert(
        sizeof(winrt_pinterface_wrapper<impl::pinterface_impl_probe>)
        == object_basicsize + sizeof(void*));

    static_assert(std::is_standard_layout_v<member_not_available>);
    static_assert(sizeof(member_kind) == sizeof(uint32_t));
    static_assert(offsetof(member_not_available, kind) == 0);
    static_assert(offsetof(member_not_available, arg_count) == sizeof(uint32_t));
    static_assert(offsetof(member_not_available, type_name) == 2 * sizeof(uint32_t));
    static_assert(
        offsetof(member_not_available, member_name)
        == 2 * sizeof(uint32_t) + sizeof(const char*));
    static_assert(
        offsetof(member_not_available, interface_name)
        == 2 * sizeof(uint32_t) + 2 * sizeof(const char*));
    static_assert(
        sizeof(member_not_available) == 2 * sizeof(uint32_t) + 3 * sizeof(const char*));

    // Both structs below put a uint32_t among pointers - member_site's flags
    // after the last of them, error_info's message_length between two - so a
    // 64-bit target pads it out to the next pointer and a 32-bit one does not.
    // That is what the alignof(const char*) term in these stands for: the
    // uint32_t together with whatever the compiler puts after it.

    static_assert(std::is_standard_layout_v<member_site>);
    static_assert(offsetof(member_site, kind) == 0);
    static_assert(offsetof(member_site, arg_count) == sizeof(uint32_t));
    static_assert(offsetof(member_site, type_name) == 2 * sizeof(uint32_t));
    static_assert(
        offsetof(member_site, member_name)
        == 2 * sizeof(uint32_t) + sizeof(const char*));
    static_assert(
        offsetof(member_site, interface_name)
        == 2 * sizeof(uint32_t) + 2 * sizeof(const char*));
    static_assert(
        offsetof(member_site, flags) == 2 * sizeof(uint32_t) + 3 * sizeof(const char*));
    static_assert(
        sizeof(member_site)
        == 2 * sizeof(uint32_t) + 3 * sizeof(const char*) + alignof(const char*));

    static_assert(std::is_standard_layout_v<error_info>);
    static_assert(sizeof(error_kind) == sizeof(uint32_t));
    static_assert(offsetof(error_info, kind) == 0);
    static_assert(offsetof(error_info, hresult) == sizeof(uint32_t));
    static_assert(offsetof(error_info, message) == 2 * sizeof(uint32_t));
    static_assert(
        offsetof(error_info, message_length)
        == 2 * sizeof(uint32_t) + sizeof(const char*));
    static_assert(
        offsetof(error_info, restricted_error_info)
        == 2 * sizeof(uint32_t) + sizeof(const char*) + alignof(const char*));
    static_assert(
        offsetof(error_info, what)
        == 2 * sizeof(uint32_t) + 2 * sizeof(const char*) + alignof(const char*));
    static_assert(
        offsetof(error_info, site)
        == 2 * sizeof(uint32_t) + 3 * sizeof(const char*) + alignof(const char*));
    static_assert(
        sizeof(error_info)
        == 2 * sizeof(uint32_t) + 4 * sizeof(const char*) + alignof(const char*));

    static_assert(sizeof(call_error) == sizeof(uint32_t));

    static_assert(sizeof(winrt::guid) == 16);
    static_assert(std::is_trivially_copyable_v<winrt::guid>);
    static_assert(sizeof(winrt::hstring) == sizeof(void*));
    static_assert(sizeof(winrt::Windows::Foundation::IUnknown) == sizeof(void*));
    static_assert(sizeof(winrt::Windows::Foundation::IInspectable) == sizeof(void*));
    static_assert(sizeof(winrt::Windows::Foundation::DateTime) == 8);
    static_assert(sizeof(winrt::Windows::Foundation::TimeSpan) == 8);

    /**
     * Identifies the layout of runtime_api, and is checked before anything
     * else in it is read.
     *
     * It changes with every major version, so that a module and a runtime of
     * different generations never pair. The version pair alone would not be
     * enough: the 3.x runtimes already declared ABI major 4, with a different
     * layout, and a newer module would have loaded against them and called
     * through the wrong slots.
     */
    const winrt::guid runtime_api_guid{"F7412A59-7226-4D6D-8BD5-597661C55196"};

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
    const uint16_t runtime_abi_version_minor = 0;

    PyTypeObject* register_python_type(
        PyObject* module,
        PyType_Spec* type_spec,
        PyObject* base_type,
        PyTypeObject* metaclass) noexcept;
    PyTypeObject* get_python_type(std::string_view qualified_name) noexcept;
    bool is_buffer_compatible(
        Py_buffer const& view, Py_ssize_t itemsize, const char* format) noexcept;
    PyObject* convert_datetime(winrt::Windows::Foundation::DateTime value) noexcept;
    winrt::Windows::Foundation::DateTime convert_to_datetime(PyObject* obj);
    PyObject* convert_guid(winrt::guid value) noexcept;
    winrt::guid convert_to_guid(PyObject* obj);
    PyTypeObject* get_object_type() noexcept;
    void set_member_not_available_error(member_not_available const& info) noexcept;
    void set_error(error_info const& info) noexcept;
    void set_call_error(
        call_error error, member_site const* site, Py_ssize_t arg_count) noexcept;
    int32_t report_unraisable() noexcept;
    void toggle_python_reference(PyObject* obj, bool is_last_reference) noexcept;
    int32_t async_wait(
        winrt::Windows::Foundation::IInspectable const& async,
        uint32_t timeout_ms,
        winrt::guid const& handler_iid,
        async_set_completed_fn set_completed) noexcept;

    // ----- values a module names rather than compiles -------------------
    //
    // A projection is a table the runtime interprets, so a module that
    // compiles against a WinRT type no longer shares generated code with the
    // package that projects it: it names the type and lets the runtime do the
    // rest. A type is named by the qualified Python name it is bound to.

    PyObject* wrap_object(
        winrt::Windows::Foundation::IInspectable const& value,
        char const* qualified_name) noexcept;
    bool unwrap_object(PyObject* obj, winrt::guid const& iid, void** result) noexcept;
    PyObject* struct_to_python(PyTypeObject* type, void const* value) noexcept;
    bool struct_from_python(PyTypeObject* type, PyObject* obj, void* out) noexcept;

    namespace cpp::_winrt
    {
        PyObject* Array_New(std::unique_ptr<py::Array> array) noexcept;
    } // namespace cpp::_winrt

    struct runtime_api
    {
        winrt::guid runtime_api_guid;
        uint16_t abi_version_major;
        uint16_t abi_version_minor;
        decltype(register_python_type)* register_python_type;
        decltype(get_python_type)* get_python_type;
        decltype(is_buffer_compatible)* is_buffer_compatible;
        decltype(convert_datetime)* convert_datetime;
        decltype(convert_to_datetime)* convert_to_datetime;
        decltype(convert_guid)* convert_guid;
        decltype(convert_to_guid)* convert_to_guid;
        decltype(get_object_type)* get_object_type;
        decltype(cpp::_winrt::Array_New)* array_new;
        decltype(set_member_not_available_error)* set_member_not_available_error;
        decltype(set_error)* set_error;
        decltype(set_call_error)* set_call_error;
        decltype(report_unraisable)* report_unraisable;
        decltype(toggle_python_reference)* toggle_python_reference;
        decltype(async_wait)* async_wait;
        decltype(wrap_object)* wrap_object;
        decltype(unwrap_object)* unwrap_object;
        decltype(struct_to_python)* struct_to_python;
        decltype(struct_from_python)* struct_from_python;
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
                "winrt._winrt._C_API ABI minor version mismatch: expected >= %d, got %d",
                runtime_abi_version_minor,
                PyWinRT_API->abi_version_minor);
            return -1;
        }

        return 0;
    }

    /// Stops a debug build of Python when a wrapper below is called in a
    /// module whose init did not call import_winrt_runtime(), which would
    /// otherwise crash on a null PyWinRT_API. It is empty in a release build.
    inline void assert_runtime_imported() noexcept
    {
#ifdef Py_DEBUG
        if (!PyWinRT_API)
        {
            Py_FatalError("import_winrt_runtime() was not called in module init");
        }
#endif
    }

    inline PyTypeObject* register_python_type(
        PyObject* module,
        PyType_Spec* type_spec,
        PyObject* base_type,
        PyTypeObject* metaclass) noexcept
    {
        assert_runtime_imported();
        return (*PyWinRT_API->register_python_type)(
            module, type_spec, base_type, metaclass);
    }

    inline PyTypeObject* get_python_type(std::string_view qualified_name) noexcept
    {
        assert_runtime_imported();
        return (*PyWinRT_API->get_python_type)(qualified_name);
    }

    inline void set_member_not_available_error(
        member_not_available const& info) noexcept
    {
        assert_runtime_imported();
        (*PyWinRT_API->set_member_not_available_error)(info);
    }

    inline void set_error(error_info const& info) noexcept
    {
        assert_runtime_imported();
        (*PyWinRT_API->set_error)(info);
    }

    inline void set_call_error(
        call_error error, member_site const* site, Py_ssize_t arg_count) noexcept
    {
        assert_runtime_imported();
        (*PyWinRT_API->set_call_error)(error, site, arg_count);
    }

    inline int32_t report_unraisable() noexcept
    {
        assert_runtime_imported();
        return (*PyWinRT_API->report_unraisable)();
    }

    inline void toggle_python_reference(PyObject* obj, bool is_last_reference) noexcept
    {
        assert_runtime_imported();
        (*PyWinRT_API->toggle_python_reference)(obj, is_last_reference);
    }

    inline bool is_buffer_compatible(
        Py_buffer const& view, Py_ssize_t itemsize, const char* format) noexcept
    {
        assert_runtime_imported();
        return (*PyWinRT_API->is_buffer_compatible)(view, itemsize, format);
    }

    inline PyObject* convert_datetime(
        winrt::Windows::Foundation::DateTime value) noexcept
    {
        assert_runtime_imported();
        return (*PyWinRT_API->convert_datetime)(value);
    }

    inline winrt::Windows::Foundation::DateTime convert_to_datetime(PyObject* obj)
    {
        assert_runtime_imported();
        return (*PyWinRT_API->convert_to_datetime)(obj);
    }

    inline PyObject* convert_guid(winrt::guid value) noexcept
    {
        assert_runtime_imported();
        return (*PyWinRT_API->convert_guid)(value);
    }

    inline winrt::guid convert_to_guid(PyObject* obj)
    {
        assert_runtime_imported();
        return (*PyWinRT_API->convert_to_guid)(obj);
    }

    inline PyTypeObject* get_object_type() noexcept
    {
        assert_runtime_imported();
        return (*PyWinRT_API->get_object_type)();
    }

    inline PyObject* wrap_object(
        winrt::Windows::Foundation::IInspectable const& value,
        char const* qualified_name) noexcept
    {
        assert_runtime_imported();
        return (*PyWinRT_API->wrap_object)(value, qualified_name);
    }

    inline bool unwrap_object(
        PyObject* obj, winrt::guid const& iid, void** result) noexcept
    {
        assert_runtime_imported();
        return (*PyWinRT_API->unwrap_object)(obj, iid, result);
    }

    inline PyObject* struct_to_python(PyTypeObject* type, void const* value) noexcept
    {
        assert_runtime_imported();
        return (*PyWinRT_API->struct_to_python)(type, value);
    }

    inline bool struct_from_python(
        PyTypeObject* type, PyObject* obj, void* out) noexcept
    {
        assert_runtime_imported();
        return (*PyWinRT_API->struct_from_python)(type, obj, out);
    }

    inline int32_t async_wait(
        winrt::Windows::Foundation::IInspectable const& async,
        uint32_t timeout_ms,
        winrt::guid const& handler_iid,
        async_set_completed_fn set_completed) noexcept
    {
        assert_runtime_imported();
        return (*PyWinRT_API->async_wait)(
            async, timeout_ms, handler_iid, set_completed);
    }

    namespace cpp::_winrt
    {
        inline PyObject* Array_New(std::unique_ptr<py::Array> array) noexcept
        {
            assert_runtime_imported();
            return (*PyWinRT_API->array_new)(std::move(array));
        }
    } // namespace cpp::_winrt
#endif
} // namespace py
