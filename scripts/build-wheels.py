import subprocess
from glob import glob
from itertools import chain
from pathlib import Path


PROJECTION_PATH = (Path(__file__).parent.parent / "projection").resolve()


for package in chain(
    ["winrt-runtime"],
    glob("interop/winrt-Windows*", root_dir=str(PROJECTION_PATH)),
    glob("winrt-Windows*", root_dir=str(PROJECTION_PATH)),
):
    subprocess.check_call(
        [
            "cibuildwheel",
            "--only",
            "cp312-win_arm64",
            str(PROJECTION_PATH / package),
        ]
    )
