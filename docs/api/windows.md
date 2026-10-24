# `winrt.windows` namespace

The `winrt.windows` namespace package contains the automatically generate bindings
for the Windows SDK. Each WinRT namespace is distributed as a separate package
on PyPI. For example, the `winrt-Windows.Foundation` package contains the
bindings for the `Windows.Foundation` namespace and can be imported in Python
as `import winrt.windows.foundation`.

These modules are generated, so their reference is Microsoft's documentation,
<https://learn.microsoft.com/en-us/uwp/api/>, read with the Python naming
conventions and other rules in [The WinRT type system](../types.md).

!!! tip

    The packages include type hints so you can use those if
    you are unsure of a return type or a parameter type.

## Interop modules

There are also special `interop` modules that provide extra functionality
to bridge between WinRT and other interfaces.

- [`winrt.windows.graphics.capture.interop` module](windows/graphics/capture/interop.md)
- [`winrt.windows.graphics.directx.direct3d11.interop` module](windows/graphics/directx/direct3d11/interop.md)
- [`winrt.windows.media.interop` module](windows/media/interop.md)
- [`winrt.windows.system.interop` module](windows/system/interop.md)
- [`winrt.windows.ui.composition.interop` module](windows/ui/composition/interop.md)
- [`winrt.windows.ui.xaml.hosting.interop` module](windows/ui/xaml/hosting/interop.md)
