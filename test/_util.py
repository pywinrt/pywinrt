"""
Helper functions shared by tests.
"""

import asyncio

from typing import Any, Callable


def async_test(test: Callable[..., Any]) -> Callable[..., None]:
    """
    Decorator to run async test methods.
    """
    def wrapper(*args: Any, **kwargs: Any) -> None:
        asyncio.run(test(*args, **kwargs))
    return wrapper
