# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from . import _winrt_windows_services_targetedcontent

class TargetedContentAppInstallationState(enum.IntEnum):
    NOT_APPLICABLE = 0
    NOT_INSTALLED = 1
    INSTALLED = 2

class TargetedContentAvailability(enum.IntEnum):
    NONE = 0
    PARTIAL = 1
    ALL = 2

class TargetedContentInteraction(enum.IntEnum):
    IMPRESSION = 0
    CLICK_THROUGH = 1
    HOVER = 2
    LIKE = 3
    DISLIKE = 4
    DISMISS = 5
    INELIGIBLE = 6
    ACCEPT = 7
    DECLINE = 8
    DEFER = 9
    CANCELED = 10
    CONVERSION = 11
    OPPORTUNITY = 12

class TargetedContentObjectKind(enum.IntEnum):
    COLLECTION = 0
    ITEM = 1
    VALUE = 2

class TargetedContentValueKind(enum.IntEnum):
    STRING = 0
    URI = 1
    NUMBER = 2
    BOOLEAN = 3
    FILE = 4
    IMAGE_FILE = 5
    ACTION = 6
    STRINGS = 7
    URIS = 8
    NUMBERS = 9
    BOOLEANS = 10
    FILES = 11
    IMAGE_FILES = 12
    ACTIONS = 13

TargetedContentAction = _winrt_windows_services_targetedcontent.TargetedContentAction
TargetedContentAvailabilityChangedEventArgs = _winrt_windows_services_targetedcontent.TargetedContentAvailabilityChangedEventArgs
TargetedContentChangedEventArgs = _winrt_windows_services_targetedcontent.TargetedContentChangedEventArgs
TargetedContentCollection = _winrt_windows_services_targetedcontent.TargetedContentCollection
TargetedContentContainer = _winrt_windows_services_targetedcontent.TargetedContentContainer
TargetedContentFile = _winrt_windows_services_targetedcontent.TargetedContentFile
TargetedContentImage = _winrt_windows_services_targetedcontent.TargetedContentImage
TargetedContentItem = _winrt_windows_services_targetedcontent.TargetedContentItem
TargetedContentItemState = _winrt_windows_services_targetedcontent.TargetedContentItemState
TargetedContentObject = _winrt_windows_services_targetedcontent.TargetedContentObject
TargetedContentStateChangedEventArgs = _winrt_windows_services_targetedcontent.TargetedContentStateChangedEventArgs
TargetedContentSubscription = _winrt_windows_services_targetedcontent.TargetedContentSubscription
TargetedContentSubscriptionOptions = _winrt_windows_services_targetedcontent.TargetedContentSubscriptionOptions
TargetedContentValue = _winrt_windows_services_targetedcontent.TargetedContentValue