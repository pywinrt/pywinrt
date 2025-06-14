# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum
import typing
import uuid as _uuid

import winrt.runtime._internals
import winrt.system
from winrt._winrt_windows_foundation import (
    EventRegistrationToken,
    HResult,
    Point,
    Rect,
    Size,
    Deferral,
    GuidHelper,
    MemoryBuffer,
    PropertyValue,
    Uri,
    WwwFormUrlDecoder,
    WwwFormUrlDecoderEntry,
    _IAsyncAction,
    IAsyncAction,
    _IAsyncActionWithProgress,
    IAsyncActionWithProgress,
    _IAsyncInfo,
    IAsyncInfo,
    _IAsyncOperationWithProgress,
    IAsyncOperationWithProgress,
    _IAsyncOperation,
    IAsyncOperation,
    _IClosable,
    IClosable,
    _IGetActivationFactory,
    IGetActivationFactory,
    _IMemoryBuffer,
    IMemoryBuffer,
    _IMemoryBufferReference,
    IMemoryBufferReference,
    _IPropertyValue,
    IPropertyValue,
    _IReferenceArray,
    IReferenceArray,
    _IReference,
    IReference,
    _IStringable,
    IStringable,
    _IWwwFormUrlDecoderEntry,
    IWwwFormUrlDecoderEntry,
)

__all__ = [
    "AsyncStatus",
    "PropertyType",
    "EventRegistrationToken",
    "HResult",
    "Point",
    "Rect",
    "Size",
    "Deferral",
    "GuidHelper",
    "MemoryBuffer",
    "PropertyValue",
    "Uri",
    "WwwFormUrlDecoder",
    "WwwFormUrlDecoderEntry",
    "IAsyncAction",
    "IAsyncActionWithProgress",
    "IAsyncInfo",
    "IAsyncOperationWithProgress",
    "IAsyncOperation",
    "IClosable",
    "IGetActivationFactory",
    "IMemoryBuffer",
    "IMemoryBufferReference",
    "IPropertyValue",
    "IReferenceArray",
    "IReference",
    "IStringable",
    "IWwwFormUrlDecoderEntry",
    "AsyncActionCompletedHandler",
    "AsyncActionProgressHandler",
    "AsyncActionWithProgressCompletedHandler",
    "AsyncOperationCompletedHandler",
    "AsyncOperationProgressHandler",
    "AsyncOperationWithProgressCompletedHandler",
    "DeferralCompletedHandler",
    "EventHandler",
    "TypedEventHandler",
]
T = typing.TypeVar('T')
TProgress = typing.TypeVar('TProgress')
TResult = typing.TypeVar('TResult')
TSender = typing.TypeVar('TSender')

class AsyncStatus(enum.IntEnum):
    CANCELED = 2
    COMPLETED = 1
    ERROR = 3
    STARTED = 0

class PropertyType(enum.IntEnum):
    EMPTY = 0
    UINT8 = 1
    INT16 = 2
    UINT16 = 3
    INT32 = 4
    UINT32 = 5
    INT64 = 6
    UINT64 = 7
    SINGLE = 8
    DOUBLE = 9
    CHAR16 = 10
    BOOLEAN = 11
    STRING = 12
    INSPECTABLE = 13
    DATE_TIME = 14
    TIME_SPAN = 15
    GUID = 16
    POINT = 17
    SIZE = 18
    RECT = 19
    OTHER_TYPE = 20
    UINT8_ARRAY = 1025
    INT16_ARRAY = 1026
    UINT16_ARRAY = 1027
    INT32_ARRAY = 1028
    UINT32_ARRAY = 1029
    INT64_ARRAY = 1030
    UINT64_ARRAY = 1031
    SINGLE_ARRAY = 1032
    DOUBLE_ARRAY = 1033
    CHAR16_ARRAY = 1034
    BOOLEAN_ARRAY = 1035
    STRING_ARRAY = 1036
    INSPECTABLE_ARRAY = 1037
    DATE_TIME_ARRAY = 1038
    TIME_SPAN_ARRAY = 1039
    GUID_ARRAY = 1040
    POINT_ARRAY = 1041
    SIZE_ARRAY = 1042
    RECT_ARRAY = 1043
    OTHER_TYPE_ARRAY = 1044

winrt.runtime._internals.mixin_sequence(WwwFormUrlDecoder)
AsyncActionCompletedHandler = typing.Callable[[IAsyncAction, AsyncStatus], None]
AsyncActionProgressHandler = typing.Callable[[IAsyncActionWithProgress[TProgress], TProgress], None]
AsyncActionWithProgressCompletedHandler = typing.Callable[[IAsyncActionWithProgress[TProgress], AsyncStatus], None]
AsyncOperationCompletedHandler = typing.Callable[[IAsyncOperation[TResult], AsyncStatus], None]
AsyncOperationProgressHandler = typing.Callable[[IAsyncOperationWithProgress[TResult, TProgress], TProgress], None]
AsyncOperationWithProgressCompletedHandler = typing.Callable[[IAsyncOperationWithProgress[TResult, TProgress], AsyncStatus], None]
DeferralCompletedHandler = typing.Callable[[], None]
EventHandler = typing.Callable[[winrt.system.Object, T], None]
TypedEventHandler = typing.Callable[[TSender, TResult], None]
