from typing import TYPE_CHECKING, Annotated, Any, Protocol, TypeAlias, runtime_checkable
import warnings

from typing_extensions import Buffer as _Buffer, deprecated

from winrt._winrt import (
    Array,
    Object,
    Object_Static,
    box_boolean,
    box_char16,
    box_date_time,
    box_double,
    box_guid,
    box_int8,
    box_int16,
    box_int32,
    box_int64,
    box_single,
    box_string,
    box_time_span,
    box_uint8,
    box_uint16,
    box_uint32,
    box_uint64,
    unbox_boolean,
    unbox_char16,
    unbox_date_time,
    unbox_double,
    unbox_guid,
    unbox_int8,
    unbox_int16,
    unbox_int32,
    unbox_int64,
    unbox_single,
    unbox_string,
    unbox_time_span,
    unbox_uint8,
    unbox_uint16,
    unbox_uint32,
    unbox_uint64,
)
import winrt.runtime._internals


class BufferFormat(str):
    """
    The `PEP 3118`_ buffer format of a WinRT fundamental type.

    This is what :attr:`memoryview.format` reports for an :class:`Array` of
    that type and what :class:`Array` accepts in place of the type.

    .. _PEP 3118: https://peps.python.org/pep-3118/
    """

    __slots__ = ()

    _annotation_kind_ = "buffer_format"


class StructFormat(str):
    """
    The :mod:`struct` module format of a WinRT fundamental type.

    It is the same character as the buffer format for every type but
    ``Char16``, where it is ``"H"``, because :mod:`struct` has no ``"u"``
    format at all.
    """

    __slots__ = ()

    _annotation_kind_ = "struct_format"


class WinrtSignature(str):
    """
    The WinRT type signature of a WinRT fundamental type.

    The signature of a parameterized type is composed from the signatures of
    its type arguments, and its interface identifier is hashed from that.
    """

    __slots__ = ()

    _annotation_kind_ = "winrt_signature"


# The fundamental types that no Python type names on its own, each annotated
# with one spelling of what it is per kind of format. A consumer finds the one
# it wants by isinstance over typing.get_args(), never by position.
Int8 = Annotated[int, BufferFormat("b"), StructFormat("b"), WinrtSignature("i1")]
UInt8 = Annotated[int, BufferFormat("B"), StructFormat("B"), WinrtSignature("u1")]
Int16 = Annotated[int, BufferFormat("h"), StructFormat("h"), WinrtSignature("i2")]
UInt16 = Annotated[int, BufferFormat("H"), StructFormat("H"), WinrtSignature("u2")]
Int32 = Annotated[int, BufferFormat("i"), StructFormat("i"), WinrtSignature("i4")]
UInt32 = Annotated[int, BufferFormat("I"), StructFormat("I"), WinrtSignature("u4")]
Int64 = Annotated[int, BufferFormat("q"), StructFormat("q"), WinrtSignature("i8")]
UInt64 = Annotated[int, BufferFormat("Q"), StructFormat("Q"), WinrtSignature("u8")]
Single = Annotated[float, BufferFormat("f"), StructFormat("f"), WinrtSignature("f4")]
Double = Annotated[float, BufferFormat("d"), StructFormat("d"), WinrtSignature("f8")]
Char16 = Annotated[str, BufferFormat("u"), StructFormat("H"), WinrtSignature("c2")]

winrt.runtime._internals.mixin_sequence(Array)

# What an IBuffer or an array parameter takes besides an Array: any buffer,
# named for whether WinRT reads it or writes into it, as typeshed's aliases of
# the same names are.
ReadableBuffer: TypeAlias = _Buffer
WriteableBuffer: TypeAlias = _Buffer

# PEP 702 deprecates a class or a function, not an alias, so the type checkers
# see a deprecated copy of typeshed's collections.abc.Buffer, and the real one
# is what __getattr__() hands out at run time.
if TYPE_CHECKING:

    @deprecated(
        "Use collections.abc.Buffer instead, "
        "or typing_extensions.Buffer before Python 3.12"
    )
    @runtime_checkable
    class Buffer(Protocol):
        def __buffer__(self, flags: int, /) -> memoryview: ...

else:

    def __getattr__(name: str) -> Any:
        if name == "Buffer":
            warnings.warn(
                "winrt.system.Buffer is deprecated: use collections.abc.Buffer, "
                "or typing_extensions.Buffer before Python 3.12",
                DeprecationWarning,
                stacklevel=2,
            )
            return _Buffer

        raise AttributeError(f"module {__name__!r} has no attribute {name!r}")


__all__ = [
    "BufferFormat",
    "StructFormat",
    "WinrtSignature",
    "Int8",
    "UInt8",
    "Int16",
    "UInt16",
    "Int32",
    "UInt32",
    "Int64",
    "UInt64",
    "Single",
    "Double",
    "Char16",
    "Object",
    "Object_Static",
    "Array",
    "ReadableBuffer",
    "WriteableBuffer",
    "box_boolean",
    "box_int8",
    "box_uint8",
    "box_int16",
    "box_uint16",
    "box_int32",
    "box_uint32",
    "box_int64",
    "box_uint64",
    "box_single",
    "box_double",
    "box_char16",
    "box_string",
    "box_guid",
    "box_date_time",
    "box_time_span",
    "unbox_boolean",
    "unbox_int8",
    "unbox_uint8",
    "unbox_int16",
    "unbox_uint16",
    "unbox_int32",
    "unbox_uint32",
    "unbox_int64",
    "unbox_uint64",
    "unbox_single",
    "unbox_double",
    "unbox_char16",
    "unbox_string",
    "unbox_guid",
    "unbox_date_time",
    "unbox_time_span",
]
