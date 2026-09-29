// A Python subclass of a composable WinRT class.
//
// Deriving from a composable class makes a pair of objects. The outer one is
// assembled here: WinRT enters it for everything the class can be asked, it
// answers the overridable interfaces - and any interface the Python class says
// it implements - from Python through implements.cpp's tearoffs, and hands
// everything else to the inner object that the composable factory made when it
// composed the class into this one.
//
// The reference between the two objects is a toggle rather than a plain count.
// The Python object owns the outer and the outer only points back at it while
// nothing but Python is holding the pair, so that the two of them are an
// ordinary Python object as far as the garbage collector is concerned; the
// moment WinRT takes a reference the outer takes one on the Python object too,
// and the pair is kept alive from the WinRT side until it lets go again.
//
// The outer is also a weak reference source, because XAML holds its elements
// weakly: the strong count and the weak count sit in the object itself, and
// the memory outlives the strong count by as long as a weak reference to it
// does.

#include <Python.h>

#include <pywinrt/base.h>

#include "callbacks.h"
#include "compose.h"
#include "implements.h"
#include "interp.h"
#include "objects.h"
#include "types.h"

#include <algorithm>
#include <atomic>
#include <memory>
#include <vector>

namespace py::interp
{
    namespace
    {
        /**
         * A Python object composed into a WinRT one, which answers an
         * overridable member by calling the method the projection binds it to.
         *
         * The reference to the Python object belongs to the pair rather than
         * to this: the outer object holds it while WinRT is holding the pair
         * and only points at it the rest of the time, so nothing is held here.
         */
        struct composed_calls final : reverse_call
        {
            int32_t run(reverse_slot const& slot, void* args) noexcept override
            {
                return call_python(*slot.member, *slot.overload, obj, slot.op, args);
            }

            PyObject* target() const noexcept override
            {
                return obj;
            }

            /// Borrowed - see the class comment - and null once the pair has
            /// been taken apart.
            PyObject* obj;
        };

        /**
         * The WinRT object a Python class is composed into.
         */
        struct compose_object final : shapes::reverse_target
        {
            /// IInspectable, which is also what IUnknown and IAgileObject
            /// answer with.
            shapes::com_head head;
            /// py::IPywinrtObject, which is how a value that comes back to
            /// Python is recognised as having started there, and how a call on
            /// an overridable member finds the inner object.
            shapes::com_head py_head;
            /// IWeakReferenceSource, which is the object itself seen as one.
            shapes::com_head source_head;
            /// IWeakReference, which outlives the rest of the object.
            shapes::com_head weak_head;

            std::atomic<uint32_t> references{1};
            /// Weak references, plus one the object holds itself for as long
            /// as it has any strong ones.
            std::atomic<uint32_t> weak_references{1};
            /// Held over every change of the count that crosses between one
            /// and two or reaches zero, together with what that change does
            /// to the Python object.
            winrt::slim_mutex lock;

            composed_calls calls;
            /// The object the class was composed over, which answers
            /// everything the Python class does not.
            winrt::Windows::Foundation::IInspectable inner;
            /// What GetRuntimeClassName answers with.
            winrt::hstring class_name;
            /// Reserved once and never added to again, because a WinRT caller
            /// holds a pointer into it.
            std::vector<tearoff> tearoffs;

            int32_t invoke(uint16_t slot, void* args) noexcept override;
        };

        compose_object* object_of(void* self) noexcept
        {
            return static_cast<compose_object*>(
                static_cast<shapes::com_head*>(self)->target);
        }

        int32_t __stdcall object_query_interface(
            void* self, winrt::guid const& id, void** object) noexcept;

        uint32_t __stdcall object_add_ref(void* self) noexcept;

        uint32_t __stdcall object_release(void* self) noexcept;

        // ----- the reference count and the toggle --------------------------
        //
        // The count moves on its own while it stays at two or more, where the
        // pair holds the Python object and one reference more or less changes
        // nothing else. A move between one and two, or down to zero, changes
        // what the pair holds as well, so it is made with the GIL held, which
        // the Python object needs, and under the object's lock, so that no
        // other thread's move lands between the count and the toggle; on a
        // free-threaded build the lock is all that keeps them apart. The GIL
        // is always taken first and nothing under the lock runs Python,
        // because a thread holding the GIL and waiting for the lock would
        // otherwise wait on one holding the lock and waiting for the GIL.

        /**
         * Adds a reference to @p owner if its count is two or more.
         *
         * @returns The count it was raised to, or zero if it is below two and
         * was left alone.
         */
        uint32_t add_while_held(compose_object* owner) noexcept
        {
            auto references = owner->references.load(std::memory_order_relaxed);

            while (references >= 2)
            {
                if (owner->references.compare_exchange_weak(
                        references, references + 1, std::memory_order_relaxed))
                {
                    return references + 1;
                }
            }

            return 0;
        }

        /**
         * Gives up a reference to @p owner if its count is three or more.
         *
         * @returns The count it was lowered to, or zero if it is below three
         * and was left alone.
         */
        uint32_t release_while_held(compose_object* owner) noexcept
        {
            auto references = owner->references.load(std::memory_order_relaxed);

            while (references >= 3)
            {
                if (owner->references.compare_exchange_weak(
                        references,
                        references - 1,
                        std::memory_order_release,
                        std::memory_order_relaxed))
                {
                    return references - 1;
                }
            }

            return 0;
        }

        /**
         * Takes a reference on @p owner unless the Python object is gone or
         * going, which is what tells a weak reference that the object it
         * points at is gone.
         */
        bool take_reference(compose_object* owner) noexcept
        {
            if (add_while_held(owner) != 0)
            {
                return true;
            }

            auto gil = ensure_gil();
            winrt::slim_lock_guard const guard{owner->lock};

            auto const references = owner->references.load(std::memory_order_relaxed);

            if (references == 0)
            {
                return false;
            }

            if (references == 1)
            {
                // A weak reference holds nothing on the Python object, and
                // neither does the pair while the Python wrapper's is its only
                // reference, so Python may be letting go of the object on
                // another thread; it is taken only if its own count has not
                // already reached zero. The wrapper's reference is what keeps
                // the object's memory there to be looked at, and the wrapper
                // gives it up under this lock.
                if (!PyUnstable_TryIncRef(owner->calls.obj))
                {
                    return false;
                }

                PyObject_GC_UnTrack(owner->calls.obj);
            }

            owner->references++;

            return true;
        }

        /**
         * Lets go of everything @p owner holds, once nothing holds @p owner.
         *
         * The memory stays until the last weak reference to it is gone,
         * because a weak reference is a pointer into the object, so what
         * happens here is only the letting go.
         */
        void release_object(compose_object* owner) noexcept
        {
            {
                // WinRT lets go of the object on whatever thread it happens to
                // be on, and letting go of the Python state in it can run a
                // finalizer.
                auto gil = ensure_gil();

                owner->calls.obj = nullptr;
                owner->tearoffs.clear();
                owner->class_name.clear();
                owner->inner = nullptr;
            }

            if (--owner->weak_references == 0)
            {
                delete owner;
            }
        }

        uint32_t __stdcall object_add_ref(void* self) noexcept
        {
            auto* const owner = object_of(self);

            auto references = add_while_held(owner);
            if (references != 0)
            {
                return references;
            }

            // The caller holds the one reference there is, which is the
            // Python wrapper's, so the Python object is alive and is Python's
            // to hand on.
            auto gil = ensure_gil();
            winrt::slim_lock_guard const guard{owner->lock};

            references = ++owner->references;

            if (references == 2)
            {
                // The pair has a holder that is not the Python wrapper, so the
                // WinRT object takes the Python one rather than only pointing
                // at it, and the pair stops being something the garbage
                // collector may take.
                Py_INCREF(owner->calls.obj);
                PyObject_GC_UnTrack(owner->calls.obj);
            }

            return references;
        }

        uint32_t __stdcall object_release(void* self) noexcept
        {
            auto* const owner = object_of(self);

            auto references = release_while_held(owner);
            if (references != 0)
            {
                return references;
            }

            auto gil = ensure_gil();
            PyObject* dropped{};

            {
                winrt::slim_lock_guard const guard{owner->lock};

                references = --owner->references;

                if (references == 1)
                {
                    // The Python wrapper holds the only reference left, so the
                    // WinRT object goes back to pointing at the Python one.
                    dropped = owner->calls.obj;
                    PyObject_GC_Track(dropped);
                }
            }

            if (dropped)
            {
                // That may have been the last reference to the Python object,
                // in which case the pair is gone by the time this returns and
                // nothing here may touch it again.
                Py_DECREF(dropped);

                return 1;
            }

            if (references == 0)
            {
                release_object(owner);
            }

            return references;
        }

        // ----- IInspectable ------------------------------------------------

        int32_t __stdcall object_query_interface(
            void* self, winrt::guid const& id, void** object) noexcept
        {
            auto* const owner = object_of(self);

            if (id == winrt::guid_of<winrt::Windows::Foundation::IUnknown>()
                || id == winrt::guid_of<winrt::Windows::Foundation::IInspectable>()
                || id == winrt::guid_of<winrt::impl::IAgileObject>())
            {
                *object = &owner->head;
                object_add_ref(&owner->head);

                return 0;
            }

            if (id == winrt::guid_of<winrt::impl::IWeakReferenceSource>())
            {
                *object = &owner->source_head;
                object_add_ref(&owner->head);

                return 0;
            }

            if (id == winrt::guid_of<winrt::impl::IMarshal>())
            {
                return winrt::impl::make_marshaler(
                    reinterpret_cast<winrt::impl::unknown_abi*>(&owner->head), object);
            }

            if (id == winrt::guid_of<py::IPywinrtObject>())
            {
                *object = &owner->py_head;
                object_add_ref(&owner->head);

                return 0;
            }

            for (auto& iface : owner->tearoffs)
            {
                if (id == *static_cast<winrt::guid const*>(iface.info->guid))
                {
                    *object = &iface.head;
                    object_add_ref(&owner->head);

                    return 0;
                }
            }

            // Everything the Python class does not answer itself belongs to
            // the object it was composed over. Its own interfaces come back
            // delegating to this one, so a caller that reaches the pair
            // through any of them still holds the pair.
            if (owner->inner)
            {
                return static_cast<winrt::impl::unknown_abi*>(
                           winrt::get_abi(owner->inner))
                    ->QueryInterface(id, object);
            }

            *object = nullptr;

            return winrt::impl::error_no_interface;
        }

        int32_t __stdcall object_get_iids(
            void* self, uint32_t* count, winrt::guid** array) noexcept
        {
            auto* const owner = object_of(self);

            *count = 0;
            *array = nullptr;

            winrt::com_array<winrt::guid> inherited;

            try
            {
                if (owner->inner)
                {
                    inherited = winrt::get_interfaces(owner->inner);
                }
            }
            catch (...)
            {
                return winrt::to_hresult();
            }

            auto const size = owner->tearoffs.size() + inherited.size();

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

            auto* next = iids;

            for (auto const& iface : owner->tearoffs)
            {
                *next++ = *static_cast<winrt::guid const*>(iface.info->guid);
            }

            std::copy(inherited.begin(), inherited.end(), next);

            *count = static_cast<uint32_t>(size);
            *array = iids;

            return 0;
        }

        int32_t __stdcall object_get_runtime_class_name(
            void* self, void** name) noexcept
        {
            try
            {
                *name = winrt::detach_abi(winrt::hstring{object_of(self)->class_name});

                return 0;
            }
            catch (...)
            {
                *name = nullptr;
                return winrt::to_hresult();
            }
        }

        int32_t __stdcall object_get_trust_level(
            void* /*self*/, int32_t* level) noexcept
        {
            *level = 0;

            return 0;
        }

        // ----- py::IPywinrtObject ------------------------------------------

        int32_t __stdcall object_get_py_object(void* self, PyObject*& result) noexcept
        {
            auto gil = ensure_gil();

            result = Py_NewRef(object_of(self)->calls.obj);

            return 0;
        }

        int32_t __stdcall object_get_composable_inner(
            void* self, winrt::Windows::Foundation::IInspectable& inner) noexcept
        {
            inner = object_of(self)->inner;

            return 0;
        }

        // ----- IWeakReferenceSource and IWeakReference ----------------------

        int32_t __stdcall source_get_weak_reference(void* self, void** result) noexcept
        {
            auto* const owner = object_of(self);

            ++owner->weak_references;

            *result = &owner->weak_head;

            return 0;
        }

        uint32_t __stdcall weak_add_ref(void* self) noexcept
        {
            return ++object_of(self)->weak_references;
        }

        uint32_t __stdcall weak_release(void* self) noexcept
        {
            auto* const owner = object_of(self);

            auto const weak_references = --owner->weak_references;

            if (weak_references == 0)
            {
                delete owner;
            }

            return weak_references;
        }

        int32_t __stdcall weak_query_interface(
            void* self, winrt::guid const& id, void** object) noexcept
        {
            auto* const owner = object_of(self);

            if (id == winrt::guid_of<winrt::impl::IWeakReference>()
                || id == winrt::guid_of<winrt::Windows::Foundation::IUnknown>()
                || id == winrt::guid_of<winrt::impl::IAgileObject>())
            {
                *object = &owner->weak_head;
                weak_add_ref(self);

                return 0;
            }

            if (id == winrt::guid_of<winrt::impl::IMarshal>())
            {
                return winrt::impl::make_marshaler(
                    reinterpret_cast<winrt::impl::unknown_abi*>(&owner->weak_head),
                    object);
            }

            *object = nullptr;

            return winrt::impl::error_no_interface;
        }

        int32_t __stdcall weak_resolve(
            void* self, winrt::guid const& id, void** object) noexcept
        {
            auto* const owner = object_of(self);

            if (!take_reference(owner))
            {
                *object = nullptr;

                return 0;
            }

            auto const hr = object_query_interface(&owner->head, id, object);

            object_release(&owner->head);

            return hr;
        }

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

        shapes::vtable_entry const weak_source_vtable[]
            = {reinterpret_cast<shapes::vtable_entry>(&object_query_interface),
               reinterpret_cast<shapes::vtable_entry>(&object_add_ref),
               reinterpret_cast<shapes::vtable_entry>(&object_release),
               reinterpret_cast<shapes::vtable_entry>(&source_get_weak_reference)};

        shapes::vtable_entry const weak_reference_vtable[]
            = {reinterpret_cast<shapes::vtable_entry>(&weak_query_interface),
               reinterpret_cast<shapes::vtable_entry>(&weak_add_ref),
               reinterpret_cast<shapes::vtable_entry>(&weak_release),
               reinterpret_cast<shapes::vtable_entry>(&weak_resolve)};

        int32_t compose_object::invoke(uint16_t /*slot*/, void* /*args*/) noexcept
        {
            // The object's own four vtables are IInspectable's,
            // py::IPywinrtObject's and the two halves of a weak reference, and
            // every entry of all of them is a function of its own above, so no
            // trampoline ever dispatches here.
            return winrt::impl::error_not_implemented;
        }

        // ----- building one --------------------------------------------------

        /**
         * Whether @p overload composes an instance rather than activating one,
         * which is what the implicit outer object says.
         */
        bool composes(overload_desc const& overload) noexcept
        {
            for (uint16_t i = 0; i < overload.arg_count; i++)
            {
                if (overload.args[i].is_implicit)
                {
                    return true;
                }
            }

            return false;
        }

        /**
         * The name GetRuntimeClassName answers with.
         *
         * C++/WinRT names a composed object after the first interface it
         * implements itself, so an object whose class leaves nothing to be
         * overridden has no name of its own and one whose class does is named
         * after the first of those interfaces. A Python class may say what it
         * is called instead, which is what a XAML metadata provider needs.
         */
        winrt::hstring composed_class_name(PyTypeObject* type, type_entry* named)
        {
            pyobj_handle declared{};

            if (PyObject_GetOptionalAttrString(
                    reinterpret_cast<PyObject*>(type),
                    "_runtime_class_name_",
                    declared.put())
                < 0)
            {
                throw python_exception();
            }

            // _winrt.Object has an attribute of that name too - the name a
            // WinRT object answers with - so what settles it is a class that
            // has written a string there.
            if (declared && PyUnicode_Check(declared.get()))
            {
                return convert_to<winrt::hstring>(declared.get());
            }

            if (!named)
            {
                return {};
            }

            return winrt::to_hstring(named->winrt_name);
        }
    } // namespace

    /**
     * The object @p abi was composed over, or @c nullptr when it was not
     * composed at all.
     *
     * @returns A pointer the caller owns a reference to.
     */
    void* composable_inner(void* abi) noexcept
    {
        winrt::com_ptr<py::IPywinrtObject> composed;

        if (static_cast<::IUnknown*>(abi)->QueryInterface(
                winrt::guid_of<py::IPywinrtObject>(), composed.put_void())
            != 0)
        {
            return nullptr;
        }

        winrt::Windows::Foundation::IInspectable inner;

        if (composed->GetComposableInner(inner) != 0)
        {
            return nullptr;
        }

        return winrt::detach_abi(inner);
    }

    /**
     * Makes an instance of @p type, which is a Python class derived from the
     * composable class @p entry: the Python object, and the WinRT object it is
     * composed into, each holding the other.
     *
     * @returns A new reference to the Python object, or @c nullptr with a
     * Python error set.
     */
    PyObject* make_composed_object(
        PyTypeObject* type,
        type_entry& entry,
        member_desc const& constructor,
        overload_desc& overload,
        PyObject* const* args,
        Py_ssize_t nargs) noexcept
    {
        if (!composes(overload))
        {
            PyErr_Format(
                PyExc_TypeError,
                "'%s' cannot be derived from, because '%s' activates an "
                "instance rather than composing one",
                entry.winrt_name,
                overload.winrt_name);
            return nullptr;
        }

        // What the outer object answers itself: every interface the Python
        // class says it implements, and every interface the WinRT class leaves
        // to whatever derives from it.
        std::vector<type_entry*> interfaces;

        if (!collect_interfaces(type, interfaces))
        {
            return nullptr;
        }

        for (auto* const iface : interfaces)
        {
            if (!ensure_interface_vtable(*iface))
            {
                return nullptr;
            }
        }

        auto const overridables = entry.owner->table->type(entry.index).overridables();

        type_entry* named{};

        for (uint32_t i = 0; i < overridables.size(); i++)
        {
            auto* const iface = ensure_named_entry(*entry.owner, overridables[i]);
            if (!iface)
            {
                return nullptr;
            }

            // An overridable interface is exclusive to the class, so the
            // class's own record is where its members are written down.
            if (!ensure_interface_vtable(*iface, entry))
            {
                return nullptr;
            }

            if (!named)
            {
                named = iface;
            }

            if (std::find(interfaces.begin(), interfaces.end(), iface)
                == interfaces.end())
            {
                interfaces.push_back(iface);
            }
        }

        pyobj_handle self{type->tp_alloc(type, 0)};
        if (!self)
        {
            return nullptr;
        }

        // A weak reference to the pair takes the Python object from whatever
        // thread resolves it, which a free-threaded build only allows for an
        // object that has been told to expect it.
        PyUnstable_EnableTryIncRef(self.get());

        auto outer = std::make_unique<compose_object>();

        outer->head.vtable = inspectable_vtable;
        outer->head.target = outer.get();
        outer->py_head.vtable = pywinrt_object_vtable;
        outer->py_head.target = outer.get();
        outer->source_head.vtable = weak_source_vtable;
        outer->source_head.target = outer.get();
        outer->weak_head.vtable = weak_reference_vtable;
        outer->weak_head.target = outer.get();
        outer->calls.obj = self.get();

        try
        {
            outer->class_name = composed_class_name(type, named);
        }
        catch (...)
        {
            to_PyErr();
            return nullptr;
        }

        make_tearoffs(interfaces, outer->head, outer->calls, outer->tearoffs);

        composing compose{&outer->head, nullptr};

        // The factory hands back the composed instance as well, which is the
        // inner object seen through this one and so says nothing the wrapper
        // does not already hold.
        pyobj_handle instance{
            call_member(constructor, overload, nullptr, args, nargs, &compose)};

        winrt::attach_abi(outer->inner, compose.inner);

        if (!instance)
        {
            object_release(&outer.release()->head);
            return nullptr;
        }

        // The wrapper holds the outer object seen as the class's own default
        // interface, so that two Python objects of one WinRT object compare
        // and hash the same however either of them reached it.
        void* abi{};

        auto const hr = object_query_interface(
            &outer->head, *static_cast<winrt::guid const*>(entry.guid), &abi);

        // The object holds the only reference to itself until the wrapper
        // takes one, so letting go of it here is what gives it back.
        object_release(&outer.release()->head);

        if (hr != 0)
        {
            try
            {
                winrt::check_hresult(hr);
            }
            catch (...)
            {
                to_PyErr();
                return nullptr;
            }
        }

        abi_of(self.get()) = abi;

        return self.detach();
    }
} // namespace py::interp
