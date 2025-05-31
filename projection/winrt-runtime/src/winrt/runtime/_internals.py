import asyncio
import os
from collections.abc import Awaitable, Callable
from pathlib import Path
from typing import TypeVar, Protocol, TYPE_CHECKING

from typing_extensions import Self

from winrt._winrt import add_dll_directory, remove_dll_directory

# Unfortunately, we can't import at runtime because of circular imports.
if TYPE_CHECKING:
    from winrt.windows.foundation import HResult, AsyncStatus


class _DllCookie:
    def __init__(self, cookie: int) -> None:
        self.cookie = cookie

    def close(self):
        if self.cookie:
            remove_dll_directory(self.cookie)
            self.cookie = None

    def __del__(self):
        self.close()


def register_dll_search_path(module_path: str) -> _DllCookie:
    """
    Register a module's directory as a DLL search path.

    Args:
        module_path: The path to a module file (i.e. ``__file__``)

    Returns:
        An cookie object that will remove the search path when closed.
    """
    return _DllCookie(add_dll_directory(os.fspath(Path(module_path).parent.resolve())))


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
