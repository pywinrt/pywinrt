<!-- Refer to https://keepachangelog.com/en/1.0.0/ for guidance. -->

# Changelog

What changed in the `winui2-Microsoft.UI.Xaml` package, which projects
WinUI 2. It keeps a prefix and a top-level package of its own because
WinUI 2 defines the same namespaces as the Windows App SDK's WinUI
component.

What changed in `winrt-runtime`, which every package runs on, is in [its own
changelog](../../runtime/CHANGELOG.md). The releases before v4.0 are in the
[changelog at the root of the repository](../../CHANGELOG.md).

## [4!2.8.7] - 2026-10-23

See the [4.0 release notes][4.0] for what these changes mean together.

### Added
- Protected constructors are projected, so a class such as `Panel` is derivable.
- Protected properties are projected, such as `Control._default_style_key`.
- Members and types WinRT marks deprecated are `@deprecated` in the type hints.

### Changed
- BREAKING: Published as one package, `winui2-Microsoft.UI.Xaml`, not seven.
- BREAKING: A frozen application must carry each module's `_table.pywinrt`.
- BREAKING: Versioned by the WinUI 2 release it projects, as `4!2.8.7`.
- BREAKING: An overloaded method has one name again, with no v3 alias.
- A package is one `py3-none-any` wheel of data, not compiled code.
- Requires `winrt-runtime>=4.0.0,<5` rather than one `~=` release of it.
- Requires another package with a floor rather than `~=`.
- The type hints use `X | None`, `collections.abc` and PEP 695 type parameters.
- The projected collections are `collections.abc` types in the type hints.
- `winrt.system.Object` is the base of every interface in the type hints.
- The type hints infer the variance of a generic interface's type parameters.
- Pyright requires an implementation of an interface to define all its members.
- The type hints no longer declare the empty private class of each interface.
- An integer parameter is typed as taking a `typing.SupportsIndex` as well.
- The wheels carry the text of their license.

### Removed
- BREAKING: Python 3.9 and 3.10 are no longer supported.

### Fixed
- Fixed `@typing.overload` missing from the type hints of overloaded methods.
- Fixed methods being dropped when two overloads could not be told apart.
- Fixed the type hints of a handler refusing one that returns a value.
- Fixed the type hints of a struct passed by reference refusing a tuple.
- Fixed the type hints of a struct's constructor refusing a tuple for a field.
- Fixed `[all]` leaving out the packages of the interfaces a class implements.
- Fixed the type hints incorrectly accepting keywords for a constructor.
- Fixed type hints incorrectly accepting a call to a class with no constructor.

[4.0]: https://pywinrt.readthedocs.io/en/latest/release-notes/4.0/
