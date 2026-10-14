# `winrt.system` module

## Type aliases

### `Buffer`

```python
type Buffer
```

Alias of [`collections.abc.Buffer`][Buffer]. The type hints of what goes into
WinRT name it as `ReadableBuffer` or `WriteableBuffer`, which say whether
WinRT reads the buffer or writes into it.

!!! version-added "Added in version 3.2"

!!! deprecated "Deprecated since version 4.0"

    Use [`ReadableBuffer`](#readablebuffer) or
    [`WriteableBuffer`](#writeablebuffer) to say whether WinRT reads the buffer
    or writes into it, or [`collections.abc.Buffer`][Buffer] itself
    (`typing_extensions.Buffer` before Python 3.12). Importing `Buffer` from
    `winrt.system` warns with a `DeprecationWarning`.

!!! seealso "See also"

    [Buffers](../types.md#buffers)

### `ReadableBuffer`

```python
type ReadableBuffer
```

Alias of [`collections.abc.Buffer`][Buffer] that says WinRT only reads the
buffer, as the same name says in the standard library's type hints. The type
hints use it for an [`IBuffer`](../types.md#buffers) that goes in, and beside
`Array` for an [array parameter](../types.md#arrays-as-parameters) that WinRT
reads. A type checker sees only `Buffer`. An `IBuffer` takes the bytes of any
buffer, and an array checks the layout when the method is called: a buffer
whose layout is not that of the array's element type raises `BufferError`.

!!! version-added "Added in version 2.3"

### `WriteableBuffer`

```python
type WriteableBuffer
```

Alias of [`collections.abc.Buffer`][Buffer] that says WinRT writes into the
buffer, as the same name says in the standard library's type hints. The type
hints use it for an [`IBuffer`](../types.md#buffers) that WinRT fills, and
beside `Array` for an [array parameter](../types.md#arrays-as-parameters) that
WinRT fills. A type checker sees only `Buffer`, so the buffer is checked when
the method is called. A read-only one, such as `bytes`, raises `TypeError` for
an `IBuffer` and `BufferError` for an array, as does one whose layout is not
that of the array's element type.

!!! version-added "Added in version 2.3"

[Buffer]: https://docs.python.org/3/library/collections.abc.html#collections.abc.Buffer

## Annotations

The aliases this module gives the WinRT fundamental types that no Python
type names on its own - `Int8`, `UInt8`, `Int16`, `UInt16`,
`Int32`, `UInt32`, `Int64`, `UInt64`, `Single`, `Double` and
`Char16` - are [`typing.Annotated`](https://docs.python.org/3/library/typing.html#typing.Annotated)
aliases of the Python type each one is projected as. The annotations say what
that WinRT type is called in each of three languages, and each one is found by
its own class:

```python
from typing import get_args
from winrt.system import Int32, WinrtSignature

signature = next(
    a for a in get_args(Int32)[1:] if isinstance(a, WinrtSignature)
)
```

### `BufferFormat`

```python
class BufferFormat(str)
```

The [PEP 3118][pep3118] buffer format of a WinRT fundamental type, such as
`"i"` for `Int32`. This is what
[`memoryview.format`](https://docs.python.org/3/builtins/stdtypes.html#memoryview.format)
reports for an [`Array`](#array) of that type.

!!! version-added "Added in version 4.0"

### `StructFormat`

```python
class StructFormat(str)
```

The [`struct`][struct] module format of a WinRT fundamental type. It is the
same character as the buffer format for every type but `Char16`,
where it is `"H"`, because [`struct`][struct] has no `"u"` format at
all.

!!! version-added "Added in version 4.0"

### `WinrtSignature`

```python
class WinrtSignature(str)
```

The WinRT type signature of a WinRT fundamental type, such as `"i4"`
for `Int32`. The signature of a parameterized type is composed from
the signatures of its type arguments, and its interface identifier is
hashed from that.

!!! version-added "Added in version 4.0"

[pep3118]: https://peps.python.org/pep-3118/
[struct]: https://docs.python.org/3/library/struct.html#module-struct

## Fundamental types

### `Object`

```python
class Object
```

A wrapper around the WinRT `System.Object` type.

This is the base type of all WinRT runtime objects and cannot be
instantiated directly. A Python class that implements a WinRT interface is
an `Object` too: `isinstance()` and `issubclass()` say so, although it does
not derive from `Object`, which holds a WinRT object that a Python
implementation does not have, and its instances have the members below,
which answer what WinRT is told about them.

!!! version-changed "Changed in version 4.0"
    A Python implementation of a WinRT interface is an instance of `Object`,
    with its members.

#### `as_`

```python
as_(type, /)
```

The object as another interface it implements, or as its runtime class. Raises
`OSError` if the object does not implement it, and `TypeError` if `type` is
not a runtime class or an interface. See
[Query interface](../types.md#query-interface).

!!! version-added "Added in version 3.0"

**Introspection attributes**

WinRT objects have the following attributes for inspecting various metatdata
at runtime:

#### `_runtime_class_name_`

```python
_runtime_class_name_: str
```

Gets the WinRT runtime class name of the object. A Python implementation of
an interface is no runtime class, so it gives the name of the first interface
among its class's bases: `class Converter(IValueConverter)` answers with the
name of `IValueConverter`.

!!! version-added "Added in version 2.1"

#### `_iids_`

```python
_iids_: Array[uuid.UUID]
```

Gets the Interface Identifiers (IIDS) of the WinRT interfaces
implemented by the object.

!!! version-added "Added in version 2.1"

### `Object_Static`

```python
class Object_Static(type)
```

The metaclass of `Object` and of every projected class and interface. A
class's static members are on a metaclass of its own, named after the class
with `_Static`, such as `Uri_Static`, which derives from this one. The type
hints name it, and code has no need to.

!!! version-added "Added in version 4.0"

### `Array`

```python
class Array(type, [initializer, ] /)
```

A wrapper around the WinRT `System.Array` type.

This type implements the Python sequence protocol.

| Parameter | Type | Description |
|---|---|---|
| `type` | [`type`](https://docs.python.org/3/builtins/functions.html#type) | The type to use for elements of the array. This is a projected WinRT type, a Python type that a WinRT type is projected as, or one of the `winrt.system` aliases for a fundamental type. |
| `initializer` | `int` or `iter` or `buffer` | An optional iterator of values to use to initialize the array. If an integer value is given, an empty array of that size will be initialized. For value types, any object supporting the CPython buffer protocol with the correct layout can be used as an initializer. |

!!! seealso "See also"

    [Arrays](../types.md#arrays) for how an array behaves and where arrays
    appear in the projection.

!!! version-changed "Changed in version 4.0"

    * `Array` is a [`Sequence`](https://docs.python.org/3/library/collections.abc.html#collections.abc.Sequence)
      with item assignment rather than a `MutableSequence`, and
      `insert()` was removed.
    * Added slicing, equality with another `Array` and a `repr()`.
    * The element type can be one of the `winrt.system` aliases.
    * `Array` is `@typing.final`.
    * `Array` is safe to use from several threads on the free-threaded build.

!!! deprecated "Deprecated since version 4.0"

    Passing a format string, such as `"I"`, in place of the type.
    Every format string has a type that names the same thing: a
    `winrt.system` alias for the eleven scalars, [`bool`](https://docs.python.org/3/builtins/functions.html#bool)
    for `"?"` and the enum type itself for `"i"` and `"I"`.

Creation examples:

```python
from winrt.system import Array, UInt32
from winrt.windows.foundation import Point

# array of 10 32-bit unsigned integers.
a1 = Array(UInt32, 10)
# array of 3 points with initial values
a2 = Array(Point, [Point(1, 1), Point(2, 2), Point(3, 3)])
```

Sequence protocol examples:

```python
# get the number of elements in the array
size = len(a1)
# get the first element of the array
item = a1[0]
# get the last element of the array
item = a1[-1]
# iterate all items of the array
for item in a1: ...
# test for element in array
if item in a1: ...
```

## Boxing

Some APIs require a [`winrt.system.Object`](#object) to be passed as a parameter.
In order to pass other types like strings and numbers, they must be boxed.
Likewise, when a method returns a [`winrt.system.Object`](#object), it may need to
be unboxed to get the original value.

### `box_*`

```python
box_boolean(value: bool, /) -> Object
box_int8(value: int, /) -> Object
box_uint8(value: int, /) -> Object
box_int16(value: int, /) -> Object
box_uint16(value: int, /) -> Object
box_int32(value: int, /) -> Object
box_uint32(value: int, /) -> Object
box_int64(value: int, /) -> Object
box_uint64(value: int, /) -> Object
box_single(value: float, /) -> Object
box_double(value: float, /) -> Object
box_char16(value: str, /) -> Object
box_string(value: str, /) -> Object
box_guid(value: uuid.UUID, /) -> Object
box_date_time(value: datetime.datetime, /) -> Object
box_time_span(value: datetime.timedelta, /) -> Object
```

Boxes the given value into a [`winrt.system.Object`](#object).

Essentially, this is shorthand for calling:

```python
from winrt.windows.foundation import PropertyValue

obj = PropertyValue.create_xyz(value)
```

!!! version-added "Added in version 3.0"

### `unbox_*`

```python
unbox_boolean(value: Object, /) -> bool
unbox_int8(value: Object, /) -> int
unbox_uint8(value: Object, /) -> int
unbox_int16(value: Object, /) -> int
unbox_uint16(value: Object, /) -> int
unbox_int32(value: Object, /) -> int
unbox_uint32(value: Object, /) -> int
unbox_int64(value: Object, /) -> int
unbox_uint64(value: Object, /) -> int
unbox_single(value: Object, /) -> float
unbox_double(value: Object, /) -> float
unbox_char16(value: Object, /) -> str
unbox_string(value: Object, /) -> str
unbox_guid(value: Object, /) -> uuid.UUID
unbox_date_time(value: Object, /) -> datetime.datetime
unbox_time_span(value: Object, /) -> datetime.timedelta
```

Unboxes the given [`winrt.system.Object`](#object) into the original value.

The value has to be boxed as that type: `unbox_int32()` takes what
`box_int32()` makes, and not an `Int16` or an `Int64`. The `get_*()` methods of
`IPropertyValue` convert between numeric types where they can:

```python
from winrt.windows.foundation import IPropertyValue

value = obj.as_(IPropertyValue).get_int32()
```

**Raises:** [`TypeError`][TypeError] if the value is not boxed as that type.

!!! version-added "Added in version 3.0"

!!! version-changed "Changed in version 4.0"

    A value boxed as another type raises `TypeError`. Previously it raised
    `OSError`.

[TypeError]: https://docs.python.org/3/builtins/exceptions.html#TypeError
