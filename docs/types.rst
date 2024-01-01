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
- All other identifiers are converted to ``lower_case_with_underscores``
  (namespaces, fields, properties, methods, events, etc.).

.. _PEP8 naming conventions: https://peps.python.org/pep-0008/#naming-conventions


----------
Namespaces
----------

Each Windows SDK namespace is projected as a Python package and follows the usual
Python import conventions::

    # import members of a namespace
    from winrt.windows.foundation import Uri
    # or import the namespace module itself
    import winrt.windows.foundation as wf


-----------------
Fundamental types
-----------------

The WinRT type system defines the following fundamental types which are mapped
to the corresponding Python builtin types.

=========== ================== ===================== ===========
WinRT type  Python type        Python format string  Description
=========== ================== ===================== ===========
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
``Boolean`` :class:`bool`      ``"?"``               an 8-bit Boolean value
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
projected as an :class:`enum.IntFlags` type.

For arrays and buffers containing enums, use the ``"i"`` format string for
regular enums and ``"I"`` for flags.


-------
Structs
-------

Structs are simple data types that are passed by value in WinRT. In the Python
projection, each struct is wrapped in a Python object.

Fields
======

The fields of a structure are projected as Python :term:`descriptor`-like
attributes. Getting and setting are supported but deleting is not allowed. The
names of fields are converted to use the ``lower_case_with_underscores``
:ref:`naming convention <naming>`.

Example::

    from winrt.windows.foundation import Point

    point = Point(1, 2)
    x = point.x
    point.x = 3

-------
Objects
-------

.. todo:: explain objects aka runtime classes

Methods
=======

.. todo:: explain method naming and overload rules

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

-----------------
Specialized types
-----------------

Some types have special handling and don't strictly follow the patterns described
above.

Awaitables
==========

.. todo:: document IAsync*

Buffers
=======

.. todo:: document IBuffer and IMemoryBuffer

Collections
===========

.. todo:: document Iterable, Sequence, Map


Context managers
================

Any type that implements `IClosable <https://learn.microsoft.com/uwp/api/windows.foundation.iclosable>`_
can (and probably should) be used as a context manager in Python::

    from winrt.windows.foundation import MemoryBuffer

    with MemoryBuffer(256) as buf:
        ...

.. note:: .NET programmers may recognize this as similar to ``using`` statements
    in C# with ``IDisposable`` types.


Date and time
=============

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
creating ``datetime`` objects to pass to Windows APIs as well. Naive ``datetime``
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
