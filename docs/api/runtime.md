# `winrt.runtime` module

## Activation

Functions related to preparing the WinRT runtime for use.

### `init_apartment`

```python
init_apartment(apartment_type: ApartmentType) -> None
```

Initializes the WinRT runtime in the specified apartment type.

Calling this function is only necessary if you need a single threaded, e.g.
for UI thread, or if you need to re-initalize the thread with a different
apartment type.

| Parameter | Type | Description |
|---|---|---|
| `apartment_type` | [`ApartmentType`](#apartmenttype) | The apartment type to initialize the runtime in. |

!!! version-added "Added in version 3.0"

### `uninit_apartment`

```python
uninit_apartment() -> None
```

Uninitializes the WinRT runtime.

Not necessary on thread exit, but can be used uninitalize and re-initalize
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
