"""
The call shapes and struct layouts that a projection table implies, against
the census the generator assigned shapes from and Python's struct module.
"""

import json
import pathlib
import struct
import unittest
from typing import Any

from ._tables import (
    TYPE_EXTERNAL,
    NO_REF,
    read,
    defined,
)


# The census that every member's forward shape id was assigned from.
SHAPES_JSON = pathlib.Path(__file__).parents[2] / "runtime" / "src" / "shapes.json"

# How a struct field of each type code is spelled in a shape key. A field keeps
# its exact width, because it is laid out in memory rather than passed.
FIELD_CODES = {
    1: "b",
    2: "i1",
    3: "u1",
    4: "i2",
    5: "u2",
    6: "i4",
    7: "u4",
    8: "i8",
    9: "u8",
    10: "f4",
    11: "f8",
    12: "c2",
    13: "p",
    15: "i4",
    16: "i8",
    17: "p",
    18: "i8",
    19: "i8",
    20: "i4",
    21: "u4",
    23: "p",
    24: "p",
    25: "p",
    26: "p",
    27: "p",
}

GUID_FIELDS = "u4u2u2" + "u1" * 8

# The type codes of what the metadata declares as a struct of its own: the
# guid, the hresult, the event token, the datetime and the timespan.
STRUCT_CODES = {14: GUID_FIELDS, 15: "i4", 16: "i8", 18: "i8", 19: "i8"}

CODE_INT64 = 8
CODE_UINT64 = 9
CODE_SINGLE = 10
CODE_DOUBLE = 11
CODE_STRUCT = 22

PARAM_IN = 0
PARAM_OUT = 1


class ShapeKeys:
    """
    Rebuilds a member's ABI call shape key from the table records alone, as
    ``PyWinRT/AbiShapes.cs`` builds it from the metadata.

    Every output, every half of an array and every input passed by reference
    is a pointer; an input of 32 bits or less is widened to one, a 64-bit
    integer is ``i8`` whatever its sign, and a struct is its fields flattened.
    """

    def __init__(self) -> None:
        self.tables: dict[str, dict[str, Any]] = {}

    def table(self, namespace: str) -> dict[str, Any]:
        if namespace not in self.tables:
            package = "test_winrt" if namespace == "TestComponent" else "winrt"
            self.tables[namespace] = read(package, *namespace.lower().split("."))

        return self.tables[namespace]

    def fields(self, table: dict[str, Any], ref: int) -> str:
        record = table["types"][ref]

        # a struct from another namespace is laid out in that namespace's table
        if record["flags"] & TYPE_EXTERNAL:
            owner = self.table(record["namespace"])
            record = defined(owner)[record["name"]]
            table = owner

        return "".join(
            self.fields(table, field["type"])
            if field["code"] == CODE_STRUCT
            else STRUCT_CODES.get(field["code"]) or FIELD_CODES[field["code"]]
            for field in record["fields"]
        )

    def argument(self, table: dict[str, Any], param: dict[str, Any]) -> str:
        if param["category"] != PARAM_IN:
            # an output, or a count and a pointer for an array
            return "p" if param["category"] == PARAM_OUT else "pp"

        code = param["code"]

        if param["is_by_reference"]:
            return "p"

        if code == CODE_STRUCT:
            return "{" + self.fields(table, param["type"]) + "}"

        if code in STRUCT_CODES:
            return "{" + STRUCT_CODES[code] + "}"

        if code in (CODE_INT64, CODE_UINT64):
            return "i8"

        return {CODE_SINGLE: "f4", CODE_DOUBLE: "f8"}.get(code, "p")

    def key(self, table: dict[str, Any], member: dict[str, Any]) -> str:
        return "".join(self.argument(table, param) for param in member["params"])


class TestShapeKeys(unittest.TestCase):
    """
    A member the census has no shape for is still described in full, so that a
    later runtime can call it from its parameters alone (the rule in
    ``runtime/src/table-format.md``). This checks that the records really are
    enough: the key rebuilt from them names the shape the generator gave the
    member, for every member with one.
    """

    def test_the_records_rebuild_every_shape(self) -> None:
        census = json.loads(SHAPES_JSON.read_text())["shape_ids"]
        keys = ShapeKeys()
        checked = 0
        wrong: list[tuple[str, str, str]] = []

        for namespace in (
            "TestComponent",
            "Windows.Foundation",
            "Windows.Foundation.Collections",
        ):
            table = keys.table(namespace)

            for type_record in table["types"]:
                for group in type_record["groups"]:
                    for member in group["members"]:
                        if member["forward_shape"] == NO_REF:
                            continue

                        key = keys.key(table, member)
                        checked += 1

                        if census.get(key) != member["forward_shape"]:
                            wrong.append(
                                (type_record["name"], member["winrt_name"], key)
                            )

        self.assertEqual(wrong, [])
        self.assertGreater(checked, 0)


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

    def test_a_struct_of_another_namespace_is_not_measured_here(self) -> None:
        # The table holds only the name of Windows.Foundation.Point, so it
        # cannot say how big ManipulationDelta is. The runtime measures the
        # field from the Windows.Foundation entry instead (test_projection).
        delta = defined(read("winrt", "windows", "ui", "input"))["ManipulationDelta"]

        self.assertIsNone(delta["size"])
        self.assertIsNone(delta["align"])
        self.assertEqual(
            [f["offset"] for f in delta["fields"]], [None, None, None, None]
        )
