# `winrt.system.hresult` module

WinRT methods will raise [`OSError`](https://docs.python.org/3/builtins/exceptions.html#OSError)
exceptions when they fail with
[`OSError.winerror`](https://docs.python.org/3/builtins/exceptions.html#OSError.winerror)
set to an [HRESULT](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-erref/0642cb2f-2075-4469-918c-4441e69c548a)
error code. This module contains some of the most common error codes used with
WinRT types, but it is not exhaustive (there are 1000s of known error codes!).

!!! version-added "Added in version 3.2"

## Windows error codes

Most error codes are defined by Microsoft. Here are a few examples.

!!! note

    Not all available members of this module are documented here. See
    the Python source code for the complete list.

### `S_OK`

```python
S_OK: HResult
```

Operation successful.

### `E_FAIL`

```python
E_FAIL: HResult
```

Unspecified error.

### `E_INVALIDARG`

```python
E_INVALIDARG: HResult
```

One or more arguments are invalid.

### `WIN32_ERROR_CANCELLED`

```python
WIN32_ERROR_CANCELLED: HResult
```

The operation was canceled by the user.

## PyWinRT error codes

The following error codes are specific to PyWinRT.

### `PYWINRT_E_UNRAISABLE_PYTHON_EXCEPTION`

```python
PYWINRT_E_UNRAISABLE_PYTHON_EXCEPTION: HResult
```

This error code is propagated when a Python exception is raised in a
WinRT callback. It is not intended to be handled by the user. Instead
the exception should be handled in the callback itself.

!!! seealso "See also"

    [Exceptions](../types.md#exceptions)
