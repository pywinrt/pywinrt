// Calling a WinRT member that a projection table describes, and the
// descriptors every other file here is written in terms of.
//
// A projection package says what its members are and no longer how to call
// them, so the descriptors below are what members.cpp builds out of a table
// once, at import, and what interp.cpp reads on every call. They hold what the
// call path would otherwise have to look up again each time: where each
// argument sits in the buffer the trampoline reads, which interface the object
// has to be queried for, and which Python type a wrapped value belongs to.
//
// The table itself stays the source of truth. Nothing here is written down
// twice: a descriptor is a resolved view of the records in table.h, and
// anything it could not resolve at import - a type from a package that has not
// been imported yet - is filled in the first time a call needs it.

#pragma once

#include <Python.h>

#include "shapes.h"
#include "table.h"

#include <cstdint>
#include <deque>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace py::interp
{
    struct projection;
    struct type_entry;

    /**
     * One field of a WinRT struct, at the offset the reader computed for it.
     */
    struct field_desc
    {
        table::type_code code;
        /// Byte offset of the field in the struct's blob.
        uint32_t offset;
        /// The type the field's value belongs to, or @c table::no_ref.
        uint32_t type;
        /// Resolved on first use, because the package that defines it may not
        /// have been imported when this type was built.
        type_entry* info;
        /// The struct this is a field of, which is what the getter needs to
        /// find the value in the object.
        type_entry* declaring;
        /// The attribute name, interned, for the keyword arguments of
        /// __init__() and __replace__().
        PyObject* name;
    };

    /**
     * One parameter of one overload.
     */
    struct arg_desc
    {
        table::type_code code;
        table::param_category category;
        bool is_return;
        bool is_implicit;
        /// The ABI passes a pointer to the value rather than the value, which
        /// is what metadata spells @c ref on an input.
        bool by_reference;
        /// Where the value - or, for an output or a by-reference input, the
        /// pointer to it - goes in the buffer the trampoline reads.
        uint16_t offset;
        /// Where an output is received, or a by-reference input is passed
        /// from, in the block of storage the call allocates beside the
        /// arguments.
        uint16_t out_offset;
        /// The ABI hands back an IInspectable that has to be queried for the
        /// interface @c info holds, which is what plain activation does: the
        /// factory returns the object, not its default interface.
        bool query_interface;
        uint32_t type;
        type_entry* info;
    };

    /**
     * One way of calling a member: one overload of a method or a constructor,
     * or the single member behind a property accessor or an event registration.
     */
    struct overload_desc
    {
        /// The vtable entry, counted from the start of the interface.
        uint32_t slot;
        shapes::shape_desc const* shape;
        /// The IID to query the instance for, or @c nullptr when the object the
        /// member is reached through already holds that interface. For a static
        /// or a constructor it is the activation factory interface instead, and
        /// @c factory below is where the answer is kept.
        void const* iface;
        /// The activation factory, once it has been asked for and turned out to
        /// be agile. A factory that is not agile belongs to the apartment that
        /// asked for it, so it is fetched again every time, which is what
        /// C++/WinRT's own factory cache does.
        void* factory;
        char const* winrt_name;
        char const* iface_name;
        uint16_t in_count;
        uint16_t out_count;
        uint16_t arg_count;
        /// Bytes of storage the outputs need, including their alignment.
        uint16_t out_size;
        /// Whether @c out_size and the output offsets are final. An output
        /// whose type is a struct from another package cannot be measured until
        /// that package has been imported, so those are laid out on the first
        /// call instead.
        bool prepared;
        arg_desc* args;
    };

    /**
     * One Python attribute: a method with its overloads, a property with its
     * getter and setter, or one half of an event registration.
     */
    struct member_desc
    {
        projection* owner;
        /// The type whose attribute this is, for an error message.
        type_entry* declaring;
        char const* type_name;
        /// Interned, and the __name__ of the descriptor object.
        PyObject* py_name;
        table::group_kind kind;
        bool is_static;
        bool deprecated;
        uint16_t count;
        overload_desc* overloads;
    };

    /**
     * What the runtime knows about one type in a table beyond what the table
     * says: the Python types built for it and the layout of its values.
     */
    struct type_entry
    {
        projection* owner;
        uint32_t index;
        table::category category;
        /// The wrapper type, which for an interface is the one bound to the
        /// underscored name.
        PyTypeObject* py_type;
        /// The metaclass that carries the statics of a class, or @c nullptr.
        PyTypeObject* statics;
        /// The abstract type a Python implementation of an interface derives
        /// from, bound to the interface's public name, or @c nullptr.
        PyTypeObject* implements;
        /// The IID that an instance of @c py_type holds a pointer to: the
        /// interface itself, or a class's default interface. @c nullptr for a
        /// static class, a struct, an enum or a delegate.
        void const* guid;
        /// The runtime class name, for activation.
        std::wstring class_name;
        char const* winrt_name;
        std::string tp_name;
        /// The constructor group, or @c nullptr for a type that cannot be
        /// instantiated from Python.
        member_desc* constructor;

        // structs only
        uint32_t size;
        uint32_t align;
        /// Where the blob starts in the wrapper object.
        uint32_t blob_offset;
        std::vector<field_desc> fields;
        /// Whether any field holds a string or an interface pointer, so that
        /// copying or releasing this struct is more than a memcpy. A wrapper no
        /// longer holds a C++ value whose constructor and destructor did that,
        /// so a blittable struct is told apart here instead: it gets the dealloc
        /// that only frees the object.
        bool owns_resources;
    };

    /**
     * One namespace's table and everything built from it, for one interpreter.
     *
     * The table is mapped once per process and never unmapped, so the views
     * into it are good for the life of the process. Everything else here is a
     * Python object or points at one, so it belongs to the interpreter that
     * imported the package.
     */
    struct projection
    {
        table::file const* table;
        /// e.g. "winrt.windows.foundation"
        std::string module_name;
        /// Borrowed: the module's dict is what the types are put into, and the
        /// module owns them.
        PyObject* module;
        std::vector<type_entry> types;
        /// The types this namespace defines, by the name they are bound to.
        std::unordered_map<std::string_view, uint32_t> by_py_name;
        /// Names that a descriptor points at and the table does not spell:
        /// qualified WinRT names, mostly. A deque because the pointers into it
        /// have to survive the next one being added.
        std::deque<std::string> strings;
        /// The descriptors the types were built with. They are pointed at by
        /// objects that may outlive the projection's references to the types,
        /// so they are freed with the module state rather than cleared with it.
        std::vector<std::unique_ptr<member_desc[]>> members;
        std::vector<std::unique_ptr<overload_desc[]>> overloads;
        std::vector<std::unique_ptr<arg_desc[]>> args;
        /// The property and field arrays the types were created with, which
        /// CPython requires to outlive them.
        std::vector<std::unique_ptr<PyGetSetDef[]>> getsets;
        /// Interned attribute names, kept alive for the descriptors.
        std::vector<PyObject*> names;

        void release_types() noexcept;

        ~projection();
    };

    // ----- interp.cpp -----------------------------------------------------

    PyObject* call_member(
        member_desc const& member,
        overload_desc& overload,
        void* self,
        PyObject* const* args,
        Py_ssize_t nargs) noexcept;

    overload_desc* select_overload(
        member_desc const& member, Py_ssize_t nargs) noexcept;

    /**
     * Whether @p arg needs storage of its own in the block beside the argument
     * buffer: an output is received into it, and a by-reference input is
     * passed out of it.
     */
    inline bool needs_storage(arg_desc const& arg) noexcept
    {
        return arg.category == table::param_category::out || arg.by_reference;
    }

    PyObject* convert_out(projection& owner, arg_desc& arg, void* storage) noexcept;

    type_entry* resolve(projection& owner, uint32_t type, type_entry*& cache) noexcept;

    // ----- shapes.cpp -----------------------------------------------------

    shapes::shape_desc const* get_forward_shape(uint32_t id) noexcept;

    uint32_t forward_shape_count() noexcept;

    uint32_t reverse_shape_count() noexcept;
} // namespace py::interp
