# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum
import datetime
import sys
import types
import typing

import winrt.system
import winrt.windows.applicationmodel.background
import winrt.windows.foundation
import winrt.windows.graphics.imaging
import winrt.windows.storage.streams
import winrt.windows.ui.xaml

class BitmapCreateOptions(enum.IntFlag):
    NONE = 0
    IGNORE_IMAGE_CACHE = 0x8

class DecodePixelType(enum.IntEnum):
    PHYSICAL = 0
    LOGICAL = 1

class SvgImageSourceLoadStatus(enum.IntEnum):
    SUCCESS = 0
    NETWORK_ERROR = 1
    INVALID_FORMAT = 2
    OTHER = 3

Self = typing.TypeVar('Self')

class BitmapImage(winrt.system.Object):
    uri_source: typing.Optional[winrt.windows.foundation.Uri]
    decode_pixel_width: winrt.system.Int32
    decode_pixel_height: winrt.system.Int32
    create_options: BitmapCreateOptions
    decode_pixel_type: DecodePixelType
    auto_play: winrt.system.Boolean
    is_animated_bitmap: winrt.system.Boolean
    is_playing: winrt.system.Boolean
    create_options_property: typing.ClassVar[typing.Optional[winrt.windows.ui.xaml.DependencyProperty]]
    decode_pixel_height_property: typing.ClassVar[typing.Optional[winrt.windows.ui.xaml.DependencyProperty]]
    decode_pixel_width_property: typing.ClassVar[typing.Optional[winrt.windows.ui.xaml.DependencyProperty]]
    uri_source_property: typing.ClassVar[typing.Optional[winrt.windows.ui.xaml.DependencyProperty]]
    decode_pixel_type_property: typing.ClassVar[typing.Optional[winrt.windows.ui.xaml.DependencyProperty]]
    auto_play_property: typing.ClassVar[typing.Optional[winrt.windows.ui.xaml.DependencyProperty]]
    is_animated_bitmap_property: typing.ClassVar[typing.Optional[winrt.windows.ui.xaml.DependencyProperty]]
    is_playing_property: typing.ClassVar[typing.Optional[winrt.windows.ui.xaml.DependencyProperty]]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> BitmapImage: ...
    @typing.overload
    def __new__(cls: typing.Type[BitmapImage], uri_source: typing.Optional[winrt.windows.foundation.Uri]) -> BitmapImage:...
    @typing.overload
    def __new__(cls: typing.Type[BitmapImage]) -> BitmapImage:...
    def play(self) -> None: ...
    def stop(self) -> None: ...
    def add_download_progress(self, handler: typing.Optional[DownloadProgressEventHandler], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_download_progress(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_image_failed(self, handler: typing.Optional[winrt.windows.ui.xaml.ExceptionRoutedEventHandler], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_image_failed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_image_opened(self, handler: typing.Optional[winrt.windows.ui.xaml.RoutedEventHandler], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_image_opened(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class BitmapSource(winrt.system.Object):
    pixel_height: winrt.system.Int32
    pixel_width: winrt.system.Int32
    pixel_height_property: typing.ClassVar[typing.Optional[winrt.windows.ui.xaml.DependencyProperty]]
    pixel_width_property: typing.ClassVar[typing.Optional[winrt.windows.ui.xaml.DependencyProperty]]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> BitmapSource: ...
    def set_source(self, stream_source: typing.Optional[winrt.windows.storage.streams.IRandomAccessStream], /) -> None: ...
    def set_source_async(self, stream_source: typing.Optional[winrt.windows.storage.streams.IRandomAccessStream], /) -> winrt.windows.foundation.IAsyncAction: ...

class DownloadProgressEventArgs(winrt.system.Object):
    progress: winrt.system.Int32
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> DownloadProgressEventArgs: ...

class RenderTargetBitmap(winrt.system.Object):
    pixel_height: winrt.system.Int32
    pixel_width: winrt.system.Int32
    pixel_height_property: typing.ClassVar[typing.Optional[winrt.windows.ui.xaml.DependencyProperty]]
    pixel_width_property: typing.ClassVar[typing.Optional[winrt.windows.ui.xaml.DependencyProperty]]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> RenderTargetBitmap: ...
    def __new__(cls: typing.Type[RenderTargetBitmap]) -> RenderTargetBitmap:...
    def get_pixels_async(self) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.storage.streams.IBuffer]: ...
    @typing.overload
    def render_async(self, element: typing.Optional[winrt.windows.ui.xaml.UIElement], /) -> winrt.windows.foundation.IAsyncAction: ...
    @typing.overload
    def render_async(self, element: typing.Optional[winrt.windows.ui.xaml.UIElement], scaled_width: winrt.system.Int32, scaled_height: winrt.system.Int32, /) -> winrt.windows.foundation.IAsyncAction: ...

class SoftwareBitmapSource(winrt.system.Object):
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, *args) -> None: ...
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SoftwareBitmapSource: ...
    def __new__(cls: typing.Type[SoftwareBitmapSource]) -> SoftwareBitmapSource:...
    def close(self) -> None: ...
    def set_bitmap_async(self, software_bitmap: typing.Optional[winrt.windows.graphics.imaging.SoftwareBitmap], /) -> winrt.windows.foundation.IAsyncAction: ...

class SurfaceImageSource(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SurfaceImageSource: ...
    @typing.overload
    def __new__(cls: typing.Type[SurfaceImageSource], pixel_width: winrt.system.Int32, pixel_height: winrt.system.Int32) -> SurfaceImageSource:...
    @typing.overload
    def __new__(cls: typing.Type[SurfaceImageSource], pixel_width: winrt.system.Int32, pixel_height: winrt.system.Int32, is_opaque: winrt.system.Boolean) -> SurfaceImageSource:...

class SvgImageSource(winrt.system.Object):
    uri_source: typing.Optional[winrt.windows.foundation.Uri]
    rasterize_pixel_width: winrt.system.Double
    rasterize_pixel_height: winrt.system.Double
    rasterize_pixel_height_property: typing.ClassVar[typing.Optional[winrt.windows.ui.xaml.DependencyProperty]]
    rasterize_pixel_width_property: typing.ClassVar[typing.Optional[winrt.windows.ui.xaml.DependencyProperty]]
    uri_source_property: typing.ClassVar[typing.Optional[winrt.windows.ui.xaml.DependencyProperty]]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SvgImageSource: ...
    @typing.overload
    def __new__(cls: typing.Type[SvgImageSource]) -> SvgImageSource:...
    @typing.overload
    def __new__(cls: typing.Type[SvgImageSource], uri_source: typing.Optional[winrt.windows.foundation.Uri]) -> SvgImageSource:...
    def set_source_async(self, stream_source: typing.Optional[winrt.windows.storage.streams.IRandomAccessStream], /) -> winrt.windows.foundation.IAsyncOperation[SvgImageSourceLoadStatus]: ...
    def add_open_failed(self, handler: winrt.windows.foundation.TypedEventHandler[SvgImageSource, SvgImageSourceFailedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_open_failed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_opened(self, handler: winrt.windows.foundation.TypedEventHandler[SvgImageSource, SvgImageSourceOpenedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_opened(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class SvgImageSourceFailedEventArgs(winrt.system.Object):
    status: SvgImageSourceLoadStatus
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SvgImageSourceFailedEventArgs: ...

class SvgImageSourceOpenedEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SvgImageSourceOpenedEventArgs: ...

class VirtualSurfaceImageSource(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> VirtualSurfaceImageSource: ...
    @typing.overload
    def __new__(cls: typing.Type[VirtualSurfaceImageSource], pixel_width: winrt.system.Int32, pixel_height: winrt.system.Int32) -> VirtualSurfaceImageSource:...
    @typing.overload
    def __new__(cls: typing.Type[VirtualSurfaceImageSource], pixel_width: winrt.system.Int32, pixel_height: winrt.system.Int32, is_opaque: winrt.system.Boolean) -> VirtualSurfaceImageSource:...

class WriteableBitmap(winrt.system.Object):
    pixel_buffer: typing.Optional[winrt.windows.storage.streams.IBuffer]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> WriteableBitmap: ...
    def __new__(cls: typing.Type[WriteableBitmap], pixel_width: winrt.system.Int32, pixel_height: winrt.system.Int32) -> WriteableBitmap:...
    def invalidate(self) -> None: ...

class XamlRenderingBackgroundTask(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> XamlRenderingBackgroundTask: ...
    def on_run(self, task_instance: typing.Optional[winrt.windows.applicationmodel.background.IBackgroundTaskInstance], /) -> None: ...

DownloadProgressEventHandler = typing.Callable[[typing.Optional[winrt.system.Object], typing.Optional[DownloadProgressEventArgs]], None]
