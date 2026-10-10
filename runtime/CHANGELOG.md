<!-- Refer to https://keepachangelog.com/en/1.0.0/ for guidance. -->

# Changelog

What changed in `winrt-runtime`, the package that every PyWinRT projection
package runs on.

The releases before v4.0 are in the [changelog at the root of the
repository](../CHANGELOG.md).

## [Unreleased]

### Added
- An `IMemoryBuffer` such as a `MemoryBuffer` now supports the buffer protocol.

### Changed
- `op.completed = None` and `op.progress = None` now raise `TypeError`.

### Deprecated
- `IMemoryBuffer.create_reference()` warns; use the buffer protocol instead.

### Fixed
- Fixed `Array(T, list)` reading freed memory if a conversion edits the list.
- Fixed closing an `IMemoryBufferReference` with a live `memoryview` crashing.
- Fixed `_measure_override()` and the like never called on a `Grid` subclass.
- Fixed iterating over a WinRT map, or its keys or items, leaking memory.
- Fixed WinRT failing to set, or losing, the length of a Python buffer it fills.
- Fixed WinRT writing into `bytes`; a buffer WinRT fills must be writable.
