import array
import types
import typing
import sys
import uuid

if sys.version_info >= (3, 9):
    from typing import Annotated

    # type aliases with struct format string annotation
    Boolean = Annotated[bool, "?"]
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
else:
    Boolean = bool
    Int8 = int
    UInt8 = int
    Int16 = int
    UInt16 = int
    Int32 = int
    UInt32 = int
    Int64 = int
    UInt64 = int
    Single = float
    Double = float

Char16 = UInt16  # Char16 also implies valid UTF-16 codepoint

# WinRT GUIDs are marshaled to Python UUIDs
Guid = uuid.UUID

MTA: int
STA: int

def init_apartment(apartment_type: int) -> None: ...
def uninit_apartment() -> None: ...
def initialize_with_window(obj: Object, hwnd: int) -> None: ...

class Object: ...

_Self = typing.TypeVar("_Self")
_T = typing.TypeVar("_T")

class Array(typing.Generic[_T]):
    if sys.version_info >= (3, 9):
        def __class_getitem__(cls, key: typing.Any) -> types.GenericAlias: ...
    @typing.overload
    def __new__(cls: _Self, __type: typing.Union[_T, str], __size: int, /) -> _Self: ...
    @typing.overload
    def __new__(cls: _Self, __type: typing.Union[_T, str], __initializer: memoryview, /) -> _Self: ...
    @typing.overload
    def __new__(cls: _Self, __type: str, __initializer: array.array, /) -> _Self: ...
    @typing.overload
    def __new__(cls: _Self, __type: typing.Union[_T, str], __initializer: Array[_T], /) -> _Self: ...
    @typing.overload
    def __new__(cls: _Self, __type: typing.Union[_T, str], __initializer: typing.List[_T], /) -> _Self: ...
    @typing.overload
    def __new__(
        cls: _Self, __type: typing.Union[_T, str], __initializer: typing.Tuple[_T], /
    ) -> _Self: ...
    def __len__(self) -> int: ...
    def __getitem__(self, __i: typing.SupportsIndex) -> _T: ...
