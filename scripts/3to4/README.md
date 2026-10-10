# PyWinRT v3 to v4 migration guide

PyWinRT v4 renames the packages and the Python modules of the Windows App SDK
and WebView2 projections, and publishes the Windows App SDK and WinUI 2 in
fewer, larger packages. This describes what you need to change in your own
code and in your dependency lists. Everything else that changed in v4 is in
[CHANGELOG.md](../../CHANGELOG.md).

## Upgrade every package, not only the runtime

A v3 projection package does not load with the v4 runtime. If
`pip install -U winrt-runtime` has upgraded the runtime and left the v3
packages behind (pip only warns about their `winrt-runtime~=3.2.1.0`
requirement), importing one of them fails with:

```text
AttributeError: module 'winrt._winrt' has no attribute '_C_API'
```

Upgrade the projection packages as well, under the names below, until no v3
package is left.

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

### Version constraints

The packages whose names did not change need their constraints looked at too.
A v4 projection package is versioned with an epoch, `4!10.0.28000.2705`, which
sorts above every v3 version, so a v3 constraint that caps the version or pins
it - `winrt-Windows.Foundation>=3.2,<3.3`, `~=3.2.1` or `==3.2.1` - refuses
every v4 release. A bare floor such as `>=3.2` still resolves.

`distributions.csv` in this directory lists every distribution v3.2.1
published, with the v4 distribution it became and the first v4 version of it,
and `inspect_source.py` (below) finds requirements on them in requirements
files, `pyproject.toml`, `setup.cfg` and the strings of a `setup.py`.
`winrt-sdk`, `winrt-WindowsAppSDK` and `winrt-Microsoft.UI.Xaml` shipped C++
headers in v3 and have no v4 distribution, since nothing compiles against
PyWinRT's headers any more; neither has `winrt-Windows.AI.ModelContextProtocol`,
whose namespace the Windows SDK dropped.

## Renamed methods

v3 named every overload of a method after its
`[Windows.Foundation.Metadata.Overload]` attribute, which renamed about 10% of
methods. v4 names overloads by the number of arguments again, as v2 did, and
uses the attribute only where two overloads take the same number of
arguments. So `find_all_async_aqs_filter(...)` is `find_all_async(...)` again,
and so are its other overloads.

The Windows SDK packages (`winrt-Windows.*`) keep each v3 name as an alias
that raises a `DeprecationWarning`, so code written for v3 keeps working until
the aliases are removed in a future release. The Windows App SDK, WebView2 and
WinUI 2 packages have no aliases, since code written for them has to be edited
for the new package names anyway: there a v3 name raises `AttributeError`.

A v3 name cannot be kept as an alias when v4 uses it for another overload of
the same method. The only one is in the Windows SDK:
`TileUpdateManagerForUser.create_tile_updater_for_application()` took no
arguments in v3 and is `create_tile_updater_for_application_for_user()` in
v4, and the v4 `create_tile_updater_for_application()` is v3's
`create_tile_updater_for_application_with_id()`, which takes an application
id. Code that calls the old name gets a `TypeError` for the missing argument
rather than a `DeprecationWarning`.

`methods.csv` in this directory maps each v3 name to its v4 name, and
`values.csv` lists the properties that hand back an `HResult`. Both are
written by `generate.py`, as is `distributions.csv`.

There is also an `inspect_source.py` script that looks for renamed methods in
your Python files, along with the other changes that it can find by looking at
the source: the `winui3` and `webview2` imports, the deprecated
`HResult.value` and `EventRegistrationToken.value`, the deprecated `_from()`,
format strings passed to `winrt.system.Array`, the uses of `Matrix3x2` and
`Matrix4x4`, whose product is `@`, and requirements on v3 distributions that
were renamed, removed or constrained to v3.

### Usage

Pass the source files as arguments to the script. For example:

    py .\scripts\3to4\inspect_source.py ..\bleak\bleak\backends\winrt\client.py

The script will print out the potential matches and suggest the new names:

    ..\bleak\bleak\backends\winrt\client.py:693:41
    possible match: winrt.windows.devices.bluetooth.BluetoothLEDevice.get_gatt_services_with_cache_mode_async
    rename to: get_gatt_services_async

The first line is `<file name>:<line number>:<column number>`. The second line
gives the fully qualified name of the method or attribute that was found in the
source file. The third line gives the new name that should be used.

Some editors, like VS Code, will automatically turn the first line into a link
that can jump to the location in the source file.

A requirements file, a `pyproject.toml` or a `setup.py` is passed the same way,
and a requirement is reported with what it should become:

    requirements.txt:2:1
    possible match: winui3-Microsoft.UI.Xaml.Controls>=3.2,<3.3
    rename to: winrt-Microsoft.WindowsAppSDK.WinUI>=4!2.5.1

The script only reports. It changes no file, so the edits are yours to make,
and since several v3 distributions became one v4 distribution, the requirements
it suggests may repeat each other.

### Caveats

* The script does not search in comments, docstrings or string annotations for
  code changes. It searches the string literals of a Python file for
  requirements, and everything but comments in any other file.
* A floor at the first v4 version is the least the suggested requirement can
  say. If the code uses something a later upstream release added, raise it.
* The script doesn't do any static analysis to infer types, so it may produce
  false positives. It matches a method by its name alone, and `.value` only on
  a property that hands back an `HResult` or on a name that has `token` in it.
  It reports every call of a method named `_from`, whatever it is called on.
  It reports every use of `Matrix3x2` and `Matrix4x4`, under whatever name
  they are imported as, since it cannot tell which `*` has two matrices on
  either side of it; a matrix that only comes out of a property or a method,
  with its type never named, is not seen.
* Where v3 had several names for the overloads of one method, check the type
  hints to make sure the arguments you pass select the overload you want.
* An array of enums was spelled `Array("i", ...)` or `Array("I", ...)` in v3;
  spell it with the enum type rather than with the integer type the script
  suggests.

## `_from()` is `as_()`

`Type._from(obj)` is deprecated and raises a `DeprecationWarning`. Write
`obj.as_(Type)` instead, which has done the same thing since v3. The script
reports each call with the `as_()` call that replaces it.

## The product of two matrices is `@`

`Matrix3x2 * Matrix3x2` and `Matrix4x4 * Matrix4x4` were the matrix product,
as in C# and C++. In v4 the product is `@`, as it is for NumPy arrays, and `*`
between two matrices raises `TypeError` rather than quietly becoming an
elementwise product; `*=` follows, so `m *= other` is `m @= other`. A matrix
times a number is still `*`, and the number can now be on either side.

A vector on the left of `@` is transformed by the matrix or the rotation on
the right, in the row vector order that `transform()` uses: `v @ m` is
`v.transform(m)`. The quaternion keeps `*` for its product and takes `@` as
the same thing.

The buffer of a `winrt.system.Array` of `Vector2`, `Vector3`, `Vector4`,
`Quaternion`, `Plane`, `Matrix3x2` or `Matrix4x4` is a block of floats with
one more dimension than the element, such as `(n, 3)` for `Vector3` and
`(n, 4, 4)` for `Matrix4x4`, rather than one item per element with a field
per float. `numpy.asarray(array)` is then that block, ready to use; code that
read the fields of a structured NumPy array made from one reads columns
instead.

## Removed

* `winrt.system.Array` is a `collections.abc.Sequence` whose items can be
  assigned, not a `collections.abc.MutableSequence`, since its length is
  fixed. Of the methods that v3 gave it for being one, `reverse()` is the only
  one that worked, and it is gone; assign the items in a loop instead. The
  others - `insert()`, `append()`, `extend()`, `pop()`, `remove()`, `clear()`,
  `+=` and `del a[i]` - raised `TypeError` in v3 and are gone as well.
* `DesktopWindowXamlSourceNative` from
  `winrt.windows.ui.xaml.hosting.interop` is no longer a
  `winrt.system.Object`, so it cannot be passed where a WinRT object is
  expected. It is still what `source.as_(DesktopWindowXamlSourceNative)`
  returns.
* `winrt-Windows.AI.ModelContextProtocol` is gone, because the Windows SDK no
  longer has the namespace.
* Python 3.9 and 3.10 are no longer supported.

## If you are coming from v2

The v2 packages were `winrt-` names too, but they are not these packages:
`winrt-Microsoft.UI.Xaml` and the other per-namespace Windows App SDK names are
grouped by component as above, `winrt-Microsoft.Web.WebView2.Core` is named
after its NuGet package now, and every package is versioned by the metadata it
projects rather than by the generator. Install the v4 names from the tables
above rather than un-pinning the v2 ones.
