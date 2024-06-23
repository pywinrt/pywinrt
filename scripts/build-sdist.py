import os
import subprocess
from glob import iglob
from itertools import chain
from pathlib import Path

PROJECT_DIR = Path(__file__).parent.parent
PROJECTION_PATH = (PROJECT_DIR / "projection").resolve()
WINDOWS_APP_SDK_PATH = (PROJECT_DIR / "_tools/Microsoft.WindowsAppSDK").resolve()

os.environ["WINDOWS_APP_SDK_PATH"] = str(WINDOWS_APP_SDK_PATH)


try:
    import winrt_sdk  # noqa: F401
    import winrt_windows_app_sdk  # noqa: F401
except ImportError:
    print(
        "winrt-sdk and winrt-WindowsAppSDK must be installed first installed in order for build to succeed."
    )
    exit(1)


for package_path in chain(
    [str(PROJECTION_PATH / "winrt-runtime")],
    iglob(str(PROJECTION_PATH / "interop" / "winrt-*")),
    iglob(str(PROJECTION_PATH / "winrt-Windows.*")),
    iglob(str(PROJECTION_PATH / "winrt-Microsoft.*")),
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
