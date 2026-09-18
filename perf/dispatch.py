r"""Per-call dispatch overhead of the projection.

The other scripts in ``perf/`` each measure one conversion.  This one measures
what it costs to call a projected member at all, which is the number that
decides whether a change to the shape of the generated code -- a different
optimizer setting, a different dispatch mechanism, caching the non-default
interface pointer -- is worth taking.  Run it before and after a change and
compare.

The cases call ``TestComponent`` wherever they can, because its members do
nothing but return, so what is left is the projection rather than the
implementation behind it.  Two of them have to come from the Windows SDK:
``DataWriter.WriteBuffer()`` because it is one generated function with a
one-argument and a three-argument branch, which is the only way to price an
argument without also changing what is called, and ``JsonValue`` because
``TestComponent`` has no static that returns a wrapped object.

The numbers only mean anything from a Release build -- the Debug build of the
same tree is several times slower and needs ``python_d.exe``::

    cmake -S projection -B _build/bench -G Ninja -DPYWINRT_FULL_PROJECTION=OFF -DCMAKE_BUILD_TYPE=Release
    cmake --build _build/bench -j 4
    cmake --install _build/bench --prefix _build/bench-install
    $env:PYTHONPATH = "_build/bench-install"
    py perf\dispatch.py

That is still not what ships: wheels are compiled with ``/O2 /GL`` and linked
with ``/LTCG``, which CMake Release does not do.  Only compare numbers taken
from the same kind of build.

Any command line arguments are substrings, and only the cases whose name
contains one of them are run.
"""

import sys
import timeit
from typing import NamedTuple

REPEAT = 10

# how many elements the IVector<String> cases work with
ITEMS = 1000

COMMON = """
import test_winrt.testcomponent as tc
import winrt.windows.data.json as wdj
import winrt.windows.storage.streams as wss
"""

TESTS = """
tests = tc.TestRunner.make_tests()
"""

# an empty buffer, so that both overloads of WriteBuffer() write nothing and
# the writer does not grow while it is being measured
WRITER = """
writer = wss.DataWriter()
buffer = wss.Buffer(0)
"""

VECTOR = """
vector = tc.TestRunner.create_string_vector()
"""

# a struct and the tuple of the same fields, for the two ways a struct
# argument can be given
STRUCTS = (
    TESTS
    + """
import uuid
fields = (1, 2, 3, 4, 5, 6, 7, 8.0, 9.0, uuid.UUID(int=0))
blittable = tc.Blittable(*fields)
"""
)

FULL_VECTOR = (
    VECTOR
    + f"""
for i in range({ITEMS}):
    vector.append(f"item{{i}}")
"""
)

# a callable that echoes what it is given, for the delegate case, and an event
# handler that does nothing
ECHO = (
    TESTS
    + """
echo = lambda a: (a, a)
"""
)

HANDLER = """
override = tc.Override()
handler = lambda sender, args: None
"""

# a Python list and a Python dict for the cases that hand one to WinRT, which
# wraps rather than copies it and then calls back into Python per element
PY_COLLECTIONS = (
    TESTS
    + f"""
items = [f"item{{i}}" for i in range({ITEMS})]
mapping = {{f"key{{i}}": f"value{{i}}" for i in range({ITEMS})}}
"""
)


class Case(NamedTuple):
    name: str
    """What the case is called in the output."""
    count: int
    """How many times the statement is run per timing."""
    scale: int
    """How many calls one run of the statement makes, for the per-call average."""
    setup: str
    """Statements run once per timing, after :data:`COMMON`."""
    stmt: str
    """The statement being timed."""


CASES = (
    # Simple() converts nothing and does nothing, so this is the floor for a
    # member of an interface the wrapper already holds.
    Case("instance method, 0 args", 200_000, 1, TESTS, "tests.simple()"),
    # The two WriteBuffer() overloads are branches of a single generated
    # function, so the difference between these two rows is exactly the two
    # extra convert_to<uint32_t>() calls.
    Case("instance method, 1 arg", 200_000, 1, WRITER, "writer.write_buffer(buffer)"),
    Case(
        "instance method, 3 args",
        200_000,
        1,
        WRITER,
        "writer.write_buffer(buffer, 0, 0)",
    ),
    Case("property get", 200_000, 1, TESTS, "tests.percentage"),
    # a static goes through the activation factory rather than through an
    # object the wrapper already holds, and unlike the rows above it also
    # builds a wrapper for what it returns
    Case("static method", 200_000, 1, "", "wdj.JsonValue.create_number_value(1.0)"),
    # the subscript slot, which normalises the index before calling GetAt();
    # append() and the iterator are projected members with nothing in front of
    # them
    Case("IVector<String> get", 200_000, 1, FULL_VECTOR, "vector[500]"),
    Case("IVector<String> append", 50_000, 1, VECTOR, 'vector.append("item")'),
    Case("IVector<String> iterate", 200, ITEMS, FULL_VECTOR, "for s in vector: pass"),
    # the other direction: a Python list and a Python dict passed to WinRT,
    # which wraps each of them and reads it back an element at a time. The
    # TestComponent method copies what it is given, so these price Size() plus
    # GetAt() per item and the mapping iterator per entry.
    Case(
        "list as IVector<String>",
        200,
        ITEMS,
        PY_COLLECTIONS,
        "tests.collection5(items)",
    ),
    Case(
        "dict as IMap<String, String>",
        200,
        ITEMS,
        PY_COLLECTIONS,
        "tests.collection3(mapping)",
    ),
    # A struct is handed to WinRT as a blit of the wrapper's fields rather
    # than converted one field at a time, so what a member taking one spends
    # its time on is finding the wrapper type: Param13() takes two structs and
    # returns two, and each direction asks for the type. Passing a tuple
    # instead is the one struct path that does convert per field, and it goes
    # to the type registry for the conversion function as well.
    Case(
        "two structs in, two out",
        100_000,
        1,
        STRUCTS,
        "tests.param13(blittable, blittable)",
    ),
    Case(
        "the same, passing a tuple",
        50_000,
        1,
        STRUCTS,
        "tests.param13(fields, blittable)",
    ),
    # the other direction: a Python callable handed to WinRT as a delegate.
    # Param7Call() takes one, invokes it once with an integer and checks what
    # comes back, so this prices building the delegate, one call from WinRT
    # into Python, and the conversions each way.
    Case("delegate invoked once", 100_000, 1, ECHO, "tests.param7_call(echo)"),
    # an event handler added and taken off again, which is the delegate
    # above plus what an event source does with one
    Case(
        "event add and remove",
        100_000,
        1,
        HANDLER,
        "override.remove_overridable_called(override.add_overridable_called(handler))",
    ),
    # get() on an operation that completed before the timing started, so this
    # is the wait machinery and the result conversion without any waiting
    Case(
        "completed get()",
        100_000,
        1,
        "op = tc.TestRunner.create_async_operation(0, 42)\nop.get()",
        "op.get()",
    ),
    # GetString() on a number raises E_ILLEGAL_METHOD_CALL, so this is the
    # HRESULT to Python exception path and nothing else
    Case(
        "exception path",
        20_000,
        1,
        "value = wdj.JsonValue.create_number_value(1.0)",
        "try:\n    value.get_string()\nexcept OSError:\n    pass",
    ),
)


def main(patterns: list[str]) -> None:
    cases = [c for c in CASES if not patterns or any(p in c.name for p in patterns)]

    if not cases:
        sys.exit(f"no case matches {patterns}")

    width = max(len(c.name) for c in cases)
    print(f"{'':{width}}  {'ns/op':>9}")

    for case in cases:
        times = timeit.repeat(
            case.stmt,
            setup=COMMON + case.setup,
            number=case.count,
            repeat=REPEAT,
        )
        per_call = min(times) / (case.count * case.scale)
        print(f"{case.name:{width}}  {int(per_call * 1e9):>9_}", flush=True)


if __name__ == "__main__":
    main(sys.argv[1:])
