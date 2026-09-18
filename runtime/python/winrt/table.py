"""
Compiles the text form of a projection table into the binary the runtime reads.

A projection package carries one ``_table.pywinrt.txt`` per WinRT namespace,
written by ``PyWinRT.exe`` and reviewed like any other source. The
``_table.pywinrt`` that sits beside the namespace's ``__init__.py`` wherever the
projection is installed is built from it here, which is why nothing generated is
checked in as a binary that no reviewer can read.

The two forms say the same things in the same order: the text names its records
where the binary numbers them, and everything else the binary carries - offsets,
sizes, the string and GUID pools - follows from the order the records are in.
Both are written up in ``runtime/src/table-format.md``, which is also where the
vocabulary below is explained; this module is one end of that contract and
``runtime/src/table.cpp`` is the other.
"""

import argparse
import struct
import uuid
from dataclasses import dataclass, field
from pathlib import Path
from typing import NoReturn

#: The format this writes. The major is the compatibility generation, so a text
#: table of another major says something this does not know how to compile.
FORMAT_MAJOR = 4
FORMAT_MINOR = 0

#: A reference to nothing: no type, no shape.
NO_REF = 0xFFFFFFFF

HEADER_SIZE = 48
SECTION_ENTRY_SIZE = 12
TYPE_RECORD_WORDS = 22
GROUP_RECORD_WORDS = 4
MEMBER_RECORD_WORDS = 10
PARAM_RECORD_WORDS = 4
FIELD_RECORD_WORDS = 4

CATEGORIES = ("enum", "struct", "interface", "class", "delegate")

TYPE_FLAGS = {
    "external": 1 << 3,
    "static": 1 << 4,
    "composable": 1 << 5,
    "parameterized": 1 << 6,
    "concrete": 1 << 7,
    "activatable": 1 << 8,
    "python_type": 1 << 9,
    "iterable": 1 << 10,
    "iterator": 1 << 11,
    "sequence": 1 << 12,
    "mutable_sequence": 1 << 13,
    "mapping": 1 << 14,
    "mutable_mapping": 1 << 15,
    "awaitable": 1 << 16,
    "closeable": 1 << 17,
    "stringable": 1 << 18,
    "buffer": 1 << 19,
    "buffer_length": 1 << 20,
}

GROUP_KINDS = ("method", "property", "event", "constructor")

GROUP_FLAGS = {"static": 1 << 3, "deprecated": 1 << 4}

MEMBER_KINDS = ("method", "get", "put", "add", "remove", "ctor")

#: The ``name=value`` tokens a member's line can carry.
MEMBER_VALUES = ("slot", "inputs", "outputs", "declaring", "shape", "reverse", "role")

MEMBER_FLAGS = {
    "static": 1 << 3,
    "overridable": 1 << 4,
    "protected": 1 << 5,
    "deprecated": 1 << 6,
    "default_overload": 1 << 7,
}

#: Where the member's protocol role sits in its flags.
MEMBER_ROLE_SHIFT = 8

ROLES = (
    "none",
    "size",
    "get_at",
    "set_at",
    "remove_at",
    "insert_at",
    "first",
    "current",
    "has_current",
    "move_next",
    "lookup",
    "has_key",
    "insert",
    "remove",
    "status",
    "completed",
    "get_results",
    "to_string",
    "value",
    "close",
)

PARAM_CATEGORIES = ("in", "out", "pass_array", "fill_array", "receive_array")

#: The ``name=value`` tokens a parameter's line can carry.
PARAM_VALUES = ("type", "name")

#: The ``name=value`` token a struct field's line can carry.
FIELD_VALUES = ("type",)

PARAM_FLAGS = {"return": 1 << 3, "implicit": 1 << 4, "by_reference": 1 << 5}

TYPE_CODES = (
    "void",
    "boolean",
    "int8",
    "uint8",
    "int16",
    "uint16",
    "int32",
    "uint32",
    "int64",
    "uint64",
    "single",
    "double",
    "char16",
    "string",
    "guid",
    "hresult",
    "event_token",
    "object",
    "datetime",
    "timespan",
    "enum32",
    "enum_u32",
    "struct",
    "interface",
    "class",
    "delegate",
    "parameterized",
    "reference",
)

#: The attributes of a type that name one other type.
TYPE_REFERENCES = ("base", "default")

#: The attributes of a type that name a list of other types, in the order the
#: binary pools those lists in.
TYPE_LISTS = {
    "requires": "requires",
    "argument": "arguments",
    "factory": "factories",
    "composable": "composable",
    "overridable": "overridable",
}


class TableTextError(Exception):
    """Raised when a table's text does not say what it has to."""


@dataclass
class Param:
    category: int
    code: int
    type: str | None
    name: str
    flags: int


@dataclass
class Member:
    kind: int
    winrt: str
    slot: int = 0
    inputs: int = 0
    outputs: int = 0
    declaring: str | None = None
    shape: int = NO_REF
    reverse: int = NO_REF
    role: int = 0
    flags: int = 0
    params: list[Param] = field(default_factory=list)


@dataclass
class Group:
    kind: int
    py: str
    flags: int = 0
    members: list[Member] = field(default_factory=list)


@dataclass
class Field:
    py: str
    winrt: str
    code: int
    type: str | None


@dataclass
class Type:
    category: int
    name: str
    flags: int
    py: str = ""
    guid: str | None = None
    signature: str = ""
    base: str | None = None
    default: str | None = None
    requires: list[str] = field(default_factory=list)
    arguments: list[str] = field(default_factory=list)
    factories: list[str] = field(default_factory=list)
    composable: list[str] = field(default_factory=list)
    overridable: list[str] = field(default_factory=list)
    fields: list[Field] = field(default_factory=list)
    groups: list[Group] = field(default_factory=list)


@dataclass
class Table:
    major: int
    minor: int
    generator: str
    namespace: str
    types: list[Type] = field(default_factory=list)


def compile_table(source: Path | str, destination: Path | str) -> None:
    """
    Compiles one namespace's table from its text form into its binary form.

    Args:
        source: The ``_table.pywinrt.txt`` to read.
        destination: The ``_table.pywinrt`` to write.
    """
    source = Path(source)

    table = parse(source.read_text(encoding="utf-8"), source)

    Path(destination).write_bytes(build(table))


def parse(text: str, source: Path | str = "<text>") -> Table:
    """
    Reads the text form of a table.

    Args:
        text: The contents of a ``_table.pywinrt.txt``.
        source: What to call it in an error message.
    """
    parser = _Parser(source)

    for number, line in enumerate(text.splitlines(), start=1):
        parser.line(number, line)

    return parser.finish()


class _Parser:
    """
    Reads a table line by line.

    Indentation says what a line describes: a type at the left margin, then what
    belongs to it, then the members of one of its groups, then the parameters of
    one of those members. Anything optional is a ``name=value`` token or a bare
    flag, so that only the fixed head of a line is read by position.
    """

    def __init__(self, source: Path | str) -> None:
        self._source = source
        self._table: Table | None = None
        self._type: Type | None = None
        self._group: Group | None = None
        self._member: Member | None = None
        self._header: dict[str, str] = {}

    def line(self, number: int, text: str) -> None:
        self._number = number

        if not text.strip() or text.lstrip().startswith("#"):
            return

        indent = len(text) - len(text.lstrip(" "))

        if indent % 4:
            self._fail(f"the line is indented by {indent} spaces")

        level = indent // 4
        keyword, _, rest = text.strip().partition(" ")

        if level == 0:
            if self._table is None and keyword in ("format", "generator", "namespace"):
                self._header[keyword] = rest
                return

            self._start_type(keyword, rest)
        elif level == 1:
            self._type_line(keyword, rest)
        elif level == 2:
            self._member_line(keyword, rest)
        elif level == 3:
            self._param_line(keyword, rest)
        else:
            self._fail("the line is indented further than a parameter")

    def finish(self) -> Table:
        if self._table is None:
            self._start_table()

        assert self._table is not None

        return self._table

    def _start_table(self) -> None:
        for name in ("format", "generator", "namespace"):
            if name not in self._header:
                self._fail(f"the table does not say what its {name} is")

        version = self._header["format"].split(".")

        if len(version) != 2 or not all(part.isdigit() for part in version):
            self._fail(f"'{self._header['format']}' is not a format version")

        major, minor = int(version[0]), int(version[1])

        if major != FORMAT_MAJOR:
            self._fail(
                f"the table is format {major}.{minor} and this writes format "
                f"{FORMAT_MAJOR}.{FORMAT_MINOR}"
            )

        if minor > FORMAT_MINOR:
            self._fail(
                f"the table is format {major}.{minor}, which says more than "
                f"format {FORMAT_MAJOR}.{FORMAT_MINOR} can write"
            )

        self._table = Table(
            major=major,
            minor=minor,
            generator=self._header["generator"],
            namespace=self._header["namespace"],
        )

    def _start_type(self, keyword: str, rest: str) -> None:
        if self._table is None:
            self._start_table()

        assert self._table is not None

        if keyword not in CATEGORIES:
            self._fail(f"'{keyword}' is not a kind of type")

        name, *flags = rest.split(" ") if rest else [""]

        if not name:
            self._fail(f"the {keyword} has no name")

        self._type = Type(
            category=CATEGORIES.index(keyword),
            name=name,
            flags=self._flags(flags, TYPE_FLAGS, "type"),
        )
        self._group = None
        self._member = None

        self._table.types.append(self._type)

    def _type_line(self, keyword: str, rest: str) -> None:
        if self._type is None:
            self._fail(f"'{keyword}' belongs to no type")

        if keyword in GROUP_KINDS:
            name, *flags = rest.split(" ") if rest else [""]

            if not name:
                self._fail(f"the {keyword} has no name, not even a dash")

            self._group = Group(
                kind=GROUP_KINDS.index(keyword),
                py="" if name == "-" else name,
                flags=self._flags(flags, GROUP_FLAGS, "group"),
            )
            self._member = None

            self._type.groups.append(self._group)
            return

        if keyword in ("py", "signature"):
            setattr(self._type, keyword, rest)
        elif keyword == "guid":
            self._type.guid = rest
        elif keyword in TYPE_REFERENCES:
            setattr(self._type, keyword, rest)
        elif keyword in TYPE_LISTS:
            getattr(self._type, TYPE_LISTS[keyword]).append(rest)
        elif keyword == "field":
            self._type.fields.append(self._field(rest))
        else:
            self._fail(f"'{keyword}' says nothing about a type")

    def _field(self, rest: str) -> Field:
        tokens = rest.split(" ")

        if len(tokens) < 3:
            self._fail("a field is a Python name, a WinRT name and a type code")

        py, winrt, code, *extra = tokens
        values, _ = self._values(extra, FIELD_VALUES, {}, "field")

        return Field(py=py, winrt=winrt, code=self._code(code), type=values.get("type"))

    def _member_line(self, keyword: str, rest: str) -> None:
        if self._group is None:
            self._fail(f"'{keyword}' belongs to no group")

        if keyword not in MEMBER_KINDS:
            self._fail(f"'{keyword}' is not a kind of member")

        name, *tokens = rest.split(" ") if rest else [""]

        if not name:
            self._fail(f"the {keyword} has no WinRT name")

        values, flags = self._values(tokens, MEMBER_VALUES, MEMBER_FLAGS, "member")

        self._member = Member(
            kind=MEMBER_KINDS.index(keyword),
            winrt=name,
            slot=self._number_value(values, "slot", 0),
            inputs=self._number_value(values, "inputs", 0),
            outputs=self._number_value(values, "outputs", 0),
            declaring=values.get("declaring"),
            shape=self._number_value(values, "shape", NO_REF),
            reverse=self._number_value(values, "reverse", NO_REF),
            role=self._role(values.get("role")),
            flags=flags,
        )

        self._group.members.append(self._member)

    def _param_line(self, keyword: str, rest: str) -> None:
        if self._member is None:
            self._fail(f"'{keyword}' belongs to no member")

        if keyword not in PARAM_CATEGORIES:
            self._fail(f"'{keyword}' is not a parameter category")

        code, *tokens = rest.split(" ") if rest else [""]

        if not code:
            self._fail("the parameter has no type code")

        values, flags = self._values(tokens, PARAM_VALUES, PARAM_FLAGS, "parameter")

        self._member.params.append(
            Param(
                category=PARAM_CATEGORIES.index(keyword),
                code=self._code(code),
                type=values.get("type"),
                name=values.get("name", ""),
                flags=flags,
            )
        )

    def _values(
        self, tokens: list[str], names: tuple[str, ...], flags: dict[str, int], what: str
    ) -> tuple[dict[str, str], int]:
        """
        Reads the ``name=value`` tokens and the bare flags at the end of a line.

        A token this kind of line has no use for is refused rather than passed
        over, since a table is written by a program: anything unexpected in one
        is a mistake at the writing end, and the reader is where it shows.
        """
        values: dict[str, str] = {}
        value_of_flags = 0

        for token in tokens:
            name, separator, value = token.partition("=")

            if separator:
                if name not in names:
                    self._fail(f"'{token}' says nothing about a {what}")

                values[name] = value
            elif name in flags:
                value_of_flags |= flags[name]
            else:
                self._fail(f"'{token}' says nothing about a {what}")

        return values, value_of_flags

    def _number_value(self, values: dict[str, str], name: str, default: int) -> int:
        if name not in values:
            return default

        value = values[name]

        if not value.isdigit():
            self._fail(f"'{name}={value}' is not a number")

        return int(value)

    def _code(self, name: str) -> int:
        if name not in TYPE_CODES:
            self._fail(f"'{name}' is not a type code")

        return TYPE_CODES.index(name)

    def _role(self, name: str | None) -> int:
        if name is None:
            return 0

        if name not in ROLES:
            self._fail(f"'{name}' is not a protocol role")

        return ROLES.index(name)

    def _flags(self, names: list[str], flags: dict[str, int], what: str) -> int:
        value = 0

        for name in names:
            if name not in flags:
                self._fail(f"'{name}' is not a {what} flag")

            value |= flags[name]

        return value

    def _fail(self, message: str) -> NoReturn:
        raise TableTextError(f"{self._source}:{self._number}: {message}")


def build(table: Table) -> bytes:
    """
    Writes the binary form of a table that has been read.

    The records keep the order they are in, since that order is what a reference
    to one of them becomes: an index the runtime resolves without searching.
    """
    types = table.types
    index = {}

    for position, type_ in enumerate(types):
        if type_.name in index:
            raise TableTextError(f"'{type_.name}' is written twice")

        index[type_.name] = position

    def ref(name: str | None) -> int:
        if name is None:
            return NO_REF

        if name not in index:
            raise TableTextError(f"'{name}' is named but never written")

        return index[name]

    groups: list[Group] = []
    members: list[Member] = []
    params: list[Param] = []
    fields: list[Field] = []
    first_group: list[int] = []
    first_field: list[int] = []
    first_member: list[int] = []
    first_param: list[int] = []

    for type_ in types:
        first_group.append(len(groups))

        for group in type_.groups:
            groups.append(group)
            first_member.append(len(members))

            for member in group.members:
                members.append(member)
                first_param.append(len(params))
                params.extend(member.params)

        first_field.append(len(fields))
        fields.extend(type_.fields)

    strings = _Strings()
    guids = _Guids()
    refs: list[int] = []

    # Every string, GUID and list is pooled before anything is laid out, so that
    # the sections have their final size when the offsets are computed.
    strings.add(table.generator)
    strings.add(table.namespace)

    for type_ in types:
        winrt_namespace, winrt_name = _split(type_.name)

        strings.add(winrt_name)
        strings.add(winrt_namespace)
        strings.add(type_.py)
        strings.add(type_.signature)

        if type_.guid is not None:
            guids.add(type_.guid)

    for group in groups:
        strings.add(group.py)

    for member in members:
        strings.add(member.winrt)

    for param in params:
        strings.add(param.name)

    for struct_field in fields:
        strings.add(struct_field.py)
        strings.add(struct_field.winrt)

    def add_list(names: list[str]) -> int:
        if not names:
            return 0

        first = len(refs)
        refs.extend(ref(name) for name in names)

        return first

    lists = [
        (
            add_list(type_.requires),
            add_list(type_.arguments),
            add_list(type_.factories),
            add_list(type_.composable),
            add_list(type_.overridable),
        )
        for type_ in types
    ]

    sections = [
        ("FLDS", len(fields) * FIELD_RECORD_WORDS * 4),
        ("GRUP", len(groups) * GROUP_RECORD_WORDS * 4),
        ("GUID", len(guids) * 16),
        ("MEMB", len(members) * MEMBER_RECORD_WORDS * 4),
        ("PARM", len(params) * PARAM_RECORD_WORDS * 4),
        ("REFS", len(refs) * 4),
        ("STRS", len(strings)),
        ("TYPE", len(types) * TYPE_RECORD_WORDS * 4),
    ]

    directory_offset = _align(HEADER_SIZE, 8)
    offset = _align(directory_offset + len(sections) * SECTION_ENTRY_SIZE, 8)
    offsets = {}

    for tag, size in sections:
        offsets[tag] = offset
        offset = _align(offset + size, 8)

    data = bytearray(offset)

    struct.pack_into(
        "<8sHHIII",
        data,
        0,
        b"PYWINRT\x1a",
        table.major,
        table.minor,
        len(data),
        len(sections),
        directory_offset,
    )
    struct.pack_into(
        "<IIII",
        data,
        24,
        strings.add(table.generator),
        strings.add(table.namespace),
        max((m.shape + 1 for m in members if m.shape != NO_REF), default=0),
        max((m.reverse + 1 for m in members if m.reverse != NO_REF), default=0),
    )

    for position, (tag, size) in enumerate(sections):
        struct.pack_into(
            "<III",
            data,
            directory_offset + position * SECTION_ENTRY_SIZE,
            int.from_bytes(tag.encode("ascii"), "little"),
            offsets[tag],
            size,
        )

    data[offsets["STRS"] : offsets["STRS"] + len(strings)] = bytes(strings)
    data[offsets["GUID"] : offsets["GUID"] + len(guids) * 16] = bytes(guids)

    _pack_words(data, offsets["REFS"], refs)

    for position, type_ in enumerate(types):
        winrt_namespace, winrt_name = _split(type_.name)
        requires, arguments, factories, composable, overridable = lists[position]

        _pack_words(
            data,
            offsets["TYPE"] + position * TYPE_RECORD_WORDS * 4,
            [
                type_.flags | type_.category,
                strings.add(winrt_name),
                strings.add(winrt_namespace),
                strings.add(type_.py),
                strings.add(type_.signature),
                NO_REF if type_.guid is None else guids.add(type_.guid),
                ref(type_.base),
                ref(type_.default),
                requires,
                len(type_.requires),
                arguments,
                len(type_.arguments),
                first_group[position] if type_.groups else 0,
                len(type_.groups),
                first_field[position],
                len(type_.fields),
                factories,
                len(type_.factories),
                composable,
                len(type_.composable),
                overridable,
                len(type_.overridable),
            ],
        )

    for position, group in enumerate(groups):
        _pack_words(
            data,
            offsets["GRUP"] + position * GROUP_RECORD_WORDS * 4,
            [
                strings.add(group.py),
                group.flags | group.kind,
                first_member[position],
                len(group.members),
            ],
        )

    for position, member in enumerate(members):
        _pack_words(
            data,
            offsets["MEMB"] + position * MEMBER_RECORD_WORDS * 4,
            [
                member.flags | member.kind | (member.role << MEMBER_ROLE_SHIFT),
                strings.add(member.winrt),
                ref(member.declaring),
                member.slot,
                member.shape,
                first_param[position],
                len(member.params),
                member.inputs,
                member.outputs,
                member.reverse,
            ],
        )

    for position, param in enumerate(params):
        _pack_words(
            data,
            offsets["PARM"] + position * PARAM_RECORD_WORDS * 4,
            [
                param.flags | param.category,
                param.code,
                ref(param.type),
                strings.add(param.name),
            ],
        )

    for position, struct_field in enumerate(fields):
        _pack_words(
            data,
            offsets["FLDS"] + position * FIELD_RECORD_WORDS * 4,
            [
                strings.add(struct_field.py),
                strings.add(struct_field.winrt),
                struct_field.code,
                ref(struct_field.type),
            ],
        )

    return bytes(data)


def _split(name: str) -> tuple[str, str]:
    """
    Splits the name a type goes by into the WinRT namespace and name it records.

    A parameterized type's arguments are part of what tells two records apart,
    but they are written down as a list of their own, so they are not part of
    either. A type argument that is still standing in for a type belongs to no
    namespace.
    """
    if "<" in name:
        name = name[: name.index("<")]

    namespace, _, simple = name.rpartition(".")

    return namespace, simple


def _align(value: int, alignment: int) -> int:
    return (value + alignment - 1) // alignment * alignment


def _pack_words(data: bytearray, offset: int, words: list[int]) -> None:
    struct.pack_into(f"<{len(words)}I", data, offset, *words)


class _Strings:
    """The NUL terminated UTF-8 strings of a table, each one stored once."""

    def __init__(self) -> None:
        # offset zero is the empty string
        self._data = bytearray(b"\x00")
        self._offsets = {"": 0}

    def add(self, value: str) -> int:
        offset = self._offsets.get(value)

        if offset is None:
            offset = len(self._data)
            self._data += value.encode("utf-8") + b"\x00"
            self._offsets[value] = offset

        return offset

    def __len__(self) -> int:
        return len(self._data)

    def __bytes__(self) -> bytes:
        return bytes(self._data)


class _Guids:
    """
    The GUIDs of a table, each one stored once, in the memory order that
    ``winrt::guid`` uses.
    """

    def __init__(self) -> None:
        self._indexes: dict[str, int] = {}
        self._data = bytearray()

    def add(self, value: str) -> int:
        index = self._indexes.get(value)

        if index is None:
            index = len(self._indexes)
            self._data += uuid.UUID(value).bytes_le
            self._indexes[value] = index

        return index

    def __len__(self) -> int:
        return len(self._indexes)

    def __bytes__(self) -> bytes:
        return bytes(self._data)


def main(argv: list[str] | None = None) -> None:
    parser = argparse.ArgumentParser(description=__doc__.strip().splitlines()[0])
    parser.add_argument("source", type=Path, help="the _table.pywinrt.txt to read")
    parser.add_argument("destination", type=Path, help="the _table.pywinrt to write")

    args = parser.parse_args(argv)

    compile_table(args.source, args.destination)


if __name__ == "__main__":
    main()
