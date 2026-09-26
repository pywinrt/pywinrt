

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

`winrt-runtime` and `winrt-Microsoft.UI.Interop` are the only things here that
are compiled against C++/WinRT; the other interop modules are written against
raw COM. The headers are generated from the NuGet packages and committed, like
the rest of the generated tree, so this has to be run whenever
`.config/_tools.json` changes or a module starts including another namespace:

    py .\scripts\generate-cppwinrt.py

Each package carries the headers it includes. `winrt-runtime` carries what
`pywinrt/base.h` includes, in `runtime/src/include/cppwinrt`, and an interop
module carries the whole closure of the namespaces it includes in a `cppwinrt`
directory of its own. Either way they are found at the same place whether the
module is built here or from its source distribution, and neither is installed.

## Regenerating other upstream code

Normally this is not needed unless we need to pull in changes from upstream.

If the pythoncapi compat headers are updated, run:

    .\scripts\update-pythoncapi-compat.ps1

## Running tests

In a Visual Studio command prompt, run:

    cmake -S . -B _build/test-projection -G Ninja
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

The free-threaded build of Python needs a tree of its own, because its
extension modules are built against a different ABI. Adding the following to
the first command picks `python314t.lib` instead:

    -DPython3_FIND_ABI="ANY;ANY;ANY;ON"

Install that tree to its own prefix and run the tests with `python3.14t_d.exe`.
`test/test_threads.py` is the part of the suite that is written for it: its
threads run at the same time only when there is no GIL.

## Linting

The rules and the files they apply to are in `ruff.toml` and `mypy.ini`, so an
editor checks the same thing as the `lint` job in `.github/workflows/lint.yaml`
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

Both scripts build every package that is published unless they are told to
narrow it. `--family` builds everything built against one upstream, including
the interop modules, and `--package` names distributions outright:

    py .\scripts\build-bdist.py --family webview2
    py .\scripts\build-sdist.py --package winrt-Windows.Foundation

`--tag` builds what one release tag publishes, which is narrower than
`--family`: it is how the workflow builds a release, and it fails on a tag
whose version is not the one the tree carries.

    py .\scripts\build-sdist.py --tag wheels/winrt-Windows.System.Interop/v4.0.0

## Releasing

A release publishes one *unit* rather than everything, and a unit is named
after the directory its sources are in. There are two kinds.

A family of generated packages — `winrt`, `winui2`, `wasdk` and `webview2`,
the directories under `projection/` — is released on its upstream's schedule
and takes its version from the NuGet package its metadata came from, which
`.config/_tools.json` pins. Moving a pin is a change of its own, made the way
the sections above describe.

A package written by hand has no upstream release to follow, so it keeps its
own `version.txt` and is released on its own: `runtime`, `table` and each
directory under `interop/`. Bump one when that package changes, in semver,
keeping the major — it is the compatibility generation, and the build refuses
a tree whose hand-written versions disagree about it. Moving a pin does *not*
bump these: a new Windows SDK leaves an interop module alone unless something
it compiles against actually changed, which the diff shows because those
packages carry the headers they compile. The exception is
`interop/winrt-wasdk-bootstrap`, whose wheel carries
`Microsoft.WindowsAppRuntime.Bootstrap.dll` out of the App SDK pin, so moving
that pin does change it.

An interop package requires the newest `winrt-runtime` that one of the
`winrt._winrt` functions it calls first shipped in, not the runtime of the
tree, so that a fix to it can be installed without upgrading the runtime.
`RUNTIME_FUNCTIONS` in `scripts/versions.py` records that version for each
function. Adding a function for the interop packages to call means listing it
there with the version it first ships in, and bumping `runtime/version.txt` to
that version in the same change: `generate-pyproject.py` refuses a call to a
function that is not listed, and a listed version the runtime has not reached.

Either way, regenerate the packaging afterwards so that every package carries
the version it is published with:

    py .\scripts\generate-pyproject.py

Then print the tag that releases each unit and push the one you mean:

    py .\scripts\release-tags.py

    git push origin wheels/webview2/e4v1.0.4191.47

A version in a tag carries a `v`, and the epoch goes in front of that as
`e4`, since a literal `!` is a history expansion in a shell. The tag has to
carry the version the tree already carries: the build refuses a tag that
names anything else rather than publishing a version nobody asked for.

Pushing the tag runs `.github/workflows/wheels.yaml`, which builds that unit
and uploads it to PyPI. Release `runtime` before anything else and `winrt`
before the units that depend on it, because a compiled wheel's import test
installs the packages it depends on; `release-tags.py` prints them in that
order.

The same workflow can be started by hand, which is how everything else is
done:

- `family` and `packages` are the build filters above. `family` groups by the
  upstream a package was built against, so it is wider than a release: it
  covers the interop modules built against a family, which a release of that
  family leaves alone.
- `publish` says where the result is uploaded, and is `none` by default. A
  `none` run is a compile test of a branch, and it is what builds the
  wheelhouse a release candidate is tested from; `testpypi` is the dry run
  that proves the token path.
- `python` builds for one interpreter only, which back-fills wheels for a new
  CPython without a version change: the packages that are already published
  keep their versions and only the missing builds are uploaded.


## Profiling

`perf/dispatch.py` measures what it costs to call a projected member at all,
which is the number that a change to the runtime is judged by. Its docstring
has the commands and the cautions that come with reading a single row.

It needs a `Release` build, which is compiled with link time optimization, the
same as the wheels are, so it takes noticeably longer to link than the other
configurations and it is what users run. Turning that off to compare against is
an edit to the top-level `CMakeLists.txt`: passing
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
