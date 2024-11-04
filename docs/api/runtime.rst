===========================
:mod:`winrt.runtime` module
===========================

.. currentmodule:: winrt.runtime

----------
Activation
----------

Functions related to preparing the WinRT runtime for use.

.. function:: init_apartment(apartment_type: ApartmentType) -> None

    Initializes the WinRT runtime in the specified apartment type.

    Calling this function is only necessary if you need a single threaded, e.g.
    for UI thread, or if you need to re-initalize the thread with a different
    apartment type.

    :param apartment_type: The apartment type to initialize the runtime in.
    :type apartment_type: ApartmentType

    .. versionadded:: unreleased

.. function:: uninit_apartment() -> None

    Uninitializes the WinRT runtime.

    Not necessary on thread exit, but can be used uninitalize and re-initalize
    a thread if needed.

    .. versionadded:: unreleased


.. class:: ApartmentType

    Enumeration of COM apartment types.

    .. attribute:: SINGLE_THREADED

        Single-threaded apartment.

    .. attribute:: MULTI_THREADED

        Multi-threaded apartment.

    .. versionadded:: unreleased
