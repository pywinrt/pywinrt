import pathlib
import subprocess
import sys

REPO_ROOT_PATH = pathlib.Path(__file__).parent.parent.resolve()
PROJECTION_PATH = REPO_ROOT_PATH / "projection"
PYWINRT_EXE = (
    (REPO_ROOT_PATH / "_build" / "Windows" / "x86" / "Debug" / "pywinrt.exe")
    if "--debug" in sys.argv
    else (REPO_ROOT_PATH / "_tools" / "PyWinRT" / "bin" / "pywinrt.exe")
)

if not PYWINRT_EXE.exists():
    raise RuntimeError("pywinrt.exe not found. Please run `./scripts/fetch-tools.cmd`")


# generate headers for windows sdk

WINDOWS_SDK = "10.0.22621.0+"
SDK_PACKAGE_PATH = PROJECTION_PATH / "winrt-sdk" / "pywinrt"

subprocess.check_call(
    [
        PYWINRT_EXE,
        "-input",
        WINDOWS_SDK,
        "-output",
        PROJECTION_PATH,
        "-header-path",
        SDK_PACKAGE_PATH,
    ]
)

raise SystemExit

# generate headers for test component

TEST_PACKAGE_METADATA = (
    REPO_ROOT_PATH
    / "_tools"
    / "KennyKerr.Windows.TestWinRT"
    / "lib"
    / "uap10.0"
    / "TestComponent.winmd"
)

subprocess.check_call(
    [
        PYWINRT_EXE,
        "-input",
        TEST_PACKAGE_METADATA,
        "-reference",
        WINDOWS_SDK,
        "-output",
        PROJECTION_PATH,
    ]
)
