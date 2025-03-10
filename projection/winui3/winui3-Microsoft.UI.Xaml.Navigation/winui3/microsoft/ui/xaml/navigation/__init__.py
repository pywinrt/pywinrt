# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum
import typing
import uuid as _uuid

import winrt.system
from winui3._winui3_microsoft_ui_xaml_navigation import (
    FrameNavigationOptions,
    NavigatingCancelEventArgs,
    NavigationEventArgs,
    NavigationFailedEventArgs,
    PageStackEntry,
)
from typing import TYPE_CHECKING
if TYPE_CHECKING:
    from winui3._winui3_microsoft_ui_xaml_navigation import (
        FrameNavigationOptions_Static,
    )

__all__ = [
    "NavigationCacheMode",
    "NavigationMode",
    "FrameNavigationOptions",
    "NavigatingCancelEventArgs",
    "NavigationEventArgs",
    "NavigationFailedEventArgs",
    "PageStackEntry",
    "NavigatedEventHandler",
    "NavigatingCancelEventHandler",
    "NavigationFailedEventHandler",
    "NavigationStoppedEventHandler",
]

class NavigationCacheMode(enum.IntEnum):
    DISABLED = 0
    REQUIRED = 1
    ENABLED = 2

class NavigationMode(enum.IntEnum):
    NEW = 0
    BACK = 1
    FORWARD = 2
    REFRESH = 3

NavigatedEventHandler = typing.Callable[[winrt.system.Object, NavigationEventArgs], None]
NavigatingCancelEventHandler = typing.Callable[[winrt.system.Object, NavigatingCancelEventArgs], None]
NavigationFailedEventHandler = typing.Callable[[winrt.system.Object, NavigationFailedEventArgs], None]
NavigationStoppedEventHandler = typing.Callable[[winrt.system.Object, NavigationEventArgs], None]
