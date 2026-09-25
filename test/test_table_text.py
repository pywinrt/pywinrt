"""
The text form of a projection table, and the compiler that turns it into one.

A projection package carries its table as text, because a generated binary is a
diff nobody can read, and the runtime reads a binary, because that is what it
can map and index without parsing. So the seam between the two is what this
covers: everything the text can say has to survive being compiled, and a text
that says something wrong has to be refused where it is written rather than
where it is read.

The compiler ships in ``winrt-runtime`` (``winrt/table.py``), since that is
where the other end of the format already lives and where a build can reach it.
Nothing here touches the file system: a table is compiled in memory and handed
straight to the runtime's own reader.
"""

import unittest

import winrt._winrt
from winrt.table import TableTextError, build, parse
from winrt.table.version import VERSION

# A table that uses every kind of record, since the point is that nothing the
# text can say is lost on the way to the binary.
TABLE = """\
format 4.0
census 6cba0b28-2f9e-4d2a-9f4e-0f0b6a2e4d11 7
namespace Test.Sample

enum Test.Sample.Grade python_type
    py Grade
    signature enum(Test.Sample.Grade;i4)
    constant NONE 0
    constant LOW -10
    constant HIGH 20

enum Test.Sample.Facet python_type flags_enum
    py Facet
    signature enum(Test.Sample.Facet;u4)
    constant FRONT 0x1
    constant BACK 0x80000000

struct Test.Sample.Extent python_type
    signature struct(Test.Sample.Extent;i4;f8)
    field width Width int32
    field weight Weight double

interface Test.Sample.IThing python_type stringable
    py _IThing
    guid 30d5a829-7fa4-4026-83bb-d75bae4ea99e
    signature {30d5a829-7fa4-4026-83bb-d75bae4ea99e}
    requires Windows.Foundation.IClosable
    property size
        get get_Size slot=6 inputs=0 outputs=1 declaring=Test.Sample.IThing shape=1 role=size
            out uint32 return
    method measure
        method Measure slot=7 inputs=2 outputs=1 declaring=Test.Sample.IThing shape=1
            in struct type=Test.Sample.Extent name=extent by_reference
            in string name=label
            out class type=Test.Sample.Thing name=result return
    method get_many
        method GetMany slot=10 inputs=2 outputs=1 declaring=Test.Sample.IThing shape=1 role=get_many
            in uint32 name=start
            fill_array string name=items
            out uint32 return
    method relabel
        method Relabel slot=11 inputs=1 outputs=1 declaring=Test.Sample.IThing shape=1
            pass_array string name=labels
            receive_array string return
    method clear
        method Clear slot=12 inputs=0 outputs=0 declaring=Test.Sample.IThing shape=1 role=clear
    event changed
        add add_Changed slot=8 inputs=1 outputs=1 declaring=Test.Sample.IThing shape=1
            in delegate type=Test.Sample.Changed name=handler
            out event_token return
        remove remove_Changed slot=9 inputs=1 outputs=0 declaring=Test.Sample.IThing shape=1
            in event_token name=token

class Test.Sample.Thing python_type activatable
    py Thing
    signature rc(Test.Sample.Thing;{30d5a829-7fa4-4026-83bb-d75bae4ea99e})
    default Test.Sample.IThing
    requires Test.Sample.IThing
    factory Test.Sample.IThingFactory
    constructor -
        ctor .ctor slot=6 inputs=1 outputs=1 declaring=Test.Sample.IThingFactory shape=1
            in uint32 name=size
            out class type=Test.Sample.Thing return

delegate Test.Sample.Changed python_type
    py Changed
    guid 96369f54-8eb6-48f0-abce-c1b211e627c3
    method -
        method Invoke slot=3 inputs=1 outputs=0 shape=1 reverse=0
            in interface type=Test.Sample.IThing name=sender

interface Test.Sample.IThingFactory
    guid 00000036-0000-0000-c000-000000000046

interface Windows.Foundation.IClosable external closeable
    py winrt.windows.foundation._IClosable
    guid 30d5a829-7fa4-4026-83bb-d75bae4ea99e

interface Windows.Foundation.Collections.IVector<Test.Sample.Thing> external concrete
    py winrt.windows.foundation.collections._IVector[Thing]
    guid e2fcc7c1-3bfc-5a0b-b2b0-72e769d1cb7e
    argument Test.Sample.Thing
"""

CATEGORY_ENUM = 0
CATEGORY_STRUCT = 1
CATEGORY_INTERFACE = 2
CATEGORY_CLASS = 3

MEMBER_ROLE_SIZE = 1
MEMBER_ROLE_GET_MANY = 20
MEMBER_ROLE_CLEAR = 25

CATEGORY_IN = 0
CATEGORY_OUT = 1
CATEGORY_PASS_ARRAY = 2
CATEGORY_FILL_ARRAY = 3
CATEGORY_RECEIVE_ARRAY = 4

TYPE_EXTERNAL = 1 << 3
TYPE_CONCRETE = 1 << 7
TYPE_PYTHON_TYPE = 1 << 9
TYPE_FLAGS_ENUM = 1 << 22

CODE_UINT32 = 7
CODE_DOUBLE = 11
CODE_STRING = 13
CODE_STRUCT = 22

NO_REF = 0xFFFFFFFF


def read(text: str = TABLE) -> dict:
    return winrt._winrt.read_table(build(parse(text)))


def types(table: dict) -> dict[str, dict]:
    return {f"{t['namespace']}.{t['name']}": t for t in table["types"]}


class TestTextTable(unittest.TestCase):
    """What the compiler makes of a table that says everything it can."""

    def test_the_header_says_what_the_text_said(self) -> None:
        table = read()

        self.assertEqual(table["format"], (4, 0))
        self.assertEqual(table["namespace"], "Test.Sample")
        self.assertEqual(table["census"], ("6cba0b28-2f9e-4d2a-9f4e-0f0b6a2e4d11", 7))

    def test_the_binary_says_which_compiler_wrote_it(self) -> None:
        """
        The text says nothing about a version, because a committed file cannot
        know which build compiled it. The compiler stamps its own, which is the
        version of the artifact a bug report would be about.
        """
        self.assertEqual(read()["compiler"], VERSION)

    def test_the_types_keep_the_order_they_are_written_in(self) -> None:
        table = read()

        self.assertEqual(
            [f"{t['namespace']}.{t['name']}" for t in table["types"]],
            [
                "Test.Sample.Grade",
                "Test.Sample.Facet",
                "Test.Sample.Extent",
                "Test.Sample.IThing",
                "Test.Sample.Thing",
                "Test.Sample.Changed",
                "Test.Sample.IThingFactory",
                "Windows.Foundation.IClosable",
                "Windows.Foundation.Collections.IVector",
            ],
        )

    def test_a_reference_becomes_the_index_of_what_it_names(self) -> None:
        table = read()
        index = {f"{t['namespace']}.{t['name']}": i for i, t in enumerate(table["types"])}
        thing = table["types"][index["Test.Sample.Thing"]]

        self.assertEqual(thing["default_interface"], index["Test.Sample.IThing"])
        self.assertEqual(thing["interfaces"], [index["Test.Sample.IThing"]])
        self.assertEqual(thing["factories"], [index["Test.Sample.IThingFactory"]])
        self.assertEqual(
            types(table)["Test.Sample.IThing"]["interfaces"],
            [index["Windows.Foundation.IClosable"]],
        )

    def test_a_member_the_census_could_not_name_has_no_shape(self) -> None:
        """
        A member may carry no shape at all, which is what the generator
        writes for a call it found no trampoline for and for a member of a
        parameterized definition, whose ABI is not known until an instance
        fills the type arguments in. Either way the member is still
        described; only the call is missing.
        """
        table = read(TABLE.replace(" shape=1 role=size", " role=size"))
        groups = {g["py_name"]: g for g in types(table)["Test.Sample.IThing"]["groups"]}

        size = groups["size"]["members"][0]

        self.assertEqual(size["forward_shape"], NO_REF)
        self.assertEqual(size["winrt_name"], "get_Size")
        self.assertEqual(size["slot"], 6)

    def test_a_type_that_names_nothing_has_no_reference(self) -> None:
        extent = types(read())["Test.Sample.Extent"]

        self.assertEqual(extent["base"], NO_REF)
        self.assertEqual(extent["default_interface"], NO_REF)
        self.assertIsNone(extent["guid"])
        self.assertEqual(extent["interfaces"], [])

    def test_the_flags_and_the_category_share_a_word(self) -> None:
        table = types(read())

        self.assertEqual(table["Test.Sample.Extent"]["category"], CATEGORY_STRUCT)
        self.assertEqual(table["Test.Sample.Thing"]["category"], CATEGORY_CLASS)
        self.assertEqual(
            table["Windows.Foundation.IClosable"]["category"], CATEGORY_INTERFACE
        )
        self.assertTrue(table["Test.Sample.Thing"]["flags"] & TYPE_PYTHON_TYPE)
        self.assertTrue(table["Windows.Foundation.IClosable"]["flags"] & TYPE_EXTERNAL)
        self.assertFalse(table["Test.Sample.Thing"]["flags"] & TYPE_EXTERNAL)

    def test_a_struct_is_laid_out_by_the_reader(self) -> None:
        extent = types(read())["Test.Sample.Extent"]

        self.assertEqual(
            [(f["py_name"], f["winrt_name"], f["code"], f["offset"]) for f in extent["fields"]],
            [("width", "Width", 6, 0), ("weight", "Weight", CODE_DOUBLE, 8)],
        )
        self.assertEqual(extent["size"], 16)
        self.assertEqual(extent["align"], 8)

    def test_an_enum_keeps_its_constants_in_the_order_they_are_written(self) -> None:
        grade = types(read())["Test.Sample.Grade"]

        self.assertEqual(grade["category"], CATEGORY_ENUM)
        self.assertFalse(grade["flags"] & TYPE_FLAGS_ENUM)
        self.assertEqual(
            [(c["py_name"], c["value"]) for c in grade["constants"]],
            [("NONE", 0), ("LOW", (-10) & 0xFFFFFFFF), ("HIGH", 20)],
        )

    def test_a_flags_enum_is_written_in_hexadecimal_and_is_unsigned(self) -> None:
        facet = types(read())["Test.Sample.Facet"]

        self.assertTrue(facet["flags"] & TYPE_FLAGS_ENUM)
        self.assertEqual(
            [(c["py_name"], c["value"]) for c in facet["constants"]],
            [("FRONT", 1), ("BACK", 0x80000000)],
        )

    def test_a_type_that_declares_no_constant_has_none(self) -> None:
        self.assertEqual(types(read())["Test.Sample.Extent"]["constants"], [])

    def test_a_member_keeps_its_slot_its_shape_and_its_role(self) -> None:
        thing = types(read())["Test.Sample.IThing"]
        groups = {g["py_name"]: g for g in thing["groups"]}
        size = groups["size"]["members"][0]

        self.assertEqual(size["winrt_name"], "get_Size")
        self.assertEqual(size["slot"], 6)
        self.assertEqual(size["forward_shape"], 1)
        self.assertEqual(size["reverse_shape"], NO_REF)
        self.assertEqual(size["role"], MEMBER_ROLE_SIZE)
        self.assertEqual(size["in_count"], 0)
        self.assertEqual(size["out_count"], 1)

    def test_a_member_keeps_a_role_that_only_a_python_collection_plays(self) -> None:
        # The roles above twenty are the ones nothing on the Python side of a
        # projected collection calls, so a table that lost them would still
        # read - and a Python list handed to WinRT as an IVector<T> would have
        # a vtable slot with nothing behind it.
        groups = {g["py_name"]: g for g in types(read())["Test.Sample.IThing"]["groups"]}

        self.assertEqual(groups["clear"]["members"][0]["role"], MEMBER_ROLE_CLEAR)

    def test_a_parameter_keeps_what_it_is_and_how_it_travels(self) -> None:
        table = read()
        index = {f"{t['namespace']}.{t['name']}": i for i, t in enumerate(table["types"])}
        thing = table["types"][index["Test.Sample.IThing"]]
        groups = {g["py_name"]: g for g in thing["groups"]}
        params = groups["measure"]["members"][0]["params"]

        self.assertEqual([p["code"] for p in params], [CODE_STRUCT, CODE_STRING, 24])
        self.assertEqual([p["name"] for p in params], ["extent", "label", "result"])
        self.assertEqual(params[0]["type"], index["Test.Sample.Extent"])
        self.assertTrue(params[0]["is_by_reference"])
        self.assertFalse(params[1]["is_by_reference"])
        self.assertTrue(params[2]["is_return_value"])
        self.assertEqual(params[1]["type"], NO_REF)

    def test_an_array_parameter_keeps_its_category(self) -> None:
        # An array is two ABI arguments and one parameter record, and which of
        # the three array categories it is decides who owns the elements, so it
        # is the one thing about a parameter that the code does not say.
        groups = {g["py_name"]: g for g in types(read())["Test.Sample.IThing"]["groups"]}

        many = groups["get_many"]["members"][0]

        self.assertEqual(many["role"], MEMBER_ROLE_GET_MANY)
        self.assertEqual(
            [p["category"] for p in many["params"]],
            [CATEGORY_IN, CATEGORY_FILL_ARRAY, CATEGORY_OUT],
        )

        relabel = groups["relabel"]["members"][0]

        self.assertEqual(
            [p["category"] for p in relabel["params"]],
            [CATEGORY_PASS_ARRAY, CATEGORY_RECEIVE_ARRAY],
        )
        self.assertEqual([p["code"] for p in relabel["params"]], [CODE_STRING] * 2)
        self.assertTrue(relabel["params"][1]["is_return_value"])

    def test_a_constructor_group_is_bound_to_no_name(self) -> None:
        thing = types(read())["Test.Sample.Thing"]

        self.assertEqual([g["py_name"] for g in thing["groups"]], [""])
        self.assertEqual(thing["groups"][0]["members"][0]["winrt_name"], ".ctor")

    def test_an_instance_carries_its_arguments(self) -> None:
        table = read()
        index = {f"{t['namespace']}.{t['name']}": i for i, t in enumerate(table["types"])}
        vector = table["types"][index["Windows.Foundation.Collections.IVector"]]

        self.assertTrue(vector["flags"] & TYPE_CONCRETE)
        self.assertEqual(vector["generic_args"], [index["Test.Sample.Thing"]])
        self.assertEqual(vector["py_name"], "winrt.windows.foundation.collections._IVector[Thing]")

    def test_the_shape_limits_are_one_past_the_highest_used(self) -> None:
        table = read()

        self.assertEqual(table["forward_shape_limit"], 2)
        self.assertEqual(table["reverse_shape_limit"], 1)


class TestTextErrors(unittest.TestCase):
    """A text that says something wrong is refused, saying where and why."""

    def compile_with(self, *lines: str) -> str:
        text = TABLE + "\n" + "\n".join(lines) + "\n"

        with self.assertRaises(TableTextError) as caught:
            build(parse(text, "table.txt"))

        return str(caught.exception)

    def test_an_unknown_flag(self) -> None:
        self.assertIn("'sortable' is not a type flag", self.compile_with("class A sortable"))

    def test_an_unknown_keyword(self) -> None:
        message = self.compile_with("class Test.Sample.Other", "    colour blue")

        self.assertIn("'colour' says nothing about a type", message)

    def test_an_unknown_value_token(self) -> None:
        message = self.compile_with(
            "class Test.Sample.Other",
            "    method go",
            "        method Go slot=6 inputs=0 outputs=0 vtable=3",
        )

        self.assertIn("'vtable=3' says nothing about a member", message)

    def test_a_constant_that_is_not_a_number(self) -> None:
        message = self.compile_with("enum Test.Sample.Other", "    constant HIGH up")

        self.assertIn("'up' is not a constant", message)

    def test_a_constant_that_does_not_fit(self) -> None:
        message = self.compile_with(
            "enum Test.Sample.Other", "    constant HIGH 4294967296"
        )

        self.assertIn("does not fit in an enum", message)

    def test_an_unknown_type_code(self) -> None:
        message = self.compile_with(
            "class Test.Sample.Other",
            "    method go",
            "        method Go slot=6 inputs=0 outputs=0",
            "            in decimal",
        )

        self.assertIn("'decimal' is not a type code", message)

    def test_a_reference_to_a_type_that_is_not_there(self) -> None:
        message = self.compile_with("class Test.Sample.Other", "    base Test.Sample.Missing")

        self.assertIn("'Test.Sample.Missing' is named but never written", message)

    def test_a_type_that_is_written_twice(self) -> None:
        self.assertIn("'Test.Sample.Thing' is written twice", self.compile_with("class Test.Sample.Thing"))

    def test_a_line_that_is_indented_by_something_else(self) -> None:
        message = self.compile_with("class Test.Sample.Other", "  py Other")

        self.assertIn("indented by 2 spaces", message)

    def test_a_message_says_which_line_it_is_about(self) -> None:
        self.assertIn("table.txt:", self.compile_with("class A sortable"))

    def test_a_format_of_another_generation(self) -> None:
        with self.assertRaises(TableTextError) as caught:
            parse(TABLE.replace("format 4.0", "format 5.0"))

        self.assertIn("this writes format 4.0", str(caught.exception))

    def test_a_format_that_says_more_than_this_can_write(self) -> None:
        with self.assertRaises(TableTextError) as caught:
            parse(TABLE.replace("format 4.0", "format 4.9"))

        self.assertIn("says more than", str(caught.exception))

    def test_a_table_that_does_not_say_which_census_it_used(self) -> None:
        with self.assertRaises(TableTextError) as caught:
            parse(TABLE.replace("census 6cba0b28-2f9e-4d2a-9f4e-0f0b6a2e4d11 7\n", ""))

        self.assertIn("does not say what its census is", str(caught.exception))

    def test_a_census_that_is_not_a_lineage_and_a_revision(self) -> None:
        with self.assertRaises(TableTextError) as caught:
            parse(TABLE.replace(" 7\nnamespace", "\nnamespace"))

        self.assertIn("is not a census lineage and revision", str(caught.exception))


if __name__ == "__main__":
    unittest.main()
