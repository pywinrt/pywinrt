# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing

import winrt.system
import winrt.windows.foundation
import winrt.windows.foundation.collections
import winrt.windows.graphics.imaging
import winrt.windows.media

Self = typing.TypeVar('Self')

class DetectedFace(winrt.system.Object):
    face_box: winrt.windows.graphics.imaging.BitmapBounds
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> DetectedFace: ...

class FaceDetector(winrt.system.Object):
    min_detectable_face_size: winrt.windows.graphics.imaging.BitmapSize
    max_detectable_face_size: winrt.windows.graphics.imaging.BitmapSize
    is_supported: typing.ClassVar[winrt.system.Boolean]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> FaceDetector: ...
    @staticmethod
    def create_async() -> winrt.windows.foundation.IAsyncOperation[FaceDetector]: ...
    @typing.overload
    def detect_faces_async(self, image: typing.Optional[winrt.windows.graphics.imaging.SoftwareBitmap], /) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.foundation.collections.IVector[DetectedFace]]: ...
    @typing.overload
    def detect_faces_async(self, image: typing.Optional[winrt.windows.graphics.imaging.SoftwareBitmap], search_area: winrt.windows.graphics.imaging.BitmapBounds, /) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.foundation.collections.IVector[DetectedFace]]: ...
    @staticmethod
    def get_supported_bitmap_pixel_formats() -> typing.Optional[winrt.windows.foundation.collections.IVectorView[winrt.windows.graphics.imaging.BitmapPixelFormat]]: ...
    @staticmethod
    def is_bitmap_pixel_format_supported(bitmap_pixel_format: winrt.windows.graphics.imaging.BitmapPixelFormat, /) -> winrt.system.Boolean: ...

class FaceTracker(winrt.system.Object):
    min_detectable_face_size: winrt.windows.graphics.imaging.BitmapSize
    max_detectable_face_size: winrt.windows.graphics.imaging.BitmapSize
    is_supported: typing.ClassVar[winrt.system.Boolean]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> FaceTracker: ...
    @staticmethod
    def create_async() -> winrt.windows.foundation.IAsyncOperation[FaceTracker]: ...
    @staticmethod
    def get_supported_bitmap_pixel_formats() -> typing.Optional[winrt.windows.foundation.collections.IVectorView[winrt.windows.graphics.imaging.BitmapPixelFormat]]: ...
    @staticmethod
    def is_bitmap_pixel_format_supported(bitmap_pixel_format: winrt.windows.graphics.imaging.BitmapPixelFormat, /) -> winrt.system.Boolean: ...
    def process_next_frame_async(self, video_frame: typing.Optional[winrt.windows.media.VideoFrame], /) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.foundation.collections.IVector[DetectedFace]]: ...
