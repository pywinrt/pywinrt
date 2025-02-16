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
import winrt.windows.ui as windows_ui

from winrt.windows.media.closedcaptioning import ClosedCaptionColor, ClosedCaptionEdgeEffect, ClosedCaptionOpacity, ClosedCaptionSize, ClosedCaptionStyle

Self = typing.TypeVar('Self')

@typing.final
class ClosedCaptionProperties_Static(winrt._winrt.IInspectable_Static):
    # Windows.Foundation.EventRegistrationToken Windows.Media.ClosedCaptioning.ClosedCaptionProperties::add_PropertiesChanged(Windows.Foundation.EventHandler`1<System.Object>)
    def add_properties_changed(cls, handler: windows_foundation.EventHandler[winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.Media.ClosedCaptioning.ClosedCaptionProperties::remove_PropertiesChanged(Windows.Foundation.EventRegistrationToken)
    def remove_properties_changed(cls, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Media.ClosedCaptioning.ClosedCaptionColor Windows.Media.ClosedCaptioning.ClosedCaptionProperties::get_BackgroundColor()
    @_property
    def background_color(cls) -> ClosedCaptionColor: ...
    # Windows.Media.ClosedCaptioning.ClosedCaptionOpacity Windows.Media.ClosedCaptioning.ClosedCaptionProperties::get_BackgroundOpacity()
    @_property
    def background_opacity(cls) -> ClosedCaptionOpacity: ...
    # Windows.UI.Color Windows.Media.ClosedCaptioning.ClosedCaptionProperties::get_ComputedBackgroundColor()
    @_property
    def computed_background_color(cls) -> windows_ui.Color: ...
    # Windows.UI.Color Windows.Media.ClosedCaptioning.ClosedCaptionProperties::get_ComputedFontColor()
    @_property
    def computed_font_color(cls) -> windows_ui.Color: ...
    # Windows.UI.Color Windows.Media.ClosedCaptioning.ClosedCaptionProperties::get_ComputedRegionColor()
    @_property
    def computed_region_color(cls) -> windows_ui.Color: ...
    # Windows.Media.ClosedCaptioning.ClosedCaptionColor Windows.Media.ClosedCaptioning.ClosedCaptionProperties::get_FontColor()
    @_property
    def font_color(cls) -> ClosedCaptionColor: ...
    # Windows.Media.ClosedCaptioning.ClosedCaptionEdgeEffect Windows.Media.ClosedCaptioning.ClosedCaptionProperties::get_FontEffect()
    @_property
    def font_effect(cls) -> ClosedCaptionEdgeEffect: ...
    # Windows.Media.ClosedCaptioning.ClosedCaptionOpacity Windows.Media.ClosedCaptioning.ClosedCaptionProperties::get_FontOpacity()
    @_property
    def font_opacity(cls) -> ClosedCaptionOpacity: ...
    # Windows.Media.ClosedCaptioning.ClosedCaptionSize Windows.Media.ClosedCaptioning.ClosedCaptionProperties::get_FontSize()
    @_property
    def font_size(cls) -> ClosedCaptionSize: ...
    # Windows.Media.ClosedCaptioning.ClosedCaptionStyle Windows.Media.ClosedCaptioning.ClosedCaptionProperties::get_FontStyle()
    @_property
    def font_style(cls) -> ClosedCaptionStyle: ...
    # Windows.Media.ClosedCaptioning.ClosedCaptionColor Windows.Media.ClosedCaptioning.ClosedCaptionProperties::get_RegionColor()
    @_property
    def region_color(cls) -> ClosedCaptionColor: ...
    # Windows.Media.ClosedCaptioning.ClosedCaptionOpacity Windows.Media.ClosedCaptioning.ClosedCaptionProperties::get_RegionOpacity()
    @_property
    def region_opacity(cls) -> ClosedCaptionOpacity: ...

@typing.final
class ClosedCaptionProperties(winrt.system.Object, metaclass=ClosedCaptionProperties_Static):
    pass

