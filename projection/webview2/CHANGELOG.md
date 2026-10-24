<!-- Refer to https://keepachangelog.com/en/1.0.0/ for guidance. -->

# Changelog

What changed in `winrt-Microsoft.Web.WebView2`, which projects the
WebView2 SDK, and in `winrt-Microsoft.Web.WebView2.Dll`, which
redistributes the `.dll` behind it.

What changed in `winrt-runtime`, which every package runs on, is in [its own
changelog](../../runtime/CHANGELOG.md). The releases before v4.0 are in the
[changelog at the root of the repository](../../CHANGELOG.md).

## [4!1.0.4314.35] - 2026-10-24

### Added
- `CoreWebView2CrashReport`, from `CoreWebView2ProcessFailedEventArgs.crash_report`.

### Changed
- Updated WebView2 to 1.0.4314.35, from 1.0.4191.47.

## [4!1.0.4191.47.1] - 2026-10-24

`winrt-Microsoft.Web.WebView2.Dll` is not released again and stays at
`4!1.0.4191.47`.

### Changed
- The type hints declare the constructors of derivable classes as `__init__`.

## [4!1.0.4191.47] - 2026-10-23

See the [4.0 release notes][4.0] for what these changes mean together.

### Added
- Added `winrt-Microsoft.Web.WebView2.Dll`, which carries the `.dll` it needs.
- Added `load()` to `winrt.microsoft.web.webview2.dll` for WinUI on App SDK 2.x.
- The worker and find-on-page APIs, such as `CoreWebView2Find`.

### Changed
- BREAKING: Renamed from `webview2-Microsoft.Web.WebView2.Core`.
- BREAKING: Imported from `winrt.microsoft.web.webview2.core`, not `webview2`.
- BREAKING: A frozen application must carry each module's `_table.pywinrt`.
- BREAKING: Versioned by the WebView2 SDK it projects, as `4!1.0.4191.47`.
- BREAKING: An overloaded method has one name again, with no v3 alias.
- A package is one `py3-none-any` wheel of data, not compiled code.
- Updated WebView2 to 1.0.4191.47, from 1.0.3240.44.
- Requires `winrt-runtime>=4.0.0,<5` rather than one `~=` release of it.
- Requires another package with a floor rather than `~=`.
- Building the `.Dll` package's sdist downloads the `.dll` and checks its hash.
- The `.Dll` package's wheel carries the WebView2 license and an SBOM.
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
- Fixed the type hints incorrectly accepting keywords for a constructor.
- Fixed type hints incorrectly accepting a call to a class with no constructor.

[4.0]: https://pywinrt.readthedocs.io/en/latest/release-notes/4.0/
