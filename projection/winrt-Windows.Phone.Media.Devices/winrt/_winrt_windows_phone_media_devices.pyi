# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.foundation

from winrt.windows.phone.media.devices import AudioRoutingEndpoint, AvailableAudioRoutingEndpoints

Self = typing.TypeVar('Self')

class AudioRoutingManager_Static(type):
    def get_default(cls) -> typing.Optional[AudioRoutingManager]: ...

class AudioRoutingManager(winrt.system.Object, metaclass=AudioRoutingManager_Static):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> AudioRoutingManager: ...
    def get_audio_endpoint(self) -> AudioRoutingEndpoint: ...
    def set_audio_endpoint(self, endpoint: AudioRoutingEndpoint, /) -> None: ...
    def add_audio_endpoint_changed(self, endpoint_change_handler: winrt.windows.foundation.TypedEventHandler[AudioRoutingManager, winrt.system.Object], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_audio_endpoint_changed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    @_property
    def available_audio_endpoints(self) -> AvailableAudioRoutingEndpoints: ...
