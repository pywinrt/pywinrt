"""
The projection table, written by the generator and read by the runtime.

A projection package ships one ``_table.pywinrt`` per WinRT namespace and no
compiled code, and the ends of that file are in three languages and three
processes: ``PyWinRT/TableWriter.cs`` writes the text a package carries,
``winrt.table`` compiles it when the package is built, and
``runtime/src/table.cpp`` reads the result. Nothing else can tell whether they
agree about where a field is, so this writes nothing and reads everything: the
tables that the build installed, through the runtime's own reader.
``test_table_text.py`` covers the first two on their own.
``test_table_shapes.py`` and ``test_table_types.py`` read the same tables for
the call shapes and struct layouts, and for what they say about types.
"""

import importlib
import importlib.util
import pathlib
import struct
import sys
import tempfile
import types
import unittest
import uuid
from typing import Any

import test_winrt.testcomponent as tc
import winrt._winrt
import winrt.runtime._internals
from winrt.table import build, parse

from ._tables import (
    CATEGORY_STRUCT,
    CATEGORY_INTERFACE,
    CATEGORY_CLASS,
    CATEGORY_DELEGATE,
    TYPE_EXTERNAL,
    TYPE_CONCRETE,
    TYPE_DEFAULT_ACTIVATABLE,
    TYPE_PYTHON_TYPE,
    NO_REF,
    find_table,
    read,
    defined,
)


GROUP_METHOD = 0
GROUP_PROPERTY = 1
GROUP_EVENT = 2
GROUP_CONSTRUCTOR = 3

GROUP_STATIC = 1 << 3

MEMBER_PROPERTY_GET = 1
MEMBER_CONSTRUCTOR = 5

MEMBER_OVERRIDABLE = 1 << 4
MEMBER_PROTECTED = 1 << 5

# The first vtable slot after IUnknown's three and IInspectable's three.
FIRST_INTERFACE_SLOT = 6


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


def groups(type_record: dict[str, Any]) -> dict[str, dict[str, Any]]:
    return {g["py_name"]: g for g in type_record["groups"]}


class TestTableFormat(unittest.TestCase):
    """The header and the validation that stands in front of everything else."""

    def test_header(self) -> None:
        table = read("test_winrt", "testcomponent")

        self.assertEqual(table["namespace"], "TestComponent")
        # the version of winrt-table-compiler that built this binary, which
        # the text it was built from does not carry
        self.assertRegex(table["compiler"], r"^\d+\.\d+\.\d+$")
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

    def test_a_buffer_winrt_fills_is_marked(self) -> None:
        # The metadata cannot say that ReadAsync() writes into its buffer while
        # WriteAsync() only reads its own; a rule in the generator does.
        streams = defined(read("winrt", "windows", "storage", "streams"))
        read_async = groups(streams["IInputStream"])["read_async"]["members"][0]
        write_async = groups(streams["IOutputStream"])["write_async"]["members"][0]

        self.assertEqual(
            [p["is_fill_buffer"] for p in read_async["params"]],
            [True, False, False, False],
        )
        self.assertEqual(
            [p["is_fill_buffer"] for p in write_async["params"]], [False, False]
        )

    def test_a_protected_or_overridable_property_has_an_underscore(self) -> None:
        # A templated control sets Control.DefaultStyleKey, which comes through
        # IControlProtected, and a class derived from GeneralTransform provides
        # InverseCore, which IGeneralTransformOverrides declares.
        control = defined(read("winrt", "windows", "ui", "xaml", "controls"))["Control"]
        media = defined(read("winrt", "windows", "ui", "xaml", "media"))
        default_style_key = groups(control)["_default_style_key"]
        inverse_core = groups(media["GeneralTransform"])["_inverse_core"]

        self.assertEqual(default_style_key["kind"], GROUP_PROPERTY)
        self.assertEqual(
            [m["winrt_name"] for m in default_style_key["members"]],
            ["get_DefaultStyleKey", "put_DefaultStyleKey"],
        )
        self.assertEqual(
            [
                m["flags"] & (MEMBER_PROTECTED | MEMBER_OVERRIDABLE)
                for m in default_style_key["members"]
            ],
            [MEMBER_PROTECTED, MEMBER_PROTECTED],
        )
        self.assertEqual(
            [
                m["flags"] & (MEMBER_PROTECTED | MEMBER_OVERRIDABLE)
                for m in inverse_core["members"]
            ],
            [MEMBER_OVERRIDABLE],
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


class MissingTable(unittest.TestCase):
    """
    What a package whose table did not travel with it says.

    A freezer that collects the module and leaves the data file beside it
    behind produces exactly this. The table is opened and mapped, so the
    operating system is what refuses it, and what the operating system says
    names neither the file nor the module - the import path has to.
    """

    def load(self, directory: str) -> ImportError:
        """
        Loads a projection out of a directory that has no table in it.
        """
        origin = pathlib.Path(directory) / "__init__.py"
        origin.write_bytes(b"")

        name = f"_not_a_projection_{pathlib.Path(directory).name}"
        spec = importlib.util.spec_from_file_location(name, origin)
        assert spec is not None
        sys.modules[name] = types.ModuleType(name)

        try:
            with self.assertRaises(ImportError) as caught:
                winrt.runtime._internals.load_projection(spec)
        finally:
            del sys.modules[name]

        return caught.exception

    def test_it_names_the_module_and_the_table_it_wanted(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            error = self.load(directory)
            table = pathlib.Path(directory) / "_table.pywinrt"

            self.assertEqual(
                error.name, f"_not_a_projection_{pathlib.Path(directory).name}"
            )
            self.assertEqual(error.path, str(table))
            self.assertIn(error.name, str(error))
            self.assertIn(str(table), str(error))

    def test_it_keeps_what_the_operating_system_said(self) -> None:
        # the cause is what says whether the file was missing, locked or
        # unreadable, which is the part that tells the two apart
        with tempfile.TemporaryDirectory() as directory:
            error = self.load(directory)

            self.assertIsInstance(error.__cause__, FileNotFoundError)


class FailedLoad(unittest.TestCase):
    """
    What importing a package again says after building its types failed.

    The table is registered before a single type is built, and a namespace's
    types are built only once, so the next import cannot try again. It has to
    say why rather than find the table registered and return a module with no
    types in it.
    """

    # A struct whose field is a struct of a package that does not exist, which
    # the table reads and the runtime cannot build.
    TABLE = """\
format 4.0
census {census}
namespace Test.FailedLoad

struct Test.FailedLoad.Outer python_type
    signature struct(Test.FailedLoad.Outer;struct(Test.Missing.Inner;i4))
    field inner Inner struct type=Test.Missing.Inner

struct Test.Missing.Inner external
    py winrt._no_such_package.Inner
"""

    def test_a_second_import_raises_with_the_first_error_as_its_cause(self) -> None:
        lineage, revision = read("winrt", "windows", "foundation")["census"]
        data = build(parse(self.TABLE.format(census=f"{lineage} {revision}")))

        # Left to the operating system to clean up, as refuse() says.
        with tempfile.TemporaryDirectory(ignore_cleanup_errors=True) as directory:
            path = pathlib.Path(directory) / "_table.pywinrt"
            path.write_bytes(data)
            name = f"_failed_projection_{path.parent.name}"

            errors: list[ImportError] = []

            for _ in range(2):
                with self.assertRaises(ImportError) as caught:
                    winrt._winrt.load_projection(types.ModuleType(name), str(path))

                errors.append(caught.exception)

        self.assertEqual(errors[0].name, "winrt._no_such_package")
        self.assertEqual(errors[1].name, name)
        self.assertIn("new process", str(errors[1]))
        self.assertIs(errors[1].__cause__, errors[0])


class UncallableCompletedSetter(unittest.TestCase):
    """
    What waiting says for an async object whose Completed setter this runtime
    cannot call.

    The handler is handed over without the GIL, where nothing can be raised,
    so the setter is checked before the wait begins.
    """

    # IAsyncAction under another name, with no trampoline for either half of
    # its Completed property.
    TABLE = """\
format 4.0
census {census}
namespace Test.Waitable

interface Test.Waitable.IWaitable python_type awaitable
    py _IWaitable
    guid 5a648006-843a-4da9-865b-9d26e5dfad7b
    signature {{5a648006-843a-4da9-865b-9d26e5dfad7b}}
    property completed
        get get_Completed slot=7 inputs=0 outputs=1 declaring=Test.Waitable.IWaitable role=completed
            out delegate type=Windows.Foundation.AsyncActionCompletedHandler return
        put put_Completed slot=6 inputs=1 outputs=0 declaring=Test.Waitable.IWaitable
            in delegate type=Windows.Foundation.AsyncActionCompletedHandler name=handler

delegate Windows.Foundation.AsyncActionCompletedHandler external
    py winrt.windows.foundation.AsyncActionCompletedHandler
    guid a4ed5c81-76c9-40bd-8be6-b1d90fb20ae7
"""

    def test_waiting_raises_instead_of_calling_it(self) -> None:
        lineage, revision = read("winrt", "windows", "foundation")["census"]
        data = build(parse(self.TABLE.format(census=f"{lineage} {revision}")))

        # Left to the operating system to clean up, as refuse() says.
        with tempfile.TemporaryDirectory(ignore_cleanup_errors=True) as directory:
            path = pathlib.Path(directory) / "_table.pywinrt"
            path.write_bytes(data)
            name = f"_waitable_{path.parent.name}"
            winrt._winrt.load_projection(types.ModuleType(name), str(path))

        action = tc.TestRunner.create_async_action(0)
        capsule = winrt._winrt.as_interface(
            action, uuid.UUID("5a648006-843a-4da9-865b-9d26e5dfad7b")
        )
        waitable = winrt._winrt.wrap_interface(capsule, f"{name}._IWaitable")

        with self.assertRaisesRegex(TypeError, "Completed"):
            waitable.get()


if __name__ == "__main__":
    unittest.main()
