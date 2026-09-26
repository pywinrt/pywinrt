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

#include <atomic>
#include <cstdint>
#include <deque>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace py::interp::numerics
{
    enum class kind : uint8_t;
} // namespace py::interp::numerics

namespace py::interp
{
    struct projection;
    struct type_entry;

    /**
     * Reads a first-use cache that another thread may be filling in with
     * publish() at the same time: an atomic load with acquire ordering.
     *
     * A cache such as arg_desc::info is written once, after what it points at
     * has been built. A thread that reads the pointer with a plain load could
     * see it before it sees the writes that built the entry behind it, and a
     * plain read of a field another thread is writing is undefined behavior
     * besides. With this load and publish()'s store, a thread that sees the
     * value also sees everything the writer did before storing it. The same
     * holds for a flag such as type_entry::ready, which stands for a whole
     * build rather than one pointer.
     *
     * On x64 both are ordinary moves - the processor already keeps stores in
     * order - so all they change is that the compiler may not move the other
     * writes past them. On ARM64, which does not keep them in order, they are
     * its load-acquire and store-release instructions.
     */
    template<typename T>
    T load_published(T const& value) noexcept
    {
        return std::atomic_ref<T>{const_cast<T&>(value)}.load(
            std::memory_order_acquire);
    }

    /**
     * Stores @p value where load_published() may be reading it: an atomic
     * store with release ordering, made after everything @p value stands for
     * has been written.
     */
    template<typename T>
    void publish(T& target, T value) noexcept
    {
        std::atomic_ref<T>{target}.store(value, std::memory_order_release);
    }

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
        /// pointer to it - goes in the buffer the trampoline reads. An array
        /// is two arguments, and this is the first of them: the count.
        uint16_t offset;
        /// Where the elements of an array go, which is the argument after the
        /// count. Meaningless for anything else.
        uint16_t data_offset;
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
        /// The Python protocol this overload stands for, if any.
        table::member_role role;
        /// A class derived from the one that declares this member answers it
        /// itself, so a call on an object a Python class was composed into has
        /// to be made on the inner object: making it on the object itself
        /// would arrive back in Python, where the call came from.
        bool overridable;
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
     * The members behind the Python protocols a type implements.
     *
     * A WinRT collection is a Python sequence or mapping, an IIterator is a
     * Python iterator, and an async operation is awaitable, but WinRT spells
     * each of those as ordinary members: len() is Size, v[i] is GetAt(i),
     * iter(m) is First(). Which member stands for which operation is the role
     * the table gives it, and it is looked up once, when the type is built, so
     * that the slot that CPython calls has nothing left to find.
     *
     * A member is @c nullptr when the type does not have it, which is what
     * tells a read-only collection from a mutable one.
     */
    struct protocol_members
    {
        // IVector<T> and IVectorView<T>, and IMap<K, V> and IMapView<K, V>,
        // which both count the same way
        member_desc* size;
        member_desc* get_at;
        member_desc* get_many;
        member_desc* set_at;
        member_desc* remove_at;
        member_desc* insert_at;
        // IIterable<T>
        member_desc* first;
        // IIterator<T>
        member_desc* current;
        member_desc* has_current;
        member_desc* move_next;
        // IMap<K, V> and IMapView<K, V>
        member_desc* lookup;
        member_desc* has_key;
        member_desc* insert;
        member_desc* remove;
        // IAsyncInfo and the four async interfaces
        member_desc* status;
        member_desc* completed;
        member_desc* get_results;
        // IStringable
        member_desc* to_string;
        // IReference<T>, whose value is the whole of what it projects as
        member_desc* value;
        // IClosable
        member_desc* close;
    };

    /**
     * What a member reached from WinRT comes down to on the Python side.
     */
    enum class python_op
    {
        /// Call the object itself, which is what a delegate does.
        invoke,
        /// Call the method the member is named after.
        call_method,
        /// Read the attribute it is named after, which is the one output of a
        /// property getter.
        get_attribute,
        /// Write it, which is the one input of a property setter.
        set_attribute,
    };

    /**
     * What one entry of a vtable that WinRT calls a Python object through
     * stands for.
     */
    struct reverse_slot
    {
        member_desc* member;
        overload_desc* overload;
        python_op op;
    };

    /**
     * The WinRT side of a Python object that stands in for a value of one
     * type: the vtable a caller enters through, and what each of its entries
     * means.
     *
     * Nothing in it depends on which Python object is behind it, so it is
     * built once per type and shared by every object of that type. The two
     * arrays are never resized once built, because a WinRT caller holds a
     * pointer into the first of them.
     */
    struct reverse_vtable
    {
        std::vector<shapes::vtable_entry> entries;
        /// By vtable slot, so that a trampoline's slot indexes it directly.
        /// A slot with no member - one this runtime cannot answer - holds
        /// nulls.
        std::vector<reverse_slot> slots;
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
        /// The type still takes type arguments, so its members have no ABI
        /// until an instance fills them in. That is one of the two reasons a
        /// member has no trampoline - the other is a projection generated
        /// against a census without its call shape - and they want different
        /// errors, so the flag is kept rather than the table consulted.
        bool parameterized;
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
        /// Every attribute of this type, contiguous in one block, which is
        /// what the protocol slots are found in.
        member_desc* members;
        uint16_t member_count;
        /// What the table's protocol flags come to: the members that the
        /// Python slots of this type call.
        protocol_members protocol;
        /// What a Python object passed where a value of this type is expected
        /// is given to WinRT as, built the first time one is. A delegate has
        /// nothing else: no Python type stands for one, because the module
        /// binds a typing alias to its name.
        std::unique_ptr<reverse_vtable> reverse;

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
        /// Which of the Windows.Foundation.Numerics structs this is, for the
        /// arithmetic on them that no metadata describes. numerics.h says what
        /// that is and where it comes from instead.
        numerics::kind numerics_kind;
        /// Set with publish() once the entry has been built, and read with
        /// load_published() by anything that may be on another thread than
        /// the one that built it.
        bool ready;
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
        /// The method arrays of the types that have one, which CPython
        /// requires to outlive them in the same way.
        std::vector<std::unique_ptr<PyMethodDef[]>> method_defs;
        /// Interned attribute names, kept alive for the descriptors.
        std::vector<PyObject*> names;
        /// The thread that is executing the package's __init__.py, which
        /// builds every type the namespace binds.
        unsigned long loader;
        /// Set with publish() once it has.
        bool loaded;

        void release_types() noexcept;

        ~projection();
    };

    // ----- interp.cpp -----------------------------------------------------

    /**
     * The two objects a composable class's constructor passes between when a
     * Python subclass of it is being made.
     *
     * The constructor declares them as implicit parameters, which every other
     * caller leaves empty: an exact type is activated with nothing to compose
     * it into, and takes nothing back.
     */
    struct composing
    {
        /// The object WinRT is to call the class's overridable members on,
        /// which is the outer object compose.cpp assembled.
        void* outer;
        /// The object the class was composed over, which the constructor hands
        /// back and the caller owns a reference to.
        void* inner;
    };

    PyObject* call_member(
        member_desc const& member,
        overload_desc& overload,
        void* self,
        PyObject* const* args,
        Py_ssize_t nargs,
        composing* compose = nullptr) noexcept;

    PyObject* call_iterator_step(type_entry& info, void* self, bool& fused) noexcept;

    PyObject* call_indexed(
        member_desc* member, void* self, uint32_t index, bool& direct) noexcept;

    overload_desc* select_overload(
        member_desc const& member, Py_ssize_t nargs) noexcept;

    /**
     * Whether @p arg is one of the values a call hands back: an output, or an
     * array the callee allocates.
     */
    inline bool is_output(arg_desc const& arg) noexcept
    {
        if (arg.category == table::param_category::out)
        {
            return true;
        }

        return arg.category == table::param_category::receive_array;
    }

    /**
     * Whether @p arg needs storage of its own in the block beside the argument
     * buffer: an output is received into it, a by-reference input is passed
     * out of it, and an array output receives its count and its elements
     * through an array_out that lives there.
     */
    inline bool needs_storage(arg_desc const& arg) noexcept
    {
        if (arg.category == table::param_category::out)
        {
            return true;
        }

        if (arg.category == table::param_category::receive_array)
        {
            return true;
        }

        return arg.by_reference;
    }

    PyObject* convert_out(projection& owner, arg_desc& arg, void* storage) noexcept;

    void release_value(arg_desc const& arg, void* storage) noexcept;

    type_entry* resolve(projection& owner, uint32_t type, type_entry*& cache) noexcept;

    // ----- shapes.cpp -----------------------------------------------------

    shapes::shape_desc const* get_forward_shape(uint32_t id) noexcept;

    shapes::reverse_desc const* get_reverse_shape(uint32_t id) noexcept;

    uint32_t forward_shape_count() noexcept;

    uint32_t reverse_shape_count() noexcept;

    std::string_view census_lineage() noexcept;

    uint32_t census_revision() noexcept;
} // namespace py::interp
