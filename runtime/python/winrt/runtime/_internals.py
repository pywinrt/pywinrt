import asyncio
from importlib.machinery import ModuleSpec
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
from typing import Any, Self, TypeVar, Protocol, TYPE_CHECKING
import warnings

# NB: have to import Object from here instead of winrt.system to avoid circular import issues.
from winrt._winrt import (
    add_dll_directory,
    load_projection as _load_projection,
    remove_dll_directory,
    Object,
)

# Unfortunately, we can't import at runtime because of circular imports.
if TYPE_CHECKING:
    from winrt.windows.foundation import HResult, AsyncStatus


#: The file a projection package keeps its namespace's table in, beside the
#: ``__init__.py`` that loads it.
TABLE_NAME = "_table.pywinrt"


def load_projection(spec: ModuleSpec) -> None:
    """
    Creates the Python types of one WinRT namespace from its projection table.

    A projection package ships a table rather than an extension module, so this
    is what its ``__init__.py`` calls before anything else: the runtime reads
    the table beside it and builds the classes, interfaces and structs straight
    into the module. Everything after that line - the enums, the delegate
    aliases, the protocol mixins - is ordinary Python that expects them to be
    there.

    Args:
        spec: The module's own ``__spec__``, which says where it was loaded
            from and therefore where its table is.
    """
    if not spec.origin:
        raise ImportError(f"{spec.name} was not loaded from a file")

    _load_projection(
        sys.modules[spec.name], os.fspath(Path(spec.origin).parent / TABLE_NAME)
    )


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


# Staged deprecation of the method names that pywinrt v3.x used for overloaded
# methods. DeprecationWarning is aimed at developers of code that uses pywinrt.
# In a future release, this will become FutureWarning, which is shown to end
# users as well, and after that the aliases will be removed.
# https://docs.python.org/3/library/exceptions.html#DeprecationWarning
#
# NB: The calls to the functions below are generated. See the note on the
# aliases in PyWinRT/Projection/ProjectedType.cs for everything that has to be
# removed when the aliases go away.
_LEGACY_METHOD_WARNING: type[Warning] = DeprecationWarning


def _add_alias(target: type, owner: type, alias: str, name: str) -> None:
    # a real attribute of the type always wins over an alias
    if alias in vars(target):
        return

    def method(self: Any, *args: Any) -> Any:
        warnings.warn(
            f"{owner.__name__}.{alias}() is deprecated, use {name}() instead",
            _LEGACY_METHOD_WARNING,
            stacklevel=2,
        )
        return getattr(self, name)(*args)

    method.__name__ = alias
    method.__qualname__ = f"{owner.__name__}.{alias}"
    method.__doc__ = f"Deprecated alias of ``{name}()``."

    setattr(target, alias, method)


def alias_method(typ: type, alias: str, name: str) -> None:
    """
    Adds a deprecated alias of a method to a projected type.

    Args:
        typ: The projected type.
        alias: The name the method had in pywinrt v3.x.
        name: The name of the method now.
    """
    _add_alias(typ, typ, alias, name)


def alias_static_method(typ: type, alias: str, name: str) -> None:
    """
    Adds a deprecated alias of a static method to a projected type.

    Args:
        typ: The projected type.
        alias: The name the method had in pywinrt v3.x.
        name: The name of the method now.
    """
    # static methods are implemented by the metaclass of the projected type
    _add_alias(type(typ), typ, alias, name)


def alias_field(typ: type, name: str) -> None:
    """
    Adds a deprecated property for the one field a projected type used to have.

    A WinRT struct that holds a single integer - an HRESULT, an event token -
    is projected as a subclass of ``int`` rather than as a wrapper, so the
    value is the object itself. Reading the field by the name it had in
    pywinrt v3.x still works and warns.

    Args:
        typ: The projected type.
        name: The name the field had.
    """

    def field(self: Any) -> Any:
        warnings.warn(
            f"{typ.__name__}.{name} is deprecated, "
            f"the value is the {typ.__name__} itself",
            _LEGACY_METHOD_WARNING,
            stacklevel=2,
        )
        return int(self)

    field.__name__ = name
    field.__qualname__ = f"{typ.__name__}.{name}"
    field.__doc__ = f"Deprecated: the value is the {typ.__name__} itself."

    setattr(typ, name, property(field))


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
