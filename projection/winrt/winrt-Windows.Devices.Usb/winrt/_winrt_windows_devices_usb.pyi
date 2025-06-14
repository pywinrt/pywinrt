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

from winrt.windows.devices.usb import UsbControlRecipient, UsbControlTransferType, UsbEndpointType, UsbReadOptions, UsbTransferDirection, UsbWriteOptions

Self = typing.TypeVar('Self')

@typing.final
class UsbBulkInEndpointDescriptor(winrt.system.Object):
    # System.Byte Windows.Devices.Usb.UsbBulkInEndpointDescriptor::get_EndpointNumber()
    @_property
    def endpoint_number(self) -> winrt.system.UInt8: ...
    # System.UInt32 Windows.Devices.Usb.UsbBulkInEndpointDescriptor::get_MaxPacketSize()
    @_property
    def max_packet_size(self) -> winrt.system.UInt32: ...
    # Windows.Devices.Usb.UsbBulkInPipe Windows.Devices.Usb.UsbBulkInEndpointDescriptor::get_Pipe()
    @_property
    def pipe(self) -> UsbBulkInPipe: ...

@typing.final
class UsbBulkInPipe(winrt.system.Object):
    # Windows.Foundation.IAsyncAction Windows.Devices.Usb.UsbBulkInPipe::ClearStallAsync()
    def clear_stall_async(self) -> windows_foundation.IAsyncAction: ...
    # System.Void Windows.Devices.Usb.UsbBulkInPipe::FlushBuffer()
    def flush_buffer(self) -> None: ...
    # Windows.Devices.Usb.UsbReadOptions Windows.Devices.Usb.UsbBulkInPipe::get_ReadOptions()
    @_property
    def read_options(self) -> UsbReadOptions: ...
    # System.Void Windows.Devices.Usb.UsbBulkInPipe::put_ReadOptions(Windows.Devices.Usb.UsbReadOptions)
    @read_options.setter
    def read_options(self, value: UsbReadOptions) -> None: ...
    # Windows.Devices.Usb.UsbBulkInEndpointDescriptor Windows.Devices.Usb.UsbBulkInPipe::get_EndpointDescriptor()
    @_property
    def endpoint_descriptor(self) -> UsbBulkInEndpointDescriptor: ...
    # Windows.Storage.Streams.IInputStream Windows.Devices.Usb.UsbBulkInPipe::get_InputStream()
    @_property
    def input_stream(self) -> windows_storage_streams.IInputStream: ...
    # System.UInt32 Windows.Devices.Usb.UsbBulkInPipe::get_MaxTransferSizeBytes()
    @_property
    def max_transfer_size_bytes(self) -> winrt.system.UInt32: ...

@typing.final
class UsbBulkOutEndpointDescriptor(winrt.system.Object):
    # System.Byte Windows.Devices.Usb.UsbBulkOutEndpointDescriptor::get_EndpointNumber()
    @_property
    def endpoint_number(self) -> winrt.system.UInt8: ...
    # System.UInt32 Windows.Devices.Usb.UsbBulkOutEndpointDescriptor::get_MaxPacketSize()
    @_property
    def max_packet_size(self) -> winrt.system.UInt32: ...
    # Windows.Devices.Usb.UsbBulkOutPipe Windows.Devices.Usb.UsbBulkOutEndpointDescriptor::get_Pipe()
    @_property
    def pipe(self) -> UsbBulkOutPipe: ...

@typing.final
class UsbBulkOutPipe(winrt.system.Object):
    # Windows.Foundation.IAsyncAction Windows.Devices.Usb.UsbBulkOutPipe::ClearStallAsync()
    def clear_stall_async(self) -> windows_foundation.IAsyncAction: ...
    # Windows.Devices.Usb.UsbWriteOptions Windows.Devices.Usb.UsbBulkOutPipe::get_WriteOptions()
    @_property
    def write_options(self) -> UsbWriteOptions: ...
    # System.Void Windows.Devices.Usb.UsbBulkOutPipe::put_WriteOptions(Windows.Devices.Usb.UsbWriteOptions)
    @write_options.setter
    def write_options(self, value: UsbWriteOptions) -> None: ...
    # Windows.Devices.Usb.UsbBulkOutEndpointDescriptor Windows.Devices.Usb.UsbBulkOutPipe::get_EndpointDescriptor()
    @_property
    def endpoint_descriptor(self) -> UsbBulkOutEndpointDescriptor: ...
    # Windows.Storage.Streams.IOutputStream Windows.Devices.Usb.UsbBulkOutPipe::get_OutputStream()
    @_property
    def output_stream(self) -> windows_storage_streams.IOutputStream: ...

@typing.final
class UsbConfiguration(winrt.system.Object):
    # Windows.Devices.Usb.UsbConfigurationDescriptor Windows.Devices.Usb.UsbConfiguration::get_ConfigurationDescriptor()
    @_property
    def configuration_descriptor(self) -> UsbConfigurationDescriptor: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Devices.Usb.UsbDescriptor> Windows.Devices.Usb.UsbConfiguration::get_Descriptors()
    @_property
    def descriptors(self) -> typing.Sequence[UsbDescriptor]: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Devices.Usb.UsbInterface> Windows.Devices.Usb.UsbConfiguration::get_UsbInterfaces()
    @_property
    def usb_interfaces(self) -> typing.Sequence[UsbInterface]: ...

@typing.final
class UsbConfigurationDescriptor_Static(winrt._winrt.IInspectable_Static):
    # Windows.Devices.Usb.UsbConfigurationDescriptor Windows.Devices.Usb.UsbConfigurationDescriptor::Parse(Windows.Devices.Usb.UsbDescriptor)
    def parse(cls, descriptor: UsbDescriptor, /) -> UsbConfigurationDescriptor: ...
    # System.Boolean Windows.Devices.Usb.UsbConfigurationDescriptor::TryParse(Windows.Devices.Usb.UsbDescriptor,Windows.Devices.Usb.UsbConfigurationDescriptor&)
    def try_parse(cls, descriptor: UsbDescriptor, /) -> typing.Tuple[bool, UsbConfigurationDescriptor]: ...

@typing.final
class UsbConfigurationDescriptor(winrt.system.Object, metaclass=UsbConfigurationDescriptor_Static):
    # System.Byte Windows.Devices.Usb.UsbConfigurationDescriptor::get_ConfigurationValue()
    @_property
    def configuration_value(self) -> winrt.system.UInt8: ...
    # System.UInt32 Windows.Devices.Usb.UsbConfigurationDescriptor::get_MaxPowerMilliamps()
    @_property
    def max_power_milliamps(self) -> winrt.system.UInt32: ...
    # System.Boolean Windows.Devices.Usb.UsbConfigurationDescriptor::get_RemoteWakeup()
    @_property
    def remote_wakeup(self) -> bool: ...
    # System.Boolean Windows.Devices.Usb.UsbConfigurationDescriptor::get_SelfPowered()
    @_property
    def self_powered(self) -> bool: ...

@typing.final
class UsbControlRequestType(winrt.system.Object):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # Windows.Devices.Usb.UsbControlRecipient Windows.Devices.Usb.UsbControlRequestType::get_Recipient()
    @_property
    def recipient(self) -> UsbControlRecipient: ...
    # System.Void Windows.Devices.Usb.UsbControlRequestType::put_Recipient(Windows.Devices.Usb.UsbControlRecipient)
    @recipient.setter
    def recipient(self, value: UsbControlRecipient) -> None: ...
    # Windows.Devices.Usb.UsbTransferDirection Windows.Devices.Usb.UsbControlRequestType::get_Direction()
    @_property
    def direction(self) -> UsbTransferDirection: ...
    # System.Void Windows.Devices.Usb.UsbControlRequestType::put_Direction(Windows.Devices.Usb.UsbTransferDirection)
    @direction.setter
    def direction(self, value: UsbTransferDirection) -> None: ...
    # Windows.Devices.Usb.UsbControlTransferType Windows.Devices.Usb.UsbControlRequestType::get_ControlTransferType()
    @_property
    def control_transfer_type(self) -> UsbControlTransferType: ...
    # System.Void Windows.Devices.Usb.UsbControlRequestType::put_ControlTransferType(Windows.Devices.Usb.UsbControlTransferType)
    @control_transfer_type.setter
    def control_transfer_type(self, value: UsbControlTransferType) -> None: ...
    # System.Byte Windows.Devices.Usb.UsbControlRequestType::get_AsByte()
    @_property
    def as_byte(self) -> winrt.system.UInt8: ...
    # System.Void Windows.Devices.Usb.UsbControlRequestType::put_AsByte(System.Byte)
    @as_byte.setter
    def as_byte(self, value: winrt.system.UInt8) -> None: ...

@typing.final
class UsbDescriptor(winrt.system.Object):
    # System.Void Windows.Devices.Usb.UsbDescriptor::ReadDescriptorBuffer(Windows.Storage.Streams.IBuffer)
    def read_descriptor_buffer(self, buffer: winrt.system.Buffer, /) -> None: ...
    # System.Byte Windows.Devices.Usb.UsbDescriptor::get_DescriptorType()
    @_property
    def descriptor_type(self) -> winrt.system.UInt8: ...
    # System.Byte Windows.Devices.Usb.UsbDescriptor::get_Length()
    @_property
    def length(self) -> winrt.system.UInt8: ...

@typing.final
class UsbDevice_Static(winrt._winrt.IInspectable_Static):
    # Windows.Foundation.IAsyncOperation`1<Windows.Devices.Usb.UsbDevice> Windows.Devices.Usb.UsbDevice::FromIdAsync(System.String)
    def from_id_async(cls, device_id: str, /) -> windows_foundation.IAsyncOperation[UsbDevice]: ...
    # System.String Windows.Devices.Usb.UsbDevice::GetDeviceClassSelector(Windows.Devices.Usb.UsbDeviceClass)
    def get_device_class_selector(cls, usb_class: UsbDeviceClass, /) -> str: ...
    # System.String Windows.Devices.Usb.UsbDevice::GetDeviceSelector(System.UInt32,System.UInt32,System.Guid)
    def get_device_selector(cls, vendor_id: winrt.system.UInt32, product_id: winrt.system.UInt32, win_usb_interface_class: _uuid.UUID, /) -> str: ...
    # System.String Windows.Devices.Usb.UsbDevice::GetDeviceSelector(System.Guid)
    def get_device_selector_guid_only(cls, win_usb_interface_class: _uuid.UUID, /) -> str: ...
    # System.String Windows.Devices.Usb.UsbDevice::GetDeviceSelector(System.UInt32,System.UInt32)
    def get_device_selector_vid_pid_only(cls, vendor_id: winrt.system.UInt32, product_id: winrt.system.UInt32, /) -> str: ...

@typing.final
class UsbDevice(winrt.system.Object, windows_foundation.IClosable, metaclass=UsbDevice_Static):
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, exc_type: typing.Optional[typing.Type[BaseException]], exc_value: typing.Optional[BaseException], traceback: typing.Optional[types.TracebackType]) -> None: ...
    # System.Void Windows.Devices.Usb.UsbDevice::Close()
    def close(self) -> None: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Storage.Streams.IBuffer> Windows.Devices.Usb.UsbDevice::SendControlInTransferAsync(Windows.Devices.Usb.UsbSetupPacket,Windows.Storage.Streams.IBuffer)
    def send_control_in_transfer_async(self, setup_packet: UsbSetupPacket, buffer: winrt.system.Buffer, /) -> windows_foundation.IAsyncOperation[winrt.system.Buffer]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Storage.Streams.IBuffer> Windows.Devices.Usb.UsbDevice::SendControlInTransferAsync(Windows.Devices.Usb.UsbSetupPacket)
    def send_control_in_transfer_async_no_buffer(self, setup_packet: UsbSetupPacket, /) -> windows_foundation.IAsyncOperation[winrt.system.Buffer]: ...
    # Windows.Foundation.IAsyncOperation`1<System.UInt32> Windows.Devices.Usb.UsbDevice::SendControlOutTransferAsync(Windows.Devices.Usb.UsbSetupPacket,Windows.Storage.Streams.IBuffer)
    def send_control_out_transfer_async(self, setup_packet: UsbSetupPacket, buffer: winrt.system.Buffer, /) -> windows_foundation.IAsyncOperation[winrt.system.UInt32]: ...
    # Windows.Foundation.IAsyncOperation`1<System.UInt32> Windows.Devices.Usb.UsbDevice::SendControlOutTransferAsync(Windows.Devices.Usb.UsbSetupPacket)
    def send_control_out_transfer_async_no_buffer(self, setup_packet: UsbSetupPacket, /) -> windows_foundation.IAsyncOperation[winrt.system.UInt32]: ...
    # Windows.Devices.Usb.UsbConfiguration Windows.Devices.Usb.UsbDevice::get_Configuration()
    @_property
    def configuration(self) -> UsbConfiguration: ...
    # Windows.Devices.Usb.UsbInterface Windows.Devices.Usb.UsbDevice::get_DefaultInterface()
    @_property
    def default_interface(self) -> UsbInterface: ...
    # Windows.Devices.Usb.UsbDeviceDescriptor Windows.Devices.Usb.UsbDevice::get_DeviceDescriptor()
    @_property
    def device_descriptor(self) -> UsbDeviceDescriptor: ...

@typing.final
class UsbDeviceClass(winrt.system.Object):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # Windows.Foundation.IReference`1<System.Byte> Windows.Devices.Usb.UsbDeviceClass::get_SubclassCode()
    @_property
    def subclass_code(self) -> typing.Optional[winrt.system.UInt8]: ...
    # System.Void Windows.Devices.Usb.UsbDeviceClass::put_SubclassCode(Windows.Foundation.IReference`1<System.Byte>)
    @subclass_code.setter
    def subclass_code(self, value: typing.Optional[winrt.system.UInt8]) -> None: ...
    # Windows.Foundation.IReference`1<System.Byte> Windows.Devices.Usb.UsbDeviceClass::get_ProtocolCode()
    @_property
    def protocol_code(self) -> typing.Optional[winrt.system.UInt8]: ...
    # System.Void Windows.Devices.Usb.UsbDeviceClass::put_ProtocolCode(Windows.Foundation.IReference`1<System.Byte>)
    @protocol_code.setter
    def protocol_code(self, value: typing.Optional[winrt.system.UInt8]) -> None: ...
    # System.Byte Windows.Devices.Usb.UsbDeviceClass::get_ClassCode()
    @_property
    def class_code(self) -> winrt.system.UInt8: ...
    # System.Void Windows.Devices.Usb.UsbDeviceClass::put_ClassCode(System.Byte)
    @class_code.setter
    def class_code(self, value: winrt.system.UInt8) -> None: ...

@typing.final
class UsbDeviceClasses_Static(winrt._winrt.IInspectable_Static):
    # Windows.Devices.Usb.UsbDeviceClass Windows.Devices.Usb.UsbDeviceClasses::get_ActiveSync()
    @_property
    def active_sync(cls) -> UsbDeviceClass: ...
    # Windows.Devices.Usb.UsbDeviceClass Windows.Devices.Usb.UsbDeviceClasses::get_CdcControl()
    @_property
    def cdc_control(cls) -> UsbDeviceClass: ...
    # Windows.Devices.Usb.UsbDeviceClass Windows.Devices.Usb.UsbDeviceClasses::get_DeviceFirmwareUpdate()
    @_property
    def device_firmware_update(cls) -> UsbDeviceClass: ...
    # Windows.Devices.Usb.UsbDeviceClass Windows.Devices.Usb.UsbDeviceClasses::get_Irda()
    @_property
    def irda(cls) -> UsbDeviceClass: ...
    # Windows.Devices.Usb.UsbDeviceClass Windows.Devices.Usb.UsbDeviceClasses::get_Measurement()
    @_property
    def measurement(cls) -> UsbDeviceClass: ...
    # Windows.Devices.Usb.UsbDeviceClass Windows.Devices.Usb.UsbDeviceClasses::get_PalmSync()
    @_property
    def palm_sync(cls) -> UsbDeviceClass: ...
    # Windows.Devices.Usb.UsbDeviceClass Windows.Devices.Usb.UsbDeviceClasses::get_PersonalHealthcare()
    @_property
    def personal_healthcare(cls) -> UsbDeviceClass: ...
    # Windows.Devices.Usb.UsbDeviceClass Windows.Devices.Usb.UsbDeviceClasses::get_Physical()
    @_property
    def physical(cls) -> UsbDeviceClass: ...
    # Windows.Devices.Usb.UsbDeviceClass Windows.Devices.Usb.UsbDeviceClasses::get_VendorSpecific()
    @_property
    def vendor_specific(cls) -> UsbDeviceClass: ...

@typing.final
class UsbDeviceClasses(winrt.system.Object, metaclass=UsbDeviceClasses_Static):
    pass

@typing.final
class UsbDeviceDescriptor(winrt.system.Object):
    # System.UInt32 Windows.Devices.Usb.UsbDeviceDescriptor::get_BcdDeviceRevision()
    @_property
    def bcd_device_revision(self) -> winrt.system.UInt32: ...
    # System.UInt32 Windows.Devices.Usb.UsbDeviceDescriptor::get_BcdUsb()
    @_property
    def bcd_usb(self) -> winrt.system.UInt32: ...
    # System.Byte Windows.Devices.Usb.UsbDeviceDescriptor::get_MaxPacketSize0()
    @_property
    def max_packet_size0(self) -> winrt.system.UInt8: ...
    # System.Byte Windows.Devices.Usb.UsbDeviceDescriptor::get_NumberOfConfigurations()
    @_property
    def number_of_configurations(self) -> winrt.system.UInt8: ...
    # System.UInt32 Windows.Devices.Usb.UsbDeviceDescriptor::get_ProductId()
    @_property
    def product_id(self) -> winrt.system.UInt32: ...
    # System.UInt32 Windows.Devices.Usb.UsbDeviceDescriptor::get_VendorId()
    @_property
    def vendor_id(self) -> winrt.system.UInt32: ...

@typing.final
class UsbEndpointDescriptor_Static(winrt._winrt.IInspectable_Static):
    # Windows.Devices.Usb.UsbEndpointDescriptor Windows.Devices.Usb.UsbEndpointDescriptor::Parse(Windows.Devices.Usb.UsbDescriptor)
    def parse(cls, descriptor: UsbDescriptor, /) -> UsbEndpointDescriptor: ...
    # System.Boolean Windows.Devices.Usb.UsbEndpointDescriptor::TryParse(Windows.Devices.Usb.UsbDescriptor,Windows.Devices.Usb.UsbEndpointDescriptor&)
    def try_parse(cls, descriptor: UsbDescriptor, /) -> typing.Tuple[bool, UsbEndpointDescriptor]: ...

@typing.final
class UsbEndpointDescriptor(winrt.system.Object, metaclass=UsbEndpointDescriptor_Static):
    # Windows.Devices.Usb.UsbBulkInEndpointDescriptor Windows.Devices.Usb.UsbEndpointDescriptor::get_AsBulkInEndpointDescriptor()
    @_property
    def as_bulk_in_endpoint_descriptor(self) -> UsbBulkInEndpointDescriptor: ...
    # Windows.Devices.Usb.UsbBulkOutEndpointDescriptor Windows.Devices.Usb.UsbEndpointDescriptor::get_AsBulkOutEndpointDescriptor()
    @_property
    def as_bulk_out_endpoint_descriptor(self) -> UsbBulkOutEndpointDescriptor: ...
    # Windows.Devices.Usb.UsbInterruptInEndpointDescriptor Windows.Devices.Usb.UsbEndpointDescriptor::get_AsInterruptInEndpointDescriptor()
    @_property
    def as_interrupt_in_endpoint_descriptor(self) -> UsbInterruptInEndpointDescriptor: ...
    # Windows.Devices.Usb.UsbInterruptOutEndpointDescriptor Windows.Devices.Usb.UsbEndpointDescriptor::get_AsInterruptOutEndpointDescriptor()
    @_property
    def as_interrupt_out_endpoint_descriptor(self) -> UsbInterruptOutEndpointDescriptor: ...
    # Windows.Devices.Usb.UsbTransferDirection Windows.Devices.Usb.UsbEndpointDescriptor::get_Direction()
    @_property
    def direction(self) -> UsbTransferDirection: ...
    # System.Byte Windows.Devices.Usb.UsbEndpointDescriptor::get_EndpointNumber()
    @_property
    def endpoint_number(self) -> winrt.system.UInt8: ...
    # Windows.Devices.Usb.UsbEndpointType Windows.Devices.Usb.UsbEndpointDescriptor::get_EndpointType()
    @_property
    def endpoint_type(self) -> UsbEndpointType: ...

@typing.final
class UsbInterface(winrt.system.Object):
    # Windows.Foundation.Collections.IVectorView`1<Windows.Devices.Usb.UsbBulkInPipe> Windows.Devices.Usb.UsbInterface::get_BulkInPipes()
    @_property
    def bulk_in_pipes(self) -> typing.Sequence[UsbBulkInPipe]: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Devices.Usb.UsbBulkOutPipe> Windows.Devices.Usb.UsbInterface::get_BulkOutPipes()
    @_property
    def bulk_out_pipes(self) -> typing.Sequence[UsbBulkOutPipe]: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Devices.Usb.UsbDescriptor> Windows.Devices.Usb.UsbInterface::get_Descriptors()
    @_property
    def descriptors(self) -> typing.Sequence[UsbDescriptor]: ...
    # System.Byte Windows.Devices.Usb.UsbInterface::get_InterfaceNumber()
    @_property
    def interface_number(self) -> winrt.system.UInt8: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Devices.Usb.UsbInterfaceSetting> Windows.Devices.Usb.UsbInterface::get_InterfaceSettings()
    @_property
    def interface_settings(self) -> typing.Sequence[UsbInterfaceSetting]: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Devices.Usb.UsbInterruptInPipe> Windows.Devices.Usb.UsbInterface::get_InterruptInPipes()
    @_property
    def interrupt_in_pipes(self) -> typing.Sequence[UsbInterruptInPipe]: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Devices.Usb.UsbInterruptOutPipe> Windows.Devices.Usb.UsbInterface::get_InterruptOutPipes()
    @_property
    def interrupt_out_pipes(self) -> typing.Sequence[UsbInterruptOutPipe]: ...

@typing.final
class UsbInterfaceDescriptor_Static(winrt._winrt.IInspectable_Static):
    # Windows.Devices.Usb.UsbInterfaceDescriptor Windows.Devices.Usb.UsbInterfaceDescriptor::Parse(Windows.Devices.Usb.UsbDescriptor)
    def parse(cls, descriptor: UsbDescriptor, /) -> UsbInterfaceDescriptor: ...
    # System.Boolean Windows.Devices.Usb.UsbInterfaceDescriptor::TryParse(Windows.Devices.Usb.UsbDescriptor,Windows.Devices.Usb.UsbInterfaceDescriptor&)
    def try_parse(cls, descriptor: UsbDescriptor, /) -> typing.Tuple[bool, UsbInterfaceDescriptor]: ...

@typing.final
class UsbInterfaceDescriptor(winrt.system.Object, metaclass=UsbInterfaceDescriptor_Static):
    # System.Byte Windows.Devices.Usb.UsbInterfaceDescriptor::get_AlternateSettingNumber()
    @_property
    def alternate_setting_number(self) -> winrt.system.UInt8: ...
    # System.Byte Windows.Devices.Usb.UsbInterfaceDescriptor::get_ClassCode()
    @_property
    def class_code(self) -> winrt.system.UInt8: ...
    # System.Byte Windows.Devices.Usb.UsbInterfaceDescriptor::get_InterfaceNumber()
    @_property
    def interface_number(self) -> winrt.system.UInt8: ...
    # System.Byte Windows.Devices.Usb.UsbInterfaceDescriptor::get_ProtocolCode()
    @_property
    def protocol_code(self) -> winrt.system.UInt8: ...
    # System.Byte Windows.Devices.Usb.UsbInterfaceDescriptor::get_SubclassCode()
    @_property
    def subclass_code(self) -> winrt.system.UInt8: ...

@typing.final
class UsbInterfaceSetting(winrt.system.Object):
    # Windows.Foundation.IAsyncAction Windows.Devices.Usb.UsbInterfaceSetting::SelectSettingAsync()
    def select_setting_async(self) -> windows_foundation.IAsyncAction: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Devices.Usb.UsbBulkInEndpointDescriptor> Windows.Devices.Usb.UsbInterfaceSetting::get_BulkInEndpoints()
    @_property
    def bulk_in_endpoints(self) -> typing.Sequence[UsbBulkInEndpointDescriptor]: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Devices.Usb.UsbBulkOutEndpointDescriptor> Windows.Devices.Usb.UsbInterfaceSetting::get_BulkOutEndpoints()
    @_property
    def bulk_out_endpoints(self) -> typing.Sequence[UsbBulkOutEndpointDescriptor]: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Devices.Usb.UsbDescriptor> Windows.Devices.Usb.UsbInterfaceSetting::get_Descriptors()
    @_property
    def descriptors(self) -> typing.Sequence[UsbDescriptor]: ...
    # Windows.Devices.Usb.UsbInterfaceDescriptor Windows.Devices.Usb.UsbInterfaceSetting::get_InterfaceDescriptor()
    @_property
    def interface_descriptor(self) -> UsbInterfaceDescriptor: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Devices.Usb.UsbInterruptInEndpointDescriptor> Windows.Devices.Usb.UsbInterfaceSetting::get_InterruptInEndpoints()
    @_property
    def interrupt_in_endpoints(self) -> typing.Sequence[UsbInterruptInEndpointDescriptor]: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Devices.Usb.UsbInterruptOutEndpointDescriptor> Windows.Devices.Usb.UsbInterfaceSetting::get_InterruptOutEndpoints()
    @_property
    def interrupt_out_endpoints(self) -> typing.Sequence[UsbInterruptOutEndpointDescriptor]: ...
    # System.Boolean Windows.Devices.Usb.UsbInterfaceSetting::get_Selected()
    @_property
    def selected(self) -> bool: ...

@typing.final
class UsbInterruptInEndpointDescriptor(winrt.system.Object):
    # System.Byte Windows.Devices.Usb.UsbInterruptInEndpointDescriptor::get_EndpointNumber()
    @_property
    def endpoint_number(self) -> winrt.system.UInt8: ...
    # Windows.Foundation.TimeSpan Windows.Devices.Usb.UsbInterruptInEndpointDescriptor::get_Interval()
    @_property
    def interval(self) -> datetime.timedelta: ...
    # System.UInt32 Windows.Devices.Usb.UsbInterruptInEndpointDescriptor::get_MaxPacketSize()
    @_property
    def max_packet_size(self) -> winrt.system.UInt32: ...
    # Windows.Devices.Usb.UsbInterruptInPipe Windows.Devices.Usb.UsbInterruptInEndpointDescriptor::get_Pipe()
    @_property
    def pipe(self) -> UsbInterruptInPipe: ...

@typing.final
class UsbInterruptInEventArgs(winrt.system.Object):
    # Windows.Storage.Streams.IBuffer Windows.Devices.Usb.UsbInterruptInEventArgs::get_InterruptData()
    @_property
    def interrupt_data(self) -> winrt.system.Buffer: ...

@typing.final
class UsbInterruptInPipe(winrt.system.Object):
    # Windows.Foundation.IAsyncAction Windows.Devices.Usb.UsbInterruptInPipe::ClearStallAsync()
    def clear_stall_async(self) -> windows_foundation.IAsyncAction: ...
    # Windows.Foundation.EventRegistrationToken Windows.Devices.Usb.UsbInterruptInPipe::add_DataReceived(Windows.Foundation.TypedEventHandler`2<Windows.Devices.Usb.UsbInterruptInPipe,Windows.Devices.Usb.UsbInterruptInEventArgs>)
    def add_data_received(self, handler: windows_foundation.TypedEventHandler[UsbInterruptInPipe, UsbInterruptInEventArgs], /) -> windows_foundation.EventRegistrationToken: ...
    # System.Void Windows.Devices.Usb.UsbInterruptInPipe::remove_DataReceived(Windows.Foundation.EventRegistrationToken)
    def remove_data_received(self, token: typing.Union[windows_foundation.EventRegistrationToken, typing.Tuple[winrt.system.Int64]], /) -> None: ...
    # Windows.Devices.Usb.UsbInterruptInEndpointDescriptor Windows.Devices.Usb.UsbInterruptInPipe::get_EndpointDescriptor()
    @_property
    def endpoint_descriptor(self) -> UsbInterruptInEndpointDescriptor: ...

@typing.final
class UsbInterruptOutEndpointDescriptor(winrt.system.Object):
    # System.Byte Windows.Devices.Usb.UsbInterruptOutEndpointDescriptor::get_EndpointNumber()
    @_property
    def endpoint_number(self) -> winrt.system.UInt8: ...
    # Windows.Foundation.TimeSpan Windows.Devices.Usb.UsbInterruptOutEndpointDescriptor::get_Interval()
    @_property
    def interval(self) -> datetime.timedelta: ...
    # System.UInt32 Windows.Devices.Usb.UsbInterruptOutEndpointDescriptor::get_MaxPacketSize()
    @_property
    def max_packet_size(self) -> winrt.system.UInt32: ...
    # Windows.Devices.Usb.UsbInterruptOutPipe Windows.Devices.Usb.UsbInterruptOutEndpointDescriptor::get_Pipe()
    @_property
    def pipe(self) -> UsbInterruptOutPipe: ...

@typing.final
class UsbInterruptOutPipe(winrt.system.Object):
    # Windows.Foundation.IAsyncAction Windows.Devices.Usb.UsbInterruptOutPipe::ClearStallAsync()
    def clear_stall_async(self) -> windows_foundation.IAsyncAction: ...
    # Windows.Devices.Usb.UsbWriteOptions Windows.Devices.Usb.UsbInterruptOutPipe::get_WriteOptions()
    @_property
    def write_options(self) -> UsbWriteOptions: ...
    # System.Void Windows.Devices.Usb.UsbInterruptOutPipe::put_WriteOptions(Windows.Devices.Usb.UsbWriteOptions)
    @write_options.setter
    def write_options(self, value: UsbWriteOptions) -> None: ...
    # Windows.Devices.Usb.UsbInterruptOutEndpointDescriptor Windows.Devices.Usb.UsbInterruptOutPipe::get_EndpointDescriptor()
    @_property
    def endpoint_descriptor(self) -> UsbInterruptOutEndpointDescriptor: ...
    # Windows.Storage.Streams.IOutputStream Windows.Devices.Usb.UsbInterruptOutPipe::get_OutputStream()
    @_property
    def output_stream(self) -> windows_storage_streams.IOutputStream: ...

@typing.final
class UsbSetupPacket(winrt.system.Object):
    @typing.overload
    def __new__(cls: typing.Type[Self], eight_byte_buffer: winrt.system.Buffer) -> Self: ...
    @typing.overload
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # System.UInt32 Windows.Devices.Usb.UsbSetupPacket::get_Value()
    @_property
    def value(self) -> winrt.system.UInt32: ...
    # System.Void Windows.Devices.Usb.UsbSetupPacket::put_Value(System.UInt32)
    @value.setter
    def value(self, value: winrt.system.UInt32) -> None: ...
    # Windows.Devices.Usb.UsbControlRequestType Windows.Devices.Usb.UsbSetupPacket::get_RequestType()
    @_property
    def request_type(self) -> UsbControlRequestType: ...
    # System.Void Windows.Devices.Usb.UsbSetupPacket::put_RequestType(Windows.Devices.Usb.UsbControlRequestType)
    @request_type.setter
    def request_type(self, value: UsbControlRequestType) -> None: ...
    # System.Byte Windows.Devices.Usb.UsbSetupPacket::get_Request()
    @_property
    def request(self) -> winrt.system.UInt8: ...
    # System.Void Windows.Devices.Usb.UsbSetupPacket::put_Request(System.Byte)
    @request.setter
    def request(self, value: winrt.system.UInt8) -> None: ...
    # System.UInt32 Windows.Devices.Usb.UsbSetupPacket::get_Length()
    @_property
    def length(self) -> winrt.system.UInt32: ...
    # System.Void Windows.Devices.Usb.UsbSetupPacket::put_Length(System.UInt32)
    @length.setter
    def length(self, value: winrt.system.UInt32) -> None: ...
    # System.UInt32 Windows.Devices.Usb.UsbSetupPacket::get_Index()
    @_property
    def index(self) -> winrt.system.UInt32: ...
    # System.Void Windows.Devices.Usb.UsbSetupPacket::put_Index(System.UInt32)
    @index.setter
    def index(self, value: winrt.system.UInt32) -> None: ...

