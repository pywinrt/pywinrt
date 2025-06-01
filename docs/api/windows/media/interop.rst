=========================================
:mod:`winrt.windows.media.interop` module
=========================================

.. module:: winrt.windows.media.interop

    APIs for desktop interop with the `Windows.Media
    <https://learn.microsoft.com/uwp/api/windows.media>`_ namespace.


    .. function:: get_for_window(hwnd)

        Targets a single window for the creation of a graphics capture item.

        :param int hwnd: The top-level app window (``HWND``) for which the ``ISystemMediaTransportControls`` interface is retrieved.

        :return: Receives the ``ISystemMediaTransportControls`` that corresponds to the appWindow window.
        :rtype: :obj:`winrt.windows.media.SystemMediaTransportControls`

        .. versionadded:: 3.1

        .. seealso:: https://learn.microsoft.com/en-us/windows/win32/api/systemmediatransportcontrolsinterop/nf-systemmediatransportcontrolsinterop-isystemmediatransportcontrolsinterop-getforwindow
