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
    //  - The type registry epoch, which crosses as an address rather than as a
    //    call: the runtime owns the counter, every module holds a pointer to
    //    it for the lifetime of the process, and only the runtime writes it.
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
    const uint16_t runtime_abi_version_minor = 7;

    PyTypeObject* register_python_type(
        PyObject* module,
        PyType_Spec* type_spec,
        PyObject* base_type,
        PyTypeObject* metaclass) noexcept;
    PyTypeObject* get_python_type(std::string_view qualified_name) noexcept;
    void* get_struct_from_tuple_func(std::string_view capsule_name) noexcept;

    /**
     * How many times the type registry that get_python_type() and
     * get_struct_from_tuple_func() answer from has been created or torn down.
     *
     * Both of those look their argument up by string on every call, so
     * <pywinrt/convert.h> remembers what they said. What a memo is good for is
     * one registry: the values in it are borrowed from the registry, which
     * builds its own set of wrapper types for each interpreter that imports
     * the projection and drops them when that interpreter is finalized. A memo
     * therefore names the interpreter it was taken in - two live interpreters
     * have different wrapper types for the same WinRT type - and the value of
     * this counter, which changes whenever a registry appears or goes away and
     * so catches the interpreter that was finalized and replaced at the same
     * address.
     *
     * This is a counter rather than a call because it is read on every
     * conversion, which is what the memo exists to make cheap: the capsule
     * carries its address and a module reads it directly.
     */
    uint64_t get_type_registry_epoch() noexcept;
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

    // ----- the Python-backed collection callbacks -------------------------
    //
    // A Python list or dict passed to a WinRT method that takes an IVector<T>
    // or an IMap<K, V> is wrapped rather than copied, so WinRT calls back into
    // Python for every element operation it makes. The implementations of
    // those callbacks are in <pywinrt/collections.h>, and everything in them
    // that does not name T - the Python C API call, the error policy, the
    // GetMany() loops - is one of the entries below, so that only the
    // per-element conversion is left in the calling module.
    //
    // All of them:
    //
    //  - are called with the GIL held. The caller takes it once, because WinRT
    //    may call on any thread, and nothing here takes it again.
    //  - borrow the PyObject* they are given and return new references through
    //    their out parameters, which are written on success only.
    //  - return an HRESULT, because nothing throws across the boundary. A
    //    Python IndexError or KeyError means the collection has no such index
    //    or key and becomes E_BOUNDS, which is what WinRT expects from GetAt()
    //    or Lookup(). Nothing on the WinRT side could catch any other Python
    //    exception, so it is reported with report_unraisable() and returned as
    //    that HRESULT.

    /// Number of items in @p sequence, for IVector<T>::Size().
    int32_t pyseq_size(PyObject* sequence, uint32_t* size) noexcept;
    /// The item at @p index, for IVector<T>::GetAt().
    int32_t pyseq_get_at(PyObject* sequence, uint32_t index, PyObject** item) noexcept;
    /// Replaces the item at @p index, for IVector<T>::SetAt().
    int32_t pyseq_set_at(PyObject* sequence, uint32_t index, PyObject* item) noexcept;
    /// Inserts @p item before @p index, for IVector<T>::InsertAt().
    int32_t pyseq_insert_at(
        PyObject* sequence, uint32_t index, PyObject* item) noexcept;
    /// Removes the item at @p index, for IVector<T>::RemoveAt().
    int32_t pyseq_remove_at(PyObject* sequence, uint32_t index) noexcept;
    /// Adds @p item to the end, for IVector<T>::Append().
    int32_t pyseq_append(PyObject* sequence, PyObject* item) noexcept;
    /// Removes the last item, for IVector<T>::RemoveAtEnd().
    int32_t pyseq_remove_at_end(PyObject* sequence) noexcept;
    /// Finds @p item, for IVector<T>::IndexOf(). Not finding it is a success
    /// with @p found false, since a WinRT IndexOf() reports it that way.
    int32_t pyseq_index_of(
        PyObject* sequence, PyObject* item, uint32_t* index, bool* found) noexcept;
    /// Removes every item, for IVector<T>::Clear().
    int32_t pyseq_clear(PyObject* sequence) noexcept;
    /// Starts an iteration of @p iterable, for IIterable<T>::First().
    int32_t pyiter_first(PyObject* iterable, PyObject** iterator) noexcept;
    /// Advances @p iterator, for IIterator<T>::MoveNext(). The end of the
    /// iteration is a success with a null @p item.
    int32_t pyiter_next(PyObject* iterator, PyObject** item) noexcept;
    /// Number of entries in @p mapping, for IMap<K, V>::Size().
    int32_t pymap_size(PyObject* mapping, uint32_t* size) noexcept;
    /// The value @p key maps to, for IMap<K, V>::Lookup().
    int32_t pymap_lookup(PyObject* mapping, PyObject* key, PyObject** value) noexcept;
    /// Whether @p key is in @p mapping, for IMap<K, V>::HasKey().
    int32_t pymap_has_key(PyObject* mapping, PyObject* key, bool* has_key) noexcept;
    /// Maps @p key to @p value, for IMap<K, V>::Insert(), which reports
    /// through @p replaced whether the key was already there.
    int32_t pymap_insert(
        PyObject* mapping, PyObject* key, PyObject* value, bool* replaced) noexcept;
    /// Removes @p key, for IMap<K, V>::Remove().
    int32_t pymap_remove(PyObject* mapping, PyObject* key) noexcept;
    /// Removes every entry, for IMap<K, V>::Clear().
    int32_t pymap_clear(PyObject* mapping) noexcept;
    /// Advances @p iterator over the keys of @p mapping and looks the value
    /// up, for the IKeyValuePair<K, V> iterator of a mapping. The end of the
    /// iteration is a success with a null @p key and @p value.
    int32_t pymap_iter_next(
        PyObject* mapping,
        PyObject* iterator,
        PyObject** key,
        PyObject** value) noexcept;

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
        decltype(set_error)* set_error;
        decltype(set_call_error)* set_call_error;
        decltype(report_unraisable)* report_unraisable;
        decltype(toggle_python_reference)* toggle_python_reference;
        decltype(async_wait)* async_wait;
        decltype(pyseq_size)* pyseq_size;
        decltype(pyseq_get_at)* pyseq_get_at;
        decltype(pyseq_set_at)* pyseq_set_at;
        decltype(pyseq_insert_at)* pyseq_insert_at;
        decltype(pyseq_remove_at)* pyseq_remove_at;
        decltype(pyseq_append)* pyseq_append;
        decltype(pyseq_remove_at_end)* pyseq_remove_at_end;
        decltype(pyseq_index_of)* pyseq_index_of;
        decltype(pyseq_clear)* pyseq_clear;
        decltype(pyiter_first)* pyiter_first;
        decltype(pyiter_next)* pyiter_next;
        decltype(pymap_size)* pymap_size;
        decltype(pymap_lookup)* pymap_lookup;
        decltype(pymap_has_key)* pymap_has_key;
        decltype(pymap_insert)* pymap_insert;
        decltype(pymap_remove)* pymap_remove;
        decltype(pymap_clear)* pymap_clear;
        decltype(pymap_iter_next)* pymap_iter_next;
        /// The counter behind get_type_registry_epoch(), which a module reads
        /// rather than calls. It lives in the runtime and is never written by
        /// a module.
        const uint64_t* type_registry_epoch;
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

    inline uint64_t get_type_registry_epoch() noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->type_registry_epoch);
        return *PyWinRT_API->type_registry_epoch;
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

    inline void set_error(error_info const& info) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->set_error);
        (*PyWinRT_API->set_error)(info);
    }

    inline void set_call_error(
        call_error error, member_site const* site, Py_ssize_t arg_count) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->set_call_error);
        (*PyWinRT_API->set_call_error)(error, site, arg_count);
    }

    inline int32_t report_unraisable() noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->report_unraisable);
        return (*PyWinRT_API->report_unraisable)();
    }

    inline void toggle_python_reference(PyObject* obj, bool is_last_reference) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->toggle_python_reference);
        (*PyWinRT_API->toggle_python_reference)(obj, is_last_reference);
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

    inline int32_t async_wait(
        winrt::Windows::Foundation::IInspectable const& async,
        uint32_t timeout_ms,
        winrt::guid const& handler_iid,
        async_set_completed_fn set_completed) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->async_wait);
        return (*PyWinRT_API->async_wait)(
            async, timeout_ms, handler_iid, set_completed);
    }

    inline int32_t pyseq_size(PyObject* sequence, uint32_t* size) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->pyseq_size);
        return (*PyWinRT_API->pyseq_size)(sequence, size);
    }

    inline int32_t pyseq_get_at(
        PyObject* sequence, uint32_t index, PyObject** item) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->pyseq_get_at);
        return (*PyWinRT_API->pyseq_get_at)(sequence, index, item);
    }

    inline int32_t pyseq_set_at(
        PyObject* sequence, uint32_t index, PyObject* item) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->pyseq_set_at);
        return (*PyWinRT_API->pyseq_set_at)(sequence, index, item);
    }

    inline int32_t pyseq_insert_at(
        PyObject* sequence, uint32_t index, PyObject* item) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->pyseq_insert_at);
        return (*PyWinRT_API->pyseq_insert_at)(sequence, index, item);
    }

    inline int32_t pyseq_remove_at(PyObject* sequence, uint32_t index) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->pyseq_remove_at);
        return (*PyWinRT_API->pyseq_remove_at)(sequence, index);
    }

    inline int32_t pyseq_append(PyObject* sequence, PyObject* item) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->pyseq_append);
        return (*PyWinRT_API->pyseq_append)(sequence, item);
    }

    inline int32_t pyseq_remove_at_end(PyObject* sequence) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->pyseq_remove_at_end);
        return (*PyWinRT_API->pyseq_remove_at_end)(sequence);
    }

    inline int32_t pyseq_index_of(
        PyObject* sequence, PyObject* item, uint32_t* index, bool* found) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->pyseq_index_of);
        return (*PyWinRT_API->pyseq_index_of)(sequence, item, index, found);
    }

    inline int32_t pyseq_clear(PyObject* sequence) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->pyseq_clear);
        return (*PyWinRT_API->pyseq_clear)(sequence);
    }

    inline int32_t pyiter_first(PyObject* iterable, PyObject** iterator) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->pyiter_first);
        return (*PyWinRT_API->pyiter_first)(iterable, iterator);
    }

    inline int32_t pyiter_next(PyObject* iterator, PyObject** item) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->pyiter_next);
        return (*PyWinRT_API->pyiter_next)(iterator, item);
    }

    inline int32_t pymap_size(PyObject* mapping, uint32_t* size) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->pymap_size);
        return (*PyWinRT_API->pymap_size)(mapping, size);
    }

    inline int32_t pymap_lookup(
        PyObject* mapping, PyObject* key, PyObject** value) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->pymap_lookup);
        return (*PyWinRT_API->pymap_lookup)(mapping, key, value);
    }

    inline int32_t pymap_has_key(
        PyObject* mapping, PyObject* key, bool* has_key) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->pymap_has_key);
        return (*PyWinRT_API->pymap_has_key)(mapping, key, has_key);
    }

    inline int32_t pymap_insert(
        PyObject* mapping, PyObject* key, PyObject* value, bool* replaced) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->pymap_insert);
        return (*PyWinRT_API->pymap_insert)(mapping, key, value, replaced);
    }

    inline int32_t pymap_remove(PyObject* mapping, PyObject* key) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->pymap_remove);
        return (*PyWinRT_API->pymap_remove)(mapping, key);
    }

    inline int32_t pymap_clear(PyObject* mapping) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->pymap_clear);
        return (*PyWinRT_API->pymap_clear)(mapping);
    }

    inline int32_t pymap_iter_next(
        PyObject* mapping,
        PyObject* iterator,
        PyObject** key,
        PyObject** value) noexcept
    {
        WINRT_ASSERT(PyWinRT_API && PyWinRT_API->pymap_iter_next);
        return (*PyWinRT_API->pymap_iter_next)(mapping, iterator, key, value);
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
