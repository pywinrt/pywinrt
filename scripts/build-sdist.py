import subprocess
from glob import iglob
from itertools import chain
from pathlib import Path

PROJECTION_PATH = (Path(__file__).parent.parent / "projection").resolve()

try:
    import winrt_sdk  # noqa: F401
except ImportError:
    print("winrt_sdk must be installed first installed in order for build to succeed.")
    exit(1)


for package_path in chain(
    [str(PROJECTION_PATH / "winrt-runtime")],
    iglob(str(PROJECTION_PATH / "interop" / "winrt-Windows*")),
    iglob(str(PROJECTION_PATH / "winrt-Windows*")),
):
    subprocess.check_call(
        [
            "pyproject-build",
            "--sdist",
            "--no-isolation",
            "--skip-dependency-check",
            "--outdir",
            "wheelhouse",
            package_path,
        ],
    )
