// A Python object that implements WinRT interfaces.
//
// The public name of every projected interface is bound to an abstract type
// rather than to the wrapper - winrt.windows.foundation.IStringable, not the
// _IStringable a WinRT value of it comes back as - and a Python class that
// derives from one is saying it implements that interface. What is here is
// what such an object is on the WinRT side: one COM object with a tearoff per
// interface, whose vtables are assembled from the same table records the
// forward path calls through.
//
// compose.cpp is the other half of this. A Python subclass of a composable
// WinRT class is one of these with a WinRT object of its own inside it, and
// the tearoffs are the same tearoffs.
//
// What the object does with a call that arrives is the one thing that varies,
// so it is a reverse_call below: a class that derives from an interface's
// public name answers by calling its own method, and a list that stands in for
// an IVector<T> answers by running a sequence operation (pycollections.cpp).
// Everything else - the reference count, the tearoff per interface, what
// QueryInterface answers - is the same object either way.

#pragma once

#include <Python.h>

#include "interp.h"

#include <memory>
#include <span>

namespace py::interp
{
    /**
     * What the calls that arrive at one COM object come down to, and the
     * Python state they run on.
     *
     * The object owns this and lets go of it under the GIL, so anything the
     * Python side of the object holds belongs in the derived class.
     */
    struct reverse_call
    {
        virtual ~reverse_call() = default;

        /**
         * Runs the call that arrived at @p slot with the arguments a reverse
         * trampoline spilled into @p args. The GIL is held.
         */
        virtual int32_t run(reverse_slot const& slot, void* args) noexcept = 0;

        /**
         * The Python object the call runs on, which py::IPywinrtObject hands
         * back, or @c nullptr for an object that stands for no single one.
         */
        virtual PyObject* target() const noexcept = 0;
    };

    bool implements_interfaces(PyTypeObject* type) noexcept;

    void* make_implements_object(PyObject* obj, void const* iid);

    bool ensure_interface_vtable(type_entry& entry);

    void* make_reverse_object(
        std::span<type_entry* const> interfaces,
        void const* iid,
        std::unique_ptr<reverse_call> calls);
} // namespace py::interp
