# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum
import datetime
import sys
import types
import typing

import winrt.system
import winrt.windows.foundation
import winrt.windows.storage.streams

class AuthenticationProtocol(enum.IntEnum):
    BASIC = 0
    DIGEST = 1
    NTLM = 2
    KERBEROS = 3
    NEGOTIATE = 4
    CRED_SSP = 5
    CUSTOM = 6

class CredentialSaveOption(enum.IntEnum):
    UNSELECTED = 0
    SELECTED = 1
    HIDDEN = 2

class UserConsentVerificationResult(enum.IntEnum):
    VERIFIED = 0
    DEVICE_NOT_PRESENT = 1
    NOT_CONFIGURED_FOR_USER = 2
    DISABLED_BY_POLICY = 3
    DEVICE_BUSY = 4
    RETRIES_EXHAUSTED = 5
    CANCELED = 6

class UserConsentVerifierAvailability(enum.IntEnum):
    AVAILABLE = 0
    DEVICE_NOT_PRESENT = 1
    NOT_CONFIGURED_FOR_USER = 2
    DISABLED_BY_POLICY = 3
    DEVICE_BUSY = 4

Self = typing.TypeVar('Self')

class CredentialPicker(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> CredentialPicker: ...
    @typing.overload
    @staticmethod
    def pick_async(options: typing.Optional[CredentialPickerOptions], /) -> winrt.windows.foundation.IAsyncOperation[CredentialPickerResults]: ...
    @typing.overload
    @staticmethod
    def pick_async(target_name: str, message: str, /) -> winrt.windows.foundation.IAsyncOperation[CredentialPickerResults]: ...
    @typing.overload
    @staticmethod
    def pick_async(target_name: str, message: str, caption: str, /) -> winrt.windows.foundation.IAsyncOperation[CredentialPickerResults]: ...

class CredentialPickerOptions(winrt.system.Object):
    target_name: str
    previous_credential: typing.Optional[winrt.windows.storage.streams.IBuffer]
    message: str
    error_code: winrt.system.UInt32
    custom_authentication_protocol: str
    credential_save_option: CredentialSaveOption
    caption: str
    caller_saves_credential: winrt.system.Boolean
    authentication_protocol: AuthenticationProtocol
    always_display_dialog: winrt.system.Boolean
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> CredentialPickerOptions: ...
    def __new__(cls: typing.Type[CredentialPickerOptions]) -> CredentialPickerOptions:...

class CredentialPickerResults(winrt.system.Object):
    credential: typing.Optional[winrt.windows.storage.streams.IBuffer]
    credential_domain_name: str
    credential_password: str
    credential_save_option: CredentialSaveOption
    credential_saved: winrt.system.Boolean
    credential_user_name: str
    error_code: winrt.system.UInt32
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> CredentialPickerResults: ...

class UserConsentVerifier(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> UserConsentVerifier: ...
    @staticmethod
    def check_availability_async() -> winrt.windows.foundation.IAsyncOperation[UserConsentVerifierAvailability]: ...
    @staticmethod
    def request_verification_async(message: str, /) -> winrt.windows.foundation.IAsyncOperation[UserConsentVerificationResult]: ...
