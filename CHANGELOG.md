<!-- Refer to https://keepachangelog.com/en/1.0.0/ for guidance. -->

# Changelog

## [Unreleased]

## [v1.0.0-beta.4] - 2022-04-23

### Added
- Added special case for `Windows.Graphics.Capture` to include additional
  `Interop` namespace ([winsdk#5]).

### Changed
- Changed async operation interface types to never be `typing.Optional` ([issue#11]).
- Interface type hints are now `typing.Protocol`.
- Improved exception raising.

[issue#11]: https://github.com/pywinrt/pywinrt/issues/11
[winsdk#5]: https://github.com/pywinrt/python-winsdk/issues/5

## [v1.0.0-beta.3] - 2022-04-09

### Changed
- Type hints for reference types are now all `typing.Optional` ([issue#7]).

### Fixed
- Fixed identifiers conflicting with Python keywords ([issue#6]).
- Fixed broken version comparison in `_winrt.pyi`.
- Fixed `@typing.overload` used on methods that are not overloaded.
- Fixed not throwing error if `initialize_with_window()` fails.

[issue#6]: https://github.com/pywinrt/pywinrt/issues/6
[issue#7]: https://github.com/pywinrt/pywinrt/issues/7

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

[Unreleased]: https://github.com/pywinrt/pywinrt/compare/v1.0.0-beta.4...HEAD
[v1.0.0-beta.4]: https://github.com/pywinrt/pywinrt/compare/v1.0.0-beta.3...v1.0.0-beta.4
[v1.0.0-beta.3]: https://github.com/pywinrt/pywinrt/compare/v1.0.0-beta.2...v1.0.0-beta.3
[v1.0.0-beta.2]: https://github.com/pywinrt/pywinrt/compare/v1.0.0-beta.1...v1.0.0-beta.2
[v1.0.0-beta.1]: https://github.com/pywinrt/pywinrt/compare/7efce99b1115ddc50f6dbfbc163841a57596c2ec...v1.0.0-beta.1
<!-- 7efce99b1115ddc50f6dbfbc163841a57596c2ec is last release of PyWinRT from xlang project -->
