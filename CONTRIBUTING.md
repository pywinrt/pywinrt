

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
- pyright
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

    cmake -S . -B _build/projection -G Ninja -DPython3_EXECUTABLE=C:\path\to\python_d.exe
    cmake --build _build/projection
    cmake --install _build/projection --prefix _install/projection

Name the interpreter the tests will run under, as CI does: without
`Python3_EXECUTABLE`, CMake builds against the first Python it finds, which may
be another version, or one installed without the debug libraries that a debug
build links against. `python_d.exe` comes from the installer on python.org
with "Download debug binaries" selected (Modify adds them to an existing
install); the Python install manager's runtimes have no debug build. `py -0p`,
or `py list` with the install manager, lists the installed Pythons and their
paths.

Note: if using MINGW, put the following in place of `-DPython3_EXECUTABLE`
in the first command:

    -DPython3_ROOT_DIR=${MINGW_PREFIX} -DCMAKE_BUILD_TYPE=Release

Once the projection has been built (see above), tests can be run. The default
build uses the debug configuration, so that means we _must_ use `python_d.exe`
instead of `python.exe`.

    $env:PYTHONPATH="_install/projection"
    python_d.exe -X dev -m unittest

If using MSYS2 shell for MINGW, this can be simplified to:

    PYTHONPATH=_install/projection python -X dev -m unittest

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

    uvx ruff check

mypy reads the packages from one directory, `_typing`, and they are generated
into several, so `scripts/merge-packages.py` copies them together as they would
be installed. Every CMake build runs it, and so does the lint job; to refresh
`_typing` without a build, run it yourself:

    py .\scripts\merge-packages.py
    uvx mypy

The mypy extension of VS Code bundles a mypy and a `typing_extensions` of its
own, which can be too old for the stubs: as of October 2026, version 2026.6.0
of the extension bundles mypy 1.15.0 and `typing_extensions` 4.12.2, and the
stubs use `TypeForm`, which `typing_extensions` 4.13.0 added, so it reports
errors the lint job does not. Set
`"mypy-type-checker.importStrategy": "fromEnvironment"` and select a virtual
environment with a current mypy as the interpreter it runs under.

pyright, and Pylance with it, reads `pyrightconfig.json`. The lint job also
checks the merged packages with `pyrightconfig.lint.json`; an editor shows a
file under `projection/` or `interop/*/winrt/` as it is in the source tree,
where an interop package is a second copy of a module, so the lint job is the
authority for those files. Both runs need what the checked code imports,
`samples/requirements-dev.txt`, `hatchling` and `setuptools`, installed in the
environment pyright is pointed at, which is the interpreter selected in the
editor or the one given with `--pythonpath`. The lint job also puts `_typing`
on that environment's path with a `.pth` file, so that the second run checks
the merged packages as installed ones, which includes that a module exports
what the stubs import from it:

    py -m venv _install\pyright-venv
    _install\pyright-venv\Scripts\python -m pip install -r samples\requirements-dev.txt hatchling setuptools
    Set-Content _install\pyright-venv\Lib\site-packages\_typing.pth "$PWD\_typing"
    _install\pyright-venv\Scripts\python -c "import sys; assert r'$PWD\_typing' in sys.path, sys.path"
    uvx pyright --pythonpath _install\pyright-venv\Scripts\python.exe
    uvx pyright -p pyrightconfig.lint.json --pythonpath _install\pyright-venv\Scripts\python.exe

That environment is for analysis only: with `_typing` on its path, `import
winrt` finds stubs and pure-Python modules with no extension module behind
them, so run nothing from it and install no `winrt-*` package into it. The
tests run on their own interpreters with an install tree on `PYTHONPATH`, as
above. As the editor's interpreter it gives no more than any environment with
those packages: the editor reads `pyrightconfig.json`, which finds `_typing`
through `extraPaths` with any interpreter, and pyright treats what it finds
there as local code, so only the lint job's second run checks what the
packages export.

## Building the documentation

The documentation in `docs/` is built by [Zensical](https://zensical.org/),
configured by `zensical.toml`, at the version pinned in `docs/requirements.txt`,
which is the one Read the Docs installs:

    py -m venv .venv
    .venv\Scripts\python -m pip install -r docs/requirements.txt
    .venv\Scripts\zensical build --strict

The site is written to `_site/`. `--strict` fails the build on a link to a page
or an anchor that does not exist, as the Read the Docs build does.
`zensical serve` serves it at <http://localhost:8000/> and rebuilds it on
every change.

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
