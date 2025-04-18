# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum

import winrt.system
from winrt._winrt_windows_networking_sockets import (
    BandwidthStatistics,
    RoundTripTimeStatistics,
    ControlChannelTrigger,
    DatagramSocket,
    DatagramSocketControl,
    DatagramSocketInformation,
    DatagramSocketMessageReceivedEventArgs,
    MessageWebSocket,
    MessageWebSocketControl,
    MessageWebSocketInformation,
    MessageWebSocketMessageReceivedEventArgs,
    ServerMessageWebSocket,
    ServerMessageWebSocketControl,
    ServerMessageWebSocketInformation,
    ServerStreamWebSocket,
    ServerStreamWebSocketInformation,
    SocketActivityContext,
    SocketActivityInformation,
    SocketActivityTriggerDetails,
    SocketError,
    StreamSocket,
    StreamSocketControl,
    StreamSocketInformation,
    StreamSocketListener,
    StreamSocketListenerConnectionReceivedEventArgs,
    StreamSocketListenerControl,
    StreamSocketListenerInformation,
    StreamWebSocket,
    StreamWebSocketControl,
    StreamWebSocketInformation,
    WebSocketClosedEventArgs,
    WebSocketError,
    WebSocketKeepAlive,
    WebSocketServerCustomValidationRequestedEventArgs,
    _IControlChannelTriggerEventDetails,
    IControlChannelTriggerEventDetails,
    _IControlChannelTriggerResetEventDetails,
    IControlChannelTriggerResetEventDetails,
    _IWebSocket,
    IWebSocket,
    _IWebSocketControl,
    IWebSocketControl,
    _IWebSocketControl2,
    IWebSocketControl2,
    _IWebSocketInformation,
    IWebSocketInformation,
    _IWebSocketInformation2,
    IWebSocketInformation2,
)

__all__ = [
    "ControlChannelTriggerResetReason",
    "ControlChannelTriggerResourceType",
    "ControlChannelTriggerStatus",
    "MessageWebSocketReceiveMode",
    "SocketActivityConnectedStandbyAction",
    "SocketActivityKind",
    "SocketActivityTriggerReason",
    "SocketErrorStatus",
    "SocketMessageType",
    "SocketProtectionLevel",
    "SocketQualityOfService",
    "SocketSslErrorSeverity",
    "BandwidthStatistics",
    "RoundTripTimeStatistics",
    "ControlChannelTrigger",
    "DatagramSocket",
    "DatagramSocketControl",
    "DatagramSocketInformation",
    "DatagramSocketMessageReceivedEventArgs",
    "MessageWebSocket",
    "MessageWebSocketControl",
    "MessageWebSocketInformation",
    "MessageWebSocketMessageReceivedEventArgs",
    "ServerMessageWebSocket",
    "ServerMessageWebSocketControl",
    "ServerMessageWebSocketInformation",
    "ServerStreamWebSocket",
    "ServerStreamWebSocketInformation",
    "SocketActivityContext",
    "SocketActivityInformation",
    "SocketActivityTriggerDetails",
    "SocketError",
    "StreamSocket",
    "StreamSocketControl",
    "StreamSocketInformation",
    "StreamSocketListener",
    "StreamSocketListenerConnectionReceivedEventArgs",
    "StreamSocketListenerControl",
    "StreamSocketListenerInformation",
    "StreamWebSocket",
    "StreamWebSocketControl",
    "StreamWebSocketInformation",
    "WebSocketClosedEventArgs",
    "WebSocketError",
    "WebSocketKeepAlive",
    "WebSocketServerCustomValidationRequestedEventArgs",
    "IControlChannelTriggerEventDetails",
    "IControlChannelTriggerResetEventDetails",
    "IWebSocket",
    "IWebSocketControl",
    "IWebSocketControl2",
    "IWebSocketInformation",
    "IWebSocketInformation2",
]

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
    TLS13 = 10

class SocketQualityOfService(enum.IntEnum):
    NORMAL = 0
    LOW_LATENCY = 1

class SocketSslErrorSeverity(enum.IntEnum):
    NONE = 0
    IGNORABLE = 1
    FATAL = 2

