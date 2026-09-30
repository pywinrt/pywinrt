# WinUI 2 in a XAML Island

`hello_app.py` shows a window with a WinUI 2 control in it. WinUI 2 is built on
the system XAML in `Windows.UI.Xaml`, and a desktop process cannot start that
the way a UWP app does with `Application.start()`: the only way in is a XAML
Island, a `DesktopWindowXamlSource` attached to a window the process creates
itself.

It does not run from `python.exe`. Build it with PyInstaller instead, which
`build.py` does in an environment of its own, then run what it built:

```text
uv run build.py
dist\hello_app\hello_app.exe
```

## What a XAML Island needs

Each of these is required. The first two are about the executable and the
directory it is in, which is why `python.exe` cannot host an island and a
PyInstaller build can.

**A manifest that declares `maxversiontested`.** `WindowsXamlManager` refuses to
start in a process whose executable does not claim to have been tested on
Windows 10.0.18226 or later, with an error that says so. It has to be the
manifest embedded in the executable: activating one from Python is not enough.
`python.exe` has none, and neither does a virtual environment's, which is a
launcher that runs the real `python.exe` as a second process.
`hello_app.manifest` is the one the spec embeds.

**The WinUI 2 `resources.pri` beside the executable.** An unpackaged process
reads its resources from the `resources.pri` in the directory its executable
is in, and `XamlControlsResources` loads WinUI 2's styles from
`ms-appx://Microsoft.UI.Xaml.2.8/`. Without it, that fails with *Cannot locate
resource*. The spec copies the file from the installed WinUI 2 framework
package, so rebuild after that package is updated.

**WinUI 2 in the package graph.** WinUI 2 is installed as a framework package
rather than with Windows, and a process that is not packaged itself can only
activate classes from one after adding it with `TryCreatePackageDependency()`
and `AddPackageDependency()`. Those need Windows 11; the Windows App SDK's
`PackageDependency` class does the same on Windows 10. The framework package
comes with many Store apps, and is also in the `Microsoft.UI.Xaml` NuGet
package as `Microsoft.UI.Xaml.2.8.appx`.

**The `Application` before the `WindowsXamlManager`.** XAML finds the WinUI 2
types through an `Application` that implements `IXamlMetadataProvider`, and
WinUI 2's styles are merged into its resources. In an island it has to exist
before `WindowsXamlManager.initialize_for_current_thread()` is called.

**A window and a message loop of the process's own.** The island is attached
to a window with `DesktopWindowXamlSourceNative.attach_to_window()` and has to
be resized with it. Every message goes to `pretranslate_message()` before it is
dispatched, which is what makes keyboard navigation work inside the island.

**Closing the island before Python exits.** XAML calls back into Python while
it is torn down, so the `DesktopWindowXamlSource` and the `WindowsXamlManager`
are closed before the interpreter shuts down. Both are context managers.
