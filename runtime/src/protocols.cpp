// The Python protocols a projected type implements.
//
// Every slot here is the same two steps: take the member whose role in the
// table says it stands for this operation, and call it. What makes that worth
// a file of its own is that the mapping from a Python operation to a WinRT
// member is a convention rather than anything either side writes down - a
// Python mapping's __getitem__ is IMap<K, V>::Lookup, and raises KeyError
// where WinRT fails with E_BOUNDS - and that convention used to be spread over
// the generated code of every collection type in the projection.

#include <Python.h>

#define PYWINRT_RUNTIME_MODULE
#include <pywinrt/base.h>

#include "arrays.h"
#include "async.h"
#include "interp.h"
#include "objects.h"
#include "protocols.h"
#include "runtime.h"
#include "types.h"

namespace py::interp
{
    namespace
    {
        /**
         * What the table built for @p self, which is where the members behind
         * its protocol slots are.
         */
        type_entry* entry_of(PyObject* self) noexcept
        {
            auto const info = get_type_entry(Py_TYPE(self));
            if (!info)
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "'%s' is not a projected type",
                    Py_TYPE(self)->tp_name);
            }

            return info;
        }

        /**
         * Calls the member behind a protocol slot.
         *
         * @param member The member the type entry named, which is @c nullptr
         * when the type does not have one, as a read-only collection has no
         * SetAt.
         */
        PyObject* call_protocol(
            member_desc* member,
            char const* operation,
            PyObject* self,
            PyObject* const* args,
            Py_ssize_t nargs) noexcept
        {
            if (!member)
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "'%s' does not support %s",
                    Py_TYPE(self)->tp_name,
                    operation);
                return nullptr;
            }

            auto const overload = select_overload(*member, nargs);
            if (!overload)
            {
                return nullptr;
            }

            return call_member(*member, *overload, abi_of(self), args, nargs);
        }

        /**
         * Replaces the pending exception with the KeyError that a Python
         * mapping raises for a key it does not have.
         *
         * WinRT reports one by failing with E_BOUNDS, which is an OSError by
         * the time it gets here. Anything else really did fail and is left
         * alone.
         */
        void set_key_error(PyObject* key) noexcept
        {
            if (!PyErr_ExceptionMatches(PyExc_OSError))
            {
                return;
            }

#if PY_VERSION_HEX < 0x030C0000
            PyObject *type, *value, *trace;
            PyErr_Fetch(&type, &value, &trace);
            PyErr_NormalizeException(&type, &value, &trace);
            pyobj_handle raised{value};
            Py_XDECREF(type);
            Py_XDECREF(trace);
#else
            pyobj_handle raised{PyErr_GetRaisedException()};
#endif

            pyobj_handle winerror{PyObject_GetAttrString(raised.get(), "winerror")};
            if (!winerror)
            {
                PyErr_Clear();
            }

            auto const hresult = winerror ? PyLong_AsLong(winerror.get()) : 0;

            if (hresult == winrt::impl::error_out_of_bounds)
            {
                PyErr_SetObject(PyExc_KeyError, key);
                return;
            }

#if PY_VERSION_HEX < 0x030C0000
            PyErr_SetObject(
                reinterpret_cast<PyObject*>(Py_TYPE(raised.get())), raised.get());
#else
            PyErr_SetRaisedException(raised.detach());
#endif
        }

        // ----- sequences --------------------------------------------------

        Py_ssize_t protocol_length(PyObject* self) noexcept
        {
            auto const info = entry_of(self);
            if (!info)
            {
                return -1;
            }

            pyobj_handle size{
                call_protocol(info->protocol.size, "len()", self, nullptr, 0)};
            if (!size)
            {
                return -1;
            }

            return PyLong_AsSsize_t(size.get());
        }

        PyObject* sequence_item(PyObject* self, Py_ssize_t index) noexcept
        {
            auto const info = entry_of(self);
            if (!info)
            {
                return nullptr;
            }

            // An index this slot was given as a number goes to GetAt() as
            // one; anything the direct path does not describe, including a
            // negative index, keeps whatever the ordinary path makes of it.
            if (index >= 0 && static_cast<uint64_t>(index) <= UINT32_MAX)
            {
                bool direct{};

                auto* const item = call_indexed(
                    info->protocol.get_at,
                    abi_of(self),
                    static_cast<uint32_t>(index),
                    direct);

                if (direct)
                {
                    return item;
                }
            }

            pyobj_handle position{PyLong_FromSsize_t(index)};
            if (!position)
            {
                return nullptr;
            }

            PyObject* args[] = {position.get()};

            return call_protocol(info->protocol.get_at, "indexing", self, args, 1);
        }

        /**
         * The elements a slice names, which a WinRT vector reads with GetMany
         * in one call into an array the caller lends it.
         *
         * The answer is that array - a winrt.system.Array rather than a list -
         * which is what a slice of a projected vector has always been.
         */
        PyObject* sequence_slice(
            type_entry& info, PyObject* self, PyObject* key) noexcept
        {
            if (!info.protocol.get_many)
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "'%s' does not support slicing",
                    Py_TYPE(self)->tp_name);
                return nullptr;
            }

            auto const size = protocol_length(self);
            if (size == -1)
            {
                return nullptr;
            }

            Py_ssize_t start{};
            Py_ssize_t stop{};
            Py_ssize_t step{};
            Py_ssize_t length{};

            if (PySlice_GetIndicesEx(key, size, &start, &stop, &step, &length) < 0)
            {
                return nullptr;
            }

            if (step != 1)
            {
                PyErr_SetString(
                    PyExc_NotImplementedError,
                    "slices with step other than 1 are not implemented");
                return nullptr;
            }

            auto const overload = select_overload(*info.protocol.get_many, 2);
            if (!overload)
            {
                return nullptr;
            }

            // The elements parameter is the one the caller lends, and its
            // descriptor is what says what the array holds.
            arg_desc* elements{};

            for (uint16_t i = 0; i < overload->arg_count; i++)
            {
                if (overload->args[i].category == table::param_category::fill_array)
                {
                    elements = &overload->args[i];
                    break;
                }
            }

            if (!elements)
            {
                PyErr_SetString(
                    PyExc_TypeError, "GetMany does not take an array to fill");
                return nullptr;
            }

            pyobj_handle items{new_array(
                *info.protocol.get_many->owner,
                *elements,
                static_cast<uint32_t>(length))};
            if (!items)
            {
                return nullptr;
            }

            pyobj_handle first{PyLong_FromSsize_t(start)};
            if (!first)
            {
                return nullptr;
            }

            PyObject* args[] = {first.get(), items.get()};

            pyobj_handle count{
                call_protocol(info.protocol.get_many, "slicing", self, args, 2)};
            if (!count)
            {
                return nullptr;
            }

            auto const filled = PyLong_AsSsize_t(count.get());
            if (filled == -1 && PyErr_Occurred())
            {
                return nullptr;
            }

            if (filled != length)
            {
                PyErr_Format(
                    PyExc_RuntimeError,
                    "GetMany filled %zd of the %zd elements the slice names",
                    filled,
                    length);
                return nullptr;
            }

            return items.detach();
        }

        /**
         * __getitem__ of a sequence, which is an index or a slice.
         */
        PyObject* sequence_subscript(PyObject* self, PyObject* key) noexcept
        {
            if (PySlice_Check(key))
            {
                auto const info = entry_of(self);
                if (!info)
                {
                    return nullptr;
                }

                return sequence_slice(*info, self, key);
            }

            if (!PyIndex_Check(key))
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "indices must be integers, not '%s'",
                    Py_TYPE(key)->tp_name);
                return nullptr;
            }

            pyobj_handle index{PyNumber_Index(key)};
            if (!index)
            {
                return nullptr;
            }

            auto const position = PyNumber_AsSsize_t(index.get(), PyExc_IndexError);
            if (position == -1 && PyErr_Occurred())
            {
                return nullptr;
            }

            return sequence_item(self, position);
        }

        int sequence_assign(PyObject* self, Py_ssize_t index, PyObject* value) noexcept
        {
            auto const info = entry_of(self);
            if (!info)
            {
                return -1;
            }

            pyobj_handle position{PyLong_FromSsize_t(index)};
            if (!position)
            {
                return -1;
            }

            if (!value)
            {
                PyObject* args[] = {position.get()};
                pyobj_handle removed{
                    call_protocol(info->protocol.remove_at, "del", self, args, 1)};

                return removed ? 0 : -1;
            }

            PyObject* args[] = {position.get(), value};
            pyobj_handle assigned{
                call_protocol(info->protocol.set_at, "assignment", self, args, 2)};

            return assigned ? 0 : -1;
        }

        // ----- mappings ---------------------------------------------------

        PyObject* mapping_subscript(PyObject* self, PyObject* key) noexcept
        {
            auto const info = entry_of(self);
            if (!info)
            {
                return nullptr;
            }

            PyObject* args[] = {key};
            pyobj_handle value{
                call_protocol(info->protocol.lookup, "lookup", self, args, 1)};
            if (!value)
            {
                set_key_error(key);
                return nullptr;
            }

            return value.detach();
        }

        int mapping_assign(PyObject* self, PyObject* key, PyObject* value) noexcept
        {
            auto const info = entry_of(self);
            if (!info)
            {
                return -1;
            }

            if (!value)
            {
                PyObject* args[] = {key};
                pyobj_handle removed{
                    call_protocol(info->protocol.remove, "del", self, args, 1)};

                if (!removed)
                {
                    set_key_error(key);
                    return -1;
                }

                return 0;
            }

            PyObject* args[] = {key, value};
            pyobj_handle inserted{
                call_protocol(info->protocol.insert, "assignment", self, args, 2)};

            return inserted ? 0 : -1;
        }

        int mapping_contains(PyObject* self, PyObject* key) noexcept
        {
            auto const info = entry_of(self);
            if (!info)
            {
                return -1;
            }

            PyObject* args[] = {key};
            pyobj_handle answer{
                call_protocol(info->protocol.has_key, "'in'", self, args, 1)};
            if (!answer)
            {
                return -1;
            }

            return PyObject_IsTrue(answer.get());
        }

        // ----- iteration --------------------------------------------------

        PyObject* iterable_iter(PyObject* self) noexcept
        {
            auto const info = entry_of(self);
            if (!info)
            {
                return nullptr;
            }

            return call_protocol(info->protocol.first, "iteration", self, nullptr, 0);
        }

        /**
         * iter() of a mapping, which yields keys.
         *
         * WinRT iterates a map as key-value pairs, so what First() hands back
         * is wrapped in the runtime's own iterator, which takes the key of each
         * pair.
         */
        PyObject* mapping_iter(PyObject* self) noexcept
        {
            pyobj_handle pairs{iterable_iter(self)};
            if (!pairs)
            {
                return nullptr;
            }

            return wrap_mapping_iter(pairs.get());
        }

        PyObject* iterator_iter(PyObject* self) noexcept
        {
            return Py_NewRef(self);
        }

        /**
         * next() of an iterator.
         *
         * A WinRT iterator starts on its first item and is advanced after the
         * item is taken, so the end of the iteration is HasCurrent saying no,
         * which is a null return with no exception set.
         */
        PyObject* iterator_next(PyObject* self) noexcept
        {
            auto const info = entry_of(self);
            if (!info)
            {
                return nullptr;
            }

            // The three members take nothing and hand back one value each, so
            // where the table describes them the way IIterator<T> always does
            // they are called in one crossing rather than three. A table that
            // describes them some other way is answered below.
            bool fused{};

            auto* const item = call_iterator_step(*info, abi_of(self), fused);

            if (fused)
            {
                return item;
            }

            pyobj_handle has_current{call_protocol(
                info->protocol.has_current, "iteration", self, nullptr, 0)};
            if (!has_current)
            {
                return nullptr;
            }

            if (!PyObject_IsTrue(has_current.get()))
            {
                return nullptr;
            }

            pyobj_handle current{
                call_protocol(info->protocol.current, "iteration", self, nullptr, 0)};
            if (!current)
            {
                return nullptr;
            }

            pyobj_handle moved{
                call_protocol(info->protocol.move_next, "iteration", self, nullptr, 0)};
            if (!moved)
            {
                return nullptr;
            }

            return current.detach();
        }

        // ----- IStringable ------------------------------------------------

        PyObject* stringable_str(PyObject* self) noexcept
        {
            auto const info = entry_of(self);
            if (!info)
            {
                return nullptr;
            }

            return call_protocol(info->protocol.to_string, "str()", self, nullptr, 0);
        }

        // ----- awaitables -------------------------------------------------

        /**
         * Hands an async object the completed handler that the waiter is
         * about to wait on.
         *
         * The GIL is not held, so nothing here may touch Python: the argument
         * is one interface pointer, which goes into the buffer by hand rather
         * than through a conversion.
         */
        int32_t set_completed(void* context, void* async, void* handler) noexcept
        {
            auto const member = static_cast<member_desc*>(context);
            auto const overload = &member->overloads[member->count - 1];

            winrt::com_ptr<::IUnknown> queried;

            if (overload->iface)
            {
                void* iface{};
                auto const hr = static_cast<::IUnknown*>(async)->QueryInterface(
                    *static_cast<winrt::guid const*>(overload->iface), &iface);
                if (hr != 0)
                {
                    return hr;
                }

                queried.attach(static_cast<::IUnknown*>(iface));
            }

            auto const instance
                = queried ? queried.get() : static_cast<::IUnknown*>(async);

            alignas(std::max_align_t) uint8_t args[sizeof(void*)]{};
            std::memcpy(args + overload->args[0].offset, &handler, sizeof(handler));

            auto const vtable = *reinterpret_cast<void* const* const*>(instance);

            return overload->shape->invoke(vtable[overload->slot], instance, args);
        }

        /**
         * The IID of the completed handler of an async object, which is the
         * type of the value its Completed property takes.
         */
        void const* completed_handler_iid(member_desc const& completed) noexcept
        {
            auto const& setter = completed.overloads[completed.count - 1];
            auto const type = setter.args[0].type;

            if (type == table::no_ref)
            {
                return nullptr;
            }

            return completed.owner->table->type(type).guid();
        }

        /**
         * Waits for @p self to finish and returns the status of the wait.
         *
         * @returns @c false with a Python error set if the wait could not be
         * made at all.
         */
        bool wait_for_async(
            type_entry const& info,
            PyObject* self,
            uint32_t timeout_ms,
            int32_t& status) noexcept
        {
            auto const completed = info.protocol.completed;
            if (!completed)
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "'%s' cannot be waited for",
                    Py_TYPE(self)->tp_name);
                return false;
            }

            auto const iid = completed_handler_iid(*completed);
            if (!iid)
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "the table names no completed handler for '%s'",
                    Py_TYPE(self)->tp_name);
                return false;
            }

            if (set_sta_blocking_wait_error())
            {
                return false;
            }

            auto const abi = abi_of(self);

            {
                auto _gil = release_gil();
                status = async_wait_for(abi, timeout_ms, iid, set_completed, completed);
            }

            return true;
        }

        PyObject* async_get(PyObject* self, PyObject* /*unused*/) noexcept
        {
            auto const info = entry_of(self);
            if (!info)
            {
                return nullptr;
            }

            int32_t status{};

            if (!wait_for_async(*info, self, async_wait_forever, status))
            {
                return nullptr;
            }

            try
            {
                check_async_get(status);
            }
            catch (...)
            {
                to_PyErr();
                return nullptr;
            }

            if (!info->protocol.get_results)
            {
                Py_RETURN_NONE;
            }

            return call_protocol(info->protocol.get_results, "get()", self, nullptr, 0);
        }

        PyObject* async_wait(PyObject* self, PyObject* arg) noexcept
        {
            auto const info = entry_of(self);
            if (!info)
            {
                return nullptr;
            }

            auto const timeout = PyFloat_AsDouble(arg);
            if (timeout == -1.0 && PyErr_Occurred())
            {
                return nullptr;
            }

            int32_t status{};

            if (!wait_for_async(*info, self, async_timeout_ms(timeout), status))
            {
                return nullptr;
            }

            try
            {
                check_async_wait(status);
            }
            catch (...)
            {
                to_PyErr();
                return nullptr;
            }

            // The status is an AsyncStatus, and the member that reports one is
            // the Status property, so its return value is what says which
            // Python enum to build.
            auto const reported = info->protocol.status;
            if (!reported)
            {
                return PyLong_FromLong(status);
            }

            auto& arg_of_status = reported->overloads[0].args[0];

            return convert_out(*reported->owner, arg_of_status, &status);
        }

        PyMethodDef async_methods[]
            = {{"get", async_get, METH_NOARGS, nullptr},
               {"wait", async_wait, METH_O, nullptr},
               {}};

        // ----- closeables -------------------------------------------------

        PyObject* closeable_enter(PyObject* self, PyObject* /*unused*/) noexcept
        {
            return Py_NewRef(self);
        }

        PyObject* closeable_exit(
            PyObject* self, PyObject* const* /*args*/, Py_ssize_t /*nargs*/) noexcept
        {
            auto const info = entry_of(self);
            if (!info)
            {
                return nullptr;
            }

            // The three arguments say how the block ended, and Close takes the
            // object down either way, so none of them is read. Returning None
            // rather than a true value is what lets an exception propagate.
            pyobj_handle closed{call_protocol(
                info->protocol.close,
                "the context manager protocol",
                self,
                nullptr,
                0)};
            if (!closed)
            {
                return nullptr;
            }

            Py_RETURN_NONE;
        }

        PyMethodDef closeable_methods[]
            = {{"__enter__", closeable_enter, METH_NOARGS, nullptr},
               {"__exit__",
                reinterpret_cast<PyCFunction>(reinterpret_cast<void*>(closeable_exit)),
                METH_FASTCALL,
                nullptr},
               {}};

        // ----- buffers ----------------------------------------------------

        /**
         * Fills in @p view over @p size bytes at @p data, which @p self owns.
         */
        int fill_buffer_view(
            PyObject* self,
            Py_buffer* view,
            int flags,
            void* data,
            uint32_t size) noexcept
        {
            return PyBuffer_FillInfo(
                view, self, data, static_cast<Py_ssize_t>(size), 0, flags);
        }

        /**
         * __buffer__ of an IBuffer.
         *
         * This is the one protocol a table cannot describe. Where the bytes
         * start comes from IBufferByteAccess, and from IMemoryBufferByteAccess
         * for the other kind of buffer below - plain COM interfaces that the
         * metadata does not mention, so no member and no role can stand for
         * them. C++/WinRT holds both IIDs and uses them in data(), so both
         * halves of a buffer are read through it rather than through the
         * table, and all a type record says is which of the two shapes the
         * type has.
         */
        int ibuffer_view(PyObject* self, Py_buffer* view, int flags) noexcept
        {
            try
            {
                winrt::Windows::Storage::Streams::IBuffer buffer;
                winrt::copy_from_abi(buffer, abi_of(self));

                return fill_buffer_view(
                    self, view, flags, buffer.data(), buffer.Length());
            }
            catch (...)
            {
                view->obj = nullptr;
                to_PyErr();
                return -1;
            }
        }

        /**
         * __buffer__ of an IMemoryBufferReference, whose length is its
         * Capacity.
         */
        int memory_buffer_view(PyObject* self, Py_buffer* view, int flags) noexcept
        {
            try
            {
                winrt::Windows::Foundation::IMemoryBufferReference reference;
                winrt::copy_from_abi(reference, abi_of(self));

                return fill_buffer_view(
                    self, view, flags, reference.data(), reference.Capacity());
            }
            catch (...)
            {
                view->obj = nullptr;
                to_PyErr();
                return -1;
            }
        }

        /**
         * Whether @p record says the type implements @p flag.
         */
        bool implements(table::type_view const& record, uint32_t flag) noexcept
        {
            return (record.flags() & flag) != 0;
        }

        /**
         * Adds every method in a NULL terminated table to @p type.
         */
        bool add_methods(PyTypeObject* type, PyMethodDef* methods) noexcept
        {
            for (auto* method = methods; method->ml_name; method++)
            {
                pyobj_handle descriptor{PyDescr_NewMethod(type, method)};
                if (!descriptor)
                {
                    return false;
                }

                if (PyObject_SetAttrString(
                        reinterpret_cast<PyObject*>(type),
                        method->ml_name,
                        descriptor.get())
                    == -1)
                {
                    return false;
                }
            }

            return true;
        }
    } // namespace

    /**
     * Works out which member stands for each Python operation.
     *
     * The table gives each member a role, so this is only a matter of sorting
     * them: what makes GetAt the member behind v[i] is that IVector<T>
     * declares it, and the generator is where that is known. A type reached
     * through a class rather than through the interface itself has the same
     * roles on its own members, because they are the same members.
     */
    void find_protocol_members(type_entry& entry) noexcept
    {
        for (uint16_t i = 0; i < entry.member_count; i++)
        {
            auto& member = entry.members[i];

            for (uint16_t j = 0; j < member.count; j++)
            {
                switch (member.overloads[j].role)
                {
                case table::member_role::none:
                // The members that only the other direction calls: what
                // WinRT asks a Python list or dict standing in for a
                // collection, which pycollections.cpp reads off the role
                // itself rather than through a type's protocol.
                case table::member_role::get_view:
                case table::member_role::index_of:
                case table::member_role::append:
                case table::member_role::remove_at_end:
                case table::member_role::clear:
                case table::member_role::replace_all:
                case table::member_role::split:
                case table::member_role::pair_key:
                case table::member_role::pair_value:
                    continue;
                case table::member_role::size:
                    entry.protocol.size = &member;
                    break;
                case table::member_role::get_at:
                    entry.protocol.get_at = &member;
                    break;
                case table::member_role::get_many:
                    entry.protocol.get_many = &member;
                    break;
                case table::member_role::set_at:
                    entry.protocol.set_at = &member;
                    break;
                case table::member_role::remove_at:
                    entry.protocol.remove_at = &member;
                    break;
                case table::member_role::insert_at:
                    entry.protocol.insert_at = &member;
                    break;
                case table::member_role::first:
                    entry.protocol.first = &member;
                    break;
                case table::member_role::current:
                    entry.protocol.current = &member;
                    break;
                case table::member_role::has_current:
                    entry.protocol.has_current = &member;
                    break;
                case table::member_role::move_next:
                    entry.protocol.move_next = &member;
                    break;
                case table::member_role::lookup:
                    entry.protocol.lookup = &member;
                    break;
                case table::member_role::has_key:
                    entry.protocol.has_key = &member;
                    break;
                case table::member_role::insert:
                    entry.protocol.insert = &member;
                    break;
                case table::member_role::remove:
                    entry.protocol.remove = &member;
                    break;
                case table::member_role::status:
                    entry.protocol.status = &member;
                    break;
                case table::member_role::completed:
                    entry.protocol.completed = &member;
                    break;
                case table::member_role::get_results:
                    entry.protocol.get_results = &member;
                    break;
                case table::member_role::to_string:
                    entry.protocol.to_string = &member;
                    break;
                case table::member_role::value:
                    entry.protocol.value = &member;
                    break;
                case table::member_role::close:
                    entry.protocol.close = &member;
                    break;
                }
            }
        }
    }

    /**
     * Appends the type slots of the protocols @p record says the type
     * implements, which a type spec needs before the type exists.
     */
    void add_protocol_slots(
        table::type_view const& record, std::vector<PyType_Slot>& slots)
    {
        if (implements(record, table::type_flags::mapping))
        {
            slots.push_back({Py_mp_length, reinterpret_cast<void*>(protocol_length)});
            slots.push_back(
                {Py_mp_subscript, reinterpret_cast<void*>(mapping_subscript)});
            slots.push_back(
                {Py_sq_contains, reinterpret_cast<void*>(mapping_contains)});
            slots.push_back({Py_tp_iter, reinterpret_cast<void*>(mapping_iter)});

            if (implements(record, table::type_flags::mutable_mapping))
            {
                slots.push_back(
                    {Py_mp_ass_subscript, reinterpret_cast<void*>(mapping_assign)});
            }
        }
        else if (implements(record, table::type_flags::sequence))
        {
            slots.push_back({Py_sq_length, reinterpret_cast<void*>(protocol_length)});
            slots.push_back({Py_sq_item, reinterpret_cast<void*>(sequence_item)});
            slots.push_back(
                {Py_mp_subscript, reinterpret_cast<void*>(sequence_subscript)});
            slots.push_back({Py_tp_iter, reinterpret_cast<void*>(iterable_iter)});

            if (implements(record, table::type_flags::mutable_sequence))
            {
                slots.push_back(
                    {Py_sq_ass_item, reinterpret_cast<void*>(sequence_assign)});
            }
        }
        else if (implements(record, table::type_flags::iterator))
        {
            slots.push_back({Py_tp_iter, reinterpret_cast<void*>(iterator_iter)});
            slots.push_back({Py_tp_iternext, reinterpret_cast<void*>(iterator_next)});
        }
        else if (implements(record, table::type_flags::iterable))
        {
            slots.push_back({Py_tp_iter, reinterpret_cast<void*>(iterable_iter)});
        }

        if (implements(record, table::type_flags::stringable))
        {
            slots.push_back({Py_tp_str, reinterpret_cast<void*>(stringable_str)});
        }

        if (implements(record, table::type_flags::awaitable))
        {
            slots.push_back({Py_am_await, reinterpret_cast<void*>(await_async)});
        }

        if (implements(record, table::type_flags::buffer))
        {
            // Which of the two the type is decides both where the bytes
            // come from and what says how many there are, so it picks the
            // slot rather than being asked again on every view.
            slots.push_back(
                {Py_bf_getbuffer,
                 implements(record, table::type_flags::buffer_length)
                     ? reinterpret_cast<void*>(ibuffer_view)
                     : reinterpret_cast<void*>(memory_buffer_view)});
        }
    }

    /**
     * Sets the attributes of the protocols that are methods rather than slots:
     * get() and wait() on an awaitable, and the context manager that closes an
     * IClosable at the end of a with statement.
     */
    bool bind_protocol_methods(table::type_view const& record, PyTypeObject* type)
    {
        if (implements(record, table::type_flags::awaitable))
        {
            if (!add_methods(type, async_methods))
            {
                return false;
            }
        }

        if (implements(record, table::type_flags::closeable))
        {
            if (!add_methods(type, closeable_methods))
            {
                return false;
            }
        }

        return true;
    }
} // namespace py::interp
