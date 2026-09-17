"""
The ABI promise, checked from the Python side.

A projection module compiled against runtime ABI major M and minor m loads on
any runtime that declares major M and a minor no older than m. That check is
made in C++ by ``py::import_winrt_runtime()`` before the module object exists,
so every module that made it into ``sys.modules`` has already passed it. What
is checked here is the pair the module reports back to Python afterwards, since
that is the copy ``winrt.doctor``, the packaging tooling and
``scripts/abi-compat.py`` read, and a module whose reported pair disagreed with
the one it was compiled against would send all three the wrong way.
"""

from test._util import skip_without_projection

skip_without_projection()

import sys
import unittest
from collections.abc import Iterator

import winrt._winrt

# Top-level packages that projection modules are installed into.
PROJECTION_ROOTS = ("winrt", "winui2", "winui3", "webview2", "test_winrt")


def iter_reported_abi_versions() -> Iterator[tuple[str, tuple[int, int]]]:
    """
    The name and reported ABI version of every projection module the test suite
    has imported.

    unittest imports every test module before it runs any of them, so by the
    time this runs, sys.modules holds every projection module the suite
    touches. Reading it costs nothing, and it stays costing nothing in a full
    projection build, where importing every module to check this one attribute
    would mean importing four hundred of them.
    """
    for name, module in list(sys.modules.items()):
        if name.partition(".")[0] not in PROJECTION_ROOTS:
            continue

        abi_version = getattr(module, "_abi_version_", None)

        if abi_version is not None:
            yield name, abi_version


class TestAbiVersion(unittest.TestCase):
    def test_modules_agree_with_the_runtime(self) -> None:
        runtime_major, runtime_minor = winrt._winrt.abi_version

        count = 0

        for name, (major, minor) in iter_reported_abi_versions():
            count += 1

            with self.subTest(module=name):
                self.assertEqual(
                    major,
                    runtime_major,
                    "built against a different ABI major than the runtime provides",
                )
                self.assertLessEqual(
                    minor,
                    runtime_minor,
                    "needs a newer ABI minor than the runtime provides",
                )

        self.assertGreater(count, 0, "no projection modules found")

    def test_versions_are_reported(self) -> None:
        # winrt.windows.foundation is built in every configuration, so this is
        # the one module that is named rather than discovered: if the generator
        # stopped emitting the attributes, the test above would just find
        # nothing to check and still pass.
        import winrt.windows.foundation

        self.assertEqual(
            winrt.windows.foundation._abi_version_, winrt._winrt.abi_version
        )
        self.assertIsInstance(winrt.windows.foundation._generator_version_, str)
        self.assertTrue(winrt.windows.foundation._generator_version_)
