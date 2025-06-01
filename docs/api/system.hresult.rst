==================================
:mod:`winrt.system.hresult` module
==================================

.. module:: winrt.system.hresult

    WinRT methods will raise :class:`OSError` exceptions when they fail with
    :attr:`OSError.winerror` set to an `HRESULT`_ error code. This module
    contains some of the most common error codes used with WinRT types, but it is
    not exhaustive (there are 1000s of known error codes!).

    .. versionadded:: unreleased

    -------------------
    Windows error codes
    -------------------

    Most error codes are defined by Microsoft. Here are a few examples.

    .. note:: Not all available members of this module are documented here. See
        the Python source code for the complete list.

    .. attribute:: S_OK
        :type: HResult

        Operation successful.

    .. attribute:: E_FAIL
        :type: HResult

        Unspecified error.

    .. attribute:: E_INVALIDARG
        :type: HResult

        One or more arguments are invalid.

    .. attribute:: WIN32_ERROR_CANCELLED
        :type: HResult

        The operation was canceled by the user.

    -------------------
    PyWinRT error codes
    -------------------

    The following error codes are specific to PyWinRT.

    .. attribute:: PYWINRT_E_UNRAISABLE_PYTHON_EXCEPTION
        :type: HResult

        This error code is propagated when a Python exception is raised in a
        WinRT callback. It is not intended to be handled by the user. Instead
        the exception should be handled in the callback itself.

        .. seealso:: :ref:`exceptions-projection`

    .. _HRESULT: https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-erref/0642cb2f-2075-4469-918c-4441e69c548a
