===========================
:mod:`winrt.system` module
===========================

.. currentmodule:: winrt.system

.. class:: Object

    A wrapper around the WinRT ``System.Object`` type.

    This is the base type of all WinRT runtime objects and cannot be
    instantiated directly.

    .. rubric:: Introspection attributes

    WinRT objects have the following attributes for inspecting various metatdata
    at runtime:

    .. attribute:: _runtime_class_name_
        :type: str

        Gets the WinRT runtime class name of the object.

        .. versionadded:: unreleased

    .. attribute:: _iids_
        :type: Array[uuid.UUID]

        Gets the Interface Identifiers (IIDS) of the WinRT interfaces
        implemented by the object.

        .. versionadded:: unreleased

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

