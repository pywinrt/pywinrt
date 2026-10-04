import unittest

import winrt.microsoft.windows.applicationmodel.dynamicdependency.bootstrap as bootstrap
from winrt import (
    _winrt_microsoft_windows_applicationmodel_dynamicdependency_bootstrap as _bootstrap,
)


class TestBootstrap(unittest.TestCase):
    def test_initialize_no_match(self) -> None:
        with self.assertRaises(OSError):
            bootstrap.initialize("99.99")

    def test_initialize_checks_what_shutdown_made(self) -> None:
        # initialize() makes its result by calling the module's Shutdown,
        # which is an attribute anything can rebind, and fails before it asks
        # Windows for anything if the object is not one it can use.
        original = _bootstrap.Shutdown

        class NotShutdown:
            pass

        _bootstrap.Shutdown = NotShutdown  # type: ignore[misc,assignment]
        try:
            with self.assertRaisesRegex(TypeError, "NotShutdown"):
                bootstrap.initialize("99.99")
        finally:
            _bootstrap.Shutdown = original  # type: ignore[misc]
