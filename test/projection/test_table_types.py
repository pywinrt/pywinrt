"""
What a projection table says about its types: the parameterized instances,
the members the Python protocols call, the constants of an enum and the
IIDs.

The GUIDs get a check of their own. A wrong IID still loads: it makes
``QueryInterface`` ask for an interface that does not exist, or worse, for the
wrong one. Every parameterized instance is therefore hashed again here, by an
independent implementation of the WinRT algorithm, and a sample of the plain
interfaces is compared against the published values, which is what
``winrt::guid_of<T>()`` yields for them.
"""

import enum
import importlib
import unittest
import uuid
from typing import Any

from ._tables import (
    CATEGORY_ENUM,
    CATEGORY_STRUCT,
    CATEGORY_INTERFACE,
    CATEGORY_CLASS,
    CATEGORY_DELEGATE,
    TYPE_EXTERNAL,
    TYPE_PARAMETERIZED,
    TYPE_CONCRETE,
    TYPE_PYTHON_TYPE,
    TYPE_ITERABLE,
    TYPE_ITERATOR,
    TYPE_SEQUENCE,
    TYPE_MUTABLE_SEQUENCE,
    TYPE_MAPPING,
    TYPE_MUTABLE_MAPPING,
    TYPE_AWAITABLE,
    TYPE_CLOSEABLE,
    TYPE_STRINGABLE,
    TYPE_FLAGS_ENUM,
    NO_REF,
    read,
    defined,
)


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
        unusable: list[str] = []

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

    def test_a_concrete_type_requires_no_type_without_arguments(self) -> None:
        # What a type requires is what an instance of it can be queried for,
        # so for anything that values exist of it has to be a real interface
        # with an IID, closed over the type arguments along the whole chain:
        # JsonArray reaches IIterable<T> through IVector<IJsonValue>.
        unusable: list[str] = []

        for parts in (
            ("test_winrt", "testcomponent"),
            ("winrt", "windows", "foundation"),
            ("winrt", "windows", "foundation", "collections"),
            ("winrt", "windows", "data", "json"),
        ):
            types = read(*parts)["types"]

            for type_record in types:
                if (
                    type_record["flags"] & TYPE_PARAMETERIZED
                    and not type_record["flags"] & TYPE_CONCRETE
                ):
                    continue

                for index in type_record["interfaces"]:
                    named = types[index]

                    if named["flags"] & TYPE_CONCRETE:
                        continue

                    if not named["flags"] & TYPE_PARAMETERIZED:
                        continue

                    unusable.append(
                        f"{type_record['name']} -> {named['namespace']}.{named['name']}"
                    )

        self.assertEqual(unusable, [])

    def test_a_requirement_two_levels_down_is_closed(self) -> None:
        # IObservableMap<K, V> requires IMap<K, V>, which requires
        # IIterable<IKeyValuePair<K, V>>, so an instance of the first has to
        # name the instance of the last with its own type arguments.
        types = read("winrt", "windows", "foundation", "collections")["types"]
        instances = [
            t
            for t in types
            if t["name"] == "IObservableMap" and t["flags"] & TYPE_CONCRETE
        ]

        self.assertNotEqual(instances, [])

        for instance in instances:
            with self.subTest(signature=instance["signature"]):
                required = {
                    types[index]["name"]
                    for index in instance["interfaces"]
                    if types[index]["flags"] & TYPE_CONCRETE
                }

                self.assertEqual(required, {"IMap", "IIterable"})


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
                        [
                            (name, int(member))
                            for name, member in built.__members__.items()
                        ],
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
        found: set[tuple[str, str]] = set()

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
