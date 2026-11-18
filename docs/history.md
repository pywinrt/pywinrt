# History

PyWinRT began as a fork of the `pywinrt` code generator from Microsoft's
[xlang](https://github.com/microsoft/xlang/) project and is now an independent
project. Its Python packages have gone by several names:

- **April 2019 to February 2021:** Microsoft published the bindings as one
  package, [winrt](https://pypi.org/project/winrt/).
- **January 2022 to August 2023:** the fork published updates as
  [winsdk](https://pypi.org/project/winsdk/).
- **September 2023, the 2.0 betas:** the bindings were split into one package
  per Windows SDK namespace, and the top-level package went back from `winsdk`
  to `winrt`.
- **March 2025, version 3.0:** the Windows App SDK and WebView2 packages moved
  from `winrt` to top-level packages of their own, `winui3` and `webview2`, and
  `winui2` was added for WinUI 2.
- **October 2026, version 4.0:** the Windows App SDK and WebView2 packages
  moved back under `winrt`, leaving `winui2` as the only other top-level
  package. The Windows App SDK is published one package per NuGet component,
  and WinUI 2 as one package. [PyWinRT 4.0](release-notes/4.0.md) describes
  this and the other changes.
