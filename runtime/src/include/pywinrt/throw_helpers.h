// Definitions of the Microsoft STL throw helpers, so that no module in the
// projection imports MSVCP140.dll.
//
// Python ships the C runtime next to python3xx.dll and the UCRT is an operating
// system component, but the C++ standard library is neither. A module that
// imports MSVCP140.dll therefore needs a copy of a DLL the machine may not
// have, which is why winrt-runtime used to carry one in its wheel - and one
// copy of it per process, whether the process wanted ours or not.
//
// The whole of that dependency is two cold [[noreturn]] helpers that MSVC's
// containers call when a length or index check fails. Measured over the built
// projection, the imports are exactly std::_Xlength_error (nearly every module,
// from std::vector, std::string and std::map growth) and std::_Xout_of_range
// (winrt-runtime, from std::string_view::substr).
//
// <xutility> declares them without __declspec(dllimport) - _CRTIMP2 expands to
// nothing outside the CRT's own build - so defining them here resolves them
// from the including translation unit and msvcprt.lib is never consulted. The
// exception types and messages are the ones the standard library would have
// thrown: the helpers exist only to keep the throw out of the caller's inlined
// code.
//
// These are internal names rather than contractual ones, so it is worth being
// precise about how that can fail. Two of the three ways are build errors: if a
// future standard library marks them dllimport, this definition is rejected,
// and if the library member that defines them is pulled in for some other
// symbol, the linker reports a duplicate. The third is silent and is the one to
// guard against - something new starting to pull MSVCP140.dll in for a
// different symbol. Once the wheels stop carrying a copy of it, a module that
// does that fails to load on every machine without the Visual C++
// redistributable installed, so it wants checking after linking rather than
// discovering in a bug report.

#pragma once

#ifdef _MSC_VER

#include <stdexcept>

namespace std
{
    // noinline states the property these helpers exist for: the standard
    // library calls them rather than throwing inline so that the throw stays
    // out of the container code doing the bounds check. MSVC declines to inline
    // them anyway - marking them made no difference to a single module - so
    // this is here to keep that true rather than to fix anything observed.
    [[noreturn]] __declspec(noinline) inline void __cdecl _Xlength_error(
        const char* what)
    {
        throw length_error(what);
    }

    [[noreturn]] __declspec(noinline) inline void __cdecl _Xout_of_range(
        const char* what)
    {
        throw out_of_range(what);
    }
} // namespace std

#endif // _MSC_VER
