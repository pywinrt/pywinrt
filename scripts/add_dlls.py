"""
Add msvcp140.dll in to a .whl file (for use with cibuildwheel).

Not everyone has the VC++ runtime installed, so we need to include it in the
winrt-runtime wheel. All packages will be able to share this since they all
have their .pyd files in the same directory.
"""

import glob
import subprocess
import sys
import shutil
import os
import pathlib
import json

ARCH_MAP = {
    "win32": "x86",
    "amd64": "x64",
    "arm64": "arm64",
}

# absolute path to the wheel file
wheel = sys.argv[1]
# destination directory for repaired wheel
dest_dir = sys.argv[2]

# get architecture (win32, amd64, arm64)
wheel_arch = wheel.split(".")[-2].split("_")[-1]

subprocess.check_call(["wheel", "unpack", wheel])

# get name of extracted directory
wheel_dir = glob.glob("winrt_runtime-*")[0]

# find MSVC redist DLLs
vswhere = str(
    pathlib.Path(os.environ["ProgramFiles(x86)"])
    / "Microsoft Visual Studio"
    / "Installer"
    / "vswhere.exe"
)

# find the latest installed Visual Studio with the required VC tools
data = subprocess.check_output(
    [
        vswhere,
        "-format",
        "json",
        "-latest",
        "-products",
        "*",
        "-requires",
        f"Microsoft.VisualStudio.Component.VC.Tools.{"ARM64" if wheel_arch == "arm64" else "x86.x64"}",
    ]
)

properties: dict = json.loads(data)[0]

# get the path to msvcp140.dll
msvcp140_dll = glob.glob(
    properties["installationPath"]
    + f"/VC/Redist/MSVC/14.*/{ARCH_MAP[wheel_arch]}/Microsoft.*.CRT/msvcp140.dll",
)[0]

# put msvcp140.dll in the wheel
print("Adding", msvcp140_dll)
shutil.copy(msvcp140_dll, wheel_dir + "/winrt/")

subprocess.check_call(["wheel", "pack", wheel_dir, "-d", dest_dir])

shutil.rmtree(wheel_dir)
