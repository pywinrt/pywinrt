# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winrt import _winrt_microsoft_windows_appnotifications_builder

__all__ = [
    "AppNotificationAudioLooping",
    "AppNotificationButtonStyle",
    "AppNotificationDuration",
    "AppNotificationImageCrop",
    "AppNotificationScenario",
    "AppNotificationSoundEvent",
    "AppNotificationBuilder",
    "AppNotificationButton",
    "AppNotificationComboBox",
    "AppNotificationProgressBar",
    "AppNotificationTextProperties",
]

class AppNotificationAudioLooping(enum.IntEnum):
    NONE = 0
    LOOP = 1

class AppNotificationButtonStyle(enum.IntEnum):
    DEFAULT = 0
    SUCCESS = 1
    CRITICAL = 2

class AppNotificationDuration(enum.IntEnum):
    DEFAULT = 0
    LONG = 1

class AppNotificationImageCrop(enum.IntEnum):
    DEFAULT = 0
    CIRCLE = 1

class AppNotificationScenario(enum.IntEnum):
    DEFAULT = 0
    REMINDER = 1
    ALARM = 2
    INCOMING_CALL = 3
    URGENT = 4

class AppNotificationSoundEvent(enum.IntEnum):
    DEFAULT = 0
    I_M = 1
    MAIL = 2
    REMINDER = 3
    S_M_S = 4
    ALARM = 5
    ALARM2 = 6
    ALARM3 = 7
    ALARM4 = 8
    ALARM5 = 9
    ALARM6 = 10
    ALARM7 = 11
    ALARM8 = 12
    ALARM9 = 13
    ALARM10 = 14
    CALL = 15
    CALL2 = 16
    CALL3 = 17
    CALL4 = 18
    CALL5 = 19
    CALL6 = 20
    CALL7 = 21
    CALL8 = 22
    CALL9 = 23
    CALL10 = 24

AppNotificationBuilder = _winrt_microsoft_windows_appnotifications_builder.AppNotificationBuilder
AppNotificationButton = _winrt_microsoft_windows_appnotifications_builder.AppNotificationButton
AppNotificationComboBox = _winrt_microsoft_windows_appnotifications_builder.AppNotificationComboBox
AppNotificationProgressBar = _winrt_microsoft_windows_appnotifications_builder.AppNotificationProgressBar
AppNotificationTextProperties = _winrt_microsoft_windows_appnotifications_builder.AppNotificationTextProperties
