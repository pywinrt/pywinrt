# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum
import typing
import uuid as _uuid

import winrt.system
from winrt._winrt_windows_ui_xaml_media_imaging import (
    BitmapImage,
    BitmapSource,
    DownloadProgressEventArgs,
    RenderTargetBitmap,
    SoftwareBitmapSource,
    SurfaceImageSource,
    SvgImageSource,
    SvgImageSourceFailedEventArgs,
    SvgImageSourceOpenedEventArgs,
    VirtualSurfaceImageSource,
    WriteableBitmap,
    XamlRenderingBackgroundTask,
)
from typing import TYPE_CHECKING
if TYPE_CHECKING:
    from winrt._winrt_windows_ui_xaml_media_imaging import (
        BitmapSource_Static,
        SurfaceImageSource_Static,
        SvgImageSource_Static,
        XamlRenderingBackgroundTask_Static,
    )

__all__ = [
    "BitmapCreateOptions",
    "DecodePixelType",
    "SvgImageSourceLoadStatus",
    "BitmapImage",
    "BitmapSource",
    "DownloadProgressEventArgs",
    "RenderTargetBitmap",
    "SoftwareBitmapSource",
    "SurfaceImageSource",
    "SvgImageSource",
    "SvgImageSourceFailedEventArgs",
    "SvgImageSourceOpenedEventArgs",
    "VirtualSurfaceImageSource",
    "WriteableBitmap",
    "XamlRenderingBackgroundTask",
    "DownloadProgressEventHandler",
]

class BitmapCreateOptions(enum.IntFlag):
    NONE = 0x0
    IGNORE_IMAGE_CACHE = 0x8

class DecodePixelType(enum.IntEnum):
    PHYSICAL = 0
    LOGICAL = 1

class SvgImageSourceLoadStatus(enum.IntEnum):
    SUCCESS = 0
    NETWORK_ERROR = 1
    INVALID_FORMAT = 2
    OTHER = 3

DownloadProgressEventHandler = typing.Callable[[winrt.system.Object, DownloadProgressEventArgs], None]
