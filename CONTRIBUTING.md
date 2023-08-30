

## Prerequisites

### For building code generation tool

- Visual Studio (C++ workload, CMake, etc.)

### For generating projections

- Nuget (`winget install microsoft.nuget`)
- Windows SDK
- Python (runtime)

Once Nuget is installed, run:

   .\scripts\fetch-tools.ps1

### For building Python extension modules

- CMake
- C++ toolchain (Visual Studio, MINGW, etc.)
- Python (headers, libraries, debug, etc.)

### For linting

- Black
- ClangFormat

## Building pywinrt.exe

If you are not already in a Visual Studio command prompt, run:

    .\scripts\vsdevcmd-build.cmd

Otherwise you can run:

    .\scripts\build.cmd

The resulting executable will be `_build\Windows\x86\Debug\pywinrt.exe`.


## Regenerating the projection

Once a local build of `pywinrt.exe` is built, it can be used to update
the generated files of the projection. This requires that the Windows SDK is
installed either via the Visual Studio Installer or standalone.

    py .\scripts\generate-python.py

Some additional files are also generated instead by:

    py .\scripts\generate-pyproject.py

## Regenerating other upstream code

Normally this is not needed unless we need to pull in changes from upstream.

If the CppWinRT version is updated, run:

   py .\scripts\generate-cppwinrt.py

If the pythoncapi compat headers are updated, run:

   .\scripts\update-pythoncapi-compat.ps1

## Running tests

In a Visual Studio command prompt, run:

    cmake -S projection -B _build/test-projection -G Ninja -DPYWINRT_FULL_PROJECTION=OFF
    cmake --build _build/test-projection
    cmake --install _build/test-projection --prefix _build/test-install

Note: if using MINGW, add the following to the first command:

    -DPython3_ROOT_DIR=${MINGW_PREFIX} -DCMAKE_BUILD_TYPE=Release

Once the projection has been built (see above), tests can be run. The default
build uses the debug configuration, so that means we _must_ use `python_d.exe`
instead of `python.exe`. You can use `py -0p` to find the Python installation
path if Python is not in `PATH` already.

    $env:PYTHONPATH="_build/test-install"
    python_d.exe -X dev -m unittest

If using MSYS2 shell for MINGW, this can be simplified to:

    PYTHONPATH=_build/test-install python -X dev -m unittest

## Building the Nuget package

To build the nuget package for the `pywinrt.exe` tool.

    .\scripts\build_pywinrt_nuget.cmd

## Building Python wheels

To build Python wheels of the projection packages:

    py .\scripts\build-bdist.py

To only build for a specific Python and target architecture:

    py .\scripts\build-bdist.py --only cp312_win_amd64
