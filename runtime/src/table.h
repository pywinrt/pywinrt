// Reading a projection table.
//
// A projection package ships one _table.pywinrt per WinRT namespace and no
// compiled code at all, so everything a namespace used to say in C++ - its
// types, their members, the vtable slots those members sit in, and what to
// convert each argument as - is in there. The format is written up in
// table-format.md; this is the reader, and table.cpp is the only place that
// knows the byte offsets.
//
// Nothing here owns the bytes. A view is valid for as long as the buffer the
// file was opened over is, which for a loaded projection is the lifetime of the
// process.

#pragma once

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace py::table
{
    /**
     * The format version this runtime reads. A table says which version it was
     * written to, and the rules are in table-format.md: a different major is
     * refused, and a minor ahead of this one is refused because it may say
     * something that would otherwise be silently ignored.
     *
     * The major is the compatibility generation, which is the same number as
     * the ABI major in pywinrt/abi.h and the epoch in a projection package's
     * version. table.cpp asserts the two against each other. The minors are
     * unrelated and count additions to their own contract.
     */
    inline constexpr uint16_t format_major = 4;
    inline constexpr uint16_t format_minor = 0;

    /**
     * A reference that is not there: no base type, no GUID, no shape.
     */
    inline constexpr uint32_t no_ref = 0xFFFFFFFF;

    /**
     * Thrown when a table cannot be read. The message says what is wrong with
     * it and becomes the text of an ImportError.
     */
    struct format_error : std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };

    /**
     * The kind of value a parameter or a struct field holds.
     */
    enum class type_code : uint32_t
    {
        void_ = 0,
        boolean = 1,
        int8 = 2,
        uint8 = 3,
        int16 = 4,
        uint16 = 5,
        int32 = 6,
        uint32 = 7,
        int64 = 8,
        uint64 = 9,
        single = 10,
        double_ = 11,
        char16 = 12,
        string = 13,
        guid = 14,
        hresult = 15,
        event_token = 16,
        object = 17,
        datetime = 18,
        timespan = 19,
        enum32 = 20,
        enum_u32 = 21,
        struct_ = 22,
        interface_ = 23,
        class_ = 24,
        delegate = 25,
        generic = 26,
        reference = 27,

        count = 28,
    };

    enum class category : uint32_t
    {
        enum_ = 0,
        struct_ = 1,
        interface_ = 2,
        class_ = 3,
        delegate = 4,
    };

    namespace type_flags
    {
        inline constexpr uint32_t category_mask = 0x7;
        inline constexpr uint32_t external = 1 << 3;
        inline constexpr uint32_t static_class = 1 << 4;
        inline constexpr uint32_t composable = 1 << 5;
        inline constexpr uint32_t parameterized = 1 << 6;
        inline constexpr uint32_t concrete = 1 << 7;
        inline constexpr uint32_t default_activatable = 1 << 8;
        inline constexpr uint32_t python_type = 1 << 9;
        inline constexpr uint32_t iterable = 1 << 10;
        inline constexpr uint32_t iterator = 1 << 11;
        inline constexpr uint32_t sequence = 1 << 12;
        inline constexpr uint32_t mutable_sequence = 1 << 13;
        inline constexpr uint32_t mapping = 1 << 14;
        inline constexpr uint32_t mutable_mapping = 1 << 15;
        inline constexpr uint32_t awaitable = 1 << 16;
        inline constexpr uint32_t closeable = 1 << 17;
        inline constexpr uint32_t stringable = 1 << 18;
        inline constexpr uint32_t buffer = 1 << 19;
        inline constexpr uint32_t buffer_length = 1 << 20;
        inline constexpr uint32_t integer = 1 << 21;
        inline constexpr uint32_t flags_enum = 1 << 22;
    } // namespace type_flags

    enum class group_kind : uint32_t
    {
        method = 0,
        property = 1,
        event = 2,
        constructor = 3,
    };

    namespace group_flags
    {
        inline constexpr uint32_t kind_mask = 0x7;
        inline constexpr uint32_t is_static = 1 << 3;
        inline constexpr uint32_t deprecated = 1 << 4;
    } // namespace group_flags

    enum class member_kind : uint32_t
    {
        method = 0,
        property_get = 1,
        property_put = 2,
        event_add = 3,
        event_remove = 4,
        constructor = 5,
    };

    namespace member_flags
    {
        inline constexpr uint32_t kind_mask = 0x7;
        inline constexpr uint32_t is_static = 1 << 3;
        inline constexpr uint32_t overridable = 1 << 4;
        inline constexpr uint32_t protected_ = 1 << 5;
        inline constexpr uint32_t deprecated = 1 << 6;
        inline constexpr uint32_t default_overload = 1 << 7;
        inline constexpr uint32_t role_shift = 8;
        inline constexpr uint32_t role_mask = 0x1F << role_shift;
    } // namespace member_flags

    /**
     * The part a member plays in a Python protocol, which is what the slots of
     * a projected collection call.
     *
     * Which WinRT member stands for @c __len__ or @c __getitem__ is a
     * convention rather than anything the metadata states, and what settles it
     * is the parameterized interface that declares the member: the @c Size of
     * an IVector<T> is a length, the @c Size of something else is a property
     * named size. The generator knows that interface and the runtime does not,
     * so the answer is written down here rather than guessed from the name.
     */
    enum class member_role : uint32_t
    {
        none = 0,
        size = 1,
        get_at = 2,
        set_at = 3,
        remove_at = 4,
        insert_at = 5,
        first = 6,
        current = 7,
        has_current = 8,
        move_next = 9,
        lookup = 10,
        has_key = 11,
        insert = 12,
        remove = 13,
        status = 14,
        completed = 15,
        get_results = 16,
        to_string = 17,
        value = 18,
        close = 19,
        get_many = 20,
        get_view = 21,
        index_of = 22,
        append = 23,
        remove_at_end = 24,
        clear = 25,
        replace_all = 26,
        split = 27,
        pair_key = 28,
        pair_value = 29,
    };

    enum class param_category : uint32_t
    {
        in = 0,
        out = 1,
        pass_array = 2,
        fill_array = 3,
        receive_array = 4,
    };

    namespace param_flags
    {
        inline constexpr uint32_t category_mask = 0x7;
        inline constexpr uint32_t is_return_value = 1 << 3;
        inline constexpr uint32_t is_implicit = 1 << 4;
        inline constexpr uint32_t is_by_reference = 1 << 5;
    } // namespace param_flags

    /**
     * The size and alignment of a value of @p code, where a pointer is as wide
     * as it is on this architecture. A struct is not answered here, because its
     * layout is computed from its fields.
     */
    struct value_layout
    {
        uint32_t size;
        uint32_t align;
    };

    value_layout get_value_layout(type_code code);

    class file;

    /**
     * A list of type references stored in the REFS pool.
     */
    class ref_list
    {
      public:
        ref_list(file const* owner, uint32_t index, uint32_t count) noexcept
            : owner_(owner), index_(index), count_(count)
        {
        }

        uint32_t size() const noexcept
        {
            return count_;
        }

        bool empty() const noexcept
        {
            return count_ == 0;
        }

        uint32_t operator[](uint32_t position) const;

      private:
        file const* owner_;
        uint32_t index_;
        uint32_t count_;
    };

    class param_view
    {
      public:
        param_view(file const* owner, uint32_t index) noexcept
            : owner_(owner), index_(index)
        {
        }

        param_category get_category() const;
        bool is_return_value() const;
        bool is_implicit() const;
        bool is_by_reference() const;
        type_code code() const;
        uint32_t type() const;
        std::string_view name() const;

      private:
        uint32_t word(uint32_t position) const;

        file const* owner_;
        uint32_t index_;
    };

    class member_view
    {
      public:
        member_view(file const* owner, uint32_t index) noexcept
            : owner_(owner), index_(index)
        {
        }

        member_kind kind() const;
        uint32_t flags() const;
        member_role role() const;
        std::string_view winrt_name() const;
        /// The interface to query an instance for, or @c no_ref when the member
        /// is reached without one, which is plain activation.
        uint32_t declaring() const;
        uint32_t slot() const;
        uint32_t forward_shape() const;
        uint32_t reverse_shape() const;
        uint32_t param_count() const;
        param_view param(uint32_t position) const;
        uint32_t in_count() const;
        uint32_t out_count() const;

      private:
        uint32_t word(uint32_t position) const;

        file const* owner_;
        uint32_t index_;
    };

    class group_view
    {
      public:
        group_view(file const* owner, uint32_t index) noexcept
            : owner_(owner), index_(index)
        {
        }

        std::string_view py_name() const;
        group_kind kind() const;
        uint32_t flags() const;
        uint32_t member_count() const;
        member_view member(uint32_t position) const;

      private:
        uint32_t word(uint32_t position) const;

        file const* owner_;
        uint32_t index_;
    };

    class field_view
    {
      public:
        field_view(file const* owner, uint32_t index) noexcept
            : owner_(owner), index_(index)
        {
        }

        std::string_view py_name() const;
        std::string_view winrt_name() const;
        type_code code() const;
        uint32_t type() const;

      private:
        uint32_t word(uint32_t position) const;

        file const* owner_;
        uint32_t index_;
    };

    /**
     * One constant of an enum: the name it is reached by and the thirty-two
     * bits it holds.
     *
     * What those bits mean is the enum's to say and not the constant's. A
     * WinRT enum is signed unless it is a set of flags, in which case it is
     * unsigned, so the @c flags_enum bit of the type record is what decides
     * how the value is read.
     */
    class constant_view
    {
      public:
        constant_view(file const* owner, uint32_t index) noexcept
            : owner_(owner), index_(index)
        {
        }

        std::string_view py_name() const;
        uint32_t value() const;

      private:
        uint32_t word(uint32_t position) const;

        file const* owner_;
        uint32_t index_;
    };

    class type_view
    {
      public:
        type_view(file const* owner, uint32_t index) noexcept
            : owner_(owner), index_(index)
        {
        }

        uint32_t index() const noexcept
        {
            return index_;
        }

        uint32_t flags() const;
        category get_category() const;
        bool is_external() const;
        std::string_view name() const;
        std::string_view winrt_namespace() const;
        /// The attribute this type is bound to in its own module, or, for an
        /// external reference, the fully qualified name the type registry knows
        /// it by.
        std::string_view py_name() const;
        std::string_view signature() const;
        /// The IID, or @c nullptr when the type does not have one, as a runtime
        /// class does not.
        void const* guid() const;
        uint32_t base_type() const;
        uint32_t default_interface() const;
        ref_list interfaces() const;
        ref_list generic_args() const;
        ref_list factories() const;
        ref_list composable_factories() const;
        ref_list overridables() const;
        uint32_t group_count() const;
        group_view group(uint32_t position) const;
        uint32_t field_count() const;
        field_view field(uint32_t position) const;
        uint32_t constant_count() const;
        constant_view constant(uint32_t position) const;

      private:
        uint32_t word(uint32_t position) const;

        file const* owner_;
        uint32_t index_;
    };

    /**
     * The computed layout of a WinRT struct.
     *
     * Offsets are never written into the table, because a pointer is not the
     * same width on every architecture the same table serves, so they are
     * computed here from the field type codes.
     */
    struct struct_layout
    {
        uint32_t size;
        uint32_t align;
        std::vector<uint32_t> offsets;
    };

    /**
     * One namespace's table, over bytes it does not own.
     */
    class file
    {
      public:
        /**
         * Validates the header, the section directory and the string blob.
         *
         * @throws format_error if the bytes are not a table this runtime can
         * read. Everything the accessors reach is bounds checked as it is read,
         * so a table that opens is safe to walk, whatever it says.
         */
        file(void const* data, size_t size);

        uint16_t version_major() const noexcept
        {
            return major_;
        }

        uint16_t version_minor() const noexcept
        {
            return minor_;
        }

        std::string_view generator_version() const;
        std::string_view winrt_namespace() const;

        /// Which census the shape ids in this table were assigned by, which a
        /// runtime compares against the one its own trampolines came from.
        std::string_view census_lineage() const;

        /// How many times ids had been appended to that census when the table
        /// was generated.
        uint32_t census_revision() const noexcept
        {
            return census_revision_;
        }

        /// One past the highest forward shape id the table uses, so that a
        /// runtime whose shapes-generated.h is older than the table can say so at
        /// import instead of indexing out of bounds on the first call.
        uint32_t forward_shape_limit() const noexcept
        {
            return forward_shape_limit_;
        }

        uint32_t reverse_shape_limit() const noexcept
        {
            return reverse_shape_limit_;
        }

        uint32_t type_count() const noexcept
        {
            return type_count_;
        }

        type_view type(uint32_t index) const;

        /// Finds a type this namespace defines by its WinRT name, or returns
        /// @c no_ref. The defined types come first and are sorted by name.
        uint32_t find_type(std::string_view name) const;

        uint32_t group_count() const noexcept
        {
            return group_count_;
        }

        uint32_t member_count() const noexcept
        {
            return member_count_;
        }

        uint32_t param_count() const noexcept
        {
            return param_count_;
        }

        uint32_t field_count() const noexcept
        {
            return field_count_;
        }

        uint32_t constant_count() const noexcept
        {
            return constant_count_;
        }

        uint32_t guid_count() const noexcept
        {
            return guid_count_;
        }

        /// Computes the layout of the struct @p type declares.
        struct_layout get_struct_layout(type_view type) const;

        // The accessors below are what the views are made of. They are public
        // because the views are separate classes, not because anything outside
        // this file should reach for them.

        std::string_view string(uint32_t offset) const;
        void const* guid(uint32_t index) const;
        uint32_t ref(uint32_t index) const;
        uint32_t type_word(uint32_t index, uint32_t position) const;
        uint32_t group_word(uint32_t index, uint32_t position) const;
        uint32_t member_word(uint32_t index, uint32_t position) const;
        uint32_t param_word(uint32_t index, uint32_t position) const;
        uint32_t field_word(uint32_t index, uint32_t position) const;
        uint32_t constant_word(uint32_t index, uint32_t position) const;

      private:
        struct section
        {
            uint32_t offset;
            uint32_t size;
        };

        section find_section(char const (&tag)[5], uint32_t record_size) const;

        uint8_t const* data_;
        uint16_t major_;
        uint16_t minor_;
        uint32_t directory_;
        uint32_t section_count_;
        uint32_t generator_version_;
        uint32_t namespace_;
        uint32_t forward_shape_limit_;
        uint32_t reverse_shape_limit_;
        uint32_t census_lineage_;
        uint32_t census_revision_;
        section strings_;
        section guids_;
        section refs_;
        section types_;
        section groups_;
        section members_;
        section params_;
        section fields_;
        section constants_;
        uint32_t type_count_;
        uint32_t group_count_;
        uint32_t member_count_;
        uint32_t param_count_;
        uint32_t field_count_;
        uint32_t constant_count_;
        uint32_t guid_count_;
        uint32_t ref_count_;
    };
} // namespace py::table
