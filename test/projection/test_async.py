import unittest

import test_winrt.testcomponent as tc
from winrt.system.hresult import (
    E_FAIL,
    E_ILLEGAL_DELEGATE_ASSIGNMENT,
    WIN32_ERROR_CANCELLED,
)
import winrt.runtime as wr
import winrt.windows.foundation as wf


class TestAsync(unittest.TestCase):
    def test_async_action_get_sta(self) -> None:
        wr.init_apartment(wr.ApartmentType.SINGLE_THREADED)
        try:
            with self.assertRaisesRegex(
                RuntimeError, r"^get\(\) cannot block a single-threaded apartment$"
            ):
                tc.TestRunner.create_async_action(10).get()
        finally:
            wr.uninit_apartment()

    def test_async_action_get(self) -> None:
        op = tc.TestRunner.create_async_action(10)

        self.assertIsNone(op.get())
        self.assertEqual(op.status, wf.AsyncStatus.COMPLETED)

    def test_async_action_get_cancel(self) -> None:
        op = tc.TestRunner.create_async_action(10)
        op.cancel()
        with self.assertRaises(OSError) as ctx:
            op.get()

        self.assertEqual(ctx.exception.winerror, WIN32_ERROR_CANCELLED)

    def test_async_action_get_error(self) -> None:
        with self.assertRaises(OSError) as ctx:
            tc.TestRunner.create_async_action_with_error(10, E_FAIL).get()

        self.assertEqual(ctx.exception.winerror, E_FAIL)

    def test_async_action_wait_sta(self) -> None:
        wr.init_apartment(wr.ApartmentType.SINGLE_THREADED)
        try:
            with self.assertRaisesRegex(
                RuntimeError, r"^wait\(\) cannot block a single-threaded apartment$"
            ):
                tc.TestRunner.create_async_action(10).wait(1)
        finally:
            wr.uninit_apartment()

    def test_async_action_wait(self) -> None:
        status = tc.TestRunner.create_async_action(10).wait(1)
        self.assertEqual(status, wf.AsyncStatus.COMPLETED)

    def test_async_action_wait_timeout(self) -> None:
        status = tc.TestRunner.create_async_action(1000).wait(0.1)
        self.assertEqual(status, wf.AsyncStatus.STARTED)

    def test_async_action_wait_not_a_duration(self) -> None:
        # A timeout that is not a positive number of seconds asks for the
        # status as it stands rather than for a wait. Each one gets its own
        # operation because a WinRT async object accepts only one completed
        # handler, so a second wait on a running one is an error.
        for timeout in (0, -1, float("nan")):
            with self.subTest(timeout=timeout):
                op = tc.TestRunner.create_async_action(1000)
                self.assertEqual(op.wait(timeout), wf.AsyncStatus.STARTED)

    def test_async_action_wait_cancel(self) -> None:
        op = tc.TestRunner.create_async_action(10)
        op.cancel()
        status = op.wait(1)
        self.assertEqual(status, wf.AsyncStatus.CANCELED)

    def test_async_action_wait_error(self) -> None:
        status = tc.TestRunner.create_async_action_with_error(10, E_FAIL).wait(1)
        self.assertEqual(status, wf.AsyncStatus.ERROR)

    def test_async_operation_get(self) -> None:
        expected = 1
        actual = tc.TestRunner.create_async_operation(10, expected).get()
        self.assertEqual(expected, actual)

    def test_async_operation_get_sta(self) -> None:
        wr.init_apartment(wr.ApartmentType.SINGLE_THREADED)
        try:
            with self.assertRaisesRegex(
                RuntimeError, r"^get\(\) cannot block a single-threaded apartment$"
            ):
                tc.TestRunner.create_async_operation(10, 1).get()
        finally:
            wr.uninit_apartment()

    def test_async_operation_get_cancel(self) -> None:
        op = tc.TestRunner.create_async_operation(10, 1)
        op.cancel()
        with self.assertRaises(OSError) as ctx:
            op.get()

        self.assertEqual(ctx.exception.winerror, WIN32_ERROR_CANCELLED)

    def test_async_operation_get_error(self) -> None:
        with self.assertRaises(OSError) as ctx:
            tc.TestRunner.create_async_operation_with_error(10, 1, E_FAIL).get()

        self.assertEqual(ctx.exception.winerror, E_FAIL)

    def test_async_operation_wait_sta(self) -> None:
        wr.init_apartment(wr.ApartmentType.SINGLE_THREADED)
        try:
            with self.assertRaisesRegex(
                RuntimeError, r"^wait\(\) cannot block a single-threaded apartment$"
            ):
                tc.TestRunner.create_async_operation(10, 1).wait(1)
        finally:
            wr.uninit_apartment()

    def test_async_operation_wait(self) -> None:
        status = tc.TestRunner.create_async_operation(10, 1).wait(1)
        self.assertEqual(status, wf.AsyncStatus.COMPLETED)

    def test_async_operation_wait_timeout(self) -> None:
        status = tc.TestRunner.create_async_operation(1000, 1).wait(0.1)
        self.assertEqual(status, wf.AsyncStatus.STARTED)

    def test_async_operation_wait_cancel(self) -> None:
        op = tc.TestRunner.create_async_operation(10, 1)
        op.cancel()
        status = op.wait(1)
        self.assertEqual(status, wf.AsyncStatus.CANCELED)

    def test_async_operation_wait_error(self) -> None:
        status = tc.TestRunner.create_async_operation_with_error(10, 1, E_FAIL).wait(1)
        self.assertEqual(status, wf.AsyncStatus.ERROR)

    def test_async_action_with_progress_get(self) -> None:
        op = tc.TestRunner.create_async_action_with_progress(10, [1, 2])

        self.assertIsNone(op.get())
        self.assertEqual(op.status, wf.AsyncStatus.COMPLETED)

    def test_async_action_progress_handler_is_replaced(self) -> None:
        first: list[int] = []
        second: list[int] = []

        op = tc.TestRunner.create_async_action_with_progress(10, [1, 2])
        # https://github.com/microsoft/pyright/issues/11833
        op.progress = lambda sender, value: first.append(value)  # pyright: ignore[reportAttributeAccessIssue, reportUnknownLambdaType, reportUnknownArgumentType]
        op.progress = lambda sender, value: second.append(value)  # pyright: ignore[reportAttributeAccessIssue, reportUnknownLambdaType, reportUnknownArgumentType]
        op.get()

        self.assertEqual(first, [])
        self.assertEqual(second, [1, 2])

    def test_async_action_with_progress_wait(self) -> None:
        status = tc.TestRunner.create_async_action_with_progress(10, [1, 2]).wait(1)
        self.assertEqual(status, wf.AsyncStatus.COMPLETED)

    def test_async_operation_with_progress_get(self) -> None:
        expected = 3
        actual = tc.TestRunner.create_async_operation_with_progress(
            10, [1, 2], expected
        ).get()
        self.assertEqual(expected, actual)

    def test_async_operation_with_progress_wait(self) -> None:
        status = tc.TestRunner.create_async_operation_with_progress(10, [1, 2], 3).wait(
            1
        )
        self.assertEqual(status, wf.AsyncStatus.COMPLETED)

    def test_async_info_cancel(self) -> None:
        source = tc.AsyncActionSource()
        info = source.operation.as_(wf.IAsyncInfo)

        info.cancel()
        self.assertEqual(source.cancel_request_count, 1)

        source.complete()

    def test_async_handlers_refuse_none(self) -> None:
        source = tc.AsyncActionSource()
        op = source.operation

        with self.assertRaisesRegex(TypeError, "takes a callable, not None"):
            op.completed = None  # type: ignore[assignment]

        self.assertFalse(source.has_completed_handler)
        source.complete()

    def test_async_completed_takes_one_handler(self) -> None:
        source = tc.AsyncOperationSource()
        op = source.operation

        def handler(sender: wf.IAsyncOperation[int], status: wf.AsyncStatus) -> None:
            pass

        op.completed = handler

        with self.assertRaises(OSError) as ctx:
            op.completed = handler

        self.assertEqual(ctx.exception.winerror, E_ILLEGAL_DELEGATE_ASSIGNMENT)
        source.complete(1)

    def test_async_progress_refuses_none(self) -> None:
        # A C++/WinRT coroutine, which dereferences a null handler.
        op = tc.TestRunner.create_async_action_with_progress(0, [])

        with self.assertRaisesRegex(TypeError, "takes a callable, not None"):
            op.progress = None  # type: ignore[assignment]

        with self.assertRaisesRegex(TypeError, "takes a callable, not None"):
            op.completed = None  # type: ignore[assignment]

        op.get()
