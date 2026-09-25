<!-- Refer to https://keepachangelog.com/en/1.0.0/ for guidance. -->

# Changelog

What changed in the `winrt-Microsoft.WindowsAppSDK.*` packages, which
project the Windows App SDK, and in the hand-written interop modules
beside them.

Everything that affects every PyWinRT package - `winrt-runtime`, the code
generator, the versioning scheme and the packaging - is in the [changelog at
the root of the repository](../../CHANGELOG.md), which is also where the
releases before v4.0 are.

## [Unreleased]

### Changed
- Updated the Windows App SDK to 2.5.1, from 1.7.250513003, whose servicing
  ended in March. Nine namespaces are new and have a package each:
  `Microsoft.Windows.AI.Foundation`, `Microsoft.Windows.AI.MachineLearning`,
  `Microsoft.Windows.AI.Video`, `Microsoft.Windows.Vision`,
  `Microsoft.Windows.SemanticSearch`, `Microsoft.Windows.Search.AppContentIndex`,
  `Microsoft.Windows.Storage.Pickers`, `Microsoft.Windows.Foundation` and
  `Microsoft.UI.Xaml.Settings`. No namespace was removed, so the family goes
  from 69 packages to 78. Everything in it is now versioned `4!2.5.1`, the
  version of the `Microsoft.WindowsAppSDK` metapackage, which is the version
  Microsoft's own release notes and runtime installer speak.
- BREAKING: The Windows App SDK is published as one package per NuGet
  component instead of one per namespace, so its 78 packages become 7:
  `winrt-Microsoft.WindowsAppSDK.Foundation`, `.InteractiveExperiences`,
  `.WinUI`, `.Widgets`, `.AI`, `.Search`, and
  `winrt-Microsoft.Windows.AI.MachineLearning`. A package now carries every
  namespace its component owns - `.WinUI` has all 25 `Microsoft.UI.Xaml.*`
  modules and `Microsoft.UI.Text` - and its README lists them and names the
  component and version its metadata came from. Which module a namespace is
  imported from is unaffected; what changes is which package to install to
  get it. The whole family
  is 1.2 MB of wheels, and a package that hands back a type from another
  component of the same release requires it outright, since Microsoft's own
  build refuses a project that mixes component versions.
- The Windows App SDK is projected from its component packages rather than
  from the metapackage, which has carried no metadata since 2.0. That is what
  lets a release be projected at all, and it is also what decides the
  packages above: a namespace is published in the component that defines it,
  and one that several components contribute to would be published on its own
  instead. In this release none is.
- `Microsoft.WindowsAppRuntime.Bootstrap.dll` is put into
  `winrt-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap` by the
  build that produces the wheel rather than copied in afterwards, so a wheel
  built from the source distribution carries it too. The wheel also carries the
  Windows App SDK license text beside PyWinRT's own and a CycloneDX bill of
  materials naming the redistributed `.dll`, its version and Microsoft as its
  supplier.
