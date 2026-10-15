<!-- Refer to https://keepachangelog.com/en/1.0.0/ for guidance. -->

# Changelog

What changed in `winrt-runtime`, the package that every PyWinRT projection
package runs on.

The releases before v4.0 are in the [changelog at the root of the
repository](../CHANGELOG.md).

## [Unreleased]

### Added
- Python classes can derive from composable classes with protected constructors.
- An `IMemoryBuffer` such as a `MemoryBuffer` now supports the buffer protocol.
- Added `winrt.runtime.interop.get_activation_factory()`, for interop modules.
- A vector's slices can be assigned and deleted, with a step of 1.
- `copy.copy()` and `copy.deepcopy()` of a struct return the struct itself.
- `winrt.system.Object_Static`, the metaclass that the type hints name.
- A numerics struct exports a buffer, so NumPy reads one without a copy.
- `@` is the matrix and quaternion product, and `vector @ matrix` the transform.
- Numerics vectors and quaternions are sequences, and `m[r, c]` reads a matrix.
- A float buffer is taken where a numerics struct or an array of them is.

### Changed
- BREAKING: A map is unhashable, like a `dict`, since it compares by content.
- An unraisable report names the handler, method or property WinRT called.
- A character a `Char16` cannot hold, such as an emoji, raises `ValueError`.
- A negative unsigned integer raises `ValueError`, not `OverflowError`.
- An `async def` function given as a delegate handler now raises `TypeError`.
- `op.completed = None` and `op.progress = None` now raise `TypeError`.
- Adding `None` as an event handler now raises `TypeError` instead of crashing.
- `init_apartment()` raises `ValueError` for an unknown type, not `OSError`.
- Unboxing a value boxed as another type raises `TypeError`, not `OSError`.
- A missing package's type raises `ModuleNotFoundError`, not `AttributeError`.
- BREAKING: `Matrix * Matrix` raises `TypeError`, since `@` is the product.
- BREAKING: An `Array` of numerics structs exports a float buffer, as `(n, 3)`.
- BREAKING: A numerics operator takes the struct or a number, not a tuple.

### Deprecated
- `winrt.system.Buffer` warns, in favor of `collections.abc.Buffer`.
- `IMemoryBuffer.create_reference()` warns; use the buffer protocol instead.

### Removed
- Removed the undocumented `_guid_()` of interface types.
- Removed the undocumented `_assign_array_()` of projected types.

### Fixed
- Fixed the `box_*()` and `unbox_*()` type hints incorrectly accepting keywords.
- Fixed `Array()` refusing an iterable other than a `list` or a `tuple`.
- Fixed `UInt32` and `UInt64` refusing an object with `__index__()`.
- Fixed an enum value newer than the projection raising `ValueError`.
- Fixed a too long `timedelta` wrapping; it now raises `OverflowError`.
- Fixed `Array(T, list)` reading freed memory if a conversion edits the list.
- Fixed closing an `IMemoryBufferReference` with a live `memoryview` crashing.
- Fixed `_measure_override()` and the like never called on a `Grid` subclass.
- Fixed iterating over a WinRT map, or its keys or items, leaking memory.
- Fixed WinRT failing to set, or losing, the length of a Python buffer it fills.
- Fixed WinRT writing into `bytes`; a buffer WinRT fills must be writable.
- Fixed a subclass of a composable class taking a base that is not an interface.
