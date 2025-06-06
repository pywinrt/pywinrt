import asyncio
import os
from collections.abc import (
    Awaitable,
    Callable,
    Mapping,
    MutableMapping,
    MutableSequence,
    Sequence,
)
from pathlib import Path
import sys
from typing import Any, TypeVar, Protocol, TYPE_CHECKING

from typing_extensions import Self

# NB: have to import Object from here instead of winrt.system to avoid circular import issues.
from winrt._winrt import add_dll_directory, remove_dll_directory, Object

# Unfortunately, we can't import at runtime because of circular imports.
if TYPE_CHECKING:
    from winrt.windows.foundation import HResult, AsyncStatus


class _DllCookie:
    def __init__(self, cookie: int) -> None:
        self.cookie = cookie

    def close(self):
        if self.cookie:
            remove_dll_directory(self.cookie)
            self.cookie = None

    def __del__(self):
        self.close()


def register_dll_search_path(module_path: str) -> _DllCookie:
    """
    Register a module's directory as a DLL search path.

    Args:
        module_path: The path to a module file (i.e. ``__file__``)

    Returns:
        An cookie object that will remove the search path when closed.
    """
    return _DllCookie(add_dll_directory(os.fspath(Path(module_path).parent.resolve())))


# NB: The types implemented in C cannot inherit from abc.ABC since Python 3.12
# so we have to implement the protocols like this instead.
# https://github.com/python/cpython/issues/103968#issuecomment-1589928055


def mixin_sequence(typ: type) -> None:
    """
    Adds missing Python mapping methods to types that implement IVectorView and
    registers the type as a Sequence.
    """
    # mixin methods
    if not hasattr(typ, "index"):
        typ.index = Sequence.index  # type: ignore

    if not hasattr(typ, "count"):
        typ.count = Sequence.count  # type: ignore

    if not hasattr(typ, "__contains__"):
        typ.__contains__ = Sequence.__contains__  # type: ignore

    if not hasattr(typ, "__iter__"):
        typ.__iter__ = Sequence.__iter__  # type: ignore

    if not hasattr(typ, "__reversed__"):
        typ.__reversed__ = Sequence.__reversed__  # type: ignore

    Sequence.register(typ)


def mixin_mutable_sequence(typ: type) -> None:
    """
    Adds missing Python mapping methods to types that implement IVector and
    registers the type as a MutableSequence.
    """
    mixin_sequence(typ)

    if not hasattr(typ, "insert") and hasattr(typ, "insert_at"):

        def insert(self: Any, index: int, value: object) -> None:
            """
            Alias for ``insert_at`` for compatibility with Python Sequence protocol.
            """
            self.insert_at(index, value)

        typ.insert = insert  # type: ignore

    # mixin methods
    if not hasattr(typ, "append"):
        typ.append = MutableSequence.append  # type: ignore

    if not hasattr(typ, "clear"):
        typ.clear = MutableSequence.clear  # type: ignore

    if not hasattr(typ, "extend"):
        typ.extend = MutableSequence.extend  # type: ignore

    if not hasattr(typ, "reverse"):
        typ.reverse = MutableSequence.reverse  # type: ignore

    if not hasattr(typ, "pop"):
        typ.pop = MutableSequence.pop  # type: ignore

    if not hasattr(typ, "remove"):
        typ.remove = MutableSequence.remove  # type: ignore

    if not hasattr(typ, "__iadd__"):
        typ.__iadd__ = MutableSequence.__iadd__  # type: ignore

    MutableSequence.register(typ)


def mixin_mapping(typ: type) -> None:
    """
    Adds missing Python mapping methods to types that implement IMapView and
    registers the type as a Mapping.
    """
    # mixin methods
    if not hasattr(typ, "keys"):
        typ.keys = Mapping.keys  # type: ignore

    if not hasattr(typ, "items"):
        typ.items = Mapping.items  # type: ignore

    if not hasattr(typ, "values"):
        typ.values = Mapping.values  # type: ignore

    if not hasattr(typ, "get"):
        typ.get = Mapping.get  # type: ignore

    if not hasattr(typ, "__contains__"):
        typ.__contains__ = Mapping.__contains__  # type: ignore

    # HACK: Version check works around inheritance anomaly caused by hacky
    # metaclass inheritance implementation in runtime.cpp. This works as long
    # as we don't change the projection to implement rich comparison methods
    # on mapping types.

    if (
        typ.__eq__ == object.__eq__
        or typ.__eq__ == Object.__eq__
        or sys.version_info < (3, 12)
    ):
        typ.__eq__ = Mapping.__eq__  # type: ignore

    if (
        typ.__ne__ == object.__ne__
        or typ.__ne__ == Object.__ne__
        or sys.version_info < (3, 12)
    ):
        typ.__ne__ = Mapping.__ne__  # type: ignore

    Mapping.register(typ)


def mixin_mutable_mapping(typ: type) -> None:
    """
    Adds missing Python mapping methods to types that implement IMap and
    registers the type as a MutableMapping.
    """
    mixin_mapping(typ)

    # mixin methods
    if not hasattr(typ, "clear"):
        typ.clear = MutableMapping.clear  # type: ignore

    if not hasattr(typ, "pop"):
        typ.pop = MutableMapping.pop  # type: ignore
        # private-name-mangled attribute :-(
        typ._MutableMapping__marker = MutableMapping._MutableMapping__marker  # type: ignore

    if not hasattr(typ, "popitem"):
        typ.popitem = MutableMapping.popitem  # type: ignore

    if not hasattr(typ, "setdefault"):
        typ.setdefault = MutableMapping.setdefault  # type: ignore

    if not hasattr(typ, "update"):
        typ.update = MutableMapping.update  # type: ignore

    MutableMapping.register(typ)


T = TypeVar("T")


# MyPy wants covariant T but Pylance wants invariant. Invariant seems correct.
class AsyncOp(Protocol[T]):  # type: ignore [misc]
    """
    Protocol that matches both IAsyncAction and IAsyncOperation.
    """

    def cancel(self) -> None: ...
    @property
    def error_code(self) -> "HResult": ...
    @property
    def status(self) -> "AsyncStatus": ...
    def get_results(self) -> T: ...
    @property
    def completed(self) -> Callable[[Self, "AsyncStatus"], None]: ...
    @completed.setter
    def completed(self, value: Callable[[Self, "AsyncStatus"], None]) -> None: ...


def wrap_async(op: AsyncOp[T]) -> Awaitable[T]:
    """
    Wraps a WinRT async operation in a Python Awaitable.

    This is used by the runtime to implement ``__await__`` on async operations.

    Args:
        op: The WinRT async operation to wrap.

    Returns:
        An asyncio Awaitable that will be completed when the WinRT operation
        completes.
    """

    async def wait() -> T:
        loop = asyncio.get_running_loop()
        event = asyncio.Event()

        def on_complete(op: AsyncOp[T], status: "AsyncStatus") -> None:
            loop.call_soon_threadsafe(event.set)

        op.completed = on_complete

        try:
            await event.wait()
        except asyncio.CancelledError:
            op.cancel()
            # REVISIT: what if it is cancelled again?
            await event.wait()
            raise

        return op.get_results()

    return wait()
