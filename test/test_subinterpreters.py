"""
winrt-runtime refuses to be imported into any interpreter but the main one.

A WinRT callback that arrives on a thread Python has not seen attaches to the
main interpreter, so in any other interpreter it would run that interpreter's
Python code under the main one's thread state. Refusing the import is what
keeps that from happening quietly, and it is checked for both kinds of
subinterpreter: the legacy kind, which shares the main interpreter's GIL and
lets any extension module in unless the module stops it, and the isolated kind
that concurrent.interpreters makes.
"""

import sys
import unittest

try:
    from _testcapi import run_in_subinterp  # type: ignore[import-not-found]
except ImportError:
    run_in_subinterp = None

# Runs in the subinterpreter, which has only an exit status to report with.
# Where the interpreter checks the slot, which a free-threaded build does even
# for the legacy kind, CPython refuses with its own message before the runtime
# can refuse with its own.
REFUSED = """
try:
    import winrt._winrt
except ImportError as e:
    assert "interpreter" in str(e), str(e)
else:
    raise AssertionError("winrt._winrt was imported")
"""


class TestSubinterpreters(unittest.TestCase):
    @unittest.skipIf(run_in_subinterp is None, "_testcapi is not available")
    def test_legacy_subinterpreter_is_refused(self) -> None:
        self.assertEqual(run_in_subinterp(REFUSED), 0)

    def test_isolated_subinterpreter_is_refused(self) -> None:
        # A version check rather than a skip decorator, so that a type checker
        # running on an older Python does not look for the module.
        if sys.version_info < (3, 14):
            self.skipTest("concurrent.interpreters is 3.14+")
        else:
            import concurrent.interpreters as interpreters

            interp = interpreters.create()
            try:
                with self.assertRaises(interpreters.ExecutionFailed) as ctx:
                    interp.exec("import winrt._winrt")
            finally:
                interp.close()

            self.assertEqual(ctx.exception.excinfo.type.__name__, "ImportError")
