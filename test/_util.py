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
    Skips the calling test module because it needs more of the interpreter than
    there is.

    A projection package is a table that ``winrt-runtime`` interprets, and the
    interpreter is being built a piece at a time: it makes the types, calls
    their members, closes their parameterized interfaces and answers calls that
    come back the other way, so what is still missing is arrays, a Python list
    or dict passed where WinRT wants a collection, Python subclasses of
    composable classes, the numeric members of the ``Numerics`` structs and the
    compiled interop modules. Calling this before a module's imports keeps the
    suite reporting skips rather than a wall of errors. Delete the call from a
    module once the interpreter covers what it tests; ``test_projection.py``,
    ``test_delegates.py`` and ``test_implements.py`` cover the part that is
    already there.
    """
    raise unittest.SkipTest("winrt-runtime does not interpret this part of a table yet")


def skip_without_python_collections() -> None:
    """
    Skips the calling test because it passes a Python list, dict or iterable
    where WinRT expects a collection, which the interpreter does not wrap yet.
    """
    raise unittest.SkipTest(
        "winrt-runtime does not implement WinRT collections over Python objects yet"
    )


def skip_without_arrays() -> None:
    """
    Skips the calling test because it passes or receives a WinRT array, which
    the interpreter does not convert yet.
    """
    raise unittest.SkipTest("winrt-runtime does not interpret array parameters yet")


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
