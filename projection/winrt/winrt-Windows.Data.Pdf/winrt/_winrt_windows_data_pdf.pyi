# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.foundation as windows_foundation
import winrt.windows.storage as windows_storage
import winrt.windows.storage.streams as windows_storage_streams
import winrt.windows.ui as windows_ui

from winrt.windows.data.pdf import PdfPageRotation

Self = typing.TypeVar('Self')

@typing.final
class PdfDocument_Static(winrt._winrt.IInspectable_Static):
    # Windows.Foundation.IAsyncOperation`1<Windows.Data.Pdf.PdfDocument> Windows.Data.Pdf.PdfDocument::LoadFromFileAsync(Windows.Storage.IStorageFile)
    def load_from_file_async(cls, file: windows_storage.IStorageFile, /) -> windows_foundation.IAsyncOperation[PdfDocument]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Data.Pdf.PdfDocument> Windows.Data.Pdf.PdfDocument::LoadFromFileAsync(Windows.Storage.IStorageFile,System.String)
    def load_from_file_with_password_async(cls, file: windows_storage.IStorageFile, password: str, /) -> windows_foundation.IAsyncOperation[PdfDocument]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Data.Pdf.PdfDocument> Windows.Data.Pdf.PdfDocument::LoadFromStreamAsync(Windows.Storage.Streams.IRandomAccessStream)
    def load_from_stream_async(cls, input_stream: windows_storage_streams.IRandomAccessStream, /) -> windows_foundation.IAsyncOperation[PdfDocument]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Data.Pdf.PdfDocument> Windows.Data.Pdf.PdfDocument::LoadFromStreamAsync(Windows.Storage.Streams.IRandomAccessStream,System.String)
    def load_from_stream_with_password_async(cls, input_stream: windows_storage_streams.IRandomAccessStream, password: str, /) -> windows_foundation.IAsyncOperation[PdfDocument]: ...

@typing.final
class PdfDocument(winrt.system.Object, metaclass=PdfDocument_Static):
    # Windows.Data.Pdf.PdfPage Windows.Data.Pdf.PdfDocument::GetPage(System.UInt32)
    def get_page(self, page_index: winrt.system.UInt32, /) -> PdfPage: ...
    # System.Boolean Windows.Data.Pdf.PdfDocument::get_IsPasswordProtected()
    @_property
    def is_password_protected(self) -> bool: ...
    # System.UInt32 Windows.Data.Pdf.PdfDocument::get_PageCount()
    @_property
    def page_count(self) -> winrt.system.UInt32: ...

@typing.final
class PdfPage(winrt.system.Object, windows_foundation.IClosable):
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, exc_type: typing.Optional[typing.Type[BaseException]], exc_value: typing.Optional[BaseException], traceback: typing.Optional[types.TracebackType]) -> None: ...
    # System.Void Windows.Data.Pdf.PdfPage::Close()
    def close(self) -> None: ...
    # Windows.Foundation.IAsyncAction Windows.Data.Pdf.PdfPage::PreparePageAsync()
    def prepare_page_async(self) -> windows_foundation.IAsyncAction: ...
    # Windows.Foundation.IAsyncAction Windows.Data.Pdf.PdfPage::RenderToStreamAsync(Windows.Storage.Streams.IRandomAccessStream)
    def render_to_stream_async(self, output_stream: windows_storage_streams.IRandomAccessStream, /) -> windows_foundation.IAsyncAction: ...
    # Windows.Foundation.IAsyncAction Windows.Data.Pdf.PdfPage::RenderToStreamAsync(Windows.Storage.Streams.IRandomAccessStream,Windows.Data.Pdf.PdfPageRenderOptions)
    def render_with_options_to_stream_async(self, output_stream: windows_storage_streams.IRandomAccessStream, options: PdfPageRenderOptions, /) -> windows_foundation.IAsyncAction: ...
    # Windows.Data.Pdf.PdfPageDimensions Windows.Data.Pdf.PdfPage::get_Dimensions()
    @_property
    def dimensions(self) -> PdfPageDimensions: ...
    # System.UInt32 Windows.Data.Pdf.PdfPage::get_Index()
    @_property
    def index(self) -> winrt.system.UInt32: ...
    # System.Single Windows.Data.Pdf.PdfPage::get_PreferredZoom()
    @_property
    def preferred_zoom(self) -> winrt.system.Single: ...
    # Windows.Data.Pdf.PdfPageRotation Windows.Data.Pdf.PdfPage::get_Rotation()
    @_property
    def rotation(self) -> PdfPageRotation: ...
    # Windows.Foundation.Size Windows.Data.Pdf.PdfPage::get_Size()
    @_property
    def size(self) -> windows_foundation.Size: ...

@typing.final
class PdfPageDimensions(winrt.system.Object):
    # Windows.Foundation.Rect Windows.Data.Pdf.PdfPageDimensions::get_ArtBox()
    @_property
    def art_box(self) -> windows_foundation.Rect: ...
    # Windows.Foundation.Rect Windows.Data.Pdf.PdfPageDimensions::get_BleedBox()
    @_property
    def bleed_box(self) -> windows_foundation.Rect: ...
    # Windows.Foundation.Rect Windows.Data.Pdf.PdfPageDimensions::get_CropBox()
    @_property
    def crop_box(self) -> windows_foundation.Rect: ...
    # Windows.Foundation.Rect Windows.Data.Pdf.PdfPageDimensions::get_MediaBox()
    @_property
    def media_box(self) -> windows_foundation.Rect: ...
    # Windows.Foundation.Rect Windows.Data.Pdf.PdfPageDimensions::get_TrimBox()
    @_property
    def trim_box(self) -> windows_foundation.Rect: ...

@typing.final
class PdfPageRenderOptions(winrt.system.Object):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # Windows.Foundation.Rect Windows.Data.Pdf.PdfPageRenderOptions::get_SourceRect()
    @_property
    def source_rect(self) -> windows_foundation.Rect: ...
    # System.Void Windows.Data.Pdf.PdfPageRenderOptions::put_SourceRect(Windows.Foundation.Rect)
    @source_rect.setter
    def source_rect(self, value: typing.Union[windows_foundation.Rect, typing.Tuple[winrt.system.Single, winrt.system.Single, winrt.system.Single, winrt.system.Single]]) -> None: ...
    # System.Boolean Windows.Data.Pdf.PdfPageRenderOptions::get_IsIgnoringHighContrast()
    @_property
    def is_ignoring_high_contrast(self) -> bool: ...
    # System.Void Windows.Data.Pdf.PdfPageRenderOptions::put_IsIgnoringHighContrast(System.Boolean)
    @is_ignoring_high_contrast.setter
    def is_ignoring_high_contrast(self, value: bool) -> None: ...
    # System.UInt32 Windows.Data.Pdf.PdfPageRenderOptions::get_DestinationWidth()
    @_property
    def destination_width(self) -> winrt.system.UInt32: ...
    # System.Void Windows.Data.Pdf.PdfPageRenderOptions::put_DestinationWidth(System.UInt32)
    @destination_width.setter
    def destination_width(self, value: winrt.system.UInt32) -> None: ...
    # System.UInt32 Windows.Data.Pdf.PdfPageRenderOptions::get_DestinationHeight()
    @_property
    def destination_height(self) -> winrt.system.UInt32: ...
    # System.Void Windows.Data.Pdf.PdfPageRenderOptions::put_DestinationHeight(System.UInt32)
    @destination_height.setter
    def destination_height(self, value: winrt.system.UInt32) -> None: ...
    # System.Guid Windows.Data.Pdf.PdfPageRenderOptions::get_BitmapEncoderId()
    @_property
    def bitmap_encoder_id(self) -> _uuid.UUID: ...
    # System.Void Windows.Data.Pdf.PdfPageRenderOptions::put_BitmapEncoderId(System.Guid)
    @bitmap_encoder_id.setter
    def bitmap_encoder_id(self, value: _uuid.UUID) -> None: ...
    # Windows.UI.Color Windows.Data.Pdf.PdfPageRenderOptions::get_BackgroundColor()
    @_property
    def background_color(self) -> windows_ui.Color: ...
    # System.Void Windows.Data.Pdf.PdfPageRenderOptions::put_BackgroundColor(Windows.UI.Color)
    @background_color.setter
    def background_color(self, value: typing.Union[windows_ui.Color, typing.Tuple[winrt.system.UInt8, winrt.system.UInt8, winrt.system.UInt8, winrt.system.UInt8]]) -> None: ...

