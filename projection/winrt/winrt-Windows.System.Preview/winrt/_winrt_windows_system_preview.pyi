# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.devices.sensors as windows_devices_sensors
import winrt.windows.foundation as windows_foundation

from winrt.windows.system.preview import HingeState

Self = typing.TypeVar('Self')

@typing.final
class TwoPanelHingedDevicePosturePreview_Static(winrt._winrt.IInspectable_Static):
    # Windows.Foundation.IAsyncOperation`1<Windows.System.Preview.TwoPanelHingedDevicePosturePreview> Windows.System.Preview.TwoPanelHingedDevicePosturePreview::GetDefaultAsync()
    # @deprecated("TwoPanelHingedDevicePosturePreview is deprecated and might not work on all platforms. For more info, see MSDN.")
    def get_default_async(cls) -> windows_foundation.IAsyncOperation[TwoPanelHingedDevicePosturePreview]: ...

@typing.final
class TwoPanelHingedDevicePosturePreview(winrt.system.Object, metaclass=TwoPanelHingedDevicePosturePreview_Static):
    # Windows.Foundation.IAsyncOperation`1<Windows.System.Preview.TwoPanelHingedDevicePosturePreviewReading> Windows.System.Preview.TwoPanelHingedDevicePosturePreview::GetCurrentPostureAsync()
    # @deprecated("TwoPanelHingedDevicePosturePreview is deprecated and might not work on all platforms. For more info, see MSDN.")
    def get_current_posture_async(self) -> windows_foundation.IAsyncOperation[TwoPanelHingedDevicePosturePreviewReading]: ...
    # Windows.Foundation.EventRegistrationToken Windows.System.Preview.TwoPanelHingedDevicePosturePreview::add_PostureChanged(Windows.Foundation.TypedEventHandler`2<Windows.System.Preview.TwoPanelHingedDevicePosturePreview,Windows.System.Preview.TwoPanelHingedDevicePosturePreviewReadingChangedEventArgs>)
    # @deprecated("TwoPanelHingedDevicePosturePreview is deprecated and might not work on all platforms. For more info, see MSDN.")
    def add_posture_changed(self, handler: windows_foundation.TypedEventHandler[TwoPanelHingedDevicePosturePreview, TwoPanelHingedDevicePosturePreviewReadingChangedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.System.Preview.TwoPanelHingedDevicePosturePreview::remove_PostureChanged(Windows.Foundation.EventRegistrationToken)
    # @deprecated("TwoPanelHingedDevicePosturePreview is deprecated and might not work on all platforms. For more info, see MSDN.")
    def remove_posture_changed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...

@typing.final
class TwoPanelHingedDevicePosturePreviewReading(winrt.system.Object):
    # Windows.System.Preview.HingeState Windows.System.Preview.TwoPanelHingedDevicePosturePreviewReading::get_HingeState()
    @_property
    def hinge_state(self) -> HingeState: ...
    # System.String Windows.System.Preview.TwoPanelHingedDevicePosturePreviewReading::get_Panel1Id()
    @_property
    def panel1_id(self) -> str: ...
    # Windows.Devices.Sensors.SimpleOrientation Windows.System.Preview.TwoPanelHingedDevicePosturePreviewReading::get_Panel1Orientation()
    @_property
    def panel1_orientation(self) -> windows_devices_sensors.SimpleOrientation: ...
    # System.String Windows.System.Preview.TwoPanelHingedDevicePosturePreviewReading::get_Panel2Id()
    @_property
    def panel2_id(self) -> str: ...
    # Windows.Devices.Sensors.SimpleOrientation Windows.System.Preview.TwoPanelHingedDevicePosturePreviewReading::get_Panel2Orientation()
    @_property
    def panel2_orientation(self) -> windows_devices_sensors.SimpleOrientation: ...
    # Windows.Foundation.DateTime Windows.System.Preview.TwoPanelHingedDevicePosturePreviewReading::get_Timestamp()
    @_property
    def timestamp(self) -> datetime.datetime: ...

@typing.final
class TwoPanelHingedDevicePosturePreviewReadingChangedEventArgs(winrt.system.Object):
    # Windows.System.Preview.TwoPanelHingedDevicePosturePreviewReading Windows.System.Preview.TwoPanelHingedDevicePosturePreviewReadingChangedEventArgs::get_Reading()
    @_property
    def reading(self) -> TwoPanelHingedDevicePosturePreviewReading: ...

