// What the runtime's translation units share about calling into each other:
// the types a failed call is reported with, and the entry points the other
// headers here call.
//
// Nothing outside winrt-runtime is compiled against these any more. A
// projection package is a table, and an interop module exchanges objects with
// the runtime through the Python functions in winrt._winrt, so none of this is
// a contract with another module and all of it may change freely.

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

    /**
     * @c tp_basicsize of @c _winrt.Object, which every wrapper type the runtime
     * builds from a table inherits.
     */
    inline constexpr size_t object_basicsize
        = sizeof(winrt_wrapper<winrt::Windows::Foundation::IUnknown>);

    static_assert(
        std::is_standard_layout_v<winrt_wrapper<winrt::Windows::Foundation::IUnknown>>);
    static_assert(object_basicsize == sizeof(PyObject) + sizeof(void*));
    static_assert(
        offsetof(winrt_wrapper<winrt::Windows::Foundation::IUnknown>, obj)
        == sizeof(PyObject));

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

    // ----- values named by their Python type -------------------------------
    //
    // A type is named by the qualified Python name it is bound to, which is
    // how winrt._winrt.wrap_interface() is told what to wrap.

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
} // namespace py
