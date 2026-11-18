import asyncio
import contextlib
import ctypes
import gc
import sys
import threading
import traceback
import unittest
from typing import Any, Generic, TypedDict, TypeVar

import test_winrt.testcomponent as tc
from winrt.system.hresult import (
    E_FAIL,
    E_ILLEGAL_DELEGATE_ASSIGNMENT,
    WIN32_ERROR_CANCELLED,
)
import winrt.windows.foundation as wf

from ._util import async_test, catch_unraisable


class TestAsyncAwait(unittest.TestCase):
    @async_test
    async def test_async_action(self) -> None:
        op = tc.TestRunner.create_async_action(10)
        await op

        self.assertEqual(op.status, wf.AsyncStatus.COMPLETED)

    @async_test
    async def test_async_action_await_twice(self) -> None:
        op = tc.TestRunner.create_async_action(10)

        await op
        await op

        self.assertEqual(op.status, wf.AsyncStatus.COMPLETED)

    @async_test
    async def test_async_action_cancel(self) -> None:
        source = tc.AsyncActionSource()
        op = source.operation

        async def wait_with_timeout() -> None:
            async with asyncio.timeout(0):
                await op

        task = asyncio.create_task(wait_with_timeout())

        try:
            await self._run_callbacks()

            # The timeout asked the WinRT action to cancel.
            self.assertEqual(source.cancel_request_count, 1)
        finally:
            source.cancel()

        with self.assertRaises(asyncio.TimeoutError):
            await task

        self.assertEqual(op.status, wf.AsyncStatus.CANCELED)

    @async_test
    async def test_async_action_cancel_with_shield(self) -> None:
        errors = self._record_loop_errors()
        source = tc.AsyncActionSource()
        op = source.operation

        try:
            with self.assertRaises(asyncio.TimeoutError):
                async with asyncio.timeout(0):
                    await asyncio.shield(op)

            await self._run_callbacks()

            # The shield kept the cancellation from reaching the operation,
            # which is still running.
            self.assertEqual(source.cancel_request_count, 0)
            self.assertEqual(op.status, wf.AsyncStatus.STARTED)
        finally:
            # Only the source finishes the operation, which the shielded task
            # is still waiting for.
            source.complete()

        await self._run_callbacks()

        self.assertEqual(op.status, wf.AsyncStatus.COMPLETED)
        self.assertEqual(errors, [])

    @async_test
    async def test_async_action_cancel_with_cancel(self) -> None:
        errors = self._record_loop_errors()
        source = tc.AsyncActionSource()
        op = source.operation

        async def wait_with_timeout() -> None:
            async with asyncio.timeout(0):
                try:
                    await op
                except asyncio.CancelledError:
                    # what an app that cancels the operation itself does
                    op.cancel()
                    raise

        task = asyncio.create_task(wait_with_timeout())

        try:
            await self._run_callbacks()

            # The timeout asked the operation to cancel, and the await goes on
            # until it has.
            self.assertEqual(source.cancel_request_count, 1)
            self.assertFalse(task.done())
        finally:
            source.cancel()

        with self.assertRaises(TimeoutError):
            await task

        # Cancelling an operation that has finished changes nothing.
        self.assertEqual(op.status, wf.AsyncStatus.CANCELED)
        self.assertEqual(errors, [])

    @async_test
    async def test_async_action_error(self) -> None:
        op = tc.TestRunner.create_async_action_with_error(10, E_FAIL)

        with self.assertRaises(OSError) as ctx:
            await op

        self.assertEqual(ctx.exception.winerror, E_FAIL)

    @async_test
    async def test_async_action_with_progress(self) -> None:
        expected = [1, 2, 3]
        op = tc.TestRunner.create_async_action_with_progress(10, expected)

        actual: list[int] = []

        def on_progress(op: wf.IAsyncActionWithProgress[int], value: int) -> None:
            actual.append(value)

        op.progress = on_progress

        await op

        self.assertEqual(op.status, wf.AsyncStatus.COMPLETED)
        self.assertListEqual(actual, expected)

    @unittest.skipIf(sys.version_info < (3, 13), "requires Python 3.13 or later")
    @async_test
    async def test_async_action_with_progress_iter(self) -> None:
        expected = [1, 2, 3]
        op = tc.TestRunner.create_async_action_with_progress(10, expected)

        actual: list[int] = []

        async for value in WinrtAiter(op):
            actual.append(value)

        self.assertEqual(op.status, wf.AsyncStatus.COMPLETED)
        self.assertListEqual(actual, expected)

    @unittest.skipIf(sys.version_info < (3, 13), "requires Python 3.13 or later")
    @async_test
    async def test_async_action_with_progress_iter_cancel(self) -> None:
        expected = [1, 2, 3]
        # a report every 100 ms, the first of which the cancel comes before
        op = tc.TestRunner.create_async_action_with_progress(100, expected)
        asyncio.get_running_loop().call_later(0.02, op.cancel)

        actual: list[int] = []

        with self.assertRaises(asyncio.CancelledError):
            async for value in WinrtAiter(op):
                actual.append(value)

        self.assertEqual(op.status, wf.AsyncStatus.CANCELED)
        self.assertLess(len(actual), len(expected))

    @unittest.skipIf(sys.version_info < (3, 13), "requires Python 3.13 or later")
    @async_test
    async def test_async_action_with_progress_iter_cancel2(self) -> None:
        expected = [1, 2, 3, 4, 5]
        op = tc.TestRunner.create_async_action_with_progress(100, expected)

        actual: list[int] = []

        with self.assertRaises(asyncio.TimeoutError):
            async with (
                asyncio.timeout(0.2),
                # Have to do this otherwise the async op isn't canceled on timeout!
                contextlib.aclosing(WinrtAiter(op)) as aiter,
            ):
                async for value in aiter:
                    actual.append(value)

        self.assertEqual(op.status, wf.AsyncStatus.CANCELED)
        self.assertLess(len(actual), len(expected))

    @unittest.skipIf(sys.version_info < (3, 13), "requires Python 3.13 or later")
    @async_test
    async def test_async_action_with_progress_iter_error(self) -> None:
        expected = [1, 2, 3]
        op = tc.TestRunner.create_async_action_with_progress_with_error(
            10, expected, E_FAIL
        )

        actual: list[int] = []

        with self.assertRaises(OSError) as ctx:
            async for value in WinrtAiter(op):
                actual.append(value)

        self.assertEqual(ctx.exception.winerror, E_FAIL)
        self.assertEqual(op.status, wf.AsyncStatus.ERROR)
        self.assertListEqual(actual, expected)

    @async_test
    async def test_async_operation(self) -> None:
        op = tc.TestRunner.create_async_operation(10, 1)
        result = await op

        self.assertEqual(op.status, wf.AsyncStatus.COMPLETED)
        self.assertEqual(result, 1)

    @async_test
    async def test_async_operation_with_progress(self) -> None:
        expected = [1, 2, 3]
        expected_result = 4
        op = tc.TestRunner.create_async_operation_with_progress(
            10, expected, expected_result
        )

        actual: list[int] = []

        def on_progress(
            op: wf.IAsyncOperationWithProgress[int, int], value: int
        ) -> None:
            actual.append(value)

        op.progress = on_progress

        actual_result = await op

        self.assertEqual(op.status, wf.AsyncStatus.COMPLETED)
        self.assertEqual(actual_result, expected_result)
        self.assertListEqual(actual, expected)

    @async_test
    async def test_async_operation_is_not_a_future(self) -> None:
        op = tc.TestRunner.create_async_operation(10, 1)

        # Where asyncio needs a future, it makes a task around the operation.
        self.assertFalse(asyncio.isfuture(op))
        task = asyncio.ensure_future(op)
        self.assertIsInstance(task, asyncio.Task)
        self.assertEqual(await task, 1)

    @async_test
    async def test_async_operation_gather(self) -> None:
        op1 = tc.TestRunner.create_async_operation(10, 1)
        op2 = tc.TestRunner.create_async_operation(20, 2)

        self.assertEqual(await asyncio.gather(op1, op2), [1, 2])

    def _record_loop_errors(self) -> list[dict[str, Any]]:
        """
        Collects what the running loop is asked to report, which is where an
        error in a done callback goes.
        """
        errors: list[dict[str, Any]] = []

        def handler(loop: asyncio.AbstractEventLoop, context: dict[str, Any]) -> None:
            errors.append(context)

        asyncio.get_running_loop().set_exception_handler(handler)

        return errors

    async def _run_callbacks(self) -> None:
        """
        Runs the loop for long enough that a completion delivered from the
        handler, or a cancellation, reaches the tasks it is for and the tasks
        that wait for those.
        """
        for _ in range(10):
            await asyncio.sleep(0)

    @async_test
    async def test_async_operation_gather_timeout(self) -> None:
        errors = self._record_loop_errors()
        sources = [tc.AsyncOperationSource() for _ in range(2)]
        ops = [source.operation for source in sources]

        timeouts: list[asyncio.Timeout] = []

        async def gather() -> list[int]:
            async with asyncio.timeout(None) as timeout:
                timeouts.append(timeout)
                return await asyncio.gather(*ops)

        task = asyncio.create_task(gather())

        # The timeout cancels the gathering future, which cancels the task
        # around each operation, which asks WinRT to cancel it, and gather
        # finishes once the operations have. A task cancelled before it has
        # started never awaits its operation, so the timeout expires once
        # they are being awaited.
        try:
            await self._run_callbacks()
            self.assertTrue(all(source.has_completed_handler for source in sources))
            timeouts[0].reschedule(asyncio.get_running_loop().time())
            await self._run_callbacks()
            self.assertEqual([s.cancel_request_count for s in sources], [1, 1])
        finally:
            # Only the sources finish the operations, and the task does not
            # end until they have, so a failed assertion has to finish them.
            for source in sources:
                source.cancel()

        await self._run_callbacks()

        self.assertTrue(task.done())

        with self.assertRaises(TimeoutError):
            await task

        self.assertEqual([op.status for op in ops], [wf.AsyncStatus.CANCELED] * 2)
        self.assertEqual(errors, [])

    @async_test
    async def test_async_operation_gather_cancelled(self) -> None:
        errors = self._record_loop_errors()
        source = tc.AsyncOperationSource()
        op = source.operation

        # An operation cancelled through cancel() rather than through its task
        # has failed as far as asyncio is concerned.
        op.cancel()
        source.cancel()

        with self.assertRaises(OSError) as ctx:
            await asyncio.gather(op)

        self.assertEqual(ctx.exception.winerror, WIN32_ERROR_CANCELLED)
        self.assertEqual(errors, [])

    @async_test
    async def test_async_operation_gather_return_exceptions(self) -> None:
        errors = self._record_loop_errors()
        sources = [tc.AsyncOperationSource() for _ in range(2)]
        op1, op2 = (source.operation for source in sources)

        op1.cancel()
        sources[0].cancel()
        sources[1].complete(2)

        results = await asyncio.gather(op1, op2, return_exceptions=True)

        self.assertIsInstance(results[0], OSError)
        assert isinstance(results[0], OSError)
        self.assertEqual(results[0].winerror, WIN32_ERROR_CANCELLED)
        self.assertEqual(results[1], 2)
        self.assertEqual(errors, [])

    @async_test
    async def test_async_operation_shield_timeout(self) -> None:
        errors = self._record_loop_errors()
        source = tc.AsyncOperationSource()
        op = source.operation

        with self.assertRaises(TimeoutError):
            async with asyncio.timeout(0):
                await asyncio.shield(op)

        # The shield keeps the operation from being cancelled.
        self.assertEqual(source.cancel_request_count, 0)

        source.fail(E_FAIL)
        await self._run_callbacks()
        gc.collect()

        # Nothing is left to retrieve the error, so the loop reports it: since
        # 3.14 the shield does, and before that the task around the operation
        # does when it is freed.
        self.assertEqual(op.status, wf.AsyncStatus.ERROR)
        self.assertEqual(len(errors), 1)
        self.assertIsInstance(errors[0]["exception"], OSError)
        self.assertEqual(errors[0]["exception"].winerror, E_FAIL)

    @async_test
    async def test_async_operation_asyncio_wait(self) -> None:
        op1 = tc.TestRunner.create_async_operation(10, 1)
        op2 = tc.TestRunner.create_async_operation(20, 2)

        # A task around each operation is what matches the answers of
        # asyncio.wait() up with the operations.
        tasks = [asyncio.ensure_future(op) for op in (op1, op2)]
        done, pending = await asyncio.wait(tasks)

        self.assertEqual(done, set(tasks))
        self.assertEqual(pending, set())
        self.assertEqual({task.result() for task in done}, {1, 2})

    @async_test
    async def test_async_operation_falsy_result(self) -> None:
        op = tc.TestRunner.create_async_operation(10, 0)

        self.assertEqual(await op, 0)

    @async_test
    async def test_async_operation_await_twice(self) -> None:
        op = tc.TestRunner.create_async_operation(10, 1)

        self.assertEqual(await op, 1)
        self.assertEqual(await op, 1)

    @async_test
    async def test_async_operation_error_state(self) -> None:
        op = tc.TestRunner.create_async_operation_with_error(10, 1, E_FAIL)

        with self.assertRaises(OSError) as ctx:
            await op

        self.assertEqual(ctx.exception.winerror, E_FAIL)
        self.assertEqual(op.status, wf.AsyncStatus.ERROR)

    @async_test
    async def test_async_operation_cancel_is_winrts(self) -> None:
        source = tc.AsyncOperationSource()
        op = source.operation

        # cancel() is IAsyncInfo.Cancel(), a request that WinRT answers by
        # finishing the operation, which an await then raises for.
        op.cancel()
        self.assertEqual(source.cancel_request_count, 1)
        source.cancel()

        with self.assertRaises(OSError) as ctx:
            await op

        self.assertEqual(ctx.exception.winerror, WIN32_ERROR_CANCELLED)
        self.assertEqual(op.status, wf.AsyncStatus.CANCELED)

    @async_test
    async def test_async_operation_cancelled_by_the_operation(self) -> None:
        source = tc.AsyncOperationSource()
        op = source.operation

        # An operation that cancels itself, as Windows does when a device goes
        # away, has failed rather than been cancelled by Python.
        source.cancel()

        with self.assertRaises(OSError) as ctx:
            await op

        self.assertEqual(ctx.exception.winerror, WIN32_ERROR_CANCELLED)
        self.assertEqual(source.cancel_request_count, 0)

    async def _await_recording_cancellation(
        self, op: wf.IAsyncAction, seen: list[bool]
    ) -> None:
        try:
            await op
        except asyncio.CancelledError:
            seen.append(op.status != wf.AsyncStatus.STARTED)
            raise

    async def _assert_task_waits_for_the_operation(
        self,
        source: tc.AsyncActionSource,
        op: wf.IAsyncAction,
        task: asyncio.Task[None],
    ) -> None:
        # A task that was resumed by its cancellation alone would be done
        # within a couple of loop iterations.
        for _ in range(5):
            await asyncio.sleep(0)

        self.assertEqual(source.cancel_request_count, 1)
        self.assertEqual(op.status, wf.AsyncStatus.STARTED)
        self.assertFalse(task.done())

    @async_test
    async def test_async_operation_cancel_waits_for_the_operation(self) -> None:
        source = tc.AsyncActionSource()
        op = source.operation
        seen: list[bool] = []

        async def wait_with_timeout() -> None:
            async with asyncio.timeout(0):
                await self._await_recording_cancellation(op, seen)

        task = asyncio.create_task(wait_with_timeout())

        try:
            await self._assert_task_waits_for_the_operation(source, op, task)
            self.assertEqual(seen, [])
        finally:
            # Only the source finishes the operation, and the task does not
            # end until it has, so a failed assertion has to finish it too.
            source.cancel()

        with self.assertRaises(TimeoutError):
            await task

        # The task was resumed by the operation finishing, not before.
        self.assertEqual(seen, [True])
        self.assertEqual(op.status, wf.AsyncStatus.CANCELED)

    @async_test
    async def test_async_operation_task_cancel_waits_for_the_operation(self) -> None:
        for cancels in (1, 2):
            with self.subTest(cancels=cancels):
                source = tc.AsyncActionSource()
                op = source.operation
                seen: list[bool] = []

                task = asyncio.create_task(self._await_recording_cancellation(op, seen))
                await asyncio.sleep(0)

                for _ in range(cancels):
                    task.cancel()

                # Cancelling the task twice before it runs is one
                # cancellation, which asks the operation once.
                try:
                    await self._assert_task_waits_for_the_operation(source, op, task)
                    self.assertEqual(seen, [])
                finally:
                    source.cancel()

                with self.assertRaises(asyncio.CancelledError):
                    await task

                self.assertEqual(seen, [True])

    @async_test
    async def test_async_operation_second_cancel_gives_up_waiting(self) -> None:
        source = tc.AsyncActionSource()
        op = source.operation
        seen: list[bool] = []

        task = asyncio.create_task(self._await_recording_cancellation(op, seen))
        await asyncio.sleep(0)
        task.cancel()

        try:
            await self._assert_task_waits_for_the_operation(source, op, task)

            # An operation that never answers the request would keep the task
            # for good, so cancelling the task again gives up waiting for it
            # and leaves it running.
            task.cancel()
            await self._run_callbacks()

            self.assertTrue(task.cancelled())
            self.assertEqual(seen, [False])
            self.assertEqual(op.status, wf.AsyncStatus.STARTED)
            self.assertEqual(source.cancel_request_count, 1)
        finally:
            source.cancel()

    @async_test
    async def test_async_operation_abandoned_failure_is_reported(self) -> None:
        for finish in ("fail", "complete", "cancel"):
            with self.subTest(finish=finish):
                errors = self._record_loop_errors()
                source = tc.AsyncActionSource()
                op = source.operation

                task = asyncio.ensure_future(op)
                await asyncio.sleep(0)
                task.cancel()
                await self._run_callbacks()
                task.cancel()
                await self._run_callbacks()

                self.assertTrue(task.cancelled())

                # Nothing waits for the operation any more, so an error goes to
                # the loop's exception handler, and any other end is what the
                # cancellation asked for.
                if finish == "fail":
                    source.fail(E_FAIL)
                elif finish == "complete":
                    source.complete()
                else:
                    source.cancel()

                await self._run_callbacks()

                if finish == "fail":
                    self.assertEqual(len(errors), 1)
                    self.assertIn("gave up waiting", errors[0]["message"])
                    self.assertIsInstance(errors[0]["exception"], OSError)
                    self.assertEqual(errors[0]["exception"].winerror, E_FAIL)
                else:
                    self.assertEqual(errors, [])

    def test_async_operation_abandoned_failure_after_its_loop_closed(self) -> None:
        source = tc.AsyncActionSource()
        op = source.operation
        loop = asyncio.new_event_loop()
        errors: list[dict[str, Any]] = []

        def handler(loop: asyncio.AbstractEventLoop, context: dict[str, Any]) -> None:
            errors.append(context)

        loop.set_exception_handler(handler)

        async def wait() -> None:
            await op

        def run_callbacks() -> None:
            for _ in range(5):
                loop.run_until_complete(asyncio.sleep(0))

        task = loop.create_task(wait())
        run_callbacks()
        task.cancel()
        run_callbacks()
        task.cancel()
        run_callbacks()

        self.assertTrue(task.cancelled())

        loop.close()

        # There is no loop to hand the error to, and the closed loop's
        # exception handler is asked to report it all the same.
        source.fail(E_FAIL)

        self.assertEqual(len(errors), 1)
        self.assertIsInstance(errors[0]["exception"], OSError)
        self.assertEqual(errors[0]["exception"].winerror, E_FAIL)

    @async_test
    async def test_async_operation_cancel_ignored(self) -> None:
        source = tc.AsyncOperationSource()
        op = source.operation

        task = asyncio.ensure_future(op)
        await asyncio.sleep(0)
        task.cancel()

        try:
            await self._run_callbacks()
            self.assertEqual(source.cancel_request_count, 1)
        finally:
            # An operation that finishes anyway still leaves the task
            # cancelled, as the task was asked to be.
            source.complete(1)

        with self.assertRaises(asyncio.CancelledError):
            await task

        self.assertEqual(op.status, wf.AsyncStatus.COMPLETED)

    @async_test
    async def test_async_operation_waits_for_the_handler(self) -> None:
        source = tc.AsyncOperationSource()
        source.defers_completed_handler = True
        op = source.operation

        task = asyncio.ensure_future(op)
        await asyncio.sleep(0)

        self.assertTrue(source.has_completed_handler)

        source.complete(1)
        await self._run_callbacks()

        # The status says the operation has finished before its completed
        # handler has said so, and an await that is waiting for the handler
        # goes on waiting for it.
        self.assertEqual(op.status, wf.AsyncStatus.COMPLETED)
        self.assertFalse(task.done())

        source.invoke_completed_handler()

        self.assertEqual(await task, 1)

    @async_test
    async def test_async_operation_completed_on_another_thread(self) -> None:
        source = tc.AsyncOperationSource()
        op = source.operation
        thread = threading.Thread(target=source.complete, args=(1,))

        task = asyncio.ensure_future(op)
        await asyncio.sleep(0)
        thread.start()

        self.assertEqual(await task, 1)

        thread.join()

    @async_test
    async def test_async_operation_released(self) -> None:
        source = tc.AsyncOperationSource()
        op = source.operation
        source.complete(1)

        self.assertEqual(await op, 1)

        # Neither the future nor the completed handler that the operation
        # keeps holds the operation once it has been awaited. The loop lets go
        # of the callback that resumed this coroutine one iteration later.
        del op
        await asyncio.sleep(0)

        self.assertFalse(source.is_operation_alive)

    @async_test
    async def test_async_operation_awaited_by_two_tasks(self) -> None:
        source = tc.AsyncOperationSource()
        op = source.operation

        # WinRT takes one completed handler, so a second await of a running
        # operation fails, and a task around it is what two tasks can share.
        shared = asyncio.ensure_future(op)
        await asyncio.sleep(0)

        with self.assertRaises(OSError) as ctx:
            await op

        self.assertEqual(ctx.exception.winerror, E_ILLEGAL_DELEGATE_ASSIGNMENT)

        async def waiter() -> int:
            return await shared

        tasks = [asyncio.create_task(waiter()) for _ in range(2)]
        source.complete(1)

        self.assertEqual(await asyncio.gather(*tasks), [1, 1])

    def test_async_operation_completed_after_its_loop_closed(self) -> None:
        source = tc.AsyncOperationSource()
        op = source.operation
        loop = asyncio.new_event_loop()
        # The task is still pending when it is freed, which the loop reports.
        loop.set_exception_handler(lambda loop, context: None)

        async def wait() -> None:
            await op

        task = loop.create_task(wait())
        loop.run_until_complete(asyncio.sleep(0))
        loop.close()

        self.assertTrue(source.has_completed_handler)

        # The completion has no loop to go to, and the handler lets it go.
        with catch_unraisable() as unraisable:
            source.complete(1)

        self.assertEqual(unraisable, [])

        del task, op
        gc.collect()

        self.assertFalse(source.is_operation_alive)

    @async_test
    async def test_async_operation_error_raised_afresh(self) -> None:
        source = tc.AsyncOperationSource()
        op = source.operation
        source.fail(E_FAIL)

        errors: list[OSError] = []

        for _ in range(3):
            try:
                await op
            except OSError as error:
                errors.append(error)
                # As in CPython's test_future_traceback: the traceback does
                # not grow with every await.
                text = "".join(traceback.format_tb(error.__traceback__))
                self.assertEqual(text.count("await op"), 1)

        # Each one comes from get_results() itself, because a kept exception
        # would hold the operation through the frames of its traceback.
        self.assertEqual([e.winerror for e in errors], [E_FAIL] * 3)
        self.assertIsNot(errors[1], errors[0])

        del op, errors
        await asyncio.sleep(0)

        self.assertFalse(source.is_operation_alive)


T = TypeVar("T")


class Result(TypedDict, total=False):
    exception: BaseException


class WinrtAiter(Generic[T]):
    def __init__(self, op: wf.IAsyncActionWithProgress[T]):
        self._op = op
        # self._exception: Optional[BaseException] = None

        queue: asyncio.Queue[T] = asyncio.Queue()
        loop = asyncio.get_running_loop()
        result = Result()

        # Caution: don't add closure on self in these callbacks to avoid reference cycle

        def on_progress(op: wf.IAsyncActionWithProgress[T], value: T) -> None:
            loop.call_soon_threadsafe(queue.put_nowait, value)

        def on_completed(
            op: wf.IAsyncActionWithProgress[T], status: wf.AsyncStatus
        ) -> None:
            if status == wf.AsyncStatus.COMPLETED:
                pass
            elif status == wf.AsyncStatus.CANCELED:
                result["exception"] = asyncio.CancelledError()
            elif status == wf.AsyncStatus.ERROR:
                result["exception"] = ctypes.WinError(op.error_code)
            else:
                raise RuntimeError("unexpected status")

            # FIXME: shutdown() was introduced in 3.13, so we can't use it generally
            loop.call_soon_threadsafe(queue.shutdown)

        self._op.progress = on_progress
        self._op.completed = on_completed
        self._queue = queue
        self._loop = loop
        self._result = result

    def __aiter__(self) -> "WinrtAiter[T]":
        return self

    async def __anext__(self) -> T:
        try:
            return await self._queue.get()
        except asyncio.QueueShutDown:
            # this acts as signal that the operation is done
            pass

        if ex := self._result.pop("exception", None):
            raise ex

        raise StopAsyncIteration

    def __del__(self) -> None:
        if ex := self._result.pop("exception", None):
            self._loop.call_exception_handler(
                context={
                    "message": "WinrtAiter was not run to completion and has outstanding exception",
                    "exception": ex,
                }
            )

    async def aclose(self) -> None:
        if self._op.status == wf.AsyncStatus.STARTED:
            self._op.cancel()

        while True:
            try:
                await self._queue.get()
            except asyncio.QueueShutDown:
                self._result.pop("exception", None)
                break
