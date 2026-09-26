"""
Many threads at once through one runtime.

On a free-threaded build nothing serializes the threads below, so this is where
a first-use cache or a lazily built type that two threads fill in at once shows
itself: as a crash, a value of the wrong type, or one that came from another
thread's call. Under the GIL the same code checks what the GIL's switches allow.
Each case starts its threads together at a barrier, so that they reach the first
use at the same moment.
"""

import subprocess
import sys
import sysconfig
import threading
import unittest
import uuid
from collections.abc import Callable

import test_winrt.testcomponent as tc
import winrt.windows.data.json as wdj
import winrt.windows.foundation as wf

THREADS = 8

FIELDS = (1, 2, 3, 4, 5, 6, 7, 8.0, 9.0, uuid.UUID(int=0))


def run_together(work: Callable[[int], None], count: int = THREADS) -> None:
    """
    Runs ``work(i)`` on ``count`` threads that start together, and raises the
    first exception any of them raised.
    """
    barrier = threading.Barrier(count)
    errors: list[BaseException] = []

    def run(i: int) -> None:
        try:
            barrier.wait()
            work(i)
        except BaseException as e:
            errors.append(e)

    threads = [threading.Thread(target=run, args=(i,)) for i in range(count)]

    for t in threads:
        t.start()

    for t in threads:
        t.join()

    if errors:
        raise errors[0]


# What each thread of a fresh process does, where nothing has been built yet:
# every ParamNHandler delegate, the IVector<String> and IIterable<String>
# instances, the vtable a Python list stands in for an IIterable behind, and a
# type from a second package, all for the first time.
FIRST_USE_CODE = """
import threading
import uuid

import test_winrt.testcomponent as tc
import winrt.windows.data.json as wdj

tests = tc.TestRunner.make_tests()
fields = (1, 2, 3, 4, 5, 6, 7, 8.0, 9.0, uuid.UUID(int=0))
barrier = threading.Barrier({threads})
errors = []


def echo(*args):
    return args[0], args[-1]


def work(i):
    try:
        barrier.wait()
        for call in (
            tests.param1_call, tests.param2_call, tests.param3_call,
            tests.param4_call, tests.param5_call, tests.param6_call,
            tests.param7_call, tests.param8_call, tests.param9_call,
            tests.param10_call, tests.param13_call,
        ):
            call(echo)

        vector = tc.TestRunner.create_string_vector()
        vector.append(str(i))
        assert list(vector) == [str(i)], list(vector)

        ret, out = tests.collection1([str(i), "x"])
        assert list(ret) == [str(i), "x"], list(ret)
        assert list(out) == [str(i), "x"], list(out)

        first, _ = tests.param13(fields, tc.Blittable(*fields))
        assert type(first) is tc.Blittable, type(first)

        value = wdj.JsonValue.create_number_value(float(i))
        assert type(value) is wdj.JsonValue, type(value)
        assert value.get_number() == float(i)
    except BaseException as e:
        errors.append(e)


threads = [threading.Thread(target=work, args=(i,)) for i in range({threads})]
for t in threads:
    t.start()
for t in threads:
    t.join()

if errors:
    raise errors[0]
"""


class TestThreads(unittest.TestCase):
    def test_one_object(self) -> None:
        tests = tc.TestRunner.make_tests()
        blittable = tc.Blittable(*FIELDS)

        def work(i: int) -> None:
            for n in range(30):
                self.assertEqual(tests.param1(n % 2 == 0), (n % 2 == 0,) * 2)
                self.assertEqual(tests.param5(i * 1000 + n), (i * 1000 + n,) * 2)

                items = [f"{i}-{n}"]
                ret, out = tests.collection1(items)
                self.assertEqual(list(ret), items)
                self.assertEqual(list(out), items)

                first, second = tests.param13(FIELDS, blittable)
                self.assertIs(type(first), tc.Blittable)
                self.assertEqual(first, blittable)
                self.assertEqual(second, blittable)

        run_together(work)

    def test_one_delegate_type(self) -> None:
        tests = tc.TestRunner.make_tests()

        def work(i: int) -> None:
            seen = []

            def handler(value: str) -> tuple[str, str]:
                seen.append(value)
                return value, value

            for _ in range(50):
                tests.param11_call(handler)

            self.assertEqual(len(seen), 50)

        run_together(work)

    def test_one_async_operation(self) -> None:
        # One thread waits for the operation, which only one may; the rest
        # read it while it runs and complete operations of their own.
        shared = tc.TestRunner.create_async_operation(50, 7)
        done = threading.Event()

        def work(i: int) -> None:
            if i == 0:
                try:
                    self.assertEqual(shared.get(), 7)
                finally:
                    done.set()

                return

            while not done.is_set():
                self.assertIn(
                    shared.status, (wf.AsyncStatus.STARTED, wf.AsyncStatus.COMPLETED)
                )
                self.assertIsInstance(shared.id, int)

            own = tc.TestRunner.create_async_operation(1, i)
            self.assertEqual(own.get(), i)

        run_together(work)

        self.assertEqual(shared.status, wf.AsyncStatus.COMPLETED)

    def test_one_registry(self) -> None:
        # Every thread converts a value of a type the registry resolves by
        # name, and all of them must come back as the one type.
        def work(i: int) -> None:
            for n in range(100):
                value = wdj.JsonValue.create_string_value(f"{i}-{n}")
                self.assertIs(type(value), wdj.JsonValue)
                self.assertEqual(value.get_string(), f"{i}-{n}")

        run_together(work)

    def test_first_use(self) -> None:
        # A process of its own, because this one has built most of it already.
        result = subprocess.run(
            [sys.executable, "-c", FIRST_USE_CODE.format(threads=THREADS)],
            capture_output=True,
            text=True,
        )

        self.assertEqual(result.returncode, 0, result.stderr)


class TestFreeThreading(unittest.TestCase):
    def test_gil_stays_disabled(self) -> None:
        # A version check rather than a skip decorator, so that a type checker
        # running on an older Python does not look for sys._is_gil_enabled().
        if sys.version_info < (3, 13):
            self.skipTest("the GIL can only be disabled from 3.13")
        else:
            if not sysconfig.get_config_var("Py_GIL_DISABLED"):
                self.skipTest("not a free-threaded build")

            # Every module this suite imported has been loaded by now, so a
            # module that did not declare Py_mod_gil would have turned the GIL
            # back on.
            self.assertFalse(sys._is_gil_enabled())


if __name__ == "__main__":
    unittest.main()
