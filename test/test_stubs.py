import importlib.resources
import types
import unittest

import winrt.windows.security.cryptography.certificates as wscc
import winrt.windows.storage.pickers as wsp
import winrt.windows.system.power as wsysp


def _stub_lines(module: types.ModuleType) -> list[str]:
    return (
        importlib.resources.files(module)
        .joinpath("__init__.pyi")
        .read_text(encoding="utf-8")
        .splitlines()
    )


def _line(module: types.ModuleType, prefix: str) -> str:
    return next(
        line.strip() for line in _stub_lines(module) if line.lstrip().startswith(prefix)
    )


def _line_before(module: types.ModuleType, prefix: str) -> str:
    lines = _stub_lines(module)
    index = next(i for i, line in enumerate(lines) if line.lstrip().startswith(prefix))
    return lines[index - 1].strip()


class TestStubs(unittest.TestCase):
    def test_deprecated_method(self) -> None:
        self.assertEqual(
            _line_before(wsp, "def pick_single_file_and_continue("),
            '@deprecated("Instead, use PickSingleFileAsync")',
        )

    def test_deprecated_class(self) -> None:
        self.assertEqual(
            _line_before(wsysp, "class BackgroundEnergyManager("),
            '@deprecated("Background Energy Manager has been deprecated. For more info, see MSDN.")',
        )

    def test_array_valued_property(self) -> None:
        # The getter returns a winrt.system.Array, as a method returning an
        # array does, rather than one element of it.
        self.assertEqual(
            _line(wscc, "def serial_number("),
            "def serial_number(self) -> winrt.system.Array[winrt.system.UInt8]: ...",
        )
