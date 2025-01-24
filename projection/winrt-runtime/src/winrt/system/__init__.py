import os
import sys
from collections.abc import Mapping, MutableMapping, MutableSequence, Sequence
from pathlib import Path
from typing import Annotated

from typing_extensions import Buffer, TypeAlias

from .._winrt import (
    Array,
    Object,
    _add_dll_directory,
    _remove_dll_directory,
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


class _DllCookie:
    def __init__(self, cookie: int) -> None:
        self.cookie = cookie

    def close(self):
        if self.cookie:
            _remove_dll_directory(self.cookie)
            self.cookie = None

    def __del__(self):
        self.close()


def _register_dll_search_path(module_path) -> _DllCookie:
    """
    Register a module's directory as a DLL search path.

    Args:
        module_path: The path to a module file (i.e. ``__file__``)

    Returns:
        An cookie object that will remove the search path when closed.
    """
    return _DllCookie(_add_dll_directory(os.fspath(Path(module_path).parent.resolve())))


# NB: The types implemented in C cannot inherit from abc.ABC since Python 3.12
# so we have to implement the protocols like this instead.
# https://github.com/python/cpython/issues/103968#issuecomment-1589928055


def _mixin_sequence(typ) -> None:
    """
    Adds missing Python mapping methods to types that implement IVectorView and
    registers the type as a Sequence.
    """
    # mixin methods
    if not hasattr(typ, "index"):
        typ.index = Sequence.index

    if not hasattr(typ, "count"):
        typ.count = Sequence.count

    if not hasattr(typ, "__contains__"):
        typ.__contains__ = Sequence.__contains__

    if not hasattr(typ, "__iter__"):
        typ.__iter__ = Sequence.__iter__

    if not hasattr(typ, "__reversed__"):
        typ.__reversed__ = Sequence.__reversed__

    Sequence.register(typ)


def _mixin_mutable_sequence(typ) -> None:
    """
    Adds missing Python mapping methods to types that implement IVector and
    registers the type as a MutableSequence.
    """
    _mixin_sequence(typ)

    if not hasattr(typ, "insert") and hasattr(typ, "insert_at"):

        def insert(self, index: int, value: object) -> None:
            """
            Alias for ``insert_at`` for compatibility with Python Sequence protocol.
            """
            self.insert_at(index, value)

        typ.insert = insert

    # mixin methods
    if not hasattr(typ, "append"):
        typ.append = MutableSequence.append

    if not hasattr(typ, "clear"):
        typ.clear = MutableSequence.clear

    if not hasattr(typ, "extend"):
        typ.extend = MutableSequence.extend

    if not hasattr(typ, "reverse"):
        typ.reverse = MutableSequence.reverse

    if not hasattr(typ, "pop"):
        typ.pop = MutableSequence.pop

    if not hasattr(typ, "remove"):
        typ.remove = MutableSequence.remove

    if not hasattr(typ, "__iadd__"):
        typ.__iadd__ = MutableSequence.__iadd__

    MutableSequence.register(typ)


def _mixin_mapping(typ) -> None:
    """
    Adds missing Python mapping methods to types that implement IMapView and
    registers the type as a Mapping.
    """
    # mixin methods
    if not hasattr(typ, "keys"):
        typ.keys = Mapping.keys

    if not hasattr(typ, "items"):
        typ.items = Mapping.items

    if not hasattr(typ, "values"):
        typ.values = Mapping.values

    if not hasattr(typ, "get"):
        typ.get = Mapping.get

    if not hasattr(typ, "__contains__"):
        typ.__contains__ = Mapping.__contains__

    # HACK: Version check works around inheritance anomaly caused by hacky
    # metaclass inheritance implementation in runtime.cpp. This works as long
    # as we don't change the projection to implement rich comparison methods
    # on mapping types.

    if (
        typ.__eq__ == object.__eq__
        or typ.__eq__ == Object.__eq__
        or sys.version_info < (3, 12)
    ):
        typ.__eq__ = Mapping.__eq__

    if (
        typ.__ne__ == object.__ne__
        or typ.__ne__ == Object.__ne__
        or sys.version_info < (3, 12)
    ):
        typ.__ne__ = Mapping.__ne__

    Mapping.register(typ)


def _mixin_mutable_mapping(typ) -> None:
    """
    Adds missing Python mapping methods to types that implement IMap and
    registers the type as a MutableMapping.
    """
    _mixin_mapping(typ)

    # mixin methods
    if not hasattr(typ, "clear"):
        typ.clear = MutableMapping.clear

    if not hasattr(typ, "pop"):
        typ.pop = MutableMapping.pop
        # private-name-mangled attribute :-(
        typ._MutableMapping__marker = MutableMapping._MutableMapping__marker  # type: ignore

    if not hasattr(typ, "popitem"):
        typ.popitem = MutableMapping.popitem

    if not hasattr(typ, "setdefault"):
        typ.setdefault = MutableMapping.setdefault

    if not hasattr(typ, "update"):
        typ.update = MutableMapping.update

    MutableMapping.register(typ)


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

_mixin_mutable_sequence(Array)

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
