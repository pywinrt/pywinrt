# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import typing
import uuid as _uuid

import winrt.system
from winrt._winrt_windows_devices_perception_provider import (
    KnownPerceptionFrameKind,
    PerceptionControlGroup,
    PerceptionCorrelation,
    PerceptionCorrelationGroup,
    PerceptionFaceAuthenticationGroup,
    PerceptionFrame,
    PerceptionFrameProviderInfo,
    PerceptionFrameProviderManagerService,
    PerceptionPropertyChangeRequest,
    PerceptionVideoFrameAllocator,
    _IPerceptionFrameProvider,
    IPerceptionFrameProvider,
    _IPerceptionFrameProviderManager,
    IPerceptionFrameProviderManager,
)

__all__ = [
    "KnownPerceptionFrameKind",
    "PerceptionControlGroup",
    "PerceptionCorrelation",
    "PerceptionCorrelationGroup",
    "PerceptionFaceAuthenticationGroup",
    "PerceptionFrame",
    "PerceptionFrameProviderInfo",
    "PerceptionFrameProviderManagerService",
    "PerceptionPropertyChangeRequest",
    "PerceptionVideoFrameAllocator",
    "IPerceptionFrameProvider",
    "IPerceptionFrameProviderManager",
    "PerceptionStartFaceAuthenticationHandler",
    "PerceptionStopFaceAuthenticationHandler",
]

PerceptionStartFaceAuthenticationHandler = typing.Callable[[PerceptionFaceAuthenticationGroup], bool]
PerceptionStopFaceAuthenticationHandler = typing.Callable[[PerceptionFaceAuthenticationGroup], None]
