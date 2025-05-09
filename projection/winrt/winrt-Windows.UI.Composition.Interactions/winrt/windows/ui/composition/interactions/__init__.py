# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winrt._winrt_windows_ui_composition_interactions import (
    CompositionConditionalValue,
    CompositionInteractionSourceCollection,
    InteractionSourceConfiguration,
    InteractionTracker,
    InteractionTrackerCustomAnimationStateEnteredArgs,
    InteractionTrackerIdleStateEnteredArgs,
    InteractionTrackerInertiaModifier,
    InteractionTrackerInertiaMotion,
    InteractionTrackerInertiaNaturalMotion,
    InteractionTrackerInertiaRestingValue,
    InteractionTrackerInertiaStateEnteredArgs,
    InteractionTrackerInteractingStateEnteredArgs,
    InteractionTrackerRequestIgnoredArgs,
    InteractionTrackerValuesChangedArgs,
    InteractionTrackerVector2InertiaModifier,
    InteractionTrackerVector2InertiaNaturalMotion,
    VisualInteractionSource,
    _ICompositionInteractionSource,
    ICompositionInteractionSource,
    _IInteractionTrackerOwner,
    IInteractionTrackerOwner,
)
from typing import TYPE_CHECKING
if TYPE_CHECKING:
    from winrt._winrt_windows_ui_composition_interactions import (
        InteractionTrackerInertiaModifier_Static,
        InteractionTrackerVector2InertiaModifier_Static,
        VisualInteractionSource_Static,
    )

__all__ = [
    "InteractionBindingAxisModes",
    "InteractionChainingMode",
    "InteractionSourceMode",
    "InteractionSourceRedirectionMode",
    "InteractionTrackerClampingOption",
    "InteractionTrackerPositionUpdateOption",
    "VisualInteractionSourceRedirectionMode",
    "CompositionConditionalValue",
    "CompositionInteractionSourceCollection",
    "InteractionSourceConfiguration",
    "InteractionTracker",
    "InteractionTrackerCustomAnimationStateEnteredArgs",
    "InteractionTrackerIdleStateEnteredArgs",
    "InteractionTrackerInertiaModifier",
    "InteractionTrackerInertiaMotion",
    "InteractionTrackerInertiaNaturalMotion",
    "InteractionTrackerInertiaRestingValue",
    "InteractionTrackerInertiaStateEnteredArgs",
    "InteractionTrackerInteractingStateEnteredArgs",
    "InteractionTrackerRequestIgnoredArgs",
    "InteractionTrackerValuesChangedArgs",
    "InteractionTrackerVector2InertiaModifier",
    "InteractionTrackerVector2InertiaNaturalMotion",
    "VisualInteractionSource",
    "ICompositionInteractionSource",
    "IInteractionTrackerOwner",
]

class InteractionBindingAxisModes(enum.IntFlag):
    NONE = 0x0
    POSITION_X = 0x1
    POSITION_Y = 0x2
    SCALE = 0x4

class InteractionChainingMode(enum.IntEnum):
    AUTO = 0
    ALWAYS = 1
    NEVER = 2

class InteractionSourceMode(enum.IntEnum):
    DISABLED = 0
    ENABLED_WITH_INERTIA = 1
    ENABLED_WITHOUT_INERTIA = 2

class InteractionSourceRedirectionMode(enum.IntEnum):
    DISABLED = 0
    ENABLED = 1

class InteractionTrackerClampingOption(enum.IntEnum):
    AUTO = 0
    DISABLED = 1

class InteractionTrackerPositionUpdateOption(enum.IntEnum):
    DEFAULT = 0
    ALLOW_ACTIVE_CUSTOM_SCALE_ANIMATION = 1

class VisualInteractionSourceRedirectionMode(enum.IntEnum):
    OFF = 0
    CAPABLE_TOUCHPAD_ONLY = 1
    POINTER_WHEEL_ONLY = 2
    CAPABLE_TOUCHPAD_AND_POINTER_WHEEL = 3

