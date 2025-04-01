

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

## Building PyWinRT.exe

Building this tool requires .NET 8.0 SDK.

    dotnet build PyWinRT -c Release

The resulting executable will be `PyWinRT\bin\Release\net8.0\PyWinRT.exe`.


## Regenerating the projection

Once a local build of `PyWinRT.exe` is built, it can be used to update
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

To build the nuget package for the `PyWinRT.exe` tool.

    dotnet pack PyWinRT

## Building Python wheels

To build Python wheels of the projection packages:

    py .\scripts\build-bdist.py

To only build for a specific Python and target architecture:

    py .\scripts\build-bdist.py --only cp312-win_amd64


## Profiling

To measure performance, create a `RelWithDebInfo` build of the projection and
and install it, e.g. in `_install/perf`.

Download the `PerfView.exe` tool from <https://github.com/microsoft/perfview>
then start it.

Use the `Collect` menu to start a profiling session with either the `Run`
command to launch a Python process or `Collect` to attach to an already running
process.

You may need the POSIX `env.exe` command, e.g. from MSYS2, to set the `PYTHONPATH`
environment variable. You may also need to give the full path to the Python
executable and/or set the working directory.

    C:\msys64\usr\bin\env.exe PYTHONPATH=_install\perf python3.exe perf\string_iter.py

Once the session is complete (either by ending the process or stopping the
collection), expand the `.etl.zip` file in the PerfView UI and double-click on
`CPU Stacks`. Then double-click on Python process from the list that is the
one you want to profile. Then have a look at the `Flame Graph` tab.

Note: profiling using `VSInstr.exe`/`VSDiagnostics.exe` does not seem to work
very well and frequently crashes.
