# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.devices.haptics
import winrt.windows.devices.power
import winrt.windows.foundation
import winrt.windows.foundation.collections
import winrt.windows.gaming.input.forcefeedback
import winrt.windows.system

from winrt.windows.gaming.input import ArcadeStickButtons, FlightStickButtons, GameControllerButtonLabel, GameControllerSwitchKind, GameControllerSwitchPosition, GamepadButtons, OptionalUINavigationButtons, RacingWheelButtons, RequiredUINavigationButtons

Self = typing.TypeVar('Self')

class ArcadeStickReading:
    timestamp: winrt.system.UInt64
    buttons: ArcadeStickButtons
    def __init__(self, timestamp: winrt.system.UInt64, buttons: ArcadeStickButtons) -> None: ...

class FlightStickReading:
    timestamp: winrt.system.UInt64
    buttons: FlightStickButtons
    hat_switch: GameControllerSwitchPosition
    roll: winrt.system.Double
    pitch: winrt.system.Double
    yaw: winrt.system.Double
    throttle: winrt.system.Double
    def __init__(self, timestamp: winrt.system.UInt64, buttons: FlightStickButtons, hat_switch: GameControllerSwitchPosition, roll: winrt.system.Double, pitch: winrt.system.Double, yaw: winrt.system.Double, throttle: winrt.system.Double) -> None: ...

class GamepadReading:
    timestamp: winrt.system.UInt64
    buttons: GamepadButtons
    left_trigger: winrt.system.Double
    right_trigger: winrt.system.Double
    left_thumbstick_x: winrt.system.Double
    left_thumbstick_y: winrt.system.Double
    right_thumbstick_x: winrt.system.Double
    right_thumbstick_y: winrt.system.Double
    def __init__(self, timestamp: winrt.system.UInt64, buttons: GamepadButtons, left_trigger: winrt.system.Double, right_trigger: winrt.system.Double, left_thumbstick_x: winrt.system.Double, left_thumbstick_y: winrt.system.Double, right_thumbstick_x: winrt.system.Double, right_thumbstick_y: winrt.system.Double) -> None: ...

class GamepadVibration:
    left_motor: winrt.system.Double
    right_motor: winrt.system.Double
    left_trigger: winrt.system.Double
    right_trigger: winrt.system.Double
    def __init__(self, left_motor: winrt.system.Double, right_motor: winrt.system.Double, left_trigger: winrt.system.Double, right_trigger: winrt.system.Double) -> None: ...

class RacingWheelReading:
    timestamp: winrt.system.UInt64
    buttons: RacingWheelButtons
    pattern_shifter_gear: winrt.system.Int32
    wheel: winrt.system.Double
    throttle: winrt.system.Double
    brake: winrt.system.Double
    clutch: winrt.system.Double
    handbrake: winrt.system.Double
    def __init__(self, timestamp: winrt.system.UInt64, buttons: RacingWheelButtons, pattern_shifter_gear: winrt.system.Int32, wheel: winrt.system.Double, throttle: winrt.system.Double, brake: winrt.system.Double, clutch: winrt.system.Double, handbrake: winrt.system.Double) -> None: ...

class UINavigationReading:
    timestamp: winrt.system.UInt64
    required_buttons: RequiredUINavigationButtons
    optional_buttons: OptionalUINavigationButtons
    def __init__(self, timestamp: winrt.system.UInt64, required_buttons: RequiredUINavigationButtons, optional_buttons: OptionalUINavigationButtons) -> None: ...

class ArcadeStick_Static(type):
    def from_game_controller(cls, game_controller: typing.Optional[IGameController], /) -> typing.Optional[ArcadeStick]: ...
    def add_arcade_stick_added(cls, value: winrt.windows.foundation.EventHandler[ArcadeStick], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_arcade_stick_added(cls, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_arcade_stick_removed(cls, value: winrt.windows.foundation.EventHandler[ArcadeStick], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_arcade_stick_removed(cls, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def arcade_sticks(cls) -> typing.Optional[winrt.windows.foundation.collections.IVectorView[ArcadeStick]]: ...

class ArcadeStick(winrt.system.Object, metaclass=ArcadeStick_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ArcadeStick: ...
    def get_button_label(self, button: ArcadeStickButtons, /) -> GameControllerButtonLabel: ...
    def get_current_reading(self) -> ArcadeStickReading: ...
    def try_get_battery_report(self) -> typing.Optional[winrt.windows.devices.power.BatteryReport]: ...
    def add_headset_connected(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, Headset], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_headset_connected(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_headset_disconnected(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, Headset], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_headset_disconnected(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_user_changed(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, winrt.windows.system.UserChangedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_user_changed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def headset(self) -> typing.Optional[Headset]: ...
    @_property
    def is_wireless(self) -> bool: ...
    @_property
    def user(self) -> typing.Optional[winrt.windows.system.User]: ...

class FlightStick_Static(type):
    def from_game_controller(cls, game_controller: typing.Optional[IGameController], /) -> typing.Optional[FlightStick]: ...
    def add_flight_stick_added(cls, value: winrt.windows.foundation.EventHandler[FlightStick], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_flight_stick_added(cls, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_flight_stick_removed(cls, value: winrt.windows.foundation.EventHandler[FlightStick], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_flight_stick_removed(cls, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def flight_sticks(cls) -> typing.Optional[winrt.windows.foundation.collections.IVectorView[FlightStick]]: ...

class FlightStick(winrt.system.Object, metaclass=FlightStick_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> FlightStick: ...
    def get_button_label(self, button: FlightStickButtons, /) -> GameControllerButtonLabel: ...
    def get_current_reading(self) -> FlightStickReading: ...
    def try_get_battery_report(self) -> typing.Optional[winrt.windows.devices.power.BatteryReport]: ...
    def add_headset_connected(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, Headset], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_headset_connected(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_headset_disconnected(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, Headset], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_headset_disconnected(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_user_changed(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, winrt.windows.system.UserChangedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_user_changed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def hat_switch_kind(self) -> GameControllerSwitchKind: ...
    @_property
    def headset(self) -> typing.Optional[Headset]: ...
    @_property
    def is_wireless(self) -> bool: ...
    @_property
    def user(self) -> typing.Optional[winrt.windows.system.User]: ...

class Gamepad_Static(type):
    def from_game_controller(cls, game_controller: typing.Optional[IGameController], /) -> typing.Optional[Gamepad]: ...
    def add_gamepad_added(cls, value: winrt.windows.foundation.EventHandler[Gamepad], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_gamepad_added(cls, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_gamepad_removed(cls, value: winrt.windows.foundation.EventHandler[Gamepad], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_gamepad_removed(cls, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def gamepads(cls) -> typing.Optional[winrt.windows.foundation.collections.IVectorView[Gamepad]]: ...

class Gamepad(winrt.system.Object, metaclass=Gamepad_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> Gamepad: ...
    def get_button_label(self, button: GamepadButtons, /) -> GameControllerButtonLabel: ...
    def get_current_reading(self) -> GamepadReading: ...
    def try_get_battery_report(self) -> typing.Optional[winrt.windows.devices.power.BatteryReport]: ...
    def add_headset_connected(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, Headset], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_headset_connected(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_headset_disconnected(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, Headset], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_headset_disconnected(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_user_changed(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, winrt.windows.system.UserChangedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_user_changed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def headset(self) -> typing.Optional[Headset]: ...
    @_property
    def is_wireless(self) -> bool: ...
    @_property
    def user(self) -> typing.Optional[winrt.windows.system.User]: ...
    @_property
    def vibration(self) -> GamepadVibration: ...
    @vibration.setter
    def vibration(self, value: GamepadVibration) -> None: ...

class Headset(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> Headset: ...
    def try_get_battery_report(self) -> typing.Optional[winrt.windows.devices.power.BatteryReport]: ...
    @_property
    def capture_device_id(self) -> str: ...
    @_property
    def render_device_id(self) -> str: ...

class RacingWheel_Static(type):
    def from_game_controller(cls, game_controller: typing.Optional[IGameController], /) -> typing.Optional[RacingWheel]: ...
    def add_racing_wheel_added(cls, value: winrt.windows.foundation.EventHandler[RacingWheel], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_racing_wheel_added(cls, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_racing_wheel_removed(cls, value: winrt.windows.foundation.EventHandler[RacingWheel], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_racing_wheel_removed(cls, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def racing_wheels(cls) -> typing.Optional[winrt.windows.foundation.collections.IVectorView[RacingWheel]]: ...

class RacingWheel(winrt.system.Object, metaclass=RacingWheel_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> RacingWheel: ...
    def get_button_label(self, button: RacingWheelButtons, /) -> GameControllerButtonLabel: ...
    def get_current_reading(self) -> RacingWheelReading: ...
    def try_get_battery_report(self) -> typing.Optional[winrt.windows.devices.power.BatteryReport]: ...
    def add_headset_connected(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, Headset], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_headset_connected(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_headset_disconnected(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, Headset], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_headset_disconnected(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_user_changed(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, winrt.windows.system.UserChangedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_user_changed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def headset(self) -> typing.Optional[Headset]: ...
    @_property
    def is_wireless(self) -> bool: ...
    @_property
    def user(self) -> typing.Optional[winrt.windows.system.User]: ...
    @_property
    def has_clutch(self) -> bool: ...
    @_property
    def has_handbrake(self) -> bool: ...
    @_property
    def has_pattern_shifter(self) -> bool: ...
    @_property
    def max_pattern_shifter_gear(self) -> winrt.system.Int32: ...
    @_property
    def max_wheel_angle(self) -> winrt.system.Double: ...
    @_property
    def wheel_motor(self) -> typing.Optional[winrt.windows.gaming.input.forcefeedback.ForceFeedbackMotor]: ...

class RawGameController_Static(type):
    def from_game_controller(cls, game_controller: typing.Optional[IGameController], /) -> typing.Optional[RawGameController]: ...
    def add_raw_game_controller_added(cls, value: winrt.windows.foundation.EventHandler[RawGameController], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_raw_game_controller_added(cls, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_raw_game_controller_removed(cls, value: winrt.windows.foundation.EventHandler[RawGameController], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_raw_game_controller_removed(cls, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def raw_game_controllers(cls) -> typing.Optional[winrt.windows.foundation.collections.IVectorView[RawGameController]]: ...

class RawGameController(winrt.system.Object, metaclass=RawGameController_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> RawGameController: ...
    def get_button_label(self, button_index: winrt.system.Int32, /) -> GameControllerButtonLabel: ...
    def get_current_reading(self, button_array: winrt.system.Array[bool], switch_array: winrt.system.Array[GameControllerSwitchPosition], axis_array: winrt.system.Array[winrt.system.Double], /) -> winrt.system.UInt64: ...
    def get_switch_kind(self, switch_index: winrt.system.Int32, /) -> GameControllerSwitchKind: ...
    def try_get_battery_report(self) -> typing.Optional[winrt.windows.devices.power.BatteryReport]: ...
    def add_headset_connected(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, Headset], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_headset_connected(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_headset_disconnected(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, Headset], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_headset_disconnected(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_user_changed(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, winrt.windows.system.UserChangedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_user_changed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def headset(self) -> typing.Optional[Headset]: ...
    @_property
    def is_wireless(self) -> bool: ...
    @_property
    def user(self) -> typing.Optional[winrt.windows.system.User]: ...
    @_property
    def axis_count(self) -> winrt.system.Int32: ...
    @_property
    def button_count(self) -> winrt.system.Int32: ...
    @_property
    def force_feedback_motors(self) -> typing.Optional[winrt.windows.foundation.collections.IVectorView[winrt.windows.gaming.input.forcefeedback.ForceFeedbackMotor]]: ...
    @_property
    def hardware_product_id(self) -> winrt.system.UInt16: ...
    @_property
    def hardware_vendor_id(self) -> winrt.system.UInt16: ...
    @_property
    def switch_count(self) -> winrt.system.Int32: ...
    @_property
    def display_name(self) -> str: ...
    @_property
    def non_roamable_id(self) -> str: ...
    @_property
    def simple_haptics_controllers(self) -> typing.Optional[winrt.windows.foundation.collections.IVectorView[winrt.windows.devices.haptics.SimpleHapticsController]]: ...

class UINavigationController_Static(type):
    def from_game_controller(cls, game_controller: typing.Optional[IGameController], /) -> typing.Optional[UINavigationController]: ...
    def add_u_i_navigation_controller_added(cls, value: winrt.windows.foundation.EventHandler[UINavigationController], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_u_i_navigation_controller_added(cls, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_u_i_navigation_controller_removed(cls, value: winrt.windows.foundation.EventHandler[UINavigationController], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_u_i_navigation_controller_removed(cls, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def u_i_navigation_controllers(cls) -> typing.Optional[winrt.windows.foundation.collections.IVectorView[UINavigationController]]: ...

class UINavigationController(winrt.system.Object, metaclass=UINavigationController_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> UINavigationController: ...
    def get_current_reading(self) -> UINavigationReading: ...
    def get_optional_button_label(self, button: OptionalUINavigationButtons, /) -> GameControllerButtonLabel: ...
    def get_required_button_label(self, button: RequiredUINavigationButtons, /) -> GameControllerButtonLabel: ...
    def try_get_battery_report(self) -> typing.Optional[winrt.windows.devices.power.BatteryReport]: ...
    def add_headset_connected(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, Headset], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_headset_connected(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_headset_disconnected(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, Headset], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_headset_disconnected(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_user_changed(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, winrt.windows.system.UserChangedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_user_changed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def headset(self) -> typing.Optional[Headset]: ...
    @_property
    def is_wireless(self) -> bool: ...
    @_property
    def user(self) -> typing.Optional[winrt.windows.system.User]: ...

class IGameController(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> IGameController: ...
    def add_headset_connected(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, Headset], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_headset_connected(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_headset_disconnected(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, Headset], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_headset_disconnected(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_user_changed(self, value: winrt.windows.foundation.TypedEventHandler[IGameController, winrt.windows.system.UserChangedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_user_changed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def headset(self) -> typing.Optional[Headset]: ...
    @_property
    def is_wireless(self) -> bool: ...
    @_property
    def user(self) -> typing.Optional[winrt.windows.system.User]: ...

class IGameControllerBatteryInfo(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> IGameControllerBatteryInfo: ...
    def try_get_battery_report(self) -> typing.Optional[winrt.windows.devices.power.BatteryReport]: ...
