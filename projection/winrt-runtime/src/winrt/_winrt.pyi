import array
import types
from typing import (
    Any,
    Generic,
    ItemsView,
    Iterable,
    Iterator,
    KeysView,
    List,
    SupportsIndex,
    Tuple,
    Type,
    TypeVar,
    Union,
    ValuesView,
    overload,
)
from uuid import UUID

MTA: int
STA: int

def init_apartment(apartment_type: int) -> None: ...
def uninit_apartment() -> None: ...
def initialize_with_window(obj: Object, hwnd: int) -> None: ...

_Self = TypeVar("_Self")
_T = TypeVar("_T")
_T_co = TypeVar("_T_co", covariant=True)  # Any type covariant containers.
_KT = TypeVar("_KT")
_VT = TypeVar("_VT")
_VT_co = TypeVar("_VT_co", covariant=True)  # Value type covariant containers.

# these classes don't actually exist but are just used to simplify type checking

class Sequence(Generic[_T_co]):
    # collections.abc.Sequence mixin methods
    def index(self, value: Any, start: int = 0, stop: int = ...) -> int: ...
    def count(self, value: Any) -> int: ...
    def __reversed__(self) -> Iterator[_T_co]: ...

class MutableSequence(Sequence[_T]):
    # alias for insert_at
    def insert(self, index: SupportsIndex, value: _T) -> None: ...
    # collections.abc.MutableSequence mixin methods
    def extend(self, values: Iterable[_T]) -> None: ...
    def reverse(self) -> None: ...
    def pop(self, index: int = -1) -> _T: ...
    def remove(self, value: _T) -> None: ...
    def __iadd__(self: _Self, values: Iterable[_T]) -> _Self: ...

class Mapping(Generic[_KT, _VT_co]):
    # collections.abc.Mapping mixin methods
    @overload
    def get(self, __key: _KT) -> _VT_co | None: ...
    @overload
    def get(self, __key: _KT, default: _VT_co | _T) -> _VT_co | _T: ...
    def items(self) -> ItemsView[_KT, _VT_co]: ...
    def keys(self) -> KeysView[_KT]: ...
    def values(self) -> ValuesView[_VT_co]: ...
    def __eq__(self, __other: object) -> bool: ...

class MutableMapping(Mapping[_KT, _VT]):
    # collections.abc.MutableMapping mixin methods
    @overload
    def pop(self, __key: _KT) -> _VT: ...
    @overload
    def pop(self, __key: _KT, default: _VT) -> _VT: ...
    @overload
    def pop(self, __key: _KT, default: _T) -> _VT | _T: ...
    def popitem(self) -> Tuple[_KT, _VT]: ...
    @overload
    def setdefault(
        self: MutableMapping[_KT, _T | None], __key: _KT, __default: None = None
    ) -> _T | None: ...
    @overload
    def setdefault(self, __key: _KT, __default: _VT) -> _VT: ...
    @overload
    def update(self, __m: Iterable[Tuple[_KT, _VT]], **kwargs: _VT) -> None: ...
    @overload
    def update(self, **kwargs: _VT) -> None: ...

# actual runtime classes

class Object:
    @property
    def _iids_(self) -> Array[UUID]: ...
    @property
    def _runtime_class_name_(self) -> str: ...

class Array(MutableSequence[_T]):
    _winrt_element_type_name_: str
    def __class_getitem__(cls, key: Any) -> types.GenericAlias: ...
    @overload
    def __init__(
        self,
        type: Union[Type[_T], str],
        size: int,
    ) -> None: ...
    @overload
    def __init__(
        self,
        type: Union[Type[_T], str],
        initializer: memoryview,
    ) -> None: ...
    @overload
    def __init__(self, type: str, initializer: array.array) -> None: ...
    @overload
    def __init__(
        self,
        type: Union[Type[_T], str],
        initializer: Array[_T],
    ) -> None: ...
    @overload
    def __init__(
        self,
        type: Union[Type[_T], str],
        initializer: List[_T],
        /,
    ) -> None: ...
    @overload
    def __init__(
        self,
        __type: Union[Type[_T], str],
        __initializer: Tuple[_T],
    ) -> None: ...
    def __len__(self) -> int: ...
    @overload
    def __getitem__(self, index: SupportsIndex) -> _T: ...
    @overload
    def __getitem__(self, index: slice) -> MutableSequence[_T]: ...
    @overload
    def __setitem__(self, index: SupportsIndex, value: _T) -> None: ...
    @overload
    def __setitem__(self, index: slice, value: Iterable[_T]) -> None: ...
    @overload
    def __delitem__(self, index: SupportsIndex) -> None: ...
    @overload
    def __delitem__(self, index: slice) -> None: ...
