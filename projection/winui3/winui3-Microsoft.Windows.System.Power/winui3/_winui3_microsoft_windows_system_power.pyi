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

from winui3.microsoft.windows.system.power import BatteryStatus, DisplayStatus, EffectivePowerMode, EnergySaverStatus, PowerSourceKind, PowerSupplyStatus, SystemSuspendStatus, UserPresenceStatus

Self = typing.TypeVar('Self')

@typing.final
class PowerManager_Static(winrt._winrt.IInspectable_Static):
    # Windows.Foundation.EventRegistrationToken Microsoft.Windows.System.Power.PowerManager::add_BatteryStatusChanged(Windows.Foundation.EventHandler`1<System.Object>)
    def add_battery_status_changed(cls, handler: windows_foundation.EventHandler[winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Microsoft.Windows.System.Power.PowerManager::remove_BatteryStatusChanged(Windows.Foundation.EventRegistrationToken)
    def remove_battery_status_changed(cls, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Microsoft.Windows.System.Power.PowerManager::add_DisplayStatusChanged(Windows.Foundation.EventHandler`1<System.Object>)
    def add_display_status_changed(cls, handler: windows_foundation.EventHandler[winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Microsoft.Windows.System.Power.PowerManager::remove_DisplayStatusChanged(Windows.Foundation.EventRegistrationToken)
    def remove_display_status_changed(cls, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Microsoft.Windows.System.Power.PowerManager::add_EffectivePowerModeChanged(Windows.Foundation.EventHandler`1<System.Object>)
    def add_effective_power_mode_changed(cls, handler: windows_foundation.EventHandler[winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Microsoft.Windows.System.Power.PowerManager::remove_EffectivePowerModeChanged(Windows.Foundation.EventRegistrationToken)
    def remove_effective_power_mode_changed(cls, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Microsoft.Windows.System.Power.PowerManager::add_EnergySaverStatusChanged(Windows.Foundation.EventHandler`1<System.Object>)
    def add_energy_saver_status_changed(cls, handler: windows_foundation.EventHandler[winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Microsoft.Windows.System.Power.PowerManager::remove_EnergySaverStatusChanged(Windows.Foundation.EventRegistrationToken)
    def remove_energy_saver_status_changed(cls, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Microsoft.Windows.System.Power.PowerManager::add_PowerSourceKindChanged(Windows.Foundation.EventHandler`1<System.Object>)
    def add_power_source_kind_changed(cls, handler: windows_foundation.EventHandler[winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Microsoft.Windows.System.Power.PowerManager::remove_PowerSourceKindChanged(Windows.Foundation.EventRegistrationToken)
    def remove_power_source_kind_changed(cls, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Microsoft.Windows.System.Power.PowerManager::add_PowerSupplyStatusChanged(Windows.Foundation.EventHandler`1<System.Object>)
    def add_power_supply_status_changed(cls, handler: windows_foundation.EventHandler[winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Microsoft.Windows.System.Power.PowerManager::remove_PowerSupplyStatusChanged(Windows.Foundation.EventRegistrationToken)
    def remove_power_supply_status_changed(cls, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Microsoft.Windows.System.Power.PowerManager::add_RemainingChargePercentChanged(Windows.Foundation.EventHandler`1<System.Object>)
    def add_remaining_charge_percent_changed(cls, handler: windows_foundation.EventHandler[winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Microsoft.Windows.System.Power.PowerManager::remove_RemainingChargePercentChanged(Windows.Foundation.EventRegistrationToken)
    def remove_remaining_charge_percent_changed(cls, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Microsoft.Windows.System.Power.PowerManager::add_RemainingDischargeTimeChanged(Windows.Foundation.EventHandler`1<System.Object>)
    def add_remaining_discharge_time_changed(cls, handler: windows_foundation.EventHandler[winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Microsoft.Windows.System.Power.PowerManager::remove_RemainingDischargeTimeChanged(Windows.Foundation.EventRegistrationToken)
    def remove_remaining_discharge_time_changed(cls, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Microsoft.Windows.System.Power.PowerManager::add_SystemIdleStatusChanged(Windows.Foundation.EventHandler`1<System.Object>)
    def add_system_idle_status_changed(cls, handler: windows_foundation.EventHandler[winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Microsoft.Windows.System.Power.PowerManager::remove_SystemIdleStatusChanged(Windows.Foundation.EventRegistrationToken)
    def remove_system_idle_status_changed(cls, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Microsoft.Windows.System.Power.PowerManager::add_SystemSuspendStatusChanged(Windows.Foundation.EventHandler`1<System.Object>)
    def add_system_suspend_status_changed(cls, handler: windows_foundation.EventHandler[winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Microsoft.Windows.System.Power.PowerManager::remove_SystemSuspendStatusChanged(Windows.Foundation.EventRegistrationToken)
    def remove_system_suspend_status_changed(cls, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Foundation.EventRegistrationToken Microsoft.Windows.System.Power.PowerManager::add_UserPresenceStatusChanged(Windows.Foundation.EventHandler`1<System.Object>)
    def add_user_presence_status_changed(cls, handler: windows_foundation.EventHandler[winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Microsoft.Windows.System.Power.PowerManager::remove_UserPresenceStatusChanged(Windows.Foundation.EventRegistrationToken)
    def remove_user_presence_status_changed(cls, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Microsoft.Windows.System.Power.BatteryStatus Microsoft.Windows.System.Power.PowerManager::get_BatteryStatus()
    @_property
    def battery_status(cls) -> BatteryStatus: ...
    # Microsoft.Windows.System.Power.DisplayStatus Microsoft.Windows.System.Power.PowerManager::get_DisplayStatus()
    @_property
    def display_status(cls) -> DisplayStatus: ...
    # Windows.Foundation.IAsyncOperation`1<Microsoft.Windows.System.Power.EffectivePowerMode> Microsoft.Windows.System.Power.PowerManager::get_EffectivePowerMode()
    @_property
    def effective_power_mode(cls) -> windows_foundation.IAsyncOperation[EffectivePowerMode]: ...
    # Microsoft.Windows.System.Power.EnergySaverStatus Microsoft.Windows.System.Power.PowerManager::get_EnergySaverStatus()
    @_property
    def energy_saver_status(cls) -> EnergySaverStatus: ...
    # Microsoft.Windows.System.Power.PowerSourceKind Microsoft.Windows.System.Power.PowerManager::get_PowerSourceKind()
    @_property
    def power_source_kind(cls) -> PowerSourceKind: ...
    # Microsoft.Windows.System.Power.PowerSupplyStatus Microsoft.Windows.System.Power.PowerManager::get_PowerSupplyStatus()
    @_property
    def power_supply_status(cls) -> PowerSupplyStatus: ...
    # System.Int32 Microsoft.Windows.System.Power.PowerManager::get_RemainingChargePercent()
    @_property
    def remaining_charge_percent(cls) -> winrt.system.Int32: ...
    # Windows.Foundation.TimeSpan Microsoft.Windows.System.Power.PowerManager::get_RemainingDischargeTime()
    @_property
    def remaining_discharge_time(cls) -> datetime.timedelta: ...
    # Microsoft.Windows.System.Power.SystemSuspendStatus Microsoft.Windows.System.Power.PowerManager::get_SystemSuspendStatus()
    @_property
    def system_suspend_status(cls) -> SystemSuspendStatus: ...
    # Microsoft.Windows.System.Power.UserPresenceStatus Microsoft.Windows.System.Power.PowerManager::get_UserPresenceStatus()
    @_property
    def user_presence_status(cls) -> UserPresenceStatus: ...
    # Microsoft.Windows.System.Power.EffectivePowerMode Microsoft.Windows.System.Power.PowerManager::get_EffectivePowerMode2()
    @_property
    def effective_power_mode2(cls) -> EffectivePowerMode: ...

@typing.final
class PowerManager(winrt.system.Object, metaclass=PowerManager_Static):
    pass

