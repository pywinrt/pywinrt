

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

- Ruff
- mypy
- ClangFormat
- CSharpier (`dotnet tool restore`)

## Building PyWinRT.exe

Building this tool requires .NET 10.0 SDK.

    dotnet build PyWinRT -c Release

The resulting executable will be `PyWinRT\bin\Release\net10.0\PyWinRT.exe`.


## Regenerating the projection

Once a local build of `PyWinRT.exe` is built, it can be used to update
the generated files of the projection. This requires that the Windows SDK is
installed either via the Visual Studio Installer or standalone.

    py .\scripts\generate-pywinrt.py

The generator also merges the census of ABI call shapes it takes while it runs
into `runtime/src/shapes.json`, and writes the trampolines that `winrt-runtime`
compiles from it to `runtime/src/shapes-generated.h`. Both are committed, and a
run may only append to the census; `runtime/src/table-format.md` describes the
table format and the census.

Some additional files are also generated instead by:

    py .\scripts\generate-pyproject.py

## Moving the Windows App SDK pin

The Windows App SDK is a metapackage over component packages that are
published separately, and `.config/_tools.json` records the version of each
one. Do not edit those by hand: Microsoft's own build refuses a project that
references a set of components it did not ship together, so the set belongs to
a release rather than to us. Run

    py .\scripts\resolve-wasdk.py

which resolves the newest stable release, takes the component set from its
nuspec, checks it against the list the Runtime component carries, and writes
the result. Then fetch and regenerate as above.

## Generating the C++/WinRT headers

`winrt-runtime` and the eight interop modules are the only things here that are
compiled, and they include C++/WinRT headers. They are generated from the NuGet
packages and committed, like the rest of the generated tree, so this has to be
run whenever `.config/_tools.json` changes or a module starts including another
namespace:

    py .\scripts\generate-cppwinrt.py

Each package carries the headers it includes. `winrt-runtime` carries what
`pywinrt/base.h` includes, inside the package beside the PyWinRT headers that
`winrt._include` hands out, and an interop module carries the namespaces it
includes beyond those in a `cppwinrt` directory of its own. Either way they are
found at the same place whether the module is built here or from its source
distribution.

## Regenerating other upstream code

Normally this is not needed unless we need to pull in changes from upstream.

If the pythoncapi compat headers are updated, run:

    .\scripts\update-pythoncapi-compat.ps1

## Running tests

In a Visual Studio command prompt, run:

    cmake -S projection -B _build/test-projection -G Ninja -DPYWINRT_FULL_PROJECTION=OFF
    cmake --build _build/test-projection
    cmake --install _build/test-projection --prefix _install/test

Note: if using MINGW, add the following to the first command:

    -DPython3_ROOT_DIR=${MINGW_PREFIX} -DCMAKE_BUILD_TYPE=Release

Once the projection has been built (see above), tests can be run. The default
build uses the debug configuration, so that means we _must_ use `python_d.exe`
instead of `python.exe`. You can use `py -0p` to find the Python installation
path if Python is not in `PATH` already.

    $env:PYTHONPATH="_install/test"
    python_d.exe -X dev -m unittest

If using MSYS2 shell for MINGW, this can be simplified to:

    PYTHONPATH=_install/test python -X dev -m unittest

## Linting

The rules and the files they apply to are in `ruff.toml` and `mypy.ini`, so an
editor checks the same thing as the `lint` job in `.github/workflows/test.yaml`
does:

    pipx run ruff check

mypy reads the packages from one directory, `_install/lint`, and they are
generated into several, so the lint job copies them together first with the
`merge packages` step of that workflow. Run that step, then:

    pipx run mypy

## Building the Nuget package

To build the nuget package for the `PyWinRT.exe` tool.

    dotnet pack PyWinRT

## Building Python wheels

A projection package is data, so its wheel is `py3-none-any` and is built once.
`winrt-runtime` and the interop modules are compiled, so cibuildwheel builds
theirs for each version of Python and each target architecture. With no
arguments, both halves are built:

    py .\scripts\build-bdist.py

Either half can be built on its own, and any other argument is passed on to
cibuildwheel:

    py .\scripts\build-bdist.py --pure
    py .\scripts\build-bdist.py --compiled --only cp312-win_amd64

The pure half needs `winrt-table-compiler` installed rather than merely
importable, because hatchling finds the hook that compiles the tables through
the entry point in its metadata.

A release also needs the source distributions:

    py .\scripts\build-sdist.py


## Profiling

`perf/dispatch.py` measures what it costs to call a projected member at all,
which is the number that a change to the runtime is judged by. Its docstring
has the commands and the cautions that come with reading a single row.

It needs a `Release` build, which is compiled with link time optimization, the
same as the wheels are, so it takes noticeably longer to link than the other
configurations and it is what users run. Turning that off to compare against is
an edit to `projection/CMakeLists.txt`: passing
`-DCMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE=OFF` does nothing, because the
plain `set()` there shadows the cache variable.

To find where the time goes, create a `RelWithDebInfo` build of the projection
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
