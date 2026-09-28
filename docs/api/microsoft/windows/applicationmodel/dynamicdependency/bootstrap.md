# `winrt.microsoft.windows.applicationmodel.dynamicdependency.bootstrap` module

APIs for bootstrapping the Windows App runtime.

!!! version-changed "Changed in version 3.0"

    The top-level package name has been changed from `winrt` to `winui3`.

!!! version-changed "Changed in version 4.0"

    The top-level package name has been changed back from `winui3` to
    `winrt`.

## `initialize`

```python
initialize(version: str = RELEASE_VERSION, min_version: str = RUNTIME_VERSION, options: InitializeOptions = InitializeOptions.NONE) -> Shutdown
```

Initialize the Windows App SDK runtime.

| Parameter | Type | Description |
|---|---|---|
| `version` | `str` | The Windows App SDK *product* release version to load, e.g. “1.5” or “1.5-preview”. |
| `min_version` | `str` | The minimum version of the Windows App SDK *runtime* to load, e.g. “5001.70.1338.0”. |
| `options` | [`InitializeOptions`](#initializeoptions) | Initialization option flags. |

**Returns:** A context manager that will shutdown the Windows App SDK runtime when exited.

**Raises:** `OSError` – if the initialization fails.

!!! version-added "Added in version 2.1"

Example:

```python
with initialize(options=InitializeOptions.ON_NO_MATCH_SHOW_UI):
    # main code here
    ...
```

## `RELEASE_VERSION`

```python
RELEASE_VERSION: str
```

The package release version of the Windows App SDK used at compile time.

!!! version-added "Added in version 2.1"

## `RUNTIME_VERSION`

```python
RUNTIME_VERSION: str
```

The runtime version of the Windows App SDK used at compile time.

!!! version-added "Added in version 2.1"

## `InitializeOptions`

```python
class InitializeOptions(enum.IntFlag)
```

**Bases:** [`enum.IntFlag`](https://docs.python.org/3/library/enum.html#enum.IntFlag)

Initialization option flags for [`initialize()`](#initialize).

### `NONE`

Default behavior

### `ON_ERROR_DEBUG_BREAK`

If not successful call DebugBreak()

### `ON_ERROR_DEBUG_BREAK_IF_DEBUGGER_ATTACHED`

If not successful call DebugBreak() if a debugger is attached to the process

### `ON_ERROR_FAIL_FAST`

If not successful perform a fail-fast

### `ON_NO_MATCH_SHOW_UI`

If a compatible Windows App Runtime framework package is not found show UI

### `ON_PACKAGE_IDENTITY_NOOP`

Do nothing (do not error) if the process has package identity

!!! version-added "Added in version 2.1"
