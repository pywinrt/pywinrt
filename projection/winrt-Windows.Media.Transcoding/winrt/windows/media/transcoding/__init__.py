# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from . import _winrt_windows_media_transcoding

class MediaVideoProcessingAlgorithm(enum.IntEnum):
    DEFAULT = 0
    MRF_CRF444 = 1

class TranscodeFailureReason(enum.IntEnum):
    NONE = 0
    UNKNOWN = 1
    INVALID_PROFILE = 2
    CODEC_NOT_FOUND = 3

MediaTranscoder = _winrt_windows_media_transcoding.MediaTranscoder
PrepareTranscodeResult = _winrt_windows_media_transcoding.PrepareTranscodeResult