"""
A Python callable seen from WinRT as a delegate.

``TestComponent`` has a ``ParamNCall`` for every parameter shape a delegate can
have and a handler type to go with it, so the round trip is exercised over the
whole set rather than over a representative one. Each ``ParamNCall`` invokes
the handler with values of its own and checks what comes back, so a handler
that echoes what it was given passes only if the call arrived intact and the
answer got home; the values are asserted here as well, because a conversion
that is wrong in both directions would echo just as happily.
"""

import gc
import struct
import threading
import time
import unittest
import uuid
import weakref
from typing import Any

import test_winrt.testcomponent as tc
import winrt.windows.foundation as wf
import winrt.windows.foundation.collections as wfc
from winrt.system.hresult import PYWINRT_E_UNRAISABLE_PYTHON_EXCEPTION

from ._util import catch_unraisable

BLITTABLE = tc.Blittable(
    1, 2, 3, 4, -5, -6, -7, 8.0, 9.0, uuid.UUID("bb28bca1-a46d-5897-a3e9-f9ec16930875")
)


class TestDelegateShapes(unittest.TestCase):
    """
    One test per ``ParamNHandler``, which between them cover every argument
    class a call from WinRT into Python has: the integers that travel in a
    register, the two floating point widths, a character, a string, and three
    structs passed by value.
    """

    def setUp(self) -> None:
        self.tests = tc.TestRunner.make_tests()

    def check(self, call: Any, *expected: Any) -> None:
        """
        Hands ``call`` a handler that echoes what it is given and asserts that
        it was given ``expected``.
        """
        seen = []

        def handler(*args: Any) -> Any:
            seen.append(args)
            # The return value comes first and the output parameter follows,
            # and every ParamN hands back the value it was given twice.
            return args[0], args[-1]

        call(handler)

        self.assertEqual(seen, [expected])

    def test_bool(self) -> None:
        self.check(self.tests.param1_call, True)

    def test_uint8(self) -> None:
        self.check(self.tests.param2_call, 1)

    def test_uint16(self) -> None:
        self.check(self.tests.param3_call, 2)

    def test_uint32(self) -> None:
        self.check(self.tests.param4_call, 3)

    def test_uint64(self) -> None:
        self.check(self.tests.param5_call, 4)

    def test_int16(self) -> None:
        self.check(self.tests.param6_call, 5)

    def test_int32(self) -> None:
        self.check(self.tests.param7_call, 6)

    def test_int64(self) -> None:
        self.check(self.tests.param8_call, 7)

    def test_single(self) -> None:
        # A float and a double travel in different registers and differ in
        # what they can hold, so the value that comes back from the narrower
        # one is 8.1 rounded to it.
        self.check(self.tests.param9_call, struct.unpack("f", struct.pack("f", 8.1))[0])

    def test_double(self) -> None:
        self.check(self.tests.param10_call, 9.1)

    def test_char16(self) -> None:
        self.check(self.tests.param11_call, "W")

    def test_string(self) -> None:
        self.check(self.tests.param12_call, "WinRT")

    def test_blittable_struct(self) -> None:
        self.check(self.tests.param13_call, BLITTABLE, BLITTABLE)

    def test_non_blittable_struct(self) -> None:
        value = tc.NonBlittable(False, "X", "WinRT", 1234)

        self.check(self.tests.param14_call, value, value)

    def test_nested_struct(self) -> None:
        value = tc.Nested(BLITTABLE, tc.NonBlittable(True, "X", "WinRT", 1234))

        self.check(self.tests.param15_call, value, value)

    def test_interface_argument(self) -> None:
        # TestHandler takes the ITests the component hands it and returns
        # nothing, so this is the one shape with an interface pointer in and
        # no output at all.
        seen = []

        tc.TestRunner.test_consumer(lambda callee: seen.append(callee))

        self.assertEqual(len(seen), 1)
        self.assertIsInstance(seen[0], tc._ITests)


class TestDelegate(unittest.TestCase):
    def test_no_arguments(self) -> None:
        # A Deferral calls its handler when it completes, which is a delegate
        # that takes nothing and returns nothing.
        called = threading.Event()

        deferral = wf.Deferral(called.set)
        self.assertFalse(called.is_set())

        deferral.complete()
        self.assertTrue(called.is_set())

    def test_exception_is_unraisable(self) -> None:
        # WinRT called the handler, so there is no Python caller to raise to:
        # the exception goes to sys.unraisablehook and WinRT is told that one
        # happened.
        tests = tc.TestRunner.make_tests()

        def handler(value: int) -> None:
            raise RuntimeError("test")

        with (
            self.assertRaisesRegex(OSError, "Unraisable Python exception") as ctx,
            catch_unraisable() as exceptions,
        ):
            # the handler's signature is wrong on purpose
            tests.param7_call(handler)  # type: ignore[arg-type]

        self.assertEqual(ctx.exception.winerror, PYWINRT_E_UNRAISABLE_PYTHON_EXCEPTION)
        self.assertIsInstance(exceptions[0].exc_value, RuntimeError)

    def test_wrong_number_of_return_values(self) -> None:
        tests = tc.TestRunner.make_tests()

        with (
            self.assertRaises(OSError),
            catch_unraisable() as exceptions,
        ):
            tests.param7_call(lambda value: value)  # type: ignore[arg-type,return-value]

        self.assertIsInstance(exceptions[0].exc_value, TypeError)

    def test_not_callable(self) -> None:
        with self.assertRaisesRegex(TypeError, "takes a callable"):
            wf.Deferral(42)  # type: ignore

    def test_callable_is_released(self) -> None:
        # The delegate holds a reference to the callable for as long as WinRT
        # holds the delegate, and gives it back afterwards.
        class Handler:
            def __call__(self) -> None:
                pass

        handler = Handler()
        ref = weakref.ref(handler)

        deferral = wf.Deferral(handler)
        del handler

        gc.collect()
        self.assertIsNotNone(ref(), "the delegate should still hold the callable")

        del deferral
        gc.collect()
        self.assertIsNone(ref())


class TestEvent(unittest.TestCase):
    def test_add_and_remove(self) -> None:
        calls = []

        obj = tc.Override()
        token = obj.add_overridable_called(lambda sender, args: calls.append(sender))

        obj.call_overridable()
        self.assertEqual(len(calls), 1)

        obj.remove_overridable_called(token)

        obj.call_overridable()
        self.assertEqual(len(calls), 1, "the handler was not removed")

    def test_parameterized_handler(self) -> None:
        # MapChangedEventHandler<K, V> is an instance of a parameterized
        # delegate, which is still a callable and not an interface to unwrap.
        seen = []

        m = wfc.StringMap()
        token = m.add_map_changed(lambda sender, args: seen.append(args.key))
        m.insert("hello", "world")
        m.remove_map_changed(token)

        self.assertEqual(seen, ["hello"])

    def test_handler_is_agile(self) -> None:
        """
        An upper bound on what one raise costs, measured against raising the
        same event with no handler attached.

        A delegate that does not answer ``IAgileObject`` still works, so the
        only thing that catches one is what it costs: C++/WinRT's event source
        takes an ``IAgileReference`` to it instead and resolves that on every
        raise, which the dispatch spike measured at 186 microseconds against
        319 nanoseconds. Resolving one is cheap when the raise is on the
        thread that added the handler, which is what this does, so the bound
        is loose: it is here to catch a delegate that has to be marshalled,
        not to tell an agile one from a free-threaded one.
        """
        count = 2000
        obj = tc.Override()

        def per_raise() -> float:
            start = time.perf_counter()

            for _ in range(count):
                obj.call_overridable()

            return (time.perf_counter() - start) / count

        unhandled = per_raise()

        token = obj.add_overridable_called(lambda sender, args: None)

        try:
            handled = per_raise()
        finally:
            obj.remove_overridable_called(token)

        self.assertLess(
            handled - unhandled,
            50e-6,
            "the handler is not agile, so every raise resolves an agile reference",
        )

    def test_handler_on_a_winrt_thread(self) -> None:
        # An async operation completes on a thread of WinRT's own, so this is
        # a delegate reached without a Python caller anywhere below it: the
        # invoke has to take the GIL rather than find it held.
        done = threading.Event()
        seen: dict[str, Any] = {}

        def handler(sender: Any, status: Any) -> None:
            seen["thread"] = threading.get_ident()
            seen["status"] = status
            seen["result"] = sender.get_results()
            done.set()

        operation = tc.TestRunner.create_async_operation(10, 42)
        operation.completed = handler

        self.assertTrue(done.wait(5), "the completion handler was never called")
        self.assertNotEqual(seen["thread"], threading.get_ident())
        self.assertEqual(seen["status"], wf.AsyncStatus.COMPLETED)
        self.assertEqual(seen["result"], 42)


if __name__ == "__main__":
    unittest.main()
