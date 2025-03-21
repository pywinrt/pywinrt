# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winrt._winrt_windows_media_capture_frames import (
    AudioMediaFrame,
    BufferMediaFrame,
    DepthMediaFrame,
    DepthMediaFrameFormat,
    InfraredMediaFrame,
    MediaFrameArrivedEventArgs,
    MediaFrameFormat,
    MediaFrameReader,
    MediaFrameReference,
    MediaFrameSource,
    MediaFrameSourceController,
    MediaFrameSourceGetPropertyResult,
    MediaFrameSourceGroup,
    MediaFrameSourceInfo,
    MultiSourceMediaFrameArrivedEventArgs,
    MultiSourceMediaFrameReader,
    MultiSourceMediaFrameReference,
    VideoMediaFrame,
    VideoMediaFrameFormat,
)

__all__ = [
    "MediaFrameReaderAcquisitionMode",
    "MediaFrameReaderStartStatus",
    "MediaFrameSourceGetPropertyStatus",
    "MediaFrameSourceKind",
    "MediaFrameSourceSetPropertyStatus",
    "MultiSourceMediaFrameReaderStartStatus",
    "AudioMediaFrame",
    "BufferMediaFrame",
    "DepthMediaFrame",
    "DepthMediaFrameFormat",
    "InfraredMediaFrame",
    "MediaFrameArrivedEventArgs",
    "MediaFrameFormat",
    "MediaFrameReader",
    "MediaFrameReference",
    "MediaFrameSource",
    "MediaFrameSourceController",
    "MediaFrameSourceGetPropertyResult",
    "MediaFrameSourceGroup",
    "MediaFrameSourceInfo",
    "MultiSourceMediaFrameArrivedEventArgs",
    "MultiSourceMediaFrameReader",
    "MultiSourceMediaFrameReference",
    "VideoMediaFrame",
    "VideoMediaFrameFormat",
]

class MediaFrameReaderAcquisitionMode(enum.IntEnum):
    REALTIME = 0
    BUFFERED = 1

class MediaFrameReaderStartStatus(enum.IntEnum):
    SUCCESS = 0
    UNKNOWN_FAILURE = 1
    DEVICE_NOT_AVAILABLE = 2
    OUTPUT_FORMAT_NOT_SUPPORTED = 3
    EXCLUSIVE_CONTROL_NOT_AVAILABLE = 4

class MediaFrameSourceGetPropertyStatus(enum.IntEnum):
    SUCCESS = 0
    UNKNOWN_FAILURE = 1
    NOT_SUPPORTED = 2
    DEVICE_NOT_AVAILABLE = 3
    MAX_PROPERTY_VALUE_SIZE_TOO_SMALL = 4
    MAX_PROPERTY_VALUE_SIZE_REQUIRED = 5

class MediaFrameSourceKind(enum.IntEnum):
    CUSTOM = 0
    COLOR = 1
    INFRARED = 2
    DEPTH = 3
    AUDIO = 4
    IMAGE = 5
    METADATA = 6

class MediaFrameSourceSetPropertyStatus(enum.IntEnum):
    SUCCESS = 0
    UNKNOWN_FAILURE = 1
    NOT_SUPPORTED = 2
    INVALID_VALUE = 3
    DEVICE_NOT_AVAILABLE = 4
    NOT_IN_CONTROL = 5

class MultiSourceMediaFrameReaderStartStatus(enum.IntEnum):
    SUCCESS = 0
    NOT_SUPPORTED = 1
    INSUFFICIENT_RESOURCES = 2
    DEVICE_NOT_AVAILABLE = 3
    UNKNOWN_FAILURE = 4

