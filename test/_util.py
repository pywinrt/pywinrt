"""
Helper functions shared by tests.
"""

import asyncio
import contextlib
import sys
import unittest
from types import TracebackType
from typing import Any, Protocol
from collections.abc import Callable, Generator


def skip_without_projection() -> None:
    """
    Skips the calling test module because it needs a projection package.

    The generator emits a table per namespace instead of a compiled extension
    module, and ``winrt-runtime`` does not interpret one yet, so importing any
    projection package fails. Calling this before those imports keeps the suite
    reporting skips rather than a wall of import errors while the interpreter is
    being built. Delete the call from a module once the interpreter covers what
    it tests.
    """
    raise unittest.SkipTest("winrt-runtime does not interpret projection tables yet")


def async_test(test: Callable[..., Any]) -> Callable[..., None]:
    """
    Decorator to run async test methods.
    """

    def wrapper(*args: Any, **kwargs: Any) -> None:
        asyncio.run(test(*args, **kwargs))

    return wrapper


class UnraisableHookArgs(Protocol):
    exc_type: type[BaseException]
    exc_value: BaseException | None
    exc_traceback: TracebackType | None
    err_msg: str | None
    object: object


@contextlib.contextmanager
def catch_unraisable() -> Generator[list[UnraisableHookArgs], None, None]:
    old_hook = sys.unraisablehook
    try:
        exceptions: list[UnraisableHookArgs] = []

        def catch(exc: UnraisableHookArgs) -> None:
            exceptions.append(exc)

        sys.unraisablehook = catch

        yield exceptions
    finally:
        sys.unraisablehook = old_hook
