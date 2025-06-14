# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.devices.enumeration as windows_devices_enumeration
import winrt.windows.foundation as windows_foundation
import winrt.windows.foundation.collections as windows_foundation_collections
import winrt.windows.networking as windows_networking
import winrt.windows.security.credentials as windows_security_credentials
import winrt.windows.storage.streams as windows_storage_streams

from winrt.windows.devices.wifidirect import WiFiDirectAdvertisementListenStateDiscoverability, WiFiDirectAdvertisementPublisherStatus, WiFiDirectConfigurationMethod, WiFiDirectConnectionStatus, WiFiDirectDeviceSelectorType, WiFiDirectError, WiFiDirectPairingProcedure

Self = typing.TypeVar('Self')

@typing.final
class WiFiDirectAdvertisement(winrt.system.Object):
    # Windows.Devices.WiFiDirect.WiFiDirectAdvertisementListenStateDiscoverability Windows.Devices.WiFiDirect.WiFiDirectAdvertisement::get_ListenStateDiscoverability()
    @_property
    def listen_state_discoverability(self) -> WiFiDirectAdvertisementListenStateDiscoverability: ...
    # System.Void Windows.Devices.WiFiDirect.WiFiDirectAdvertisement::put_ListenStateDiscoverability(Windows.Devices.WiFiDirect.WiFiDirectAdvertisementListenStateDiscoverability)
    @listen_state_discoverability.setter
    def listen_state_discoverability(self, value: WiFiDirectAdvertisementListenStateDiscoverability) -> None: ...
    # System.Boolean Windows.Devices.WiFiDirect.WiFiDirectAdvertisement::get_IsAutonomousGroupOwnerEnabled()
    @_property
    def is_autonomous_group_owner_enabled(self) -> bool: ...
    # System.Void Windows.Devices.WiFiDirect.WiFiDirectAdvertisement::put_IsAutonomousGroupOwnerEnabled(System.Boolean)
    @is_autonomous_group_owner_enabled.setter
    def is_autonomous_group_owner_enabled(self, value: bool) -> None: ...
    # Windows.Foundation.Collections.IVector`1<Windows.Devices.WiFiDirect.WiFiDirectInformationElement> Windows.Devices.WiFiDirect.WiFiDirectAdvertisement::get_InformationElements()
    @_property
    def information_elements(self) -> typing.MutableSequence[WiFiDirectInformationElement]: ...
    # System.Void Windows.Devices.WiFiDirect.WiFiDirectAdvertisement::put_InformationElements(Windows.Foundation.Collections.IVector`1<Windows.Devices.WiFiDirect.WiFiDirectInformationElement>)
    @information_elements.setter
    def information_elements(self, value: typing.MutableSequence[WiFiDirectInformationElement]) -> None: ...
    # Windows.Devices.WiFiDirect.WiFiDirectLegacySettings Windows.Devices.WiFiDirect.WiFiDirectAdvertisement::get_LegacySettings()
    @_property
    def legacy_settings(self) -> WiFiDirectLegacySettings: ...
    # Windows.Foundation.Collections.IVector`1<Windows.Devices.WiFiDirect.WiFiDirectConfigurationMethod> Windows.Devices.WiFiDirect.WiFiDirectAdvertisement::get_SupportedConfigurationMethods()
    @_property
    def supported_configuration_methods(self) -> typing.MutableSequence[WiFiDirectConfigurationMethod]: ...

@typing.final
class WiFiDirectAdvertisementPublisher(winrt.system.Object):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # System.Void Windows.Devices.WiFiDirect.WiFiDirectAdvertisementPublisher::Start()
    def start(self) -> None: ...
    # System.Void Windows.Devices.WiFiDirect.WiFiDirectAdvertisementPublisher::Stop()
    def stop(self) -> None: ...
    # Windows.Foundation.EventRegistrationToken Windows.Devices.WiFiDirect.WiFiDirectAdvertisementPublisher::add_StatusChanged(Windows.Foundation.TypedEventHandler`2<Windows.Devices.WiFiDirect.WiFiDirectAdvertisementPublisher,Windows.Devices.WiFiDirect.WiFiDirectAdvertisementPublisherStatusChangedEventArgs>)
    def add_status_changed(self, handler: windows_foundation.TypedEventHandler[WiFiDirectAdvertisementPublisher, WiFiDirectAdvertisementPublisherStatusChangedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.Devices.WiFiDirect.WiFiDirectAdvertisementPublisher::remove_StatusChanged(Windows.Foundation.EventRegistrationToken)
    def remove_status_changed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Devices.WiFiDirect.WiFiDirectAdvertisement Windows.Devices.WiFiDirect.WiFiDirectAdvertisementPublisher::get_Advertisement()
    @_property
    def advertisement(self) -> WiFiDirectAdvertisement: ...
    # Windows.Devices.WiFiDirect.WiFiDirectAdvertisementPublisherStatus Windows.Devices.WiFiDirect.WiFiDirectAdvertisementPublisher::get_Status()
    @_property
    def status(self) -> WiFiDirectAdvertisementPublisherStatus: ...

@typing.final
class WiFiDirectAdvertisementPublisherStatusChangedEventArgs(winrt.system.Object):
    # Windows.Devices.WiFiDirect.WiFiDirectError Windows.Devices.WiFiDirect.WiFiDirectAdvertisementPublisherStatusChangedEventArgs::get_Error()
    @_property
    def error(self) -> WiFiDirectError: ...
    # Windows.Devices.WiFiDirect.WiFiDirectAdvertisementPublisherStatus Windows.Devices.WiFiDirect.WiFiDirectAdvertisementPublisherStatusChangedEventArgs::get_Status()
    @_property
    def status(self) -> WiFiDirectAdvertisementPublisherStatus: ...

@typing.final
class WiFiDirectConnectionListener(winrt.system.Object):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # Windows.Foundation.EventRegistrationToken Windows.Devices.WiFiDirect.WiFiDirectConnectionListener::add_ConnectionRequested(Windows.Foundation.TypedEventHandler`2<Windows.Devices.WiFiDirect.WiFiDirectConnectionListener,Windows.Devices.WiFiDirect.WiFiDirectConnectionRequestedEventArgs>)
    def add_connection_requested(self, handler: windows_foundation.TypedEventHandler[WiFiDirectConnectionListener, WiFiDirectConnectionRequestedEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.Devices.WiFiDirect.WiFiDirectConnectionListener::remove_ConnectionRequested(Windows.Foundation.EventRegistrationToken)
    def remove_connection_requested(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...

@typing.final
class WiFiDirectConnectionParameters_Static(winrt._winrt.IInspectable_Static):
    # Windows.Devices.Enumeration.DevicePairingKinds Windows.Devices.WiFiDirect.WiFiDirectConnectionParameters::GetDevicePairingKinds(Windows.Devices.WiFiDirect.WiFiDirectConfigurationMethod)
    def get_device_pairing_kinds(cls, configuration_method: WiFiDirectConfigurationMethod, /) -> windows_devices_enumeration.DevicePairingKinds: ...

@typing.final
class WiFiDirectConnectionParameters(winrt.system.Object, windows_devices_enumeration.IDevicePairingSettings, metaclass=WiFiDirectConnectionParameters_Static):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # System.Int16 Windows.Devices.WiFiDirect.WiFiDirectConnectionParameters::get_GroupOwnerIntent()
    @_property
    def group_owner_intent(self) -> winrt.system.Int16: ...
    # System.Void Windows.Devices.WiFiDirect.WiFiDirectConnectionParameters::put_GroupOwnerIntent(System.Int16)
    @group_owner_intent.setter
    def group_owner_intent(self, value: winrt.system.Int16) -> None: ...
    # Windows.Devices.WiFiDirect.WiFiDirectPairingProcedure Windows.Devices.WiFiDirect.WiFiDirectConnectionParameters::get_PreferredPairingProcedure()
    @_property
    def preferred_pairing_procedure(self) -> WiFiDirectPairingProcedure: ...
    # System.Void Windows.Devices.WiFiDirect.WiFiDirectConnectionParameters::put_PreferredPairingProcedure(Windows.Devices.WiFiDirect.WiFiDirectPairingProcedure)
    @preferred_pairing_procedure.setter
    def preferred_pairing_procedure(self, value: WiFiDirectPairingProcedure) -> None: ...
    # Windows.Foundation.Collections.IVector`1<Windows.Devices.WiFiDirect.WiFiDirectConfigurationMethod> Windows.Devices.WiFiDirect.WiFiDirectConnectionParameters::get_PreferenceOrderedConfigurationMethods()
    @_property
    def preference_ordered_configuration_methods(self) -> typing.MutableSequence[WiFiDirectConfigurationMethod]: ...

@typing.final
class WiFiDirectConnectionRequest(winrt.system.Object, windows_foundation.IClosable):
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, exc_type: typing.Optional[typing.Type[BaseException]], exc_value: typing.Optional[BaseException], traceback: typing.Optional[types.TracebackType]) -> None: ...
    # System.Void Windows.Devices.WiFiDirect.WiFiDirectConnectionRequest::Close()
    def close(self) -> None: ...
    # Windows.Devices.Enumeration.DeviceInformation Windows.Devices.WiFiDirect.WiFiDirectConnectionRequest::get_DeviceInformation()
    @_property
    def device_information(self) -> windows_devices_enumeration.DeviceInformation: ...

@typing.final
class WiFiDirectConnectionRequestedEventArgs(winrt.system.Object):
    # Windows.Devices.WiFiDirect.WiFiDirectConnectionRequest Windows.Devices.WiFiDirect.WiFiDirectConnectionRequestedEventArgs::GetConnectionRequest()
    def get_connection_request(self) -> WiFiDirectConnectionRequest: ...

@typing.final
class WiFiDirectDevice_Static(winrt._winrt.IInspectable_Static):
    @typing.overload
    # Windows.Foundation.IAsyncOperation`1<Windows.Devices.WiFiDirect.WiFiDirectDevice> Windows.Devices.WiFiDirect.WiFiDirectDevice::FromIdAsync(System.String)
    def from_id_async(cls, device_id: str, /) -> windows_foundation.IAsyncOperation[WiFiDirectDevice]: ...
    @typing.overload
    # Windows.Foundation.IAsyncOperation`1<Windows.Devices.WiFiDirect.WiFiDirectDevice> Windows.Devices.WiFiDirect.WiFiDirectDevice::FromIdAsync(System.String,Windows.Devices.WiFiDirect.WiFiDirectConnectionParameters)
    def from_id_async(cls, device_id: str, connection_parameters: WiFiDirectConnectionParameters, /) -> windows_foundation.IAsyncOperation[WiFiDirectDevice]: ...
    @typing.overload
    # System.String Windows.Devices.WiFiDirect.WiFiDirectDevice::GetDeviceSelector()
    def get_device_selector(cls) -> str: ...
    @typing.overload
    # System.String Windows.Devices.WiFiDirect.WiFiDirectDevice::GetDeviceSelector(Windows.Devices.WiFiDirect.WiFiDirectDeviceSelectorType)
    def get_device_selector(cls, type: WiFiDirectDeviceSelectorType, /) -> str: ...

@typing.final
class WiFiDirectDevice(winrt.system.Object, windows_foundation.IClosable, metaclass=WiFiDirectDevice_Static):
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, exc_type: typing.Optional[typing.Type[BaseException]], exc_value: typing.Optional[BaseException], traceback: typing.Optional[types.TracebackType]) -> None: ...
    # System.Void Windows.Devices.WiFiDirect.WiFiDirectDevice::Close()
    def close(self) -> None: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Networking.EndpointPair> Windows.Devices.WiFiDirect.WiFiDirectDevice::GetConnectionEndpointPairs()
    def get_connection_endpoint_pairs(self) -> typing.Sequence[windows_networking.EndpointPair]: ...
    # Windows.Foundation.EventRegistrationToken Windows.Devices.WiFiDirect.WiFiDirectDevice::add_ConnectionStatusChanged(Windows.Foundation.TypedEventHandler`2<Windows.Devices.WiFiDirect.WiFiDirectDevice,System.Object>)
    def add_connection_status_changed(self, handler: windows_foundation.TypedEventHandler[WiFiDirectDevice, winrt.system.Object], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.Devices.WiFiDirect.WiFiDirectDevice::remove_ConnectionStatusChanged(Windows.Foundation.EventRegistrationToken)
    def remove_connection_status_changed(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Devices.WiFiDirect.WiFiDirectConnectionStatus Windows.Devices.WiFiDirect.WiFiDirectDevice::get_ConnectionStatus()
    @_property
    def connection_status(self) -> WiFiDirectConnectionStatus: ...
    # System.String Windows.Devices.WiFiDirect.WiFiDirectDevice::get_DeviceId()
    @_property
    def device_id(self) -> str: ...

@typing.final
class WiFiDirectInformationElement_Static(winrt._winrt.IInspectable_Static):
    # Windows.Foundation.Collections.IVector`1<Windows.Devices.WiFiDirect.WiFiDirectInformationElement> Windows.Devices.WiFiDirect.WiFiDirectInformationElement::CreateFromBuffer(Windows.Storage.Streams.IBuffer)
    def create_from_buffer(cls, buffer: winrt.system.Buffer, /) -> typing.MutableSequence[WiFiDirectInformationElement]: ...
    # Windows.Foundation.Collections.IVector`1<Windows.Devices.WiFiDirect.WiFiDirectInformationElement> Windows.Devices.WiFiDirect.WiFiDirectInformationElement::CreateFromDeviceInformation(Windows.Devices.Enumeration.DeviceInformation)
    def create_from_device_information(cls, device_information: windows_devices_enumeration.DeviceInformation, /) -> typing.MutableSequence[WiFiDirectInformationElement]: ...

@typing.final
class WiFiDirectInformationElement(winrt.system.Object, metaclass=WiFiDirectInformationElement_Static):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # Windows.Storage.Streams.IBuffer Windows.Devices.WiFiDirect.WiFiDirectInformationElement::get_Value()
    @_property
    def value(self) -> winrt.system.Buffer: ...
    # System.Void Windows.Devices.WiFiDirect.WiFiDirectInformationElement::put_Value(Windows.Storage.Streams.IBuffer)
    @value.setter
    def value(self, value: winrt.system.Buffer) -> None: ...
    # System.Byte Windows.Devices.WiFiDirect.WiFiDirectInformationElement::get_OuiType()
    @_property
    def oui_type(self) -> winrt.system.UInt8: ...
    # System.Void Windows.Devices.WiFiDirect.WiFiDirectInformationElement::put_OuiType(System.Byte)
    @oui_type.setter
    def oui_type(self, value: winrt.system.UInt8) -> None: ...
    # Windows.Storage.Streams.IBuffer Windows.Devices.WiFiDirect.WiFiDirectInformationElement::get_Oui()
    @_property
    def oui(self) -> winrt.system.Buffer: ...
    # System.Void Windows.Devices.WiFiDirect.WiFiDirectInformationElement::put_Oui(Windows.Storage.Streams.IBuffer)
    @oui.setter
    def oui(self, value: winrt.system.Buffer) -> None: ...

@typing.final
class WiFiDirectLegacySettings(winrt.system.Object):
    # System.String Windows.Devices.WiFiDirect.WiFiDirectLegacySettings::get_Ssid()
    @_property
    def ssid(self) -> str: ...
    # System.Void Windows.Devices.WiFiDirect.WiFiDirectLegacySettings::put_Ssid(System.String)
    @ssid.setter
    def ssid(self, value: str) -> None: ...
    # Windows.Security.Credentials.PasswordCredential Windows.Devices.WiFiDirect.WiFiDirectLegacySettings::get_Passphrase()
    @_property
    def passphrase(self) -> windows_security_credentials.PasswordCredential: ...
    # System.Void Windows.Devices.WiFiDirect.WiFiDirectLegacySettings::put_Passphrase(Windows.Security.Credentials.PasswordCredential)
    @passphrase.setter
    def passphrase(self, value: windows_security_credentials.PasswordCredential) -> None: ...
    # System.Boolean Windows.Devices.WiFiDirect.WiFiDirectLegacySettings::get_IsEnabled()
    @_property
    def is_enabled(self) -> bool: ...
    # System.Void Windows.Devices.WiFiDirect.WiFiDirectLegacySettings::put_IsEnabled(System.Boolean)
    @is_enabled.setter
    def is_enabled(self, value: bool) -> None: ...

