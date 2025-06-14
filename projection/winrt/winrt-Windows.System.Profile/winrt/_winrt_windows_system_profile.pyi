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
import winrt.windows.storage.streams as windows_storage_streams
import winrt.windows.system as windows_system

from winrt.windows.system.profile import PlatformAutomaticAppSignInPolicy, PlatformDataCollectionLevel, SystemIdentificationSource, SystemOutOfBoxExperienceState, UnsupportedAppRequirementReasons

Self = typing.TypeVar('Self')

@typing.final
class AnalyticsInfo_Static(winrt._winrt.IInspectable_Static):
    # Windows.Foundation.IAsyncOperation`1<Windows.Foundation.Collections.IMapView`2<System.String,System.String>> Windows.System.Profile.AnalyticsInfo::GetSystemPropertiesAsync(Windows.Foundation.Collections.IIterable`1<System.String>)
    def get_system_properties_async(cls, attribute_names: typing.Iterable[str], /) -> windows_foundation.IAsyncOperation[typing.Mapping[str, str]]: ...
    # System.String Windows.System.Profile.AnalyticsInfo::get_DeviceForm()
    @_property
    def device_form(cls) -> str: ...
    # Windows.System.Profile.AnalyticsVersionInfo Windows.System.Profile.AnalyticsInfo::get_VersionInfo()
    @_property
    def version_info(cls) -> AnalyticsVersionInfo: ...

@typing.final
class AnalyticsInfo(winrt.system.Object, metaclass=AnalyticsInfo_Static):
    pass

@typing.final
class AnalyticsVersionInfo(winrt.system.Object):
    # System.String Windows.System.Profile.AnalyticsVersionInfo::get_DeviceFamily()
    @_property
    def device_family(self) -> str: ...
    # System.String Windows.System.Profile.AnalyticsVersionInfo::get_DeviceFamilyVersion()
    @_property
    def device_family_version(self) -> str: ...
    # System.String Windows.System.Profile.AnalyticsVersionInfo::get_ProductName()
    @_property
    def product_name(self) -> str: ...

@typing.final
class AppApplicability_Static(winrt._winrt.IInspectable_Static):
    # Windows.Foundation.Collections.IVectorView`1<Windows.System.Profile.UnsupportedAppRequirement> Windows.System.Profile.AppApplicability::GetUnsupportedAppRequirements(Windows.Foundation.Collections.IIterable`1<System.String>)
    def get_unsupported_app_requirements(cls, capabilities: typing.Iterable[str], /) -> typing.Sequence[UnsupportedAppRequirement]: ...

@typing.final
class AppApplicability(winrt.system.Object, metaclass=AppApplicability_Static):
    pass

@typing.final
class EducationSettings_Static(winrt._winrt.IInspectable_Static):
    # System.Boolean Windows.System.Profile.EducationSettings::get_IsEducationEnvironment()
    @_property
    def is_education_environment(cls) -> bool: ...

@typing.final
class EducationSettings(winrt.system.Object, metaclass=EducationSettings_Static):
    pass

@typing.final
class HardwareIdentification_Static(winrt._winrt.IInspectable_Static):
    # Windows.System.Profile.HardwareToken Windows.System.Profile.HardwareIdentification::GetPackageSpecificToken(Windows.Storage.Streams.IBuffer)
    def get_package_specific_token(cls, nonce: winrt.system.Buffer, /) -> HardwareToken: ...

@typing.final
class HardwareIdentification(winrt.system.Object, metaclass=HardwareIdentification_Static):
    pass

@typing.final
class HardwareToken(winrt.system.Object):
    # Windows.Storage.Streams.IBuffer Windows.System.Profile.HardwareToken::get_Certificate()
    @_property
    def certificate(self) -> winrt.system.Buffer: ...
    # Windows.Storage.Streams.IBuffer Windows.System.Profile.HardwareToken::get_Id()
    @_property
    def id(self) -> winrt.system.Buffer: ...
    # Windows.Storage.Streams.IBuffer Windows.System.Profile.HardwareToken::get_Signature()
    @_property
    def signature(self) -> winrt.system.Buffer: ...

@typing.final
class KnownRetailInfoProperties_Static(winrt._winrt.IInspectable_Static):
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_BatteryLifeDescription()
    @_property
    def battery_life_description(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_DisplayDescription()
    @_property
    def display_description(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_DisplayModelName()
    @_property
    def display_model_name(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_FormFactor()
    @_property
    def form_factor(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_FrontCameraDescription()
    @_property
    def front_camera_description(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_GraphicsDescription()
    @_property
    def graphics_description(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_HasNfc()
    @_property
    def has_nfc(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_HasOpticalDrive()
    @_property
    def has_optical_drive(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_HasSdSlot()
    @_property
    def has_sd_slot(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_IsFeatured()
    @_property
    def is_featured(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_IsOfficeInstalled()
    @_property
    def is_office_installed(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_ManufacturerName()
    @_property
    def manufacturer_name(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_Memory()
    @_property
    def memory(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_ModelName()
    @_property
    def model_name(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_Price()
    @_property
    def price(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_ProcessorDescription()
    @_property
    def processor_description(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_RearCameraDescription()
    @_property
    def rear_camera_description(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_RetailAccessCode()
    @_property
    def retail_access_code(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_ScreenSize()
    @_property
    def screen_size(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_StorageDescription()
    @_property
    def storage_description(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_Weight()
    @_property
    def weight(cls) -> str: ...
    # System.String Windows.System.Profile.KnownRetailInfoProperties::get_WindowsEdition()
    @_property
    def windows_edition(cls) -> str: ...

@typing.final
class KnownRetailInfoProperties(winrt.system.Object, metaclass=KnownRetailInfoProperties_Static):
    pass

@typing.final
class PlatformAutomaticAppSignInManager_Static(winrt._winrt.IInspectable_Static):
    # Windows.System.Profile.PlatformAutomaticAppSignInPolicy Windows.System.Profile.PlatformAutomaticAppSignInManager::get_Policy()
    @_property
    def policy(cls) -> PlatformAutomaticAppSignInPolicy: ...

@typing.final
class PlatformAutomaticAppSignInManager(winrt.system.Object, metaclass=PlatformAutomaticAppSignInManager_Static):
    pass

@typing.final
class PlatformDiagnosticsAndUsageDataSettings_Static(winrt._winrt.IInspectable_Static):
    # System.Boolean Windows.System.Profile.PlatformDiagnosticsAndUsageDataSettings::CanCollectDiagnostics(Windows.System.Profile.PlatformDataCollectionLevel)
    def can_collect_diagnostics(cls, level: PlatformDataCollectionLevel, /) -> bool: ...
    # Windows.Foundation.EventRegistrationToken Windows.System.Profile.PlatformDiagnosticsAndUsageDataSettings::add_CollectionLevelChanged(Windows.Foundation.EventHandler`1<System.Object>)
    def add_collection_level_changed(cls, handler: windows_foundation.EventHandler[winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.System.Profile.PlatformDiagnosticsAndUsageDataSettings::remove_CollectionLevelChanged(Windows.Foundation.EventRegistrationToken)
    def remove_collection_level_changed(cls, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.System.Profile.PlatformDataCollectionLevel Windows.System.Profile.PlatformDiagnosticsAndUsageDataSettings::get_CollectionLevel()
    @_property
    def collection_level(cls) -> PlatformDataCollectionLevel: ...

@typing.final
class PlatformDiagnosticsAndUsageDataSettings(winrt.system.Object, metaclass=PlatformDiagnosticsAndUsageDataSettings_Static):
    pass

@typing.final
class RetailInfo_Static(winrt._winrt.IInspectable_Static):
    # System.Boolean Windows.System.Profile.RetailInfo::get_IsDemoModeEnabled()
    @_property
    def is_demo_mode_enabled(cls) -> bool: ...
    # Windows.Foundation.Collections.IMapView`2<System.String,System.Object> Windows.System.Profile.RetailInfo::get_Properties()
    @_property
    def properties(cls) -> typing.Mapping[str, winrt.system.Object]: ...

@typing.final
class RetailInfo(winrt.system.Object, metaclass=RetailInfo_Static):
    pass

@typing.final
class SharedModeSettings_Static(winrt._winrt.IInspectable_Static):
    # System.Boolean Windows.System.Profile.SharedModeSettings::get_IsEnabled()
    @_property
    def is_enabled(cls) -> bool: ...
    # System.Boolean Windows.System.Profile.SharedModeSettings::get_ShouldAvoidLocalStorage()
    @_property
    def should_avoid_local_storage(cls) -> bool: ...

@typing.final
class SharedModeSettings(winrt.system.Object, metaclass=SharedModeSettings_Static):
    pass

@typing.final
class SmartAppControlPolicy_Static(winrt._winrt.IInspectable_Static):
    # Windows.Foundation.EventRegistrationToken Windows.System.Profile.SmartAppControlPolicy::add_Changed(Windows.Foundation.EventHandler`1<System.Object>)
    def add_changed(cls, handler: windows_foundation.EventHandler[winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.System.Profile.SmartAppControlPolicy::remove_Changed(Windows.Foundation.EventRegistrationToken)
    def remove_changed(cls, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # System.Boolean Windows.System.Profile.SmartAppControlPolicy::get_IsEnabled()
    @_property
    def is_enabled(cls) -> bool: ...

@typing.final
class SmartAppControlPolicy(winrt.system.Object, metaclass=SmartAppControlPolicy_Static):
    pass

@typing.final
class SystemIdentification_Static(winrt._winrt.IInspectable_Static):
    # Windows.System.Profile.SystemIdentificationInfo Windows.System.Profile.SystemIdentification::GetSystemIdForPublisher()
    def get_system_id_for_publisher(cls) -> SystemIdentificationInfo: ...
    # Windows.System.Profile.SystemIdentificationInfo Windows.System.Profile.SystemIdentification::GetSystemIdForUser(Windows.System.User)
    def get_system_id_for_user(cls, user: windows_system.User, /) -> SystemIdentificationInfo: ...

@typing.final
class SystemIdentification(winrt.system.Object, metaclass=SystemIdentification_Static):
    pass

@typing.final
class SystemIdentificationInfo(winrt.system.Object):
    # Windows.Storage.Streams.IBuffer Windows.System.Profile.SystemIdentificationInfo::get_Id()
    @_property
    def id(self) -> winrt.system.Buffer: ...
    # Windows.System.Profile.SystemIdentificationSource Windows.System.Profile.SystemIdentificationInfo::get_Source()
    @_property
    def source(self) -> SystemIdentificationSource: ...

@typing.final
class SystemSetupInfo_Static(winrt._winrt.IInspectable_Static):
    # Windows.Foundation.EventRegistrationToken Windows.System.Profile.SystemSetupInfo::add_OutOfBoxExperienceStateChanged(Windows.Foundation.EventHandler`1<System.Object>)
    def add_out_of_box_experience_state_changed(cls, handler: windows_foundation.EventHandler[winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.System.Profile.SystemSetupInfo::remove_OutOfBoxExperienceStateChanged(Windows.Foundation.EventRegistrationToken)
    def remove_out_of_box_experience_state_changed(cls, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.System.Profile.SystemOutOfBoxExperienceState Windows.System.Profile.SystemSetupInfo::get_OutOfBoxExperienceState()
    @_property
    def out_of_box_experience_state(cls) -> SystemOutOfBoxExperienceState: ...

@typing.final
class SystemSetupInfo(winrt.system.Object, metaclass=SystemSetupInfo_Static):
    pass

@typing.final
class UnsupportedAppRequirement(winrt.system.Object):
    # Windows.System.Profile.UnsupportedAppRequirementReasons Windows.System.Profile.UnsupportedAppRequirement::get_Reasons()
    @_property
    def reasons(self) -> UnsupportedAppRequirementReasons: ...
    # System.String Windows.System.Profile.UnsupportedAppRequirement::get_Requirement()
    @_property
    def requirement(self) -> str: ...

@typing.final
class WindowsIntegrityPolicy_Static(winrt._winrt.IInspectable_Static):
    # Windows.Foundation.EventRegistrationToken Windows.System.Profile.WindowsIntegrityPolicy::add_PolicyChanged(Windows.Foundation.EventHandler`1<System.Object>)
    def add_policy_changed(cls, handler: windows_foundation.EventHandler[winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.System.Profile.WindowsIntegrityPolicy::remove_PolicyChanged(Windows.Foundation.EventRegistrationToken)
    def remove_policy_changed(cls, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # System.Boolean Windows.System.Profile.WindowsIntegrityPolicy::get_CanDisable()
    @_property
    def can_disable(cls) -> bool: ...
    # System.Boolean Windows.System.Profile.WindowsIntegrityPolicy::get_IsDisableSupported()
    @_property
    def is_disable_supported(cls) -> bool: ...
    # System.Boolean Windows.System.Profile.WindowsIntegrityPolicy::get_IsEnabled()
    @_property
    def is_enabled(cls) -> bool: ...
    # System.Boolean Windows.System.Profile.WindowsIntegrityPolicy::get_IsEnabledForTrial()
    @_property
    def is_enabled_for_trial(cls) -> bool: ...

@typing.final
class WindowsIntegrityPolicy(winrt.system.Object, metaclass=WindowsIntegrityPolicy_Static):
    pass

