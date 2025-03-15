===================================
:mod:`winrt.runtime.interop` module
===================================

.. currentmodule:: winrt.runtime.interop

Various functions for interoperating with Win32 and COM.

.. function:: def initialize_with_window(obj: Object, hwnd: int) -> None: ...

    Provide an owner window to a Windows Runtime (WinRT) object used in a desktop application.

    :param obj: A WinRT object that implements `IInitializeWithWindow` interface.
    :type obj: Object
    :param hwnd: The handle of the window to be used as the owner window.
    :type hwnd: int

    .. versionadded:: 3.0

    .. seealso:: `Microsoft docs <https://learn.microsoft.com/en-us/windows/win32/api/shobjidl_core/nf-shobjidl_core-iinitializewithwindow-initialize>`_
