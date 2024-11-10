import timeit

COUNT = 100_000
REPEAT = 10

times = timeit.repeat(
    "unbox_guid(box_guid(test))",
    setup="""
from uuid import uuid4
from winrt.system import box_guid, unbox_guid

test = uuid4()
""",
    number=COUNT,
    repeat=REPEAT,
)

print(f"{int(min(times) / COUNT * 1e9):_} ns per iteration average")
