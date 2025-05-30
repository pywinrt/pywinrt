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
import winrt.windows.storage.streams as windows_storage_streams
import winui3.microsoft.ui as microsoft_ui
import winui3.microsoft.ui.dispatching as microsoft_ui_dispatching

from winui3.microsoft.graphics.display import DisplayAdvancedColorKind, DisplayHdrMetadataFormat

Self = typing.TypeVar('Self')

@typing.final
class DisplayAdvancedColorInfo(winrt.system.Object):
    # System.Boolean Microsoft.Graphics.Display.DisplayAdvancedColorInfo::IsAdvancedColorKindAvailable(Microsoft.Graphics.Display.DisplayAdvancedColorKind)
    def is_advanced_color_kind_available(self, kind: DisplayAdvancedColorKind, /) -> bool: ...
    # System.Boolean Microsoft.Graphics.Display.DisplayAdvancedColorInfo::IsHdrMetadataFormatCurrentlySupported(Microsoft.Graphics.Display.DisplayHdrMetadataFormat)
    def is_hdr_metadata_format_currently_supported(self, format: DisplayHdrMetadataFormat, /) -> bool: ...
    # Windows.Foundation.Point Microsoft.Graphics.Display.DisplayAdvancedColorInfo::get_BluePrimary()
    @_property
    def blue_primary(self) -> windows_foundation.Point: ...
    # Microsoft.Graphics.Display.DisplayAdvancedColorKind Microsoft.Graphics.Display.DisplayAdvancedColorInfo::get_CurrentAdvancedColorKind()
    @_property
    def current_advanced_color_kind(self) -> DisplayAdvancedColorKind: ...
    # Windows.Foundation.Point Microsoft.Graphics.Display.DisplayAdvancedColorInfo::get_GreenPrimary()
    @_property
    def green_primary(self) -> windows_foundation.Point: ...
    # System.Double Microsoft.Graphics.Display.DisplayAdvancedColorInfo::get_MaxAverageFullFrameLuminanceInNits()
    @_property
    def max_average_full_frame_luminance_in_nits(self) -> winrt.system.Double: ...
    # System.Double Microsoft.Graphics.Display.DisplayAdvancedColorInfo::get_MaxLuminanceInNits()
    @_property
    def max_luminance_in_nits(self) -> winrt.system.Double: ...
    # System.Double Microsoft.Graphics.Display.DisplayAdvancedColorInfo::get_MinLuminanceInNits()
    @_property
    def min_luminance_in_nits(self) -> winrt.system.Double: ...
    # Windows.Foundation.Point Microsoft.Graphics.Display.DisplayAdvancedColorInfo::get_RedPrimary()
    @_property
    def red_primary(self) -> windows_foundation.Point: ...
    # System.Double Microsoft.Graphics.Display.DisplayAdvancedColorInfo::get_SdrWhiteLevelInNits()
    @_property
    def sdr_white_level_in_nits(self) -> winrt.system.Double: ...
    # Windows.Foundation.Point Microsoft.Graphics.Display.DisplayAdvancedColorInfo::get_WhitePoint()
    @_property
    def white_point(self) -> windows_foundation.Point: ...

@typing.final
class DisplayInformation_Static(winrt._winrt.IInspectable_Static):
    # Microsoft.Graphics.Display.DisplayInformation Microsoft.Graphics.Display.DisplayInformation::CreateForDisplayId(Microsoft.UI.DisplayId)
    def create_for_display_id(cls, display_id: typing.Union[microsoft_ui.DisplayId, typing.Tuple[winrt.system.UInt64]], /) -> DisplayInformation: ...
    # Microsoft.Graphics.Display.DisplayInformation Microsoft.Graphics.Display.DisplayInformation::CreateForWindowId(Microsoft.UI.WindowId)
    def create_for_window_id(cls, window_id: typing.Union[microsoft_ui.WindowId, typing.Tuple[winrt.system.UInt64]], /) -> DisplayInformation: ...

@typing.final
class DisplayInformation(winrt.system.Object, windows_foundation.IClosable, metaclass=DisplayInformation_Static):
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, exc_type: typing.Optional[typing.Type[BaseException]], exc_value: typing.Optional[BaseException], traceback: typing.Optional[types.TracebackType]) -> None: ...
    # System.Void Microsoft.Graphics.Display.DisplayInformation::Close()
    def close(self) -> None: ...
    # Microsoft.Graphics.Display.DisplayAdvancedColorInfo Microsoft.Graphics.Display.DisplayInformation::GetAdvancedColorInfo()
    def get_advanced_color_info(self) -> DisplayAdvancedColorInfo: ...
    # Windows.Storage.Streams.IRandomAccessStream Microsoft.Graphics.Display.DisplayInformation::GetColorProfile()
    def get_color_profile(self) -> windows_storage_streams.IRandomAccessStream: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Storage.Streams.IRandomAccessStream> Microsoft.Graphics.Display.DisplayInformation::GetColorProfileAsync()
    def get_color_profile_async(self) -> windows_foundation.IAsyncOperation[windows_storage_streams.IRandomAccessStream]: ...
    # Windows.Foundation.EventRegistrationToken Microsoft.Graphics.Display.DisplayInformation::add_AdvancedColorInfoChanged(Windows.Foundation.TypedEventHandler`2<Microsoft.Graphics.Display.DisplayInformation,System.Object>)
    def add_advanced_color_info_changed(self, handler: windows_foundation.TypedEventHandler[DisplayInformation, winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Microsoft.Graphics.Display.DisplayInformation::remove_AdvancedColorInfoChanged(Windows.Foundation.EventRegistrationToken)
    def remove_advanced_color_info_changed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Microsoft.Graphics.Display.DisplayInformation::add_ColorProfileChanged(Windows.Foundation.TypedEventHandler`2<Microsoft.Graphics.Display.DisplayInformation,System.Object>)
    def add_color_profile_changed(self, handler: windows_foundation.TypedEventHandler[DisplayInformation, winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Microsoft.Graphics.Display.DisplayInformation::remove_ColorProfileChanged(Windows.Foundation.EventRegistrationToken)
    def remove_color_profile_changed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Microsoft.Graphics.Display.DisplayInformation::add_Destroyed(Windows.Foundation.TypedEventHandler`2<Microsoft.Graphics.Display.DisplayInformation,System.Object>)
    def add_destroyed(self, handler: windows_foundation.TypedEventHandler[DisplayInformation, winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Microsoft.Graphics.Display.DisplayInformation::remove_Destroyed(Windows.Foundation.EventRegistrationToken)
    def remove_destroyed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Microsoft.Graphics.Display.DisplayInformation::add_IsStereoEnabledChanged(Windows.Foundation.TypedEventHandler`2<Microsoft.Graphics.Display.DisplayInformation,System.Object>)
    def add_is_stereo_enabled_changed(self, handler: windows_foundation.TypedEventHandler[DisplayInformation, winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Microsoft.Graphics.Display.DisplayInformation::remove_IsStereoEnabledChanged(Windows.Foundation.EventRegistrationToken)
    def remove_is_stereo_enabled_changed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Microsoft.UI.Dispatching.DispatcherQueue Microsoft.Graphics.Display.DisplayInformation::get_DispatcherQueue()
    @_property
    def dispatcher_queue(self) -> microsoft_ui_dispatching.DispatcherQueue: ...
    # System.Boolean Microsoft.Graphics.Display.DisplayInformation::get_IsStereoEnabled()
    @_property
    def is_stereo_enabled(self) -> bool: ...

