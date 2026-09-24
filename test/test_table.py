"""
The projection table, written by the generator and read by the runtime.

A projection package ships one ``_table.pywinrt`` per WinRT namespace and no
compiled code, and the ends of that file are in three languages and three
processes: ``PyWinRT/TableWriter.cs`` writes the text a package carries,
``winrt/table.py`` compiles it when the package is built, and
``runtime/src/table.cpp`` reads the result. Nothing else can tell whether they
agree about where a field is, so this writes nothing and reads everything: the
tables that the build installed, through the runtime's own reader.
``test_table_text.py`` covers the first two on their own.

The GUIDs get a check of their own. A wrong IID still loads: it makes
``QueryInterface`` ask for an interface that does not exist, or worse, for the
wrong one. Every parameterized instance is therefore hashed again here, by an
independent implementation of the WinRT algorithm, and a sample of the plain
interfaces is compared against the published values, which is what
``winrt::guid_of<T>()`` yields for them.
"""

import enum
import importlib
import pathlib
import struct
import sys
import tempfile
import types
import unittest
import uuid
from typing import Any

import winrt._winrt

# The category in the low bits of a type record's flags.
CATEGORY_ENUM = 0
CATEGORY_STRUCT = 1
CATEGORY_INTERFACE = 2
CATEGORY_CLASS = 3
CATEGORY_DELEGATE = 4

TYPE_EXTERNAL = 1 << 3
TYPE_PARAMETERIZED = 1 << 6
TYPE_CONCRETE = 1 << 7
TYPE_DEFAULT_ACTIVATABLE = 1 << 8
TYPE_PYTHON_TYPE = 1 << 9
TYPE_ITERABLE = 1 << 10
TYPE_ITERATOR = 1 << 11
TYPE_SEQUENCE = 1 << 12
TYPE_MUTABLE_SEQUENCE = 1 << 13
TYPE_MAPPING = 1 << 14
TYPE_MUTABLE_MAPPING = 1 << 15
TYPE_AWAITABLE = 1 << 16
TYPE_CLOSEABLE = 1 << 17
TYPE_STRINGABLE = 1 << 18
TYPE_FLAGS_ENUM = 1 << 22

GROUP_METHOD = 0
GROUP_PROPERTY = 1
GROUP_EVENT = 2
GROUP_CONSTRUCTOR = 3

GROUP_STATIC = 1 << 3

MEMBER_PROPERTY_GET = 1
MEMBER_CONSTRUCTOR = 5

NO_REF = 0xFFFFFFFF

# The part a member plays in a Python protocol, in the free bits of its flags.
ROLE_SIZE = 1
ROLE_GET_AT = 2
ROLE_SET_AT = 3
ROLE_REMOVE_AT = 4
ROLE_INSERT_AT = 5
ROLE_FIRST = 6
ROLE_CURRENT = 7
ROLE_HAS_CURRENT = 8
ROLE_MOVE_NEXT = 9
ROLE_LOOKUP = 10
ROLE_HAS_KEY = 11
ROLE_INSERT = 12
ROLE_REMOVE = 13
ROLE_STATUS = 14
ROLE_COMPLETED = 15
ROLE_GET_RESULTS = 16
ROLE_TO_STRING = 17
ROLE_VALUE = 18
ROLE_CLOSE = 19
ROLE_GET_MANY = 20
ROLE_GET_VIEW = 21
ROLE_INDEX_OF = 22
ROLE_APPEND = 23
ROLE_REMOVE_AT_END = 24
ROLE_CLEAR = 25
ROLE_REPLACE_ALL = 26

# The members each protocol calls, by the type flag that claims it. A
# collection is called in both directions - Python reading a WinRT one, and
# WinRT reading a Python object through the vtable the runtime assembles over
# it - so the set includes the members that only the second of those uses.
REQUIRED_ROLES = {
    TYPE_SEQUENCE: {ROLE_SIZE, ROLE_GET_AT, ROLE_GET_MANY, ROLE_INDEX_OF},
    TYPE_MUTABLE_SEQUENCE: {
        ROLE_SET_AT,
        ROLE_INSERT_AT,
        ROLE_REMOVE_AT,
        ROLE_APPEND,
        ROLE_REMOVE_AT_END,
        ROLE_CLEAR,
        ROLE_REPLACE_ALL,
        ROLE_GET_VIEW,
    },
    TYPE_MAPPING: {ROLE_SIZE, ROLE_LOOKUP, ROLE_HAS_KEY},
    TYPE_MUTABLE_MAPPING: {ROLE_INSERT, ROLE_REMOVE, ROLE_CLEAR, ROLE_GET_VIEW},
    TYPE_ITERATOR: {
        ROLE_CURRENT,
        ROLE_HAS_CURRENT,
        ROLE_MOVE_NEXT,
        ROLE_GET_MANY,
    },
    TYPE_AWAITABLE: {ROLE_STATUS, ROLE_COMPLETED, ROLE_GET_RESULTS},
    TYPE_STRINGABLE: {ROLE_TO_STRING},
    TYPE_CLOSEABLE: {ROLE_CLOSE},
}

# The type codes of the two parameterized interfaces a member can pass: a
# concrete instance, and the IReference<T> that the projection unwraps.
CODE_GENERIC = 26
CODE_REFERENCE = 27

# The first vtable slot after IUnknown's three and IInspectable's three.
FIRST_INTERFACE_SLOT = 6

# The namespace that winrt::impl::generate_guid hashes a parameterized
# interface's signature under.
PINTERFACE_NAMESPACE = uuid.UUID("11f47ad5-7b73-42c0-abae-878b1e16adee")

# A sample of published IIDs, which is what C++/WinRT's guid_of<T>() answers for
# these types.
PUBLISHED_IIDS = {
    ("Windows.Foundation", "IAsyncInfo"): "00000036-0000-0000-c000-000000000046",
    ("Windows.Foundation", "IClosable"): "30d5a829-7fa4-4026-83bb-d75bae4ea99e",
    ("Windows.Foundation", "IPropertyValue"): "4bd682dd-7554-40e9-9a9b-82654ede7e62",
    ("Windows.Foundation", "IStringable"): "96369f54-8eb6-48f0-abce-c1b211e627c3",
    (
        "Windows.Foundation.Collections",
        "IIterable<String>",
    ): "e2fcc7c1-3bfc-5a0b-b2b0-72e769d1cb7e",
}


def find_table(*parts: str) -> bytes:
    """
    Reads an installed table by the path of its namespace package.

    The projection packages cannot be imported yet, so the file is found the way
    the import system would find it, by walking sys.path.
    """
    for entry in sys.path:
        path = pathlib.Path(entry).joinpath(*parts, "_table.pywinrt")

        if path.is_file():
            return path.read_bytes()

    raise unittest.SkipTest(f"no {'/'.join(parts)} table on sys.path")


def read(*parts: str) -> dict[str, Any]:
    return winrt._winrt.read_table(find_table(*parts))


def refuse(case: unittest.TestCase, data: bytes) -> str:
    """
    Puts ``data`` through the import path and returns why it was refused.

    The checks that compare a table against what this winrt-runtime was
    built with are made where a namespace is loaded into its module rather
    than where the bytes are read, so a table that is well formed but not
    this runtime's has to go through ``load_projection`` to be seen.

    The directory is left to the operating system to clean up: a table is
    mapped once and never unmapped, because the names and descriptors built
    from it point into it, and Windows does not delete a file that a view is
    still mapped over.
    """
    with tempfile.TemporaryDirectory(ignore_cleanup_errors=True) as directory:
        path = pathlib.Path(directory) / "_table.pywinrt"
        path.write_bytes(data)

        with case.assertRaises(ImportError) as caught:
            winrt._winrt.load_projection(
                types.ModuleType(f"_not_a_projection_{path.parent.name}"), str(path)
            )

        return str(caught.exception)


def defined(table: dict[str, Any]) -> dict[str, dict[str, Any]]:
    """The types the namespace defines, by WinRT name."""
    return {
        t["name"]: t
        for t in table["types"]
        if not t["flags"] & TYPE_EXTERNAL and not t["flags"] & TYPE_CONCRETE
    }


def groups(type_record: dict[str, Any]) -> dict[str, dict[str, Any]]:
    return {g["py_name"]: g for g in type_record["groups"]}


def roles(type_record: dict[str, Any]) -> set[int]:
    """The protocol roles the members of a type play, without the plain ones."""
    return {
        member["role"] for group in type_record["groups"] for member in group["members"]
    } - {0}


def pinterface_guid(signature: str) -> uuid.UUID:
    """
    The IID of a parameterized instance, computed from its WinRT signature.

    This is a version 5 UUID under the WinRT namespace, which is exactly what
    ``winrt::impl::generate_guid`` does with a constexpr SHA-1.
    """
    return uuid.uuid5(PINTERFACE_NAMESPACE, signature)


def guid_of(type_record: dict[str, Any]) -> uuid.UUID | None:
    # a GUID crosses as the sixteen bytes of a winrt::guid, which is the memory
    # order a Windows GUID has rather than the network order a UUID has
    return (
        None if type_record["guid"] is None else uuid.UUID(bytes_le=type_record["guid"])
    )


class TestTableFormat(unittest.TestCase):
    """The header and the validation that stands in front of everything else."""

    def test_header(self) -> None:
        table = read("test_winrt", "testcomponent")

        # the major is the compatibility generation, the same number as the
        # ABI major and the epoch in a projection package's version
        self.assertEqual(table["format"][0], winrt._winrt.abi_version[0])
        self.assertEqual(table["namespace"], "TestComponent")
        self.assertRegex(table["generator"], r"^\d+\.\d+\.\d+$")
        self.assertGreater(table["forward_shape_limit"], 0)
        self.assertGreater(table["reverse_shape_limit"], 0)

        # the census the shape ids were assigned by: one lineage for the
        # whole tree, since one winrt-runtime resolves every table in it. The
        # revision is the oldest one that has the ids this table names, so two
        # namespaces share the lineage and each carries a revision of its own.
        lineage, revision = table["census"]

        self.assertEqual(uuid.UUID(lineage).version, 4)
        self.assertGreater(revision, 0)

        other_lineage, other_revision = read("winrt", "windows", "foundation")["census"]

        self.assertEqual(other_lineage, lineage)
        self.assertGreater(other_revision, 0)

    def test_rejects_what_is_not_a_table(self) -> None:
        for name, data in (
            ("empty", b""),
            ("short", b"PYWINRT"),
            ("wrong magic", b"PYWINRU\x1a" + bytes(40)),
        ):
            with self.subTest(case=name), self.assertRaises(ImportError):
                winrt._winrt.read_table(data)

    def test_rejects_a_truncated_table(self) -> None:
        data = find_table("test_winrt", "testcomponent")

        with self.assertRaises(ImportError):
            winrt._winrt.read_table(data[: len(data) // 2])

    def test_rejects_a_newer_format(self) -> None:
        data = bytearray(find_table("test_winrt", "testcomponent"))
        # the minor version, which a runtime refuses to read when it is ahead of
        # its own
        struct.pack_into("<H", data, 10, 99)

        with self.assertRaises(ImportError):
            winrt._winrt.read_table(bytes(data))

    def test_rejects_another_census(self) -> None:
        """
        A table whose shape ids were assigned by a census that is not this
        runtime's, which is what generating against an empty directory
        produces. The ids start at zero again in that run's own order, so
        each of them names a trampoline for some other signature, and none
        of them is out of range.
        """
        data = find_table("test_winrt", "testcomponent")
        lineage = winrt._winrt.read_table(data)["census"][0]
        other = str(uuid.UUID(int=uuid.UUID(lineage).int ^ 1))

        message = refuse(self, data.replace(lineage.encode(), other.encode()))

        self.assertIn(other, message)
        self.assertIn(lineage, message)

    def test_rejects_a_later_revision_of_this_census(self) -> None:
        """
        A table from the same census after someone else appended to it,
        which the lineage alone does not catch and the shape id bound does
        not either, because their census is still smaller than this one.
        """
        data = bytearray(find_table("test_winrt", "testcomponent"))
        # the census revision, which a runtime has no trampoline for beyond
        # the one its own shapes-generated.h was written from
        struct.pack_into("<I", data, 44, 99)

        self.assertIn("revision 99", refuse(self, bytes(data)))


class TestTableContents(unittest.TestCase):
    """What the writer put in and the reader takes out."""

    table: dict[str, Any]
    types: dict[str, dict[str, Any]]

    @classmethod
    def setUpClass(cls) -> None:
        cls.table = read("test_winrt", "testcomponent")
        cls.types = defined(cls.table)

    def test_types_are_sorted_and_defined_first(self) -> None:
        names = [
            t["name"]
            for t in self.table["types"]
            if not t["flags"] & TYPE_EXTERNAL and not t["flags"] & TYPE_CONCRETE
        ]

        self.assertEqual(names, sorted(names))

        external = [
            i for i, t in enumerate(self.table["types"]) if t["flags"] & TYPE_EXTERNAL
        ]
        internal = [
            i
            for i, t in enumerate(self.table["types"])
            if not t["flags"] & TYPE_EXTERNAL
        ]

        if external and internal:
            self.assertLess(max(internal), min(external))

    def test_categories(self) -> None:
        for name, category in (
            ("Blittable", CATEGORY_STRUCT),
            ("Class", CATEGORY_CLASS),
            ("ITests", CATEGORY_INTERFACE),
            ("TestHandler", CATEGORY_DELEGATE),
        ):
            with self.subTest(type=name):
                self.assertEqual(self.types[name]["category"] & 0x7, category)

    def test_an_interface_is_bound_to_a_private_name(self) -> None:
        # a class keeps its name, an interface gets an underscore so that the
        # two cannot collide
        self.assertEqual(self.types["Class"]["py_name"], "Class")
        self.assertEqual(self.types["ITests"]["py_name"], "_ITests")

    def test_a_property_getter_is_the_first_slot_of_its_interface(self) -> None:
        # ITests declares Percentage first, so its getter is the first entry
        # after IUnknown's three and IInspectable's three
        member = groups(self.types["ITests"])["percentage"]["members"][0]

        self.assertEqual(member["winrt_name"], "get_Percentage")
        self.assertEqual(member["kind"], MEMBER_PROPERTY_GET)
        self.assertEqual(member["slot"], FIRST_INTERFACE_SLOT)
        self.assertEqual(member["in_count"], 0)
        self.assertEqual(member["out_count"], 1)

    def test_a_ref_parameter_is_marked_by_reference(self) -> None:
        # Param13 takes one Blittable by value and the next by reference,
        # which the ABI passes as a pointer. The two spellings are the same
        # machine convention on x64 and different ones on x86, so the flag is
        # the only thing that says which a call has to build.
        member = groups(self.types["ITests"])["param13"]["members"][0]
        params = member["params"]

        self.assertEqual([p["name"] for p in params], ["a", "b", "c", ""])
        self.assertEqual(
            [p["is_by_reference"] for p in params], [False, True, False, False]
        )

    def test_an_event_is_an_add_and_a_remove(self) -> None:
        group = groups(self.types["ITests"])["event1"]

        self.assertEqual(group["kind"], GROUP_EVENT)
        self.assertEqual(
            [m["winrt_name"] for m in group["members"]], ["add_Event1", "remove_Event1"]
        )
        # an event registration is two consecutive slots
        self.assertEqual(group["members"][1]["slot"], group["members"][0]["slot"] + 1)

    def test_a_static_member_is_declared_by_its_factory_interface(self) -> None:
        runner = self.types["TestRunner"]
        group = groups(runner)["test_producer"]

        self.assertTrue(group["flags"] & GROUP_STATIC)

        member = group["members"][0]
        declaring = self.table["types"][member["declaring"]]

        self.assertEqual(declaring["name"], "ITestRunnerStatics")
        # TestProducer is the first method the static interface declares
        self.assertEqual(member["slot"], FIRST_INTERFACE_SLOT)
        self.assertIn(member["declaring"], runner["factories"])

    def test_plain_activation_has_no_declaring_interface(self) -> None:
        cls = self.types["Class"]

        self.assertTrue(cls["flags"] & TYPE_DEFAULT_ACTIVATABLE)

        group = next(g for g in cls["groups"] if g["kind"] == GROUP_CONSTRUCTOR)
        member = next(m for m in group["members"] if m["in_count"] == 0)

        self.assertEqual(member["kind"], MEMBER_CONSTRUCTOR)
        # IActivationFactory::ActivateInstance, which no interface record names
        self.assertEqual(member["declaring"], NO_REF)
        self.assertEqual(member["slot"], FIRST_INTERFACE_SLOT)
        self.assertEqual(member["out_count"], 1)

    def test_overloads_are_sorted_by_argument_count(self) -> None:
        # WinRT overloads a method by the number of arguments, so a group is
        # dispatched on that and the members are in the order it compares them
        table = read("winrt", "windows", "foundation")
        overloaded = 0

        for type_record in table["types"]:
            for group in type_record["groups"]:
                if group["kind"] not in (GROUP_METHOD, GROUP_CONSTRUCTOR):
                    continue

                counts = [m["in_count"] for m in group["members"]]

                if len(counts) > 1:
                    overloaded += 1

                with self.subTest(type=type_record["name"], group=group["py_name"]):
                    self.assertEqual(counts, sorted(counts))
                    self.assertEqual(len(counts), len(set(counts)))

        self.assertGreater(overloaded, 0)

    def test_every_member_has_a_shape(self) -> None:
        limit = self.table["forward_shape_limit"]
        count = 0

        for type_record in self.table["types"]:
            for group in type_record["groups"]:
                for member in group["members"]:
                    count += 1
                    self.assertNotEqual(member["forward_shape"], NO_REF)
                    self.assertLess(member["forward_shape"], limit)

        self.assertGreater(count, 0)

    def test_a_python_type_has_a_name_and_a_reference_does_not(self) -> None:
        for type_record in self.table["types"]:
            with self.subTest(type=type_record["name"]):
                if type_record["flags"] & TYPE_PYTHON_TYPE:
                    self.assertTrue(type_record["py_name"])
                    self.assertFalse(type_record["flags"] & TYPE_EXTERNAL)
                elif type_record["flags"] & TYPE_EXTERNAL:
                    # a reference is resolved through the type registry, which
                    # knows a type by its fully qualified Python name
                    self.assertIn(".", type_record["py_name"])


class TestStructLayout(unittest.TestCase):
    """
    The layouts the reader computes, against what Python's struct module makes
    of the same fields.

    Nothing writes an offset into a table, because a pointer is not the same
    width on every architecture that one table serves, so getting this wrong
    would be a silent misread of every struct in the projection.
    """

    types: dict[str, dict[str, Any]]

    @classmethod
    def setUpClass(cls) -> None:
        cls.types = defined(read("test_winrt", "testcomponent"))

    def test_blittable(self) -> None:
        # uint8, uint16, uint32, uint64, int16, int32, int64, single, double and
        # a GUID, which is four bytes then two shorts then eight bytes
        blittable = self.types["Blittable"]

        self.assertEqual(blittable["size"], struct.calcsize("BHIQhiqfdI2H8B"))
        self.assertEqual(blittable["align"], 8)
        self.assertEqual(
            [f["offset"] for f in blittable["fields"]],
            [0, 2, 4, 8, 16, 20, 24, 32, 40, 48],
        )

    def test_a_pointer_field_is_as_wide_as_a_pointer(self) -> None:
        # boolean, char16, string and IReference<int32>, so the last two are
        # pointers and where they land says how wide the reader thinks one is
        non_blittable = self.types["NonBlittable"]
        pointer = struct.calcsize("P")

        self.assertEqual(
            [f["offset"] for f in non_blittable["fields"]],
            [0, 2, pointer, 2 * pointer],
        )
        self.assertEqual(non_blittable["size"], 3 * pointer)
        self.assertEqual(non_blittable["align"], pointer)

    def test_a_nested_struct_is_laid_out_in_place(self) -> None:
        nested = self.types["Nested"]
        blittable = self.types["Blittable"]
        non_blittable = self.types["NonBlittable"]

        # A nested struct's fields sit in the outer one rather than behind a
        # pointer, so the second starts where the first ends. The size is that
        # rounded up to the outer struct's own alignment, which is not always
        # the sum: on x86 NonBlittable is twelve bytes and Blittable is aligned
        # to eight, so Nested ends four bytes short of a multiple of its
        # alignment and is padded out to one.
        end = blittable["size"] + non_blittable["size"]
        align = nested["align"]

        self.assertEqual(nested["size"], -(-end // align) * align)
        self.assertEqual(
            [f["offset"] for f in nested["fields"]], [0, blittable["size"]]
        )


class TestTableParameterizedTypes(unittest.TestCase):
    """What a member of a concrete parameterized type is allowed to name."""

    def test_a_concrete_type_names_no_type_without_arguments(self) -> None:
        # A parameterized type with a type argument still standing in for a
        # type has no IID and no members, so a parameter of one cannot be
        # converted. The definition's own members may name one - they have no
        # call shape either and are never callable - but anything that values
        # exist of must not, and working that out means composing the type
        # arguments along the whole chain of required interfaces rather than
        # one level at a time.
        unusable = []

        for parts in (
            ("test_winrt", "testcomponent"),
            ("winrt", "windows", "foundation"),
            ("winrt", "windows", "foundation", "collections"),
            ("winrt", "windows", "data", "json"),
        ):
            table = read(*parts)
            types = table["types"]

            for type_record in types:
                if (
                    type_record["flags"] & TYPE_PARAMETERIZED
                    and not type_record["flags"] & TYPE_CONCRETE
                ):
                    continue

                for group in type_record["groups"]:
                    for member in group["members"]:
                        for param in member["params"]:
                            if param["code"] not in (CODE_GENERIC, CODE_REFERENCE):
                                continue

                            if param["type"] == NO_REF:
                                continue

                            named = types[param["type"]]

                            if named["flags"] & TYPE_CONCRETE:
                                continue

                            if not named["flags"] & TYPE_PARAMETERIZED:
                                continue

                            unusable.append(
                                f"{type_record['name']}.{member['winrt_name']}"
                                f" -> {named['namespace']}.{named['name']}"
                            )

        self.assertEqual(unusable, [])


class TestTableProtocolRoles(unittest.TestCase):
    """The members the Python protocols of a type call."""

    tables = (
        ("test_winrt", "testcomponent"),
        ("winrt", "windows", "foundation"),
        ("winrt", "windows", "foundation", "collections"),
        ("winrt", "windows", "data", "json"),
    )

    def test_a_type_has_the_members_its_protocols_call(self) -> None:
        # The type flags say which protocols a type implements and the member
        # roles say which members those protocols call, so a type that claims
        # one and does not name its members would have a slot with nothing to
        # call - which is what a missing entry in the generator's table of
        # roles would silently produce.
        checked = 0

        for parts in self.tables:
            table = read(*parts)

            for type_record in table["types"]:
                if type_record["flags"] & TYPE_EXTERNAL:
                    continue

                required: set[int] = set()

                for flag, needed in REQUIRED_ROLES.items():
                    if type_record["flags"] & flag:
                        required |= needed

                # An IIterator is iterable without a First, because iter() on
                # one hands back the iterator itself.
                iterable = type_record["flags"] & TYPE_ITERABLE
                iterator = type_record["flags"] & TYPE_ITERATOR

                if iterable and not iterator:
                    required.add(ROLE_FIRST)

                if not required:
                    continue

                checked += 1

                with self.subTest(type=type_record["name"]):
                    self.assertEqual(required - roles(type_record), set())

        self.assertGreater(checked, 0)

    def test_a_concrete_type_has_the_roles_of_the_type_it_instantiates(self) -> None:
        # A concrete instance carries its own members, substituted, so the
        # roles have to survive that rather than be found again on the type
        # arguments' side.
        checked = 0

        for parts in self.tables:
            table = read(*parts)

            for type_record in table["types"]:
                if not type_record["flags"] & TYPE_CONCRETE:
                    continue

                definition = table["types"][type_record["base"]]

                if definition["flags"] & TYPE_EXTERNAL:
                    continue

                checked += 1

                with self.subTest(signature=type_record["signature"]):
                    self.assertEqual(roles(type_record), roles(definition))

        self.assertGreater(checked, 0)


class TestTableEnums(unittest.TestCase):
    """
    The constants of an enum, which are the whole of what one is.

    An enum is the one projected type with no interface and no members, so the
    table is the only place its constants are written down and the class the
    runtime builds is the only place they end up. The stub says them a second
    time, for a type checker, and nothing executes that.
    """

    def test_an_enum_carries_its_constants_in_the_order_it_declares_them(
        self,
    ) -> None:
        status = defined(read("winrt", "windows", "foundation"))["AsyncStatus"]

        self.assertFalse(status["flags"] & TYPE_FLAGS_ENUM)
        self.assertEqual(
            [(c["py_name"], c["value"]) for c in status["constants"]],
            [("CANCELED", 2), ("COMPLETED", 1), ("ERROR", 3), ("STARTED", 0)],
        )

    def test_a_flags_enum_says_so(self) -> None:
        attributes = defined(read("winrt", "windows", "storage"))["FileAttributes"]

        self.assertTrue(attributes["flags"] & TYPE_FLAGS_ENUM)
        self.assertEqual(
            dict((c["py_name"], c["value"]) for c in attributes["constants"])[
                "DIRECTORY"
            ],
            0x10,
        )

    def test_a_negative_constant_is_kept_as_its_bits(self) -> None:
        # A plain enum is a signed int32 and a flags enum an unsigned uint32,
        # so a record holds the thirty-two bits and the type says how to read
        # them.
        priority = defined(read("winrt", "windows", "system"))[
            "DispatcherQueuePriority"
        ]
        constants = dict((c["py_name"], c["value"]) for c in priority["constants"])

        self.assertFalse(priority["flags"] & TYPE_FLAGS_ENUM)
        self.assertEqual(constants["LOW"], (-10) & 0xFFFFFFFF)

    def test_only_an_enum_has_constants(self) -> None:
        for parts in (
            ("winrt", "windows", "foundation"),
            ("winrt", "windows", "storage"),
            ("test_winrt", "testcomponent"),
        ):
            table = read(*parts)

            for type_record in table["types"]:
                if type_record["category"] & 0x7 == CATEGORY_ENUM:
                    continue

                with self.subTest(type=type_record["name"]):
                    self.assertEqual(type_record["constants"], [])

    def test_the_class_the_runtime_builds_is_what_the_table_says(self) -> None:
        checked = 0

        for parts, module_name in (
            (("winrt", "windows", "foundation"), "winrt.windows.foundation"),
            (("winrt", "windows", "storage"), "winrt.windows.storage"),
            (("winrt", "windows", "system"), "winrt.windows.system"),
        ):
            table = read(*parts)
            module = importlib.import_module(module_name)

            for type_record in table["types"]:
                if type_record["category"] & 0x7 != CATEGORY_ENUM:
                    continue

                if not type_record["flags"] & TYPE_PYTHON_TYPE:
                    continue

                checked += 1
                built = getattr(module, type_record["py_name"])
                is_flags = bool(type_record["flags"] & TYPE_FLAGS_ENUM)

                with self.subTest(type=type_record["name"]):
                    self.assertTrue(
                        issubclass(built, enum.IntFlag if is_flags else enum.IntEnum)
                    )
                    self.assertEqual(built.__module__, module_name)
                    self.assertEqual(
                        [(name, int(member)) for name, member in built.__members__.items()],
                        [
                            (
                                c["py_name"],
                                c["value"]
                                if is_flags
                                else c["value"] - ((c["value"] & 0x80000000) << 1),
                            )
                            for c in type_record["constants"]
                        ],
                    )

        self.assertGreater(checked, 0)


class TestTableGuids(unittest.TestCase):
    """The IIDs, which a wrong value does not fail to load."""

    def test_parameterized_instances_hash_to_their_iid(self) -> None:
        count = 0

        for parts in (
            ("test_winrt", "testcomponent"),
            ("winrt", "windows", "foundation"),
            ("winrt", "windows", "foundation", "collections"),
        ):
            table = read(*parts)

            for type_record in table["types"]:
                if not type_record["flags"] & TYPE_CONCRETE:
                    continue

                count += 1

                with self.subTest(signature=type_record["signature"]):
                    self.assertEqual(
                        guid_of(type_record),
                        pinterface_guid(type_record["signature"]),
                    )

        self.assertGreater(count, 0)

    def test_a_sample_against_the_published_values(self) -> None:
        found = set()

        for parts in (
            ("winrt", "windows", "foundation"),
            ("winrt", "windows", "foundation", "collections"),
            ("test_winrt", "testcomponent"),
        ):
            table = read(*parts)

            for type_record in table["types"]:
                name = type_record["name"]

                if type_record["flags"] & TYPE_CONCRETE:
                    name = (
                        f"{name}<"
                        + ", ".join(
                            table["types"][arg]["name"]
                            for arg in type_record["generic_args"]
                        )
                        + ">"
                    )

                key = (type_record["namespace"], name)
                expected = PUBLISHED_IIDS.get(key)

                if expected is None:
                    continue

                found.add(key)

                with self.subTest(type=key):
                    self.assertEqual(guid_of(type_record), uuid.UUID(expected))

        self.assertEqual(found, set(PUBLISHED_IIDS))

    def test_an_interface_has_an_iid_and_a_class_does_not(self) -> None:
        table = read("test_winrt", "testcomponent")

        for type_record in table["types"]:
            if type_record["flags"] & TYPE_EXTERNAL:
                continue

            with self.subTest(type=type_record["name"]):
                if type_record["category"] & 0x7 in (
                    CATEGORY_INTERFACE,
                    CATEGORY_DELEGATE,
                ):
                    self.assertIsNotNone(guid_of(type_record))
                elif type_record["category"] & 0x7 in (CATEGORY_CLASS, CATEGORY_STRUCT):
                    self.assertIsNone(guid_of(type_record))


if __name__ == "__main__":
    unittest.main()
