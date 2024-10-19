# SPDX-License-Identifier: MIT
# Copyright 2024 David Lechner <david@pybricks.com>

from ctypes import WinError
from concurrent.futures import Future, wait
from typing import TypeVar

from winrt.windows.foundation import IAsyncOperation, AsyncStatus

T = TypeVar("T")


def wait_for(operation: IAsyncOperation[T]) -> T:
    """
    Wait for the given async operation to complete and return its result.

    For use in non-asyncio apps.
    """
    future = Future[T]()

    def completed(async_op: IAsyncOperation, status: AsyncStatus):
        try:
            if status == AsyncStatus.COMPLETED:
                future.set_result(async_op.get_results())
            elif status == AsyncStatus.ERROR:
                future.set_exception(WinError(async_op.error_code.value))
            elif status == AsyncStatus.CANCELED:
                future.cancel()
        except BaseException as e:
            future.set_exception(e)

    operation.completed = completed

    wait([future])

    return future.result()
