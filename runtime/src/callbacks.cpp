// A call that arrives from WinRT, made into a Python one.
//
// The trampoline that caught the call has already spilled its arguments into a
// buffer laid out exactly as the forward path lays one out - both ends compute
// it from the same shape - so the member's descriptor says where each argument
// is here as much as it does there. What is different is the direction of
// every conversion and who owns what: an argument WinRT passed in belongs to
// the caller for the length of the call, and a value Python hands back is
// handed over.
//
// A Python exception has nowhere to be raised, because WinRT is the caller. It
// goes to sys.unraisablehook and WinRT is told that one happened, which is
// what a generated delegate did too.

#include <Python.h>

#include <pywinrt/base.h>

#include "arrays.h"
#include "callbacks.h"
#include "delegates.h"
#include "generics.h"
#include "interp.h"
#include "objects.h"
#include "structs.h"

namespace py::interp
{
    namespace
    {
        template<typename T>
        T load(void const* storage) noexcept
        {
            T value;
            std::memcpy(&value, storage, sizeof(value));
            return value;
        }

        template<typename T>
        void put(void* storage, T value) noexcept
        {
            std::memcpy(storage, &value, sizeof(value));
        }
    } // namespace

    python_args::~python_args()
    {
        // Before the references, because taking an array back leaves the
        // object that was lent it empty rather than freeing it.
        for (uint16_t i = 0; i < lent_count; i++)
        {
            take_back_array(*lent[i]);
        }

        for (uint16_t i = 0; i < count; i++)
        {
            Py_DECREF(values[i]);
        }
    }

    /**
     * Remembers an array the caller lent, so that the call gives it back
     * however it ends.
     */
    void python_args::add_lent(table_array* array)
    {
        if (lent_count == max_arrays)
        {
            take_back_array(*array);

            throw winrt::hresult_not_implemented(
                L"the callback lends more arrays than the runtime takes back");
        }

        lent[lent_count++] = array;
    }

    /**
     * Converts every input of @p overload into @p in, in the order Python
     * takes them.
     *
     * @throws python_exception on a conversion failure, and
     * winrt::hresult_error for a shape the interpreter cannot take apart.
     */
    void read_inputs(
        member_desc const& member,
        overload_desc& overload,
        uint8_t* buffer,
        python_args& in)
    {
        if (overload.in_count > max_args)
        {
            throw winrt::hresult_not_implemented(
                L"the callback takes more arguments than the runtime passes");
        }

        for (uint16_t i = 0; i < overload.arg_count; i++)
        {
            auto& arg = overload.args[i];

            if (arg.category == table::param_category::pass_array
                || arg.category == table::param_category::fill_array)
            {
                table_array* lent{};

                in.values[in.count]
                    = array_argument_to_python(*member.owner, arg, buffer, lent);

                if (!in.values[in.count])
                {
                    throw python_exception();
                }

                in.count++;

                if (lent)
                {
                    in.add_lent(lent);
                }

                continue;
            }

            if (arg.category != table::param_category::in || arg.is_implicit)
            {
                continue;
            }

            // A by-reference input arrives as a pointer to the value
            // rather than the value itself.
            auto const storage = arg.by_reference ? load<uint8_t*>(buffer + arg.offset)
                                                  : buffer + arg.offset;

            in.values[in.count] = convert_borrowed(*member.owner, arg, storage);

            if (!in.values[in.count])
            {
                throw python_exception();
            }

            in.count++;
        }
    }

    /**
     * Stores what Python handed back where @p overload's outputs point.
     *
     * @throws python_exception if @p result is not what the member's
     * outputs need.
     */
    void write_outputs(
        member_desc const& member,
        overload_desc& overload,
        PyObject* result,
        uint8_t* buffer)
    {
        // The return value comes first and the declared outputs follow in
        // order, which is what the projection hands back from a call and
        // so what it asks for here.
        if (overload.out_count > 1)
        {
            auto const shape_of_result
                = PyTuple_Check(result)
                  && PyTuple_GET_SIZE(result) == overload.out_count;

            if (!shape_of_result)
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "'%s' must hand back a tuple of %d values",
                    overload.winrt_name,
                    static_cast<int>(overload.out_count));
                throw python_exception();
            }
        }

        // Where the declared outputs start, which is after the return
        // value when there is one.
        Py_ssize_t next = 0;

        for (uint16_t i = 0; i < overload.arg_count; i++)
        {
            auto const& arg = overload.args[i];

            if (is_output(arg) && arg.is_return)
            {
                next = 1;
                break;
            }
        }

        for (uint16_t i = 0; i < overload.arg_count; i++)
        {
            auto& arg = overload.args[i];

            if (!is_output(arg) || arg.is_implicit)
            {
                continue;
            }

            auto* const value
                = overload.out_count == 1
                      ? result
                      : PyTuple_GET_ITEM(result, arg.is_return ? 0 : next++);

            if (arg.category == table::param_category::receive_array)
            {
                array_result_to_abi(*member.owner, arg, value, buffer);
                continue;
            }

            if (auto* const storage = load<void*>(buffer + arg.offset))
            {
                convert_to_abi(*member.owner, arg, value, storage);
            }
        }
    }

    /**
     * Converts one value of @p arg's type from the ABI to Python without
     * taking what the ABI holds.
     *
     * This is the other direction of a call: an argument WinRT passed in to a
     * Python implementation belongs to the caller for the length of the call,
     * so a reference has to be added rather than moved. Everything after that
     * is the same conversion, which is why only the cases that own something
     * are here.
     */
    PyObject* convert_borrowed(projection& owner, arg_desc& arg, void* storage) noexcept
    {
        switch (arg.code)
        {
        case table::type_code::string:
        {
            winrt::hstring text;
            winrt::attach_abi(text, load<void*>(storage));
            auto const result = convert(text);
            winrt::detach_abi(text);

            return result;
        }
        case table::type_code::object:
        case table::type_code::interface_:
        case table::type_code::class_:
        case table::type_code::delegate:
        case table::type_code::generic:
        case table::type_code::reference:
            if (auto const abi = load<void*>(storage))
            {
                static_cast<::IUnknown*>(abi)->AddRef();
            }

            break;
        case table::type_code::struct_:
        {
            auto const info = resolve(owner, arg.type, arg.info);
            if (!info)
            {
                return nullptr;
            }

            return struct_to_python(*info, storage);
        }
        default:
            break;
        }

        return convert_out(owner, arg, storage);
    }

    /**
     * Converts @p value and stores it where an output pointer points.
     *
     * Unlike convert_in(), which fills a register slot, this writes the width
     * the type code says, because what it writes to is a @c T* the caller
     * handed over. Whatever it stores belongs to that caller afterwards: a
     * string and an interface pointer each go over with a reference.
     *
     * @throws python_exception on a conversion failure.
     */
    void convert_to_abi(
        projection& owner, arg_desc& arg, PyObject* value, void* storage)
    {
        switch (arg.code)
        {
        case table::type_code::boolean:
            put<bool>(storage, convert_to<bool>(value));
            return;
        case table::type_code::int8:
            put<int8_t>(storage, convert_to<int8_t>(value));
            return;
        case table::type_code::uint8:
            put<uint8_t>(storage, convert_to<uint8_t>(value));
            return;
        case table::type_code::int16:
            put<int16_t>(storage, convert_to<int16_t>(value));
            return;
        case table::type_code::uint16:
            put<uint16_t>(storage, convert_to<uint16_t>(value));
            return;
        case table::type_code::char16:
            put<char16_t>(storage, convert_to<char16_t>(value));
            return;
        case table::type_code::int32:
        case table::type_code::hresult:
        case table::type_code::enum32:
            put<int32_t>(storage, convert_to<int32_t>(value));
            return;
        case table::type_code::uint32:
        case table::type_code::enum_u32:
            put<uint32_t>(storage, convert_to<uint32_t>(value));
            return;
        case table::type_code::int64:
            put<int64_t>(storage, convert_to<int64_t>(value));
            return;
        case table::type_code::uint64:
            put<uint64_t>(storage, convert_to<uint64_t>(value));
            return;
        case table::type_code::single:
            put<float>(storage, convert_to<float>(value));
            return;
        case table::type_code::double_:
            put<double>(storage, convert_to<double>(value));
            return;
        case table::type_code::datetime:
            put<int64_t>(
                storage,
                convert_to<winrt::Windows::Foundation::DateTime>(value)
                    .time_since_epoch()
                    .count());
            return;
        case table::type_code::timespan:
            put<int64_t>(
                storage,
                convert_to<winrt::Windows::Foundation::TimeSpan>(value).count());
            return;
        case table::type_code::event_token:
            put<int64_t>(storage, convert_to<int64_t>(value));
            return;
        case table::type_code::guid:
            put<winrt::guid>(storage, convert_to<winrt::guid>(value));
            return;
        case table::type_code::string:
        {
            auto text = convert_to<winrt::hstring>(value);
            put<void*>(storage, winrt::detach_abi(text));
            return;
        }
        case table::type_code::object:
            put<void*>(storage, unwrap_abi(value, nullptr));
            return;
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

            put<void*>(storage, unwrap_abi(value, info->guid, info));
            return;
        }
        case table::type_code::reference:
        {
            auto const info = resolve(owner, arg.type, arg.info);
            if (!info)
            {
                throw python_exception();
            }

            put<void*>(storage, reference_from_python(*info, value));
            return;
        }
        case table::type_code::delegate:
        {
            if (Py_IsNone(value))
            {
                put<void*>(storage, nullptr);
                return;
            }

            auto const info = resolve(owner, arg.type, arg.info);
            if (!info)
            {
                throw python_exception();
            }

            auto const abi = Py_IS_TYPE(value, info->py_type)
                                 ? unwrap_abi(value, info->guid)
                                 : make_delegate(*info, value);

            if (!abi)
            {
                throw python_exception();
            }

            put<void*>(storage, abi);
            return;
        }
        case table::type_code::struct_:
        {
            auto const info = resolve(owner, arg.type, arg.info);
            if (!info)
            {
                throw python_exception();
            }

            // A wrapper is copied out of rather than emptied, because the
            // Python object it belongs to goes on holding what is in it, so
            // the strings and interface pointers of the copy are its own. One
            // built from a tuple already owns what store_field() put in it.
            auto const borrowed = Py_IS_TYPE(value, info->py_type);

            struct_from_python(*info, value, storage);

            if (info->owns_resources && borrowed)
            {
                copy_struct_resources(*info, storage);
            }

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
     * Runs @p op on @p target with the arguments that a reverse trampoline
     * spilled into @p args, and stores what comes back where @p overload's
     * outputs point.
     *
     * The GIL must be held.
     */
    int32_t call_python(
        member_desc const& member,
        overload_desc& overload,
        PyObject* target,
        python_op op,
        void* args) noexcept
    {
        auto* const buffer = static_cast<uint8_t*>(args);

        try
        {
            try
            {
                python_args in;

                read_inputs(member, overload, buffer, in);

                pyobj_handle result{};

                switch (op)
                {
                case python_op::invoke:
                    result.attach(
                        PyObject_Vectorcall(target, in.values, in.count, nullptr));
                    break;
                case python_op::call_method:
                {
                    pyobj_handle method{PyObject_GetAttr(target, member.py_name)};
                    if (!method)
                    {
                        throw python_exception();
                    }

                    result.attach(PyObject_Vectorcall(
                        method.get(), in.values, in.count, nullptr));
                    break;
                }
                case python_op::get_attribute:
                    result.attach(PyObject_GetAttr(target, member.py_name));
                    break;
                case python_op::set_attribute:
                    if (PyObject_SetAttr(target, member.py_name, in.values[0]) == -1)
                    {
                        throw python_exception();
                    }

                    return 0;
                }

                if (!result)
                {
                    throw python_exception();
                }

                if (overload.out_count == 0)
                {
                    return 0;
                }

                write_outputs(member, overload, result.get(), buffer);

                return 0;
            }
            catch (python_exception const&)
            {
                write_unraisable_and_throw();
            }
        }
        catch (...)
        {
            return winrt::to_hresult();
        }
    }
} // namespace py::interp
