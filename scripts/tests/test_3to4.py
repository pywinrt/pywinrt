"""
What scripts/3to4/inspect_source.py finds in code written for PyWinRT v3.

The script is run the way a user runs it, over a file written to a temporary
directory, and what it prints is compared line for line, so a hit that goes
missing and a hit that should not be there both fail.
"""

import csv
import re
import subprocess
import sys
import tempfile
import textwrap
import unittest
from pathlib import Path

SCRIPT = Path(__file__).parent.parent / "3to4" / "inspect_source.py"

SOURCE = textwrap.dedent(
    """\
    import winui3.microsoft.ui.xaml
    from webview2.microsoft.web.webview2.core import CoreWebView2
    from winrt.microsoft.web import webview2
    from winrt.system import Array, Int32

    services = device.get_gatt_services_with_cache_mode_async(mode)
    updater = manager.create_tile_updater_for_application("id")
    code = op.error_code.value
    cookie = self._changed_token.value
    kind = api_type.value
    a = Array("i", [1, 2])
    b = Array(Int32, [1, 2])
    items.insert(0, 1)
    window = winui3.microsoft.ui.xaml.Window()
    count = loop.run_until_complete(reader.load_async(4))
    """
)


def run(path: Path, *args: str) -> list[str]:
    result = subprocess.run(
        [sys.executable, SCRIPT, *args, path],
        capture_output=True,
        text=True,
        check=True,
    )

    return result.stdout.splitlines()


class Inspect(unittest.TestCase):
    def setUp(self):
        directory = tempfile.TemporaryDirectory()
        self.addCleanup(directory.cleanup)
        self.path = Path(directory.name) / "backend.py"
        self.path.write_bytes(SOURCE.encode())

    def test_report(self):
        p = self.path

        self.assertEqual(
            run(p),
            [
                f"{p}:1:8",
                "possible match: winui3.microsoft.ui.xaml",
                "rename to: winrt.microsoft.ui.xaml",
                f"{p}:2:1",
                "possible match: webview2.microsoft.web.webview2.core",
                "rename to: winrt.microsoft.web.webview2.core",
                f"{p}:6:19",
                "possible match: winrt.windows.devices.bluetooth.BluetoothLEDevice"
                ".get_gatt_services_with_cache_mode_async",
                "rename to: get_gatt_services_async",
                f"{p}:7:19",
                "possible match: winrt.windows.ui.notifications.TileUpdateManagerForUser"
                ".create_tile_updater_for_application",
                "rename to: create_tile_updater_for_application_for_user"
                " (v4 uses the old name for something else)",
                f"{p}:8:22",
                "possible match: winrt.windows.foundation.HResult.value",
                "rename to: error_code, without .value: an HResult is an int",
                f"{p}:9:30",
                "possible match: winrt.windows.foundation.EventRegistrationToken.value",
                "rename to: _changed_token, without .value:"
                " an EventRegistrationToken is an int",
                f"{p}:11:11",
                'possible match: winrt.system.Array("i", ...)',
                "rename to: winrt.system.Int32, or the enum type for an array of enums",
                f"{p}:15:14",
                "possible match: run_until_complete() of a WinRT async operation",
                "rename to: asyncio.run() of a coroutine that awaits the operation",
            ],
        )

    def test_fix_rewrites_the_packages_only(self):
        self.assertEqual(
            run(self.path, "--fix")[-1], f"{self.path}: rewrote 3 package names"
        )

        self.assertEqual(
            self.path.read_bytes().decode(),
            SOURCE.replace("import winui3.", "import winrt.")
            .replace("from webview2.", "from winrt.")
            .replace("= winui3.", "= winrt."),
        )


def floor(distribution: str) -> str:
    """
    The floor distributions.csv gives the v4 @p distribution.
    """
    for row in DISTRIBUTIONS:
        if row["new_distribution"] == distribution:
            return row["floor"]

    raise KeyError(distribution)


TABLE_PATH = SCRIPT.parent / "distributions.csv"

DISTRIBUTIONS = list(csv.DictReader(TABLE_PATH.read_text().splitlines()))

REQUIREMENTS = textwrap.dedent(
    """\
    winui3-Microsoft.UI.Xaml>=3.2,<3.3
    winui3-Microsoft.UI.Xaml.Controls>=3.2,<3.3
    winui3-Microsoft.UI.Windowing[all]>=3.2,<3.3 ; sys_platform == "win32"
    winrt-Windows.Foundation~=3.2.1
    winrt-Windows.Devices.Bluetooth>=3.2
    winrt-runtime<4.1
    winrt-sdk
    bleak>=1
    # winui3-Microsoft.UI.Composition>=3.2
    """
)

SETUP = textwrap.dedent(
    """\
    # webview2-Microsoft.Web.WebView2.Core
    extras = {"webview2": ["webview2-Microsoft.Web.WebView2.Core==3.2.1"]}
    """
)


class Requirements(unittest.TestCase):
    def setUp(self):
        directory = tempfile.TemporaryDirectory()
        self.addCleanup(directory.cleanup)
        self.directory = Path(directory.name)

    def write(self, name: str, text: str) -> Path:
        path = self.directory / name
        path.write_bytes(text.encode())
        return path

    def test_report(self):
        p = self.write("requirements.txt", REQUIREMENTS)
        winui = floor("winrt-Microsoft.WindowsAppSDK.WinUI")
        interactive = floor("winrt-Microsoft.WindowsAppSDK.InteractiveExperiences")

        self.assertEqual(
            run(p),
            [
                f"{p}:1:1",
                "possible match: winui3-Microsoft.UI.Xaml>=3.2,<3.3",
                f"rename to: winrt-Microsoft.WindowsAppSDK.WinUI{winui}",
                f"{p}:2:1",
                "possible match: winui3-Microsoft.UI.Xaml.Controls>=3.2,<3.3",
                f"rename to: winrt-Microsoft.WindowsAppSDK.WinUI{winui}",
                f"{p}:3:1",
                "possible match: winui3-Microsoft.UI.Windowing[all]>=3.2,<3.3",
                "rename to: winrt-Microsoft.WindowsAppSDK.InteractiveExperiences[all]"
                f"{interactive}",
                f"{p}:4:1",
                "possible match: winrt-Windows.Foundation~=3.2.1",
                f"rename to: winrt-Windows.Foundation{floor('winrt-Windows.Foundation')}",
                f"{p}:7:1",
                "possible match: winrt-sdk",
                "rename to: nothing: there is no v4 distribution,"
                " see scripts/3to4/README.md",
            ],
        )

    def test_fix(self):
        p = self.write("requirements.txt", REQUIREMENTS)
        winui = floor("winrt-Microsoft.WindowsAppSDK.WinUI")
        interactive = floor("winrt-Microsoft.WindowsAppSDK.InteractiveExperiences")
        foundation = floor("winrt-Windows.Foundation")

        self.assertEqual(
            run(p, "--fix")[-1],
            f"{p}: rewrote 4 requirements, dropped 1 that became duplicates",
        )

        self.assertEqual(
            p.read_bytes().decode(),
            textwrap.dedent(
                f"""\
                winrt-Microsoft.WindowsAppSDK.WinUI{winui}
                winrt-Microsoft.WindowsAppSDK.InteractiveExperiences[all]{interactive} ; sys_platform == "win32"
                winrt-Windows.Foundation{foundation}
                winrt-Windows.Devices.Bluetooth>=3.2
                winrt-runtime<4.1
                winrt-sdk
                bleak>=1
                # winui3-Microsoft.UI.Composition>=3.2
                """
            ),
        )

    def test_fix_merges_a_toml_array(self):
        p = self.write(
            "pyproject.toml",
            textwrap.dedent(
                """\
                [project.optional-dependencies]
                winui3 = [
                    "winui3-Microsoft.UI.Xaml>=3.2,<3.3;sys_platform=='win32'",
                    "winui3-Microsoft.UI.Xaml.Controls>=3.2,<3.3;sys_platform=='win32'",
                ]
                other = ["winui3-Microsoft.UI.Xaml>=3.2,<3.3"]
                """
            ),
        )
        winui = floor("winrt-Microsoft.WindowsAppSDK.WinUI")

        run(p, "--fix")

        # the one-line array of another extra is not a duplicate of the first
        self.assertEqual(
            p.read_bytes().decode(),
            textwrap.dedent(
                f"""\
                [project.optional-dependencies]
                winui3 = [
                    "winrt-Microsoft.WindowsAppSDK.WinUI{winui};sys_platform=='win32'",
                ]
                other = ["winrt-Microsoft.WindowsAppSDK.WinUI{winui}"]
                """
            ),
        )

    def test_python_strings_only(self):
        p = self.write("setup.py", SETUP)
        webview2 = floor("winrt-Microsoft.Web.WebView2")

        self.assertEqual(
            run(p),
            [
                f"{p}:2:25",
                "possible match: webview2-Microsoft.Web.WebView2.Core==3.2.1",
                f"rename to: winrt-Microsoft.Web.WebView2{webview2}",
            ],
        )

        run(p, "--fix")

        self.assertEqual(
            p.read_bytes().decode(),
            SETUP.replace(
                '"webview2-Microsoft.Web.WebView2.Core==3.2.1"',
                f'"winrt-Microsoft.Web.WebView2{webview2}"',
            ),
        )


class Tables(unittest.TestCase):
    """
    distributions.csv against the tree and against the README's tables, which
    are written by hand.
    """

    def test_every_new_distribution_is_in_the_tree(self):
        repo = SCRIPT.parent.parent.parent
        names = set()

        for pattern in (
            "projection/*/*/pyproject.toml",
            "interop/*/pyproject.toml",
            "runtime/pyproject.toml",
        ):
            for path in repo.glob(pattern):
                found = re.search(r'^name = "(.*)"$', path.read_text(), re.MULTILINE)
                assert found is not None
                names.add(found[1])

        for row in DISTRIBUTIONS:
            if row["new_distribution"]:
                with self.subTest(row["distribution"]):
                    self.assertIn(row["new_distribution"], names)

    def test_readme_agrees(self):
        readme = (SCRIPT.parent / "README.md").read_text()
        # | `a` ... | `b` | rows of the three tables of renamed distributions
        rows = [
            (re.findall(r"`([^`]+)`", first), second)
            for first, second in re.findall(
                r"^\| (.*?) +\| `([^`]+)` +\|$", readme, re.MULTILINE
            )
        ]
        by_distribution = {names[0]: new for names, new in rows if len(names) == 1}

        for row in DISTRIBUTIONS:
            old, new = row["distribution"], row["new_distribution"]
            prefix, _, namespace = old.partition("-")

            if old == new or prefix == "winrt":
                continue

            with self.subTest(old):
                if old in by_distribution:
                    self.assertEqual(by_distribution[old], new)
                elif prefix == "webview2":
                    self.assertIn(f"`{old}` is now `{new}`", readme)
                else:
                    # the longest pattern that matches the namespace wins, so
                    # a row can take the exception to a wildcard above it
                    matches = [
                        (len(pattern), row_new)
                        for patterns, row_new in rows
                        for pattern in patterns
                        if namespace == pattern
                        or (
                            pattern.endswith("*") and namespace.startswith(pattern[:-1])
                        )
                    ]
                    self.assertTrue(matches, "not in the README")
                    self.assertEqual(max(matches)[1], new)
