import timeit

COUNT = 100
REPEAT = 10

times = timeit.repeat(
    """
for k, v in m.items():
    pass
""",
    setup="""
from winrt.windows.foundation.collections import StringMap

m = StringMap()

m.update({f"key{i}": f"value{i}" for i in range(1000)})
""",
    number=COUNT,
    repeat=REPEAT,
)

print(f"{int(min(times) / COUNT * 1e9):_} ns per iteration average")
