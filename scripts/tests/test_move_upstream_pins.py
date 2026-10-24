"""
What scripts/move-upstream-pins.py finds moved, writes and deletes.

Nothing here fetches, generates or asks the network: the App SDK's resolution
is put in place of NuGet's answer, and the files a moved pin deletes are made
in a temporary directory put in place of the tree.
"""

import importlib.util
import sys
import tempfile
import unittest
from pathlib import Path
from types import ModuleType
from unittest import mock

SCRIPTS = Path(__file__).parent.parent
SCRIPT = SCRIPTS / "move-upstream-pins.py"


def load_script() -> ModuleType:
    spec = importlib.util.spec_from_file_location("move_upstream_pins", SCRIPT)
    assert spec is not None and spec.loader is not None
    module = importlib.util.module_from_spec(spec)

    # the script imports the modules beside it by their bare names
    sys.path.insert(0, str(SCRIPTS))

    try:
        spec.loader.exec_module(module)
    finally:
        sys.path.remove(str(SCRIPTS))

    return module


move_upstream_pins = load_script()
versions = move_upstream_pins.versions
app_sdk = move_upstream_pins.app_sdk

TOOLS = {
    "Microsoft.UI.Xaml": "2.8.7",
    "Microsoft.Web.WebView2": "1.0.4191.47",
    "Microsoft.Windows.AI.MachineLearning": "2.1.74",
    "Microsoft.Windows.CppWinRT": "3.0.260818.1",
    "Microsoft.Windows.SDK.CPP": "10.0.28000.2705",
    "Microsoft.WindowsAppSDK": "2.5.1",
    "Microsoft.WindowsAppSDK.Foundation": "2.3.12",
    "Microsoft.WindowsAppSDK.Runtime": "2.5.1",
    "Microsoft.WindowsAppSDK.Widgets": "2.0.5",
    "PyWinRT.TestWinRT": "1.6.1",
}

PINS = {
    "winrt": "10.0.28000.2705",
    "winui2": "2.8.7",
    "wasdk": "2.5.1",
    "webview2": "1.0.4191.47",
}


class MovedPins(unittest.TestCase):
    def test_nothing_moved(self) -> None:
        self.assertEqual(move_upstream_pins.moved_pins(PINS, TOOLS), {})

    def test_a_moved_pin_is_found(self) -> None:
        pins = PINS | {"webview2": "1.0.4314.35"}

        self.assertEqual(
            move_upstream_pins.moved_pins(pins, TOOLS), {"webview2": "1.0.4314.35"}
        )

    def test_the_app_sdk_is_compared_by_its_metapackage(self) -> None:
        # a component is not a pin, so a copy whose components are behind
        # moves nothing
        tools = TOOLS | {"Microsoft.WindowsAppSDK.Foundation": "2.3.11"}

        self.assertEqual(move_upstream_pins.moved_pins(PINS, tools), {})


class ToolsWithPins(unittest.TestCase):
    def test_a_pin_is_copied_and_nothing_else_changes(self) -> None:
        tools = move_upstream_pins.tools_with_pins(TOOLS, {"webview2": "1.0.4314.35"})

        self.assertEqual(tools, TOOLS | {"Microsoft.Web.WebView2": "1.0.4314.35"})
        self.assertEqual(list(tools), sorted(tools))

    def test_the_app_sdk_takes_the_components_of_its_release(self) -> None:
        # the release dropped Widgets and moved the rest
        resolved = {
            "Microsoft.Windows.AI.MachineLearning": "2.2.0",
            "Microsoft.WindowsAppSDK": "2.6.0",
            "Microsoft.WindowsAppSDK.Foundation": "2.4.0",
            "Microsoft.WindowsAppSDK.Runtime": "2.6.0",
        }

        with mock.patch.object(app_sdk, "resolve", return_value=resolved) as resolve:
            tools = move_upstream_pins.tools_with_pins(TOOLS, {"wasdk": "2.6.0"})

        resolve.assert_called_once_with("2.6.0")
        self.assertEqual(
            tools,
            {
                package: version
                for package, version in TOOLS.items()
                if not package.startswith("Microsoft.WindowsAppSDK")
            }
            | resolved,
        )
        self.assertEqual(list(tools), sorted(tools))


class StaleFixes(unittest.TestCase):
    def setUp(self) -> None:
        root = Path(self.enterContext(tempfile.TemporaryDirectory()))
        self.projection = root / "projection"
        self.interop = root / "interop"
        self.enterContext(
            mock.patch.object(versions, "PROJECTION_PATH", self.projection)
        )
        self.enterContext(mock.patch.object(versions, "INTEROP_PATH", self.interop))

    def test_a_family_loses_its_fixes(self) -> None:
        self.assertEqual(
            move_upstream_pins.stale_fixes("webview2"),
            [self.projection / "webview2" / "fixes.json"],
        )

    def test_the_app_sdk_loses_the_fixes_of_its_interop_modules_too(self) -> None:
        self.assertEqual(
            move_upstream_pins.stale_fixes("wasdk"),
            [
                self.projection / "wasdk" / "fixes.json",
                self.interop / "winrt-Microsoft.UI.Interop" / "version.txt",
                self.interop
                / "winrt-Microsoft.WindowsAppRuntime.Bootstrap"
                / "version.txt",
            ],
        )


class FamilyReport(unittest.TestCase):
    def setUp(self) -> None:
        self.enterContext(
            mock.patch.object(
                versions,
                "nuget_versions",
                return_value={"winrt": "10.0.29000.1", "webview2": "1.0.4314.35"},
            )
        )
        self.enterContext(
            mock.patch.object(
                versions,
                "family_versions",
                return_value={"winrt": "4!10.0.29000.1", "webview2": "4!1.0.4314.35"},
            )
        )

    def test_names_the_tag(self) -> None:
        lines = move_upstream_pins.family_report(
            "webview2", "1.0.4191.47", set(), set(), set()
        )

        self.assertIn("## Microsoft.Web.WebView2 1.0.4191.47 → 1.0.4314.35", lines)
        self.assertIn(
            "Released as `4!1.0.4314.35` by the tag `wheels/webview2/e4v1.0.4314.35`.",
            lines,
        )

    def test_new_names_of_a_token_family_take_days(self) -> None:
        added = {f"winrt-Windows.New{n}" for n in range(9)}
        lines = move_upstream_pins.family_report(
            "winrt", "10.0.28000.2705", added, set(), added
        )

        self.assertIn("- Not on PyPI yet: 9", lines)
        self.assertTrue(any("takes about 3 days" in line for line in lines))

    def test_new_names_of_a_trusted_family_need_pending_publishers(self) -> None:
        added = {"winrt-Microsoft.Web.WebView2.New"}
        lines = move_upstream_pins.family_report(
            "webview2", "1.0.4191.47", added, set(), added
        )

        self.assertTrue(any("pending publisher" in line for line in lines))
