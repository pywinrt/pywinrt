import pathlib
import subprocess
import sys

REPO_ROOT_PATH = pathlib.Path(__file__).parent.parent.resolve()
PROJECTION_PATH = REPO_ROOT_PATH / "projection"

if "--nuget" in sys.argv:
    PYWINRT_EXE = REPO_ROOT_PATH / "_tools" / "PyWinRT" / "bin" / "pywinrt.exe"
elif "--debug" in sys.argv:
    PYWINRT_EXE = (
        REPO_ROOT_PATH / "_build" / "Windows" / "x86" / "Debug" / "pywinrt.exe"
    )
else:
    PYWINRT_EXE = (
        REPO_ROOT_PATH / "_build" / "Windows" / "x86" / "Release" / "pywinrt.exe"
    )

if not PYWINRT_EXE.exists():
    raise RuntimeError("pywinrt.exe not found. Please run `./scripts/fetch-tools.cmd`")


# generate code for windows sdk

MINIMAL_NAMESPACES = [
    "Windows.Data.Json",
    "Windows.Devices.Geolocation",
    "Windows.Foundation",
    "Windows.Graphics.Capture",
    "Windows.Graphics.DirectX",
    "Windows.Storage.Streams",
]
WINDOWS_SDK = "10.0.22621.0+"
SDK_PACKAGE_PATH = PROJECTION_PATH / "winrt-sdk" / "src" / "winrt_sdk" / "pywinrt"

include_args = []

if "--minimal" in sys.argv:
    for ns in MINIMAL_NAMESPACES:
        include_args.extend(["-include", ns])

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
    + include_args
)

# generate code for test component

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
