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

Not necessary on thread exit, but can be used to uninitialize and re-initialize
a thread if needed.

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
