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
import winrt.windows.foundation
import winrt.windows.foundation.collections

from winrt.windows.devices.input import PointerDeviceType

Self = typing.TypeVar('Self')

class MouseDelta:
    x: winrt.system.Int32
    y: winrt.system.Int32
    def __init__(self, x: winrt.system.Int32, y: winrt.system.Int32) -> None: ...

class PointerDeviceUsage:
    usage_page: winrt.system.UInt32
    usage: winrt.system.UInt32
    min_logical: winrt.system.Int32
    max_logical: winrt.system.Int32
    min_physical: winrt.system.Int32
    max_physical: winrt.system.Int32
    unit: winrt.system.UInt32
    physical_multiplier: winrt.system.Single
    def __init__(self, usage_page: winrt.system.UInt32, usage: winrt.system.UInt32, min_logical: winrt.system.Int32, max_logical: winrt.system.Int32, min_physical: winrt.system.Int32, max_physical: winrt.system.Int32, unit: winrt.system.UInt32, physical_multiplier: winrt.system.Single) -> None: ...

class KeyboardCapabilities(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> KeyboardCapabilities: ...
    def __new__(cls: typing.Type[KeyboardCapabilities]) -> KeyboardCapabilities:...
    @_property
    def keyboard_present(self) -> winrt.system.Int32: ...

class MouseCapabilities(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> MouseCapabilities: ...
    def __new__(cls: typing.Type[MouseCapabilities]) -> MouseCapabilities:...
    @_property
    def horizontal_wheel_present(self) -> winrt.system.Int32: ...
    @_property
    def mouse_present(self) -> winrt.system.Int32: ...
    @_property
    def number_of_buttons(self) -> winrt.system.UInt32: ...
    @_property
    def swap_buttons(self) -> winrt.system.Int32: ...
    @_property
    def vertical_wheel_present(self) -> winrt.system.Int32: ...

class MouseDevice_Static(type):
    def get_for_current_view(cls) -> typing.Optional[MouseDevice]: ...

class MouseDevice(winrt.system.Object, metaclass=MouseDevice_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> MouseDevice: ...
    def add_mouse_moved(self, handler: winrt.windows.foundation.TypedEventHandler[MouseDevice, MouseEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_mouse_moved(self, cookie: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class MouseEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> MouseEventArgs: ...
    @_property
    def mouse_delta(self) -> MouseDelta: ...

class PenButtonListener_Static(type):
    def get_default(cls) -> typing.Optional[PenButtonListener]: ...

class PenButtonListener(winrt.system.Object, metaclass=PenButtonListener_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> PenButtonListener: ...
    def is_supported(self) -> bool: ...
    def add_is_supported_changed(self, handler: winrt.windows.foundation.TypedEventHandler[PenButtonListener, winrt.system.Object], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_is_supported_changed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_tail_button_clicked(self, handler: winrt.windows.foundation.TypedEventHandler[PenButtonListener, PenTailButtonClickedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_tail_button_clicked(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_tail_button_double_clicked(self, handler: winrt.windows.foundation.TypedEventHandler[PenButtonListener, PenTailButtonDoubleClickedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_tail_button_double_clicked(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_tail_button_long_pressed(self, handler: winrt.windows.foundation.TypedEventHandler[PenButtonListener, PenTailButtonLongPressedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_tail_button_long_pressed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class PenDevice_Static(type):
    def get_from_pointer_id(cls, pointer_id: winrt.system.UInt32, /) -> typing.Optional[PenDevice]: ...

class PenDevice(winrt.system.Object, metaclass=PenDevice_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> PenDevice: ...
    @_property
    def pen_id(self) -> _uuid.UUID: ...
    @_property
    def simple_haptics_controller(self) -> typing.Optional[winrt.windows.devices.haptics.SimpleHapticsController]: ...

class PenDockListener_Static(type):
    def get_default(cls) -> typing.Optional[PenDockListener]: ...

class PenDockListener(winrt.system.Object, metaclass=PenDockListener_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> PenDockListener: ...
    def is_supported(self) -> bool: ...
    def add_docked(self, handler: winrt.windows.foundation.TypedEventHandler[PenDockListener, PenDockedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_docked(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_is_supported_changed(self, handler: winrt.windows.foundation.TypedEventHandler[PenDockListener, winrt.system.Object], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_is_supported_changed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_undocked(self, handler: winrt.windows.foundation.TypedEventHandler[PenDockListener, PenUndockedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_undocked(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class PenDockedEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> PenDockedEventArgs: ...

class PenTailButtonClickedEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> PenTailButtonClickedEventArgs: ...

class PenTailButtonDoubleClickedEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> PenTailButtonDoubleClickedEventArgs: ...

class PenTailButtonLongPressedEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> PenTailButtonLongPressedEventArgs: ...

class PenUndockedEventArgs(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> PenUndockedEventArgs: ...

class PointerDevice_Static(type):
    def get_pointer_device(cls, pointer_id: winrt.system.UInt32, /) -> typing.Optional[PointerDevice]: ...
    def get_pointer_devices(cls) -> typing.Optional[winrt.windows.foundation.collections.IVectorView[PointerDevice]]: ...

class PointerDevice(winrt.system.Object, metaclass=PointerDevice_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> PointerDevice: ...
    @_property
    def is_integrated(self) -> bool: ...
    @_property
    def max_contacts(self) -> winrt.system.UInt32: ...
    @_property
    def physical_device_rect(self) -> winrt.windows.foundation.Rect: ...
    @_property
    def pointer_device_type(self) -> PointerDeviceType: ...
    @_property
    def screen_rect(self) -> winrt.windows.foundation.Rect: ...
    @_property
    def supported_usages(self) -> typing.Optional[winrt.windows.foundation.collections.IVectorView[PointerDeviceUsage]]: ...
    @_property
    def max_pointers_with_z_distance(self) -> winrt.system.UInt32: ...

class TouchCapabilities(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> TouchCapabilities: ...
    def __new__(cls: typing.Type[TouchCapabilities]) -> TouchCapabilities:...
    @_property
    def contacts(self) -> winrt.system.UInt32: ...
    @_property
    def touch_present(self) -> winrt.system.Int32: ...
