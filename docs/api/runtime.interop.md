# `winrt.runtime.interop` module

Various functions for interoperating with Win32 and COM.

## Writing an interop module

A compiled module that wraps a COM or Win32 interop API shares no C ABI with
`winrt-runtime` and includes none of its headers. It exchanges WinRT objects
with the runtime as *interface pointer capsules*, the
[`InterfaceCapsule`](#interfacecapsule) type below. The three functions below
and that type are the whole contract, and the interop packages of PyWinRT use
them the same way:

```python
import winrt.runtime.interop as _runtime
import my_interop._native as _native
from winrt.windows.graphics.capture import GraphicsCaptureItem


def create_for_window(window: int) -> GraphicsCaptureItem:
    return _runtime.wrap_interface(
        _native.create_for_window(window), GraphicsCaptureItem
    )
```

## `InterfaceCapsule`

```python
InterfaceCapsule = NewType("InterfaceCapsule", CapsuleType)
```

A [capsule](https://docs.python.org/3/c-api/capsule.html) named
`"winrt.interface"` that holds one reference to a COM interface, which its
destructor releases. A compiled interop module's stub declares it for every
capsule its functions take or return, so that a type checker tells it apart
from any other capsule.

!!! version-added "Added in version 4.0"

## `as_interface`

```python
as_interface(obj: IInspectable | None, iid: UUID | type[IInspectable], /) -> InterfaceCapsule | None
```

An interface of a WinRT object, as an interface pointer capsule, or `None` for
`None`.

| Parameter | Type | Description |
|---|---|---|
| `obj` | [`Object`](system.md#object) | A WinRT object, or a Python implementation of a projected interface. |
| `iid` | `uuid.UUID` or `type` | The IID of the interface to query for, or a projected interface or runtime class, which stands for the interface an instance of it holds: the interface itself, or the class's default interface. |

Raises `OSError` if the object does not implement the interface and
`TypeError` if `iid` is a type that is neither a projected interface nor a
runtime class. A parameterized interface such as `IVector` has no IID of its
own until it is given type arguments, so it is named by a `uuid.UUID`.

!!! version-added "Added in version 4.0"

## `wrap_interface`

```python
wrap_interface(capsule: InterfaceCapsule | None, type: type[T] | str, /) -> T | None
```

The WinRT object an interface pointer capsule holds, as a projected type, or
`None` for `None`. The capsule keeps its own reference.

| Parameter | Type | Description |
|---|---|---|
| `capsule` | [`InterfaceCapsule`](#interfacecapsule) | An interface pointer capsule. |
| `type` | `type` or `str` | The projected interface or runtime class to wrap the object as, or the qualified name it is bound to, such as `"winrt.windows.foundation.Uri"`. |

Raises `OSError` if the object does not implement the interface that an
instance of `type` holds.

!!! version-added "Added in version 4.0"

## `hresult_error`

```python
hresult_error(hresult: int, error_info: InterfaceCapsule | None = None, /) -> OSError
```

The exception that a WinRT call which failed with `hresult` raises, for an
interop module to raise in turn.

| Parameter | Type | Description |
|---|---|---|
| `hresult` | `int` | The failure `HRESULT`, signed or unsigned. |
| `error_info` | [`InterfaceCapsule`](#interfacecapsule) | An interface pointer capsule holding the *IErrorInfo* that the failed call left on the thread, or `None`. Its message is used when it is about the same `HRESULT`. |

!!! version-added "Added in version 4.0"

## `initialize_with_window`

```python
initialize_with_window(obj: Object, hwnd: int) -> None
```

Provide an owner window to a Windows Runtime (WinRT) object used in a desktop application.

| Parameter | Type | Description |
|---|---|---|
| `obj` | [`Object`](system.md#object) | A WinRT object that implements *IInitializeWithWindow* interface. |
| `hwnd` | `int` | The handle of the window to be used as the owner window. |

!!! version-added "Added in version 3.0"

!!! seealso "See also"

    [Microsoft docs](https://learn.microsoft.com/en-us/windows/win32/api/shobjidl_core/nf-shobjidl_core-iinitializewithwindow-initialize)
