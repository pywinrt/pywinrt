"""
The ABI promise, checked from the Python side.

A compiled module built against runtime ABI major M and minor m loads on any
runtime that declares major M and a minor no older than m. That check is made
in C++ by ``py::import_winrt_runtime()`` before the module object exists, so
every module that made it into ``sys.modules`` has already passed it. What is
checked here is the pair the runtime reports back to Python, since that is the
copy ``winrt.doctor``, the packaging tooling and ``scripts/abi-compat.py`` read.

A projection package is not party to that contract at all any more: it ships a
table rather than compiled code, so what it has to agree with the runtime about
is the table format version, which ``test_table.py`` checks. The two contracts
have one rule each, and the pair below is the one for compiled consumers.
"""

import sys
import unittest

import winrt._winrt

# Top-level packages that projection packages are installed into.
PROJECTION_ROOTS = ("winrt", "winui2", "winui3", "webview2", "test_winrt")


class TestAbiVersion(unittest.TestCase):
    def test_the_runtime_reports_its_version(self) -> None:
        major, minor = winrt._winrt.abi_version

        self.assertIsInstance(major, int)
        self.assertIsInstance(minor, int)
        # The major is the compatibility generation, which is also the table
        # format major and the epoch in every projection package's version.
        self.assertGreater(major, 0)

    def test_a_projection_package_reports_no_abi_version(self) -> None:
        # The attribute went away with the compiled code that had to agree
        # about it. Anything still reporting one would send winrt.doctor and
        # the compat script looking for a contract the package is not party to.
        import winrt.windows.foundation

        reported = [
            name
            for name, module in list(sys.modules.items())
            if name.partition(".")[0] in PROJECTION_ROOTS
            and getattr(module, "_abi_version_", None) is not None
        ]

        self.assertEqual(reported, [])
        self.assertFalse(hasattr(winrt.windows.foundation, "_abi_version_"))
