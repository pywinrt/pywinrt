import array
import sys
import types
import typing

MTA: int
STA: int

def init_apartment(apartment_type: int) -> None: ...
def uninit_apartment() -> None: ...
def initialize_with_window(obj: Object, hwnd: int) -> None: ...

class Object: ...

_Self = typing.TypeVar("_Self")
_T = typing.TypeVar("_T")

class Array(typing.MutableSequence[_T]):
    _winrt_element_type_name_: str
    if sys.version_info >= (3, 9):
        def __class_getitem__(cls, key: typing.Any) -> types.GenericAlias: ...

    @typing.overload
    def __new__(
        cls: typing.Type[_Self],
        __type: typing.Union[typing.Type[_T], str],
        __size: int,
        /,
    ) -> _Self: ...
    @typing.overload
    def __new__(
        cls: typing.Type[_Self],
        __type: typing.Union[typing.Type[_T], str],
        __initializer: memoryview,
        /,
    ) -> _Self: ...
    @typing.overload
    def __new__(
        cls: typing.Type[_Self], __type: str, __initializer: array.array, /
    ) -> _Self: ...
    @typing.overload
    def __new__(
        cls: typing.Type[_Self],
        __type: typing.Union[typing.Type[_T], str],
        __initializer: Array[_T],
        /,
    ) -> _Self: ...
    @typing.overload
    def __new__(
        cls: typing.Type[_Self],
        __type: typing.Union[typing.Type[_T], str],
        __initializer: typing.List[_T],
        /,
    ) -> _Self: ...
    @typing.overload
    def __new__(
        cls: typing.Type[_Self],
        __type: typing.Union[typing.Type[_T], str],
        __initializer: typing.Tuple[_T],
        /,
    ) -> _Self: ...
    def __len__(self) -> int: ...
    @typing.overload
    def __getitem__(self, index: typing.SupportsIndex) -> _T: ...
    @typing.overload
    def __getitem__(self, index: slice) -> typing.MutableSequence[_T]: ...
    @typing.overload
    def __setitem__(self, index: typing.SupportsIndex, value: _T) -> None: ...
    @typing.overload
    def __setitem__(self, index: slice, value: typing.Iterable[_T]) -> None: ...
    @typing.overload
    def __delitem__(self, index: typing.SupportsIndex) -> None: ...
    @typing.overload
    def __delitem__(self, index: slice) -> None: ...
    def insert(self, index: typing.SupportsIndex, value: _T) -> None: ...
