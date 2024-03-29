# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winrt import _winrt_windows_applicationmodel_voicecommands

__all__ = [
    "VoiceCommandCompletionReason",
    "VoiceCommandContentTileType",
    "VoiceCommand",
    "VoiceCommandCompletedEventArgs",
    "VoiceCommandConfirmationResult",
    "VoiceCommandContentTile",
    "VoiceCommandDefinition",
    "VoiceCommandDefinitionManager",
    "VoiceCommandDisambiguationResult",
    "VoiceCommandResponse",
    "VoiceCommandServiceConnection",
    "VoiceCommandUserMessage",
]

class VoiceCommandCompletionReason(enum.IntEnum):
    UNKNOWN = 0
    COMMUNICATION_FAILED = 1
    RESOURCE_LIMITS_EXCEEDED = 2
    CANCELED = 3
    TIMEOUT_EXCEEDED = 4
    APP_LAUNCHED = 5
    COMPLETED = 6

class VoiceCommandContentTileType(enum.IntEnum):
    TITLE_ONLY = 0
    TITLE_WITH_TEXT = 1
    TITLE_WITH68X68_ICON = 2
    TITLE_WITH68X68_ICON_AND_TEXT = 3
    TITLE_WITH68X92_ICON = 4
    TITLE_WITH68X92_ICON_AND_TEXT = 5
    TITLE_WITH280X140_ICON = 6
    TITLE_WITH280X140_ICON_AND_TEXT = 7

VoiceCommand = _winrt_windows_applicationmodel_voicecommands.VoiceCommand
VoiceCommandCompletedEventArgs = _winrt_windows_applicationmodel_voicecommands.VoiceCommandCompletedEventArgs
VoiceCommandConfirmationResult = _winrt_windows_applicationmodel_voicecommands.VoiceCommandConfirmationResult
VoiceCommandContentTile = _winrt_windows_applicationmodel_voicecommands.VoiceCommandContentTile
VoiceCommandDefinition = _winrt_windows_applicationmodel_voicecommands.VoiceCommandDefinition
VoiceCommandDefinitionManager = _winrt_windows_applicationmodel_voicecommands.VoiceCommandDefinitionManager
VoiceCommandDisambiguationResult = _winrt_windows_applicationmodel_voicecommands.VoiceCommandDisambiguationResult
VoiceCommandResponse = _winrt_windows_applicationmodel_voicecommands.VoiceCommandResponse
VoiceCommandServiceConnection = _winrt_windows_applicationmodel_voicecommands.VoiceCommandServiceConnection
VoiceCommandUserMessage = _winrt_windows_applicationmodel_voicecommands.VoiceCommandUserMessage
