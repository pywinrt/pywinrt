"""
What Python keeps between WinRT callbacks that arrive on the same thread.

Callbacks enter Python through py::gil_guard (runtime/src/include/pywinrt/
handles.h). On a thread Python has not seen, attaching on its own creates a
thread state that the matching release deletes again, so everything Python
keeps per thread state - threading.local, the asyncio running loop, the
contextvars context - would last for one callback only. So the runtime keeps
the thread state until the thread exits (runtime/src/handles.cpp), and
deletes it then.

A DispatcherQueue on a dedicated thread runs every work item on one OS thread
that Python did not create: the case in question. A queue on a thread Python
did create, which pumps it with the GIL released as Application.start() and
DispatcherQueue.run_event_loop() do, is the control.
"""

import asyncio
import contextvars
import ctypes
import subprocess
import sys
import threading
import unittest
import weakref
from ctypes import wintypes

from winrt.windows.system import DispatcherQueue, DispatcherQueueController
from winrt.windows.system.interop import create_dispatcher_queue_controller

_var: contextvars.ContextVar[str] = contextvars.ContextVar("_var")

PM_REMOVE = 0x0001
QS_ALLINPUT = 0x04FF


class _Probe:
    """
    Sets per-thread state in one work item and reads it back in the next.
    """

    def __init__(self) -> None:
        self.local = threading.local()
        self.loop = asyncio.new_event_loop()
        self.seen: dict[str, object] = {}
        self.done = threading.Event()

    def close(self) -> None:
        self.loop.close()

    def enqueue(self, queue: DispatcherQueue) -> None:
        if not (queue.try_enqueue(self._first) and queue.try_enqueue(self._second)):
            raise RuntimeError("try_enqueue() failed")

    def _first(self) -> None:
        self.seen["first thread"] = threading.get_native_id()
        self.local.value = "set"
        asyncio.events._set_running_loop(self.loop)
        _var.set("set")

    def _second(self) -> None:
        try:
            self.seen["second thread"] = threading.get_native_id()
            self.seen["threading.local"] = getattr(self.local, "value", None)
            self.seen["running loop"] = asyncio.events._get_running_loop() is self.loop
            self.seen["contextvar"] = _var.get(None)
        finally:
            asyncio.events._set_running_loop(None)
            self.done.set()


def _pump_until(done: threading.Event) -> None:
    """
    A message loop that, like any ctypes call, releases the GIL while it
    waits and while it dispatches.
    """
    user32 = ctypes.WinDLL("user32")
    msg = wintypes.MSG()
    while not done.is_set():
        user32.MsgWaitForMultipleObjects(0, None, False, 50, QS_ALLINPUT)
        while user32.PeekMessageW(ctypes.byref(msg), None, 0, 0, PM_REMOVE):
            user32.TranslateMessage(ctypes.byref(msg))
            user32.DispatchMessageW(ctypes.byref(msg))


class TestCallbackThreadState(unittest.TestCase):
    def check(self, seen: dict[str, object]) -> None:
        self.assertEqual(
            seen["first thread"], seen["second thread"], "not the same thread"
        )
        self.assertEqual(
            {k: seen[k] for k in ("threading.local", "running loop", "contextvar")},
            {"threading.local": "set", "running loop": True, "contextvar": "set"},
        )

    def test_thread_python_created(self) -> None:
        probe = _Probe()
        self.addCleanup(probe.close)

        def thread() -> None:
            controller = create_dispatcher_queue_controller()
            try:
                probe.enqueue(controller.dispatcher_queue)
                _pump_until(probe.done)
            finally:
                controller.shutdown_queue_async()

        t = threading.Thread(target=thread)
        t.start()
        t.join(10)
        self.check(probe.seen)

    def test_thread_python_did_not_create(self) -> None:
        probe = _Probe()
        self.addCleanup(probe.close)
        controller = DispatcherQueueController.create_on_dedicated_thread()
        try:
            probe.enqueue(controller.dispatcher_queue)
            self.assertTrue(probe.done.wait(10), "the work items didn't run")
        finally:
            controller.shutdown_queue_async()
        self.check(probe.seen)

    def test_thread_state_is_deleted_when_the_thread_exits(self) -> None:
        class Kept:
            pass

        local = threading.local()
        freed = threading.Event()
        seen: list[bool] = []

        def first() -> None:
            local.value = Kept()
            weakref.finalize(local.value, freed.set)

        def second() -> None:
            seen.append(hasattr(local, "value"))

        controller = DispatcherQueueController.create_on_dedicated_thread()
        queue = controller.dispatcher_queue
        self.assertTrue(queue.try_enqueue(first) and queue.try_enqueue(second))
        # The queue runs what it has before it shuts down, and then its
        # thread exits.
        controller.shutdown_queue_async()

        # Kept from one callback to the next, and let go of with the thread.
        self.assertTrue(freed.wait(10), "the thread state outlived its thread")
        self.assertEqual(seen, [True])

    def test_exit_with_a_thread_state_kept(self) -> None:
        # The dedicated thread is still running, with its thread state kept,
        # when the interpreter finalizes.
        code = "\n".join(
            [
                "import threading",
                "from winrt.windows.system import DispatcherQueueController",
                "controller = DispatcherQueueController.create_on_dedicated_thread()",
                "ran = threading.Event()",
                "controller.dispatcher_queue.try_enqueue(ran.set)",
                "assert ran.wait(10)",
            ]
        )
        result = subprocess.run(
            [sys.executable, "-X", "dev", "-c", code],
            capture_output=True,
            text=True,
            timeout=30,
        )
        self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == "__main__":
    unittest.main()
