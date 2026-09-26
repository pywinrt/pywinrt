# winrt-runtime

This package provides the PyWinRT runtime including the `winrt.system` module.

Compiled code outside the runtime, such as the interop packages, exchanges
WinRT objects with it through Python functions rather than a C ABI:
`winrt._winrt.as_interface()` gives an interface of a projected object as an
interface pointer capsule, `winrt._winrt.wrap_interface()` makes a projected
object of one, and `winrt._winrt.hresult_error()` builds the exception that a
failed call raises. An interface pointer capsule is named `"winrt.interface"`
and holds one reference, which its destructor releases.

It also ships the C++/WinRT headers of the Windows SDK namespaces it is built
against, for a module that is written against C++/WinRT. Use
`winrt._include.get_cppwinrt_include()` to locate them.
