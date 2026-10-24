"""
What scripts/build-bdist.py --plan answers for a set of packages.

The packages are made up here rather than read from the tree, since the
answers depend only on how each one is built and which unit releases it.
"""

import contextlib
import importlib.util
import io
import sys
import unittest
from pathlib import Path
from types import ModuleType
from typing import Any
from unittest import mock

SCRIPTS = Path(__file__).parent.parent
SCRIPT = SCRIPTS / "build-bdist.py"


def load_script() -> ModuleType:
    spec = importlib.util.spec_from_file_location("build_bdist", SCRIPT)
    assert spec is not None and spec.loader is not None
    module = importlib.util.module_from_spec(spec)

    # the script imports packages.py beside it by its bare name
    sys.path.insert(0, str(SCRIPTS))

    try:
        spec.loader.exec_module(module)
    finally:
        sys.path.remove(str(SCRIPTS))

    return module


build_bdist = load_script()
packages = build_bdist.packages
versions = packages.versions


def package(distribution: str, family: str, release: str, build: str) -> Any:
    """
    A package of @p family that the @p release unit publishes, whose wheels
    are built the way @p build names.
    """
    return packages.Package(
        Path(distribution), distribution, family, release, packages.Build(build)
    )


RUNTIME = package("winrt-runtime", "runtime", "runtime", "compiled")
TABLE = package("winrt-table-compiler", "runtime", "table", "any")
FOUNDATION = package("winrt-Windows.Foundation", "winrt", "winrt", "any")
SYSTEM_INTEROP = package(
    "winrt-Windows.System.Interop",
    "winrt",
    "winrt-Windows.System.Interop",
    "compiled",
)
XAML = package("winui2-Microsoft.UI.Xaml", "winui2", "winui2", "any")
WEBVIEW2_DLL = package(
    "winrt-Microsoft.Web.WebView2.Dll", "webview2", "webview2", "redist"
)
UI_INTEROP = package("winrt-Microsoft.UI.Interop", "wasdk", "wasdk", "compiled")


class Plan(unittest.TestCase):
    """
    The three answers, over the units a release publishes.
    """

    def test_a_compiled_package_alone_is_the_compiled_half(self) -> None:
        self.assertEqual(
            build_bdist.plan([RUNTIME]),
            {"pure": "false", "compiled": "true", "trusted": "false"},
        )

    def test_a_data_package_alone_is_the_pure_half(self) -> None:
        self.assertEqual(
            build_bdist.plan([TABLE]),
            {"pure": "true", "compiled": "false", "trusted": "false"},
        )

    def test_a_dll_package_is_in_the_pure_half(self) -> None:
        # nothing in it is compiled either, whatever architecture it carries
        self.assertEqual(
            build_bdist.plan([WEBVIEW2_DLL]),
            {"pure": "true", "compiled": "false", "trusted": "true"},
        )

    def test_a_family_with_a_dll_package_is_still_one_half(self) -> None:
        self.assertEqual(
            build_bdist.plan([XAML, WEBVIEW2_DLL])["compiled"],
            "false",
        )

    def test_both_halves_when_the_units_need_both(self) -> None:
        self.assertEqual(
            build_bdist.plan([FOUNDATION, SYSTEM_INTEROP]),
            {"pure": "true", "compiled": "true", "trusted": "false"},
        )

    def test_the_units_in_trusted_publishing_are_trusted(self) -> None:
        for unit in sorted(packages.TRUSTED_PUBLISHING):
            with self.subTest(unit=unit):
                self.assertEqual(
                    build_bdist.plan([package("x", unit, unit, "any")])["trusted"],
                    "true",
                )

    def test_an_interop_module_released_with_a_family_follows_it(self) -> None:
        self.assertEqual(build_bdist.plan([UI_INTEROP])["trusted"], "true")

    def test_the_windows_sdk_units_are_on_the_token(self) -> None:
        for unit in [RUNTIME, TABLE, FOUNDATION, SYSTEM_INTEROP]:
            with self.subTest(unit=unit.release):
                self.assertEqual(build_bdist.plan([unit])["trusted"], "false")

    def test_units_on_both_sides_are_mixed(self) -> None:
        self.assertEqual(
            build_bdist.plan([RUNTIME, TABLE, FOUNDATION, XAML, WEBVIEW2_DLL]),
            {"pure": "true", "compiled": "true", "trusted": "mixed"},
        )

    def test_several_trusted_units_together_are_not_mixed(self) -> None:
        self.assertEqual(
            build_bdist.plan([XAML, WEBVIEW2_DLL, UI_INTEROP])["trusted"], "true"
        )


class Main(unittest.TestCase):
    """
    The command line, over what the tree publishes, which it reads and does
    not build.
    """

    def run_script(self, *args: str) -> str:
        output = io.StringIO()

        with (
            mock.patch.object(packages, "set_nuget_environment"),
            mock.patch.object(sys, "argv", ["build-bdist.py", *args]),
            contextlib.redirect_stdout(output),
        ):
            build_bdist.main()

        return output.getvalue()

    def test_the_plan_is_printed_as_output_lines(self) -> None:
        tag = versions.release_tags()["runtime"]

        self.assertEqual(
            self.run_script("--plan", "--tag", tag),
            "pure=false\ncompiled=true\ntrusted=false\n",
        )

    def test_a_tag_the_tree_does_not_publish_is_refused(self) -> None:
        with self.assertRaisesRegex(SystemExit, "releases nothing") as refused:
            self.run_script("--plan", "--tag", "wheels/runtime/v0")

        self.assertNotIsInstance(refused.exception.code, int)
