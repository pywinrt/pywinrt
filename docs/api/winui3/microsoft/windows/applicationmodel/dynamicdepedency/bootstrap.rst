===================================================================================
:mod:`winui3.microsoft.windows.applicationmodel.dynamicdependency.bootstrap` module
===================================================================================

APIs for bootstrapping the Windows App runtime.

.. versionchanged:: 3.0
    The top-level package name has been changed from ``winrt`` to ``winui3``.

.. currentmodule:: winui3.microsoft.windows.applicationmodel.dynamicdependency.bootstrap

.. function:: initialize(version: str = RELEASE_VERSION, min_version: str = RUNTIME_VERSION, options: InitializeOptions = InitializeOptions.NONE) -> Shutdown

    Initialize the Windows App SDK runtime.

    :param version:
        The Windows App SDK *product* release version to load, e.g. “1.5” or “1.5-preview”.

    :param min_version:
        The minimum version of the Windows App SDK *runtime* to load, e.g. “5001.70.1338.0”.

    :param options:
        Initialization option flags.

    :returns:
        A context manager that will shutdown the Windows App SDK runtime when exited.

    :raises OSError:
        if the initialization fails.

    .. versionadded:: 2.1

    Example::

        with initialize(options=InitializeOptions.ON_NO_MATCH_SHOW_UI):
            # main code here
            ...

.. attribute:: RELEASE_VERSION
    :type: str

    The package release version of the Windows App SDK used at compile time.

    .. versionadded:: 2.1

.. attribute:: RUNTIME_VERSION
    :type: str

    The runtime version of the Windows App SDK used at compile time.

    .. versionadded:: 2.1

.. class:: InitializeOptions

    :bases: :class:`enum.IntFlag`

    Initialization option flags for :func:`initialize`.

    .. attribute:: NONE

        Default behavior

    .. attribute:: ON_ERROR_DEBUG_BREAK

        If not successful call DebugBreak()

    .. attribute:: ON_ERROR_DEBUG_BREAK_IF_DEBUGGER_ATTACHED

        If not successful call DebugBreak() if a debugger is attached to the process

    .. attribute:: ON_ERROR_FAIL_FAST

        If not successful perform a fail-fast

    .. attribute:: ON_NO_MATCH_SHOW_UI

        If a compatible Windows App Runtime framework package is not found show UI

    .. attribute:: ON_PACKAGE_IDENTITY_NOOP

        Do nothing (do not error) if the process has package identity

    .. versionadded:: 2.1
