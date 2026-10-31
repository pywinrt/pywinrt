<!-- warning: Please don't edit this file. It was automatically generated. -->

# winrt-Microsoft.WindowsAppRuntime.Bootstrap

**IMPORTANT**: Windows App SDK packages cannot be used without the Windows App
Runtime. This has to be installed manually by the end user. Read the
[PyWinRT Windows App SDK documentation](https://pywinrt.readthedocs.io/en/latest/api/microsoft/)
for more information.

Finds the Windows App Runtime for an unpackaged app and loads it, as `MddBootstrap.h` does in C++. A packaged app does not need it.

This package provides the `winrt.microsoft.windows.applicationmodel.dynamicdependency.bootstrap` module.

It is built against version 2.6.4 of the `Microsoft.WindowsAppSDK`
NuGet package and loads that release of the Windows App SDK unless told otherwise.
It carries `Microsoft.WindowsAppRuntime.Bootstrap.dll` from version 2.6.4 of the
`Microsoft.WindowsAppSDK.Foundation` NuGet package.

```
pip install winrt-Microsoft.WindowsAppRuntime.Bootstrap
```
