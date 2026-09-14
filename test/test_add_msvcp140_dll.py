import re
import tempfile
import unittest
from pathlib import Path

from scripts.add_msvcp140_dll import find_msvcp140


class TestFindMsvcp140(unittest.TestCase):
    @staticmethod
    def create_runtime(
        installation_path: Path,
        version: str,
        architecture: str,
        crt: str,
    ) -> Path:
        dll = (
            installation_path
            / "VC"
            / "Redist"
            / "MSVC"
            / version
            / architecture
            / crt
            / "msvcp140.dll"
        )
        dll.parent.mkdir(parents=True)
        dll.touch()
        return dll

    def assert_newest_selected(self, versions: list[str]) -> None:
        with tempfile.TemporaryDirectory() as directory:
            installation_path = Path(directory)
            dlls = {
                version: self.create_runtime(
                    installation_path,
                    version,
                    "x64",
                    "Microsoft.VC143.CRT",
                )
                for version in versions
            }

            selected = find_msvcp140(
                str(installation_path),
                "x64",
            )

            self.assertEqual(selected, dlls["14.44.35211"])

    def test_newest_version_wins_when_created_first(self):
        self.assert_newest_selected(["14.44.35211", "14.29.30133"])

    def test_newest_version_wins_when_created_last(self):
        self.assert_newest_selected(["14.29.30133", "14.44.35211"])

    def test_versions_are_compared_numerically(self):
        with tempfile.TemporaryDirectory() as directory:
            installation_path = Path(directory)
            self.create_runtime(
                installation_path,
                "14.9.0",
                "x64",
                "Microsoft.VC143.CRT",
            )
            newest = self.create_runtime(
                installation_path,
                "14.10.0",
                "x64",
                "Microsoft.VC143.CRT",
            )

            selected = find_msvcp140(str(installation_path), "x64")

            self.assertEqual(selected, newest)

    def test_only_requested_architecture_is_selected(self):
        with tempfile.TemporaryDirectory() as directory:
            installation_path = Path(directory)
            expected = self.create_runtime(
                installation_path,
                "14.44.35211",
                "x64",
                "Microsoft.VC143.CRT",
            )
            self.create_runtime(
                installation_path,
                "14.50.0",
                "x86",
                "Microsoft.VC143.CRT",
            )
            self.create_runtime(
                installation_path,
                "14.51.0",
                "arm64",
                "Microsoft.VC143.CRT",
            )

            selected = find_msvcp140(str(installation_path), "x64")

            self.assertEqual(selected, expected)

    def test_missing_runtime_reports_architecture_and_root(self):
        with tempfile.TemporaryDirectory() as directory:
            installation_path = Path(directory)
            redist_root = (
                installation_path / "VC" / "Redist" / "MSVC"
            )

            with self.assertRaisesRegex(
                FileNotFoundError,
                rf"x64.*{re.escape(str(redist_root))}",
            ):
                find_msvcp140(str(installation_path), "x64")

    def test_malformed_version_is_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            installation_path = Path(directory)
            redist_root = (
                installation_path / "VC" / "Redist" / "MSVC"
            )
            self.create_runtime(
                installation_path,
                "14.invalid",
                "x64",
                "Microsoft.VC143.CRT",
            )

            with self.assertRaisesRegex(
                ValueError,
                rf"14\.invalid.*x64.*{re.escape(str(redist_root))}",
            ):
                find_msvcp140(str(installation_path), "x64")


if __name__ == "__main__":
    unittest.main()
