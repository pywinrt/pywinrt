# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winui3._winui3_microsoft_ui_content import (
    ChildSiteLink,
    ContentCoordinateConverter,
    ContentDeferral,
    ContentEnvironmentSettingChangedEventArgs,
    ContentEnvironmentStateChangedEventArgs,
    ContentIsland,
    ContentIslandAutomationProviderRequestedEventArgs,
    ContentIslandEnvironment,
    ContentIslandStateChangedEventArgs,
    ContentSite,
    ContentSiteAutomationProviderRequestedEventArgs,
    ContentSiteEnvironment,
    ContentSiteEnvironmentView,
    ContentSiteRequestedStateChangedEventArgs,
    ContentSiteView,
    DesktopAttachedSiteBridge,
    DesktopChildSiteBridge,
    DesktopPopupSiteBridge,
    DesktopSiteBridge,
    _IContentSiteAutomation,
    IContentSiteAutomation,
    _IContentSiteBridge,
    IContentSiteBridge,
    _IContentSiteInput,
    IContentSiteInput,
    _IContentSiteLink,
    IContentSiteLink,
)
from typing import TYPE_CHECKING
if TYPE_CHECKING:
    from winui3._winui3_microsoft_ui_content import (
        ContentCoordinateConverter_Static,
        ContentIsland_Static,
        ContentIslandEnvironment_Static,
        ContentSite_Static,
        ContentSiteEnvironment_Static,
        ContentSiteEnvironmentView_Static,
        ContentSiteView_Static,
        DesktopSiteBridge_Static,
    )

__all__ = [
    "ContentAutomationOptions",
    "ContentCoordinateRoundingMode",
    "ContentLayoutDirection",
    "ContentSizePolicy",
    "ChildSiteLink",
    "ContentCoordinateConverter",
    "ContentDeferral",
    "ContentEnvironmentSettingChangedEventArgs",
    "ContentEnvironmentStateChangedEventArgs",
    "ContentIsland",
    "ContentIslandAutomationProviderRequestedEventArgs",
    "ContentIslandEnvironment",
    "ContentIslandStateChangedEventArgs",
    "ContentSite",
    "ContentSiteAutomationProviderRequestedEventArgs",
    "ContentSiteEnvironment",
    "ContentSiteEnvironmentView",
    "ContentSiteRequestedStateChangedEventArgs",
    "ContentSiteView",
    "DesktopAttachedSiteBridge",
    "DesktopChildSiteBridge",
    "DesktopPopupSiteBridge",
    "DesktopSiteBridge",
    "IContentSiteAutomation",
    "IContentSiteBridge",
    "IContentSiteInput",
    "IContentSiteLink",
]

class ContentAutomationOptions(enum.IntEnum):
    NONE = 0
    FRAMEWORK_BASED = 1
    FRAGMENT_BASED = 2

class ContentCoordinateRoundingMode(enum.IntEnum):
    AUTO = 0
    FLOOR = 1
    ROUND = 2
    CEILING = 3

class ContentLayoutDirection(enum.IntEnum):
    LEFT_TO_RIGHT = 0
    RIGHT_TO_LEFT = 1

class ContentSizePolicy(enum.IntEnum):
    NONE = 0
    RESIZE_CONTENT_TO_PARENT_WINDOW = 1
    RESIZE_PARENT_WINDOW_TO_CONTENT = 2

