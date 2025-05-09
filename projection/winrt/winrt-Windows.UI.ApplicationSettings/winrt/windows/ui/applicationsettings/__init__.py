# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum
import typing
import uuid as _uuid

import winrt.system
from winrt._winrt_windows_ui_applicationsettings import (
    AccountsSettingsPane,
    AccountsSettingsPaneCommandsRequestedEventArgs,
    AccountsSettingsPaneEventDeferral,
    CredentialCommand,
    SettingsCommand,
    SettingsPane,
    SettingsPaneCommandsRequest,
    SettingsPaneCommandsRequestedEventArgs,
    WebAccountCommand,
    WebAccountInvokedArgs,
    WebAccountProviderCommand,
)

__all__ = [
    "SettingsEdgeLocation",
    "SupportedWebAccountActions",
    "WebAccountAction",
    "AccountsSettingsPane",
    "AccountsSettingsPaneCommandsRequestedEventArgs",
    "AccountsSettingsPaneEventDeferral",
    "CredentialCommand",
    "SettingsCommand",
    "SettingsPane",
    "SettingsPaneCommandsRequest",
    "SettingsPaneCommandsRequestedEventArgs",
    "WebAccountCommand",
    "WebAccountInvokedArgs",
    "WebAccountProviderCommand",
    "CredentialCommandCredentialDeletedHandler",
    "WebAccountCommandInvokedHandler",
    "WebAccountProviderCommandInvokedHandler",
]

class SettingsEdgeLocation(enum.IntEnum):
    RIGHT = 0
    LEFT = 1

class SupportedWebAccountActions(enum.IntFlag):
    NONE = 0x0
    RECONNECT = 0x1
    REMOVE = 0x2
    VIEW_DETAILS = 0x4
    MANAGE = 0x8
    MORE = 0x10

class WebAccountAction(enum.IntEnum):
    RECONNECT = 0
    REMOVE = 1
    VIEW_DETAILS = 2
    MANAGE = 3
    MORE = 4

CredentialCommandCredentialDeletedHandler = typing.Callable[[CredentialCommand], None]
WebAccountCommandInvokedHandler = typing.Callable[[WebAccountCommand, WebAccountInvokedArgs], None]
WebAccountProviderCommandInvokedHandler = typing.Callable[[WebAccountProviderCommand], None]
