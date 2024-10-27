"""
Add Microsoft.Web.WebView2.Core.dll in to a .whl file (for use with cibuildwheel).
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
wheel_dir = glob.glob("winrt_Microsoft.Web.WebView2.Core-*")[0]

dll = str(
    pathlib.Path(os.environ["WEBVIEW2_PATH"])
    / "runtimes"
    / f"win-{ARCH_MAP[wheel_arch]}"
    / "native_uap"
    / "Microsoft.Web.WebView2.Core.dll"
)

# put .dll in the wheel
print("Adding", dll)
shutil.copy(dll, wheel_dir + "/winrt/microsoft/web/webview2/core/")

subprocess.check_call(["wheel", "pack", wheel_dir, "-d", dest_dir])

shutil.rmtree(wheel_dir)
