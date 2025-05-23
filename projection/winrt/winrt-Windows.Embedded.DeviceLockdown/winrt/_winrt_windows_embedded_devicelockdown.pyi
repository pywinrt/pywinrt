# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.foundation as windows_foundation
import winrt.windows.foundation.collections as windows_foundation_collections

Self = typing.TypeVar('Self')

@typing.final
class DeviceLockdownProfile_Static(winrt._winrt.IInspectable_Static):
    # Windows.Foundation.IAsyncAction Windows.Embedded.DeviceLockdown.DeviceLockdownProfile::ApplyLockdownProfileAsync(System.Guid)
    def apply_lockdown_profile_async(cls, profile_id: _uuid.UUID, /) -> windows_foundation.IAsyncAction: ...
    # System.Guid Windows.Embedded.DeviceLockdown.DeviceLockdownProfile::GetCurrentLockdownProfile()
    def get_current_lockdown_profile(cls) -> _uuid.UUID: ...
    # Windows.Embedded.DeviceLockdown.DeviceLockdownProfileInformation Windows.Embedded.DeviceLockdown.DeviceLockdownProfile::GetLockdownProfileInformation(System.Guid)
    def get_lockdown_profile_information(cls, profile_id: _uuid.UUID, /) -> DeviceLockdownProfileInformation: ...
    # Windows.Foundation.Collections.IVectorView`1<System.Guid> Windows.Embedded.DeviceLockdown.DeviceLockdownProfile::GetSupportedLockdownProfiles()
    def get_supported_lockdown_profiles(cls) -> typing.Sequence[_uuid.UUID]: ...

@typing.final
class DeviceLockdownProfile(winrt.system.Object, metaclass=DeviceLockdownProfile_Static):
    pass

@typing.final
class DeviceLockdownProfileInformation(winrt.system.Object):
    # System.String Windows.Embedded.DeviceLockdown.DeviceLockdownProfileInformation::get_Name()
    @_property
    def name(self) -> str: ...

