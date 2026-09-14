from typing import Annotated

from typing_extensions import Buffer, TypeAlias

from winrt._winrt import (
    Array,
    Object,
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


# type aliases with struct format string annotation
Int8 = Annotated[int, "b"]
UInt8 = Annotated[int, "B"]
Int16 = Annotated[int, "h"]
UInt16 = Annotated[int, "H"]
Int32 = Annotated[int, "i"]
UInt32 = Annotated[int, "I"]
Int64 = Annotated[int, "q"]
UInt64 = Annotated[int, "Q"]
Single = Annotated[float, "f"]
Double = Annotated[float, "d"]
Char16 = Annotated[str, "u"]

winrt.runtime._internals.mixin_mutable_sequence(Array)

# Type hints for Python buffer protocol - can use standard Python types in
# addition to the WinRT Array.
ReadableBuffer: TypeAlias = Buffer  # WinRT PassArray
WriteableBuffer: TypeAlias = Buffer  # WinRT FillArray


__all__ = [
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
