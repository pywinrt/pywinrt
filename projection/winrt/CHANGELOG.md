<!-- Refer to https://keepachangelog.com/en/1.0.0/ for guidance. -->

# Changelog

What changed in the `winrt-Windows.*` packages, which project the
Windows SDK.

What changed in `winrt-runtime`, which every package runs on, is in [its own
changelog](../../runtime/CHANGELOG.md), and each interop package, such as
`winrt-Windows.System.Interop`, has a changelog in its directory under
`interop/`. The releases before v4.0 are in the [changelog at the root of the
repository](../../CHANGELOG.md).

## [4!10.0.28000.2705] - Unreleased

See the [4.0 release notes][4.0] for what these changes mean together.

### Added
- Seven new namespaces, such as `Windows.AI.Agents.Mcp`, have a package each.
- Protected constructors are projected, so a class such as `Panel` is derivable.
- Protected properties are projected, such as `Control._default_style_key`.
- Members and types WinRT marks deprecated are `@deprecated` in the type hints.
- The type hints of classes that implement `IMemoryBuffer` have `__buffer__`.
- The type hints of the numerics structs have `__buffer__`, `@` and indexing.
- The type hints of `IBuffer` and `IMemoryBufferReference` have `__len__`.

### Changed
- BREAKING: A frozen application must carry each module's `_table.pywinrt`.
- BREAKING: Versioned by the Windows SDK it projects, as `4!10.0.28000.2705`.
- BREAKING: An overloaded method has one name again, called by argument count.
- BREAKING: A `TileUpdateManagerForUser` method was renamed, with no alias kept.
- A package is one `py3-none-any` wheel of data, not compiled code.
- Updated the Windows SDK to 10.0.28000.2705, from 10.0.26100.4188.
- Requires `winrt-runtime>=4.0.0,<5` rather than one `~=` release of it.
- Requires another package with a floor, or `~=` where a class derives from it.
- Requires the packages of `IAsyncOperation`, `IVectorView` and `IBuffer`.
- `HResult` and `EventRegistrationToken` are `int` subclasses, not structs.
- More return types include `None`, in `Windows.Storage`, Bluetooth and more.
- The type hints use `X | None`, `collections.abc` and PEP 695 type parameters.
- The projected collections are `collections.abc` types in the type hints.
- `winrt.system.Object` is the base of every interface in the type hints.
- The type hints infer the variance of a generic interface's type parameters.
- Pyright requires an implementation of an interface to define all its members.
- The type hints say a map has no hash, so a type checker flags `{a_map}`.
- An `IBuffer` that goes in is typed as `ReadableBuffer` or `WriteableBuffer`.
- An `IBuffer` that comes out is typed as one, not as `winrt.system.Buffer`.
- The type hints no longer declare the empty private class of each interface.
- An integer parameter is typed as taking a `typing.SupportsIndex` as well.
- A numerics method's struct parameter is typed as taking a tuple as well.
- The type hints of the `completed` and `progress` getters include `None`.
- The wheels carry the text of their license.

### Deprecated
- The v3 names of overloaded methods warn, in favor of the one name.
- `HResult.value` and `EventRegistrationToken.value` warn, as each is an `int`.
- `create_reference()` is marked `@deprecated` in the type hints.

### Removed
- BREAKING: Python 3.9 and 3.10 are no longer supported.
- BREAKING: Removed `winrt-Windows.AI.ModelContextProtocol`, as the SDK did.

### Fixed
- Fixed `@typing.overload` missing from the type hints of overloaded methods.
- Fixed methods being dropped when two overloads could not be told apart.
- Fixed the type hints of a handler refusing one that returns a value.
- Fixed the type hints of a struct passed by reference refusing a tuple.
- Fixed the type hints of a struct's constructor refusing a tuple for a field.
- Fixed `[all]` leaving out the packages of the interfaces a class implements.
- Fixed the type hints incorrectly accepting keywords for a constructor.
- Fixed type hints incorrectly accepting a call to a class with no constructor.
- Fixed the type hint of `Matrix4x4.translation()`, which is a `Vector3`.
- Fixed the type hints incorrectly typing an array property as one element.

[4.0]: https://pywinrt.readthedocs.io/en/latest/release-notes/4.0/
