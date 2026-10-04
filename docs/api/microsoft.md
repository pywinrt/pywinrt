# `winrt.microsoft` namespace

The `winrt.microsoft` namespace package contains the automatically
generated bindings for the Windows App SDK — also known as WinUI 3 — and for
the WebView2 control. Each is published on PyPI as one distribution per NuGet
package it comes from: `winrt-Microsoft.WindowsAppSDK.WinUI` carries every
namespace the App SDK's WinUI component defines, including
`Microsoft.UI.Xaml`, which is imported in Python as
`winrt.microsoft.ui.xaml`.

Since most of the code is generated, there currently aren't Python API docs.
You can use <https://learn.microsoft.com/en-us/windows/windows-app-sdk/api/winrt/>
instead, keeping in mind the Python naming conventions and other rules mentioned
in [The WinRT type system](../types.md) section.

!!! tip

    The packages include type hints so you can use those if
    you are unsure of a return type or a parameter type.

!!! version-changed "Changed in version 3.0"

    The top-level package name has been changed from `winrt` to `winui3`.

!!! version-changed "Changed in version 4.0"

    The top-level package name has been changed back from `winui3` to
    `winrt`, and the Windows App SDK packages are published one per NuGet
    component instead of one per WinRT namespace.

## Windows App SDK

!!! danger

    None of the Windows App SDK packages can be used if the Windows
    App Runtime is not installed! You must manually install this from
    [here](https://learn.microsoft.com/en-us/windows/apps/windows-app-sdk/downloads).
    Also read the [Boostrapping](#boostrapping) section below for how to initialize the runtime.

## Boostrapping

To use the Windows App SDK, you need to start the runtime at the beginning of
your program:

```python
from winrt.microsoft.windows.applicationmodel.dynamicdependency import bootstrap

# initialize Windows App runtime and prompt user if it is not installed
with bootstrap.initialize(options=bootstrap.InitializeOptions.ON_NO_MATCH_SHOW_UI):
    # your main entry point code here
    ...
```

!!! warning

    If you are using a Python runtime installed rom the Microsoft Store,
    this method will fail with `OSError(winerror=-2147009196)` (`ERROR_NOT_SUPPORTED = -2147009196`).
    This happens because the Microsoft Store version of Python is a "packaged" app
    and expects the dependency to be included in the package manifest. This should
    be able to be worked around using other dynamic dependency APIs, but this has
    not been extensively tested yet. See [Microsoft's Docs](https://learn.microsoft.com/en-us/windows/apps/desktop/modernize/framework-packages/use-the-dynamic-dependency-api)
    for more information.

## WebView2

An application that is not packaged gets `Microsoft.Web.WebView2.Core.dll`
from the `winrt-Microsoft.Web.WebView2.Dll` package. On Windows App SDK 2.x, a
WebView2 control that uses its default environment needs that `.dll` loaded
before it is created:

```python
from winrt.microsoft.web.webview2 import dll

dll.load()
```

The control uses its default environment when `source` is set or
`ensure_core_webview2_async()` is called with no environment. Without the
call, it never initializes in the first case and fails with
`ERROR_MOD_NOT_FOUND` in the second. An application that passes its own
`CoreWebView2Environment` to `ensure_core_webview2_async()` before setting
`source` does not need it. This is a WinUI bug,
[microsoft/microsoft-ui-xaml#12158](https://github.com/microsoft/microsoft-ui-xaml/issues/12158),
and the call may not be needed once it is fixed.

The default environment keeps its user data, the browser profile and caches,
in a folder next to `python.exe`, which may not be writable and is not where
an application's data belongs. Setting the `WEBVIEW2_USER_DATA_FOLDER`
environment variable before the control is created moves it:

```python
import os

os.environ["WEBVIEW2_USER_DATA_FOLDER"] = r"C:\path\to\user\data"
```

## Interop modules

There are also special modules that provide extra functionality
to bridge between WinRT and other interfaces.

- [`winrt.microsoft.ui.interop` module](microsoft/ui/interop.md)
- [`winrt.microsoft.windows.applicationmodel.dynamicdependency.bootstrap` module](microsoft/windows/applicationmodel/dynamicdependency/bootstrap.md)
