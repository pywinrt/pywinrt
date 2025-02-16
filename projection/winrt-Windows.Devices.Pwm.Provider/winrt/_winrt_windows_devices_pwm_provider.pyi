# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property
from abc import abstractmethod

import winrt._winrt
import winrt.system
import winrt.windows.foundation.collections as windows_foundation_collections

Self = typing.TypeVar('Self')

@typing.final
class _IPwmControllerProvider: ...

class IPwmControllerProvider(winrt._winrt.IInspectable):
    # System.Void Windows.Devices.Pwm.Provider.IPwmControllerProvider::AcquirePin(System.Int32)
    @abstractmethod
    def acquire_pin(self, pin: winrt.system.Int32, /) -> None: ...
    # System.Void Windows.Devices.Pwm.Provider.IPwmControllerProvider::DisablePin(System.Int32)
    @abstractmethod
    def disable_pin(self, pin: winrt.system.Int32, /) -> None: ...
    # System.Void Windows.Devices.Pwm.Provider.IPwmControllerProvider::EnablePin(System.Int32)
    @abstractmethod
    def enable_pin(self, pin: winrt.system.Int32, /) -> None: ...
    # System.Void Windows.Devices.Pwm.Provider.IPwmControllerProvider::ReleasePin(System.Int32)
    @abstractmethod
    def release_pin(self, pin: winrt.system.Int32, /) -> None: ...
    # System.Double Windows.Devices.Pwm.Provider.IPwmControllerProvider::SetDesiredFrequency(System.Double)
    @abstractmethod
    def set_desired_frequency(self, frequency: winrt.system.Double, /) -> winrt.system.Double: ...
    # System.Void Windows.Devices.Pwm.Provider.IPwmControllerProvider::SetPulseParameters(System.Int32,System.Double,System.Boolean)
    @abstractmethod
    def set_pulse_parameters(self, pin: winrt.system.Int32, duty_cycle: winrt.system.Double, invert_polarity: bool, /) -> None: ...
    # System.Double Windows.Devices.Pwm.Provider.IPwmControllerProvider::get_ActualFrequency()
    @_property
    @abstractmethod
    def actual_frequency(self) -> winrt.system.Double: ...
    # System.Double Windows.Devices.Pwm.Provider.IPwmControllerProvider::get_MaxFrequency()
    @_property
    @abstractmethod
    def max_frequency(self) -> winrt.system.Double: ...
    # System.Double Windows.Devices.Pwm.Provider.IPwmControllerProvider::get_MinFrequency()
    @_property
    @abstractmethod
    def min_frequency(self) -> winrt.system.Double: ...
    # System.Int32 Windows.Devices.Pwm.Provider.IPwmControllerProvider::get_PinCount()
    @_property
    @abstractmethod
    def pin_count(self) -> winrt.system.Int32: ...

@typing.final
class _IPwmProvider: ...

class IPwmProvider(winrt._winrt.IInspectable):
    # Windows.Foundation.Collections.IVectorView`1<Windows.Devices.Pwm.Provider.IPwmControllerProvider> Windows.Devices.Pwm.Provider.IPwmProvider::GetControllers()
    @abstractmethod
    def get_controllers(self) -> typing.Sequence[IPwmControllerProvider]: ...

