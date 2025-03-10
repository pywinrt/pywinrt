# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.foundation.collections as windows_foundation_collections
import winrt.windows.gaming.input as windows_gaming_input
import winrt.windows.gaming.input.custom as windows_gaming_input_custom
import winrt.windows.system as windows_system

from winrt.windows.gaming.input.preview import DeviceCommand, GameControllerBatteryChargingState, GameControllerBatteryKind, GameControllerBatteryLevel, GameControllerFirmwareCorruptReason, HeadsetLevel, HeadsetOperation, RemappingButtonCategory

Self = typing.TypeVar('Self')

@typing.final
class HeadsetGeqGains:
    @_property
    def band1_gain(self) -> winrt.system.Int32: ...
    @_property
    def band2_gain(self) -> winrt.system.Int32: ...
    @_property
    def band3_gain(self) -> winrt.system.Int32: ...
    @_property
    def band4_gain(self) -> winrt.system.Int32: ...
    @_property
    def band5_gain(self) -> winrt.system.Int32: ...
    def __new__(cls, band1_gain: winrt.system.Int32 = 0, band2_gain: winrt.system.Int32 = 0, band3_gain: winrt.system.Int32 = 0, band4_gain: winrt.system.Int32 = 0, band5_gain: winrt.system.Int32 = 0) -> HeadsetGeqGains: ...
    def __replace__(self, /, **changes: typing.Any) -> HeadsetGeqGains: ...
    def unpack(self) -> typing.Tuple[winrt.system.Int32, winrt.system.Int32, winrt.system.Int32, winrt.system.Int32, winrt.system.Int32]: ...

@typing.final
class GameControllerProviderInfo_Static(winrt._winrt.IInspectable_Static):
    # System.String Windows.Gaming.Input.Preview.GameControllerProviderInfo::GetParentProviderId(Windows.Gaming.Input.Custom.IGameControllerProvider)
    def get_parent_provider_id(cls, provider: windows_gaming_input_custom.IGameControllerProvider, /) -> str: ...
    # System.String Windows.Gaming.Input.Preview.GameControllerProviderInfo::GetProviderId(Windows.Gaming.Input.Custom.IGameControllerProvider)
    def get_provider_id(cls, provider: windows_gaming_input_custom.IGameControllerProvider, /) -> str: ...

@typing.final
class GameControllerProviderInfo(winrt.system.Object, metaclass=GameControllerProviderInfo_Static):
    pass

@typing.final
class LegacyGipGameControllerProvider_Static(winrt._winrt.IInspectable_Static):
    # System.Void Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::ClearPairing(Windows.System.User,System.String)
    def clear_pairing(cls, user: windows_system.User, controller_provider_id: str, /) -> None: ...
    # Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::FromGameController(Windows.Gaming.Input.IGameController)
    def from_game_controller(cls, controller: windows_gaming_input.IGameController, /) -> LegacyGipGameControllerProvider: ...
    # Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::FromGameControllerProvider(Windows.Gaming.Input.Custom.IGameControllerProvider)
    def from_game_controller_provider(cls, provider: windows_gaming_input_custom.IGameControllerProvider, /) -> LegacyGipGameControllerProvider: ...
    # System.String Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::IsCopilot(Windows.System.User,System.String)
    def is_copilot(cls, user: windows_system.User, controller_provider_id: str, /) -> str: ...
    # System.String Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::IsPilot(Windows.System.User,System.String)
    def is_pilot(cls, user: windows_system.User, controller_provider_id: str, /) -> str: ...
    # System.Void Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::PairPilotToCopilot(Windows.System.User,System.String,System.String)
    def pair_pilot_to_copilot(cls, user: windows_system.User, pilot_controller_provider_id: str, copilot_controller_provider_id: str, /) -> None: ...

@typing.final
class LegacyGipGameControllerProvider(winrt.system.Object, metaclass=LegacyGipGameControllerProvider_Static):
    # System.Void Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::ExecuteCommand(Windows.Gaming.Input.Preview.DeviceCommand)
    def execute_command(self, command: DeviceCommand, /) -> None: ...
    # Windows.Gaming.Input.Preview.GameControllerFirmwareCorruptReason Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::GetDeviceFirmwareCorruptionState()
    def get_device_firmware_corruption_state(self) -> GameControllerFirmwareCorruptReason: ...
    # System.Byte[] Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::GetExtendedDeviceInfo()
    def get_extended_device_info(self) -> winrt.system.Array[winrt.system.UInt8]: ...
    # System.Byte[] Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::GetHeadsetOperation(Windows.Gaming.Input.Preview.HeadsetOperation)
    def get_headset_operation(self, operation: HeadsetOperation, /) -> winrt.system.Array[winrt.system.UInt8]: ...
    # Windows.Foundation.Collections.IMapView`2<Windows.Gaming.Input.Preview.RemappingButtonCategory,System.Object> Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::GetStandardControllerButtonRemapping(Windows.System.User,System.Boolean)
    def get_standard_controller_button_remapping(self, user: windows_system.User, previous: bool, /) -> typing.Mapping[RemappingButtonCategory, winrt.system.Object]: ...
    # System.Boolean Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::IsInterfaceSupported(System.Guid)
    def is_interface_supported(self, interface_id: _uuid.UUID, /) -> bool: ...
    # System.Void Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::SetHeadsetOperation(Windows.Gaming.Input.Preview.HeadsetOperation,System.Byte[])
    def set_headset_operation(self, operation: HeadsetOperation, buffer: typing.Union[winrt.system.Array[winrt.system.UInt8], winrt.system.ReadableBuffer], /) -> None: ...
    # System.Void Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::SetHomeLedIntensity(System.Byte)
    def set_home_led_intensity(self, intensity: winrt.system.UInt8, /) -> None: ...
    # System.Void Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::SetStandardControllerButtonRemapping(Windows.System.User,System.Boolean,Windows.Foundation.Collections.IMapView`2<Windows.Gaming.Input.Preview.RemappingButtonCategory,System.Object>)
    def set_standard_controller_button_remapping(self, user: windows_system.User, previous: bool, remapping: typing.Mapping[RemappingButtonCategory, winrt.system.Object], /) -> None: ...
    # System.UInt32 Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::get_AppCompatVersion()
    @_property
    def app_compat_version(self) -> winrt.system.UInt32: ...
    # Windows.Gaming.Input.Preview.GameControllerBatteryChargingState Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::get_BatteryChargingState()
    @_property
    def battery_charging_state(self) -> GameControllerBatteryChargingState: ...
    # Windows.Gaming.Input.Preview.GameControllerBatteryKind Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::get_BatteryKind()
    @_property
    def battery_kind(self) -> GameControllerBatteryKind: ...
    # Windows.Gaming.Input.Preview.GameControllerBatteryLevel Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::get_BatteryLevel()
    @_property
    def battery_level(self) -> GameControllerBatteryLevel: ...
    # System.Boolean Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::get_IsFirmwareCorrupted()
    @_property
    def is_firmware_corrupted(self) -> bool: ...
    # System.Boolean Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::get_IsSyntheticDevice()
    @_property
    def is_synthetic_device(self) -> bool: ...
    # Windows.Foundation.Collections.IVectorView`1<System.String> Windows.Gaming.Input.Preview.LegacyGipGameControllerProvider::get_PreferredTypes()
    @_property
    def preferred_types(self) -> typing.Sequence[str]: ...

