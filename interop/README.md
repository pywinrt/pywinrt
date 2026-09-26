# Interop packages

This folder contains packages that wrap useful WinRT interop headers. Unlike
regular projections, these are not automatically generated.

An interop module shares no C ABI with `winrt-runtime` and includes none of its
headers. It hands a WinRT object over, or takes one, as an interface pointer
capsule, which `winrt._winrt.as_interface()` makes of a projected object and
`winrt._winrt.wrap_interface()` makes a projected object of, and a failed call
raises the exception that `winrt._winrt.hresult_error()` builds. The package's
`__init__.py` makes those calls and is the public API; the extension is flat
functions over capsules and plain values.

What every module needs for that is in `interop.h` in this directory.
`scripts/generate-pyproject.py` copies it into each package, so that a source
distribution builds on its own; edit this one and regenerate rather than
editing a copy.
