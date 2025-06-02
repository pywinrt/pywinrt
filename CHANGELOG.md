<!-- Refer to https://keepachangelog.com/en/1.0.0/ for guidance. -->

# Changelog

## [Unreleased]

### Added
- Allow passing Python mapping as arguments of type `Iterable[IKeyValuePair[K, V]]`.
- Allow passing Python buffer protocol objects as arguments of type `IBuffer`.
- Added `get()` and `wait()` methods to async operations to call them synchronously.
- Added `winrt.system.hresult` module for common error codes.

### Changed
- `asyncio` cancellation is now propagated to WinRT async actions/operations that are being awaited.
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

[Unreleased]: https://github.com/pywinrt/pywinrt/compare/v3.1.0...HEAD
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
