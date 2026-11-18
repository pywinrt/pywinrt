"""
A member that names a type from a package that is not installed.

Each test of a member puts a stand-in for the module that it names into
``sys.modules``, or a finder for it into ``sys.meta_path``, where the import
system looks before it looks on ``sys.path``. Nothing else in the suite
imports those modules, and a test that finds one already imported is skipped,
since the runtime would then find its types without importing anything.
"""

import importlib.abc
import importlib.machinery
import sys
import types
import unittest
from collections.abc import Sequence
from unittest import mock

import winrt.runtime as wr
from winrt.runtime._internals import note_missing_package
from winrt.system import box_string
from winrt.windows.media.core import MediaSource
from winrt.windows.ui.applicationsettings import SettingsCommand

CAPTURE = "winrt.windows.media.capture"
FRAMES = "winrt.windows.media.capture.frames"
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


class FailsToImport(importlib.abc.Loader):
    """
    Imports a package whose types need a type of a package it sits in that
    is missing, failing as the runtime does when it builds them.
    """

    def __init__(self, missing: str, note: str) -> None:
        self.missing = missing
        self.note = note

    def create_module(self, spec: importlib.machinery.ModuleSpec) -> None:
        return None

    def exec_module(self, module: types.ModuleType) -> None:
        error = ModuleNotFoundError(
            f"No module named {self.missing!r}", name=self.missing
        )
        error.add_note(self.note)
        raise error


class Installed(importlib.abc.MetaPathFinder):
    """Finds a package that is installed but fails to import."""

    def __init__(self, name: str, loader: importlib.abc.Loader) -> None:
        self.name = name
        self.loader = loader

    def find_spec(
        self,
        fullname: str,
        path: Sequence[str] | None,
        target: types.ModuleType | None = None,
    ) -> importlib.machinery.ModuleSpec | None:
        if fullname == self.name:
            return importlib.machinery.ModuleSpec(fullname, self.loader)

        return None


class NotInstalled(importlib.abc.MetaPathFinder):
    """Finds that a package is not installed, as the path finder would."""

    def __init__(self, name: str) -> None:
        self.name = name

    def find_spec(
        self,
        fullname: str,
        path: Sequence[str] | None,
        target: types.ModuleType | None = None,
    ) -> importlib.machinery.ModuleSpec | None:
        if fullname == self.name:
            raise ModuleNotFoundError(f"No module named {fullname!r}", name=fullname)

        return None


class TestMissingType(unittest.TestCase):
    """
    ``MediaSource.create_from_media_frame_source()`` takes a
    ``Windows.Media.Capture.Frames`` type, which is looked up before WinRT is
    called.
    """

    def setUp(self) -> None:
        for module in (CAPTURE, FRAMES):
            if module in sys.modules:
                self.skipTest(f"{module} is already imported")

        self.addCleanup(sys.modules.pop, FRAMES, None)

    def call(self) -> None:
        MediaSource.create_from_media_frame_source(None)  # type: ignore[arg-type]

    def test_not_installed(self) -> None:
        sys.modules[FRAMES] = None  # type: ignore[assignment]

        with self.assertRaisesRegex(
            ModuleNotFoundError, f"^import of {FRAMES} halted; None in sys.modules$"
        ) as cm:
            self.call()

        self.assertEqual(cm.exception.name, FRAMES)
        self.assertEqual(
            cm.exception.__notes__,
            ["The winrt-Windows.Media.Capture.Frames package provides it."],
        )

    def test_namespace_package(self) -> None:
        sys.modules[FRAMES] = namespace_package(FRAMES)

        with self.assertRaisesRegex(
            ModuleNotFoundError, f"^No module named '{FRAMES}'$"
        ) as cm:
            self.call()

        self.assertEqual(cm.exception.name, FRAMES)
        self.assertEqual(
            cm.exception.__notes__,
            ["The winrt-Windows.Media.Capture.Frames package provides it."],
        )

    def test_package_not_installed(self) -> None:
        # The import system imports the packages a module sits in first, so
        # the error is about the first that is missing.
        finder = NotInstalled(CAPTURE)
        sys.meta_path.insert(0, finder)
        self.addCleanup(sys.meta_path.remove, finder)

        with self.assertRaisesRegex(
            ModuleNotFoundError, f"^No module named '{CAPTURE}'$"
        ) as cm:
            self.call()

        self.assertEqual(cm.exception.name, CAPTURE)
        self.assertEqual(
            cm.exception.__notes__,
            [f"The winrt-Windows.Media.Capture.Frames package provides {FRAMES}."],
        )

    def test_package_fails_to_import(self) -> None:
        # The package is installed, so the note about the package it sits in
        # is the only one that is right.
        note = "The winrt-Windows.Media.Capture package provides it."
        capture = namespace_package(CAPTURE)
        capture.__path__ = []
        sys.modules[CAPTURE] = capture
        self.addCleanup(sys.modules.pop, CAPTURE, None)
        finder = Installed(FRAMES, FailsToImport(CAPTURE, note))
        sys.meta_path.insert(0, finder)
        self.addCleanup(sys.meta_path.remove, finder)

        with self.assertRaisesRegex(
            ModuleNotFoundError, f"^No module named '{CAPTURE}'$"
        ) as cm:
            self.call()

        self.assertEqual(cm.exception.name, CAPTURE)
        self.assertEqual(cm.exception.__notes__, [note])

    def test_note_fails(self) -> None:
        # The note only adds to what the error says.
        sys.modules[FRAMES] = None  # type: ignore[assignment]

        with mock.patch(
            "winrt.runtime._internals.note_missing_package",
            side_effect=RuntimeError("note"),
        ) as note:
            with self.assertRaises(ModuleNotFoundError) as cm:
                self.call()

        note.assert_called_once()
        self.assertEqual(cm.exception.name, FRAMES)
        self.assertFalse(hasattr(cm.exception, "__notes__"))

    def test_note_interrupted(self) -> None:
        # as an exception raised while handling one would be in Python
        sys.modules[FRAMES] = None  # type: ignore[assignment]

        with mock.patch(
            "winrt.runtime._internals.note_missing_package",
            side_effect=KeyboardInterrupt,
        ):
            with self.assertRaises(KeyboardInterrupt) as cm:
                self.call()

        self.assertIsInstance(cm.exception.__context__, ModuleNotFoundError)


class TestMissingDelegate(unittest.TestCase):
    """
    ``SettingsCommand``'s constructor takes a ``Windows.UI.Popups`` delegate,
    which is converted before WinRT is called.

    Every other member of the Windows SDK that takes a delegate of a namespace
    the suite does not import, and not of a package its own module sits in,
    needs XAML, a window or a background task. The class is activated only in
    a single-threaded apartment, and the tests share one: the runtime keeps
    the activation factory it gets, which does not outlive the apartment.
    """

    @classmethod
    def setUpClass(cls) -> None:
        wr.init_apartment(wr.ApartmentType.SINGLE_THREADED)
        cls.addClassCleanup(wr.uninit_apartment)

    def setUp(self) -> None:
        if POPUPS in sys.modules:
            self.skipTest(f"{POPUPS} is already imported")

        self.addCleanup(sys.modules.pop, POPUPS, None)

    def call(self) -> None:
        SettingsCommand(box_string("id"), "label", lambda command: None)

    def test_not_installed(self) -> None:
        sys.modules[POPUPS] = None  # type: ignore[assignment]

        with self.assertRaisesRegex(
            ModuleNotFoundError, f"^import of {POPUPS} halted; None in sys.modules$"
        ) as cm:
            self.call()

        self.assertEqual(cm.exception.name, POPUPS)
        self.assertEqual(
            cm.exception.__notes__,
            ["The winrt-Windows.UI.Popups package provides it."],
        )

    def test_namespace_package(self) -> None:
        sys.modules[POPUPS] = namespace_package(POPUPS)

        with self.assertRaisesRegex(
            ModuleNotFoundError, f"^No module named '{POPUPS}'$"
        ) as cm:
            self.call()

        self.assertEqual(cm.exception.name, POPUPS)
        self.assertEqual(
            cm.exception.__notes__,
            ["The winrt-Windows.UI.Popups package provides it."],
        )


class TestNoteMissingPackage(unittest.TestCase):
    """The cases no member of the Windows SDK reaches."""

    def test_another_module(self) -> None:
        # as when importing the package fails on something it imports itself,
        # whose name may start as a package of the module's does
        for name in ("other", "winrt.windows.media.cap"):
            with self.subTest(name=name):
                error = ModuleNotFoundError(f"No module named '{name}'", name=name)
                note_missing_package(error, FRAMES, "Windows.Media.Capture.Frames")
                self.assertFalse(hasattr(error, "__notes__"))

    def test_another_family(self) -> None:
        # WebView2's package is named after a namespace that its own sits
        # under, so the module does not say which package it is.
        module = "winrt.microsoft.web.webview2.core"
        error = ModuleNotFoundError(f"No module named '{module}'", name=module)

        note_missing_package(error, module, "Microsoft.Web.WebView2.Core")

        self.assertFalse(hasattr(error, "__notes__"))
