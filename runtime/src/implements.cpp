// A Python object that implements WinRT interfaces.
//
// One COM object stands for the Python object and holds a tearoff per
// interface it implements, each with a vtable of its own: the six entries
// every WinRT interface starts with, and then one reverse trampoline per
// method, chosen by the shape the generator recorded. The vtable belongs to
// the interface's table entry rather than to any one object, the same way a
// delegate's does.
//
// The reference between the two objects is one way. WinRT holds the Python
// object and Python holds nothing back, because a Python object never sees
// the COM object that stands for it - it is made when the object is passed to
// WinRT and let go of when WinRT is done with it. That is what makes this the
// simple half of the pair; compose.cpp has the other, where the two objects
// refer to each other and the reference has to toggle.
//
// The object and its tearoffs are also what pycollections.cpp builds a Python
// list or dict into, so what a call that arrives comes down to is a
// reverse_call the object holds rather than anything written in here. The one
// in this file calls a method of the Python object.

#include <Python.h>

#include <pywinrt/base.h>

#include "callbacks.h"
#include "implements.h"
#include "interp.h"
#include "module_state.h"
#include "types.h"

namespace py::interp
{
    namespace
    {
        /**
         * The WinRT object that stands for Python state.
         */
        struct reverse_object final : shapes::reverse_target
        {
            /// IInspectable, which is also what IUnknown and IAgileObject
            /// answer with.
            shapes::com_head head;
            /// py::IPywinrtObject, which is how a value that comes back to
            /// Python is recognised as having started there.
            shapes::com_head py_head;
            std::atomic<uint32_t> references{1};
            std::unique_ptr<reverse_call> calls;
            /// Reserved once and never added to again, because a WinRT caller
            /// holds a pointer into it.
            std::vector<tearoff> tearoffs;

            int32_t invoke(uint16_t slot, void* args) noexcept override;
        };

        reverse_object* object_of(void* self) noexcept
        {
            return static_cast<reverse_object*>(
                static_cast<shapes::com_head*>(self)->target);
        }

        /**
         * A Python object that implements interfaces, which answers a call by
         * calling one of its own methods.
         */
        struct method_calls final : reverse_call
        {
            explicit method_calls(PyObject* obj) noexcept : obj{Py_NewRef(obj)}
            {
            }

            ~method_calls() override
            {
                Py_CLEAR(obj);
            }

            int32_t run(reverse_slot const& slot, void* args) noexcept override
            {
                return call_python(*slot.member, *slot.overload, obj, slot.op, args);
            }

            PyObject* target() const noexcept override
            {
                return obj;
            }

            PyObject* obj;
        };

        uint32_t __stdcall object_add_ref(void* self) noexcept
        {
            return ++object_of(self)->references;
        }

        uint32_t __stdcall object_release(void* self) noexcept
        {
            auto* const owner = object_of(self);

            auto const remaining = --owner->references;
            if (remaining != 0)
            {
                return remaining;
            }

            // WinRT lets go of the object on whatever thread it happens to be
            // on, and letting go of the Python state in it can run a
            // finalizer.
            auto gil = ensure_gil();

            delete owner;

            return 0;
        }

        int32_t __stdcall object_query_interface(
            void* self, winrt::guid const& id, void** object) noexcept
        {
            auto* const owner = object_of(self);

            if (id == winrt::guid_of<winrt::Windows::Foundation::IUnknown>()
                || id == winrt::guid_of<winrt::Windows::Foundation::IInspectable>()
                || id == winrt::guid_of<winrt::impl::IAgileObject>())
            {
                *object = &owner->head;
                object_add_ref(self);

                return 0;
            }

            if (id == winrt::guid_of<py::IPywinrtObject>() && owner->calls->target())
            {
                *object = &owner->py_head;
                object_add_ref(self);

                return 0;
            }

            if (id == winrt::guid_of<winrt::impl::IMarshal>())
            {
                return winrt::impl::make_marshaler(
                    reinterpret_cast<winrt::impl::unknown_abi*>(&owner->head), object);
            }

            for (auto& iface : owner->tearoffs)
            {
                if (id == *static_cast<winrt::guid const*>(iface.info->guid))
                {
                    *object = &iface.head;
                    object_add_ref(self);

                    return 0;
                }
            }

            *object = nullptr;

            return winrt::impl::error_no_interface;
        }

        int32_t __stdcall object_get_iids(
            void* self, uint32_t* count, winrt::guid** array) noexcept
        {
            auto* const owner = object_of(self);
            auto const size = owner->tearoffs.size();

            *count = 0;
            *array = nullptr;

            if (size == 0)
            {
                return 0;
            }

            auto* const iids
                = static_cast<winrt::guid*>(CoTaskMemAlloc(size * sizeof(winrt::guid)));
            if (!iids)
            {
                return winrt::impl::error_bad_alloc;
            }

            for (size_t i = 0; i < size; i++)
            {
                iids[i]
                    = *static_cast<winrt::guid const*>(owner->tearoffs[i].info->guid);
            }

            *count = static_cast<uint32_t>(size);
            *array = iids;

            return 0;
        }

        int32_t __stdcall object_get_runtime_class_name(
            void* self, void** name) noexcept
        {
            auto* const owner = object_of(self);

            // C++/WinRT answers with the name of the first interface for an
            // object that is not a runtime class, and so does this. What a
            // Python subclass of a composable class answers is the composable
            // one's business.
            auto const first = owner->tearoffs.empty()
                                   ? "Windows.Foundation.IInspectable"
                                   : owner->tearoffs.front().info->winrt_name;

            try
            {
                *name = winrt::detach_abi(winrt::to_hstring(first));
            }
            catch (...)
            {
                *name = nullptr;
                return winrt::to_hresult();
            }

            return 0;
        }

        int32_t __stdcall object_get_trust_level(
            void* /*self*/, int32_t* level) noexcept
        {
            *level = 0;

            return 0;
        }

        int32_t __stdcall object_get_py_object(void* self, PyObject*& result) noexcept
        {
            auto gil = ensure_gil();

            result = Py_NewRef(object_of(self)->calls->target());

            return 0;
        }

        int32_t __stdcall object_get_composable_inner(
            void* /*self*/,
            winrt::Windows::Foundation::IInspectable& /*inner*/) noexcept
        {
            // Only a Python subclass of a composable class has an inner
            // object; an object that merely implements interfaces has
            // nothing behind it.
            return winrt::impl::error_not_implemented;
        }

        /**
         * IInspectable, which is what a Python object that implements
         * interfaces is when nothing has asked for one of them yet.
         */
        shapes::vtable_entry const inspectable_vtable[]
            = {reinterpret_cast<shapes::vtable_entry>(&object_query_interface),
               reinterpret_cast<shapes::vtable_entry>(&object_add_ref),
               reinterpret_cast<shapes::vtable_entry>(&object_release),
               reinterpret_cast<shapes::vtable_entry>(&object_get_iids),
               reinterpret_cast<shapes::vtable_entry>(&object_get_runtime_class_name),
               reinterpret_cast<shapes::vtable_entry>(&object_get_trust_level)};

        shapes::vtable_entry const pywinrt_object_vtable[]
            = {reinterpret_cast<shapes::vtable_entry>(&object_query_interface),
               reinterpret_cast<shapes::vtable_entry>(&object_add_ref),
               reinterpret_cast<shapes::vtable_entry>(&object_release),
               reinterpret_cast<shapes::vtable_entry>(&object_get_py_object),
               reinterpret_cast<shapes::vtable_entry>(&object_get_composable_inner)};

        // ----- the tearoffs ------------------------------------------------

        tearoff* tearoff_of(void* self) noexcept
        {
            return static_cast<tearoff*>(static_cast<shapes::com_head*>(self)->target);
        }

        /**
         * The object a tearoff belongs to, as the WinRT caller that holds the
         * object itself sees it.
         *
         * A tearoff answers the six entries every WinRT interface starts with
         * the way the object does, and the object may be one this file
         * assembled or one compose.cpp did, so it is entered through its own
         * vtable rather than called directly.
         */
        winrt::impl::inspectable_abi* owner_of(void* self) noexcept
        {
            return reinterpret_cast<winrt::impl::inspectable_abi*>(
                tearoff_of(self)->owner);
        }

        int32_t __stdcall tearoff_query_interface(
            void* self, winrt::guid const& id, void** object) noexcept
        {
            return owner_of(self)->QueryInterface(id, object);
        }

        uint32_t __stdcall tearoff_add_ref(void* self) noexcept
        {
            return owner_of(self)->AddRef();
        }

        uint32_t __stdcall tearoff_release(void* self) noexcept
        {
            return owner_of(self)->Release();
        }

        int32_t __stdcall tearoff_get_iids(
            void* self, uint32_t* count, winrt::guid** array) noexcept
        {
            return owner_of(self)->GetIids(count, array);
        }

        int32_t __stdcall tearoff_get_runtime_class_name(
            void* self, void** name) noexcept
        {
            return owner_of(self)->GetRuntimeClassName(name);
        }

        int32_t __stdcall tearoff_get_trust_level(void* self, int32_t* level) noexcept
        {
            return owner_of(self)->GetTrustLevel(
                reinterpret_cast<winrt::Windows::Foundation::TrustLevel*>(level));
        }

        int32_t reverse_object::invoke(uint16_t /*slot*/, void* /*args*/) noexcept
        {
            // The object's own two vtables are IInspectable's and
            // IPywinrtObject's, and every entry of both is a function of its
            // own above, so no trampoline ever dispatches here.
            return winrt::impl::error_not_implemented;
        }

        // ----- assembling one interface's vtable ---------------------------

        /**
         * Which Python operation the member at @p slot comes down to.
         */
        python_op operation_of(member_desc const& member, uint16_t index) noexcept
        {
            if (member.kind != table::group_kind::property)
            {
                // A method, and an event, whose two halves the projection
                // spells as the methods add_x() and remove_x().
                return python_op::call_method;
            }

            // A property group holds its getter first and its setter, if
            // there is one, second.
            return index == 0 ? python_op::get_attribute : python_op::set_attribute;
        }

        /**
         * Whether @p entry's own vtable is where @p overload belongs, read off
         * the descriptor rather than off the table record beside it.
         *
         * A member read from the interface that declares it names no interface
         * to query, because the object is already holding that one; a member
         * read from anything else - a class, or an interface that requires
         * this one - names the interface it is reached through.
         */
        bool declared_by(
            overload_desc const& overload,
            type_entry const& entry,
            type_entry const& source) noexcept
        {
            if (!overload.iface)
            {
                return &entry == &source;
            }

            return *static_cast<winrt::guid const*>(overload.iface)
                   == *static_cast<winrt::guid const*>(entry.guid);
        }

        /**
         * The interface @p type is the abstract type of, or @c nullptr.
         *
         * Every projected interface has two Python types: the wrapper a value
         * of it comes back as, bound to the underscored name, and the
         * abstract type a Python implementation derives from, bound to the
         * public one. Only the second says anything about what the object
         * behind it can answer.
         */
        type_entry* interface_of(PyTypeObject* type) noexcept
        {
            auto* const entry = find_type_entry(type);

            return entry && entry->implements == type ? entry : nullptr;
        }
    } // namespace

    int32_t tearoff::invoke(uint16_t slot, void* args) noexcept
    {
        auto const& entry = info->reverse->slots[slot];

        if (!entry.member)
        {
            return winrt::impl::error_not_implemented;
        }

        auto gil = ensure_gil();

        return calls->run(entry, args);
    }

    /**
     * Gives @p tearoffs one entry per interface in @p interfaces, all of them
     * forwarding to @p owner and answering through @p calls.
     *
     * Each interface's vtable has to have been built already. The vector is
     * never added to again once this has run, because a WinRT caller holds a
     * pointer into it.
     */
    void make_tearoffs(
        std::span<type_entry* const> interfaces,
        shapes::com_head& owner,
        reverse_call& calls,
        std::vector<tearoff>& tearoffs)
    {
        tearoffs.reserve(interfaces.size());

        for (auto* const entry : interfaces)
        {
            auto& iface = tearoffs.emplace_back();

            iface.head.vtable = entry->reverse->entries.data();
            iface.head.target = &iface;
            iface.owner = &owner;
            iface.calls = &calls;
            iface.info = entry;
        }
    }

    /**
     * Every interface @p type implements, and everything those require, in the
     * order to try them.
     */
    bool collect_interfaces(PyTypeObject* type, std::vector<type_entry*>& found)
    {
        auto* const mro = type->tp_mro;
        if (!mro)
        {
            return true;
        }

        auto const add = [&found](type_entry* entry)
        {
            if (std::find(found.begin(), found.end(), entry) == found.end())
            {
                found.push_back(entry);
            }
        };

        for (Py_ssize_t i = 0; i < PyTuple_GET_SIZE(mro); i++)
        {
            auto* const base = PyTuple_GET_ITEM(mro, i);
            if (!PyType_Check(base))
            {
                continue;
            }

            auto* const entry = interface_of(reinterpret_cast<PyTypeObject*>(base));
            if (!entry)
            {
                continue;
            }

            add(entry);

            // An interface lists what it requires transitively, and an object
            // that answers one has to answer all of them.
            auto const required = entry->owner->table->type(entry->index).interfaces();

            for (uint32_t j = 0; j < required.size(); j++)
            {
                type_entry* cache{};

                auto* const other = resolve(*entry->owner, required[j], cache);
                if (!other)
                {
                    return false;
                }

                add(other);
            }
        }

        return true;
    }

    /**
     * Builds the vtable a WinRT caller enters @p entry's interface through,
     * once per interface, from the members of @p source that name @p entry as
     * what declares them.
     *
     * Its entries are the reverse trampolines that the shape ids in the table
     * name. What each of them comes down to on the Python side is the object's
     * business and not the interface's, so one vtable serves a class that
     * implements the interface and a list that stands in for one.
     *
     * The source is the interface itself for one the table describes. An
     * interface that is exclusive to a class - which every overridable one is
     * - is written down as a name and an IID, because the class redeclares
     * every member of it, so for one of those the source is that class.
     */
    bool ensure_interface_vtable(type_entry& entry, type_entry& source)
    {
        auto const s = py::cpp::_winrt::get_module_state();
        if (!s)
        {
            return false;
        }

        // Built the first time a Python object stands in for the interface,
        // on whichever thread that is.
        py::cpp::_winrt::build_guard const guard{s->build_lock};

        if (entry.reverse)
        {
            return true;
        }

        if (!entry.guid)
        {
            PyErr_Format(
                PyExc_NotImplementedError,
                "'%s' has no IID, so Python cannot implement it",
                entry.winrt_name);
            return false;
        }

        auto const record = source.owner->table->type(source.index);

        auto reverse = std::make_unique<reverse_vtable>();

        reverse->entries
            = {reinterpret_cast<shapes::vtable_entry>(&tearoff_query_interface),
               reinterpret_cast<shapes::vtable_entry>(&tearoff_add_ref),
               reinterpret_cast<shapes::vtable_entry>(&tearoff_release),
               reinterpret_cast<shapes::vtable_entry>(&tearoff_get_iids),
               reinterpret_cast<shapes::vtable_entry>(&tearoff_get_runtime_class_name),
               reinterpret_cast<shapes::vtable_entry>(&tearoff_get_trust_level)};

        // An interface's own vtable holds only its own methods: the ones it
        // inherits, and the ones a class reaches through its other interfaces,
        // are reached through a vtable of their own, so the record lists them -
        // the wrapper type has to bind them all - at slots that are this
        // interface's slots for something else.
        for (uint32_t i = 0; i < record.group_count(); i++)
        {
            auto const group = record.group(i);

            for (uint32_t j = 0; j < group.member_count(); j++)
            {
                auto const member = group.member(j);

                if (member.declaring() != entry.index)
                {
                    continue;
                }

                auto const shape = get_reverse_shape(member.reverse_shape());
                if (!shape)
                {
                    // Refusing the whole interface rather than leaving one
                    // entry out: a vtable with a hole in it is a crash
                    // waiting for the caller that reaches for it.
                    PyErr_Format(
                        PyExc_NotImplementedError,
                        "'%s' cannot be implemented in Python because "
                        "'%s' has a signature this winrt-runtime has no "
                        "callback for",
                        entry.winrt_name,
                        member.winrt_name().data());
                    return false;
                }

                if (shape->slot >= reverse->entries.size())
                {
                    reverse->entries.resize(shape->slot + 1);
                }

                reverse->entries[shape->slot] = shape->entry;
            }
        }

        for (size_t slot = 0; slot < reverse->entries.size(); slot++)
        {
            if (!reverse->entries[slot])
            {
                PyErr_Format(
                    PyExc_ImportError,
                    "'%s' names no member for vtable slot %zu",
                    entry.winrt_name,
                    slot);
                return false;
            }
        }

        reverse->slots.resize(reverse->entries.size());

        for (uint16_t i = 0; i < source.member_count; i++)
        {
            auto& member = source.members[i];

            for (uint16_t j = 0; j < member.count; j++)
            {
                auto& overload = member.overloads[j];

                // The same filter as above, read off the descriptor.
                if (!declared_by(overload, entry, source))
                {
                    continue;
                }

                if (overload.slot >= reverse->slots.size())
                {
                    PyErr_Format(
                        PyExc_ImportError,
                        "'%s' says '%s' is in vtable slot %u, which it does "
                        "not have",
                        entry.winrt_name,
                        overload.winrt_name,
                        overload.slot);
                    return false;
                }

                reverse->slots[overload.slot]
                    = {&member, &overload, operation_of(member, j)};
            }
        }

        entry.reverse = std::move(reverse);

        return true;
    }

    /**
     * The Python object @p abi started as, or @c nullptr when it did not start
     * in Python at all.
     *
     * A Python object handed to WinRT is given to it as a COM object that
     * stands for it, and py::IPywinrtObject is how that object is recognised
     * on the way back: an object that implements interfaces, a list or dict
     * read as a collection and a subclass of a composable class all answer it,
     * so a value that made the round trip is the object that went out.
     */
    PyObject* python_object_of(void* abi) noexcept
    {
        winrt::com_ptr<py::IPywinrtObject> started;

        if (static_cast<::IUnknown*>(abi)->QueryInterface(
                winrt::guid_of<py::IPywinrtObject>(), started.put_void())
            != 0)
        {
            return nullptr;
        }

        PyObject* obj{};

        if (started->GetPyObject(obj) != 0)
        {
            return nullptr;
        }

        return obj;
    }

    /**
     * Whether @p type derives from the abstract type of any projected
     * interface, which is how a Python class says it implements one.
     */
    bool implements_interfaces(PyTypeObject* type) noexcept
    {
        auto* const mro = type->tp_mro;
        if (!mro)
        {
            return false;
        }

        for (Py_ssize_t i = 0; i < PyTuple_GET_SIZE(mro); i++)
        {
            auto* const base = PyTuple_GET_ITEM(mro, i);

            if (PyType_Check(base)
                && interface_of(reinterpret_cast<PyTypeObject*>(base)))
            {
                return true;
            }
        }

        return false;
    }

    /**
     * The @p iid interface of a WinRT object that answers every interface in
     * @p interfaces by running @p calls, with one reference on it.
     *
     * Each interface's vtable has to have been built already, because a
     * missing one is a Python error and this hands back an HRESULT.
     *
     * @throws winrt::hresult_error if none of @p interfaces answers @p iid.
     */
    void* make_reverse_object(
        std::span<type_entry* const> interfaces,
        void const* iid,
        std::unique_ptr<reverse_call> calls)
    {
        auto owner = std::make_unique<reverse_object>();

        owner->head.vtable = inspectable_vtable;
        owner->head.target = owner.get();
        owner->py_head.vtable = pywinrt_object_vtable;
        owner->py_head.target = owner.get();
        owner->calls = std::move(calls);

        make_tearoffs(interfaces, owner->head, *owner->calls, owner->tearoffs);

        void* result{};

        auto const hr = object_query_interface(
            &owner->head,
            iid ? *static_cast<winrt::guid const*>(iid)
                : winrt::guid_of<winrt::Windows::Foundation::IInspectable>(),
            &result);

        // The object holds the only reference to itself until a caller takes
        // one, so letting go of it here is what gives it back.
        object_release(&owner.release()->head);

        if (hr != 0)
        {
            winrt::check_hresult(hr);
        }

        return result;
    }

    /**
     * The @p iid interface of the WinRT object that stands for @p obj, with
     * one reference on it.
     *
     * A new object is made for each call, because a Python object holds no
     * reference back to one and so has nowhere to keep it. Two of them are
     * therefore not equal to each other, which is what a Python subclass of a
     * composable class - where the pair does refer to each other both ways -
     * will not do.
     *
     * @throws python_exception if @p obj implements no interface that answers
     * @p iid.
     */
    void* make_implements_object(PyObject* obj, void const* iid)
    {
        std::vector<type_entry*> interfaces;

        if (!collect_interfaces(Py_TYPE(obj), interfaces))
        {
            throw python_exception();
        }

        for (auto* const entry : interfaces)
        {
            if (!ensure_interface_vtable(*entry))
            {
                throw python_exception();
            }
        }

        return make_reverse_object(
            interfaces, iid, std::make_unique<method_calls>(obj));
    }
} // namespace py::interp
