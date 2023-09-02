import shutil
import subprocess
import sys
from itertools import chain
from pathlib import Path

PROJECTION_PATH = (Path(__file__).parent.parent / "projection").resolve()


for package_path in chain(
    [PROJECTION_PATH / "winrt-runtime"],
    (PROJECTION_PATH / "interop").glob("winrt-Windows*"),
    PROJECTION_PATH.glob("winrt-Windows*"),
    PROJECTION_PATH.glob("winrt-Microsoft*"),
):
    subprocess.check_call(
        [
            "cibuildwheel",
            str(package_path),
        ]
        + (sys.argv[1:] or ["--platform", "windows"])
    )

    # Build directories are 10s of MBs and will cause CI to run out of space!
    shutil.rmtree(package_path / "build")
