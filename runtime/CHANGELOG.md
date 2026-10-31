<!-- Refer to https://keepachangelog.com/en/1.0.0/ for guidance. -->

# Changelog

What changed in `winrt-runtime`, the package that every PyWinRT projection
package runs on.

The releases before v4.0 are in the [changelog at the root of the
repository](../CHANGELOG.md).

## [Unreleased]

### Added
- A name a projection renames keeps working, with a `DeprecationWarning`.

## [4.0.3] - 2026-10-24

### Fixed
- `threading.local` and `contextvars` carry over between WinRT callbacks.
- A second thread composing a subclass instance is refused rather than racing.

## [4.0.2] - 2026-10-24

### Changed
- A subclass's `__init__()` has no WinRT members until `super().__init__()`.

### Fixed
- A composable subclass's `__init__` takes the arguments it declares.

## [4.0.1] - 2026-10-24

### Fixed
- Fixed a struct holding a struct of another namespace, such as `Point`, as empty.

## [4.0.0] - 2026-10-23

See the [4.0 release notes][4.0] for what these changes mean together.

### Added
- Wheels are published for Python 3.14 and 3.15.
- Wheels are published for the free-threaded Python 3.14t and 3.15t.
- A PyInstaller hook, so that freezing an application needs no configuration.
- Added `as_interface()` and `wrap_interface()` to `winrt.runtime.interop`.
- Added `hresult_error()` and `get_activation_factory()` to the same module.
- `Array` takes the type of its elements, such as `Int32`, `bool` or an enum.
- The `winrt.system` aliases carry their buffer format and WinRT signature.
- A slice of an `Array` is an `Array`, and `==` compares two arrays by value.
- The `repr()` of an `Array` shows its element type and its items.
- Structs are hashable by their fields and have `__match_args__`.
- `len()` of an `IBuffer` is its length, and of a buffer reference its capacity.
- An `IMemoryBuffer` such as a `MemoryBuffer` now supports the buffer protocol.
- A vector's slices can be assigned and deleted, with a step of 1.
- `copy.copy()` and `copy.deepcopy()` of a struct return the struct itself.
- Python classes can derive from composable classes with protected constructors.
- A composable subclass names its runtime class with `runtime_class_name=`.
- `winrt.system.Object_Static`, the metaclass that the type hints name.
- A numerics struct exports a buffer, so NumPy reads one without a copy.
- `@` is the matrix and quaternion product, and `vector @ matrix` the transform.
- A number multiplies a matrix or quaternion from the left, as it does a vector.
- Numerics vectors and quaternions are sequences, and `m[r, c]` reads a matrix.
- A float buffer is taken where a numerics struct or an array of them is.

### Changed
- BREAKING: An `Array` is a `Sequence`, so it has no `insert()` or `del a[i]`.
- BREAKING: An `Array` compares by value, so it is no longer hashable.
- BREAKING: A map is unhashable, like a `dict`, since it compares by content.
- BREAKING: `Matrix * Matrix` raises `TypeError`, since `@` is the product.
- BREAKING: An `Array` of numerics structs exports a float buffer, as `(n, 3)`.
- BREAKING: A numerics operator takes the struct or a number, not a tuple.
- A Python implementation of an interface is a `winrt.system.Object`.
- A Python implementation overrides every overload of a method with one method.
- `isinstance()` with an interface asks the object for it, as `as_()` does.
- `issubclass()` answers for a projected class and an interface it implements.
- `isinstance()` with a bare generic interface raises `TypeError`.
- Calling an interface type, such as `IStringable()`, raises `TypeError`.
- `as_()` given something it does not take raises a `TypeError` that says so.
- `==` with an object that is not from WinRT lets that object answer.
- The `Object` and `Array` types say they are in `winrt.system`, not `_winrt`.
- The `winrt.system.hresult` constants are `HResult`s, not plain `int`s.
- A vector counts a negative index from the end and raises `IndexError`.
- `insert()` on a vector takes any position, as `list.insert()` does.
- An index that a Python list passed to WinRT lacks is reported as `E_BOUNDS`.
- An array parameter of strings or objects takes only an `Array` of its type.
- `Array(T, buffer)` is refused if `T` holds strings or objects.
- An `Array` of strings or objects exports a read-only buffer.
- Assigning a string or object to an exported `Array` raises `BufferError`.
- `Array(T)` takes `uuid.UUID` for a Guid, and no other class named `UUID`.
- A fill array that a handler is given cannot be exported as a buffer.
- An empty `IBuffer` or a closed `IMemoryBufferReference` is false.
- Awaiting an async operation that has finished returns its result again.
- An operation that fails after its task stopped waiting reports to the loop.
- A member this Windows lacks raises an `AttributeError` naming it when called.
- A projection package without its table raises an `ImportError` naming both.
- `MSVCP140.dll` is no longer shipped, and nothing needs it.
- Updated to C++/WinRT 3.0.260818.1.
- An unraisable report names the handler, method or property WinRT called.
- A character a `Char16` cannot hold, such as an emoji, raises `ValueError`.
- A negative unsigned integer raises `ValueError`, not `OverflowError`.
- An `async def` function given as a delegate handler now raises `TypeError`.
- `op.completed = None` and `op.progress = None` now raise `TypeError`.
- Adding `None` as an event handler now raises `TypeError` instead of crashing.
- `init_apartment()` raises `ValueError` for an unknown type, not `OSError`.
- Unboxing a value boxed as another type raises `TypeError`, not `OSError`.
- A missing package's type raises `ModuleNotFoundError`, not `AttributeError`.
- `Array()` raises `BufferError` for a buffer of the wrong layout, as calls do.
- The integer inputs of `box_*()`, `Array()` and more are typed `SupportsIndex`.
- A `str` given where WinRT takes a collection of strings raises `TypeError`.
- A `DateTime` after the year 9999 raises `OverflowError`, not `ValueError`.

### Deprecated
- `Array("i", ...)` and the other format strings warn, in favor of the type.
- `Type._from(obj)` warns, in favor of `obj.as_(Type)`.
- `winrt.system.Buffer` warns, in favor of `collections.abc.Buffer`.

### Removed
- BREAKING: Python 3.9 and 3.10 are no longer supported.
- BREAKING: Removed the `_C_API` capsule, so a v3 projection package fails.
- BREAKING: `winrt-sdk` is gone, since no package compiles against it any more.
- BREAKING: Importing the runtime into a subinterpreter raises `ImportError`.
- Removed the undocumented `_guid_()` of interface types.
- Removed the undocumented `_assign_array_()` of projected types.

### Fixed
- Fixed a crash when Windows let go of a `bytes` passed as an `IBuffer`.
- Fixed deadlocks with objects in another apartment, such as a `StorageFile`.
- Fixed activating an out-of-process class stopping every other Python thread.
- Fixed a crash at exit when WinRT released a Python object after finalization.
- Fixed a thread state leaking each time the GIL was taken where it was held.
- Fixed a C++ exception of an unknown type crashing instead of raising.
- Fixed calling a member that an object does not implement crashing.
- Fixed `GetMany()` raising `NotImplementedError` on a Python-backed collection.
- Fixed replacing an item of a list passed as an `IVector` corrupting Python.
- Fixed a handler's writes into a fill array never reaching WinRT.
- Fixed `as_(winrt.system.Object)` raising `AttributeError`.
- Fixed the `Array` type hints refusing a tuple of more than one item.
- Fixed the `Array()` type hints incorrectly accepting keywords.
- Fixed the `box_*()` and `unbox_*()` type hints incorrectly accepting keywords.
- Fixed `Array()` refusing an iterable other than a `list` or a `tuple`.
- Fixed `UInt32` and `UInt64` refusing an object with `__index__()`.
- Fixed an enum value newer than the projection raising `ValueError`.
- Fixed a `timedelta` too long for a `TimeSpan` wrapping instead of raising.
- Fixed an aware `datetime` or a `DateTime` before 1601 raising `OSError`.
- Fixed `Array(T, list)` reading freed memory if a conversion edits the list.
- Fixed closing an `IMemoryBufferReference` with a live `memoryview` crashing.
- Fixed `_measure_override()` and the like never called on a `Grid` subclass.
- Fixed iterating over a WinRT map, or its keys or items, leaking memory.
- Fixed WinRT failing to set, or losing, the length of a Python buffer it fills.
- Fixed WinRT writing into `bytes` and other read-only buffers.
- Fixed a subclass of a composable class taking a base that is not an interface.
- Fixed a frozen application lacking `uuid`, which the runtime imports to load.

[4.0]: https://pywinrt.readthedocs.io/en/latest/release-notes/4.0/
