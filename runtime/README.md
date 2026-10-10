# winrt-runtime

This package provides the PyWinRT runtime including the `winrt.system` module.

Every PyWinRT package requires it, so pip installs it with any of them. Each
WinRT namespace is a package of its own, so install the package of each
namespace your code imports from, such as `winrt-Windows.Foundation`.

## For interop modules

Compiled code outside the runtime, such as the interop packages, exchanges
WinRT objects with it through the Python functions of `winrt.runtime.interop`
rather than a C ABI: `as_interface()` gives an interface of a projected object
as an interface pointer capsule, `wrap_interface()` makes a projected object of
one, and `hresult_error()` builds the exception that a failed call raises. An interface pointer capsule is named `"winrt.interface"`
and holds one reference, which its destructor releases.
