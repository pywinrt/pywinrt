# Installing

PyWinRT runs on Windows with CPython 3.11 or later. Its packages are on
[PyPI](https://pypi.org/), with wheels for x86, x64 and ARM64.

## Which packages

There is no single package to install. Each part of the WinRT API is a package of
its own, so an application installs the ones it uses:

```console
pip install winrt-Windows.Devices.Bluetooth winrt-Windows.Devices.Enumeration
```

```python
from winrt.windows.devices.bluetooth import BluetoothLEDevice
from winrt.windows.devices.enumeration import DeviceInformation
```

A package brings `winrt-runtime` with it, which is what runs it. It also brings
whichever of `winrt-Windows.Foundation`, `winrt-Windows.Foundation.Collections`
and `winrt-Windows.Storage.Streams` it uses, since nearly every API hands back
their types, and the packages its classes derive from, as
`winrt-Windows.UI.Xaml.Controls` brings `winrt-Windows.UI.Xaml`. A Windows App
SDK component brings every other component whose types it hands back. Any other
package whose types it names is left to the application, which is why the
example installs `winrt-Windows.Devices.Enumeration` beside
`winrt-Windows.Devices.Bluetooth`.

How the packages are cut depends on where the API comes from:

| API | Packages | Example |
|---|---|---|
| Windows SDK | one per WinRT namespace | `winrt-Windows.Devices.Bluetooth` gives `winrt.windows.devices.bluetooth` |
| Windows App SDK | one per NuGet component, each with several namespaces | `winrt-Microsoft.WindowsAppSDK.WinUI` gives `winrt.microsoft.ui.xaml` and the other WinUI 3 modules |
| WinUI 2 | one package | `winui2-Microsoft.UI.Xaml` gives the `winui2.microsoft.ui.*` modules |
| WebView2 | one package | `winrt-Microsoft.Web.WebView2` gives `winrt.microsoft.web.webview2.core` |

The page of a package on PyPI lists the modules it provides.

Some APIs that bridge WinRT and Win32 are packages of their own too, named for
the namespace they extend, such as `winrt-Windows.Graphics.Capture.Interop`. So
are the Windows App SDK's bootstrapper,
`winrt-Microsoft.WindowsAppRuntime.Bootstrap`, and the
WebView2 runtime component, `winrt-Microsoft.Web.WebView2.Dll`. Their modules
are in the [API reference](api/index.md).

## A package that is not installed

A type that an installed package names, as the result of a method for example,
may come from a package that is not installed. The call that needs it then
raises [`ModuleNotFoundError`][ModuleNotFoundError] with the module's name, and
the package to install is named after the namespace:

```text
ModuleNotFoundError: No module named 'winrt.windows.storage'
```

```console
pip install winrt-Windows.Storage
```

Each package also has an `[all]` extra, which installs every package whose types
it names, such as `winrt-Windows.Media.Ocr[all]`. Those name others in turn, so
the extra often installs eighty packages or more, most of which an application
never uses. Installing the packages an application uses is the better choice.

[ModuleNotFoundError]: https://docs.python.org/3/library/exceptions.html#ModuleNotFoundError
