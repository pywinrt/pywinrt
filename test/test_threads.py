"""
Many threads at once through one runtime.

On a free-threaded build nothing serializes the threads below, so this is where
a first-use cache or a lazily built type that two threads fill in at once shows
itself: as a crash, a value of the wrong type, or one that came from another
thread's call. Under the GIL the same code checks what the GIL's switches allow.
Each case but the composed object's starts its threads together at a barrier,
so that they reach the first use at the same moment.
"""

import ctypes
import subprocess
import sys
import sysconfig
import threading
import time
import unittest
import uuid
from collections.abc import Callable

import test_winrt.testcomponent as tc
import winrt._winrt as runtime
import winrt.windows.data.json as wdj
import winrt.windows.foundation as wf
from winrt.system import Array

THREADS = 8

FIELDS = (1, 2, 3, 4, 5, 6, 7, 8.0, 9.0, uuid.UUID(int=0))

IID_IINSPECTABLE = uuid.UUID("AF86E2E0-B12D-4C6A-9C5A-D7AA65101E90")
IID_IWEAKREFERENCESOURCE = uuid.UUID("00000038-0000-0000-C000-000000000046")


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


def method(abi: ctypes.c_void_p, slot: int, *argtypes: type) -> Callable[..., int]:
    """
    The COM method in vtable slot ``slot`` of the interface ``abi`` points at,
    as a function that ctypes calls with the GIL released.
    """
    vtable = ctypes.cast(abi, ctypes.POINTER(ctypes.POINTER(ctypes.c_void_p)))
    prototype = ctypes.WINFUNCTYPE(ctypes.c_long, ctypes.c_void_p, *argtypes)

    return prototype(vtable[0][slot])


class WeakReference:
    """
    A COM weak reference to a WinRT object, held and resolved the way WinRT
    holds and resolves one, on a thread that does not hold the GIL.
    """

    def __init__(self, obj: object) -> None:
        get_pointer = ctypes.pythonapi.PyCapsule_GetPointer
        get_pointer.restype = ctypes.c_void_p
        get_pointer.argtypes = [ctypes.py_object, ctypes.c_char_p]

        capsule = runtime.as_interface(obj, IID_IWEAKREFERENCESOURCE)  # type: ignore[call-overload]
        source = ctypes.c_void_p(get_pointer(capsule, b"winrt.interface"))

        self.abi = ctypes.c_void_p()
        get_weak_reference = method(source, 3, ctypes.c_void_p)
        if get_weak_reference(source, ctypes.byref(self.abi)) != 0:
            raise OSError("GetWeakReference failed")

        self.iid = (ctypes.c_ubyte * 16).from_buffer_copy(IID_IINSPECTABLE.bytes_le)
        self._resolve = method(self.abi, 3, ctypes.c_void_p, ctypes.c_void_p)
        self._release = method(self.abi, 2)

    def resolve(self) -> bool:
        """
        Resolves the reference, lets go of what it resolved to, and says
        whether there was anything.
        """
        strong = ctypes.c_void_p()
        if self._resolve(self.abi, ctypes.byref(self.iid), ctypes.byref(strong)) != 0:
            raise OSError("Resolve failed")

        if not strong:
            return False

        method(strong, 2)(strong)

        return True

    def release(self) -> None:
        self._release(self.abi)


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

    def test_one_array(self) -> None:
        # Every thread assigns elements that hold references - each one new,
        # so that the one it replaces is freed - while the others read, slice,
        # compare and print the same array. A string is one kind of array
        # storage, and a runtime class and a struct with a string field are
        # the other.
        def string(i: int, n: int) -> str:
            return f"{i}-{n}"

        def uri(i: int, n: int) -> wf.Uri:
            return wf.Uri(f"https://example.com/{i}/{n}")

        def struct(i: int, n: int) -> tc.NonBlittable:
            return tc.NonBlittable(True, "a", f"{i}-{n}", n)

        for element, make in ((str, string), (wf.Uri, uri), (tc.NonBlittable, struct)):
            with self.subTest(element=element):
                shared = Array(element, [make(0, n) for n in range(4)])

                def work(i: int) -> None:
                    for n in range(50):
                        shared[n % 4] = make(i, n)
                        self.assertIsInstance(shared[(n + 1) % 4], element)
                        self.assertEqual(len(shared[1:3]), 2)
                        # What these answer depends on what the other
                        # threads assigned meanwhile, so only that they answer
                        # is checked.
                        self.assertIsInstance(shared == shared, bool)
                        self.assertIsInstance(repr(shared), str)

                run_together(work)

    def test_lent_array_taken_back_while_read(self) -> None:
        # The array a WinRT caller lends a Python handler is taken back when
        # the call returns, whichever thread is reading it at that moment.
        tests = tc.TestRunner.make_tests()
        lent: list[Array[str]] = []
        stop = threading.Event()

        def read(i: int) -> None:
            while not stop.is_set():
                # Under the GIL, a reader that never lets go of it starves
                # the thread making the calls.
                time.sleep(0)

                if not lent:
                    continue

                array = lent[-1]

                try:
                    self.assertIsInstance(array[0], str)
                except IndexError:
                    pass

                self.assertLessEqual(len(array[:]), 3)

        def handler(
            passed: Array[str], filled: Array[str]
        ) -> tuple[Array[str], Array[str]]:
            lent.append(filled)

            for index, value in enumerate(passed):
                filled[index] = value

            return Array(str, list(passed)), Array(str, list(passed))

        readers = [
            threading.Thread(target=read, args=(i,)) for i in range(THREADS // 2)
        ]

        for t in readers:
            t.start()

        try:
            for _ in range(50):
                tests.array12_call(handler)
        finally:
            stop.set()

            for t in readers:
                t.join()

        self.assertEqual(len(lent), 50)
        self.assertEqual(len(lent[0]), 0)

    def test_first_use(self) -> None:
        # A process of its own, because this one has built most of it already.
        result = subprocess.run(
            [sys.executable, "-c", FIRST_USE_CODE.format(threads=THREADS)],
            capture_output=True,
            text=True,
        )

        self.assertEqual(result.returncode, 0, result.stderr)


class TestComposedObject(unittest.TestCase):
    def test_weak_reference_resolved_while_python_lets_go(self) -> None:
        # XAML holds its elements weakly and resolves them on its own thread,
        # while the last Python reference can go on any other. The GIL is given
        # up once while each instance is alive, so that a resolve can take it
        # and be the last to let go, and once more by an attribute's finalizer,
        # so that a resolve can come in while Python is taking it apart.
        class Yields:
            def __del__(self) -> None:
                time.sleep(0)

        class Composed(tc.Composable):
            yields: Yields

        composed = Composed()
        references = [WeakReference(composed)]
        self.assertTrue(references[0].resolve())
        del composed
        self.assertFalse(references[0].resolve())

        stop = threading.Event()

        def resolve() -> None:
            while not stop.is_set():
                references[-1].resolve()

        threads = [threading.Thread(target=resolve) for _ in range(THREADS // 2)]

        for t in threads:
            t.start()

        try:
            for _ in range(200):
                composed = Composed()
                composed.yields = Yields()
                references.append(WeakReference(composed))
                time.sleep(0)
                del composed
        finally:
            stop.set()

            for t in threads:
                t.join()

        try:
            for reference in references:
                self.assertFalse(reference.resolve())
        finally:
            for reference in references:
                reference.release()


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
