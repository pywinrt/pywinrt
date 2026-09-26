// A Python callable as a WinRT delegate.
//
// A delegate is the simplest thing a call from WinRT can arrive at: one vtable
// entry, Invoke, and one Python object to call. The vtable belongs to the
// delegate's table entry rather than to any one object, because nothing in it
// depends on which callable is behind it, and what to make of the arguments
// the trampoline spilled is callbacks.cpp's business.
//
// The object is a plain COM object rather than anything C++/WinRT makes,
// because a runtime that reads a signature out of a table cannot name the C++
// type of a delegate. It answers IUnknown, its own IID, IAgileObject and
// IMarshal, which is what C++/WinRT's own delegates answer: a delegate that is
// not agile still works, and a caller that has to marshal to reach it pays for
// that on every invoke.

#include <Python.h>

#include <pywinrt/base.h>

#include "callbacks.h"
#include "delegates.h"
#include "interp.h"
#include "members.h"
#include "metaclass.h"
#include "objects.h"
#include "types.h"

namespace py::interp
{
    namespace
    {
        /**
         * A Python callable seen from WinRT.
         */
        struct delegate_object final : shapes::reverse_target
        {
            shapes::com_head head;
            std::atomic<uint32_t> references{1};
            /// The delegate's table entry, which owns the vtable and the
            /// descriptor of its Invoke.
            type_entry* info;
            PyObject* callable;

            int32_t invoke(uint16_t slot, void* args) noexcept override;
        };

        /**
         * The delegate a WinRT caller is holding an interface pointer to.
         */
        delegate_object* delegate_of(void* self) noexcept
        {
            return static_cast<delegate_object*>(
                static_cast<shapes::com_head*>(self)->target);
        }

        uint32_t __stdcall delegate_add_ref(void* self) noexcept
        {
            return ++delegate_of(self)->references;
        }

        uint32_t __stdcall delegate_release(void* self) noexcept
        {
            auto* const obj = delegate_of(self);

            auto const remaining = --obj->references;
            if (remaining != 0)
            {
                return remaining;
            }

            {
                // WinRT lets go of a delegate on whatever thread it happens to
                // be on, and letting go of the callable can run a Python
                // finalizer.
                auto gil = ensure_gil();
                Py_CLEAR(obj->callable);
            }

            delete obj;

            return 0;
        }

        int32_t __stdcall delegate_query_interface(
            void* self, winrt::guid const& id, void** object) noexcept
        {
            auto* const obj = delegate_of(self);

            if (id == winrt::guid_of<winrt::Windows::Foundation::IUnknown>()
                || id == winrt::guid_of<winrt::impl::IAgileObject>()
                || id == *static_cast<winrt::guid const*>(obj->info->guid))
            {
                *object = self;
                delegate_add_ref(self);

                return 0;
            }

            if (id == winrt::guid_of<winrt::impl::IMarshal>())
            {
                return winrt::impl::make_marshaler(
                    static_cast<winrt::impl::unknown_abi*>(self), object);
            }

            *object = nullptr;

            return winrt::impl::error_no_interface;
        }

        /**
         * Calling a delegate that came back from WinRT, which is an ordinary
         * forward call through the one vtable slot it has.
         */
        PyObject* delegate_call(PyObject* self, PyObject* args, PyObject* kwds) noexcept
        {
            if (kwds && PyDict_GET_SIZE(kwds) != 0)
            {
                set_invalid_kwd_args_error();
                return nullptr;
            }

            auto const info = get_type_entry(Py_TYPE(self));
            if (!info || !info->members)
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "'%s' is not a delegate this runtime built",
                    Py_TYPE(self)->tp_name);
                return nullptr;
            }

            auto const nargs = PyTuple_GET_SIZE(args);

            auto const overload = select_overload(info->members[0], nargs);
            if (!overload)
            {
                return nullptr;
            }

            return call_member(
                info->members[0],
                *overload,
                abi_of(self),
                nargs == 0 ? nullptr : &PyTuple_GET_ITEM(args, 0),
                nargs);
        }

        int32_t delegate_object::invoke(uint16_t slot, void* args) noexcept
        {
            auto const& entry = info->reverse->slots[slot];

            if (!entry.member)
            {
                return winrt::impl::error_not_implemented;
            }

            auto gil = ensure_gil();

            return call_python(
                *entry.member, *entry.overload, callable, python_op::invoke, args);
        }
    } // namespace

    /**
     * Builds what a delegate record stands for: its IID, the descriptor of its
     * Invoke, and the vtable a Python callable is handed to WinRT behind.
     */
    bool make_delegate_type(
        projection& proj, type_entry& entry, table::type_view const& record)
    {
        entry.guid = record.guid();

        if (!entry.guid)
        {
            PyErr_Format(
                PyExc_TypeError,
                "'%s' has no IID, so nothing can be called through it",
                entry.winrt_name);
            return false;
        }

        type_members collected;

        if (!collect_members(proj, entry, record, collected))
        {
            return false;
        }

        if (entry.member_count != 1 || entry.members[0].count != 1)
        {
            PyErr_Format(
                PyExc_ImportError,
                "the table does not describe the Invoke of '%s'",
                entry.winrt_name);
            return false;
        }

        auto const shape = get_reverse_shape(record.group(0).member(0).reverse_shape());
        if (!shape)
        {
            // Said here rather than left to get_reverse_shape, which cannot
            // tell a table that asks for a trampoline past the end from one
            // that was generated without any.
            PyErr_Format(
                PyExc_NotImplementedError,
                "a Python callable cannot be used as '%s' because its Invoke has a "
                "signature this winrt-runtime has no callback for",
                entry.winrt_name);
            return false;
        }

        auto reverse = std::make_unique<reverse_vtable>();

        reverse->entries
            = {reinterpret_cast<shapes::vtable_entry>(&delegate_query_interface),
               reinterpret_cast<shapes::vtable_entry>(&delegate_add_ref),
               reinterpret_cast<shapes::vtable_entry>(&delegate_release),
               shape->entry};

        reverse->slots.resize(reverse->entries.size());
        reverse->slots[shape->slot]
            = {&entry.members[0], &entry.members[0].overloads[0], python_op::invoke};

        entry.reverse = std::move(reverse);

        // A delegate that comes the other way - out of a member, or into a
        // Python implementation of one that takes it - is a callable that
        // invokes the delegate behind it. Nothing binds the type: the module
        // binds that name to a typing alias.
        auto const object_type = get_object_type();
        if (!object_type)
        {
            return false;
        }

        pyobj_handle bases{PyTuple_Pack(1, object_type)};
        if (!bases)
        {
            return false;
        }

        PyType_Slot slots[]
            = {{Py_tp_call, reinterpret_cast<void*>(delegate_call)}, {}};

        PyType_Spec spec{
            entry.tp_name.c_str(),
            static_cast<int>(object_basicsize),
            0,
            Py_TPFLAGS_DEFAULT,
            slots};

        pytype_handle type{reinterpret_cast<PyTypeObject*>(
            PyType_FromMetaclass(nullptr, proj.module, &spec, bases.get()))};
        if (!type)
        {
            return false;
        }

        entry.py_type = type.detach();

        return remember(entry, entry.py_type);
    }

    /**
     * The WinRT delegate of type @p info that calls @p callable, with one
     * reference on it.
     *
     * @returns @c nullptr with a Python error set.
     */
    void* make_delegate(type_entry& info, PyObject* callable) noexcept
    {
        if (!PyCallable_Check(callable))
        {
            PyErr_Format(
                PyExc_TypeError,
                "'%s' takes a callable, not a '%s'",
                info.winrt_name,
                Py_TYPE(callable)->tp_name);
            return nullptr;
        }

        auto* const self = new (std::nothrow) delegate_object{};
        if (!self)
        {
            PyErr_NoMemory();
            return nullptr;
        }

        self->head.vtable = info.reverse->entries.data();
        self->head.target = self;
        self->info = &info;
        self->callable = Py_NewRef(callable);

        return &self->head;
    }
} // namespace py::interp
