// The reader for the projection tables. See table-format.md for the format and
// table.h for what a caller sees.
//
// This is the only file that knows where a field is. Every offset it follows is
// checked against the size of the buffer, and every index against the size of
// its section, so a table that opens can be walked whatever it says. None of
// those checks is expected to fire - the tables are generated - but a package
// and a runtime that do not belong together should say so rather than read past
// the end of a mapping.

#define PYWINRT_RUNTIME_MODULE
#include <pywinrt/base.h>

#include "table.h"

#include <algorithm>
#include <cstring>

namespace py::table
{
    // Breaking either contract forces every projection package to be
    // republished, so both breaks are the same event and carry the same
    // number: the epoch in a package's version.
    static_assert(
        format_major == runtime_abi_version_major,
        "the table format major and the ABI major are one compatibility generation");

    namespace
    {
        constexpr uint32_t header_size = 48;
        constexpr uint32_t section_entry_size = 12;
        constexpr uint32_t type_record_size = 22 * 4;
        constexpr uint32_t group_record_size = 4 * 4;
        constexpr uint32_t member_record_size = 10 * 4;
        constexpr uint32_t param_record_size = 4 * 4;
        constexpr uint32_t field_record_size = 4 * 4;

        constexpr uint32_t make_tag(char const (&tag)[5])
        {
            return static_cast<uint8_t>(tag[0]) | (static_cast<uint8_t>(tag[1]) << 8)
                   | (static_cast<uint8_t>(tag[2]) << 16)
                   | (static_cast<uint8_t>(tag[3]) << 24);
        }

        uint32_t read_u32(uint8_t const* data)
        {
            uint32_t value;
            std::memcpy(&value, data, sizeof(value));
            return value;
        }

        uint16_t read_u16(uint8_t const* data)
        {
            uint16_t value;
            std::memcpy(&value, data, sizeof(value));
            return value;
        }

        [[noreturn]] void fail(std::string message)
        {
            throw format_error(std::move(message));
        }
    } // namespace

    value_layout get_value_layout(type_code code)
    {
        constexpr uint32_t ptr = static_cast<uint32_t>(sizeof(void*));

        switch (code)
        {
        case type_code::boolean:
        case type_code::int8:
        case type_code::uint8:
            return {1, 1};
        case type_code::int16:
        case type_code::uint16:
        case type_code::char16:
            return {2, 2};
        case type_code::int32:
        case type_code::uint32:
        case type_code::single:
        case type_code::enum32:
        case type_code::enum_u32:
        case type_code::hresult:
            return {4, 4};
        case type_code::int64:
        case type_code::uint64:
        case type_code::double_:
        case type_code::datetime:
        case type_code::timespan:
        case type_code::event_token:
            return {8, 8};
        case type_code::guid:
            // sixteen bytes of four byte fields, so four byte aligned
            return {16, 4};
        case type_code::string:
        case type_code::object:
        case type_code::interface_:
        case type_code::class_:
        case type_code::delegate:
        case type_code::generic:
        case type_code::reference:
            return {ptr, ptr};
        default:
            fail(
                "no layout for type code "
                + std::to_string(static_cast<uint32_t>(code)));
        }
    }

    // ----- file -----------------------------------------------------------

    file::file(void const* data, size_t size) : data_(static_cast<uint8_t const*>(data))
    {
        if (size < header_size)
        {
            fail("not a projection table: too short");
        }

        if (std::memcmp(data_, "PYWINRT\x1a", 8) != 0)
        {
            fail("not a projection table: wrong magic");
        }

        major_ = read_u16(data_ + 8);
        minor_ = read_u16(data_ + 10);

        if (major_ != format_major)
        {
            fail(
                "table format version " + std::to_string(major_) + "."
                + std::to_string(minor_)
                + " cannot be read by this runtime, which reads "
                + std::to_string(format_major) + "." + std::to_string(format_minor));
        }

        if (minor_ > format_minor)
        {
            fail(
                "table format version " + std::to_string(major_) + "."
                + std::to_string(minor_) + " is newer than this runtime, which reads "
                + std::to_string(format_major) + "." + std::to_string(format_minor));
        }

        auto const file_size = read_u32(data_ + 12);

        if (file_size != size)
        {
            fail(
                "table says it is " + std::to_string(file_size) + " bytes but is "
                + std::to_string(size));
        }

        generator_version_ = read_u32(data_ + 24);
        namespace_ = read_u32(data_ + 28);
        forward_shape_limit_ = read_u32(data_ + 32);
        reverse_shape_limit_ = read_u32(data_ + 36);

        auto const section_count = read_u32(data_ + 16);
        auto const directory = read_u32(data_ + 20);

        if (directory % 8 != 0 || directory > size
            || static_cast<uint64_t>(section_count) * section_entry_size
                   > size - directory)
        {
            fail("table section directory is out of bounds");
        }

        directory_ = directory;
        section_count_ = section_count;

        for (uint32_t i = 0; i < section_count; i++)
        {
            auto const entry = data_ + directory + i * section_entry_size;
            auto const offset = read_u32(entry + 4);
            auto const length = read_u32(entry + 8);

            if (offset % 8 != 0 || offset > size || length > size - offset)
            {
                fail("table section " + std::to_string(i) + " is out of bounds");
            }
        }

        strings_ = find_section("STRS", 1);
        guids_ = find_section("GUID", 16);
        refs_ = find_section("REFS", 4);
        types_ = find_section("TYPE", type_record_size);
        groups_ = find_section("GRUP", group_record_size);
        members_ = find_section("MEMB", member_record_size);
        params_ = find_section("PARM", param_record_size);
        fields_ = find_section("FLDS", field_record_size);

        if (strings_.size < 1 || data_[strings_.offset] != 0
            || data_[strings_.offset + strings_.size - 1] != 0)
        {
            fail("table string blob is not NUL delimited");
        }

        type_count_ = types_.size / type_record_size;
        group_count_ = groups_.size / group_record_size;
        member_count_ = members_.size / member_record_size;
        param_count_ = params_.size / param_record_size;
        field_count_ = fields_.size / field_record_size;
        guid_count_ = guids_.size / 16;
        ref_count_ = refs_.size / 4;
    }

    file::section file::find_section(char const (&tag)[5], uint32_t record_size) const
    {
        auto const wanted = make_tag(tag);

        for (uint32_t i = 0; i < section_count_; i++)
        {
            auto const entry = data_ + directory_ + i * section_entry_size;

            if (read_u32(entry) != wanted)
            {
                continue;
            }

            section result{read_u32(entry + 4), read_u32(entry + 8)};

            if (result.size % record_size != 0)
            {
                fail(
                    std::string("table section ") + tag + " is "
                    + std::to_string(result.size)
                    + " bytes, which is not a whole number of "
                    + std::to_string(record_size) + " byte records");
            }

            return result;
        }

        fail(std::string("table has no ") + tag + " section");
    }

    std::string_view file::string(uint32_t offset) const
    {
        if (offset >= strings_.size)
        {
            fail("table string offset " + std::to_string(offset) + " is out of bounds");
        }

        auto const begin
            = reinterpret_cast<char const*>(data_ + strings_.offset + offset);

        // the blob ends with a NUL, so the string is terminated inside it
        return std::string_view{begin};
    }

    std::string_view file::generator_version() const
    {
        return string(generator_version_);
    }

    std::string_view file::winrt_namespace() const
    {
        return string(namespace_);
    }

    void const* file::guid(uint32_t index) const
    {
        if (index >= guid_count_)
        {
            fail("table GUID index " + std::to_string(index) + " is out of bounds");
        }

        return data_ + guids_.offset + index * 16;
    }

    uint32_t file::ref(uint32_t index) const
    {
        if (index >= ref_count_)
        {
            fail(
                "table reference index " + std::to_string(index) + " is out of bounds");
        }

        return read_u32(data_ + refs_.offset + index * 4);
    }

    uint32_t file::type_word(uint32_t index, uint32_t position) const
    {
        if (index >= type_count_)
        {
            fail("table type index " + std::to_string(index) + " is out of bounds");
        }

        return read_u32(
            data_ + types_.offset + index * type_record_size + position * 4);
    }

    uint32_t file::group_word(uint32_t index, uint32_t position) const
    {
        if (index >= group_count_)
        {
            fail("table group index " + std::to_string(index) + " is out of bounds");
        }

        return read_u32(
            data_ + groups_.offset + index * group_record_size + position * 4);
    }

    uint32_t file::member_word(uint32_t index, uint32_t position) const
    {
        if (index >= member_count_)
        {
            fail("table member index " + std::to_string(index) + " is out of bounds");
        }

        return read_u32(
            data_ + members_.offset + index * member_record_size + position * 4);
    }

    uint32_t file::param_word(uint32_t index, uint32_t position) const
    {
        if (index >= param_count_)
        {
            fail(
                "table parameter index " + std::to_string(index) + " is out of bounds");
        }

        return read_u32(
            data_ + params_.offset + index * param_record_size + position * 4);
    }

    uint32_t file::field_word(uint32_t index, uint32_t position) const
    {
        if (index >= field_count_)
        {
            fail("table field index " + std::to_string(index) + " is out of bounds");
        }

        return read_u32(
            data_ + fields_.offset + index * field_record_size + position * 4);
    }

    type_view file::type(uint32_t index) const
    {
        if (index >= type_count_)
        {
            fail("table type index " + std::to_string(index) + " is out of bounds");
        }

        return type_view{this, index};
    }

    uint32_t file::find_type(std::string_view name) const
    {
        // The types this namespace defines come first, sorted by name, so the
        // search stops at the first external record.
        uint32_t low = 0;
        uint32_t high = type_count_;

        while (low < high)
        {
            auto const middle = low + (high - low) / 2;
            auto const candidate = type(middle);

            if (candidate.is_external())
            {
                high = middle;
                continue;
            }

            auto const comparison = candidate.name().compare(name);

            if (comparison == 0)
            {
                return middle;
            }

            if (comparison < 0)
            {
                low = middle + 1;
            }
            else
            {
                high = middle;
            }
        }

        return no_ref;
    }

    struct_layout file::get_struct_layout(type_view type) const
    {
        struct_layout result{0, 1, {}};
        uint32_t offset = 0;
        auto const count = type.field_count();

        result.offsets.reserve(count);

        for (uint32_t i = 0; i < count; i++)
        {
            auto const field = type.field(i);
            value_layout layout;

            if (field.code() == type_code::struct_)
            {
                auto const nested = get_struct_layout(this->type(field.type()));
                layout = {nested.size, nested.align};
            }
            else
            {
                layout = get_value_layout(field.code());
            }

            offset = (offset + layout.align - 1) / layout.align * layout.align;
            result.offsets.push_back(offset);
            offset += layout.size;
            result.align = std::max(result.align, layout.align);
        }

        result.size = (offset + result.align - 1) / result.align * result.align;

        return result;
    }

    // ----- views ----------------------------------------------------------

    uint32_t ref_list::operator[](uint32_t position) const
    {
        if (position >= count_)
        {
            fail(
                "table list position " + std::to_string(position)
                + " is out of bounds");
        }

        return owner_->ref(index_ + position);
    }

    uint32_t type_view::word(uint32_t position) const
    {
        return owner_->type_word(index_, position);
    }

    uint32_t type_view::flags() const
    {
        return word(0);
    }

    category type_view::get_category() const
    {
        return static_cast<category>(flags() & type_flags::category_mask);
    }

    bool type_view::is_external() const
    {
        return (flags() & type_flags::external) != 0;
    }

    std::string_view type_view::name() const
    {
        return owner_->string(word(1));
    }

    std::string_view type_view::winrt_namespace() const
    {
        return owner_->string(word(2));
    }

    std::string_view type_view::py_name() const
    {
        return owner_->string(word(3));
    }

    std::string_view type_view::signature() const
    {
        return owner_->string(word(4));
    }

    void const* type_view::guid() const
    {
        auto const index = word(5);

        return index == no_ref ? nullptr : owner_->guid(index);
    }

    uint32_t type_view::base_type() const
    {
        return word(6);
    }

    uint32_t type_view::default_interface() const
    {
        return word(7);
    }

    ref_list type_view::interfaces() const
    {
        return {owner_, word(8), word(9)};
    }

    ref_list type_view::generic_args() const
    {
        return {owner_, word(10), word(11)};
    }

    uint32_t type_view::group_count() const
    {
        return word(13);
    }

    group_view type_view::group(uint32_t position) const
    {
        if (position >= group_count())
        {
            fail(
                "table group position " + std::to_string(position)
                + " is out of bounds");
        }

        return group_view{owner_, word(12) + position};
    }

    uint32_t type_view::field_count() const
    {
        return word(15);
    }

    field_view type_view::field(uint32_t position) const
    {
        if (position >= field_count())
        {
            fail(
                "table field position " + std::to_string(position)
                + " is out of bounds");
        }

        return field_view{owner_, word(14) + position};
    }

    ref_list type_view::factories() const
    {
        return {owner_, word(16), word(17)};
    }

    ref_list type_view::composable_factories() const
    {
        return {owner_, word(18), word(19)};
    }

    ref_list type_view::overridables() const
    {
        return {owner_, word(20), word(21)};
    }

    uint32_t group_view::word(uint32_t position) const
    {
        return owner_->group_word(index_, position);
    }

    std::string_view group_view::py_name() const
    {
        return owner_->string(word(0));
    }

    uint32_t group_view::flags() const
    {
        return word(1);
    }

    group_kind group_view::kind() const
    {
        return static_cast<group_kind>(flags() & group_flags::kind_mask);
    }

    uint32_t group_view::member_count() const
    {
        return word(3);
    }

    member_view group_view::member(uint32_t position) const
    {
        if (position >= member_count())
        {
            fail(
                "table member position " + std::to_string(position)
                + " is out of bounds");
        }

        return member_view{owner_, word(2) + position};
    }

    uint32_t member_view::word(uint32_t position) const
    {
        return owner_->member_word(index_, position);
    }

    uint32_t member_view::flags() const
    {
        return word(0);
    }

    member_kind member_view::kind() const
    {
        return static_cast<member_kind>(flags() & member_flags::kind_mask);
    }

    member_role member_view::role() const
    {
        return static_cast<member_role>(
            (flags() & member_flags::role_mask) >> member_flags::role_shift);
    }

    std::string_view member_view::winrt_name() const
    {
        return owner_->string(word(1));
    }

    uint32_t member_view::declaring() const
    {
        return word(2);
    }

    uint32_t member_view::slot() const
    {
        return word(3);
    }

    uint32_t member_view::forward_shape() const
    {
        return word(4);
    }

    uint32_t member_view::param_count() const
    {
        return word(6);
    }

    param_view member_view::param(uint32_t position) const
    {
        if (position >= param_count())
        {
            fail(
                "table parameter position " + std::to_string(position)
                + " is out of bounds");
        }

        return param_view{owner_, word(5) + position};
    }

    uint32_t member_view::in_count() const
    {
        return word(7);
    }

    uint32_t member_view::out_count() const
    {
        return word(8);
    }

    uint32_t member_view::reverse_shape() const
    {
        return word(9);
    }

    uint32_t param_view::word(uint32_t position) const
    {
        return owner_->param_word(index_, position);
    }

    param_category param_view::get_category() const
    {
        return static_cast<param_category>(word(0) & param_flags::category_mask);
    }

    bool param_view::is_return_value() const
    {
        return (word(0) & param_flags::is_return_value) != 0;
    }

    bool param_view::is_implicit() const
    {
        return (word(0) & param_flags::is_implicit) != 0;
    }

    bool param_view::is_by_reference() const
    {
        return (word(0) & param_flags::is_by_reference) != 0;
    }

    type_code param_view::code() const
    {
        return static_cast<type_code>(word(1));
    }

    uint32_t param_view::type() const
    {
        return word(2);
    }

    std::string_view param_view::name() const
    {
        return owner_->string(word(3));
    }

    uint32_t field_view::word(uint32_t position) const
    {
        return owner_->field_word(index_, position);
    }

    std::string_view field_view::py_name() const
    {
        return owner_->string(word(0));
    }

    std::string_view field_view::winrt_name() const
    {
        return owner_->string(word(1));
    }

    type_code field_view::code() const
    {
        return static_cast<type_code>(word(2));
    }

    uint32_t field_view::type() const
    {
        return word(3);
    }
} // namespace py::table

namespace py::cpp::_winrt
{
    namespace
    {
        using namespace py::table;

        /**
         * Sets @p key in @p dict to @p value, stealing the reference to
         * @p value, and returns false with a Python error set if either the
         * value or the insertion failed.
         */
        bool set_item(PyObject* dict, const char* key, PyObject* value) noexcept
        {
            pyobj_handle owned{value};

            return owned && PyDict_SetItemString(dict, key, owned.get()) == 0;
        }

        pyobj_handle make_ref_list(ref_list const& list)
        {
            pyobj_handle result{PyList_New(list.size())};
            if (!result)
            {
                return result;
            }

            for (uint32_t i = 0; i < list.size(); i++)
            {
                pyobj_handle item{PyLong_FromUnsignedLong(list[i])};
                if (!item)
                {
                    return pyobj_handle{};
                }

                if (PyList_SetItem(result.get(), i, item.detach()) < 0)
                {
                    return pyobj_handle{};
                }
            }

            return result;
        }

        pyobj_handle make_param(param_view const& param)
        {
            pyobj_handle result{PyDict_New()};
            if (!result)
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "category",
                    PyLong_FromUnsignedLong(
                        static_cast<uint32_t>(param.get_category()))))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "code",
                    PyLong_FromUnsignedLong(static_cast<uint32_t>(param.code()))))
            {
                return pyobj_handle{};
            }

            if (!set_item(result.get(), "type", PyLong_FromUnsignedLong(param.type())))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "name",
                    PyUnicode_FromStringAndSize(
                        param.name().data(),
                        static_cast<Py_ssize_t>(param.name().size()))))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "is_return_value",
                    PyBool_FromLong(param.is_return_value())))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(), "is_implicit", PyBool_FromLong(param.is_implicit())))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "is_by_reference",
                    PyBool_FromLong(param.is_by_reference())))
            {
                return pyobj_handle{};
            }

            return result;
        }

        pyobj_handle make_member(member_view const& member)
        {
            pyobj_handle params{PyList_New(member.param_count())};
            if (!params)
            {
                return pyobj_handle{};
            }

            for (uint32_t i = 0; i < member.param_count(); i++)
            {
                auto param = make_param(member.param(i));
                if (!param)
                {
                    return pyobj_handle{};
                }

                if (PyList_SetItem(params.get(), i, param.detach()) < 0)
                {
                    return pyobj_handle{};
                }
            }

            pyobj_handle result{PyDict_New()};
            if (!result)
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "winrt_name",
                    PyUnicode_FromStringAndSize(
                        member.winrt_name().data(),
                        static_cast<Py_ssize_t>(member.winrt_name().size()))))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "kind",
                    PyLong_FromUnsignedLong(static_cast<uint32_t>(member.kind()))))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(), "flags", PyLong_FromUnsignedLong(member.flags())))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "role",
                    PyLong_FromUnsignedLong(static_cast<uint32_t>(member.role()))))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "declaring",
                    PyLong_FromUnsignedLong(member.declaring())))
            {
                return pyobj_handle{};
            }

            if (!set_item(result.get(), "slot", PyLong_FromUnsignedLong(member.slot())))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "forward_shape",
                    PyLong_FromUnsignedLong(member.forward_shape())))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "reverse_shape",
                    PyLong_FromUnsignedLong(member.reverse_shape())))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "in_count",
                    PyLong_FromUnsignedLong(member.in_count())))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "out_count",
                    PyLong_FromUnsignedLong(member.out_count())))
            {
                return pyobj_handle{};
            }

            if (!set_item(result.get(), "params", params.detach()))
            {
                return pyobj_handle{};
            }

            return result;
        }

        pyobj_handle make_group(group_view const& group)
        {
            pyobj_handle members{PyList_New(group.member_count())};
            if (!members)
            {
                return pyobj_handle{};
            }

            for (uint32_t i = 0; i < group.member_count(); i++)
            {
                auto member = make_member(group.member(i));
                if (!member)
                {
                    return pyobj_handle{};
                }

                if (PyList_SetItem(members.get(), i, member.detach()) < 0)
                {
                    return pyobj_handle{};
                }
            }

            pyobj_handle result{PyDict_New()};
            if (!result)
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "py_name",
                    PyUnicode_FromStringAndSize(
                        group.py_name().data(),
                        static_cast<Py_ssize_t>(group.py_name().size()))))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "kind",
                    PyLong_FromUnsignedLong(static_cast<uint32_t>(group.kind()))))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(), "flags", PyLong_FromUnsignedLong(group.flags())))
            {
                return pyobj_handle{};
            }

            if (!set_item(result.get(), "members", members.detach()))
            {
                return pyobj_handle{};
            }

            return result;
        }

        pyobj_handle make_type(file const& table, type_view const& type)
        {
            pyobj_handle groups{PyList_New(type.group_count())};
            if (!groups)
            {
                return pyobj_handle{};
            }

            for (uint32_t i = 0; i < type.group_count(); i++)
            {
                auto group = make_group(type.group(i));
                if (!group)
                {
                    return pyobj_handle{};
                }

                if (PyList_SetItem(groups.get(), i, group.detach()) < 0)
                {
                    return pyobj_handle{};
                }
            }

            auto const layout = table.get_struct_layout(type);
            pyobj_handle fields{PyList_New(type.field_count())};
            if (!fields)
            {
                return pyobj_handle{};
            }

            for (uint32_t i = 0; i < type.field_count(); i++)
            {
                auto const field = type.field(i);
                pyobj_handle item{PyDict_New()};
                if (!item)
                {
                    return pyobj_handle{};
                }

                if (!set_item(
                        item.get(),
                        "py_name",
                        PyUnicode_FromStringAndSize(
                            field.py_name().data(),
                            static_cast<Py_ssize_t>(field.py_name().size()))))
                {
                    return pyobj_handle{};
                }

                if (!set_item(
                        item.get(),
                        "winrt_name",
                        PyUnicode_FromStringAndSize(
                            field.winrt_name().data(),
                            static_cast<Py_ssize_t>(field.winrt_name().size()))))
                {
                    return pyobj_handle{};
                }

                if (!set_item(
                        item.get(),
                        "code",
                        PyLong_FromUnsignedLong(static_cast<uint32_t>(field.code()))))
                {
                    return pyobj_handle{};
                }

                if (!set_item(
                        item.get(), "type", PyLong_FromUnsignedLong(field.type())))
                {
                    return pyobj_handle{};
                }

                if (!set_item(
                        item.get(),
                        "offset",
                        PyLong_FromUnsignedLong(layout.offsets[i])))
                {
                    return pyobj_handle{};
                }

                if (PyList_SetItem(fields.get(), i, item.detach()) < 0)
                {
                    return pyobj_handle{};
                }
            }

            auto const guid = type.guid();
            pyobj_handle result{PyDict_New()};
            if (!result)
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "name",
                    PyUnicode_FromStringAndSize(
                        type.name().data(),
                        static_cast<Py_ssize_t>(type.name().size()))))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "namespace",
                    PyUnicode_FromStringAndSize(
                        type.winrt_namespace().data(),
                        static_cast<Py_ssize_t>(type.winrt_namespace().size()))))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "py_name",
                    PyUnicode_FromStringAndSize(
                        type.py_name().data(),
                        static_cast<Py_ssize_t>(type.py_name().size()))))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "signature",
                    PyUnicode_FromStringAndSize(
                        type.signature().data(),
                        static_cast<Py_ssize_t>(type.signature().size()))))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "category",
                    PyLong_FromUnsignedLong(
                        static_cast<uint32_t>(type.get_category()))))
            {
                return pyobj_handle{};
            }

            if (!set_item(result.get(), "flags", PyLong_FromUnsignedLong(type.flags())))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "guid",
                    guid ? PyBytes_FromStringAndSize(static_cast<char const*>(guid), 16)
                         : Py_NewRef(Py_None)))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(), "base", PyLong_FromUnsignedLong(type.base_type())))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "default_interface",
                    PyLong_FromUnsignedLong(type.default_interface())))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "interfaces",
                    make_ref_list(type.interfaces()).detach()))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "generic_args",
                    make_ref_list(type.generic_args()).detach()))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "factories",
                    make_ref_list(type.factories()).detach()))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "composable_factories",
                    make_ref_list(type.composable_factories()).detach()))
            {
                return pyobj_handle{};
            }

            if (!set_item(
                    result.get(),
                    "overridables",
                    make_ref_list(type.overridables()).detach()))
            {
                return pyobj_handle{};
            }

            if (!set_item(result.get(), "groups", groups.detach()))
            {
                return pyobj_handle{};
            }

            if (!set_item(result.get(), "fields", fields.detach()))
            {
                return pyobj_handle{};
            }

            if (!set_item(result.get(), "size", PyLong_FromUnsignedLong(layout.size)))
            {
                return pyobj_handle{};
            }

            if (!set_item(result.get(), "align", PyLong_FromUnsignedLong(layout.align)))
            {
                return pyobj_handle{};
            }

            return result;
        }
    } // namespace

    /**
     * Reads a projection table and returns everything in it as plain Python
     * objects.
     *
     * This is what test/test_table.py checks the generator's writer against: the
     * two ends of the format are in different languages and different processes,
     * so the only way to know they agree is to write a table with one and read
     * it back with the other.
     */
    PyObject* read_table(PyObject* /*unused*/, PyObject* obj) noexcept
    {
        py::buffer_view view{obj, PyBUF_SIMPLE};
        if (!view)
        {
            return nullptr;
        }

        try
        {
            py::table::file table{view.data(), view.size()};
            pyobj_handle types{PyList_New(table.type_count())};
            if (!types)
            {
                return nullptr;
            }

            for (uint32_t i = 0; i < table.type_count(); i++)
            {
                auto type = make_type(table, table.type(i));
                if (!type)
                {
                    return nullptr;
                }

                if (PyList_SetItem(types.get(), i, type.detach()) < 0)
                {
                    return nullptr;
                }
            }

            pyobj_handle result{PyDict_New()};
            if (!result)
            {
                return nullptr;
            }

            if (!set_item(
                    result.get(),
                    "format",
                    Py_BuildValue(
                        "(II)", table.version_major(), table.version_minor())))
            {
                return nullptr;
            }

            if (!set_item(
                    result.get(),
                    "generator",
                    PyUnicode_FromStringAndSize(
                        table.generator_version().data(),
                        static_cast<Py_ssize_t>(table.generator_version().size()))))
            {
                return nullptr;
            }

            if (!set_item(
                    result.get(),
                    "namespace",
                    PyUnicode_FromStringAndSize(
                        table.winrt_namespace().data(),
                        static_cast<Py_ssize_t>(table.winrt_namespace().size()))))
            {
                return nullptr;
            }

            if (!set_item(
                    result.get(),
                    "forward_shape_limit",
                    PyLong_FromUnsignedLong(table.forward_shape_limit())))
            {
                return nullptr;
            }

            if (!set_item(
                    result.get(),
                    "reverse_shape_limit",
                    PyLong_FromUnsignedLong(table.reverse_shape_limit())))
            {
                return nullptr;
            }

            if (!set_item(
                    result.get(),
                    "guid_count",
                    PyLong_FromUnsignedLong(table.guid_count())))
            {
                return nullptr;
            }

            if (!set_item(
                    result.get(),
                    "member_count",
                    PyLong_FromUnsignedLong(table.member_count())))
            {
                return nullptr;
            }

            if (!set_item(result.get(), "types", types.detach()))
            {
                return nullptr;
            }

            return result.detach();
        }
        catch (py::table::format_error const& error)
        {
            PyErr_SetString(PyExc_ImportError, error.what());
            return nullptr;
        }
        catch (...)
        {
            PyErr_SetString(PyExc_RuntimeError, "failed to read the projection table");
            return nullptr;
        }
    }
} // namespace py::cpp::_winrt
