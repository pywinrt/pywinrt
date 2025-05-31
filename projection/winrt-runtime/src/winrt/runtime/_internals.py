import asyncio
from collections.abc import Awaitable, Callable
from typing import TypeVar, Protocol, TYPE_CHECKING

from typing_extensions import Self

# Unfortunately, we can't import at runtime because of circular imports.
if TYPE_CHECKING:
    from winrt.windows.foundation import HResult, AsyncStatus


T = TypeVar("T")


# MyPy wants covariant T but Pylance wants invariant. Invariant seems correct.
class AsyncOp(Protocol[T]):  # type: ignore [misc]
    """
    Protocol that matches both IAsyncAction and IAsyncOperation.
    """

    def cancel(self) -> None: ...
    @property
    def error_code(self) -> "HResult": ...
    @property
    def status(self) -> "AsyncStatus": ...
    def get_results(self) -> T: ...
    @property
    def completed(self) -> Callable[[Self, "AsyncStatus"], None]: ...
    @completed.setter
    def completed(self, value: Callable[[Self, "AsyncStatus"], None]) -> None: ...


def wrap_async(op: AsyncOp[T]) -> Awaitable[T]:
    """
    Wraps a WinRT async operation in a Python Awaitable.

    This is used by the runtime to implement ``__await__`` on async operations.

    Args:
        op: The WinRT async operation to wrap.

    Returns:
        An asyncio Awaitable that will be completed when the WinRT operation
        completes.
    """

    async def wait() -> T:
        loop = asyncio.get_running_loop()
        event = asyncio.Event()

        def on_complete(op: AsyncOp[T], status: "AsyncStatus") -> None:
            loop.call_soon_threadsafe(event.set)

        op.completed = on_complete

        try:
            await event.wait()
        except asyncio.CancelledError:
            op.cancel()
            # REVISIT: what if it is cancelled again?
            await event.wait()
            raise

        return op.get_results()

    return wait()
