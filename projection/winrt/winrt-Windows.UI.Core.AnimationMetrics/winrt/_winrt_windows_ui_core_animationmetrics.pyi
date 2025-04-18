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
import winrt.windows.foundation as windows_foundation
import winrt.windows.foundation.collections as windows_foundation_collections

from winrt.windows.ui.core.animationmetrics import AnimationEffect, AnimationEffectTarget, PropertyAnimationType

Self = typing.TypeVar('Self')

@typing.final
class AnimationDescription(winrt.system.Object):
    def __new__(cls: typing.Type[Self], effect: AnimationEffect, target: AnimationEffectTarget) -> Self: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.UI.Core.AnimationMetrics.IPropertyAnimation> Windows.UI.Core.AnimationMetrics.AnimationDescription::get_Animations()
    @_property
    def animations(self) -> typing.Sequence[IPropertyAnimation]: ...
    # Windows.Foundation.TimeSpan Windows.UI.Core.AnimationMetrics.AnimationDescription::get_DelayLimit()
    @_property
    def delay_limit(self) -> datetime.timedelta: ...
    # Windows.Foundation.TimeSpan Windows.UI.Core.AnimationMetrics.AnimationDescription::get_StaggerDelay()
    @_property
    def stagger_delay(self) -> datetime.timedelta: ...
    # System.Single Windows.UI.Core.AnimationMetrics.AnimationDescription::get_StaggerDelayFactor()
    @_property
    def stagger_delay_factor(self) -> winrt.system.Single: ...
    # System.Int32 Windows.UI.Core.AnimationMetrics.AnimationDescription::get_ZOrder()
    @_property
    def z_order(self) -> winrt.system.Int32: ...

@typing.final
class OpacityAnimation(winrt.system.Object, IPropertyAnimation):
    # System.Single Windows.UI.Core.AnimationMetrics.OpacityAnimation::get_FinalOpacity()
    @_property
    def final_opacity(self) -> winrt.system.Single: ...
    # Windows.Foundation.IReference`1<System.Single> Windows.UI.Core.AnimationMetrics.OpacityAnimation::get_InitialOpacity()
    @_property
    def initial_opacity(self) -> typing.Optional[winrt.system.Single]: ...
    # Windows.Foundation.Point Windows.UI.Core.AnimationMetrics.OpacityAnimation::get_Control1()
    @_property
    def control1(self) -> windows_foundation.Point: ...
    # Windows.Foundation.Point Windows.UI.Core.AnimationMetrics.OpacityAnimation::get_Control2()
    @_property
    def control2(self) -> windows_foundation.Point: ...
    # Windows.Foundation.TimeSpan Windows.UI.Core.AnimationMetrics.OpacityAnimation::get_Delay()
    @_property
    def delay(self) -> datetime.timedelta: ...
    # Windows.Foundation.TimeSpan Windows.UI.Core.AnimationMetrics.OpacityAnimation::get_Duration()
    @_property
    def duration(self) -> datetime.timedelta: ...
    # Windows.UI.Core.AnimationMetrics.PropertyAnimationType Windows.UI.Core.AnimationMetrics.OpacityAnimation::get_Type()
    @_property
    def type(self) -> PropertyAnimationType: ...

@typing.final
class PropertyAnimation(winrt.system.Object, IPropertyAnimation):
    # Windows.Foundation.Point Windows.UI.Core.AnimationMetrics.PropertyAnimation::get_Control1()
    @_property
    def control1(self) -> windows_foundation.Point: ...
    # Windows.Foundation.Point Windows.UI.Core.AnimationMetrics.PropertyAnimation::get_Control2()
    @_property
    def control2(self) -> windows_foundation.Point: ...
    # Windows.Foundation.TimeSpan Windows.UI.Core.AnimationMetrics.PropertyAnimation::get_Delay()
    @_property
    def delay(self) -> datetime.timedelta: ...
    # Windows.Foundation.TimeSpan Windows.UI.Core.AnimationMetrics.PropertyAnimation::get_Duration()
    @_property
    def duration(self) -> datetime.timedelta: ...
    # Windows.UI.Core.AnimationMetrics.PropertyAnimationType Windows.UI.Core.AnimationMetrics.PropertyAnimation::get_Type()
    @_property
    def type(self) -> PropertyAnimationType: ...

@typing.final
class ScaleAnimation(winrt.system.Object, IPropertyAnimation):
    # Windows.Foundation.Point Windows.UI.Core.AnimationMetrics.ScaleAnimation::get_Control1()
    @_property
    def control1(self) -> windows_foundation.Point: ...
    # Windows.Foundation.Point Windows.UI.Core.AnimationMetrics.ScaleAnimation::get_Control2()
    @_property
    def control2(self) -> windows_foundation.Point: ...
    # Windows.Foundation.TimeSpan Windows.UI.Core.AnimationMetrics.ScaleAnimation::get_Delay()
    @_property
    def delay(self) -> datetime.timedelta: ...
    # Windows.Foundation.TimeSpan Windows.UI.Core.AnimationMetrics.ScaleAnimation::get_Duration()
    @_property
    def duration(self) -> datetime.timedelta: ...
    # Windows.UI.Core.AnimationMetrics.PropertyAnimationType Windows.UI.Core.AnimationMetrics.ScaleAnimation::get_Type()
    @_property
    def type(self) -> PropertyAnimationType: ...
    # System.Single Windows.UI.Core.AnimationMetrics.ScaleAnimation::get_FinalScaleX()
    @_property
    def final_scale_x(self) -> winrt.system.Single: ...
    # System.Single Windows.UI.Core.AnimationMetrics.ScaleAnimation::get_FinalScaleY()
    @_property
    def final_scale_y(self) -> winrt.system.Single: ...
    # Windows.Foundation.IReference`1<System.Single> Windows.UI.Core.AnimationMetrics.ScaleAnimation::get_InitialScaleX()
    @_property
    def initial_scale_x(self) -> typing.Optional[winrt.system.Single]: ...
    # Windows.Foundation.IReference`1<System.Single> Windows.UI.Core.AnimationMetrics.ScaleAnimation::get_InitialScaleY()
    @_property
    def initial_scale_y(self) -> typing.Optional[winrt.system.Single]: ...
    # Windows.Foundation.Point Windows.UI.Core.AnimationMetrics.ScaleAnimation::get_NormalizedOrigin()
    @_property
    def normalized_origin(self) -> windows_foundation.Point: ...

@typing.final
class TranslationAnimation(winrt.system.Object, IPropertyAnimation):
    # Windows.Foundation.Point Windows.UI.Core.AnimationMetrics.TranslationAnimation::get_Control1()
    @_property
    def control1(self) -> windows_foundation.Point: ...
    # Windows.Foundation.Point Windows.UI.Core.AnimationMetrics.TranslationAnimation::get_Control2()
    @_property
    def control2(self) -> windows_foundation.Point: ...
    # Windows.Foundation.TimeSpan Windows.UI.Core.AnimationMetrics.TranslationAnimation::get_Delay()
    @_property
    def delay(self) -> datetime.timedelta: ...
    # Windows.Foundation.TimeSpan Windows.UI.Core.AnimationMetrics.TranslationAnimation::get_Duration()
    @_property
    def duration(self) -> datetime.timedelta: ...
    # Windows.UI.Core.AnimationMetrics.PropertyAnimationType Windows.UI.Core.AnimationMetrics.TranslationAnimation::get_Type()
    @_property
    def type(self) -> PropertyAnimationType: ...

@typing.final
class _IPropertyAnimation: ...

class IPropertyAnimation(winrt._winrt.IInspectable):
    # Windows.Foundation.Point Windows.UI.Core.AnimationMetrics.IPropertyAnimation::get_Control1()
    @_property
    @abstractmethod
    def control1(self) -> windows_foundation.Point: ...
    # Windows.Foundation.Point Windows.UI.Core.AnimationMetrics.IPropertyAnimation::get_Control2()
    @_property
    @abstractmethod
    def control2(self) -> windows_foundation.Point: ...
    # Windows.Foundation.TimeSpan Windows.UI.Core.AnimationMetrics.IPropertyAnimation::get_Delay()
    @_property
    @abstractmethod
    def delay(self) -> datetime.timedelta: ...
    # Windows.Foundation.TimeSpan Windows.UI.Core.AnimationMetrics.IPropertyAnimation::get_Duration()
    @_property
    @abstractmethod
    def duration(self) -> datetime.timedelta: ...
    # Windows.UI.Core.AnimationMetrics.PropertyAnimationType Windows.UI.Core.AnimationMetrics.IPropertyAnimation::get_Type()
    @_property
    @abstractmethod
    def type(self) -> PropertyAnimationType: ...

