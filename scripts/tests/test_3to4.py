"""
What scripts/3to4/inspect_source.py finds in code written for PyWinRT v3.

The script is run the way a user runs it, over a file written to a temporary
directory, and what it prints is compared line for line, so a hit that goes
missing and a hit that should not be there both fail.
"""

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
