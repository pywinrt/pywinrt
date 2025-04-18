# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winrt._winrt_windows_web_ui import (
    WebViewControlContentLoadingEventArgs,
    WebViewControlDOMContentLoadedEventArgs,
    WebViewControlDeferredPermissionRequest,
    WebViewControlLongRunningScriptDetectedEventArgs,
    WebViewControlNavigationCompletedEventArgs,
    WebViewControlNavigationStartingEventArgs,
    WebViewControlNewWindowRequestedEventArgs,
    WebViewControlPermissionRequest,
    WebViewControlPermissionRequestedEventArgs,
    WebViewControlScriptNotifyEventArgs,
    WebViewControlSettings,
    WebViewControlUnsupportedUriSchemeIdentifiedEventArgs,
    WebViewControlUnviewableContentIdentifiedEventArgs,
    WebViewControlWebResourceRequestedEventArgs,
    _IWebViewControl,
    IWebViewControl,
    _IWebViewControl2,
    IWebViewControl2,
)

__all__ = [
    "WebViewControlPermissionState",
    "WebViewControlPermissionType",
    "WebViewControlContentLoadingEventArgs",
    "WebViewControlDOMContentLoadedEventArgs",
    "WebViewControlDeferredPermissionRequest",
    "WebViewControlLongRunningScriptDetectedEventArgs",
    "WebViewControlNavigationCompletedEventArgs",
    "WebViewControlNavigationStartingEventArgs",
    "WebViewControlNewWindowRequestedEventArgs",
    "WebViewControlPermissionRequest",
    "WebViewControlPermissionRequestedEventArgs",
    "WebViewControlScriptNotifyEventArgs",
    "WebViewControlSettings",
    "WebViewControlUnsupportedUriSchemeIdentifiedEventArgs",
    "WebViewControlUnviewableContentIdentifiedEventArgs",
    "WebViewControlWebResourceRequestedEventArgs",
    "IWebViewControl",
    "IWebViewControl2",
]

class WebViewControlPermissionState(enum.IntEnum):
    UNKNOWN = 0
    DEFER = 1
    ALLOW = 2
    DENY = 3

class WebViewControlPermissionType(enum.IntEnum):
    GEOLOCATION = 0
    UNLIMITED_INDEXED_DB_QUOTA = 1
    MEDIA = 2
    POINTER_LOCK = 3
    WEB_NOTIFICATIONS = 4
    SCREEN = 5
    IMMERSIVE_VIEW = 6

