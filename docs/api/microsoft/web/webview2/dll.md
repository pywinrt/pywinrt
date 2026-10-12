# `winrt.microsoft.web.webview2.dll` module

`Microsoft.Web.WebView2.Core.dll`, the component the WebView2 projection needs,
redistributed in the `winrt-Microsoft.Web.WebView2.Dll` package, one wheel per
architecture. It comes from the same release of the `Microsoft.Web.WebView2`
NuGet package as the WebView2 projection packages.

Importing the module puts the directory the `.dll` is in on the DLL search path,
which is how WinRT finds it. `winrt-Microsoft.Web.WebView2` depends on this
package and imports it, so an application only imports it itself to call
[`load()`](#load). [WebView2](../../../microsoft.md#webview2) says when that is
needed.

!!! version-added "Added in version 4.0"

## `DLL_NAME`

```python
DLL_NAME: str
```

The file name of the `.dll`, `"Microsoft.Web.WebView2.Core.dll"`.

## `get_dll_path`

```python
get_dll_path() -> str
```

**Returns:** The full path of the `.dll` in the package.

## `load`

```python
load() -> None
```

Loads the `.dll` into the process and keeps it loaded. Calling it again does
nothing.

Being on the DLL search path is enough for WinRT, but code that loads the
`.dll` by name with `LoadLibraryW` does not search that path, and finds the
`.dll` only once it is loaded. The WinUI WebView2 control of Windows App SDK
2.x is such code.

**Raises:** [`OSError`][OSError] if the `.dll` cannot be loaded.

[OSError]: https://docs.python.org/3/builtins/exceptions.html#OSError
