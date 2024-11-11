"""
Helper functions shared by tests.
"""

import asyncio
import contextlib
import sys
from types import TracebackType
from typing import Any, Callable, Generator, List, Optional, Protocol


def async_test(test: Callable[..., Any]) -> Callable[..., None]:
    """
    Decorator to run async test methods.
    """

    def wrapper(*args: Any, **kwargs: Any) -> None:
        asyncio.run(test(*args, **kwargs))

    return wrapper


class UnraisableHookArgs(Protocol):
    exc_type: type[BaseException]
    exc_value: Optional[BaseException]
    exc_traceback: Optional[TracebackType]
    err_msg: Optional[str]
    object: object


@contextlib.contextmanager
def catch_unraisable() -> Generator[List[UnraisableHookArgs], None, None]:
    old_hook = sys.unraisablehook
    try:
        exceptions: List[UnraisableHookArgs] = []

        def catch(exc: UnraisableHookArgs) -> None:
            exceptions.append(exc)

        sys.unraisablehook = catch

        yield exceptions
    finally:
        sys.unraisablehook = old_hook
