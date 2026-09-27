# `winrt.windows.system.interop` module

APIs for desktop interop with the
[Windows.System](https://learn.microsoft.com/uwp/api/windows.system) namespace.

## `create_dispatcher_queue_controller`

```python
create_dispatcher_queue_controller(thread_type=DispatcherQueueThreadType.CURRENT, apartment_type=DispatcherQueueThreadApartmentType.NONE)
```

Creates a DispatcherQueueController.

Use the created DispatcherQueueController to create and manage the lifetime
of a DispatcherQueue to run queued tasks in priority order on the dispatcher
queue's thread.

If *thread_type* is [`DispatcherQueueThreadType.DEDICATED`](#dedicated), then this
function creates a thread, initializes it with the specified COM apartment,
and associates a DispatcherQueue with that thread. The dispatcher queue
event loop runs on the new dedicated thread until the dispatcher queue is
explicitly shut down. To avoid thread and memory leaks, call
`DispatcherQueueController.shutdown_queue_async` when you are finished
with the dispatcher queue.

If *thread_type* is [`DispatcherQueueThreadType.CURRENT`](#current), then a
`DispatcherQueue` is created and associated with the current thread. An
error results if there is already a `DispatcherQueue` associated with the
current thread. The current thread must pump messages to allow the
dispatcher queue to dispatch tasks. Before the current thread exits, it
must call `DispatcherQueueController.shutdown_queue_async`, and continue
pumping messages until the `IAsyncAction` completes.

This call does not return until the `DispatcherQueueController` and new
thread (if any) are created.

| Parameter | Type | Description |
|---|---|---|
| `thread_type` | [`DispatcherQueueThreadType`](#dispatcherqueuethreadtype) | Thread affinity for a new `DispatcherQueueController`. |
| `apartment_type` | [`DispatcherQueueThreadApartmentType`](#dispatcherqueuethreadapartmenttype) | Specifies whether to initialize the COM apartment on the new thread as an application single-threaded apartment (ASTA) or a single-threaded apartment (STA). This field is relevant only if *thread_type* is [`DispatcherQueueThreadType.DEDICATED`](#dedicated). Use [`DispatcherQueueThreadApartmentType.NONE`](#none) when *thread_type* is [`DispatcherQueueThreadType.CURRENT`](#current). |

**Returns:** The created dispatcher queue controller.

**Return type:** `winrt.windows.ui.composition.DispatcherQueueController`

**Raises:** [`OSError`](https://docs.python.org/3/builtins/exceptions.html#OSError) on failure

!!! version-added "Added in version 2.2"

!!! seealso "See also"

    <https://learn.microsoft.com/en-us/windows/win32/api/dispatcherqueue/nf-dispatcherqueue-createdispatcherqueuecontroller>

## `DispatcherQueueThreadApartmentType`

```python
class DispatcherQueueThreadApartmentType
```

Defines constants that specify options around type of COM apartment for a
new `DispatcherQueueController`.

### `NONE`

No COM threading apartment type specified.

### `ASTA`

Specifies an application single-threaded apartment (ASTA) COM threading apartment.

### `STA`

Specifies a single-threaded apartment (STA) COM threading apartment.

!!! version-added "Added in version 2.2"

## `DispatcherQueueThreadType`

```python
class DispatcherQueueThreadType
```

Defines constants that specify options around thread affinity for a new
`DispatcherQueueController`.

### `CURRENT`

The current thread.

### `DEDICATED`

A new dedicated thread.

!!! version-added "Added in version 2.2"
