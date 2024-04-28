==============================================
:mod:`winrt.windows.foundation.interop` module
==============================================

APIs for interop with the `Windows.Foundation
<https://learn.microsoft.com/uwp/api/windows.foundation>`_ namespace.

.. currentmodule:: winrt.windows.foundation.interop

.. function:: box(value, [ptype])

    Boxes a Python value into a ``System.Object``.

    This function is analogous to ``winrt::box_value`` in C++/WinRT.

    :param value:
        The Python value to box.
    :type value:
        None | bool | int | float | str | uuid.UUID | datetime.datetime |
        datetime.timedelta | winrt.system.Object | winrt.system.Array |
        winrt.windows.foundation.Point | winrt.windows.foundation.Size |
        winrt.windows.foundation.Rect
    :param ptype:
        The property type to box the value as. If not provided, the type
        will be inferred from the value. If the value is an ``int``, ``float``,
        or ``System.Array`` the type must be provided.
    :type ptype: winrt.windows.foundation.PropertyType

    :returns: a ``System.Object`` representing the boxed value
    :rtype: winrt.system.Object

    :raises TypeError:
        if *value* is not a supported type or if *ptype* is not a
        :class:`winrt.windows.foundation.PropertyType` or if *ptype* is
        omitted for types that require it.
    :raises ValueError:
        if *ptype* is not a supported type.

    .. versionadded:: 2.0.0

.. function:: unbox(obj)

    Unboxes a ``System.Object`` into a Python value.

    This function is analogous to ``winrt::unbox_value`` in C++/WinRT.

    :param obj:
        The ``System.Object`` to unbox.
    :type obj: winrt.system.Object | None 

    :returns: the unboxed Python value
    :rtype:
        None | bool | int | float | str | uuid.UUID | datetime.datetime |
        datetime.timedelta | winrt.system.Object | winrt.system.Array |
        winrt.windows.foundation.Point | winrt.windows.foundation.Size |
        winrt.windows.foundation.Rect

    :raises TypeError:
        if *obj* is not a :class:winrt.system.Object`

    .. versionadded:: 2.0.0
