"""
Add msvcp140.dll in to a .whl file (for use with cibuildwheel).

Not everyone has the VC++ runtime installed, so we need to include it in the
winrt-runtime wheel. All packages will be able to share this since they all
have their .pyd files in the same directory.
"""

import json
import os
import pathlib
import shutil
import subprocess
import sys

ARCH_MAP = {
    "win32": "x86",
    "win_amd64": "x64",
    "win_arm64": "arm64",
}


def parse_redist_version(
    path: pathlib.Path, redist_root: pathlib.Path
) -> tuple[int, ...]:
    version = path.relative_to(redist_root).parts[0]

    try:
        return tuple(int(component) for component in version.split("."))
    except ValueError as error:
        raise ValueError(
            f"Invalid MSVC redistributable version directory: {version}"
        ) from error


def find_msvcp140(
    installation_path: str, architecture: str
) -> pathlib.Path:
    redist_root = (
        pathlib.Path(installation_path) / "VC" / "Redist" / "MSVC"
    )
    candidates = sorted(
        redist_root.glob(
            f"14.*/{architecture}/Microsoft.*.CRT/msvcp140.dll"
        )
    )

    print(f"Searching for MSVCP140.dll for architecture {architecture}")
    print(f"MSVC redistributable root: {redist_root}")
    for candidate in candidates:
        print(f"MSVC redistributable candidate: {candidate}")

    if not candidates:
        raise FileNotFoundError(
            f"No MSVCP140.dll found for architecture {architecture} "
            f"under {redist_root}"
        )

    try:
        selected = max(
            candidates,
            key=lambda path: parse_redist_version(path, redist_root),
        )
    except ValueError as error:
        raise ValueError(
            f"{error} while searching for architecture {architecture} "
            f"under {redist_root}"
        ) from error

    print(f"Selected MSVC redistributable: {selected}")
    return selected


def main(wheel: str, dest_dir: str) -> None:
    wheel_arch = wheel.split(".")[-2].split("-")[-1]

    subprocess.check_call(["wheel", "unpack", wheel])

    wheel_dirs = list(pathlib.Path.cwd().glob("winrt_runtime-*"))
    if len(wheel_dirs) != 1:
        raise RuntimeError(
            "Expected exactly one unpacked winrt-runtime wheel directory, "
            f"found {len(wheel_dirs)}"
        )
    wheel_dir = wheel_dirs[0]

    try:
        vswhere = os.fspath(
            pathlib.Path(os.environ["ProgramFiles(x86)"])
            / "Microsoft Visual Studio"
            / "Installer"
            / "vswhere.exe"
        )

        data = subprocess.check_output(
            [
                vswhere,
                "-format",
                "json",
                "-latest",
                "-products",
                "*",
                "-requires",
                "Microsoft.VisualStudio.Component.VC.Tools."
                + ("ARM64" if wheel_arch == "win_arm64" else "x86.x64"),
            ]
        )

        properties: dict[str, str] = json.loads(data)[0]
        msvcp140_dll = find_msvcp140(
            properties["installationPath"], ARCH_MAP[wheel_arch]
        )

        print(f"Adding {msvcp140_dll} to {wheel_dir}")
        shutil.copy(msvcp140_dll, wheel_dir / "winrt")

        subprocess.check_call(
            ["wheel", "pack", os.fspath(wheel_dir), "-d", dest_dir]
        )
    finally:
        shutil.rmtree(wheel_dir)


if __name__ == "__main__":
    if len(sys.argv) != 3:
        raise SystemExit(
            "usage: add_msvcp140_dll.py <wheel> <destination-directory>"
        )

    main(sys.argv[1], sys.argv[2])
