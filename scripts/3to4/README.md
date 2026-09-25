# PyWinRT v3 to v4 migration guide

PyWinRT v4 renames the packages and the Python modules of the Windows App SDK
and WebView2 projections, and publishes the Windows App SDK and WinUI 2 in
fewer, larger packages. This describes what you need to change in your own
code and in your dependency lists. Everything else that changed in v4 is in
[CHANGELOG.md](../../CHANGELOG.md).

## The `winui3` and `webview2` top-level packages are gone

v3 gave the Windows App SDK and WebView2 a top-level Python package and a
distribution prefix of their own. They are back under `winrt`, where they were
in v2:

| v3 import                                 | v4 import                               |
| ----------------------------------------- | --------------------------------------- |
| `winui3.microsoft.ui.xaml`                | `winrt.microsoft.ui.xaml`               |
| `winui3.microsoft.ui.interop`             | `winrt.microsoft.ui.interop`            |
| `winui3.microsoft.windows.appnotifications` | `winrt.microsoft.windows.appnotifications` |
| `webview2.microsoft.web.webview2.core`    | `winrt.microsoft.web.webview2.core`     |

The rule is the whole of it: replace the leading `winui3.` or `webview2.` with
`winrt.` and leave the rest of the module name alone.

The `winui2` top-level package stays. WinUI 2 defines the same
`Microsoft.UI.Xaml.*` namespaces as the Windows App SDK's WinUI component, so
the two cannot be imported from one package, and keeping WinUI 2 apart is the
only thing the v3 prefixes were needed for. Its distributions are regrouped
below.

## Renamed distributions

`webview2-Microsoft.Web.WebView2.Core` is now `winrt-Microsoft.Web.WebView2`.
The distribution is named after the `Microsoft.Web.WebView2` NuGet package it
is generated from rather than after the one namespace in it, which is how the
Windows App SDK packages below are named too. The module you import is still
`winrt.microsoft.web.webview2.core`.

The Windows App SDK is also published one distribution per NuGet component
instead of one per namespace, so a `winui3-<Namespace>` package becomes the
component package that carries `<Namespace>`:

| Namespace                                      | v4 distribution                                 |
| ---------------------------------------------- | ----------------------------------------------- |
| `Microsoft.Graphics.DirectX`                   | `winrt-Microsoft.WindowsAppSDK.InteractiveExperiences` |
| `Microsoft.Graphics.Display`                   | `winrt-Microsoft.WindowsAppSDK.InteractiveExperiences` |
| `Microsoft.Graphics.Imaging`                   | `winrt-Microsoft.WindowsAppSDK.AI`              |
| `Microsoft.Security.Authentication.OAuth`      | `winrt-Microsoft.WindowsAppSDK.Foundation`      |
| `Microsoft.UI`                                 | `winrt-Microsoft.WindowsAppSDK.InteractiveExperiences` |
| `Microsoft.UI.Composition*`                    | `winrt-Microsoft.WindowsAppSDK.InteractiveExperiences` |
| `Microsoft.UI.Content`                         | `winrt-Microsoft.WindowsAppSDK.InteractiveExperiences` |
| `Microsoft.UI.Dispatching`                     | `winrt-Microsoft.WindowsAppSDK.InteractiveExperiences` |
| `Microsoft.UI.Input*`                          | `winrt-Microsoft.WindowsAppSDK.InteractiveExperiences` |
| `Microsoft.UI.System`                          | `winrt-Microsoft.WindowsAppSDK.InteractiveExperiences` |
| `Microsoft.UI.Text`                            | `winrt-Microsoft.WindowsAppSDK.WinUI`           |
| `Microsoft.UI.Windowing`                       | `winrt-Microsoft.WindowsAppSDK.InteractiveExperiences` |
| `Microsoft.UI.Xaml*`                           | `winrt-Microsoft.WindowsAppSDK.WinUI`           |
| `Microsoft.Windows.AI` and `Microsoft.Windows.AI.*`, except the next row | `winrt-Microsoft.WindowsAppSDK.AI` |
| `Microsoft.Windows.AI.MachineLearning`         | `winrt-Microsoft.Windows.AI.MachineLearning`    |
| `Microsoft.Windows.AppLifecycle`               | `winrt-Microsoft.WindowsAppSDK.Foundation`      |
| `Microsoft.Windows.AppNotifications*`          | `winrt-Microsoft.WindowsAppSDK.Foundation`      |
| `Microsoft.Windows.ApplicationModel.*`         | `winrt-Microsoft.WindowsAppSDK.Foundation`      |
| `Microsoft.Windows.BadgeNotifications`         | `winrt-Microsoft.WindowsAppSDK.Foundation`      |
| `Microsoft.Windows.Foundation`                 | `winrt-Microsoft.WindowsAppSDK.Foundation`      |
| `Microsoft.Windows.Globalization`              | `winrt-Microsoft.WindowsAppSDK.Foundation`      |
| `Microsoft.Windows.Management.Deployment`      | `winrt-Microsoft.WindowsAppSDK.Foundation`      |
| `Microsoft.Windows.Media.Capture`              | `winrt-Microsoft.WindowsAppSDK.Foundation`      |
| `Microsoft.Windows.PushNotifications`          | `winrt-Microsoft.WindowsAppSDK.Foundation`      |
| `Microsoft.Windows.Search.AppContentIndex`     | `winrt-Microsoft.WindowsAppSDK.Search`          |
| `Microsoft.Windows.Security.AccessControl`     | `winrt-Microsoft.WindowsAppSDK.Foundation`      |
| `Microsoft.Windows.SemanticSearch`             | `winrt-Microsoft.WindowsAppSDK.AI`              |
| `Microsoft.Windows.Storage*`                   | `winrt-Microsoft.WindowsAppSDK.Foundation`      |
| `Microsoft.Windows.System*`                    | `winrt-Microsoft.WindowsAppSDK.Foundation`      |
| `Microsoft.Windows.Vision`                     | `winrt-Microsoft.WindowsAppSDK.AI`              |
| `Microsoft.Windows.Widgets*`                   | `winrt-Microsoft.WindowsAppSDK.Widgets`         |
| `Microsoft.Windows.Workloads`                  | `winrt-Microsoft.WindowsAppSDK.AI`              |

Each package's README lists the namespaces it carries, and a package that
hands back a type from another component of the same release requires it
outright, so naming the ones you import from is enough.

WinUI 2 is grouped the same way, into the one `Microsoft.UI.Xaml` NuGet
package all six of its namespaces come from:

| v3 distribution                                     | v4 distribution            |
| --------------------------------------------------- | -------------------------- |
| `winui2-Microsoft.UI.Xaml.Automation.Peers`         | `winui2-Microsoft.UI.Xaml` |
| `winui2-Microsoft.UI.Xaml.Controls`                 | `winui2-Microsoft.UI.Xaml` |
| `winui2-Microsoft.UI.Xaml.Controls.AnimatedVisuals` | `winui2-Microsoft.UI.Xaml` |
| `winui2-Microsoft.UI.Xaml.Controls.Primitives`      | `winui2-Microsoft.UI.Xaml` |
| `winui2-Microsoft.UI.Xaml.Media`                    | `winui2-Microsoft.UI.Xaml` |
| `winui2-Microsoft.UI.Xaml.XamlTypeInfo`             | `winui2-Microsoft.UI.Xaml` |

The modules you import from them are unchanged.

The two hand-written modules are renamed but not regrouped:

| v3 distribution                                                       | v4 distribution                                                      |
| --------------------------------------------------------------------- | -------------------------------------------------------------------- |
| `winui3-Microsoft.UI.Interop`                                          | `winrt-Microsoft.UI.Interop`                                          |
| `winui3-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap` | `winrt-Microsoft.Windows.ApplicationModel.DynamicDependency.Bootstrap` |

## If you are coming from v2

The v2 packages were `winrt-` names too, but they are not these packages:
`winrt-Microsoft.UI.Xaml` and the other per-namespace Windows App SDK names are
grouped by component as above, `winrt-Microsoft.Web.WebView2.Core` is named
after its NuGet package now, and every package is versioned by the metadata it
projects rather than by the generator. Install the v4 names from the tables
above rather than un-pinning the v2 ones.
