import subprocess
from glob import iglob
from itertools import chain
from pathlib import Path

PROJECTION_PATH = (Path(__file__).parent.parent / "projection").resolve()


for package_path in chain(
    [str(PROJECTION_PATH / "winrt-runtime")],
    iglob(str(PROJECTION_PATH / "interop" / "winrt-Windows*")),
    iglob(str(PROJECTION_PATH / "winrt-Windows*")),
):
    subprocess.check_call(
        [
            "cibuildwheel",
            "--only",
            "cp312-win_arm64",
            package_path,
        ]
    )
