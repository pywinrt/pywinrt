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

import pathlib
import struct
import sys
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
TYPE_CONCRETE = 1 << 7
TYPE_DEFAULT_ACTIVATABLE = 1 << 8
TYPE_PYTHON_TYPE = 1 << 9

GROUP_METHOD = 0
GROUP_PROPERTY = 1
GROUP_EVENT = 2
GROUP_CONSTRUCTOR = 3

GROUP_STATIC = 1 << 3

MEMBER_PROPERTY_GET = 1
MEMBER_CONSTRUCTOR = 5

NO_REF = 0xFFFFFFFF

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


def defined(table: dict[str, Any]) -> dict[str, dict[str, Any]]:
    """The types the namespace defines, by WinRT name."""
    return {
        t["name"]: t
        for t in table["types"]
        if not t["flags"] & TYPE_EXTERNAL and not t["flags"] & TYPE_CONCRETE
    }


def groups(type_record: dict[str, Any]) -> dict[str, dict[str, Any]]:
    return {g["py_name"]: g for g in type_record["groups"]}


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
