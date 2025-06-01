===========================
:mod:`winrt.system` module
===========================

.. currentmodule:: winrt.system

------------
Type aliases
------------

.. data:: Buffer

    Alias of :class:`collections.abc.Buffer`. This is the projected type for
    `Windows.Storage.Streams.IBuffer`_.

    .. versionadded:: unreleased

    .. caution:: The WinRT type system does not distinguish between
        read-only and writeable buffers. Do not use immutable types like
        :class:`bytes` when the WinRT API expects a writeable buffer!

    .. seealso:: :ref:`buffer-projection`

.. _Windows.Storage.Streams.IBuffer: https://learn.microsoft.com/en-us/uwp/api/windows.storage.streams.ibuffer

.. data:: ReadableBuffer

    Alias of :class:`collections.abc.Buffer` that indicates the buffer will only
    be read from. This is used for WinRT ``PassArray`` parameters. The buffer
    format must also match the WinRT array type.

    .. versionadded:: 2.3

.. data:: WriteableBuffer

    Alias of :class:`collections.abc.Buffer` that indicates the buffer will be
    written to. This is used for WinRT ``FillArray`` parameters. The buffer
    format must also match the WinRT array type.

    .. versionadded:: 2.3

-----------------
Fundamental types
-----------------

.. class:: Object

    A wrapper around the WinRT ``System.Object`` type.

    This is the base type of all WinRT runtime objects and cannot be
    instantiated directly.

    .. rubric:: Type casting

    Sometimes WinRT wrapper objects may be returned as a base type or interface
    type that needs to be cast to a different type. In these cases, use the
    ``as_`` method to cast the object to the desired type.

    .. method:: as_(type):

        Casts the object to the specified type.

        :param type: The type to cast the object to.
        :type type: type
        :return: The object cast to the specified type.

        .. versionadded:: 3.0

    .. rubric:: Introspection attributes

    WinRT objects have the following attributes for inspecting various metatdata
    at runtime:

    .. attribute:: _runtime_class_name_
        :type: str

        Gets the WinRT runtime class name of the object.

        .. versionadded:: 2.1

    .. attribute:: _iids_
        :type: Array[uuid.UUID]

        Gets the Interface Identifiers (IIDS) of the WinRT interfaces
        implemented by the object.

        .. versionadded:: 2.1

.. class:: Array(type, [initializer, ] /)

    A wrapper around the WinRT ``System.Array`` type.

    This type implements the Python sequence protocol.

    :param type:
        The type to use for elements of the array. This can be a WinRT
        type or a format string for fundamental types.
    :type type: str or type
    :param initializer:
        An optional iterator of values to use to initialize the array.
        If an integer value is given, an empty array of that size will
        be initialized. For value types, any object supporting the
        CPython buffer protocol with the correct layout can be used
        as an initializer.
    :type initializer: int or iter or buffer


    Creation examples::

        from winrt.system import Array
        from winrt.windows.foundation import Point

        # array of 10 32-bit unsigned integers.
        a1 = Array("I", 10)
        # array of 3 points with initial values
        a2 = Array(Point, [Point(1, 1), Point(2, 2), Point(3, 3)])

    Sequence protocol examples::

        # get the number of elements in the array
        size = len(a1)
        # get the first element of the array
        item = a1[0]
        # get the last element of the array
        item = a1[-1]
        # iterate all items of the array
        for item in a1: ...
        # test for element in array
        if item in a1: ...

------
Boxing
------

Some APIs require a :class:`winrt.system.Object` to be passed as a parameter.
In order to pass other types like strings and numbers, they must be boxed.
Likewise, when a method returns a :class:`winrt.system.Object`, it may need to
be unboxed to get the original value.

.. function:: box_boolean(value: bool) -> Object
    box_int8(value: str) -> Object
    box_uint8(value: str) -> Object
    box_int16(value: int) -> Object
    box_uint16(value: int) -> Object
    box_int32(value: int) -> Object
    box_uint32(value: int) -> Object
    box_int64(value: int) -> Object
    box_uint64(value: int) -> Object
    box_single(value: float) -> Object
    box_double(value: float) -> Object
    box_char16(value: str) -> Object
    box_string(value: str) -> Object
    box_guid(value: uuid.UUID) -> Object
    box_date_time(value: datetime.datetime) -> Object
    box_time_span(value: datetime.timedelta) -> Object

    Boxes the given value into a :class:`winrt.system.Object`.

    Essentially, this is shorthand for calling::

        from winrt.windows.foundation import PropertyValue

        obj = PropertyValue.create_xyz(value)

    .. versionadded:: 3.0

.. function:: unbox_boolean(value: Object) -> bool
    unbox_int8(value: Object) -> str
    unbox_uint8(value: Object) -> str
    unbox_int16(value: Object) -> int
    unbox_uint16(value: Object) -> int
    unbox_int32(value: Object) -> int
    unbox_uint32(value: Object) -> int
    unbox_int64(value: Object) -> int
    unbox_uint64(value: Object) -> int
    unbox_single(value: Object) -> float
    unbox_double(value: Object) -> float
    unbox_char16(value: Object) -> str
    unbox_string(value: Object) -> str
    unbox_guid(value: Object) -> uuid.UUID
    unbox_date_time(value: Object) -> datetime.datetime
    unbox_time_span(value: Object) -> datetime.timedelta

    Unboxes the given :class:`winrt.system.Object` into the original value.

    Essentially, this is shorthand for calling::

        from winrt.windows.foundation import IPropertyValue

        value = obj.as_(IPropertyValue).get_xyz()

    .. versionadded:: 3.0
