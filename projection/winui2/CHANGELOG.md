<!-- Refer to https://keepachangelog.com/en/1.0.0/ for guidance. -->

# Changelog

What changed in the `winui2-Microsoft.UI.Xaml` package, which projects
WinUI 2. It keeps a prefix and a top-level package of its own because
WinUI 2 defines the same namespaces as the Windows App SDK's WinUI
component.

Everything that affects every PyWinRT package - `winrt-runtime`, the code
generator, the versioning scheme and the packaging - is in the [changelog at
the root of the repository](../../CHANGELOG.md), which is also where the
releases before v4.0 are.

## [Unreleased]

### Changed
- BREAKING: WinUI 2 is published as one package, `winui2-Microsoft.UI.Xaml`,
  named after the NuGet package it is generated from, instead of one package
  per namespace. The six `winui2-Microsoft.UI.Xaml.*` packages -
  `.Automation.Peers`, `.Controls`, `.Controls.AnimatedVisuals`,
  `.Controls.Primitives`, `.Media` and `.XamlTypeInfo` - are gone, and their
  modules are all in this one. Which module a namespace is imported from is
  unaffected; what changes is which package to install to get it. The whole
  family is one 0.13 MB wheel.
