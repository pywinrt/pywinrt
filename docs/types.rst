=====================
The WinRT type system
=====================

PyWinRT is a "projection" that uses the `Windows Runtime (WinRT) type system`_
to automatically generate Python bindings for the Windows SDK. This page describes
how the WinRT types look and feel in this Python projection.

.. _Windows Runtime (WinRT) type system: https://learn.microsoft.com/en-us/uwp/winrt-cref/winrt-type-system

.. _naming:

------------------
Naming conventions
------------------

In the WinRT documentation, most names use ``PascalCasing`` in the style of .NET.
In the Python projection, names are adapted to fit `PEP8 naming conventions`_.

- Type names remain as ``CapitalizedWords``.
- Constants, such as enum members are converted to ``UPPER_CASE_WITH_UNDERSCORES``.
- Namespaces are converted to ``lowercasewithoutunderscores``.
- All other identifiers are converted to ``lower_case_with_underscores``
  (fields, properties, methods, events, etc.).
- If an identifier is a Python keyword, an underscore is appended to the name.

.. _PEP8 naming conventions: https://peps.python.org/pep-0008/#naming-conventions


---------------------
Distribution packages
---------------------

Each WinRT namespace is projected as a separate Python package. The packages are
named like `winrt-Windows.Foundation <https://pypi.org/project/winrt-Windows.Foundation/>`_
where ``winrt`` is the root namespace for a specific SDK or DLL and
``Windows.Foundation`` is a WinRT namespace within that SDK or DLL.

There are also some PyWinRT-specific sub-packages distributed in the
`winrt-runtime <https://pypi.org/project/winrt-runtime/>`_ package. As well as
some extra interop packages that bridge between WinRT and Win32 types.

.. seealso:: :doc:`api/index`

----------
Namespaces
----------

Each Windows SDK namespace is projected as a Python package and follows the usual
Python import conventions::

    # import members of a namespace
    from winrt.windows.foundation import Uri
    # or import the namespace module itself
    import winrt.windows.foundation as wf


As with the distribution package names, the WinRT namespace names have a root
namespace that matches the part of the package name before the ``-`` followed
by the namespace. The modules names are ``lowercasewithoutunderscores``, so
namespaces with multiple words in one segment, like ``Windows.AI.MachineLearning``
are projected as ``winrt.windows.ai.machinelearning``.

-----------------
Fundamental types
-----------------

The WinRT type system defines the following fundamental types which are mapped
to the corresponding Python builtin types.

=========== ================== ===================== ===========
WinRT type  Python type        Python format string  Description
=========== ================== ===================== ===========
``Boolean`` :class:`bool`      ``"?"``               an 8-bit Boolean value
``Int8``    :class:`int`       ``"b"``               an 8-bit signed integer
``Int16``   :class:`int`       ``"h"``               a 16-bit signed integer
``Int32``   :class:`int`       ``"i"``               a 32-bit signed integer
``Int64``   :class:`int`       ``"q"``               a 64-bit signed integer
``UInt8``   :class:`int`       ``"B"``               an 8-bit unsigned integer
``UInt16``  :class:`int`       ``"H"``               a 16-bit unsigned integer
``UInt32``  :class:`int`       ``"I"``               a 32-bit unsigned integer
``UInt64``  :class:`int`       ``"Q"``               a 64-bit unsigned integer
``Single``  :class:`float`     ``"f"``               a 32-bit IEEE 754 floating point number
``Double``  :class:`float`     ``"d"``               a 64-bit IEEE 754 floating point number
``Char16``  :class:`str` [#s]_ ``"u"`` [#u]_         a 16-bit non-numeric value representing a UTF-16 code unit
``String``  :class:`str`       ``"P"``               an immutable sequence of Char16 used to represent text
``Guid``    :class:`uuid.UUID` ``"T{I2H8B}"`` [#g]_  a 128-bit standard globally unique identifier
=========== ================== ===================== ===========

The Python format strings use the syntax `defined in the struct module`_ and
the `PEP 3118 additions`_ and are used as the format for the buffer protocol for
`arrays`_ of these types. These format strings can be read at runtime by using
:attr:`memoryview.format`.

.. _defined in the struct module: https://docs.python.org/3/library/struct.html#format-characters
.. _PEP 3118 additions: https://peps.python.org/pep-3118/#additions-to-the-struct-string-syntax

.. [#s] Strings that are converted to ``Char16`` can only contain one character,
    similar to how :func:`ord` works.
.. [#u] ``"u"`` is deprecated in the :mod:`array` module and is not
    compatible with the :mod:`struct` module. Use ``"H"`` instead if needed.
.. [#g] Use ``"I2H8B"`` with the :mod:`struct` module since it does not support
    the PEP 3118 ``T{}`` syntax.

.. _Windows GUID Structure: https://learn.microsoft.com/en-us/windows/win32/api/guiddef/ns-guiddef-guid

-----
Enums
-----

WinRT enums are projected using the Python standard library :mod:`enum` module. The
WinRT Type system has a ``[Flags]`` attribute that indicates if an enum is
treated as bit flags or not. Enum types without this attribute are projected as
an :class:`enum.IntEnum` type or if the ``[Flags]`` attribute is present, the type is
projected as an :class:`enum.IntFlag` type.

For arrays and buffers containing enums, use the ``"i"`` format string for
regular enums and ``"I"`` for flags.


-------
Structs
-------

Structs are simple data types that are passed by value in WinRT. In the Python
projection, each struct is wrapped in a Python object that is similar to a
:class:`typing.NamedTuple` or frozen :mod:`dataclasses`. The names of fields are
converted to use the ``lower_case_with_underscores`` :ref:`naming convention <naming>`.

Projected structs are :func:`@typing.final <typing.final>` classes, so they
cannot be subclassed. Projected structs are immutable. To modify a struct, use
:func:`copy.replace` which creates a modified copy.

As a convenience, plain :class:`tuple` objects can be used in place of projected
structs when calling methods that expect a projected struct or when setting
properties that expect a projected struct.

Each projected struct with more than one field also has a ``unpack()`` method
the converts the projected struct to a tuple.

Example::

    # window.position is a winrt.windows.graphics.PointInt32

    # fails with AttributeError because Point is immutable
    window.position.x = 200

    # modifying
    new = copy.replace(window.position, x=200)

    # passing a tuple
    window.move_and_resize((100, 100, 800, 600))

    # unpacking
    x, y = window.position.unpack()

.. versionchanged:: 2.2

    Changed ``__repr__`` implementation to give a representation that can be
    passed to ``eval()``.

.. versionchanged:: 3.0

    * Structs are now immutable. In previous versions, attributes could be set.
    * Added ``__replace__`` method to allow for use with :func:`copy.replace`.
    * Added ``unpack()`` method to convert to a tuple. Added support for using
      plain tuples in place of projected structs as method arguments.

-------
Objects
-------

.. todo:: explain objects aka runtime classes

Methods
=======

.. todo:: explain method naming and overload rules

    https://github.com/pywinrt/pywinrt/blob/main/projection/readme.md#method-overloading

Properties
==========

Properties are projected as Python :term:`descriptor`-like attributes. Properties
with WinRT getter support getting and properties with a WinRT setter allow setting.
Deleting properties is never allowed.

Names of properties are converted to use the ``lower_case_with_underscores``
:ref:`naming convention <naming>`.

Example::

    from winrt.windows.foundation import Uri

    uri = Uri("https://example.com")
    print(uri.scheme_name)

Static properties are implemented as class attributes via a metaclass, so are
accessed by using the type object rather than an instance object::

    from winrt.windows.foundation import GuidHelper

    empty_uuid = GuidHelper.empty

.. versionchanged:: v1.0.0b8

    Previous beta releases implemented static properties as ``get_name()`` and
    ``put_name()`` static methods instead of class attributes.

Events
======

.. todo:: explain events

    https://github.com/pywinrt/pywinrt/blob/main/projection/readme.md#event-handlers

----------
Interfaces
----------

.. todo:: explain interfaces

---------
Delegates
---------

.. todo:: explain delegates
.. todo:: explain exceptions in callbacks

------
Arrays
------

.. todo:: document array types

.. _exceptions-projection:

----------
Exceptions
----------

PyWinRT uses the CppWinRT projection under the hood. Any C++ exception that is
not handled gets propagated to Python as an :class:`OSError` exception with
:attr:`OSError.winerror` set to an `HRESULT`_ error code.

.. seealso:: :mod:`winrt.system.hresult` for some common error codes.

On the other hand, Python exceptions cannot be propagated to C++ code. WinRT
requires that errors are serializable, but Python exceptions are not. So if a
Python callback from C++ code (i.e. an event handler or other delegate or a
method of a Python subclass of a WinRT interface) raises an exception that isn't
handled before the method returns, it will trigger the :func:`sys.unraisablehook`
handler in Python and cause the C++ code to receive a an HRESULT error code
of :attr:`~winrt.system.hresult.PYWINRT_E_UNRAISABLE_PYTHON_EXCEPTION`.
This can cause undefined behavior in the C++ code in some cases, so it should
be avoided.

.. _HRESULT: https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-erref/0642cb2f-2075-4469-918c-4441e69c548a


-----------------
Specialized types
-----------------

Some types have special handling and don't strictly follow the patterns described
above.

Awaitables
==========

WinRT has many ``_async`` methods that that perform background operations and
return a type that derives from ``Windows.Foundation.IAsyncInfo`` to be able to
wait for the result. There are four fundamental types that derive from this:

- `IAsyncAction <https://learn.microsoft.com/en-us/uwp/api/windows.foundation.iasyncaction>`_
  \- represents an operation that does not return a value
- `IAsyncOperation<TResult> <https://learn.microsoft.com/en-us/uwp/api/windows.foundation.iasyncoperation-1>`_
  \- represents an operation that returns a value of type ``TResult``
- `IAsyncOperationWithProgress<TResult, TProgress> <https://learn.microsoft.com/en-us/uwp/api/windows.foundation.iasyncactionwithprogress-1>`_
  \- represents an operation that returns a value of type ``TResult`` and reports
  progress of type ``TProgress``
- `IAsyncActionWithProgress<U> <https://learn.microsoft.com/en-us/uwp/api/windows.foundation.iasyncoperationwithprogress-2>`_
  \- represents an operation that does not return a value and reports progress
  of type ``TProgress``

Synchronous usage
-----------------

PyWinRT exposes the CppWinRT extension methods for calling these methods
synchronously (i.e. when not using ``asyncio``).

.. method:: IAsyncAction.get(self) -> None
    IAsyncOperation.get(self) -> TResult
    IAsyncActionWithProgress.get(self) -> None
    IAsyncOperationWithProgress.get(self) -> TResult

    These methods block until the operation is complete.

    Returns:
        The result of the operation.

    Raises:
        :class:`OSError` if the operation failed or was canceled.
        :class:`RuntimeError` if called from a single-threaded apartment (i.e.
        a GUI thread).

    .. warning:: This method can't be interrupted by :kbd:`CTRL+C` which means
        that a :class:`KeyboardInterrupt` will not be raised until the operation
        is complete.

    .. versionadded:: 3.2

.. method:: IAsyncAction.wait(self, timeout: float) -> AsyncStatus
    IAsyncOperation.wait(self, timeout: float) -> AsyncStatus
    IAsyncActionWithProgress.wait(self, timeout: float) -> AsyncStatus
    IAsyncOperationWithProgress.wait(self, timeout: float) -> AsyncStatus

    These methods block until the operation is complete or the timeout is reached,
    whichever comes first.

    Args:
        timeout: The timeout in seconds.

    Returns:
        The status of the operation. In case of a timeout, the status will be
        ``AsyncStatus.STARTED``.

    Raises:
        :class:`RuntimeError` if called from a single-threaded apartment (i.e.
        a GUI thread).

    .. warning:: This method can't be interrupted by :kbd:`CTRL+C`, which means
        a :class:`KeyboardInterrupt` will not be raised until the operation is
        complete or the timeout is reached.

    .. versionadded:: 3.2

.. _async-projection:

Asynchronous usage
------------------

.. note:: WinRT async methods look like Python coroutines (methods defined with
    ``async def``) but they are not. This means they do not return a
    :class:`~collections.abc.Coroutine` object and therefore cannot be
    used with methods like :func:`asyncio.create_task`. They are only
    :class:`~collections.abc.Awaitable` objects.

If you are using ``asyncio``, then you can use the ``await`` keyword to wait
for the result of async WinRT methods::

    thing = await winrt_obj.get_thing_async()

.. versionchanged:: 3.2

    If the :class:`~collections.abc.Awaitable` that wraps the operation is
    canceled, it will now propagate the cancellation to the WinRT async
    action/operation. To restore the previous behavior, you can wrap the
    operation in :func:`asyncio.shield`.

Other usage
-----------

If you can't use either the synchronous helpers or ``asyncio`` (i.e. a GUI app
that doesn't use asyncio), then you can use the ``completed``  property to set
a callback that will be called when the operation is complete. In GUI apps where
the async method is called from the main thread (that is initialized as a single-
threaded apartment), the callback will occur on the main thread. Otherwise, the
callback will be called on a different thread.

You must also be careful about not creating a reference cycle to the operation,
otherwise it will cause a memory leak. This can happen if the callback is a
closure and references an object that references the operation itself.

.. todo:: add tips on how to iterate over progress events

.. _buffer-projection:

Buffers
=======

`Windows.Storage.Streams.IBuffer`_ is projected as :class:`winrt.system.Buffer`,
which is an alias for :class:`collections.abc.Buffer`. When used as a method
parameter, any Python object that implements the buffer protocol can be used,
for example, a :class:`bytearray`, :class:`bytes`, or a :class:`memoryview`.
Although care must be taken to ensure that immutable types like :class:`bytes`
are not used when the WinRT API expects a writeable buffer! The WinRT type
system does not distinguish between read-only and writeable buffers, so there
isn't a way to enforce this at runtime.

Buffers received as a return value can likewise be used with anything that
supports the buffer protocol. For example, :class:`memoryview` can be used to
access the buffer memory directly. Or, the struct module can be used to unpack
formatted binary data.

Using native Python classes to access the memory is significantly more efficient
than using the WinRT `Windows.Storage.Streams.IDataReader`_ or
`Windows.Storage.Streams.IDataWriter`_ classes.

`Windows.Foundation.IMemoryBuffer`_ may also be accessed using the Python buffer
protocol via the `Windows.Foundation.IMemoryBufferReference`. Care should be
taken since the underlying memory can be released.

.. _Windows.Storage.Streams.IBuffer: https://learn.microsoft.com/en-us/uwp/api/windows.storage.streams.ibuffer
.. _Windows.Foundation.IMemoryBuffer: https://learn.microsoft.com/en-us/uwp/api/windows.foundation.imemorybuffer
.. _Windows.Foundation.IMemoryBufferReference: https://learn.microsoft.com/en-us/uwp/api/windows.foundation.imemorybufferreference
.. _Windows.Storage.Streams.IDataReader: https://learn.microsoft.com/en-us/uwp/api/windows.storage.streams.idatareader
.. _Windows.Storage.Streams.IDataWriter: https://learn.microsoft.com/en-us/uwp/api/windows.storage.streams.idatawriter

Collections
===========

Most of the generic types in `Windows.Foundation.Collections`_ are projected
as `collections.abc`_ types. The WinRT runtime types are still present behind
the scenes, bute the type hints use only the Python standard library types to
encourage users to use the Pythonic APIs. Furthermore, types that inherit from
these interfaces are also extended to support the Pythonic APIs.

.. _Windows.Foundation.Collections: https://learn.microsoft.com/en-us/uwp/api/windows.foundation.collections
.. _collections.abc: https://docs.python.org/3/library/collections.abc.html

Sequences
---------

`IVector<T>`_ is projected as `MutableSequence[T]`_ and `IVectorView<T>`_ is
projected as `Sequence[T]`_. These types behave very much like Python lists, so
you can iterate them with a ``for`` loop, index them ``seq[0]``, slice them
``seq[:3]`` use them with ``len(seq)`` and search with ``value in seq``.

For mutable sequences, items can be modified with ``seq[0] = value``, deleted
with ``del seq[0]``, appended with ``seq.append(value)`` or ``seq.extend(seq2)``,
inserted with ``seq.insert(0, value)``, removed with ``seq.remove(value)``, and
cleared with ``seq.clear()``. You can also reverse the sequence in place with
``seq.reverse()``, pop an item at a specific index using ``seq.pop(index)``
(or the last item with ``seq.pop()``), or add items using the ``+=`` operator
like ``seq += [value1, value2]``.

.. _IVector<T>: https://learn.microsoft.com/en-us/uwp/api/windows.foundation.collections.ivector-1
.. _MutableSequence[T]: https://docs.python.org/3/library/collections.abc.html#collections.abc.MutableSequence
.. _IVectorView<T>: https://learn.microsoft.com/en-us/uwp/api/windows.foundation.collections.ivectorview-1
.. _Sequence[T]: https://docs.python.org/3/library/collections.abc.html#collections.abc.Sequence

Mappings
--------

`IMap<K, V>`_ is projected as `MutableMapping[K, V]`_ and `IMapView<K, V>`_ is
projected as `Mapping[K, V]`_. These types behave like Python dictionaries, so
you can access values with ``map[key]`` or ``map.get(key)``, check for keys with
``key in map``, and iterate over keys with a ``for`` loop. You can also retrieve
all keys, values, or key/value pairs using ``map.keys()``, ``map.values()``, and
``map.items()`` respectively. Equality operators (``==`` and ``!=``) can be used
to compare mappings for equality based on their key-value pairs. You can also
use ``len(map)`` to get the number of keys in the mapping.

.. note:: Python iterates over the keys only which might not be what you expect
    if you are used to using the same types in .NET. In Python, you will write::

        for key in map:
            print(key)

    To get both the keys and values, use the ``items()`` method::

        for key, value in map.items():
            print(key, value)

For mutable mappings, you can add or update items with ``map[key] = value``,
use ``value = map.setdefault(key, default)`` to insert a key with a default
value if it doesn't exist, delete items with ``del map[key]``, and clear all
items with ``map.clear()``. You can also remove and return a value while
removing the key using ``map.pop(key)`` or ``map.popitem()``. Multiple values
can be set at the same time using ``map.update(other)``.

There is also a special handling for ``IIterable<IKeyValuePair<K, V>>`` when
used as an argument to methods where a Python mapping is allowed. This means you
can pass a Python dictionary as the argument.

Example::

    data = NotificationData({"my_key": "my_value"})

.. _IMap<K, V>: https://learn.microsoft.com/en-us/uwp/api/windows.foundation.collections.imap-2
.. _MutableMapping[K, V]: https://docs.python.org/3/library/collections.abc.html#collections.abc.MutableMapping
.. _IMapView<K, V>: https://learn.microsoft.com/en-us/uwp/api/windows.foundation.collections.imapview-2
.. _Mapping[K, V]: https://docs.python.org/3/library/collections.abc.html#collections.abc.Mapping

Iterators
---------

`IIterable<T>`_ becomes `Iterable[T]`_. Any iterable Python type can be used
as an argument and return values can be used as any other iterable in Python.
Don't use the WinRT ``first()`` method to get an iterator, instead use the
builtin :func:`iter()` function or other Python features like ``for`` loops.

`IIterator<T>`_ is projected as `Iterator[T]`_ however these objects are rarely
used directly in Python. Instead, use ``for`` loops or generator expressions to
do the iterating for you. In rare cases, iterators might be used with :func:`next()`.
The WinRT methods on this object should be avoided.

.. _IIterable<T>: https://learn.microsoft.com/en-us/uwp/api/windows.foundation.collections.iiterable-1
.. _Iterable[T]: https://docs.python.org/3/library/collections.abc.html#collections.abc.Iterable
.. _IIterator<T>: https://learn.microsoft.com/en-us/uwp/api/windows.foundation.collections.iiterator-1
.. _Iterator[T]: https://docs.python.org/3/library/collections.abc.html#collections.abc.Iterator


Context managers
================

Any type that implements `IClosable <https://learn.microsoft.com/uwp/api/windows.foundation.iclosable>`_
can (and probably should) be used as a context manager in Python::

    from winrt.windows.foundation import MemoryBuffer

    with MemoryBuffer(256) as buf:
        ...

Generally, when an object it closable, it means that there are unmanaged
resources that may need to be released in a deterministic manner as opposed
waiting for the garbage collector to run to clean them up.

.. note:: .NET programmers may recognize this as similar to ``using`` statements
    in C# with ``IDisposable`` types.


Date and time
=============

There are a few foundational time-related types in WinRT that are projected
as the analogous Python types.

Windows.Foundation.DateTime
---------------------------

This type is converted to a :class:`datetime.datetime` object from the standard
Python library.

WinRT has a resolution of 100 nanoseconds while the Python type has a resolution
of 1 microsecond, so there is a small loss of precision in the conversion. Python
also has a much smaller allowed range of dates (years from 1 to 9999).

WinRT serializes values of this type as 100s of nanoseconds since since January 1, 1601 (UTC).
It uses a signed 64-bit integer for this, so the ``"q"`` format string is used in Python.

WinRT uses UTC for all values, so any ``datetime`` object returned from a Windows
API will use that timezone. It is recommended to use the UTC timezone when
creating ``datetime`` objects to pass to Windows APIs as well. "Naive" ``datetime``
objects (without a timezone) are assumed to use the local timezone and will
be converted to UTC.

Example::

    # set notification to expire 10 seconds from now
    toaster.expiration_time = datetime.now(timezone.utc) + timedelta(seconds=10)


Windows.Foundation.TimeSpan
---------------------------

This type is converted to a :class:`datetime.timedelta` object from the standard
Python library.

WinRT has a resolution of 100 nanoseconds while the Python type has a resolution
of 1 microsecond, so there is a small loss of precision in the conversion. Python
is also limited to +/-999999999 days.

WinRT serializes values of this type as 100s of nanoseconds.
It uses a signed 64-bit integer for this, so the ``"q"`` format string is used in Python.
