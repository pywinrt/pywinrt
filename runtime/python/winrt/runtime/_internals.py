import asyncio
from importlib.machinery import ModuleSpec
import os
from contextvars import Context, copy_context
from collections.abc import (
    Callable,
    Generator,
    Mapping,
    MutableMapping,
    MutableSequence,
    Sequence,
)
from pathlib import Path
import sys
from typing import Any, Generic, Self, TypeVar, Protocol, TYPE_CHECKING
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
    from winrt.windows.foundation import AsyncStatus


#: The file a projection package keeps its namespace's table in, beside the
#: ``__init__.py`` that loads it. It is part of the table format, which
#: ``runtime/src/table-format.md`` is the contract for; winrt-table-compiler
#: spells it for itself, as the two ends of the format spell the magic and the
#: record sizes for themselves.
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

    table = os.fspath(Path(spec.origin).parent / TABLE_NAME)

    try:
        _load_projection(sys.modules[spec.name], table)
    except OSError as error:
        # The table is opened and mapped, so a missing or unreadable one
        # arrives here as an OSError that says only what the operating system
        # said - no file name, no module. That is nearly always a package
        # whose module was copied without the data file beside it, which is
        # what a freezer does when nothing tells it about the table, so the
        # message has to name both and say what kind of thing is missing.
        raise ImportError(
            f"{spec.name} could not read its projection table, which a"
            " projection package carries beside the module that loads it."
            f" Reading {table} failed: {error}",
            name=spec.name,
            path=table,
        ) from error


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
    Protocol that matches the four async interfaces, whose objects the runtime
    makes asyncio futures with a :class:`FutureState` each.
    """

    @property
    def status(self) -> "AsyncStatus": ...
    def get_results(self) -> T: ...
    @property
    def completed(self) -> Callable[[Self, "AsyncStatus"], None]: ...
    @completed.setter
    def completed(self, value: Callable[[Self, "AsyncStatus"], None]) -> None: ...


class FutureState(Generic[T]):
    """
    What makes one async operation an asyncio future.

    The runtime keeps one of these beside each projected async operation and
    implements the future's methods by calling the method of the same name
    here, with the operation as the first argument. A projected object takes
    no part in garbage collection, so this must not keep the operation alive
    for longer than a wait: it holds the operation only from when the
    completed handler is set until the completion has been delivered to the
    event loop, which is while something is waiting for it.

    Completion is known only from the completed handler. The status of an
    operation can read ``COMPLETED`` before its handler has run, so the status
    is never what :meth:`done` answers from.

    Cancelling does not complete the future. It asks WinRT to cancel, and the
    future is done only once the operation has finished, so that a task
    awaiting it does not move on while the operation still runs.

    A result is read once and kept, but an error is not: ``get_results()`` is
    asked again each time, which raises a new exception. One that was kept
    would reach the operation through the frames of its traceback, and the
    operation would never be freed.
    """

    __slots__ = (
        "blocking",
        "_loop",
        "_op",
        "_callbacks",
        "_status",
        "_cancel_requested",
        "_cancel_message",
        "_results_read",
        "_result",
    )

    def __init__(self) -> None:
        #: The future's ``_asyncio_future_blocking``.
        self.blocking = False
        self._loop: asyncio.AbstractEventLoop | None = None
        self._op: AsyncOp[T] | None = None
        self._callbacks: list[tuple[Callable[[AsyncOp[T]], object], Context]] = []
        self._status: AsyncStatus | None = None
        self._cancel_requested = False
        self._cancel_message: Any = None
        self._results_read = False
        self._result: T | None = None

    def _attach(self, op: AsyncOp[T]) -> asyncio.AbstractEventLoop:
        """
        Binds the future to the running event loop and sets the completed
        handler of the operation, the first time anything needs either.
        """
        if self._loop is not None:
            return self._loop

        loop = asyncio.get_running_loop()

        # The handler holds the state rather than the operation, because WinRT
        # keeps the handler for as long as the operation exists, and one that
        # held the operation would keep it alive for good.
        def completed(_: AsyncOp[T], status: "AsyncStatus") -> None:
            try:
                loop.call_soon_threadsafe(self._deliver, status)
            except RuntimeError:
                # The loop is closed, so nothing will run the callbacks and
                # the operation must not be kept for them.
                self._op = None

        self._loop = loop
        self._op = op

        try:
            op.completed = completed
        except BaseException:
            self._loop = None
            self._op = None
            raise

        return loop

    def _deliver(self, status: "AsyncStatus") -> None:
        """
        Marks the future done, on its event loop, and schedules its callbacks.
        """
        assert self._loop is not None
        op = self._op
        assert op is not None
        callbacks = self._callbacks

        self._status = status
        self._op = None
        self._callbacks = []

        for fn, context in callbacks:
            self._loop.call_soon(fn, op, context=context)

    def _read_results(self, op: AsyncOp[T]) -> T:
        """
        What ``get_results()`` returned, which is asked for once, or what it
        raises, which it is asked for every time.
        """
        if not self._results_read:
            self._result = op.get_results()
            self._results_read = True

        return self._result  # type: ignore [return-value]

    def _make_cancelled_error(self, op: AsyncOp[T]) -> asyncio.CancelledError:
        """
        The error that a cancelled future raises, which ``asyncio.gather()``
        asks the future for.
        """
        if self._cancel_message is None:
            return asyncio.CancelledError()

        return asyncio.CancelledError(self._cancel_message)

    def get_loop(self, op: AsyncOp[T]) -> asyncio.AbstractEventLoop:
        return self._attach(op)

    def add_done_callback(
        self,
        op: AsyncOp[T],
        fn: Callable[[AsyncOp[T]], object],
        *,
        context: Context | None = None,
    ) -> None:
        loop = self._attach(op)

        if context is None:
            context = copy_context()

        if self._status is None:
            self._callbacks.append((fn, context))
        else:
            loop.call_soon(fn, op, context=context)

    def remove_done_callback(
        self, op: AsyncOp[T], fn: Callable[[AsyncOp[T]], object]
    ) -> int:
        kept = [(f, context) for f, context in self._callbacks if f != fn]
        removed = len(self._callbacks) - len(kept)
        self._callbacks = kept

        return removed

    def done(self, op: AsyncOp[T]) -> bool:
        self._attach(op)

        return self._status is not None

    def cancelled(self, op: AsyncOp[T]) -> bool:
        return self.done(op) and self._cancel_requested

    def result(self, op: AsyncOp[T]) -> T:
        if not self.done(op):
            raise asyncio.InvalidStateError("Result is not ready.")

        if self._cancel_requested:
            raise self._make_cancelled_error(op)

        return self._read_results(op)

    def exception(self, op: AsyncOp[T]) -> BaseException | None:
        if not self.done(op):
            raise asyncio.InvalidStateError("Exception is not set.")

        if self._cancel_requested:
            raise self._make_cancelled_error(op)

        try:
            self._read_results(op)
        except Exception as error:
            return error

        return None

    def cancel(self, op: AsyncOp[T], msg: Any | None = None) -> bool:
        """
        Says whether ``cancel(msg)`` is to ask WinRT to cancel the operation,
        and records that it has if so. The runtime makes the request.
        """
        if self._status is not None:
            return False

        if self._cancel_requested:
            return False

        # With no completed handler set, nothing here knows whether the
        # operation has finished, so WinRT is asked instead: a finished one
        # has nothing to cancel, and nothing would ever answer the request.
        if self._loop is None:
            from winrt.windows.foundation import AsyncStatus

            if op.status != AsyncStatus.STARTED:
                return False

        self._cancel_requested = True
        self._cancel_message = msg

        return True

    def iterate(self, op: AsyncOp[T]) -> Generator[Any, None, T]:
        """
        What ``__await__`` of the operation returns, which waits as
        ``asyncio.Future.__await__`` waits: by handing the operation itself to
        the task, which waits for its done callback.
        """
        if not self.done(op):
            self.blocking = True
            yield op

        if not self.done(op):
            raise RuntimeError("await wasn't used with future")

        return self.result(op)
