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
    "stable-abi",
)
XAML = package("winui2-Microsoft.UI.Xaml", "winui2", "winui2", "any")
WEBVIEW2_DLL = package(
    "winrt-Microsoft.Web.WebView2.Dll", "webview2", "webview2", "redist"
)
UI_INTEROP = package("winrt-Microsoft.UI.Interop", "wasdk", "wasdk", "stable-abi")
# the same module with a fix to it waiting, which makes it a unit of its own
UI_INTEROP_FIX = package(
    "winrt-Microsoft.UI.Interop",
    "wasdk",
    "winrt-Microsoft.UI.Interop",
    "stable-abi",
)


class Plan(unittest.TestCase):
    """
    The answers, over the units a release publishes.
    """

    def test_a_compiled_package_alone_is_the_compiled_part(self) -> None:
        self.assertEqual(
            build_bdist.plan([RUNTIME]),
            {
                "pure": "false",
                "compiled": "true",
                "stable_abi": "false",
                "trusted": "false",
            },
        )

    def test_an_interop_module_alone_is_the_stable_abi_part(self) -> None:
        self.assertEqual(
            build_bdist.plan([SYSTEM_INTEROP]),
            {
                "pure": "false",
                "compiled": "false",
                "stable_abi": "true",
                "trusted": "false",
            },
        )

    def test_a_data_package_alone_is_the_pure_part(self) -> None:
        self.assertEqual(
            build_bdist.plan([TABLE]),
            {
                "pure": "true",
                "compiled": "false",
                "stable_abi": "false",
                "trusted": "false",
            },
        )

    def test_a_dll_package_is_in_the_pure_part(self) -> None:
        # nothing in it is compiled either, whatever architecture it carries
        self.assertEqual(
            build_bdist.plan([WEBVIEW2_DLL]),
            {
                "pure": "true",
                "compiled": "false",
                "stable_abi": "false",
                "trusted": "true",
            },
        )

    def test_a_family_with_a_dll_package_is_still_one_part(self) -> None:
        plan = build_bdist.plan([XAML, WEBVIEW2_DLL])

        self.assertEqual((plan["compiled"], plan["stable_abi"]), ("false", "false"))

    def test_each_part_the_units_need(self) -> None:
        self.assertEqual(
            build_bdist.plan([RUNTIME, FOUNDATION, SYSTEM_INTEROP]),
            {
                "pure": "true",
                "compiled": "true",
                "stable_abi": "true",
                "trusted": "false",
            },
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

    def test_a_fix_released_on_its_own_still_follows_the_family(self) -> None:
        self.assertEqual(build_bdist.plan([UI_INTEROP_FIX])["trusted"], "true")

    def test_the_windows_sdk_units_are_on_the_token(self) -> None:
        for unit in [RUNTIME, TABLE, FOUNDATION, SYSTEM_INTEROP]:
            with self.subTest(unit=unit.release):
                self.assertEqual(build_bdist.plan([unit])["trusted"], "false")

    def test_units_on_both_sides_are_mixed(self) -> None:
        self.assertEqual(
            build_bdist.plan([RUNTIME, TABLE, FOUNDATION, XAML, WEBVIEW2_DLL]),
            {
                "pure": "true",
                "compiled": "true",
                "stable_abi": "false",
                "trusted": "mixed",
            },
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
            "pure=false\ncompiled=true\nstable_abi=false\ntrusted=false\n",
        )

    def build(self, *args: str) -> tuple[list[list[str]], str]:
        """
        The cibuildwheel arguments of each build that @p args asks for, which
        are not run, and what the script prints.
        """
        with (
            mock.patch.object(build_bdist.subprocess, "check_call") as check_call,
            mock.patch.object(build_bdist.shutil, "rmtree"),
        ):
            output = self.run_script(*args)

        return [call.args[0][2:] for call in check_call.call_args_list], output

    def test_the_stable_abi_part_is_built_per_architecture(self) -> None:
        commands, _ = self.build(
            "--stable-abi",
            "--package",
            "winrt-Windows.Media.Interop",
            "--platform",
            "windows",
            "--archs",
            "AMD64",
        )

        self.assertEqual(commands, [["--platform", "windows", "--archs", "AMD64"]])

    def test_the_abi3_wheel_serves_a_cpython_that_is_not_free_threaded(
        self,
    ) -> None:
        commands, output = self.build(
            "--stable-abi",
            "--package",
            "winrt-Windows.Media.Interop",
            "--only",
            "cp316-win_amd64",
        )

        self.assertEqual(commands, [])
        self.assertIn("its abi3 wheel serves cp316-win_amd64", output)

    def test_a_free_threaded_cpython_gets_a_wheel_of_its_own(self) -> None:
        commands, _ = self.build(
            "--stable-abi",
            "--package",
            "winrt-Windows.Media.Interop",
            "--only",
            "cp316t-win_amd64",
        )

        self.assertEqual(commands, [["--only", "cp316t-win_amd64"]])

    def test_the_compiled_part_builds_any_cpython(self) -> None:
        commands, _ = self.build(
            "--compiled", "--package", "winrt-runtime", "--only", "cp316-win_amd64"
        )

        self.assertEqual(commands, [["--only", "cp316-win_amd64"]])

    def test_a_tag_the_tree_does_not_publish_is_refused(self) -> None:
        with self.assertRaisesRegex(SystemExit, "releases nothing") as refused:
            self.run_script("--plan", "--tag", "wheels/runtime/v0")

        self.assertNotIsInstance(refused.exception.code, int)
