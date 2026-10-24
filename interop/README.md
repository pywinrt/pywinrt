# Interop packages

This folder contains packages that wrap useful WinRT interop headers. Unlike
regular projections, these are not automatically generated.

An interop module shares no C ABI with `winrt-runtime` and includes none of its
headers. It hands a WinRT object over, or takes one, as an interface pointer
capsule, which `winrt.runtime.interop.as_interface()` makes of a projected
object and `winrt.runtime.interop.wrap_interface()` makes a projected object
of, and a failed call raises the exception that
`winrt.runtime.interop.hresult_error()` builds. Those three functions are the
contract, the same one a third-party interop module uses; `winrt._winrt` is
their implementation. The package's `__init__.py` makes those calls and is the
public API; the extension is flat functions over capsules and plain values.

What every module needs for that is in `interop.h` in this directory.
`scripts/generate-pyproject.py` copies it into each package, so that a source
distribution builds on its own; edit this one and regenerate rather than
editing a copy.
