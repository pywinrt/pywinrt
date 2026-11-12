"""
A member that names a type from a package that is not installed.

Each test puts a stand-in for the module that such a member names into
``sys.modules``, where the import system finds it before it looks on
``sys.path``. Nothing else in the suite imports those modules, and a test that
finds one already imported is skipped, since the runtime would then find its
types without importing anything.
"""

import importlib.machinery
import sys
import types
import unittest

import winrt.runtime as wr
from winrt.system import box_string
from winrt.windows.ui.applicationsettings import SettingsCommand

POPUPS = "winrt.windows.ui.popups"


def namespace_package(name: str) -> types.ModuleType:
    """
    A module as the import system makes one for a directory with no
    ``__init__.py``, which a namespace is when its package is missing and the
    package of a namespace below it is installed.
    """
    module = types.ModuleType(name)
    module.__spec__ = importlib.machinery.ModuleSpec(name, loader=None, is_package=True)
    return module


class TestMissingDelegate(unittest.TestCase):
    """
    ``SettingsCommand``'s constructor takes a ``Windows.UI.Popups`` delegate,
    which is converted before WinRT is called. The class is activated only in
    a single-threaded apartment.
    """

    def setUp(self) -> None:
        if POPUPS in sys.modules:
            self.skipTest(f"{POPUPS} is already imported")

        self.addCleanup(sys.modules.pop, POPUPS, None)
        wr.init_apartment(wr.ApartmentType.SINGLE_THREADED)
        self.addCleanup(wr.uninit_apartment)

    def test_namespace_package(self) -> None:
        sys.modules[POPUPS] = namespace_package(POPUPS)

        with self.assertRaisesRegex(
            ModuleNotFoundError, f"^No module named '{POPUPS}'$"
        ) as cm:
            SettingsCommand(box_string("id"), "label", lambda command: None)

        self.assertEqual(cm.exception.name, POPUPS)
