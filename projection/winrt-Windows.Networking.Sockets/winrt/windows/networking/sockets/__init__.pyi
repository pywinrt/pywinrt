# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum
import datetime
import sys
import types
import typing

import winrt.system
import winrt.windows.applicationmodel.background
import winrt.windows.foundation
import winrt.windows.foundation.collections
import winrt.windows.networking
import winrt.windows.networking.connectivity
import winrt.windows.security.credentials
import winrt.windows.security.cryptography.certificates
import winrt.windows.storage.streams
import winrt.windows.web

class ControlChannelTriggerResetReason(enum.IntEnum):
    FAST_USER_SWITCHED = 0
    LOW_POWER_EXIT = 1
    QUIET_HOURS_EXIT = 2
    APPLICATION_RESTART = 3

class ControlChannelTriggerResourceType(enum.IntEnum):
    REQUEST_SOFTWARE_SLOT = 0
    REQUEST_HARDWARE_SLOT = 1

class ControlChannelTriggerStatus(enum.IntEnum):
    HARDWARE_SLOT_REQUESTED = 0
    SOFTWARE_SLOT_ALLOCATED = 1
    HARDWARE_SLOT_ALLOCATED = 2
    POLICY_ERROR = 3
    SYSTEM_ERROR = 4
    TRANSPORT_DISCONNECTED = 5
    SERVICE_UNAVAILABLE = 6

class MessageWebSocketReceiveMode(enum.IntEnum):
    FULL_MESSAGE = 0
    PARTIAL_MESSAGE = 1

class SocketActivityConnectedStandbyAction(enum.IntEnum):
    DO_NOT_WAKE = 0
    WAKE = 1

class SocketActivityKind(enum.IntEnum):
    NONE = 0
    STREAM_SOCKET_LISTENER = 1
    DATAGRAM_SOCKET = 2
    STREAM_SOCKET = 3

class SocketActivityTriggerReason(enum.IntEnum):
    NONE = 0
    SOCKET_ACTIVITY = 1
    CONNECTION_ACCEPTED = 2
    KEEP_ALIVE_TIMER_EXPIRED = 3
    SOCKET_CLOSED = 4

class SocketErrorStatus(enum.IntEnum):
    UNKNOWN = 0
    OPERATION_ABORTED = 1
    HTTP_INVALID_SERVER_RESPONSE = 2
    CONNECTION_TIMED_OUT = 3
    ADDRESS_FAMILY_NOT_SUPPORTED = 4
    SOCKET_TYPE_NOT_SUPPORTED = 5
    HOST_NOT_FOUND = 6
    NO_DATA_RECORD_OF_REQUESTED_TYPE = 7
    NON_AUTHORITATIVE_HOST_NOT_FOUND = 8
    CLASS_TYPE_NOT_FOUND = 9
    ADDRESS_ALREADY_IN_USE = 10
    CANNOT_ASSIGN_REQUESTED_ADDRESS = 11
    CONNECTION_REFUSED = 12
    NETWORK_IS_UNREACHABLE = 13
    UNREACHABLE_HOST = 14
    NETWORK_IS_DOWN = 15
    NETWORK_DROPPED_CONNECTION_ON_RESET = 16
    SOFTWARE_CAUSED_CONNECTION_ABORT = 17
    CONNECTION_RESET_BY_PEER = 18
    HOST_IS_DOWN = 19
    NO_ADDRESSES_FOUND = 20
    TOO_MANY_OPEN_FILES = 21
    MESSAGE_TOO_LONG = 22
    CERTIFICATE_EXPIRED = 23
    CERTIFICATE_UNTRUSTED_ROOT = 24
    CERTIFICATE_COMMON_NAME_IS_INCORRECT = 25
    CERTIFICATE_WRONG_USAGE = 26
    CERTIFICATE_REVOKED = 27
    CERTIFICATE_NO_REVOCATION_CHECK = 28
    CERTIFICATE_REVOCATION_SERVER_OFFLINE = 29
    CERTIFICATE_IS_INVALID = 30

class SocketMessageType(enum.IntEnum):
    BINARY = 0
    UTF8 = 1

class SocketProtectionLevel(enum.IntEnum):
    PLAIN_SOCKET = 0
    SSL = 1
    SSL_ALLOW_NULL_ENCRYPTION = 2
    BLUETOOTH_ENCRYPTION_ALLOW_NULL_AUTHENTICATION = 3
    BLUETOOTH_ENCRYPTION_WITH_AUTHENTICATION = 4
    SSL3_ALLOW_WEAK_ENCRYPTION = 5
    TLS10 = 6
    TLS11 = 7
    TLS12 = 8
    UNSPECIFIED = 9

class SocketQualityOfService(enum.IntEnum):
    NORMAL = 0
    LOW_LATENCY = 1

class SocketSslErrorSeverity(enum.IntEnum):
    NONE = 0
    IGNORABLE = 1
    FATAL = 2

Self = typing.TypeVar('Self')

class BandwidthStatistics:
    outbound_bits_per_second: winrt.system.UInt64
    inbound_bits_per_second: winrt.system.UInt64
    outbound_bits_per_second_instability: winrt.system.UInt64
    inbound_bits_per_second_instability: winrt.system.UInt64
    outbound_bandwidth_peaked: winrt.system.Boolean
    inbound_bandwidth_peaked: winrt.system.Boolean
    def __new__(cls: typing.Type[BandwidthStatistics], outbound_bits_per_second: winrt.system.UInt64, inbound_bits_per_second: winrt.system.UInt64, outbound_bits_per_second_instability: winrt.system.UInt64, inbound_bits_per_second_instability: winrt.system.UInt64, outbound_bandwidth_peaked: winrt.system.Boolean, inbound_bandwidth_peaked: winrt.system.Boolean) -> BandwidthStatistics: ...

class RoundTripTimeStatistics:
    variance: winrt.system.UInt32
    max: winrt.system.UInt32
    min: winrt.system.UInt32
    sum: winrt.system.UInt32
    def __new__(cls: typing.Type[RoundTripTimeStatistics], variance: winrt.system.UInt32, max: winrt.system.UInt32, min: winrt.system.UInt32, sum: winrt.system.UInt32) -> RoundTripTimeStatistics: ...

class ControlChannelTrigger(winrt.system.Object):
    server_keep_alive_interval_in_minutes: winrt.system.UInt32
    control_channel_trigger_id: str
    current_keep_alive_interval_in_minutes: winrt.system.UInt32
    keep_alive_trigger: typing.Optional[winrt.windows.applicationmodel.background.IBackgroundTrigger]
    push_notification_trigger: typing.Optional[winrt.windows.applicationmodel.background.IBackgroundTrigger]
    transport_object: typing.Optional[winrt.system.Object]
    is_wake_from_low_power_supported: winrt.system.Boolean
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, *args) -> None: ...
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ControlChannelTrigger: ...
    @typing.overload
    def __new__(cls: typing.Type[ControlChannelTrigger], channel_id: str, server_keep_alive_interval_in_minutes: winrt.system.UInt32) -> ControlChannelTrigger:...
    @typing.overload
    def __new__(cls: typing.Type[ControlChannelTrigger], channel_id: str, server_keep_alive_interval_in_minutes: winrt.system.UInt32, resource_request_type: ControlChannelTriggerResourceType) -> ControlChannelTrigger:...
    def close(self) -> None: ...
    def decrease_network_keep_alive_interval(self) -> None: ...
    def flush_transport(self) -> None: ...
    def using_transport(self, transport: typing.Optional[winrt.system.Object], /) -> None: ...
    def wait_for_push_enabled(self) -> ControlChannelTriggerStatus: ...

class DatagramSocket(winrt.system.Object):
    control: typing.Optional[DatagramSocketControl]
    information: typing.Optional[DatagramSocketInformation]
    output_stream: typing.Optional[winrt.windows.storage.streams.IOutputStream]
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, *args) -> None: ...
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> DatagramSocket: ...
    def __new__(cls: typing.Type[DatagramSocket]) -> DatagramSocket:...
    def bind_endpoint_async(self, local_host_name: typing.Optional[winrt.windows.networking.HostName], local_service_name: str, /) -> winrt.windows.foundation.IAsyncAction: ...
    @typing.overload
    def bind_service_name_async(self, local_service_name: str, /) -> winrt.windows.foundation.IAsyncAction: ...
    @typing.overload
    def bind_service_name_async(self, local_service_name: str, adapter: typing.Optional[winrt.windows.networking.connectivity.NetworkAdapter], /) -> winrt.windows.foundation.IAsyncAction: ...
    def cancel_i_o_async(self) -> winrt.windows.foundation.IAsyncAction: ...
    def close(self) -> None: ...
    @typing.overload
    def connect_async(self, endpoint_pair: typing.Optional[winrt.windows.networking.EndpointPair], /) -> winrt.windows.foundation.IAsyncAction: ...
    @typing.overload
    def connect_async(self, remote_host_name: typing.Optional[winrt.windows.networking.HostName], remote_service_name: str, /) -> winrt.windows.foundation.IAsyncAction: ...
    @typing.overload
    def enable_transfer_ownership(self, task_id: winrt.system.Guid, /) -> None: ...
    @typing.overload
    def enable_transfer_ownership(self, task_id: winrt.system.Guid, connected_standby_action: SocketActivityConnectedStandbyAction, /) -> None: ...
    @typing.overload
    @staticmethod
    def get_endpoint_pairs_async(remote_host_name: typing.Optional[winrt.windows.networking.HostName], remote_service_name: str, /) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.foundation.collections.IVectorView[winrt.windows.networking.EndpointPair]]: ...
    @typing.overload
    @staticmethod
    def get_endpoint_pairs_async(remote_host_name: typing.Optional[winrt.windows.networking.HostName], remote_service_name: str, sort_options: winrt.windows.networking.HostNameSortOptions, /) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.foundation.collections.IVectorView[winrt.windows.networking.EndpointPair]]: ...
    @typing.overload
    def get_output_stream_async(self, endpoint_pair: typing.Optional[winrt.windows.networking.EndpointPair], /) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.storage.streams.IOutputStream]: ...
    @typing.overload
    def get_output_stream_async(self, remote_host_name: typing.Optional[winrt.windows.networking.HostName], remote_service_name: str, /) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.storage.streams.IOutputStream]: ...
    def join_multicast_group(self, host: typing.Optional[winrt.windows.networking.HostName], /) -> None: ...
    @typing.overload
    def transfer_ownership(self, socket_id: str, /) -> None: ...
    @typing.overload
    def transfer_ownership(self, socket_id: str, data: typing.Optional[SocketActivityContext], /) -> None: ...
    @typing.overload
    def transfer_ownership(self, socket_id: str, data: typing.Optional[SocketActivityContext], keep_alive_time: datetime.timedelta, /) -> None: ...
    def add_message_received(self, event_handler: winrt.windows.foundation.TypedEventHandler[DatagramSocket, DatagramSocketMessageReceivedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_message_received(self, event_cookie: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class DatagramSocketControl(winrt.system.Object):
    quality_of_service: SocketQualityOfService
    outbound_unicast_hop_limit: winrt.system.UInt8
    inbound_buffer_size_in_bytes: winrt.system.UInt32
    dont_fragment: winrt.system.Boolean
    multicast_only: winrt.system.Boolean
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> DatagramSocketControl: ...

class DatagramSocketInformation(winrt.system.Object):
    local_address: typing.Optional[winrt.windows.networking.HostName]
    local_port: str
    remote_address: typing.Optional[winrt.windows.networking.HostName]
    remote_port: str
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> DatagramSocketInformation: ...

class DatagramSocketMessageReceivedEventArgs(winrt.system.Object):
    local_address: typing.Optional[winrt.windows.networking.HostName]
    remote_address: typing.Optional[winrt.windows.networking.HostName]
    remote_port: str
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> DatagramSocketMessageReceivedEventArgs: ...
    def get_data_reader(self) -> typing.Optional[winrt.windows.storage.streams.DataReader]: ...
    def get_data_stream(self) -> typing.Optional[winrt.windows.storage.streams.IInputStream]: ...

class MessageWebSocket(winrt.system.Object):
    control: typing.Optional[MessageWebSocketControl]
    information: typing.Optional[MessageWebSocketInformation]
    output_stream: typing.Optional[winrt.windows.storage.streams.IOutputStream]
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, *args) -> None: ...
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> MessageWebSocket: ...
    def __new__(cls: typing.Type[MessageWebSocket]) -> MessageWebSocket:...
    @typing.overload
    def close(self) -> None: ...
    @typing.overload
    def close(self, code: winrt.system.UInt16, reason: str, /) -> None: ...
    def connect_async(self, uri: typing.Optional[winrt.windows.foundation.Uri], /) -> winrt.windows.foundation.IAsyncAction: ...
    def send_final_frame_async(self, data: typing.Optional[winrt.windows.storage.streams.IBuffer], /) -> winrt.windows.foundation.IAsyncOperationWithProgress[winrt.system.UInt32, winrt.system.UInt32]: ...
    def send_nonfinal_frame_async(self, data: typing.Optional[winrt.windows.storage.streams.IBuffer], /) -> winrt.windows.foundation.IAsyncOperationWithProgress[winrt.system.UInt32, winrt.system.UInt32]: ...
    def set_request_header(self, header_name: str, header_value: str, /) -> None: ...
    def add_message_received(self, event_handler: winrt.windows.foundation.TypedEventHandler[MessageWebSocket, MessageWebSocketMessageReceivedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_message_received(self, event_cookie: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_closed(self, event_handler: winrt.windows.foundation.TypedEventHandler[IWebSocket, WebSocketClosedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_closed(self, event_cookie: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_server_custom_validation_requested(self, event_handler: winrt.windows.foundation.TypedEventHandler[MessageWebSocket, WebSocketServerCustomValidationRequestedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_server_custom_validation_requested(self, event_cookie: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class MessageWebSocketControl(winrt.system.Object):
    message_type: SocketMessageType
    max_message_size: winrt.system.UInt32
    receive_mode: MessageWebSocketReceiveMode
    desired_unsolicited_pong_interval: datetime.timedelta
    client_certificate: typing.Optional[winrt.windows.security.cryptography.certificates.Certificate]
    actual_unsolicited_pong_interval: datetime.timedelta
    server_credential: typing.Optional[winrt.windows.security.credentials.PasswordCredential]
    proxy_credential: typing.Optional[winrt.windows.security.credentials.PasswordCredential]
    outbound_buffer_size_in_bytes: winrt.system.UInt32
    supported_protocols: typing.Optional[winrt.windows.foundation.collections.IVector[str]]
    ignorable_server_certificate_errors: typing.Optional[winrt.windows.foundation.collections.IVector[winrt.windows.security.cryptography.certificates.ChainValidationResult]]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> MessageWebSocketControl: ...

class MessageWebSocketInformation(winrt.system.Object):
    bandwidth_statistics: BandwidthStatistics
    local_address: typing.Optional[winrt.windows.networking.HostName]
    protocol: str
    server_certificate: typing.Optional[winrt.windows.security.cryptography.certificates.Certificate]
    server_certificate_error_severity: SocketSslErrorSeverity
    server_certificate_errors: typing.Optional[winrt.windows.foundation.collections.IVectorView[winrt.windows.security.cryptography.certificates.ChainValidationResult]]
    server_intermediate_certificates: typing.Optional[winrt.windows.foundation.collections.IVectorView[winrt.windows.security.cryptography.certificates.Certificate]]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> MessageWebSocketInformation: ...

class MessageWebSocketMessageReceivedEventArgs(winrt.system.Object):
    message_type: SocketMessageType
    is_message_complete: winrt.system.Boolean
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> MessageWebSocketMessageReceivedEventArgs: ...
    def get_data_reader(self) -> typing.Optional[winrt.windows.storage.streams.DataReader]: ...
    def get_data_stream(self) -> typing.Optional[winrt.windows.storage.streams.IInputStream]: ...

class ServerMessageWebSocket(winrt.system.Object):
    control: typing.Optional[ServerMessageWebSocketControl]
    information: typing.Optional[ServerMessageWebSocketInformation]
    output_stream: typing.Optional[winrt.windows.storage.streams.IOutputStream]
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, *args) -> None: ...
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ServerMessageWebSocket: ...
    @typing.overload
    def close(self) -> None: ...
    @typing.overload
    def close(self, code: winrt.system.UInt16, reason: str, /) -> None: ...
    def add_closed(self, value: winrt.windows.foundation.TypedEventHandler[ServerMessageWebSocket, WebSocketClosedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_closed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_message_received(self, value: winrt.windows.foundation.TypedEventHandler[ServerMessageWebSocket, MessageWebSocketMessageReceivedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_message_received(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class ServerMessageWebSocketControl(winrt.system.Object):
    message_type: SocketMessageType
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ServerMessageWebSocketControl: ...

class ServerMessageWebSocketInformation(winrt.system.Object):
    bandwidth_statistics: BandwidthStatistics
    local_address: typing.Optional[winrt.windows.networking.HostName]
    protocol: str
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ServerMessageWebSocketInformation: ...

class ServerStreamWebSocket(winrt.system.Object):
    information: typing.Optional[ServerStreamWebSocketInformation]
    input_stream: typing.Optional[winrt.windows.storage.streams.IInputStream]
    output_stream: typing.Optional[winrt.windows.storage.streams.IOutputStream]
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, *args) -> None: ...
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ServerStreamWebSocket: ...
    @typing.overload
    def close(self) -> None: ...
    @typing.overload
    def close(self, code: winrt.system.UInt16, reason: str, /) -> None: ...
    def add_closed(self, value: winrt.windows.foundation.TypedEventHandler[ServerStreamWebSocket, WebSocketClosedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_closed(self, token: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class ServerStreamWebSocketInformation(winrt.system.Object):
    bandwidth_statistics: BandwidthStatistics
    local_address: typing.Optional[winrt.windows.networking.HostName]
    protocol: str
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ServerStreamWebSocketInformation: ...

class SocketActivityContext(winrt.system.Object):
    data: typing.Optional[winrt.windows.storage.streams.IBuffer]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SocketActivityContext: ...
    def __new__(cls: typing.Type[SocketActivityContext], data: typing.Optional[winrt.windows.storage.streams.IBuffer]) -> SocketActivityContext:...

class SocketActivityInformation(winrt.system.Object):
    context: typing.Optional[SocketActivityContext]
    datagram_socket: typing.Optional[DatagramSocket]
    id: str
    socket_kind: SocketActivityKind
    stream_socket: typing.Optional[StreamSocket]
    stream_socket_listener: typing.Optional[StreamSocketListener]
    task_id: winrt.system.Guid
    all_sockets: typing.ClassVar[typing.Optional[winrt.windows.foundation.collections.IMapView[str, SocketActivityInformation]]]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SocketActivityInformation: ...

class SocketActivityTriggerDetails(winrt.system.Object):
    reason: SocketActivityTriggerReason
    socket_information: typing.Optional[SocketActivityInformation]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SocketActivityTriggerDetails: ...

class SocketError(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SocketError: ...
    @staticmethod
    def get_status(hresult: winrt.system.Int32, /) -> SocketErrorStatus: ...

class StreamSocket(winrt.system.Object):
    control: typing.Optional[StreamSocketControl]
    information: typing.Optional[StreamSocketInformation]
    input_stream: typing.Optional[winrt.windows.storage.streams.IInputStream]
    output_stream: typing.Optional[winrt.windows.storage.streams.IOutputStream]
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, *args) -> None: ...
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> StreamSocket: ...
    def __new__(cls: typing.Type[StreamSocket]) -> StreamSocket:...
    def cancel_i_o_async(self) -> winrt.windows.foundation.IAsyncAction: ...
    def close(self) -> None: ...
    @typing.overload
    def connect_async(self, endpoint_pair: typing.Optional[winrt.windows.networking.EndpointPair], /) -> winrt.windows.foundation.IAsyncAction: ...
    @typing.overload
    def connect_async(self, remote_host_name: typing.Optional[winrt.windows.networking.HostName], remote_service_name: str, /) -> winrt.windows.foundation.IAsyncAction: ...
    @typing.overload
    def connect_async(self, remote_host_name: typing.Optional[winrt.windows.networking.HostName], remote_service_name: str, protection_level: SocketProtectionLevel, /) -> winrt.windows.foundation.IAsyncAction: ...
    @typing.overload
    def connect_async(self, remote_host_name: typing.Optional[winrt.windows.networking.HostName], remote_service_name: str, protection_level: SocketProtectionLevel, adapter: typing.Optional[winrt.windows.networking.connectivity.NetworkAdapter], /) -> winrt.windows.foundation.IAsyncAction: ...
    @typing.overload
    def enable_transfer_ownership(self, task_id: winrt.system.Guid, /) -> None: ...
    @typing.overload
    def enable_transfer_ownership(self, task_id: winrt.system.Guid, connected_standby_action: SocketActivityConnectedStandbyAction, /) -> None: ...
    @typing.overload
    @staticmethod
    def get_endpoint_pairs_async(remote_host_name: typing.Optional[winrt.windows.networking.HostName], remote_service_name: str, /) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.foundation.collections.IVectorView[winrt.windows.networking.EndpointPair]]: ...
    @typing.overload
    @staticmethod
    def get_endpoint_pairs_async(remote_host_name: typing.Optional[winrt.windows.networking.HostName], remote_service_name: str, sort_options: winrt.windows.networking.HostNameSortOptions, /) -> winrt.windows.foundation.IAsyncOperation[winrt.windows.foundation.collections.IVectorView[winrt.windows.networking.EndpointPair]]: ...
    @typing.overload
    def transfer_ownership(self, socket_id: str, /) -> None: ...
    @typing.overload
    def transfer_ownership(self, socket_id: str, data: typing.Optional[SocketActivityContext], /) -> None: ...
    @typing.overload
    def transfer_ownership(self, socket_id: str, data: typing.Optional[SocketActivityContext], keep_alive_time: datetime.timedelta, /) -> None: ...
    def upgrade_to_ssl_async(self, protection_level: SocketProtectionLevel, validation_host_name: typing.Optional[winrt.windows.networking.HostName], /) -> winrt.windows.foundation.IAsyncAction: ...

class StreamSocketControl(winrt.system.Object):
    quality_of_service: SocketQualityOfService
    outbound_unicast_hop_limit: winrt.system.UInt8
    outbound_buffer_size_in_bytes: winrt.system.UInt32
    no_delay: winrt.system.Boolean
    keep_alive: winrt.system.Boolean
    ignorable_server_certificate_errors: typing.Optional[winrt.windows.foundation.collections.IVector[winrt.windows.security.cryptography.certificates.ChainValidationResult]]
    serialize_connection_attempts: winrt.system.Boolean
    client_certificate: typing.Optional[winrt.windows.security.cryptography.certificates.Certificate]
    min_protection_level: SocketProtectionLevel
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> StreamSocketControl: ...

class StreamSocketInformation(winrt.system.Object):
    bandwidth_statistics: BandwidthStatistics
    local_address: typing.Optional[winrt.windows.networking.HostName]
    local_port: str
    protection_level: SocketProtectionLevel
    remote_address: typing.Optional[winrt.windows.networking.HostName]
    remote_host_name: typing.Optional[winrt.windows.networking.HostName]
    remote_port: str
    remote_service_name: str
    round_trip_time_statistics: RoundTripTimeStatistics
    session_key: typing.Optional[winrt.windows.storage.streams.IBuffer]
    server_certificate: typing.Optional[winrt.windows.security.cryptography.certificates.Certificate]
    server_certificate_error_severity: SocketSslErrorSeverity
    server_certificate_errors: typing.Optional[winrt.windows.foundation.collections.IVectorView[winrt.windows.security.cryptography.certificates.ChainValidationResult]]
    server_intermediate_certificates: typing.Optional[winrt.windows.foundation.collections.IVectorView[winrt.windows.security.cryptography.certificates.Certificate]]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> StreamSocketInformation: ...

class StreamSocketListener(winrt.system.Object):
    control: typing.Optional[StreamSocketListenerControl]
    information: typing.Optional[StreamSocketListenerInformation]
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, *args) -> None: ...
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> StreamSocketListener: ...
    def __new__(cls: typing.Type[StreamSocketListener]) -> StreamSocketListener:...
    def bind_endpoint_async(self, local_host_name: typing.Optional[winrt.windows.networking.HostName], local_service_name: str, /) -> winrt.windows.foundation.IAsyncAction: ...
    @typing.overload
    def bind_service_name_async(self, local_service_name: str, /) -> winrt.windows.foundation.IAsyncAction: ...
    @typing.overload
    def bind_service_name_async(self, local_service_name: str, protection_level: SocketProtectionLevel, /) -> winrt.windows.foundation.IAsyncAction: ...
    @typing.overload
    def bind_service_name_async(self, local_service_name: str, protection_level: SocketProtectionLevel, adapter: typing.Optional[winrt.windows.networking.connectivity.NetworkAdapter], /) -> winrt.windows.foundation.IAsyncAction: ...
    def cancel_i_o_async(self) -> winrt.windows.foundation.IAsyncAction: ...
    def close(self) -> None: ...
    @typing.overload
    def enable_transfer_ownership(self, task_id: winrt.system.Guid, /) -> None: ...
    @typing.overload
    def enable_transfer_ownership(self, task_id: winrt.system.Guid, connected_standby_action: SocketActivityConnectedStandbyAction, /) -> None: ...
    @typing.overload
    def transfer_ownership(self, socket_id: str, /) -> None: ...
    @typing.overload
    def transfer_ownership(self, socket_id: str, data: typing.Optional[SocketActivityContext], /) -> None: ...
    def add_connection_received(self, event_handler: winrt.windows.foundation.TypedEventHandler[StreamSocketListener, StreamSocketListenerConnectionReceivedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_connection_received(self, event_cookie: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class StreamSocketListenerConnectionReceivedEventArgs(winrt.system.Object):
    socket: typing.Optional[StreamSocket]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> StreamSocketListenerConnectionReceivedEventArgs: ...

class StreamSocketListenerControl(winrt.system.Object):
    quality_of_service: SocketQualityOfService
    outbound_unicast_hop_limit: winrt.system.UInt8
    outbound_buffer_size_in_bytes: winrt.system.UInt32
    no_delay: winrt.system.Boolean
    keep_alive: winrt.system.Boolean
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> StreamSocketListenerControl: ...

class StreamSocketListenerInformation(winrt.system.Object):
    local_port: str
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> StreamSocketListenerInformation: ...

class StreamWebSocket(winrt.system.Object):
    control: typing.Optional[StreamWebSocketControl]
    information: typing.Optional[StreamWebSocketInformation]
    input_stream: typing.Optional[winrt.windows.storage.streams.IInputStream]
    output_stream: typing.Optional[winrt.windows.storage.streams.IOutputStream]
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, *args) -> None: ...
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> StreamWebSocket: ...
    def __new__(cls: typing.Type[StreamWebSocket]) -> StreamWebSocket:...
    @typing.overload
    def close(self) -> None: ...
    @typing.overload
    def close(self, code: winrt.system.UInt16, reason: str, /) -> None: ...
    def connect_async(self, uri: typing.Optional[winrt.windows.foundation.Uri], /) -> winrt.windows.foundation.IAsyncAction: ...
    def set_request_header(self, header_name: str, header_value: str, /) -> None: ...
    def add_closed(self, event_handler: winrt.windows.foundation.TypedEventHandler[IWebSocket, WebSocketClosedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_closed(self, event_cookie: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...
    def add_server_custom_validation_requested(self, event_handler: winrt.windows.foundation.TypedEventHandler[StreamWebSocket, WebSocketServerCustomValidationRequestedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_server_custom_validation_requested(self, event_cookie: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class StreamWebSocketControl(winrt.system.Object):
    no_delay: winrt.system.Boolean
    desired_unsolicited_pong_interval: datetime.timedelta
    client_certificate: typing.Optional[winrt.windows.security.cryptography.certificates.Certificate]
    actual_unsolicited_pong_interval: datetime.timedelta
    server_credential: typing.Optional[winrt.windows.security.credentials.PasswordCredential]
    proxy_credential: typing.Optional[winrt.windows.security.credentials.PasswordCredential]
    outbound_buffer_size_in_bytes: winrt.system.UInt32
    supported_protocols: typing.Optional[winrt.windows.foundation.collections.IVector[str]]
    ignorable_server_certificate_errors: typing.Optional[winrt.windows.foundation.collections.IVector[winrt.windows.security.cryptography.certificates.ChainValidationResult]]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> StreamWebSocketControl: ...

class StreamWebSocketInformation(winrt.system.Object):
    bandwidth_statistics: BandwidthStatistics
    local_address: typing.Optional[winrt.windows.networking.HostName]
    protocol: str
    server_certificate: typing.Optional[winrt.windows.security.cryptography.certificates.Certificate]
    server_certificate_error_severity: SocketSslErrorSeverity
    server_certificate_errors: typing.Optional[winrt.windows.foundation.collections.IVectorView[winrt.windows.security.cryptography.certificates.ChainValidationResult]]
    server_intermediate_certificates: typing.Optional[winrt.windows.foundation.collections.IVectorView[winrt.windows.security.cryptography.certificates.Certificate]]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> StreamWebSocketInformation: ...

class WebSocketClosedEventArgs(winrt.system.Object):
    code: winrt.system.UInt16
    reason: str
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> WebSocketClosedEventArgs: ...

class WebSocketError(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> WebSocketError: ...
    @staticmethod
    def get_status(hresult: winrt.system.Int32, /) -> winrt.windows.web.WebErrorStatus: ...

class WebSocketKeepAlive(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> WebSocketKeepAlive: ...
    def __new__(cls: typing.Type[WebSocketKeepAlive]) -> WebSocketKeepAlive:...
    def run(self, task_instance: typing.Optional[winrt.windows.applicationmodel.background.IBackgroundTaskInstance], /) -> None: ...

class WebSocketServerCustomValidationRequestedEventArgs(winrt.system.Object):
    server_certificate: typing.Optional[winrt.windows.security.cryptography.certificates.Certificate]
    server_certificate_error_severity: SocketSslErrorSeverity
    server_certificate_errors: typing.Optional[winrt.windows.foundation.collections.IVectorView[winrt.windows.security.cryptography.certificates.ChainValidationResult]]
    server_intermediate_certificates: typing.Optional[winrt.windows.foundation.collections.IVectorView[winrt.windows.security.cryptography.certificates.Certificate]]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> WebSocketServerCustomValidationRequestedEventArgs: ...
    def get_deferral(self) -> typing.Optional[winrt.windows.foundation.Deferral]: ...
    def reject(self) -> None: ...

class IControlChannelTriggerEventDetails(winrt.system.Object):
    control_channel_trigger: typing.Optional[ControlChannelTrigger]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> IControlChannelTriggerEventDetails: ...

class IControlChannelTriggerResetEventDetails(winrt.system.Object):
    hardware_slot_reset: winrt.system.Boolean
    reset_reason: ControlChannelTriggerResetReason
    software_slot_reset: winrt.system.Boolean
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> IControlChannelTriggerResetEventDetails: ...

class IWebSocket(winrt.system.Object):
    output_stream: typing.Optional[winrt.windows.storage.streams.IOutputStream]
    def __enter__(self: Self) -> Self: ...
    def __exit__(self, *args) -> None: ...
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> IWebSocket: ...
    @typing.overload
    def close(self, code: winrt.system.UInt16, reason: str, /) -> None: ...
    @typing.overload
    def close(self) -> None: ...
    def connect_async(self, uri: typing.Optional[winrt.windows.foundation.Uri], /) -> winrt.windows.foundation.IAsyncAction: ...
    def set_request_header(self, header_name: str, header_value: str, /) -> None: ...
    def add_closed(self, event_handler: winrt.windows.foundation.TypedEventHandler[IWebSocket, WebSocketClosedEventArgs], /) -> winrt.windows.foundation.EventRegistrationToken: ...
    def remove_closed(self, event_cookie: winrt.windows.foundation.EventRegistrationToken, /) -> None: ...

class IWebSocketControl(winrt.system.Object):
    outbound_buffer_size_in_bytes: winrt.system.UInt32
    proxy_credential: typing.Optional[winrt.windows.security.credentials.PasswordCredential]
    server_credential: typing.Optional[winrt.windows.security.credentials.PasswordCredential]
    supported_protocols: typing.Optional[winrt.windows.foundation.collections.IVector[str]]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> IWebSocketControl: ...

class IWebSocketControl2(winrt.system.Object):
    ignorable_server_certificate_errors: typing.Optional[winrt.windows.foundation.collections.IVector[winrt.windows.security.cryptography.certificates.ChainValidationResult]]
    outbound_buffer_size_in_bytes: winrt.system.UInt32
    proxy_credential: typing.Optional[winrt.windows.security.credentials.PasswordCredential]
    server_credential: typing.Optional[winrt.windows.security.credentials.PasswordCredential]
    supported_protocols: typing.Optional[winrt.windows.foundation.collections.IVector[str]]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> IWebSocketControl2: ...

class IWebSocketInformation(winrt.system.Object):
    bandwidth_statistics: BandwidthStatistics
    local_address: typing.Optional[winrt.windows.networking.HostName]
    protocol: str
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> IWebSocketInformation: ...

class IWebSocketInformation2(winrt.system.Object):
    server_certificate: typing.Optional[winrt.windows.security.cryptography.certificates.Certificate]
    server_certificate_error_severity: SocketSslErrorSeverity
    server_certificate_errors: typing.Optional[winrt.windows.foundation.collections.IVectorView[winrt.windows.security.cryptography.certificates.ChainValidationResult]]
    server_intermediate_certificates: typing.Optional[winrt.windows.foundation.collections.IVectorView[winrt.windows.security.cryptography.certificates.Certificate]]
    bandwidth_statistics: BandwidthStatistics
    local_address: typing.Optional[winrt.windows.networking.HostName]
    protocol: str
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> IWebSocketInformation2: ...
