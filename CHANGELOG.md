<!-- Refer to https://keepachangelog.com/en/1.0.0/ for guidance. -->

# Changelog

## [Unreleased]

### Added
- Added `-reference` option to `pywinrt.exe`.
- Added `-header-path` option to `pywinrt.exe`.

### Fixed
- Fixed mixin of `__eq__` for mappings.
- Fixed parsing structs with guid field.
- Fixed single byte struct field arg format.
- Fixed importing `enum` when not needed.
- Fixed compiler warning about converting `Py_ssize_t` in `ComArray`.

### Removed
- Removed generation of base/core files.

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

[Unreleased]: https://github.com/pywinrt/pywinrt/compare/v1.0.0-beta.10...HEAD
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
