import importlib.resources
import types
import unittest

import winrt.windows.storage.pickers as wsp
import winrt.windows.system.power as wsysp


def _line_before(module: types.ModuleType, prefix: str) -> str:
    lines = (
        importlib.resources.files(module)
        .joinpath("__init__.pyi")
        .read_text(encoding="utf-8")
        .splitlines()
    )
    index = next(i for i, line in enumerate(lines) if line.lstrip().startswith(prefix))
    return lines[index - 1].strip()


class TestStubs(unittest.TestCase):
    def test_deprecated_method(self):
        self.assertEqual(
            _line_before(wsp, "def pick_single_file_and_continue("),
            '@deprecated("Instead, use PickSingleFileAsync")',
        )

    def test_deprecated_class(self):
        self.assertEqual(
            _line_before(wsysp, "class BackgroundEnergyManager("),
            '@deprecated("Background Energy Manager has been deprecated. For more info, see MSDN.")',
        )
