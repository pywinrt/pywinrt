==========================================
:mod:`winrt.windows.system.interop` module
==========================================

APIs for desktop interop with the `Windows.System
<https://learn.microsoft.com/uwp/api/windows.system>`_ namespace.

.. currentmodule:: winrt.windows.system.interop

.. function:: create_dispatcher_queue_controller(thread_type=DispatcherQueueThreadType.CURRENT, apartment_type=DispatcherQueueThreadApartmentType.NONE)

    Creates a DispatcherQueueController.

    Use the created DispatcherQueueController to create and manage the lifetime
    of a DispatcherQueue to run queued tasks in priority order on the dispatcher
    queue's thread.

    If *thread_type* is :data:`DispatcherQueueThreadType.DEDICATED`, then this
    function creates a thread, initializes it with the specified COM apartment,
    and associates a DispatcherQueue with that thread. The dispatcher queue
    event loop runs on the new dedicated thread until the dispatcher queue is
    explicitly shut down. To avoid thread and memory leaks, call
    ``DispatcherQueueController.shutdown_queue_async`` when you are finished
    with the dispatcher queue.

    If *thread_type* is :data:`DispatcherQueueThreadType.CURRENT`, then a
    ``DispatcherQueue`` is created and associated with the current thread. An
    error results if there is already a ``DispatcherQueue`` associated with the
    current thread. The current thread must pump messages to allow the
    dispatcher queue to dispatch tasks. Before the current thread exits, it
    must call ``DispatcherQueueController.shutdown_queue_async``, and continue
    pumping messages until the ``IAsyncAction`` completes.

    This call does not return until the ``DispatcherQueueController`` and new
    thread (if any) are created.

    :param DispatcherQueueThreadType thread_type: Thread affinity for a new
        ``DispatcherQueueController``.

    :param DispatcherQueueThreadApartmentType apartment_type: Specifies whether
        to initialize the COM apartment on the new thread as an application
        single-threaded apartment (ASTA) or a single-threaded apartment
        (STA). This field is relevant only if *thread_type* is
        :data:`DispatcherQueueThreadType.DEDICATED`. Use
        :data:`DispatcherQueueThreadApartmentType.NONE` when *thread_type*
        is :data:`DispatcherQueueThreadType.CURRENT`.

    :return: The created dispatcher queue controller.
    :rtype: :obj:`winrt.windows.ui.composition.DispatcherQueueController`

    :raises: :exc:`OSError` on failure

    .. versionadded:: unreleased

    .. seealso:: https://learn.microsoft.com/en-us/windows/win32/api/dispatcherqueue/nf-dispatcherqueue-createdispatcherqueuecontroller

.. class:: DispatcherQueueThreadApartmentType

    Defines constants that specify options around type of COM apartment for a
    new ``DispatcherQueueController``.

    .. data:: NONE

        No COM threading apartment type specified.

    .. data:: ASTA

        Specifies an application single-threaded apartment (ASTA) COM threading apartment.

    .. data:: STA

        Specifies a single-threaded apartment (STA) COM threading apartment.

    .. versionadded:: unreleased

.. class:: DispatcherQueueThreadType

        Defines constants that specify options around thread affinity for a new
        ``DispatcherQueueController``.

        .. data:: CURRENT

            The current thread.

        .. data:: DEDICATED

            A new dedicated thread.

    .. versionadded:: unreleased
