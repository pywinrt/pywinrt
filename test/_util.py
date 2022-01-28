"""
Helper functions shared by tests.
"""

import asyncio


def async_test(test):
    """
    Decorator to run async test methods.
    """
    def wrapper(*args, **kwargs):
        asyncio.run(test(*args, **kwargs))
    return wrapper
