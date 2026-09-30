# The WinRT type system

PyWinRT is a "projection" that uses the [Windows Runtime (WinRT) type system][winrt-type-system]
to automatically generate Python bindings for the Windows SDK. This page describes
how the WinRT types look and feel in this Python projection.

[winrt-type-system]: https://learn.microsoft.com/en-us/uwp/winrt-cref/winrt-type-system

## Naming conventions

In the WinRT documentation, most names use `PascalCasing` in the style of .NET.
In the Python projection, names are adapted to fit [PEP8 naming conventions][pep8-naming].

- Type names remain as `CapitalizedWords`.
- Constants, such as enum members are converted to `UPPER_CASE_WITH_UNDERSCORES`.
- Namespaces are converted to `lowercasewithoutunderscores`.
- All other identifiers are converted to `lower_case_with_underscores`
  (fields, properties, methods, events, etc.).
- If an identifier is a Python keyword, an underscore is appended to the name.

[pep8-naming]: https://peps.python.org/pep-0008/#naming-conventions

## Distribution packages

Each WinRT namespace is projected as a separate Python package. The packages are
named like [winrt-Windows.Foundation](https://pypi.org/project/winrt-Windows.Foundation/)
where `winrt` is the root namespace for a specific SDK or DLL and
`Windows.Foundation` is a WinRT namespace within that SDK or DLL.

There are also some PyWinRT-specific sub-packages distributed in the
[winrt-runtime](https://pypi.org/project/winrt-runtime/) package. As well as
some extra interop packages that bridge between WinRT and Win32 types.

A namespace package holds no compiled code. It is a table describing the
namespace - its types, their members and the ABI call each member makes - with
the type hints beside it, so one `py3-none-any` wheel serves every supported
version of Python and every architecture. `winrt-runtime` reads the table and
makes the calls, so it and the interop packages are the only ones that are
compiled for a particular interpreter.

!!! seealso "See also"

    [API reference](api/index.md)

## Namespaces

Each Windows SDK namespace is projected as a Python package and follows the usual
Python import conventions:

```python
# import members of a namespace
from winrt.windows.foundation import Uri
# or import the namespace module itself
import winrt.windows.foundation as wf
```

As with the distribution package names, the WinRT namespace names have a root
namespace that matches the part of the package name before the `-` followed
by the namespace. The modules names are `lowercasewithoutunderscores`, so
namespaces with multiple words in one segment, like `Windows.AI.MachineLearning`
are projected as `winrt.windows.ai.machinelearning`.

## Fundamental types

The WinRT type system defines the following fundamental types which are mapped
to the corresponding Python builtin types.

| WinRT type | Python type | Python format string | Description |
|---|---|---|---|
| `Boolean` | [`bool`][bool] | `"?"` | an 8-bit Boolean value |
| `Int8` | [`int`][int] | `"b"` | an 8-bit signed integer |
| `Int16` | [`int`][int] | `"h"` | a 16-bit signed integer |
| `Int32` | [`int`][int] | `"i"` | a 32-bit signed integer |
| `Int64` | [`int`][int] | `"q"` | a 64-bit signed integer |
| `UInt8` | [`int`][int] | `"B"` | an 8-bit unsigned integer |
| `UInt16` | [`int`][int] | `"H"` | a 16-bit unsigned integer |
| `UInt32` | [`int`][int] | `"I"` | a 32-bit unsigned integer |
| `UInt64` | [`int`][int] | `"Q"` | a 64-bit unsigned integer |
| `Single` | [`float`][float] | `"f"` | a 32-bit IEEE 754 floating point number |
| `Double` | [`float`][float] | `"d"` | a 64-bit IEEE 754 floating point number |
| `Char16` | [`str`][str] [^s] | `"u"` [^u] | a 16-bit non-numeric value representing a UTF-16 code unit |
| `String` | [`str`][str] | `"P"` | an immutable sequence of Char16 used to represent text |
| `Guid` | [`uuid.UUID`][uuid.UUID] | `"T{I2H8B}"` [^g] | a 128-bit standard globally unique identifier |

The Python format strings use the syntax [defined in the struct module][struct-format]
and the [PEP 3118 additions][pep3118-additions] and are used as the format for the
buffer protocol for [arrays](#arrays) of these types. These format strings can be
read at runtime by using [`memoryview.format`][memoryview.format].

The eleven types that have no Python type of their own are named in Python by
the aliases in [`winrt.system`](api/system.md), such as `winrt.system.Int32`. Each
alias is annotated with what its type is called in each of the three languages
involved - [`winrt.system.BufferFormat`](api/system.md#bufferformat),
[`winrt.system.StructFormat`](api/system.md#structformat)
and [`winrt.system.WinrtSignature`](api/system.md#winrtsignature) - so a program
that needs one of them finds it with an [`isinstance()`][isinstance] check over
[`typing.get_args()`][typing.get_args].

[bool]: https://docs.python.org/3/builtins/functions.html#bool
[int]: https://docs.python.org/3/builtins/functions.html#int
[float]: https://docs.python.org/3/builtins/functions.html#float
[str]: https://docs.python.org/3/builtins/stdtypes.html#str
[uuid.UUID]: https://docs.python.org/3/library/uuid.html#uuid.UUID
[memoryview.format]: https://docs.python.org/3/builtins/stdtypes.html#memoryview.format
[isinstance]: https://docs.python.org/3/builtins/functions.html#isinstance
[typing.get_args]: https://docs.python.org/3/library/typing.html#typing.get_args
[struct-format]: https://docs.python.org/3/library/struct.html#format-characters
[pep3118-additions]: https://peps.python.org/pep-3118/#additions-to-the-struct-string-syntax

[^s]: Strings that are converted to `Char16` can only contain one character,
    similar to how [`ord()`](https://docs.python.org/3/builtins/functions.html#ord) works.
[^u]: `"u"` is deprecated in the [`array`](https://docs.python.org/3/library/array.html#module-array)
    module and is not compatible with the [`struct`][struct] module. Use `"H"` instead
    if needed, which is what [`winrt.system.StructFormat`](api/system.md#structformat)
    carries for `Char16`.
[^g]: Use `"I2H8B"` with the [`struct`][struct] module since it does not support
    the PEP 3118 `T{}` syntax.

[struct]: https://docs.python.org/3/library/struct.html#module-struct

## Enums

WinRT enums are projected using the Python standard library
[`enum`](https://docs.python.org/3/library/enum.html#module-enum) module. The
WinRT Type system has a `[Flags]` attribute that indicates if an enum is
treated as bit flags or not. Enum types without this attribute are projected as
an [`enum.IntEnum`](https://docs.python.org/3/library/enum.html#enum.IntEnum) type
or if the `[Flags]` attribute is present, the type is
projected as an [`enum.IntFlag`](https://docs.python.org/3/library/enum.html#enum.IntFlag) type.

An array of enums is named with the enum type itself, as in
`Array(SomeEnum, 3)`. A buffer passed where such an array is expected uses
the `"i"` format string for a regular enum and `"I"` for a flags enum.

## Structs

Structs are simple data types that are passed by value in WinRT. In the Python
projection, each struct is wrapped in a Python object that is similar to a
[`typing.NamedTuple`](https://docs.python.org/3/library/typing.html#typing.NamedTuple)
or frozen [`dataclasses`](https://docs.python.org/3/library/dataclasses.html#module-dataclasses).
The names of fields are converted to use the `lower_case_with_underscores`
[naming convention](#naming-conventions).

Projected structs are [`@typing.final`](https://docs.python.org/3/library/typing.html#typing.final)
classes, so they cannot be subclassed. Projected structs are immutable. To modify a
struct, use [`copy.replace()`][copy.replace] which creates a modified copy.

As a convenience, plain [`tuple`](https://docs.python.org/3/builtins/stdtypes.html#tuple)
objects can be used in place of projected
structs when calling methods that expect a projected struct or when setting
properties that expect a projected struct.

Each projected struct with more than one field also has a `unpack()` method
the converts the projected struct to a tuple.

Example:

```python
# window.position is a winrt.windows.graphics.PointInt32

# fails with AttributeError because Point is immutable
window.position.x = 200

# modifying
new = copy.replace(window.position, x=200)

# passing a tuple
window.move_and_resize((100, 100, 800, 600))

# unpacking
x, y = window.position.unpack()
```

!!! version-changed "Changed in version 2.2"

    Changed `__repr__` implementation to give a representation that can be
    passed to `eval()`.

!!! version-changed "Changed in version 3.0"

    * Structs are now immutable. In previous versions, attributes could be set.
    * Added `__replace__` method to allow for use with [`copy.replace()`][copy.replace].
    * Added `unpack()` method to convert to a tuple. Added support for using
      plain tuples in place of projected structs as method arguments.

[copy.replace]: https://docs.python.org/3/library/copy.html#copy.replace

## Objects

!!! todo "Todo"

    explain objects aka runtime classes

### Methods

!!! todo "Todo"

    explain method naming and overload rules

    <https://github.com/pywinrt/pywinrt/blob/main/projection/readme.md#method-overloading>

### Properties

Properties are projected as Python [descriptor](https://docs.python.org/3/glossary.html#term-descriptor)-like
attributes. Properties with WinRT getter support getting and properties with a WinRT
setter allow setting. Deleting properties is never allowed.

Names of properties are converted to use the `lower_case_with_underscores`
[naming convention](#naming-conventions).

Example:

```python
from winrt.windows.foundation import Uri

uri = Uri("https://example.com")
print(uri.scheme_name)
```

Static properties are implemented as class attributes via a metaclass, so are
accessed by using the type object rather than an instance object:

```python
from winrt.windows.foundation import GuidHelper

empty_uuid = GuidHelper.empty
```

!!! version-changed "Changed in version v1.0.0b8"

    Previous beta releases implemented static properties as `get_name()` and
    `put_name()` static methods instead of class attributes.

### Events

An event is projected as a pair of methods, `add_<event>()` and
`remove_<event>()`, with the event name converted to the
`lower_case_with_underscores` [naming convention](#naming-conventions).
`add_<event>()` takes a handler and returns an `EventRegistrationToken`;
passing that token to `remove_<event>()` removes the handler again. An event
can have any number of handlers, and each `add_<event>()` call registers one
more. The token is a subclass of [`int`][int], so it can be compared, hashed
and used as a dictionary key.

The handler is a callable that takes two positional arguments, the object that
raised the event and an event arguments object, and returns nothing. The types
of both are in the stubs as the parameters of the event's delegate type.

```python
from winrt.windows.devices.bluetooth.advertisement import (
    BluetoothLEAdvertisementReceivedEventArgs,
    BluetoothLEAdvertisementWatcher,
)


def on_received(
    sender: BluetoothLEAdvertisementWatcher,
    args: BluetoothLEAdvertisementReceivedEventArgs,
) -> None:
    print(args.bluetooth_address)


watcher = BluetoothLEAdvertisementWatcher()
token = watcher.add_received(on_received)
watcher.start()
...
watcher.stop()
watcher.remove_received(token)
```

Static events are added and removed on the type object rather than on an
instance, in the same way as [static properties](#properties):

```python
from winrt.windows.applicationmodel.core import CoreApplication

token = CoreApplication.add_unhandled_error_detected(on_error)
```

#### Which thread calls the handler

WinRT calls the handler on whatever thread raises the event, and the handler
runs as ordinary Python code on that thread. For most objects that is a WinRT
thread pool thread, not one of the program's own. In a GUI app, an object that lives on the
UI thread raises its events there, so a XAML event handler runs on the UI
thread.

This means a handler must not touch anything that belongs to a particular
thread. With `asyncio`, hand the work to the event loop with
[`loop.call_soon_threadsafe()`][call_soon_threadsafe] rather than doing it in
the handler, and use an [`asyncio.Queue`][asyncio.Queue] or an
[`asyncio.Future`][asyncio.Future] to deliver the event arguments to a
coroutine:

```python
import asyncio

from winrt.windows.devices.bluetooth.advertisement import BluetoothLEAdvertisementWatcher


async def scan() -> None:
    loop = asyncio.get_running_loop()
    received = asyncio.Queue()

    watcher = BluetoothLEAdvertisementWatcher()
    token = watcher.add_received(
        lambda sender, args: loop.call_soon_threadsafe(received.put_nowait, args)
    )

    try:
        watcher.start()

        while True:
            args = await received.get()
            print(args.bluetooth_address)
    finally:
        watcher.stop()
        watcher.remove_received(token)
```

The sender carries on after the handler returns, so a property of the sender
read later from a task on the event loop describes the object as it is then,
not as it was when the event was raised. Where that matters, read the property
in the handler and hand the value to the event loop instead of the object.

Some events are designed for the handler to finish its work later: their
event arguments have a `get_deferral()` method. The object that raised the
event waits until the deferral is completed, so a handler can take the
deferral, schedule a coroutine on the event loop, and have that coroutine call
`complete()` on the deferral when it is done. Everything else the handler
needs must still be read before the handler returns.

```python
def on_suspending(sender, args):
    deferral = args.suspending_operation.get_deferral()

    async def save() -> None:
        try:
            await save_state()
        finally:
            deferral.complete()

    asyncio.run_coroutine_threadsafe(save(), loop)
```

[call_soon_threadsafe]: https://docs.python.org/3/library/asyncio-eventloop.html#asyncio.loop.call_soon_threadsafe
[asyncio.Queue]: https://docs.python.org/3/library/asyncio-queue.html#asyncio.Queue
[asyncio.Future]: https://docs.python.org/3/library/asyncio-future.html#asyncio.Future

#### Removing handlers

The WinRT object keeps a reference to the handler until it is removed. A
handler that is a closure or a bound method keeps its own object alive too, so
an object that subscribes to an event on a WinRT object it owns will not be
garbage collected until the handler is removed, however unreachable the pair
become. Keep the token and remove the handler when the object is done with the
event. [`contextlib.ExitStack`][ExitStack] pairs an `add_<event>()` with its
`remove_<event>()` at the point of subscribing:

```python
import contextlib

with contextlib.ExitStack() as stack:
    token = frame_pool.add_frame_arrived(on_frame)
    stack.callback(frame_pool.remove_frame_arrived, token)
    ...
```

`remove_<event>()` stops the handler from being called again, but it does not
wait for a call that is already in progress on another thread. A handler can
still be running when `remove_<event>()` returns.

Removing a handler is not required before the interpreter exits. An event
that is raised while Python is shutting down, or after it has shut down, is
dropped rather than delivered.

[ExitStack]: https://docs.python.org/3/library/contextlib.html#contextlib.ExitStack

#### Exceptions in handlers

A handler has no Python caller to raise to, so an exception that escapes one
goes to [`sys.unraisablehook()`][unraisablehook] and the WinRT code that raised
the event gets the error code
[`PYWINRT_E_UNRAISABLE_PYTHON_EXCEPTION`](api/system.hresult.md#pywinrt_e_unraisable_python_exception).
Handle exceptions inside the handler. See [Exceptions](#exceptions).

[unraisablehook]: https://docs.python.org/3/library/sys.html#sys.unraisablehook
[int]: https://docs.python.org/3/library/functions.html#int

!!! version-changed "Changed in version 4.0"

    `EventRegistrationToken` is a subclass of `int`. Previously it was a
    struct with a single `value` field.

## Interfaces

!!! todo "Todo"

    explain interfaces

## Delegates

!!! todo "Todo"

    explain delegates

!!! todo "Todo"

    explain exceptions in callbacks

## Arrays

!!! todo "Todo"

    document array types

## Exceptions

PyWinRT uses the CppWinRT projection under the hood. Any C++ exception that is
not handled gets propagated to Python as an [`OSError`][OSError] exception with
[`OSError.winerror`][OSError.winerror] set to an [HRESULT][hresult] error code.

!!! seealso "See also"

    [`winrt.system.hresult`](api/system.hresult.md) for some common error codes.

On the other hand, Python exceptions cannot be propagated to C++ code. WinRT
requires that errors are serializable, but Python exceptions are not. So if a
Python callback from C++ code (i.e. an event handler or other delegate or a
method of a Python subclass of a WinRT interface) raises an exception that isn't
handled before the method returns, it will trigger the
[`sys.unraisablehook()`](https://docs.python.org/3/library/sys.html#sys.unraisablehook)
handler in Python and cause the C++ code to receive a an HRESULT error code
of [`PYWINRT_E_UNRAISABLE_PYTHON_EXCEPTION`](api/system.hresult.md#pywinrt_e_unraisable_python_exception).
This can cause undefined behavior in the C++ code in some cases, so it should
be avoided.

[OSError]: https://docs.python.org/3/builtins/exceptions.html#OSError
[OSError.winerror]: https://docs.python.org/3/builtins/exceptions.html#OSError.winerror
[hresult]: https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-erref/0642cb2f-2075-4469-918c-4441e69c548a

## Specialized types

Some types have special handling and don't strictly follow the patterns described
above.

### Awaitables

WinRT has many `_async` methods that that perform background operations and
return a type that derives from `Windows.Foundation.IAsyncInfo` to be able to
wait for the result. There are four fundamental types that derive from this:

- [IAsyncAction](https://learn.microsoft.com/en-us/uwp/api/windows.foundation.iasyncaction)
  \- represents an operation that does not return a value
- [IAsyncOperation&lt;TResult&gt;](https://learn.microsoft.com/en-us/uwp/api/windows.foundation.iasyncoperation-1)
  \- represents an operation that returns a value of type `TResult`
- [IAsyncOperationWithProgress&lt;TResult, TProgress&gt;](https://learn.microsoft.com/en-us/uwp/api/windows.foundation.iasyncactionwithprogress-1)
  \- represents an operation that returns a value of type `TResult` and reports
  progress of type `TProgress`
- [IAsyncActionWithProgress&lt;U&gt;](https://learn.microsoft.com/en-us/uwp/api/windows.foundation.iasyncoperationwithprogress-2)
  \- represents an operation that does not return a value and reports progress
  of type `TProgress`

#### Synchronous usage

PyWinRT exposes the CppWinRT extension methods for calling these methods
synchronously (i.e. when not using `asyncio`).

##### `get`

```python
IAsyncAction.get(self) -> None
IAsyncOperation.get(self) -> TResult
IAsyncActionWithProgress.get(self) -> None
IAsyncOperationWithProgress.get(self) -> TResult
```

These methods block until the operation is complete.

**Returns:** The result of the operation.

**Raises:**

- [`OSError`][OSError] if the operation failed or was canceled.
- [`RuntimeError`][RuntimeError] if called from a single-threaded apartment (i.e.
  a GUI thread).

!!! warning

    This method can't be interrupted by ++ctrl+c++ which means
    that a [`KeyboardInterrupt`][KeyboardInterrupt] will not be raised until the operation
    is complete.

!!! version-added "Added in version 3.2"

##### `wait`

```python
IAsyncAction.wait(self, timeout: float) -> AsyncStatus
IAsyncOperation.wait(self, timeout: float) -> AsyncStatus
IAsyncActionWithProgress.wait(self, timeout: float) -> AsyncStatus
IAsyncOperationWithProgress.wait(self, timeout: float) -> AsyncStatus
```

These methods block until the operation is complete or the timeout is reached,
whichever comes first.

| Parameter | Type | Description |
|---|---|---|
| `timeout` | `float` | The timeout in seconds. A timeout that is not a positive number of seconds returns the status as it stands, without waiting. |

**Returns:** The status of the operation. In case of a timeout, the status will be
`AsyncStatus.STARTED`.

**Raises:**

- [`RuntimeError`][RuntimeError] if called from a single-threaded apartment (i.e.
  a GUI thread).

!!! warning

    This method can't be interrupted by ++ctrl+c++, which means
    a [`KeyboardInterrupt`][KeyboardInterrupt] will not be raised until the operation is
    complete or the timeout is reached.

!!! version-added "Added in version 3.2"

[RuntimeError]: https://docs.python.org/3/builtins/exceptions.html#RuntimeError
[KeyboardInterrupt]: https://docs.python.org/3/builtins/exceptions.html#KeyboardInterrupt

#### Asynchronous usage

!!! note

    WinRT async methods look like Python coroutines (methods defined with
    `async def`) but they are not. This means they do not return a
    [`Coroutine`](https://docs.python.org/3/library/collections.abc.html#collections.abc.Coroutine)
    object and therefore cannot be used with methods like
    [`asyncio.create_task()`](https://docs.python.org/3/library/asyncio-task.html#asyncio.create_task).
    They are only [`Awaitable`][Awaitable] objects.

If you are using `asyncio`, then you can use the `await` keyword to wait
for the result of async WinRT methods:

```python
thing = await winrt_obj.get_thing_async()
```

!!! version-changed "Changed in version 3.2"

    If the [`Awaitable`][Awaitable] that wraps the operation is
    canceled, it will now propagate the cancellation to the WinRT async
    action/operation. To restore the previous behavior, you can wrap the
    operation in [`asyncio.shield()`](https://docs.python.org/3/library/asyncio-task.html#asyncio.shield).

[Awaitable]: https://docs.python.org/3/library/collections.abc.html#collections.abc.Awaitable

#### Other usage

If you can't use either the synchronous helpers or `asyncio` (i.e. a GUI app
that doesn't use asyncio), then you can use the `completed`  property to set
a callback that will be called when the operation is complete. In GUI apps where
the async method is called from the main thread (that is initialized as a single-
threaded apartment), the callback will occur on the main thread. Otherwise, the
callback will be called on a different thread.

You must also be careful about not creating a reference cycle to the operation,
otherwise it will cause a memory leak. This can happen if the callback is a
closure and references an object that references the operation itself.

!!! todo "Todo"

    add tips on how to iterate over progress events

### Buffers

[Windows.Storage.Streams.IBuffer][IBuffer] is projected as
[`winrt.system.Buffer`](api/system.md#buffer),
which is an alias for [`collections.abc.Buffer`](https://docs.python.org/3/library/collections.abc.html#collections.abc.Buffer).
When used as a method parameter, any Python object that implements the buffer
protocol can be used, for example, a [`bytearray`](https://docs.python.org/3/builtins/stdtypes.html#bytearray),
[`bytes`][bytes], or a [`memoryview`][memoryview].
Although care must be taken to ensure that immutable types like [`bytes`][bytes]
are not used when the WinRT API expects a writeable buffer! The WinRT type
system does not distinguish between read-only and writeable buffers, so there
isn't a way to enforce this at runtime.

Buffers received as a return value can likewise be used with anything that
supports the buffer protocol. For example, [`memoryview`][memoryview] can be used to
access the buffer memory directly. Or, the struct module can be used to unpack
formatted binary data.

Using native Python classes to access the memory is significantly more efficient
than using the WinRT [Windows.Storage.Streams.IDataReader][IDataReader] or
[Windows.Storage.Streams.IDataWriter][IDataWriter] classes.

[Windows.Foundation.IMemoryBuffer][IMemoryBuffer] may also be accessed using the Python buffer
protocol via the *Windows.Foundation.IMemoryBufferReference*. Care should be
taken since the underlying memory can be released.

[bytes]: https://docs.python.org/3/builtins/stdtypes.html#bytes
[memoryview]: https://docs.python.org/3/builtins/stdtypes.html#memoryview
[IBuffer]: https://learn.microsoft.com/en-us/uwp/api/windows.storage.streams.ibuffer
[IMemoryBuffer]: https://learn.microsoft.com/en-us/uwp/api/windows.foundation.imemorybuffer
[IDataReader]: https://learn.microsoft.com/en-us/uwp/api/windows.storage.streams.idatareader
[IDataWriter]: https://learn.microsoft.com/en-us/uwp/api/windows.storage.streams.idatawriter

### Collections

Most of the generic types in [Windows.Foundation.Collections][wfc] are projected
as [collections.abc](https://docs.python.org/3/library/collections.abc.html) types.
The WinRT runtime types are still present behind
the scenes, bute the type hints use only the Python standard library types to
encourage users to use the Pythonic APIs. Furthermore, types that inherit from
these interfaces are also extended to support the Pythonic APIs.

[wfc]: https://learn.microsoft.com/en-us/uwp/api/windows.foundation.collections

#### Sequences

[IVector&lt;T&gt;][IVector] is projected as [MutableSequence[T]][MutableSequence] and
[IVectorView&lt;T&gt;][IVectorView] is projected as [Sequence[T]][Sequence]. These types
behave very much like Python lists, so you can iterate them with a `for` loop, index
them `seq[0]`, slice them `seq[:3]` use them with `len(seq)` and search with
`value in seq`.
A slice is a [`winrt.system.Array`](#arrays), which is a `Sequence` as well, so
it can be indexed, sliced and searched the same way.

For mutable sequences, items can be modified with `seq[0] = value`, deleted
with `del seq[0]`, appended with `seq.append(value)` or `seq.extend(seq2)`,
inserted with `seq.insert(0, value)`, removed with `seq.remove(value)`, and
cleared with `seq.clear()`. You can also reverse the sequence in place with
`seq.reverse()`, pop an item at a specific index using `seq.pop(index)`
(or the last item with `seq.pop()`), or add items using the `+=` operator
like `seq += [value1, value2]`.

[IVector]: https://learn.microsoft.com/en-us/uwp/api/windows.foundation.collections.ivector-1
[MutableSequence]: https://docs.python.org/3/library/collections.abc.html#collections.abc.MutableSequence
[IVectorView]: https://learn.microsoft.com/en-us/uwp/api/windows.foundation.collections.ivectorview-1
[Sequence]: https://docs.python.org/3/library/collections.abc.html#collections.abc.Sequence

#### Mappings

[IMap&lt;K, V&gt;][IMap] is projected as [MutableMapping[K, V]][MutableMapping] and
[IMapView&lt;K, V&gt;][IMapView] is projected as [Mapping[K, V]][Mapping]. These types
behave like Python dictionaries, so you can access values with `map[key]` or
`map.get(key)`, check for keys with `key in map`, and iterate over keys with a
`for` loop. You can also retrieve all keys, values, or key/value pairs using
`map.keys()`, `map.values()`, and `map.items()` respectively. Equality operators
(`==` and `!=`) can be used to compare mappings for equality based on their
key-value pairs. You can also use `len(map)` to get the number of keys in the mapping.

!!! note

    Python iterates over the keys only which might not be what you expect
    if you are used to using the same types in .NET. In Python, you will write:

    ```python
    for key in map:
        print(key)
    ```

    To get both the keys and values, use the `items()` method:

    ```python
    for key, value in map.items():
        print(key, value)
    ```

For mutable mappings, you can add or update items with `map[key] = value`,
use `value = map.setdefault(key, default)` to insert a key with a default
value if it doesn't exist, delete items with `del map[key]`, and clear all
items with `map.clear()`. You can also remove and return a value while
removing the key using `map.pop(key)` or `map.popitem()`. Multiple values
can be set at the same time using `map.update(other)`.

There is also a special handling for `IIterable<IKeyValuePair<K, V>>` when
used as an argument to methods where a Python mapping is allowed. This means you
can pass a Python dictionary as the argument.

Example:

```python
data = NotificationData({"my_key": "my_value"})
```

[IMap]: https://learn.microsoft.com/en-us/uwp/api/windows.foundation.collections.imap-2
[MutableMapping]: https://docs.python.org/3/library/collections.abc.html#collections.abc.MutableMapping
[IMapView]: https://learn.microsoft.com/en-us/uwp/api/windows.foundation.collections.imapview-2
[Mapping]: https://docs.python.org/3/library/collections.abc.html#collections.abc.Mapping

#### Iterators

[IIterable&lt;T&gt;][IIterable] becomes [Iterable[T]][Iterable]. Any iterable Python
type can be used as an argument and return values can be used as any other iterable
in Python. Don't use the WinRT `first()` method to get an iterator, instead use the
builtin [`iter()`](https://docs.python.org/3/builtins/functions.html#iter) function
or other Python features like `for` loops.

[IIterator&lt;T&gt;][IIterator] is projected as [Iterator[T]][Iterator] however these
objects are rarely used directly in Python. Instead, use `for` loops or generator
expressions to do the iterating for you. In rare cases, iterators might be used with
[`next()`](https://docs.python.org/3/builtins/functions.html#next).
The WinRT methods on this object should be avoided.

[IIterable]: https://learn.microsoft.com/en-us/uwp/api/windows.foundation.collections.iiterable-1
[Iterable]: https://docs.python.org/3/library/collections.abc.html#collections.abc.Iterable
[IIterator]: https://learn.microsoft.com/en-us/uwp/api/windows.foundation.collections.iiterator-1
[Iterator]: https://docs.python.org/3/library/collections.abc.html#collections.abc.Iterator

### Context managers

Any type that implements [IClosable](https://learn.microsoft.com/uwp/api/windows.foundation.iclosable)
can (and probably should) be used as a context manager in Python:

```python
from winrt.windows.foundation import MemoryBuffer

with MemoryBuffer(256) as buf:
    ...
```

Generally, when an object it closable, it means that there are unmanaged
resources that may need to be released in a deterministic manner as opposed
waiting for the garbage collector to run to clean them up.

!!! note

    .NET programmers may recognize this as similar to `using` statements
    in C# with `IDisposable` types.

### Date and time

There are a few foundational time-related types in WinRT that are projected
as the analogous Python types.

#### Windows.Foundation.DateTime

This type is converted to a [`datetime.datetime`](https://docs.python.org/3/library/datetime.html#datetime.datetime)
object from the standard Python library.

WinRT has a resolution of 100 nanoseconds while the Python type has a resolution
of 1 microsecond, so there is a small loss of precision in the conversion. Python
also has a much smaller allowed range of dates (years from 1 to 9999).

WinRT serializes values of this type as 100s of nanoseconds since since January 1, 1601 (UTC).
It uses a signed 64-bit integer for this, so the `"q"` format string is used in Python.

WinRT uses UTC for all values, so any `datetime` object returned from a Windows
API will use that timezone. It is recommended to use the UTC timezone when
creating `datetime` objects to pass to Windows APIs as well. "Naive" `datetime`
objects (without a timezone) are assumed to use the local timezone and will
be converted to UTC.

Example:

```python
# set notification to expire 10 seconds from now
toaster.expiration_time = datetime.now(timezone.utc) + timedelta(seconds=10)
```

#### Windows.Foundation.TimeSpan

This type is converted to a [`datetime.timedelta`](https://docs.python.org/3/library/datetime.html#datetime.timedelta)
object from the standard Python library.

WinRT has a resolution of 100 nanoseconds while the Python type has a resolution
of 1 microsecond, so there is a small loss of precision in the conversion. Python
is also limited to +/-999999999 days.

WinRT serializes values of this type as 100s of nanoseconds.
It uses a signed 64-bit integer for this, so the `"q"` format string is used in Python.
