# `winrt.windows` namespace

The `winrt.windows` namespace package contains the automatically generate bindings
for the Windows SDK. Each WinRT namespace is distributed as a separate package
on PyPI. For example, the `winrt-Windows.Foundation` package contains the
bindings for the `Windows.Foundation` namespace and can be imported in Python
as `import winrt.windows.foundation`.

Since most of the code is generated, there currently aren't Python API docs.
You can use <https://learn.microsoft.com/en-us/uwp/api/> instead, keeping in mind
the Python naming conventions and other rules mentioned in
[The WinRT type system](../types.md) section.

!!! tip

    The packages include type hints so you can use those if
    you are unsure of a return type or a parameter type.

!!! todo "Todo"

    We could consider using <https://github.com/MicrosoftDocs/winrt-api> to autogenerate Python docs.

## Interop modules

There are also special `interop` modules that provide extra functionality
to bridge between WinRT and other interfaces.

- [`winrt.windows.graphics.capture.interop` module](windows/graphics/capture/interop.md)
- [`winrt.windows.graphics.directx.direct3d11.interop` module](windows/graphics/directx/direct3d11/interop.md)
- [`winrt.windows.media.interop` module](windows/media/interop.md)
- [`winrt.windows.system.interop` module](windows/system/interop.md)
- [`winrt.windows.ui.composition.interop` module](windows/ui/composition/interop.md)
