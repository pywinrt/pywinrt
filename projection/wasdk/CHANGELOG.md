<!-- Refer to https://keepachangelog.com/en/1.0.0/ for guidance. -->

# Changelog

What changed in the packages that project the Windows App SDK:

- `winrt-Microsoft.WindowsAppSDK.AI`
- `winrt-Microsoft.WindowsAppSDK.Foundation`
- `winrt-Microsoft.WindowsAppSDK.InteractiveExperiences`
- `winrt-Microsoft.WindowsAppSDK.Search`
- `winrt-Microsoft.WindowsAppSDK.Widgets`
- `winrt-Microsoft.WindowsAppSDK.WinUI`
- `winrt-Microsoft.Windows.AI.MachineLearning`

and in the two interop packages released with them:

- `winrt-Microsoft.UI.Interop`
- `winrt-Microsoft.WindowsAppRuntime.Bootstrap`

What changed in `winrt-runtime`, which every package runs on, is in [its own
changelog](../../runtime/CHANGELOG.md). The releases before v4.0 are in the
[changelog at the root of the repository](../../CHANGELOG.md).

## [Unreleased]

### Changed
- Updated the Windows App SDK to [2.6.4][Windows App SDK 2.6.4], from 2.5.1.

## [4!2.5.1.1] - 2026-10-24

Only the packages these changes touch are released, and the rest, the interop
packages among them, stay at `4!2.5.1`.

### Changed
- The type hints declare the constructors of derivable classes as `__init__`.

## [4!2.5.1] - 2026-10-23

See the [4.0 release notes][4.0] for what these changes mean together.

### Added
- Nine new namespaces, such as `Microsoft.Windows.Storage.Pickers`.
- Protected constructors are projected, so a class such as `Panel` is derivable.
- Protected properties are projected, such as `Control._default_style_key`.
- Members and types WinRT marks deprecated are `@deprecated` in the type hints.
- The interop packages' wheels are published for 3.14, 3.15, 3.14t and 3.15t.

### Changed
- BREAKING: `winui3-*` is now `winrt-*`, imported from `winrt.microsoft`.
- BREAKING: Published as 7 packages, one per NuGet component, not per namespace.
- BREAKING: Bootstrap package is `winrt-Microsoft.WindowsAppRuntime.Bootstrap`.
- BREAKING: A frozen application must carry each module's `_table.pywinrt`.
- BREAKING: Versioned by the Windows App SDK release it projects, as `4!2.5.1`.
- BREAKING: An overloaded method has one name again, with no v3 alias.
- A package is one `py3-none-any` wheel of data, not compiled code.
- Updated the Windows App SDK to 2.5.1, from 1.7.250513003.
- The two interop packages are released with the family and carry its version.
- Requires `winrt-runtime>=4.0.0,<5` rather than one `~=` release of it.
- Requires a component it hands back types of with `~=`, others with a floor.
- A wheel built from the bootstrap package's sdist carries its `.dll` too.
- The bootstrap package's wheel carries the App SDK license and an SBOM.
- `bootstrap.initialize()` and its shutdown release the GIL while they wait.
- `winrt-Microsoft.UI.Interop` builds from its sdist with nothing from PyWinRT.
- `MSVCP140.dll` is no longer needed by the interop packages.
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
- The wheels carry the text of their license.

### Removed
- BREAKING: Python 3.9 and 3.10 are no longer supported.

### Fixed
- Fixed `@typing.overload` missing from the type hints of overloaded methods.
- Fixed methods being dropped when two overloads could not be told apart.
- Fixed the type hints of a handler refusing one that returns a value.
- Fixed the type hints of a struct passed by reference refusing a tuple.
- Fixed the type hints of a struct's constructor refusing a tuple for a field.
- Fixed the type hints incorrectly accepting keywords for a constructor.
- Fixed type hints incorrectly accepting a call to a class with no constructor.
- Fixed the type hints incorrectly typing an array property as one element.
- Fixed `bootstrap.RELEASE_VERSION`, `RUNTIME_VERSION` and `Shutdown` missing.
- Fixed the interop packages declaring no dependency on what they import.
- Fixed the interop packages' sdists needing environment variables to build.

[4.0]: https://pywinrt.readthedocs.io/en/latest/release-notes/4.0/
[Windows App SDK 2.6.4]: https://github.com/microsoft/WindowsAppSDK/releases/tag/v2.6.4
