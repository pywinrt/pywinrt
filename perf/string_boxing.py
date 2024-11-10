import timeit

COUNT = 100_000
REPEAT = 10

times = timeit.repeat(
    "unbox_string(box_string('test'))",
    setup="from winrt.system import box_string, unbox_string",
    number=COUNT,
    repeat=REPEAT,
)

print(f"{int(min(times) / COUNT * 1e9):_} ns per iteration average")
