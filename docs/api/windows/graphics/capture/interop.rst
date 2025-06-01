=====================================================
:mod:`winrt.windows.graphics.capture.interop` module
=====================================================

.. module:: winrt.windows.graphics.capture.interop

    APIs for desktop interop with the `Windows.Graphics.Capture
    <https://learn.microsoft.com/uwp/api/windows.graphics.capture>`_ namespace.


    .. function:: create_for_monitor(monitor)

        Targets a monitor(s) for the creation of a graphics capture item.

        :param int monitor: The monitor handle (``HMONITOR``) that represents the monitor to capture.

        :return: A graphics capture item.
        :rtype: :obj:`winrt.windows.graphics.capture.GraphicsCaptureItem`

        .. seealso:: https://learn.microsoft.com/windows/win32/api/windows.graphics.capture.interop/nf-windows-graphics-capture-interop-igraphicscaptureiteminterop-createformonitor


    .. function:: create_for_window(window)

        Targets a single window for the creation of a graphics capture item.

        :param int window: The window handle (``HWND``) that represents the window to capture.

        :return: A graphics capture item.
        :rtype: :obj:`winrt.windows.graphics.capture.GraphicsCaptureItem`

        .. seealso:: https://learn.microsoft.com/windows/win32/api/windows.graphics.capture.interop/nf-windows-graphics-capture-interop-igraphicscaptureiteminterop-createforwindow
