// Passing, lending and receiving a WinRT array from a type code.
//
// The elements a caller passes or lends are never copied: they are borrowed
// from a Python object that exports a buffer whose format and item size match
// the element type exactly, which is how the generated projection read one
// too. Only the elements a call hands back are owned here, and they are owned
// the way winrt::com_array owns them - allocated by CoTaskMemAlloc, each
// string and interface pointer released one by one - because that is what the
// callee allocated them as.
//
// The element conversions themselves are the ones every other value goes
// through, in interp.cpp and callbacks.cpp. What this file adds is the buffer
// format and the element width that the table only says implicitly, and a
// py::Array whose element type is a type code.
//
// The allocator is the one winrt::com_array uses, spelled the way C++/WinRT
// spells it, because an array crossing the ABI has to be freed by whichever
// side did not allocate it and CoTaskMemAlloc is what that means.

#include <Python.h>

#define PYWINRT_RUNTIME_MODULE
#include <pywinrt/base.h>

#include "_winrt_array.h"
#include "arrays.h"
#include "callbacks.h"
#include "interp.h"
#include "structs.h"
#include "types.h"

#include <memory>
#include <string>

namespace py::interp
{
    namespace
    {
        /**
         * The PEP 3118 format of a value that is not a struct, or @c nullptr
         * for one that is: a struct's format is built from its fields.
         */
        char const* scalar_format(table::type_code code) noexcept
        {
            switch (code)
            {
            case table::type_code::boolean:
                return "?";
            case table::type_code::int8:
                return "b";
            case table::type_code::uint8:
                return "B";
            case table::type_code::int16:
                return "h";
            case table::type_code::uint16:
                return "H";
            case table::type_code::char16:
                return "u";
            case table::type_code::int32:
            case table::type_code::hresult:
            case table::type_code::enum32:
                return "i";
            case table::type_code::uint32:
            case table::type_code::enum_u32:
                return "I";
            case table::type_code::int64:
            case table::type_code::datetime:
            case table::type_code::timespan:
            case table::type_code::event_token:
                return "q";
            case table::type_code::uint64:
                return "Q";
            case table::type_code::single:
                return "f";
            case table::type_code::double_:
                return "d";
            case table::type_code::guid:
                return "T{I2H8B}";
            case table::type_code::string:
            case table::type_code::object:
            case table::type_code::interface_:
            case table::type_code::class_:
            case table::type_code::delegate:
            case table::type_code::generic:
            case table::type_code::reference:
                return "P";
            default:
                return nullptr;
            }
        }

        /**
         * Appends the PEP 3118 format of one value to @p out.
         *
         * A struct is spelled out field by field, each one named, which is
         * what memoryview shows and what the generated projection wrote down
         * per struct.
         *
         * @returns @c false with a Python error set.
         */
        bool append_format(
            projection& owner, arg_desc& value, std::string& out) noexcept
        {
            if (auto const scalar = scalar_format(value.code))
            {
                out += scalar;
                return true;
            }

            if (value.code != table::type_code::struct_)
            {
                PyErr_Format(
                    PyExc_NotImplementedError,
                    "no array element format for type code %d",
                    static_cast<int>(value.code));
                return false;
            }

            auto const info = resolve(owner, value.type, value.info);
            if (!info)
            {
                return false;
            }

            out += "T{";

            for (auto& field : info->fields)
            {
                arg_desc member{};
                member.code = field.code;
                member.type = field.type;
                member.info = field.info;

                if (!append_format(owner, member, out))
                {
                    return false;
                }

                field.info = member.info;

                auto const name = PyUnicode_AsUTF8(field.name);
                if (!name)
                {
                    return false;
                }

                out += ':';
                out += name;
                out += ':';
            }

            out += '}';

            return true;
        }

        /**
         * The WinRT name of a value's type, which is what
         * Array._winrt_element_type_name_ answers.
         */
        wchar_t const* fundamental_name(table::type_code code) noexcept
        {
            switch (code)
            {
            case table::type_code::boolean:
                return L"Boolean";
            case table::type_code::int8:
                return L"Int8";
            case table::type_code::uint8:
                return L"UInt8";
            case table::type_code::int16:
                return L"Int16";
            case table::type_code::uint16:
                return L"UInt16";
            case table::type_code::int32:
                return L"Int32";
            case table::type_code::uint32:
                return L"UInt32";
            case table::type_code::int64:
                return L"Int64";
            case table::type_code::uint64:
                return L"UInt64";
            case table::type_code::single:
                return L"Single";
            case table::type_code::double_:
                return L"Double";
            case table::type_code::char16:
                return L"Char16";
            case table::type_code::string:
                return L"String";
            case table::type_code::guid:
                return L"Guid";
            case table::type_code::object:
                return L"Object";
            case table::type_code::hresult:
                return L"Windows.Foundation.HResult";
            case table::type_code::event_token:
                return L"Windows.Foundation.EventRegistrationToken";
            case table::type_code::datetime:
                return L"Windows.Foundation.DateTime";
            case table::type_code::timespan:
                return L"Windows.Foundation.TimeSpan";
            default:
                return nullptr;
            }
        }

        /**
         * The WinRT name of @p value's type.
         *
         * @returns @c false with a Python error set.
         */
        bool element_name(
            projection& owner, arg_desc& value, std::wstring& out) noexcept
        {
            if (auto const fundamental = fundamental_name(value.code))
            {
                out = fundamental;
                return true;
            }

            auto const info = resolve(owner, value.type, value.info);
            if (!info)
            {
                return false;
            }

            auto const name = info->winrt_name;
            out.assign(name, name + std::strlen(name));

            return true;
        }

        /**
         * How wide one element is in the buffer that holds it.
         *
         * @returns @c false with a Python error set.
         */
        bool element_size(projection& owner, arg_desc& value, uint32_t& out) noexcept
        {
            if (value.code == table::type_code::struct_)
            {
                auto const info = resolve(owner, value.type, value.info);
                if (!info)
                {
                    return false;
                }

                out = info->size;

                return true;
            }

            try
            {
                out = table::get_value_layout(value.code).size;
                return true;
            }
            catch (...)
            {
                to_PyErr();
                return false;
            }
        }

        /**
         * Duplicates the strings and adds a reference to the interface
         * pointers of a value that has already been copied byte for byte.
         */
        void copy_value_resources(arg_desc const& value, void* storage)
        {
            switch (value.code)
            {
            case table::type_code::string:
                *static_cast<void**>(storage)
                    = duplicate_string(*static_cast<void**>(storage));
                break;
            case table::type_code::object:
            case table::type_code::interface_:
            case table::type_code::class_:
            case table::type_code::delegate:
            case table::type_code::generic:
            case table::type_code::reference:
                if (auto const abi = *static_cast<void**>(storage))
                {
                    static_cast<::IUnknown*>(abi)->AddRef();
                }
                break;
            case table::type_code::struct_:
                if (value.info)
                {
                    copy_struct_resources(*value.info, storage);
                }
                break;
            default:
                break;
            }
        }

        /**
         * Writes one argument into the buffer a trampoline reads, which packs
         * every argument that travels in a register as a whole one.
         */
        void store_argument(uint8_t* args, uint16_t offset, uintptr_t value) noexcept
        {
            std::memcpy(args + offset, &value, sizeof(value));
        }

        /**
         * Reads one argument out of the buffer a trampoline spilled.
         */
        template<typename T>
        T load_argument(uint8_t const* args, uint16_t offset) noexcept
        {
            T value;
            std::memcpy(&value, args + offset, sizeof(value));
            return value;
        }

        /**
         * Whether a copy of one value is more than a copy of its bytes: a
         * string has to be duplicated and an interface pointer referenced.
         */
        bool value_owns_resources(arg_desc const& value) noexcept
        {
            switch (value.code)
            {
            case table::type_code::string:
            case table::type_code::object:
            case table::type_code::interface_:
            case table::type_code::class_:
            case table::type_code::delegate:
            case table::type_code::generic:
            case table::type_code::reference:
                return true;
            case table::type_code::struct_:
                return value.info && value.info->owns_resources;
            default:
                return false;
            }
        }

    } // namespace

    /**
     * A WinRT array whose element type is a type code.
     *
     * py::ComArray<T> is the same thing for generated code, where T is a C++
     * type and the compiler writes the conversions. Here the element type is
     * whatever a table named, so every operation goes through the value
     * conversions the rest of the interpreter uses, and the storage is managed
     * by hand because there is no winrt::com_array<T> to hold it.
     *
     * An array either owns its elements - it allocated them, or took over the
     * ones a call handed back - or is lent them by a WinRT caller for the
     * length of one call, which is the only way a Python implementation can
     * write into the array it was given to fill.
     */
    struct table_array final : py::Array
    {
        table_array(
            projection& owner,
            arg_desc const& element,
            uint32_t value_size,
            std::string format,
            std::wstring name)
            : owner_{&owner}, element_{element}, value_size_{value_size},
              format_{std::move(format)}, name_{std::move(name)}
        {
        }

        ~table_array() override
        {
            clear();
        }

        /**
         * Takes over @p count elements that a call allocated.
         */
        void attach(uint32_t count, void* data) noexcept
        {
            clear();
            size_ = count;
            data_ = data;
            owns_ = true;
        }

        /**
         * Points at @p count elements a WinRT caller owns, so that what
         * Python writes into the array goes to the caller rather than to a
         * copy of it.
         */
        void lend(uint32_t count, void* data) noexcept
        {
            clear();
            size_ = count;
            data_ = data;
            owns_ = false;
        }

        /**
         * Gives back the elements a WinRT caller lent, which its call is
         * about to stop answering for.
         */
        void take_back() noexcept
        {
            size_ = 0;
            data_ = nullptr;
            owns_ = true;
        }

        bool Alloc(uint32_t size) noexcept override
        {
            clear();

            if (size == 0)
            {
                return true;
            }

            auto const bytes = static_cast<size_t>(size) * value_size_;
            auto const data = ::WINRT_IMPL_CoTaskMemAlloc(bytes);
            if (!data)
            {
                PyErr_NoMemory();
                return false;
            }

            std::memset(data, 0, bytes);

            size_ = size;
            data_ = data;

            return true;
        }

        std::wstring_view WinrtElementTypeName() noexcept override
        {
            return name_;
        }

        std::string_view Format() noexcept override
        {
            return format_;
        }

        uint32_t Size() noexcept override
        {
            return size_;
        }

        size_t ValueSize() noexcept override
        {
            return value_size_;
        }

        void* Data() noexcept override
        {
            return data_;
        }

        PyObject* At(uint32_t index) noexcept override
        {
            if (index >= size_)
            {
                PyErr_SetString(PyExc_IndexError, "index out of range");
                return nullptr;
            }

            // The array goes on holding the element, so the conversion that
            // borrows is the right one here.
            return convert_borrowed(*owner_, element_, at(index));
        }

        bool Set(Py_ssize_t index, PyObject* item) noexcept override
        {
            if (index < 0)
            {
                index += size_;
            }

            if (index < 0 || static_cast<uint32_t>(index) >= size_)
            {
                PyErr_SetString(PyExc_IndexError, "index out of range");
                return false;
            }

            auto* const slot = at(static_cast<uint32_t>(index));

            try
            {
                release_value(element_, slot);
                std::memset(slot, 0, value_size_);
                convert_to_abi(*owner_, element_, item, slot);
                return true;
            }
            catch (...)
            {
                to_PyErr();
                return false;
            }
        }

      private:
        uint8_t* at(uint32_t index) const noexcept
        {
            return static_cast<uint8_t*>(data_)
                   + static_cast<size_t>(index) * value_size_;
        }

        void clear() noexcept
        {
            if (data_ && owns_)
            {
                for (uint32_t i = 0; i < size_; i++)
                {
                    release_value(element_, at(i));
                }

                ::WINRT_IMPL_CoTaskMemFree(data_);
            }

            data_ = nullptr;
            size_ = 0;
            owns_ = true;
        }

        projection* owner_;
        arg_desc element_;
        uint32_t value_size_;
        std::string format_;
        std::wstring name_;
        uint32_t size_{};
        void* data_{};
        /// Whether the elements are this array's to free, which a lent one's
        /// are not.
        bool owns_{true};
    };

    namespace
    {
        /**
         * An empty array of @p element's type, with everything the buffer
         * protocol and the element conversions need worked out.
         *
         * @returns @c nullptr with a Python error set.
         */
        std::unique_ptr<table_array> make_table_array(
            projection& owner, arg_desc& element) noexcept
        {
            uint32_t value_size{};
            if (!element_size(owner, element, value_size))
            {
                return nullptr;
            }

            std::string format;
            if (!append_format(owner, element, format))
            {
                return nullptr;
            }

            std::wstring name;
            if (!element_name(owner, element, name))
            {
                return nullptr;
            }

            try
            {
                return std::make_unique<table_array>(
                    owner, element, value_size, std::move(format), std::move(name));
            }
            catch (...)
            {
                to_PyErr();
                return nullptr;
            }
        }

        /**
         * The element of an array of values of @p info's type, which is what a
         * winrt.system.Array named after a projected type holds.
         *
         * @returns @c false with a Python error set.
         */
        bool element_of(type_entry& info, arg_desc& element) noexcept
        {
            switch (info.category)
            {
            case table::category::enum_:
            {
                // A flags enum is read unsigned and every other one signed,
                // which is the same split that makes one class enum.IntFlag
                // and the other enum.IntEnum.
                auto const record = info.owner->table->type(info.index);
                auto const is_flags
                    = (record.flags() & table::type_flags::flags_enum) != 0;

                element.code
                    = is_flags ? table::type_code::enum_u32 : table::type_code::enum32;
                break;
            }
            case table::category::struct_:
                element.code = table::type_code::struct_;
                break;
            case table::category::class_:
                element.code = table::type_code::class_;
                break;
            case table::category::interface_:
                element.code = table::type_code::interface_;
                break;
            case table::category::delegate:
                element.code = table::type_code::delegate;
                break;
            default:
                PyErr_Format(
                    PyExc_TypeError,
                    "'%s' is not an element type a WinRT array can hold",
                    info.winrt_name);
                return false;
            }

            element.type = info.index;
            element.info = &info;

            return true;
        }
    } // namespace

    /**
     * Borrows the elements an array argument passes or lends from @p obj.
     *
     * Nothing is copied: the callee reads, and for a lent array writes, the
     * memory the Python object exports, so the buffer has to hold exactly the
     * elements the parameter is declared with and stays borrowed until the
     * call is over.
     *
     * @param writable Whether the callee writes the elements, which is what a
     * lent array - a fill array - is for.
     * @returns @c false with a Python error set.
     */
    bool borrow_array(
        projection& owner,
        arg_desc& element,
        PyObject* obj,
        bool writable,
        Py_buffer* view) noexcept
    {
        uint32_t value_size{};
        if (!element_size(owner, element, value_size))
        {
            return false;
        }

        std::string format;
        if (!append_format(owner, element, format))
        {
            return false;
        }

        auto const flags
            = PyBUF_C_CONTIGUOUS | PyBUF_FORMAT | (writable ? PyBUF_WRITABLE : 0);

        if (PyObject_GetBuffer(obj, view, flags) < 0)
        {
            return false;
        }

        if (!is_buffer_compatible(*view, value_size, format.c_str()))
        {
            PyBuffer_Release(view);
            return false;
        }

        return true;
    }

    /**
     * Fills in the two ABI arguments of an array a call passes or lends: the
     * count of elements and the pointer to them.
     *
     * The elements stay borrowed from @p value until the call is over, which
     * is what @p view holds them for.
     *
     * @returns @c false with a Python error set.
     */
    bool borrow_array_argument(
        projection& owner,
        arg_desc& arg,
        PyObject* value,
        Py_buffer* view,
        uint8_t* args) noexcept
    {
        // A failed borrow leaves a view alone, and the call releases every
        // view it handed out, so a zeroed one is what makes the failure a
        // no-op rather than a release of whatever was on the stack.
        std::memset(view, 0, sizeof(Py_buffer));

        auto const writable = arg.category == table::param_category::fill_array;

        if (!borrow_array(owner, arg, value, writable, view))
        {
            return false;
        }

        store_argument(args, arg.offset, static_cast<uintptr_t>(view->shape[0]));
        store_argument(args, arg.data_offset, reinterpret_cast<uintptr_t>(view->buf));

        return true;
    }

    /**
     * Points the two ABI arguments of an array a call receives at the count
     * and the pointer the callee fills in.
     */
    void point_at_array_output(
        arg_desc const& arg, array_out& received, uint8_t* args) noexcept
    {
        store_argument(args, arg.offset, reinterpret_cast<uintptr_t>(&received.count));
        store_argument(
            args, arg.data_offset, reinterpret_cast<uintptr_t>(&received.data));
    }

    /**
     * Takes over the elements a call allocated and hands them to Python as a
     * winrt.system.Array.
     *
     * @p received is emptied whether or not this succeeds: what it named is
     * either owned by the array or given back here.
     *
     * @returns @c nullptr with a Python error set.
     */
    PyObject* array_take_python(
        projection& owner, arg_desc& element, array_out& received) noexcept
    {
        auto array = make_table_array(owner, element);
        if (!array)
        {
            release_array(element, received);
            return nullptr;
        }

        array->attach(received.count, received.data);
        received = {};

        return cpp::_winrt::Array_New(std::move(array));
    }

    /**
     * Copies @p count elements that belong to a WinRT caller into a
     * winrt.system.Array of its own.
     *
     * A call that arrives from WinRT lends its arrays for the length of the
     * call only, and nothing says how long the Python object made of one will
     * live, so this is the one place an array is copied.
     *
     * @returns @c nullptr with a Python error set.
     */
    PyObject* array_to_python(
        projection& owner, arg_desc& element, uint32_t count, void const* data) noexcept
    {
        auto array = make_table_array(owner, element);
        if (!array)
        {
            return nullptr;
        }

        if (!array->Alloc(count))
        {
            return nullptr;
        }

        if (count == 0)
        {
            return cpp::_winrt::Array_New(std::move(array));
        }

        auto* const bytes = static_cast<uint8_t*>(array->Data());
        auto const width = array->ValueSize();

        if (!value_owns_resources(element))
        {
            std::memcpy(bytes, data, count * width);
        }
        else
        {
            // One element at a time, so that a failure part way through
            // leaves the rest of the array as Alloc() zeroed it and the
            // array owns exactly what has been duplicated.
            auto const* const source = static_cast<uint8_t const*>(data);

            try
            {
                for (uint32_t i = 0; i < count; i++)
                {
                    std::memcpy(bytes + i * width, source + i * width, width);
                    copy_value_resources(element, bytes + i * width);
                }
            }
            catch (...)
            {
                to_PyErr();
                return nullptr;
            }
        }

        return cpp::_winrt::Array_New(std::move(array));
    }

    /**
     * Lends @p count elements that a WinRT caller owns to Python for the
     * length of one call.
     *
     * A lent array - a fill array - is the one the callee writes, so it has to
     * be the caller's own memory and not a copy of it. What that costs is that
     * the array has to be handed back the moment the call is over, which is
     * what @p lent is for: the object is an ordinary winrt.system.Array and
     * nothing stops a callback from keeping it.
     *
     * @returns @c nullptr with a Python error set.
     */
    PyObject* array_lend_python(
        projection& owner,
        arg_desc& element,
        uint32_t count,
        void* data,
        table_array*& lent) noexcept
    {
        auto array = make_table_array(owner, element);
        if (!array)
        {
            return nullptr;
        }

        array->lend(count, data);
        lent = array.get();

        auto const object = cpp::_winrt::Array_New(std::move(array));
        if (!object)
        {
            lent = nullptr;
        }

        return object;
    }

    /**
     * Takes back what array_lend_python() lent, leaving the Python object it
     * was lent through empty.
     */
    void take_back_array(table_array& lent) noexcept
    {
        lent.take_back();
    }

    /**
     * The Python argument an array a WinRT caller passed or lent becomes.
     *
     * A passed array is read only and nothing says how long the Python object
     * made of it will live, so it gets elements of its own. A lent array is
     * what the caller asked to have written, so it has to be the caller's own
     * elements; @p lent is how the call takes them back when it returns.
     *
     * @returns @c nullptr with a Python error set.
     */
    PyObject* array_argument_to_python(
        projection& owner,
        arg_desc& arg,
        uint8_t const* args,
        table_array*& lent) noexcept
    {
        auto const count = load_argument<uint32_t>(args, arg.offset);
        auto* const data = load_argument<void*>(args, arg.data_offset);

        if (arg.category == table::param_category::pass_array)
        {
            return array_to_python(owner, arg, count, data);
        }

        return array_lend_python(owner, arg, count, data, lent);
    }

    /**
     * Stores the array a Python implementation handed back where the two
     * arguments of a receive array point.
     *
     * @throws python_exception if @p value does not hold the elements the
     * parameter is declared with.
     */
    void array_result_to_abi(
        projection& owner, arg_desc& arg, PyObject* value, uint8_t* args)
    {
        auto* const count = load_argument<uint32_t*>(args, arg.offset);

        if (!count)
        {
            return;
        }

        auto* const data = load_argument<void**>(args, arg.data_offset);

        if (!data)
        {
            return;
        }

        array_out result{};
        array_from_python(owner, arg, value, result);

        *count = result.count;
        *data = result.data;
    }

    /**
     * Allocates the elements a WinRT caller receives and copies @p value into
     * them.
     *
     * This is a receive array answered from Python: the caller gets storage of
     * its own, so the elements are duplicated into memory it will free.
     *
     * @throws python_exception if @p value does not hold the elements the
     * parameter is declared with.
     */
    void array_from_python(
        projection& owner, arg_desc& element, PyObject* value, array_out& out)
    {
        Py_buffer view{};

        if (!borrow_array(owner, element, value, false, &view))
        {
            throw python_exception();
        }

        auto const count = static_cast<uint32_t>(view.shape[0]);
        auto const width = static_cast<size_t>(view.itemsize);

        if (count == 0)
        {
            PyBuffer_Release(&view);
            out = {};
            return;
        }

        array_out result{};
        result.data = ::WINRT_IMPL_CoTaskMemAlloc(count * width);

        if (!result.data)
        {
            PyBuffer_Release(&view);
            PyErr_NoMemory();
            throw python_exception();
        }

        std::memset(result.data, 0, count * width);

        auto* const bytes = static_cast<uint8_t*>(result.data);
        auto const* const source = static_cast<uint8_t const*>(view.buf);

        if (!value_owns_resources(element))
        {
            std::memcpy(bytes, source, count * width);
            result.count = count;
        }
        else
        {
            // The count grows as each element is duplicated, so that a
            // failure part way through leaves behind an array that owns
            // exactly what it holds.
            try
            {
                for (uint32_t i = 0; i < count; i++)
                {
                    std::memcpy(bytes + i * width, source + i * width, width);
                    copy_value_resources(element, bytes + i * width);
                    result.count = i + 1;
                }
            }
            catch (...)
            {
                PyBuffer_Release(&view);
                release_array(element, result);
                throw;
            }
        }

        PyBuffer_Release(&view);

        out = result;
    }

    /**
     * Gives back the elements a call allocated but nothing took over.
     *
     * An element whose type could not be resolved cannot be released, because
     * what holds a resource is a property of that type; the only way to get
     * here with one is a failure that already has an exception of its own.
     */
    void release_array(arg_desc const& element, array_out& received) noexcept
    {
        if (!received.data)
        {
            received = {};
            return;
        }

        uint32_t width{};

        if (element.code == table::type_code::struct_)
        {
            width = element.info ? element.info->size : 0;
        }
        else
        {
            try
            {
                width = table::get_value_layout(element.code).size;
            }
            catch (...)
            {
                width = 0;
            }
        }

        if (width != 0)
        {
            auto* const bytes = static_cast<uint8_t*>(received.data);

            for (uint32_t i = 0; i < received.count; i++)
            {
                release_value(element, bytes + i * width);
            }
        }

        ::WINRT_IMPL_CoTaskMemFree(received.data);

        received = {};
    }

    /**
     * A new winrt.system.Array of @p count default elements, which is what a
     * caller lends to a member that fills one.
     *
     * @returns @c nullptr with a Python error set.
     */
    PyObject* new_array(projection& owner, arg_desc& element, uint32_t count) noexcept
    {
        auto array = make_table_array(owner, element);
        if (!array)
        {
            return nullptr;
        }

        if (!array->Alloc(count))
        {
            return nullptr;
        }

        return cpp::_winrt::Array_New(std::move(array));
    }

    /**
     * _assign_array_(): says what a winrt.system.Array holds. Every projected
     * type carries it, a struct as much as a class, because an array of any of
     * them can be made.
     */
    PyObject* type_assign_array(PyObject* cls, PyObject* arg) noexcept
    {
        auto const info = get_type_entry(reinterpret_cast<PyTypeObject*>(cls));
        if (!info)
        {
            PyErr_Format(
                PyExc_TypeError,
                "'%s' is not a projected type",
                reinterpret_cast<PyTypeObject*>(cls)->tp_name);
            return nullptr;
        }

        arg_desc element{};
        if (!element_of(*info, element))
        {
            return nullptr;
        }

        auto array = make_table_array(*info->owner, element);
        if (!array)
        {
            return nullptr;
        }

        if (!cpp::_winrt::Array_Assign(arg, std::move(array)))
        {
            return nullptr;
        }

        Py_RETURN_NONE;
    }
} // namespace py::interp
