==================================================
:mod:`winrt.windows.ui.composition.interop` module
==================================================

.. module:: winrt.windows.ui.composition.interop

    APIs for desktop interop with the `Windows.UI.Composition
    <https://learn.microsoft.com/uwp/api/windows.ui.composition>`_ namespace.

    .. function:: create_desktop_window_target(compositor, hwnd_target, *, is_topmost=False)

        Creates a visual layer on a window to host Composition content.

        :param winrt.windows.ui.composition.Compositor compositor: The Compositor
            object that is used to create the ``DesktopWindowTarget``.

        :param int hwnd_target: The window handle (HWND) that the visual layer will
            be attached to.

        :param bool is_topmost:

        :return: The created ``DesktopWindowTarget``.
        :rtype: :class:`winrt.windows.ui.composition.DesktopWindowTarget`

        :raises: :exc:`OSError` on failure

        .. versionadded:: 2.2
