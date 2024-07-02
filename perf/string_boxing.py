import timeit

COUNT = 100_000
REPEAT = 10

times = timeit.repeat(
    "unbox(box('test'))",
    setup="from winrt.windows.foundation.interop import box, unbox",
    number=COUNT,
    repeat=REPEAT,
)

print(f"{int(min(times) / COUNT * 1e9):_} ns per iteration average")
