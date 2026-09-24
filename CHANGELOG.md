<!-- Refer to https://keepachangelog.com/en/1.0.0/ for guidance. -->

# Changelog

## [Unreleased]

### Added
- Wheels are now published for Python 3.14 and Python 3.15.
- Added `winrt-table-compiler`, a small pure-Python package that compiles the
  table a projection package carries as text into the binary form that
  `winrt-runtime` reads. It is a build-time dependency of every projection
  package and there is nothing in it to import.
- `winrt.system.Array` now accepts the type of its elements wherever it used to
  need a format string. The eleven `winrt.system` aliases - `Int8`, `UInt8`,
  `Int16`, `UInt16`, `Int32`, `UInt32`, `Int64`, `UInt64`, `Single`, `Double`
  and `Char16` - name the scalars, `bool` names `Boolean`, and a projected enum
  type names an array of that enum, which had no spelling at all before.
- The `winrt.system` aliases say what their annotation means. Each one carries
  a `winrt.system.BufferFormat`, a `winrt.system.StructFormat` and a
  `winrt.system.WinrtSignature`, so code that reads one finds it with an
  `isinstance` check over `typing.get_args` rather than by its position. They
  are subclasses of `str`, and the buffer format comes first, so the single
  string the aliases used to carry reads back the same as it did.

### Changed
- A projection package no longer contains a compiled extension module. Each
  one is a single `py3-none-any` wheel holding the type stubs, a small
  `__init__.py` and a table that describes the namespace: its types, their
  members, and the ABI call each member makes. `winrt-runtime` reads that
  table and makes the calls, so it and the interop packages are the only parts
  of the projection that are still compiled. One wheel now serves every
  supported version of Python and every architecture, where a package used to
  be built once for each combination of the two, and installing a package from
  its source distribution needs neither a C++ compiler nor the Windows SDK.
  All 410 projection packages together are 6.2 MB of wheels; the 420 of v3.2.1
  were 49.2 MB for each version of Python and each architecture. The largest
  of them, `winui3-Microsoft.UI.Xaml.Controls`, is 0.28 MB where it was
  3.54 MB.
- A tool that freezes an application, such as PyInstaller, has to be told to
  collect each namespace's table, which is the `_table.pywinrt` file beside
  its `__init__.py`. Collecting the Python modules and the extension modules
  of a projection package is no longer enough to make it work, since the table
  is neither.
- Calling a WinRT member costs more than it did in v3.2.1. A projection
  package used to be C++ compiled for the member being called, and it is now a
  table that the runtime reads at the moment of the call; interpreting it is
  what pays for the package being data. Measured on one machine with both
  builds on the same day, most kinds of member cost 10 to 45 % more per call,
  and two cost about twice as much: a `dict` passed where an `IMap` is
  expected, and `get()` on an async operation that has already completed. In
  absolute terms that is tens of nanoseconds added to calls that took 60 to
  400 ns, so what the member itself does - reaching a device, a file or the
  display - still dominates by orders of magnitude. Treat any single one of
  these figures as approximate: a benchmark row moves by up to 20 % between
  builds of code that cannot affect it.
- Generating a projection of your own WinRT component needs nothing but the
  tool. The table names the ABI call shape of each member by an id, the ids
  are assigned by a census that `winrt-runtime` compiles its call trampolines
  from, and the `PyWinRT` NuGet package now carries that census beside
  `pywinrt.exe`. It is the one the `winrt-runtime` of the same release was
  built from, so a component whose members all have shapes it already lists
  works with the published wheel and no option has to be given. A different
  census is named with `--shapes <file>`, for targeting a `winrt-runtime`
  other than the one the tool shipped with. What the tool writes for your
  component is a table and the type stubs beside it, so projecting one needs
  no compiler either.
- A component that makes a call no shape in the census covers is now reported
  while it is generated, naming each shape the census is missing. Those
  members are still projected and the rest of the namespace is unaffected;
  using one raises rather than dispatching to the wrong trampoline. The way to
  get such a shape covered is to add a member of that shape to
  https://github.com/pywinrt/testwinrt, which puts it in the next
  `winrt-runtime` release. `--emit-shapes <dir>` still appends one locally,
  but a projection generated that way loads only against a `winrt-runtime`
  built from `<dir>`, so it is not a route to a publishable package.
- A table now records which census its shape ids were assigned by, as a
  lineage and a revision, and `winrt-runtime` refuses one whose census is not
  the one its own trampolines came from. Before this, a projection generated
  against a census that was not the runtime's would import and then call the
  wrong trampoline, with the arguments laid out for another signature.
- The type stubs now use modern annotation syntax. Optional and union types are
  spelled `X | None` and `X | Y` instead of `typing.Optional[X]` and
  `typing.Union[X, Y]`, `typing.Tuple` and `typing.Type` are now the builtin
  `tuple` and `type`, and the abstract collection types come from
  `collections.abc` instead of `typing`. This only affects how the annotations
  read; they mean the same thing to type checkers.
- The type stubs now follow the conventions that type checkers and stub linters
  expect of a stub. A method that returns the type it was called on says
  `typing.Self` instead of naming a type variable, an empty class body is `...`
  instead of `pass`, a default value that is not a literal is written `...`, and
  `typing.Generic` comes last in a list of base classes. The redundant `__str__`
  declaration is gone from the classes that implement `IStringable`; `str()` on
  them works the same as it always has.
- `winrt-runtime` no longer ships a copy of `MSVCP140.dll`, and no module in
  the projection needs the Visual C++ redistributable any more. The whole of
  that dependency was two cold standard library helpers, which are now compiled
  into each module instead. Windows binds a DLL import by base name to whichever
  copy loaded first, so the copy we shipped became the one every package in the
  process used, and it could be older than another package needed - importing
  `winrt` before some packages was enough to stop them loading ([#139]).
- Overloaded methods are called by the number of arguments again instead of
  using a separate name for each overload. The
  `[Windows.Foundation.Metadata.Overload]` attribute is now only used for
  overloads that take the same number of arguments as another overload and for
  overridable methods.
- Python classes that implement a WinRT interface now implement all overloads of
  a method with a single method. The old names are still called if they are
  defined, so this only shows up as a type checker error.
- BREAKING: `TileUpdateManagerForUser.create_tile_updater_for_application()` is
  now `create_tile_updater_for_application_for_user()`. The old name is the
  overload that takes an application id, so it could not be kept as an alias.
- Greatly improved the speed of the `PyWinRT` code generation tool.
- A member is no longer checked against `ApiInformation` before it is called.
  A member that another interface declares is reached by querying that
  interface, which answers the question the call actually depends on; a member
  declared by the default interface of a class is not checked at all, since an
  object cannot exist without implementing it; and a static member is simply
  called, since there is no object to query for one. Reaching a member that
  this version of Windows does not have therefore fails in the call or in the
  activation factory, and the metadata is consulted only then, to say so. Two
  things change in what you see: the `AttributeError` now names the member, as
  in `method 'Windows.Media.Ocr.OcrEngine.IsLanguageSupported' is not available
  in this version of Windows`, and an argument that cannot be converted raises
  `TypeError` before it, where the version error used to come first. A
  projection of your own component is answered the same way. The check used to
  be left out of a `--component-dlls` build because `ApiInformation` cannot
  always answer for a third-party component, and now that the question is only
  asked after a call has failed, an unanswerable one costs nothing and falls
  back to reporting the `HRESULT`.
- BREAKING: The `winrt-sdk` package is gone. It shipped the C++/WinRT headers
  and the hand-written PyWinRT headers that a generated projection compiled
  against, and no projection package compiles any more. A module written by
  hand to pass WinRT values to Python - the interop packages here are the
  examples - includes `<pywinrt/base.h>`, which `winrt-runtime` ships
  and `winrt._include.get_include()` locates, the way NumPy and pybind11
  publish their headers:

  ```python
  from setuptools import setup
  from winrt._include import get_include

  setup(..., include_dirs=[get_include(), "path/to/cppwinrt"])
  ```

  Such a module needs nothing generated: it names the WinRT type it is handing
  over and the runtime resolves the name. `pybase.h` is now
  `<pywinrt/base.h>`, mirroring C++/WinRT's own `<winrt/base.h>`.
- `<pywinrt/base.h>` is now an umbrella header over ten smaller ones - among
  them `<pywinrt/abi.h>`, which holds the ABI struct that a compiled module
  and `winrt-runtime` agree on, and `<pywinrt/convert.h>`, which holds the
  Python-to-WinRT conversions. Code that includes `<pywinrt/base.h>` needs no
  change: it still provides everything.
- `winrt-runtime` and the interop modules are compiled without RTTI (`/GR-`
  on MSVC, `-fno-rtti` otherwise). Nothing in them uses `dynamic_cast`, and
  the only uses of `typeid` were a dozen `typeid(T).name()` calls in the
  messages of `NotImplementedError`, which now name the WinRT type instead of
  giving an MSVC mangled name.
- The `PyWinRT` dotnet tool now targets .NET 10 and needs the .NET 10 runtime
  to run. This only affects generating your own projection with the tool; the
  published wheels are unchanged and have never needed .NET installed.
- An index that a Python sequence does not have is now reported to WinRT as
  `E_BOUNDS`, which is what a WinRT collection raises for an index out of
  range, instead of being written out as an unraisable `IndexError`. A WinRT
  caller that reads past the end of a list therefore fails with a bounds error
  rather than the generic "Unraisable Python exception". Everything else a
  Python object can raise while WinRT is reading it still goes to
  `sys.unraisablehook`, since WinRT has no way to report it.
- `HResult` and `EventRegistrationToken` are projected as subclasses of `int`
  rather than as structs with a single field in them, since that is what each
  of them is. An `EventRegistrationToken` can be compared, hashed and used as
  a dictionary key like the integer it is, and an `HResult` can be compared
  against the codes in `winrt.system.hresult` directly.
- BREAKING: A package is now versioned by the metadata it projects instead of
  by the version of the generator that read that metadata.
  `winrt-Windows.Storage` is `4!10.0.26100.4188`, the version of the
  `Microsoft.Windows.SDK.CPP` NuGet package its winmd files come from, and the
  WinUI 2, Windows App SDK and WebView2 packages are likewise numbered by their
  own upstream. Each package's README names which NuGet package and version
  that is. So a package now says which Windows APIs are in it, an upstream
  release no longer has to wait for a PyWinRT release to be numbered after,
  and the right way to pin one is the way its upstream numbers it -
  `winui3-Microsoft.UI.Xaml==4!1.7.*` - rather than by a PyWinRT version.
  `winrt-runtime` keeps plain semantic versioning, since its version is its
  own.
- BREAKING: The `4!` in front of those versions is a PEP 440 epoch and says
  which generation of PyWinRT the package was built for. It is the same number
  as `winrt-runtime`'s major version and it changes only when every projection
  package has to be rebuilt anyway, so a `pip freeze` says which runtime a
  package goes with. It is also what makes the new numbering sort above the
  old: without it, `10.0.26100.4188` would have to sort against the `3.2.1` it
  replaces, and `winui3-Microsoft.UI.Xaml` would go backwards from `3.2.1` to
  `1.7.250513003`. Some tools that rewrite dependency constraints have
  historically mishandled epochs; the spelling is `4!1.7.250513003` wherever
  one is written out.
- BREAKING: A package now requires `winrt-runtime>=<the version it was
  generated with>,<5` where it used to pin `winrt-runtime~=<version>.0`. Any
  runtime of the same generation can run it, so pip is free to install a newer
  one. The upper bound is the only one anywhere in PyWinRT's metadata, and it
  is there because that incompatibility is known rather than guessed: a
  package of one generation is refused by the runtime of the next, so without
  it pip would upgrade the runtime on its own into an import error. Building a
  package from its source distribution caps `winrt-table-compiler` the same
  way, for the same reason - a compiler of the next generation would write a
  table that the runtime the wheel then asks for cannot read.
- `winrt-Windows.Foundation`, `winrt-Windows.Foundation.Collections` and
  `winrt-Windows.Storage.Streams` are now installed with any package that
  hands back one of their types, instead of being offered by that package's
  `[all]` extra. Nearly everything in the projection returns an
  `IAsyncOperation`, an `IVectorView` or an `IBuffer`, so a minimal install
  was usually missing one of these and said so only when a call returned. The
  three together are under a megabyte and reference nothing outside the set,
  so nothing else is pulled in with them.
- Everything else one package says about another is now a floor rather than a
  `~=` pin, including the packages named by the `[all]` extra. Packages do not
  call into each other's compiled code - a type from another package is
  resolved through WinRT by name - so the pin was protecting nothing, and it
  made installing two packages that were generated from different upstream
  releases fail to resolve. Packages that do come from one set of metadata,
  such as `winui3-Microsoft.UI.Xaml` and `winui3-Microsoft.UI.Xaml.Controls`,
  are still pinned to each other, because they are generated and released
  together.

### Deprecated
- Passing a format string to `winrt.system.Array` is deprecated and raises a
  `DeprecationWarning`. Pass the type of the elements instead: `bool` for
  `"?"`, `winrt.system.Int8` for `"b"`, `UInt8` for `"B"`, `Int16` for `"h"`,
  `UInt16` for `"H"`, `Int32` for `"i"`, `UInt32` for `"I"`, `Int64` for `"q"`,
  `UInt64` for `"Q"`, `Single` for `"f"`, `Double` for `"d"` and `Char16` for
  `"u"`. An array of enums was spelled `"i"` or `"I"` and is now spelled with
  the enum type. Format strings will be removed in a future release.
- `HResult.value` and `EventRegistrationToken.value` are deprecated. The value
  of each is now the object itself, so reading the field still works and
  raises a `DeprecationWarning`. It will be removed in a future release.
- The method names that v3.x generated from the
  `[Windows.Foundation.Metadata.Overload]` attribute are still available as
  aliases, but calling one raises a `DeprecationWarning`. They will be removed
  in a future release.

### Removed
- Dropped support for Python 3.9 and Python 3.10. PyWinRT now requires
  Python 3.11 or later. Python 3.9 is past its upstream end of life and
  Python 3.10 reaches its own in October 2026.

### Fixed
- Fixed a Python thread state leaking every time the projection took the GIL on
  a thread that already held it. `PyGILState_Ensure()` reports whether the
  caller already had the GIL, and the RAII wrapper used that answer as its
  "holding nothing" value, so it skipped the matching `PyGILState_Release()` in
  exactly that case. The release does not unlock anything there, but it is what
  balances the counter that decides when the state of a thread the projection
  did not create is torn down. The most common way to reach it was garbage
  collecting an object that held an event handler.
- Fixed a C++ exception of an unrecognized type crashing the process on its way
  out of a WinRT call. Only the standard exception types were caught, and
  anything else escaped a `noexcept` function and terminated the interpreter;
  they now raise `RuntimeError`.
- Fixed calling a member that an object does not implement crashing the process
  instead of raising `AttributeError`. This happened whenever the metadata of
  the member was present but the object was not, for example an object from an
  older version of a component, an object implemented in Python, or any object
  at all in a `--component-dlls` build, where the check was omitted entirely.
- Fixed `GetMany()` raising `NotImplementedError` on every collection backed by
  a Python object. WinRT code that reads a wrapped list or dict in batches -
  which is what C++/WinRT's own `to_vector()` does - failed instead of getting
  the items.
- Fixed replacing an item of a Python list passed to WinRT as an `IVector`. The
  value was released one time too many, which corrupts the interpreter, and it
  only worked at all if the object was a `list` rather than any other sequence.
- Fixed the type hint of `Matrix4x4.translation()`, which said `Vector2`
  where the value is a `Vector3`.
- Fixed `@typing.overload` missing from the type hints of overloaded methods.
- Fixed methods being silently dropped when two overloads could not be told
  apart.
- Fixed building the `Windows.Graphics.DirectX.Direct3D11` interop module on
  MSYS2. mingw-w64 now ships an interop header, so the `__has_include` check
  stopped selecting our own declarations, but the `.idl` that header is
  generated from is missing `CreateDirect3D11SurfaceFromDXGISurface`, which is
  now declared separately there.

[#139]: https://github.com/pywinrt/pywinrt/issues/139

## [v3.2.1] - 2025-06-06

## Changed
- Changed version scheme to allow for single-package bug fix releases ([#105]).

### Fixed
- Fixed possible circular import exception ([#132]).

[#105]: https://github.com/pywinrt/pywinrt/issues/105
[#132]: https://github.com/pywinrt/pywinrt/issues/132

## [v3.2.0] - 2025-06-04

### Added
- Allow passing Python mapping as arguments of type `Iterable[IKeyValuePair[K, V]]`.
- Allow passing Python buffer protocol objects as arguments of type `IBuffer`.
- Added `get()` and `wait()` methods to async operations to call them synchronously.
- Added `winrt.system.hresult` module for common error codes.

### Changed
- `asyncio` cancellation is now propagated to WinRT async actions/operations that are being awaited.
- Updated WebView2 to 1.0.3240.44.
- Updated Windows SDK to 10.0.26100.4188.
- Updated Windows App SDK to 1.7.250513003.
- Updated to CppWinRT 2.0.250303.1.
- Changed type hints from `winrt.windows.storage.streams.IBuffer` to `winrt.system.Buffer`.

### Fixed
- Fixed building with GCC 15 ([#116]).
- Fixed some exceptions being silently ignored in async methods ([#112]).
- Fixed possible `AttributeError: module 'winrt' has no attribute '_winrt'` on import.
- Fixed missing `py.typed` in `winrt.runtime` and `winrt.system` packages.

[#112]: https://github.com/pywinrt/pywinrt/issues/112
[#116]: https://github.com/pywinrt/pywinrt/issues/116

## [v3.1.0] - 2025-04-11

### Added
- Added `winrt.windows.media.interop` package.

### Changed
- Updated Windows App SDK to 1.7.250310001.
- Updated WebView2 to 1.0.3124.44.

### Fixed
- Fixed `IIterator.__iter__()` returning an object without taking a reference.
- Fixed `IIterator.__iter__()` returning an invalid object pointer, leading to a crash.

## [v3.0.0] - 2025-03-15

### Added
- Added new `winrt.runtime` module.
- Added new `winrt.runtime.interop` module.
- Added new `box_...()` and `unbox_...()` functions to `winrt.system`.
- Added support for `==`, `!=` and `hash()` of `winrt.system.Object`.
- Added `winrt.system.Object.as_()` method.
- Added `__replace__()` on projected structs for use with `copy.replace()`.
- Added `unpack()` method on projected structs with more than one field.

### Changed
- BREAKING: Namespaces other than the Windows SDK now have a different top-level
  package name other than `winrt`.
- BREAKING: Use `[Windows.Foundation.Metadata.Overload]` attribute for method
  names when available. This exposes some APIs that otherwise could not be called
  but also renames many existing methods.
- BREAKING: Projected structs are now immutable.
- Allow basic `tuple` in addition to projected struct type as argument value.
- Use more informative error messages, if available, when raising `WinError`.
- Type hints are no longer `typing.Optional` by default.
- `winrt.windows.foundation.IVector[T]` input parameters now accept any `typing.MutableSequence[T]`.
- `winrt.windows.foundation.IVectorView[T]` input parameters now accept any `typing.Sequence[T]`.
- `winrt.windows.foundation.IMap[K, V]` input parameters now accept any `typing.MutableMapping[K, V]`.
- `winrt.windows.foundation.IMapView[K, V]` input parameters now accept any `typing.Mapping[K, V]`.
- Implemented type hints for interface inheritance ([#80]).
- Implemented proper inheritance of composable classes ([#25]).
- Failing to convert from `uuid.UUID` now raises `TypeError`.
- Regenerated files for Windows 11 SDK 10.0.26100.2454.

### Fixed
- Fixed `AttributeError` when calling `winrt.microsoft.web.webview2.core` object members.
- Fixed missing `Microsoft.Web.WebView2.Core.dll`.

### Removed
- BREAKING: Removed `winrt.windows.foundation.interop` module.
- Removed `_from()` methods from type hints.

[#25]: https://github.com/pywinrt/pywinrt/issues/25
[#80]: https://github.com/pywinrt/pywinrt/issues/80

## [v2.3.0] - 2024-10-19

### Added
- Added `winrt.windows.graphics.directx.direct3d11.interop` package.

### Changed
- Regenerated files for Windows 11 SDK 10.0.26100.1742 (24H2).
- Updated to Windows App SDK 1.6.240829007

### Fixed
- Fixed type checker errors when passing Python buffer protocol object to WinRT Array parameter.

## [v2.2.0] - 2024-08-18

### Added
- Added `__repr__` implementation for projected struct types.
- Added `--component-dlls` option to PyWinRT.exe for user component packages.
- Added `winrt.windows.system.interop` package.
- Added `winrt.windows.ui.composition.interop` package.
- Added support for Python 3.13.
- Added extra operators and methods for `winrt.windows.numerics` types.

### Changed
- General performance improvements.

### Fixed
- Fixed spelling of `initialize` in `__all__` in Windows App bootstrap package.

## [v2.1.0] - 2024-06-23

### Added
- Added `winrt.Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap`
  interop package.
- Added `_iids_` and `_runtime_class_name_` properties to `system.Object`.
- Added `__eq__` and `__ne__` methods to projected structs.
- Added projection of inherited members of composed classes.

### Changed
- PyWinRT.exe is now a dotnet tool.
- Static events are moved to the metaclass.

### Fixed
- Fixed possible crash when checking if API is available at runtime.
- Fixed memory leak when converting Python object to `char16_t` or `hstring`.
- Fixed importing modules containing delegates with parameters that reference
  other modules.
- Fixed `TypeError` when trying to pass composed runtime object to API that
  requires subclass ([#25]).
- Fixed `TypeError` when passing `None` to APIs that require a `system.Object`.
- Fixed `ui.xaml.Application.start()` methods not releasing GIL.
- Fixed some type hints not working when not all dependent packages installed.
- Fixed type hints should not have `_from()` method in projected static classes.
- Fixed type hints for methods that return an array.
- Fixed handling of bool, char, string and IReference types in projected structs.

[#25]: https://github.com/pywinrt/pywinrt/issues/25

### Post-release

#### v2.1.0-post.1

Released for `winrt.Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap`
package only.

##### Fixed
- Fixed `winrt.Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap`
  package missing `Microsoft.WindowsAppRuntime.Bootstrap.dll`.


## [v2.0.1] - 2024-04-28

### Fixed
- Fixed modules with protected methods/properties failing to compile.

## [v2.0.0] - 2024-04-27

### Added
- Added `py.typed` to each subpackage.
- Added `winrt.windows.foundation.interop` module with `box` and `unbox` methods.

### Changed
- Regenerated files for Windows 11 SDK 10.0.22621.0 February 2024 update.
- Updated to Windows App SDK 1.5.240311000
- Updated to CppWinRT 2.0.240111.5

### Fixed
- Fixed implicitly converting `None` to `System.Object`.
- Fixed `KeyError` when accessing accessing values of mappings that return `None`.
- Fixed building from source package with non-MSVC compiler ([#44]).
- Fixed module load failing on systems without `msvcp140.dll` ([#43]).
- Fixed installing from source in MSYS2 CLANG64 ([#47]).
- Fixed installing from source in MSYS2 MINGW64 ([#46]).
- Fixed crash when `winrt.system.Array()` called with no arguments.

[#43]: https://github.com/pywinrt/pywinrt/issues/43
[#44]: https://github.com/pywinrt/pywinrt/issues/44
[#46]: https://github.com/pywinrt/pywinrt/issues/46
[#47]: https://github.com/pywinrt/pywinrt/issues/47

## [v2.0.0-beta.2] - 2023-11-29

### Added
- Added `@final` decorator to runtime class type hints.
- Added support for Windows App SDK (`Microsoft.Windows` namespace).

### Fixed
- Fixed missing `__iter__` type hint on sequences.
- Fixed index type on sequences index operators.
- Fixed `py.typed` and `_winrt.pyi` missing from `winrt-runtime` package.

## [v2.0.0-beta.1] - 2023-09-01

### Added
- Added `-reference` option to `pywinrt.exe`.
- Added `-header-path` option to `pywinrt.exe`.
- Added `__all__` in generated Python modules.

### Changed
- Changed code generator to generate module per namespace instead of monolith.
- Moved delegate type aliases from .pyi to .py files.
- Changed type hints for non-static properties to use `@property`.
- Moved static methods to metaclass.

### Fixed
- Fixed mixin of `__eq__` for mappings.
- Fixed parsing structs with guid field.
- Fixed single byte struct field arg format.
- Fixed importing `enum` when not needed.
- Fixed compiler warning about converting `Py_ssize_t` in `ComArray`.
- Fixed `NotImplementedError` for delegates with `array_view`.
- Fixed possible crash due to not checking return value of `convert()` in some places.
- Fixed memory leak in wrapped struct types.
- Fixed generating delegates with out parameters are return arrays.
- Fixed setters of static properties.
- Fixed subpackage names that are Python keywords.

### Removed
- Removed generation of base/core files.
- Removed `-module` option from `pywinrt.exe`.
- Removed `winrt.system.Guid` type alias.
- Removed `winrt.system.Boolean` type alias.
- Removed support for Python 3.8.

## [v1.0.0-beta.10] - 2023-08-11

### Added
- Added positional-only parameters indication to type hints.

### Changed
- Changed how `collections.abc` protocols are implemented.
- Changed type hints for buffer protocol to align with PEP 688.

### Fixed
- Fixed structs should not inherit from `_winrt.Object`.

### Removed
- Removed support for Python 3.7.

## [v1.0.0-beta.9] - 2023-04-15

### Fixed
- Fixed compiling generated code with clang and gcc.

## [v1.0.0-beta.8] - 2023-04-02

### Added
- Added new `Array` type for wrapping COM arrays.
- Added support for conversion to/from `char16_t`/`str`.
- Added support for slicing `IVectorView`.

### Changed
- Provide useful error message when `NotImplementedError` is raised.
- Use `PyType_FromModuleAndSpec` in Python >= 3.9.
- `datetime` conversion now uses Windows API instead Python APIs with float.
- Array input arguments must now implement Python buffer protocol (no longer
  accepts list/sequence).
- Moved top-level package `__init__.py` to `system` subpackage.
- Moved fundamental type aliases to `system` package and made runtime types.
- Array output parameters now return `system.Array` instead of `list`.
- Fill arrays now take a buffer as the argument and fill that buffer instead
  of taking a size as the argument and returning a new array.
- Use `typing.ClassVar` annotation for static properties.
- Static properties are now class attributes instead of static methods.

### Fixed
- Fixed checking wrong number of input parameters on methods with array parameters.
- Fixed use after free of array input arguments ([winsdk#20]).
- Fixed missing `__await__` on runtime types that inherit `IAsyncOperation` ([winsdk#21]).
- Fixed compile error in generated code for delegates that return non-void.
- Fixed `buffer_format<>` not generated for enum types.

[winsdk#20]: https://github.com/pywinrt/python-winsdk/issues/20
[winsdk#21]: https://github.com/pywinrt/python-winsdk/issues/21

## [v1.0.0-beta.7] - 2022-09-09

### Changed
- Attempting to delete attributes now returns `AttributeError` instead of `TypeError`.

### Fixed
- Fixed crash when accessing new APIs on older versions of Windows ([winsdk#13]).

[winsdk#13]: https://github.com/pywinrt/python-winsdk/issues/13

## [v1.0.0-beta.6] - 2022-06-21

### Changed
- `Windows.Foundation.DateTime` is now converted to/from `datetime.datetime`.
- `Windows.Foundation.TimeSpan` is now converted to/from `datetime.timedelta`.
- Type hints now use `__new__` instead of `__init__`.

### Fixed
- Fixed syntax error in type hints for map with generic value type.
- Fixed type hints for GUIDs.
- Fixed type hints for methods overloaded from required interfaces.

## [v1.0.0-beta.5] - 2022-06-07

### Changed
- Don't `try/except` imports in `.pyi` files.
- Don't use `typing.Optional` in generic type parameters.
- Fully implement sequence protocol for `IVector`/`IVectorView`.
- Fully implement mapping protocol for `IMap`/`IMapView`.

### Fixed
- Fixed support for sub-interpreters.
- Fixed `__enter__` and `__exit__` type hints for types that implement `IClosable`.
- Fixed duplicate `TypeVar` definitions ([pywinrt#12]).
- Fixed type hints for iterators/iterables/sequences/mappings.
- Fixed leaks in object deallocation.
- Fixed including Windows SDK extensions.

### Removed
- Removed implicit dict to struct conversion.
- Removed use of `typing.Protocol`.
- Removed `interfaces` kwarg in type hints.

[pywinrt#12]: https://github.com/pywinrt/pywinrt/issues/12

## [v1.0.0-beta.4] - 2022-04-23

### Added
- Added special case for `Windows.Graphics.Capture` to include additional
  `Interop` namespace ([winsdk#5]).

### Changed
- Changed async operation interface types to never be `typing.Optional` ([pywinrt#11]).
- Interface type hints are now `typing.Protocol`.
- Improved exception raising.

[pywinrt#11]: https://github.com/pywinrt/pywinrt/issues/11
[winsdk#5]: https://github.com/pywinrt/python-winsdk/issues/5

## [v1.0.0-beta.3] - 2022-04-09

### Changed
- Type hints for reference types are now all `typing.Optional` ([pywinrt#7]).

### Fixed
- Fixed identifiers conflicting with Python keywords ([pywinrt#6]).
- Fixed broken version comparison in `_winrt.pyi`.
- Fixed `@typing.overload` used on methods that are not overloaded.
- Fixed not throwing error if `initialize_with_window()` fails.

[pywinrt#6]: https://github.com/pywinrt/pywinrt/issues/6
[pywinrt#7]: https://github.com/pywinrt/pywinrt/issues/7

## [v1.0.0-beta.2] - 2022-02-12

### Added
- Added `__contains__` special method for map types.

### Changed
- Improved type hints.

## [v1.0.0-beta.1] - 2022-01-27

### Added
- Added this changelog.
- Implemented Python buffer protocol for `IBuffer` and `IMemoryBufferReference`.
- Added support for canceling async operations.
- Added generation of type hint files.
- Added `_winrt.initialize_with_window()` function.
- Added runtime support for PEP 585 generic types.

### Changed
- Improved error handling.
- Allow any sequence, not just `list` for `System.Array` arguments.
- Allow buffer protocol for `System.Array` arguments of fundamental types.
- Replaced use of deprecated `asyncio.get_event_loop()`.
- Make use of `DefaultOverloadAttribute` when generating overloads.
- Renamed `_winrt.winrt_base` type to `_winrt.Object`.

### Fixed
- Fixed error handling in GUID code.
- Fixed `__enter__()` and `__exit__()` methods on `IClosable`.
- Fixed memory leaks of WinRT objects.
- Fixed use after free of Python objects.

### Removed
- Removed selecting apartment thread type during module init.

[Unreleased]: https://github.com/pywinrt/pywinrt/compare/v3.2.1...HEAD
[v3.2.1]: https://github.com/pywinrt/pywinrt/compare/v3.2.0...v3.2.1
[v3.2.0]: https://github.com/pywinrt/pywinrt/compare/v3.1.0...v3.2.0
[v3.1.0]: https://github.com/pywinrt/pywinrt/compare/v3.0.0...v3.1.0
[v3.0.0]: https://github.com/pywinrt/pywinrt/compare/v2.3.0...v3.0.0
[v2.3.0]: https://github.com/pywinrt/pywinrt/compare/v2.2.0...v2.3.0
[v2.2.0]: https://github.com/pywinrt/pywinrt/compare/v2.1.0...v2.2.0
[v2.1.0]: https://github.com/pywinrt/pywinrt/compare/v2.0.1...v2.1.0
[v2.0.1]: https://github.com/pywinrt/pywinrt/compare/v2.0.0...v2.0.1
[v2.0.0]: https://github.com/pywinrt/pywinrt/compare/v2.0.0-beta.2...v2.0.0
[v2.0.0-beta.2]: https://github.com/pywinrt/pywinrt/compare/v2.0.0-beta.1...v2.0.0-beta.2
[v2.0.0-beta.1]: https://github.com/pywinrt/pywinrt/compare/v1.0.0-beta.10...v2.0.0-beta.1
[v1.0.0-beta.10]: https://github.com/pywinrt/pywinrt/compare/v1.0.0-beta.9...v1.0.0-beta.10
[v1.0.0-beta.9]: https://github.com/pywinrt/pywinrt/compare/v1.0.0-beta.8...v1.0.0-beta.9
[v1.0.0-beta.8]: https://github.com/pywinrt/pywinrt/compare/v1.0.0-beta.7...v1.0.0-beta.8
[v1.0.0-beta.7]: https://github.com/pywinrt/pywinrt/compare/v1.0.0-beta.6...v1.0.0-beta.7
[v1.0.0-beta.6]: https://github.com/pywinrt/pywinrt/compare/v1.0.0-beta.5...v1.0.0-beta.6
[v1.0.0-beta.5]: https://github.com/pywinrt/pywinrt/compare/v1.0.0-beta.4...v1.0.0-beta.5
[v1.0.0-beta.4]: https://github.com/pywinrt/pywinrt/compare/v1.0.0-beta.3...v1.0.0-beta.4
[v1.0.0-beta.3]: https://github.com/pywinrt/pywinrt/compare/v1.0.0-beta.2...v1.0.0-beta.3
[v1.0.0-beta.2]: https://github.com/pywinrt/pywinrt/compare/v1.0.0-beta.1...v1.0.0-beta.2
[v1.0.0-beta.1]: https://github.com/pywinrt/pywinrt/compare/7efce99b1115ddc50f6dbfbc163841a57596c2ec...v1.0.0-beta.1
<!-- 7efce99b1115ddc50f6dbfbc163841a57596c2ec is last release of PyWinRT from xlang project -->
