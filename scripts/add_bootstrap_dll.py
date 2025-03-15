"""
Add Microsoft.WindowsAppRuntime.Bootstrap.dll in to a .whl file (for use with
cibuildwheel).
"""

import glob
import pathlib
import os
import subprocess
import sys
import shutil

ARCH_MAP = {
    "win32": "x86",
    "win_amd64": "x64",
    "win_arm64": "arm64",
}

# absolute path to the wheel file
wheel = sys.argv[1]
# destination directory for repaired wheel
dest_dir = sys.argv[2]

# get architecture (win32, win_amd64, win_arm64)
wheel_arch = wheel.split(".")[-2].split("-")[-1]

subprocess.check_call(["wheel", "unpack", wheel])

# get name of extracted directory
wheel_dir = glob.glob(
    "winui3-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap-*"
)[0]

bootstrap_dll = os.fspath(
    pathlib.Path(os.environ["WINDOWS_APP_SDK_PATH"])
    / "runtimes"
    / f"win-{ARCH_MAP[wheel_arch]}"
    / "native"
    / "Microsoft.WindowsAppRuntime.Bootstrap.dll"
)

# put .dll in the wheel
print("Adding", bootstrap_dll)
shutil.copy(bootstrap_dll, wheel_dir + "/winui3/")

subprocess.check_call(["wheel", "pack", wheel_dir, "-d", dest_dir])

shutil.rmtree(wheel_dir)
