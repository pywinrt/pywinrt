# `winrt.runtime` module

## Activation

Functions related to preparing the WinRT runtime for use.

### `init_apartment`

```python
init_apartment(apartment_type: ApartmentType, /) -> None
```

Initializes the WinRT runtime in the specified apartment type.

Calling this function is only necessary if you need a single threaded, e.g.
for UI thread, or if you need to re-initialize the thread with a different
apartment type.

| Parameter | Type | Description |
|---|---|---|
| `apartment_type` | [`ApartmentType`](#apartmenttype) | The apartment type to initialize the runtime in. |

**Raises:** [`ValueError`][ValueError] if `apartment_type` is not an
`ApartmentType`, and [`OSError`][OSError] if the thread is already in an
apartment of the other type.

!!! version-added "Added in version 3.0"

!!! version-changed "Changed in version 4.0"

    A value that is not an `ApartmentType` raises `ValueError`. Previously it
    raised `OSError`.

[ValueError]: https://docs.python.org/3/builtins/exceptions.html#ValueError
[OSError]: https://docs.python.org/3/builtins/exceptions.html#OSError

### `uninit_apartment`

```python
uninit_apartment() -> None
```

Uninitializes the WinRT runtime.

A thread that called [`init_apartment()`](#init_apartment) calls this before it
exits, and a thread can call it to uninitialize and re-initialize itself. When
it uninitializes the last apartment of the process, COM unloads the DLLs that
implement the classes, so:

- No WinRT object is kept past it. One that is crashes the process when it is
  used or collected.
- No other thread uses WinRT while it runs.
- COM is uninitialized through this function only. It lets go of the
  activation factories that `winrt-runtime` keeps before it uninitializes, and
  a call to `CoUninitialize()` from anywhere else, such as `pythoncom` or
  `ctypes`, does not. Neither does a thread that exits without calling it.

!!! version-added "Added in version 3.0"

### `ApartmentType`

```python
class ApartmentType
```

Enumeration of COM apartment types.

#### `SINGLE_THREADED`

Single-threaded apartment.

#### `MULTI_THREADED`

Multi-threaded apartment.

!!! version-added "Added in version 3.0"
