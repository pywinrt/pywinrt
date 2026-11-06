"""
What scripts/check-wheel-contents.py finds wrong with a wheel.

Each wheel here is a few files zipped into a temporary directory: the core
metadata, the license texts, and, for one that redistributes a binary, the
binary and a bill of materials naming it.
"""

import importlib.util
import json
import tempfile
import unittest
import zipfile
from pathlib import Path
from types import ModuleType
from typing import Any

SCRIPT = Path(__file__).parent.parent / "check-wheel-contents.py"


def load_script() -> ModuleType:
    spec = importlib.util.spec_from_file_location("check_wheel_contents", SCRIPT)
    assert spec is not None and spec.loader is not None
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


check_wheel_contents = load_script()


def sbom(version: str | None) -> dict[str, Any]:
    """
    A bill of materials that names the distribution, at @p version unless it
    is None, and the one binary it redistributes.
    """
    component: dict[str, Any] = {"type": "library", "name": "winrt-Example"}

    if version is not None:
        component["version"] = version

    return {
        "bomFormat": "CycloneDX",
        "metadata": {"component": component},
        "components": [{"type": "library", "name": "Example.dll"}],
    }


class Sbom(unittest.TestCase):
    def setUp(self) -> None:
        directory = tempfile.TemporaryDirectory()
        self.addCleanup(directory.cleanup)
        self.directory = Path(directory.name)

    def wheel(self, sbom_version: str | None) -> Path:
        """
        A wheel of version 1.0 that redistributes Example.dll, with a bill of
        materials that gives the distribution @p sbom_version.
        """
        path = self.directory / "winrt_example-1.0-py3-none-win_amd64.whl"
        info = "winrt_example-1.0.dist-info"

        with zipfile.ZipFile(path, "w") as wheel:
            wheel.writestr(f"{info}/METADATA", "Name: winrt-Example\nVersion: 1.0\n")
            wheel.writestr(f"{info}/licenses/LICENSE", "")
            wheel.writestr(f"{info}/licenses/Example.LICENSE", "")
            wheel.writestr(
                f"{info}/sboms/example.cdx.json", json.dumps(sbom(sbom_version))
            )
            wheel.writestr("winrt/example/Example.dll", b"")

        return path

    def test_the_version_of_the_wheel(self) -> None:
        self.assertEqual(check_wheel_contents.check_wheel(self.wheel("1.0")), [])

    def test_another_version(self) -> None:
        self.assertEqual(
            check_wheel_contents.check_wheel(self.wheel("0.9")),
            ["is 1.0 but its bill of materials says it is 0.9"],
        )

    def test_no_version(self) -> None:
        self.assertEqual(
            check_wheel_contents.check_wheel(self.wheel(None)),
            ["has a bill of materials that gives it no version"],
        )


class StableAbi(unittest.TestCase):
    def setUp(self) -> None:
        directory = tempfile.TemporaryDirectory()
        self.addCleanup(directory.cleanup)
        self.directory = Path(directory.name)

    def wheel(self, tag: str, module: bytes) -> Path:
        """
        A wheel tagged @p tag whose one extension module is @p module.
        """
        path = self.directory / f"winrt_example-1.0-{tag}-win_amd64.whl"
        info = "winrt_example-1.0.dist-info"

        with zipfile.ZipFile(path, "w") as wheel:
            wheel.writestr(f"{info}/METADATA", "Name: winrt-Example\nVersion: 1.0\n")
            wheel.writestr(f"{info}/licenses/LICENSE", "")
            wheel.writestr("winrt/_winrt_example.pyd", module)

        return path

    def test_an_abi3_module_that_links_the_stable_abi(self) -> None:
        self.assertEqual(
            check_wheel_contents.check_wheel(
                self.wheel("cp311-abi3", b"MZ ... python3.dll ...")
            ),
            [],
        )

    def test_an_abi3_module_that_links_a_version(self) -> None:
        self.assertEqual(
            check_wheel_contents.check_wheel(
                self.wheel("cp311-abi3", b"MZ ... python311.dll ...")
            ),
            [
                "is abi3 but its _winrt_example.pyd links python311.dll",
                "is abi3 but its _winrt_example.pyd does not link python3.dll",
            ],
        )

    def test_a_module_of_one_version_may_link_it(self) -> None:
        self.assertEqual(
            check_wheel_contents.check_wheel(
                self.wheel("cp314t-cp314t", b"MZ ... python314t.dll ...")
            ),
            [],
        )
