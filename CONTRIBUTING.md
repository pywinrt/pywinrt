

## Building pywinrt.exe

If you are not already in a Visual Studio command prompt, run:

    .\scripts\vsdevcmd-build.cmd

Otherwise you can run:

    .\scripts\build.cmd

The resulting executable will be `_build\Windows\x86\Debug\pywinrt\pywinrt.exe`.


## Generating a projection

Once `pywinrt.exe` is built, it can be used to generate a projection of the
Windows SDK for testing. This requires that the Windows SDK is installed either
via the Visual Studio Installer or standalone. The following must be run in
a Visual Studio command prompt.

    cd projection
    .\generate.ps1
    .\build.ps1

The resulting Python package is located at `projection\pywinrt`.

## Running tests

Once the projection has been built (see above), tests can be run. The default
build uses the debug configuration, so that means we _must_ use `python_d.exe`
instead of `python.exe`. You can use `py -0p` to find the Python installation
path if Python is not in `PATH` already.

    $env:PYTHONPATH="projection\pywinrt"
    python_d.exe -m unittest

## Building the Nuget package

    .\scripts\build_pywinrt_nuget.cmd
