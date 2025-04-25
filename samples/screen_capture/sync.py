# SPDX-License-Identifier: MIT
# Copyright 2024 David Lechner <david@pybricks.com>

from concurrent.futures import Future
from ctypes import WinError
from typing import Optional, TypeVar, Union, overload

from winrt.windows.foundation import AsyncStatus, IAsyncAction, IAsyncOperation

T = TypeVar("T")


@overload
def wait_for(operation: IAsyncOperation[T]) -> T: ...
@overload
def wait_for(operation: IAsyncAction) -> None: ...


def wait_for(operation: Union[IAsyncOperation[T], IAsyncAction]) -> Optional[T]:
    """
    Wait for the given async operation to complete and return its result.

    For use in non-asyncio apps.
    """
    future = Future[Optional[T]]()

    def completed(async_op: IAsyncOperation[T] | IAsyncAction, status: AsyncStatus):
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

    return future.result()
