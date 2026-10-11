"""Smoke-test no-GIL WinRT runtime imports and concurrent boxing."""

import os
import subprocess
import sys
import sysconfig
import textwrap
import unittest


@unittest.skipUnless(
    sysconfig.get_config_var("Py_GIL_DISABLED") == 1,
    "requires free-threaded CPython",
)
class TestFreeThreading(unittest.TestCase):
    def test_runtime_import_and_parallel_boxing(self):
        script = textwrap.dedent(
            """\
            import concurrent.futures
            import sys

            assert sys._is_gil_enabled() is False
            import winrt._winrt as runtime
            assert sys._is_gil_enabled() is False

            def worker(seed):
                for i in range(500):
                    value = seed * 100000 + i
                    assert runtime.unbox_int32(runtime.box_int32(value)) == value
                return seed

            with concurrent.futures.ThreadPoolExecutor(max_workers=8) as pool:
                assert list(pool.map(worker, range(8))) == list(range(8))
            assert sys._is_gil_enabled() is False
            """
        )
        env = os.environ.copy()
        env.pop("PYTHON_GIL", None)
        result = subprocess.run(
            [sys.executable, "-W", "error", "-c", script],
            env=env,
            capture_output=True,
            text=True,
            timeout=40,
            check=False,
        )
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)


if __name__ == "__main__":
    unittest.main()