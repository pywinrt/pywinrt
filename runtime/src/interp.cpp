// Making one WinRT call from the descriptors that types.cpp built.
//
// Everything a generated thunk used to do in code, this does from data: it
// fills the argument buffer the trampoline reads, queries the object for the
// interface that declares the member, calls the vtable slot with the GIL
// released, and converts the outputs back. Which conversion to run is decided
// by the parameter's type code, which is the only thing the table says about
// it.
//
// The conversions themselves are the ones in <pywinrt/convert.h>, so a value
// crosses the boundary the same way here as it did through a thunk. What is new
// is the switch that picks one.

#include <Python.h>

#define PYWINRT_RUNTIME_MODULE
#include <pywinrt/base.h>

#include "arrays.h"
#include "compose.h"
#include "delegates.h"
#include "generics.h"
#include "interp.h"
#include "objects.h"
#include "structs.h"
#include "types.h"

namespace py::interp
{
    namespace
    {
        /**
         * How much of a call's working storage fits on the stack. A shape whose
         * arguments and outputs need more than this falls back to the heap; the
         * census says that is no member of any metadata the tree projects.
         */
        constexpr size_t inline_storage = 512;

        /**
         * How many arguments can be given back without allocating. A cleanup
         * entry is made by at most one parameter each, and the census says the
         * widest member takes seventeen.
         */
        constexpr size_t inline_cleanup = 20;

        /**
         * How many array arguments one call can borrow a Python buffer for.
         * The census says the widest member of any metadata the tree projects
         * passes or lends three.
         */
        constexpr size_t max_buffers = 4;

        /**
         * Something an argument conversion allocated that has to be given back
         * once the call is over.
         */
        struct cleanup_entry
        {
            enum class kind
            {
                /// An HSTRING that was created for a string argument.
                string,
                /// An interface pointer that was queried for an argument.
                interface_,
                /// A struct value that was built from a Python tuple and
                /// therefore owns the strings and pointers in it.
                struct_,
            };

            kind what;
            void* value;
            type_entry const* info;
        };

        /**
         * The working storage of one call: the argument buffer the trampoline
         * reads, the block the outputs are received into, and the list of
         * things to release afterwards.
         */
        struct call_frame
        {
            call_frame(size_t args_size, size_t out_size, size_t arg_count)
            {
                if (args_size + out_size > inline_storage)
                {
                    storage = std::make_unique<uint8_t[]>(args_size + out_size);
                    args = storage.get();
                }
                else
                {
                    args = inline_bytes;
                }

                std::memset(args, 0, args_size + out_size);
                out = args + args_size;

                if (arg_count > inline_cleanup)
                {
                    overflow.resize(arg_count);
                    cleanup = overflow.data();
                }
                else
                {
                    cleanup = inline_entries;
                }
            }

            call_frame(call_frame const&) = delete;
            call_frame& operator=(call_frame const&) = delete;

            ~call_frame()
            {
                for (size_t i = 0; i < buffer_count; i++)
                {
                    PyBuffer_Release(&buffers[i]);
                }

                for (size_t i = 0; i < cleanup_count; i++)
                {
                    auto const& entry = cleanup[i];

                    switch (entry.what)
                    {
                    case cleanup_entry::kind::string:
                        release_string(entry.value);
                        break;
                    case cleanup_entry::kind::interface_:
                        static_cast<::IUnknown*>(entry.value)->Release();
                        break;
                    case cleanup_entry::kind::struct_:
                        release_struct(*entry.info, entry.value);
                        break;
                    }
                }
            }

            void add(
                cleanup_entry::kind what, void* value, type_entry const* info) noexcept
            {
                cleanup[cleanup_count++] = {what, value, info};
            }

            /**
             * A Py_buffer to borrow an array argument into, released when the
             * call is over.
             *
             * @returns @c nullptr with a Python error set when the member
             * passes more arrays than a frame holds.
             */
            Py_buffer* borrow() noexcept
            {
                if (buffer_count == max_buffers)
                {
                    PyErr_SetString(
                        PyExc_NotImplementedError,
                        "the member passes more arrays than one call can borrow");
                    return nullptr;
                }

                return &buffers[buffer_count++];
            }

            uint8_t* args;
            uint8_t* out;

          private:
            alignas(std::max_align_t) uint8_t inline_bytes[inline_storage];
            std::unique_ptr<uint8_t[]> storage;
            cleanup_entry inline_entries[inline_cleanup];
            std::vector<cleanup_entry> overflow;
            cleanup_entry* cleanup;
            size_t cleanup_count{};
            // Filled in by borrow(), so only the count needs initialising.
            Py_buffer buffers[max_buffers];
            size_t buffer_count{};
        };

        void store_widened(uint8_t* buffer, uint16_t offset, uintptr_t value) noexcept
        {
            std::memcpy(buffer + offset, &value, sizeof(value));
        }

        template<typename T>
        void store(uint8_t* buffer, uint16_t offset, T value) noexcept
        {
            std::memcpy(buffer + offset, &value, sizeof(value));
        }

        template<typename T>
        T load(void const* storage) noexcept
        {
            T value;
            std::memcpy(&value, storage, sizeof(value));
            return value;
        }

        uintptr_t widen(int32_t value) noexcept
        {
            return static_cast<uintptr_t>(static_cast<intptr_t>(value));
        }

        /**
         * The integer behind a value whose Python type is built by calling it
         * with one: an enum member's number, an HRESULT, an event token.
         */
        PyObject* integer_of(table::type_code code, void const* storage) noexcept
        {
            switch (code)
            {
            case table::type_code::enum_u32:
                return convert(load<uint32_t>(storage));
            case table::type_code::event_token:
                return convert(load<int64_t>(storage));
            default:
                // enum32 and hresult, which are both a signed 32 bit integer.
                return convert(load<int32_t>(storage));
            }
        }

        /**
         * Names the member a call was made through, for the error path.
         */
        member_site make_site(
            member_desc const& member, overload_desc const& overload) noexcept
        {
            member_kind kind{};

            switch (member.kind)
            {
            case table::group_kind::property:
                kind = member_kind::property;
                break;
            case table::group_kind::event:
                kind = member_kind::event;
                break;
            default:
                kind = member_kind::method;
                break;
            }

            uint32_t flags{};

            if (member.is_static)
            {
                flags |= site_is_static;
            }

            if (member.kind == table::group_kind::constructor)
            {
                flags |= site_is_constructor;
            }

            return {
                kind,
                overload.in_count,
                member.type_name,
                overload.winrt_name,
                overload.iface_name,
                flags};
        }
    } // namespace

    /**
     * The entry that built @p entry's Python type.
     *
     * A parameterized instance belongs to whichever table first named it, and
     * every other table that names it holds an entry with the type it found
     * and nothing else. The type itself says which entry that is.
     */
    static type_entry* definition_of(type_entry& entry) noexcept
    {
        if (!entry.py_type)
        {
            return &entry;
        }

        auto* const definition = get_type_entry(entry.py_type);

        return definition ? definition : &entry;
    }

    /**
     * Reads the type entry that @p arg or @p field names, importing the package
     * that defines it if this is the first time it has been needed.
     *
     * @returns @c nullptr with a Python error set if it cannot be resolved.
     */
    type_entry* resolve(projection& owner, uint32_t type, type_entry*& cache) noexcept
    {
        if (cache)
        {
            return cache;
        }

        if (type == table::no_ref || type >= owner.types.size())
        {
            PyErr_SetString(PyExc_TypeError, "the table names no type for this value");
            return nullptr;
        }

        auto& entry = owner.types[type];
        if (entry.py_type || entry.reverse)
        {
            cache = definition_of(entry);
            return cache;
        }

        auto const record = owner.table->type(type);

        if (record.get_category() == table::category::delegate)
        {
            // A delegate is the one record that no Python type stands for -
            // the module binds a typing alias to its name - so the type
            // registry cannot answer for it and the entry itself is the
            // answer. A reference to a delegate of another namespace carries
            // only the name, so it is the defining table that has the Invoke
            // to call.
            cache = record.is_external() ? find_defining_entry(record.py_name())
                                         : ensure_entry(owner, type);

            return cache;
        }

        if (record.flags() & table::type_flags::concrete)
        {
            // A parameterized interface closed over the types this member
            // passes, which is a type record like any other except that
            // nothing binds it in a module, so it is built on first use.
            if (!ensure_instance_type(owner, entry, record))
            {
                return nullptr;
            }

            cache = definition_of(entry);
            return cache;
        }

        if (record.py_name().empty())
        {
            // A parameterized interface with a type argument still standing in
            // for a type - IVectorView<T> as IVector<T>'s own members mention
            // it - which is a record so that the instance a caller names can be
            // built from it, and is nothing a member can pass.
            PyErr_Format(
                PyExc_TypeError,
                "'%s.%s' has no type arguments, so no member can pass one",
                std::string{record.winrt_namespace()}.c_str(),
                std::string{record.name()}.c_str());
            return nullptr;
        }

        if (!record.is_external())
        {
            // A type this namespace defines that has not been built yet, which
            // happens while the module is still executing its __init__.py.
            std::string qualified_name{owner.module_name};
            qualified_name += '.';
            qualified_name += record.py_name();

            if (!find_registered_type(qualified_name))
            {
                if (!PyErr_Occurred())
                {
                    PyErr_Format(
                        PyExc_TypeError,
                        "'%s' is not a type this projection defines",
                        qualified_name.c_str());
                }

                return nullptr;
            }

            cache = &entry;
            return cache;
        }

        // An external reference: the package that defines it knows its layout,
        // so the answer is that package's entry rather than a copy of it here.
        auto const py_name = record.py_name();

        auto py_type = find_registered_type(py_name);
        if (!py_type)
        {
            if (PyErr_Occurred())
            {
                return nullptr;
            }

            py_type = get_python_type(py_name);
            if (!py_type)
            {
                return nullptr;
            }
        }

        if (auto const definition = get_type_entry(py_type))
        {
            cache = definition;
            return cache;
        }

        // An enum, or one of the winrt.system names: a real Python type that
        // this runtime did not build, so all that is kept is the type itself.
        entry.owner = &owner;
        entry.index = type;
        entry.py_type = py_type;
        entry.category = record.get_category();
        entry.winrt_name = record.name().data();

        cache = &entry;
        return cache;
    }

    /**
     * Where the value of an input argument goes.
     *
     * Ordinarily that is the argument's own slot in the buffer the trampoline
     * reads. When the ABI wants a pointer instead, the value goes in the block
     * the frame lays out beside the arguments - the same one the outputs are
     * received into, which is the only scratch a call has - and the slot gets
     * its address, which this fills in.
     */
    static uint8_t* in_storage(arg_desc const& arg, call_frame& frame) noexcept
    {
        if (!arg.by_reference)
        {
            return frame.args + arg.offset;
        }

        auto const storage = frame.out + arg.out_offset;
        store_widened(frame.args, arg.offset, reinterpret_cast<uintptr_t>(storage));

        return storage;
    }

    /**
     * Converts one Python value into the argument buffer.
     *
     * @throws python_exception on a conversion failure.
     */
    static void convert_in(
        projection& owner, arg_desc& arg, PyObject* value, call_frame& frame)
    {
        switch (arg.code)
        {
        case table::type_code::boolean:
            store_widened(frame.args, arg.offset, convert_to<bool>(value) ? 1 : 0);
            return;
        case table::type_code::int8:
            store_widened(frame.args, arg.offset, widen(convert_to<int8_t>(value)));
            return;
        case table::type_code::uint8:
            store_widened(frame.args, arg.offset, convert_to<uint8_t>(value));
            return;
        case table::type_code::int16:
            store_widened(frame.args, arg.offset, widen(convert_to<int16_t>(value)));
            return;
        case table::type_code::uint16:
            store_widened(frame.args, arg.offset, convert_to<uint16_t>(value));
            return;
        case table::type_code::char16:
            store_widened(frame.args, arg.offset, convert_to<char16_t>(value));
            return;
        case table::type_code::int32:
        case table::type_code::hresult:
            store_widened(frame.args, arg.offset, widen(convert_to<int32_t>(value)));
            return;
        case table::type_code::uint32:
            store_widened(frame.args, arg.offset, convert_to<uint32_t>(value));
            return;
        case table::type_code::int64:
            store<int64_t>(frame.args, arg.offset, convert_to<int64_t>(value));
            return;
        case table::type_code::uint64:
            store<int64_t>(
                frame.args,
                arg.offset,
                static_cast<int64_t>(convert_to<uint64_t>(value)));
            return;
        case table::type_code::single:
            store<float>(frame.args, arg.offset, convert_to<float>(value));
            return;
        case table::type_code::double_:
            store<double>(frame.args, arg.offset, convert_to<double>(value));
            return;
        case table::type_code::datetime:
            store<int64_t>(
                frame.args,
                arg.offset,
                convert_to<winrt::Windows::Foundation::DateTime>(value)
                    .time_since_epoch()
                    .count());
            return;
        case table::type_code::timespan:
            store<int64_t>(
                frame.args,
                arg.offset,
                convert_to<winrt::Windows::Foundation::TimeSpan>(value).count());
            return;
        case table::type_code::event_token:
            store<int64_t>(frame.args, arg.offset, convert_to<int64_t>(value));
            return;
        case table::type_code::guid:
        {
            auto const value_guid = convert_to<winrt::guid>(value);
            std::memcpy(in_storage(arg, frame), &value_guid, sizeof(value_guid));
            return;
        }
        case table::type_code::enum32:
        case table::type_code::enum_u32:
            store_widened(frame.args, arg.offset, widen(convert_to<int32_t>(value)));
            return;
        case table::type_code::string:
        {
            // The callee only borrows the HSTRING for the length of the call,
            // but it still has to be a real one: a string reference needs the
            // characters to stay put, and a Python string has no wchar_t buffer
            // that does.
            auto text = convert_to<winrt::hstring>(value);
            auto const abi = winrt::detach_abi(text);
            if (abi)
            {
                frame.add(cleanup_entry::kind::string, abi, nullptr);
            }

            store_widened(frame.args, arg.offset, reinterpret_cast<uintptr_t>(abi));
            return;
        }
        case table::type_code::object:
        {
            auto const abi = unwrap_abi(value, nullptr);
            if (abi)
            {
                frame.add(cleanup_entry::kind::interface_, abi, nullptr);
            }

            store_widened(frame.args, arg.offset, reinterpret_cast<uintptr_t>(abi));
            return;
        }
        case table::type_code::interface_:
        case table::type_code::class_:
        case table::type_code::generic:
        {
            auto const info = resolve(owner, arg.type, arg.info);
            if (!info)
            {
                throw python_exception();
            }

            if (!info->guid)
            {
                PyErr_Format(
                    PyExc_NotImplementedError,
                    "no interface is known for '%s'",
                    info->winrt_name);
                throw python_exception();
            }

            auto const abi = unwrap_abi(value, info->guid, info);
            if (abi)
            {
                frame.add(cleanup_entry::kind::interface_, abi, nullptr);
            }

            store_widened(frame.args, arg.offset, reinterpret_cast<uintptr_t>(abi));
            return;
        }
        case table::type_code::reference:
        {
            auto const info = resolve(owner, arg.type, arg.info);
            if (!info)
            {
                throw python_exception();
            }

            auto const abi = reference_from_python(*info, value);
            if (abi)
            {
                frame.add(cleanup_entry::kind::interface_, abi, nullptr);
            }

            store_widened(frame.args, arg.offset, reinterpret_cast<uintptr_t>(abi));
            return;
        }
        case table::type_code::struct_:
        {
            auto const info = resolve(owner, arg.type, arg.info);
            if (!info)
            {
                throw python_exception();
            }

            auto const borrowed = Py_IS_TYPE(value, info->py_type);
            auto const storage = in_storage(arg, frame);

            struct_from_python(*info, value, storage);

            if (info->owns_resources && !borrowed)
            {
                // Built from a tuple, so the strings in it were created here.
                frame.add(cleanup_entry::kind::struct_, storage, info);
            }

            return;
        }
        case table::type_code::delegate:
        {
            void* abi{};

            if (!Py_IsNone(value))
            {
                auto const info = resolve(owner, arg.type, arg.info);
                if (!info)
                {
                    throw python_exception();
                }

                // One that came out of WinRT goes back as itself rather than
                // as a Python callable wrapped in a second delegate.
                abi = Py_IS_TYPE(value, info->py_type) ? unwrap_abi(value, info->guid)
                                                       : make_delegate(*info, value);

                if (!abi)
                {
                    throw python_exception();
                }

                frame.add(cleanup_entry::kind::interface_, abi, nullptr);
            }

            store_widened(frame.args, arg.offset, reinterpret_cast<uintptr_t>(abi));
            return;
        }
        default:
            PyErr_Format(
                PyExc_NotImplementedError,
                "no conversion for type code %d",
                static_cast<int>(arg.code));
            throw python_exception();
        }
    }

    /**
     * Converts one value of @p arg's type from the ABI to Python, taking
     * ownership of whatever the ABI handed over.
     */
    PyObject* convert_out(projection& owner, arg_desc& arg, void* storage) noexcept
    {
        try
        {
            switch (arg.code)
            {
            case table::type_code::boolean:
                return convert(load<bool>(storage));
            case table::type_code::int8:
                return convert(load<int8_t>(storage));
            case table::type_code::uint8:
                return convert(load<uint8_t>(storage));
            case table::type_code::int16:
                return convert(load<int16_t>(storage));
            case table::type_code::uint16:
                return convert(load<uint16_t>(storage));
            case table::type_code::char16:
                return convert(load<char16_t>(storage));
            case table::type_code::int32:
                return convert(load<int32_t>(storage));
            case table::type_code::uint32:
                return convert(load<uint32_t>(storage));
            case table::type_code::int64:
                return convert(load<int64_t>(storage));
            case table::type_code::uint64:
                return convert(load<uint64_t>(storage));
            case table::type_code::single:
                return convert(load<float>(storage));
            case table::type_code::double_:
                return convert(load<double>(storage));
            case table::type_code::datetime:
                return convert(
                    winrt::Windows::Foundation::DateTime{
                        winrt::Windows::Foundation::TimeSpan{load<int64_t>(storage)}});
            case table::type_code::timespan:
                return convert(
                    winrt::Windows::Foundation::TimeSpan{load<int64_t>(storage)});
            case table::type_code::guid:
                return convert(load<winrt::guid>(storage));
            case table::type_code::string:
            {
                winrt::hstring text;
                winrt::attach_abi(text, load<void*>(storage));
                return convert(text);
            }
            case table::type_code::object:
            {
                winrt::Windows::Foundation::IInspectable value;
                winrt::attach_abi(value, load<void*>(storage));
                return convert(value);
            }
            case table::type_code::enum32:
            case table::type_code::enum_u32:
            case table::type_code::hresult:
            case table::type_code::event_token:
            {
                // Four codes whose ABI is an integer and whose Python value is
                // an instance of the type the record names: an enum member, an
                // HRESULT, an event token. The type is called with the integer,
                // which is what makes a member of the enum and what a subclass
                // of int is built from.
                auto const info = resolve(owner, arg.type, arg.info);
                if (!info)
                {
                    return nullptr;
                }

                pyobj_handle value{integer_of(arg.code, storage)};
                if (!value)
                {
                    return nullptr;
                }

                return PyObject_CallOneArg(
                    reinterpret_cast<PyObject*>(info->py_type), value.get());
            }
            case table::type_code::interface_:
            case table::type_code::class_:
            case table::type_code::delegate:
            case table::type_code::generic:
            {
                auto const abi = load<void*>(storage);
                if (!abi)
                {
                    Py_RETURN_NONE;
                }

                auto const info = resolve(owner, arg.type, arg.info);
                if (!info)
                {
                    static_cast<::IUnknown*>(abi)->Release();
                    return nullptr;
                }

                if (!info->py_type)
                {
                    static_cast<::IUnknown*>(abi)->Release();
                    PyErr_Format(
                        PyExc_NotImplementedError,
                        "'%s' has no Python type yet",
                        info->winrt_name);
                    return nullptr;
                }

                if (arg.query_interface)
                {
                    return wrap_activated_abi(info->py_type, info->guid, abi);
                }

                return wrap_abi(info->py_type, abi);
            }
            case table::type_code::reference:
            {
                auto const info = resolve(owner, arg.type, arg.info);
                if (!info)
                {
                    if (auto const abi = load<void*>(storage))
                    {
                        static_cast<::IUnknown*>(abi)->Release();
                    }

                    return nullptr;
                }

                return reference_to_python(*info, load<void*>(storage));
            }
            case table::type_code::struct_:
            {
                auto const info = resolve(owner, arg.type, arg.info);
                if (!info)
                {
                    return nullptr;
                }

                // The call handed the value over, so the wrapper takes what
                // is in it rather than duplicating it and releasing the
                // original.
                return struct_take_python(*info, storage);
            }
            default:
                PyErr_Format(
                    PyExc_NotImplementedError,
                    "no conversion for type code %d",
                    static_cast<int>(arg.code));
                return nullptr;
            }
        }
        catch (...)
        {
            to_PyErr();
            return nullptr;
        }
    }

    /**
     * Releases a value that was received but never converted, which is what a
     * failure part way through the outputs leaves behind, and what an element
     * of a WinRT array is when the array goes away.
     */
    void release_value(arg_desc const& arg, void* storage) noexcept
    {
        switch (arg.code)
        {
        case table::type_code::string:
            release_string(load<void*>(storage));
            break;
        case table::type_code::object:
        case table::type_code::interface_:
        case table::type_code::class_:
        case table::type_code::delegate:
        case table::type_code::generic:
        case table::type_code::reference:
            if (auto const abi = load<void*>(storage))
            {
                static_cast<::IUnknown*>(abi)->Release();
            }
            break;
        case table::type_code::struct_:
            if (arg.info)
            {
                release_struct(*arg.info, storage);
            }
            break;
        default:
            break;
        }
    }

    /**
     * Converts one of the values a call handed back, which is a value in
     * storage of its own or a whole array the callee allocated.
     */
    static PyObject* convert_result(
        projection& owner, arg_desc& arg, uint8_t* out) noexcept
    {
        if (arg.category == table::param_category::receive_array)
        {
            return array_take_python(
                owner, arg, *reinterpret_cast<array_out*>(out + arg.out_offset));
        }

        return convert_out(owner, arg, out + arg.out_offset);
    }

    /**
     * Releases an output that was received but never converted.
     */
    static void release_output(arg_desc& arg, uint8_t* out) noexcept
    {
        if (arg.category == table::param_category::receive_array)
        {
            release_array(arg, *reinterpret_cast<array_out*>(out + arg.out_offset));
            return;
        }

        release_value(arg, out + arg.out_offset);
    }

    /**
     * Lays out the block of storage a call's outputs are received into.
     *
     * An output whose type is a struct from a package that had not been
     * imported when the descriptor was built could not be measured then, so a
     * member with one is measured here instead, once.
     */
    static bool prepare_overload(projection& owner, overload_desc& overload) noexcept
    {
        uint32_t cursor = 0;
        uint32_t widest = 1;

        for (uint16_t i = 0; i < overload.arg_count; i++)
        {
            auto& arg = overload.args[i];
            if (!needs_storage(arg))
            {
                continue;
            }

            uint32_t size{};
            uint32_t align{};

            if (arg.category == table::param_category::receive_array)
            {
                // The count and the elements, which the two array slots point
                // at; what the elements are does not change what receives
                // them.
                size = sizeof(array_out);
                align = alignof(array_out);
            }
            else if (arg.code == table::type_code::struct_)
            {
                auto const info = resolve(owner, arg.type, arg.info);
                if (!info)
                {
                    return false;
                }

                size = info->size;
                align = info->align;
            }
            else
            {
                try
                {
                    auto const layout = table::get_value_layout(arg.code);
                    size = layout.size;
                    align = layout.align;
                }
                catch (...)
                {
                    to_PyErr();
                    return false;
                }
            }

            cursor = align_up(cursor, align);
            arg.out_offset = static_cast<uint16_t>(cursor);
            cursor += size;
            widest = std::max(widest, align);
        }

        overload.out_size = static_cast<uint16_t>(align_up(cursor, widest));
        overload.prepared = true;

        return true;
    }

    /**
     * Picks the overload of @p member that takes @p nargs Python arguments.
     *
     * @returns @c nullptr with a Python error set when none does.
     */
    overload_desc* select_overload(member_desc const& member, Py_ssize_t nargs) noexcept
    {
        for (uint16_t i = 0; i < member.count; i++)
        {
            if (member.overloads[i].in_count == nargs)
            {
                return &member.overloads[i];
            }
        }

        set_invalid_arg_count_error(nargs);
        return nullptr;
    }

    /**
     * Gets the activation factory a static member or a constructor is reached
     * through, keeping it when it turns out to be agile.
     *
     * A factory that is not agile belongs to the apartment that asked for it,
     * so it cannot be handed to the next caller; C++/WinRT's own factory cache
     * makes the same distinction.
     */
    static void* get_factory(type_entry const& type, overload_desc& overload)
    {
        if (auto const cached = overload.factory)
        {
            static_cast<::IUnknown*>(cached)->AddRef();
            return cached;
        }

        void* factory{};
        winrt::check_hresult(
            winrt::impl::get_runtime_activation_factory_impl<false>(
                winrt::param::hstring{type.class_name},
                *static_cast<winrt::guid const*>(overload.iface),
                &factory));

        void* agile{};

        if (static_cast<::IUnknown*>(factory)->QueryInterface(
                winrt::guid_of<winrt::impl::IAgileObject>(), &agile)
            == 0)
        {
            static_cast<::IUnknown*>(agile)->Release();
            static_cast<::IUnknown*>(factory)->AddRef();

            if (_InterlockedCompareExchangePointer(
                    const_cast<void* volatile*>(&overload.factory), factory, nullptr))
            {
                static_cast<::IUnknown*>(factory)->Release();
            }
        }

        return factory;
    }

    /**
     * Queries @p self for the interface that declares @p overload, which a
     * member reached through anything but the interface the wrapper holds has
     * to be called on.
     *
     * Asking here is what turns a call on an object that does not implement
     * the interface into a Python exception instead of a call through a null
     * vtable.
     *
     * @returns A pointer the caller owns a reference to.
     * @throws python_exception if @p self does not implement it.
     */
    static void* query_member(
        member_desc const& member, overload_desc const& overload, void* self)
    {
        winrt::com_ptr<::IUnknown> inner;

        if (overload.overridable)
        {
            // An object a Python class was composed into answers this member
            // from Python, so this is what super() reaches through: the object
            // it was composed over, which is where the implementation the
            // subclass overrode lives. An object that was not composed has no
            // inner object and answers for itself.
            inner.attach(static_cast<::IUnknown*>(composable_inner(self)));

            if (inner)
            {
                self = inner.get();
            }
        }

        void* iface{};

        if (static_cast<::IUnknown*>(self)->QueryInterface(
                *static_cast<winrt::guid const*>(overload.iface), &iface)
            != 0)
        {
            throw_member_not_available(
                member.kind == table::group_kind::property ? member_kind::property
                                                           : member_kind::method,
                member.type_name,
                overload.winrt_name,
                overload.iface_name,
                overload.in_count);
        }

        return iface;
    }

    /**
     * Calls @p member with @p args, on @p self for an instance member or on the
     * activation factory for a static or a constructor.
     *
     * @param self The ABI pointer the instance wrapper holds, or @c nullptr.
     * @param compose The object a Python subclass is being composed into, when
     * @p member is the constructor that does it, and @c nullptr otherwise.
     * @returns A new reference to the result - @c None, one value, or a tuple
     * of the outputs in declaration order - or @c nullptr with a Python error
     * set.
     */
    PyObject* call_member(
        member_desc const& member,
        overload_desc& overload,
        void* self,
        PyObject* const* args,
        Py_ssize_t /*nargs*/,
        composing* compose) noexcept
    {
        auto const shape = overload.shape;

        if (!shape)
        {
            PyErr_Format(
                PyExc_TypeError,
                "'%s' is a member of '%s', which has no type arguments, so there "
                "is nothing to call it on",
                overload.winrt_name,
                member.type_name);
            return nullptr;
        }

        if (!overload.prepared && !prepare_overload(*member.owner, overload))
        {
            return nullptr;
        }

        winrt::com_ptr<::IUnknown> queried;

        try
        {
            if (member.is_static || member.kind == table::group_kind::constructor)
            {
                queried.attach(
                    static_cast<::IUnknown*>(get_factory(*member.declaring, overload)));
            }
            else if (overload.iface)
            {
                queried.attach(
                    static_cast<::IUnknown*>(query_member(member, overload, self)));
            }

            auto const instance
                = queried ? queried.get() : static_cast<::IUnknown*>(self);

            call_frame frame{shape->buffer_size, overload.out_size, overload.arg_count};

            Py_ssize_t next_arg = 0;

            for (uint16_t i = 0; i < overload.arg_count; i++)
            {
                auto& arg = overload.args[i];
                if (arg.category == table::param_category::in)
                {
                    if (arg.is_implicit)
                    {
                        // The outer object a Python subclass is composed into,
                        // which an exact type does not have.
                        store_widened(
                            frame.args,
                            arg.offset,
                            reinterpret_cast<uintptr_t>(
                                compose ? compose->outer : nullptr));
                        continue;
                    }

                    convert_in(*member.owner, arg, args[next_arg++], frame);
                    continue;
                }

                if (arg.category == table::param_category::out)
                {
                    store_widened(
                        frame.args,
                        arg.offset,
                        reinterpret_cast<uintptr_t>(frame.out + arg.out_offset));
                    continue;
                }

                if (arg.category == table::param_category::receive_array)
                {
                    point_at_array_output(
                        arg,
                        *reinterpret_cast<array_out*>(frame.out + arg.out_offset),
                        frame.args);
                    continue;
                }

                // A passed or a lent array: the callee reads, and for a lent
                // one writes, the memory the Python object exports, so it is
                // borrowed for as long as the call lasts rather than copied.
                auto* const view = frame.borrow();

                if (!view)
                {
                    throw python_exception();
                }

                if (!borrow_array_argument(
                        *member.owner, arg, args[next_arg++], view, frame.args))
                {
                    throw python_exception();
                }
            }

            auto const vtable = *reinterpret_cast<void* const* const*>(instance);
            auto const entry = vtable[overload.slot];

            int32_t hr{};

            {
                auto _gil = release_gil();
                hr = shape->invoke(entry, instance, frame.args);
            }

            if (hr != 0)
            {
                auto const site = make_site(member, overload);

                try
                {
                    winrt::check_hresult(hr);
                }
                catch (...)
                {
                    to_PyErr(&site);
                    return nullptr;
                }
            }

            if (overload.out_count == 0)
            {
                Py_RETURN_NONE;
            }

            // The return value comes first and the declared outputs follow in
            // order, which is what the stubs say and what the generated thunks
            // packed - the metadata puts the return value last, so this is the
            // one place the two orders differ.
            pyobj_handle result{};

            if (overload.out_count != 1)
            {
                result.attach(PyTuple_New(overload.out_count));

                if (!result)
                {
                    for (uint16_t i = 0; i < overload.arg_count; i++)
                    {
                        auto& arg = overload.args[i];
                        if (is_output(arg))
                        {
                            release_output(arg, frame.out);
                        }
                    }

                    return nullptr;
                }
            }

            Py_ssize_t next_out = 0;

            if (overload.out_count != 1)
            {
                // Where the declared outputs start, which is after the return
                // value when there is one.
                for (uint16_t i = 0; i < overload.arg_count; i++)
                {
                    auto const& arg = overload.args[i];

                    if (is_output(arg) && arg.is_return)
                    {
                        next_out = 1;
                        break;
                    }
                }
            }

            for (uint16_t i = 0; i < overload.arg_count; i++)
            {
                auto& arg = overload.args[i];
                if (!is_output(arg))
                {
                    continue;
                }

                if (arg.is_implicit)
                {
                    // The non-delegating inner object a composable factory
                    // hands back, which only a Python subclass keeps.
                    auto const inner = load<void*>(frame.out + arg.out_offset);

                    if (compose)
                    {
                        compose->inner = inner;
                    }
                    else if (inner)
                    {
                        static_cast<::IUnknown*>(inner)->Release();
                    }

                    continue;
                }

                pyobj_handle value{convert_result(*member.owner, arg, frame.out)};
                if (!value)
                {
                    // Whatever has not been converted yet is still owned here.
                    for (uint16_t j = i + 1; j < overload.arg_count; j++)
                    {
                        auto& rest = overload.args[j];
                        if (is_output(rest))
                        {
                            release_output(rest, frame.out);
                        }
                    }

                    return nullptr;
                }

                if (overload.out_count == 1)
                {
                    return value.detach();
                }

                PyTuple_SET_ITEM(
                    result.get(), arg.is_return ? 0 : next_out++, value.detach());
            }

            return result.detach();
        }
        catch (...)
        {
            auto const site = make_site(member, overload);
            to_PyErr(&site);
            return nullptr;
        }
    }
} // namespace py::interp
