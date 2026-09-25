<!-- Refer to https://keepachangelog.com/en/1.0.0/ for guidance. -->

# Changelog

What changed in `winrt-Microsoft.Web.WebView2`, which projects the
WebView2 SDK, and in `winrt-Microsoft.Web.WebView2.Dll`, which
redistributes the `.dll` behind it.

Everything that affects every PyWinRT package - `winrt-runtime`, the code
generator, the versioning scheme and the packaging - is in the [changelog at
the root of the repository](../../CHANGELOG.md), which is also where the
releases before v4.0 are.

## [Unreleased]

### Added
- Added `winrt-Microsoft.Web.WebView2.Dll`, which redistributes
  `Microsoft.Web.WebView2.Core.dll`. The projection is one `py3-none-any` wheel
  and the `.dll` is built for one architecture, so the two cannot be published
  together any more; `winrt-Microsoft.Web.WebView2` requires this package and
  imports it, which is what puts the `.dll` on the DLL search path, so nothing
  in an application has to change. It also carries the WebView2 license text
  and a CycloneDX bill of materials naming what is in it. Its source
  distribution holds no `.dll`; building one downloads the NuGet package and
  checks it against a pinned SHA-256, or takes the `.dll` out of an unpacked
  copy named by the `WEBVIEW2_PATH` environment variable, which is how a
  machine with no access to `nuget.org` builds it.

### Changed
- Updated WebView2 to 1.0.4191.47. Twelve types are new, covering the worker
  APIs - `CoreWebView2ServiceWorker`, `CoreWebView2DedicatedWorker`,
  `CoreWebView2SharedWorker` and the managers and event arguments that go with
  them - and find-on-page, `CoreWebView2Find` and `CoreWebView2FindOptions`.
  Nothing was removed.
