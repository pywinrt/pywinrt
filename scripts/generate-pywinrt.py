import itertools
import pathlib
import shutil
import subprocess
import sys

REPO_ROOT_PATH = pathlib.Path(__file__).parent.parent.resolve()
PROJECTION_PATH = REPO_ROOT_PATH / "projection"

if "--nuget" in sys.argv:
    PYWINRT_EXE = REPO_ROOT_PATH / "_tools" / "PyWinRT" / "bin" / "pywinrt.exe"

    if not PYWINRT_EXE.exists():
        raise RuntimeError(
            "pywinrt.exe not found. Please run `./scripts/fetch-tools.cmd`"
        )
elif "--debug" in sys.argv:
    PYWINRT_EXE = (
        REPO_ROOT_PATH / "_build" / "Windows" / "x86" / "Debug" / "pywinrt.exe"
    )

    if not PYWINRT_EXE.exists():
        raise RuntimeError(
            "pywinrt.exe not found. Please run `./vsdevcmd-build.cmd --build-type Debug`"
        )
else:
    PYWINRT_EXE = (
        REPO_ROOT_PATH / "_build" / "Windows" / "x86" / "Release" / "pywinrt.exe"
    )

    if not PYWINRT_EXE.exists():
        raise RuntimeError(
            "pywinrt.exe not found. Please run `./scripts/vsdevcmd-build.cmd`"
        )


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

# generate code for windows app sdk

WINDOWS_APP_SDK_PACKAGE_METADATA = (
    REPO_ROOT_PATH / "_tools" / "Microsoft.WindowsAppSDK" / "lib" / "uap10.0"
)
WINDOWS_APP_SDK_PACKAGE_METADATA2 = (
    REPO_ROOT_PATH / "_tools" / "Microsoft.WindowsAppSDK" / "lib" / "uap10.0.18362"
)
WINDOWS_APP_SDK_PACKAGE_PATH = (
    PROJECTION_PATH
    / "winrt-WindowsAppSDK"
    / "src"
    / "winrt_windows_app_sdk"
    / "pywinrt"
)

subprocess.check_call(
    [
        PYWINRT_EXE,
        "-input",
        WINDOWS_APP_SDK_PACKAGE_METADATA,
        "-input",
        WINDOWS_APP_SDK_PACKAGE_METADATA2,
        "-reference",
        WINDOWS_SDK,
        "-output",
        PROJECTION_PATH,
        "-header-path",
        WINDOWS_APP_SDK_PACKAGE_PATH,
    ]
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

# copy versioned files to non-generated packages
for path in itertools.chain(
    [PROJECTION_PATH / "winrt-sdk", PROJECTION_PATH / "winrt-runtime"],
    (PROJECTION_PATH / "interop").glob("winrt-*"),
):
    shutil.copy(
        str(PROJECTION_PATH / "winrt-Windows.Foundation" / "pywinrt-version.txt"),
        str(path / "pywinrt-version.txt"),
    )
