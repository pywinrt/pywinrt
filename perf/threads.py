r"""How the throughput of small WinRT calls scales with the number of threads.

Each thread parses the same small JSON document with ``JsonObject.parse()`` and
reads a few values back, which is about ten WinRT calls that each take a
microsecond or so.  The ``python`` case does comparable work in pure Python, so
it shows what the interpreter itself allows: on a build with the GIL, about the
same throughput however many threads share it.

What to look for is the ``winrt`` case falling below the ``python`` one as
threads are added.  Every time a call lets go of the GIL another thread waiting
for it is woken, and when the call takes less time than that hand-over, the
threads spend their time passing the GIL around rather than calling WinRT.

Run it from a Release build, installed as ``dispatch.py`` says::

    $env:PYTHONPATH = "_build/bench-install"
    py perf\threads.py

Arguments pick the cases (``winrt``, ``python``) and the thread counts. One
thread is timed whatever the counts, since every rate is shown against it::

    py perf\threads.py winrt 1 8

On the free-threaded build, ``-X gil=0`` and ``-X gil=1`` compare the two.
"""

import sys
import threading
import time
from collections.abc import Callable

# how long each thread count is timed for, in seconds
DURATION = 2.0

DOC = '{"name": "pywinrt", "version": 4.01, "tags": ["winrt", "python", "npu"]}'


def winrt_work() -> Callable[[], None]:
    import winrt.windows.data.json as wdj

    def work() -> None:
        obj = wdj.JsonObject.parse(DOC)
        obj.get_named_number("version")
        obj.get_named_string("name")
        tags = obj.get_named_array("tags")
        tags.size
        tags.get_string_at(0)
        tags.get_at(1).get_string()
        obj.stringify()

    return work


def python_work() -> Callable[[], None]:
    import json

    def work() -> None:
        obj = json.loads(DOC)
        obj["version"]
        obj["name"]
        tags = obj["tags"]
        len(tags)
        tags[0]
        str(tags[1])
        json.dumps(obj)

    return work


CASES = {"winrt": winrt_work, "python": python_work}


def run(work: Callable[[], None], threads: int) -> float:
    """Iterations per second of @p work shared by @p threads threads."""
    stop = threading.Event()
    start = threading.Barrier(threads + 1)
    counts = [0] * threads

    def loop(i: int) -> None:
        n = 0
        start.wait()
        while not stop.is_set():
            for _ in range(100):
                work()
            n += 100
        counts[i] = n

    workers = [threading.Thread(target=loop, args=(i,)) for i in range(threads)]

    for w in workers:
        w.start()

    start.wait()
    began = time.perf_counter()
    time.sleep(DURATION)
    stop.set()

    for w in workers:
        w.join()

    return sum(counts) / (time.perf_counter() - began)


def main(args: list[str]) -> None:
    names = [a for a in args if a in CASES] or list(CASES)
    counts = [int(a) for a in args if a.isdigit()] or [2, 4, 8]
    counts = sorted({1, *counts})

    gil = getattr(sys, "_is_gil_enabled", lambda: True)()
    print(f"Python {sys.version.split()[0]}, GIL {'enabled' if gil else 'disabled'}")

    for name in names:
        work = CASES[name]()
        work()  # imports and first-use costs stay out of the timing

        base = None
        for threads in counts:
            rate = run(work, threads)
            base = base or rate
            print(
                f"{name:6} {threads:2} threads  {rate:10,.0f} it/s  {rate / base:5.2f}x",
                flush=True,
            )


if __name__ == "__main__":
    main(sys.argv[1:])
