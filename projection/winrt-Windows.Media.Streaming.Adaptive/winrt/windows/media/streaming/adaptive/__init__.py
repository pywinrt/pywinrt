# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winrt._winrt_windows_media_streaming_adaptive import (
    AdaptiveMediaSource,
    AdaptiveMediaSourceAdvancedSettings,
    AdaptiveMediaSourceCorrelatedTimes,
    AdaptiveMediaSourceCreationResult,
    AdaptiveMediaSourceDiagnosticAvailableEventArgs,
    AdaptiveMediaSourceDiagnostics,
    AdaptiveMediaSourceDownloadBitrateChangedEventArgs,
    AdaptiveMediaSourceDownloadCompletedEventArgs,
    AdaptiveMediaSourceDownloadFailedEventArgs,
    AdaptiveMediaSourceDownloadRequestedDeferral,
    AdaptiveMediaSourceDownloadRequestedEventArgs,
    AdaptiveMediaSourceDownloadResult,
    AdaptiveMediaSourceDownloadStatistics,
    AdaptiveMediaSourcePlaybackBitrateChangedEventArgs,
)

__all__ = [
    "AdaptiveMediaSourceCreationStatus",
    "AdaptiveMediaSourceDiagnosticType",
    "AdaptiveMediaSourceDownloadBitrateChangedReason",
    "AdaptiveMediaSourceResourceType",
    "AdaptiveMediaSource",
    "AdaptiveMediaSourceAdvancedSettings",
    "AdaptiveMediaSourceCorrelatedTimes",
    "AdaptiveMediaSourceCreationResult",
    "AdaptiveMediaSourceDiagnosticAvailableEventArgs",
    "AdaptiveMediaSourceDiagnostics",
    "AdaptiveMediaSourceDownloadBitrateChangedEventArgs",
    "AdaptiveMediaSourceDownloadCompletedEventArgs",
    "AdaptiveMediaSourceDownloadFailedEventArgs",
    "AdaptiveMediaSourceDownloadRequestedDeferral",
    "AdaptiveMediaSourceDownloadRequestedEventArgs",
    "AdaptiveMediaSourceDownloadResult",
    "AdaptiveMediaSourceDownloadStatistics",
    "AdaptiveMediaSourcePlaybackBitrateChangedEventArgs",
]

class AdaptiveMediaSourceCreationStatus(enum.IntEnum):
    SUCCESS = 0
    MANIFEST_DOWNLOAD_FAILURE = 1
    MANIFEST_PARSE_FAILURE = 2
    UNSUPPORTED_MANIFEST_CONTENT_TYPE = 3
    UNSUPPORTED_MANIFEST_VERSION = 4
    UNSUPPORTED_MANIFEST_PROFILE = 5
    UNKNOWN_FAILURE = 6

class AdaptiveMediaSourceDiagnosticType(enum.IntEnum):
    MANIFEST_UNCHANGED_UPON_RELOAD = 0
    MANIFEST_MISMATCH_UPON_RELOAD = 1
    MANIFEST_SIGNALED_END_OF_LIVE_EVENT_UPON_RELOAD = 2
    MEDIA_SEGMENT_SKIPPED = 3
    RESOURCE_NOT_FOUND = 4
    RESOURCE_TIMED_OUT = 5
    RESOURCE_PARSING_ERROR = 6
    BITRATE_DISABLED = 7
    FATAL_MEDIA_SOURCE_ERROR = 8

class AdaptiveMediaSourceDownloadBitrateChangedReason(enum.IntEnum):
    SUFFICIENT_INBOUND_BITS_PER_SECOND = 0
    INSUFFICIENT_INBOUND_BITS_PER_SECOND = 1
    LOW_BUFFER_LEVEL = 2
    POSITION_CHANGED = 3
    TRACK_SELECTION_CHANGED = 4
    DESIRED_BITRATES_CHANGED = 5
    ERROR_IN_PREVIOUS_BITRATE = 6

class AdaptiveMediaSourceResourceType(enum.IntEnum):
    MANIFEST = 0
    INITIALIZATION_SEGMENT = 1
    MEDIA_SEGMENT = 2
    KEY = 3
    INITIALIZATION_VECTOR = 4
    MEDIA_SEGMENT_INDEX = 5

