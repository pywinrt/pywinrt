# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum
import typing
import uuid as _uuid

import winrt.system
from winrt._winrt_windows_devices_sms import (
    SmsEncodedLength,
    DeleteSmsMessageOperation,
    DeleteSmsMessagesOperation,
    GetSmsDeviceOperation,
    GetSmsMessageOperation,
    GetSmsMessagesOperation,
    SendSmsMessageOperation,
    SmsAppMessage,
    SmsBinaryMessage,
    SmsBroadcastMessage,
    SmsDevice,
    SmsDevice2,
    SmsDeviceMessageStore,
    SmsFilterRule,
    SmsFilterRules,
    SmsMessageReceivedEventArgs,
    SmsMessageReceivedTriggerDetails,
    SmsMessageRegistration,
    SmsReceivedEventDetails,
    SmsSendMessageResult,
    SmsStatusMessage,
    SmsTextMessage,
    SmsTextMessage2,
    SmsVoicemailMessage,
    SmsWapMessage,
    _ISmsBinaryMessage,
    ISmsBinaryMessage,
    _ISmsDevice,
    ISmsDevice,
    _ISmsMessage,
    ISmsMessage,
    _ISmsMessageBase,
    ISmsMessageBase,
    _ISmsTextMessage,
    ISmsTextMessage,
)

__all__ = [
    "CellularClass",
    "SmsBroadcastType",
    "SmsDataFormat",
    "SmsDeviceStatus",
    "SmsEncoding",
    "SmsFilterActionType",
    "SmsGeographicalScope",
    "SmsMessageClass",
    "SmsMessageFilter",
    "SmsMessageType",
    "SmsModemErrorCode",
    "SmsEncodedLength",
    "DeleteSmsMessageOperation",
    "DeleteSmsMessagesOperation",
    "GetSmsDeviceOperation",
    "GetSmsMessageOperation",
    "GetSmsMessagesOperation",
    "SendSmsMessageOperation",
    "SmsAppMessage",
    "SmsBinaryMessage",
    "SmsBroadcastMessage",
    "SmsDevice",
    "SmsDevice2",
    "SmsDeviceMessageStore",
    "SmsFilterRule",
    "SmsFilterRules",
    "SmsMessageReceivedEventArgs",
    "SmsMessageReceivedTriggerDetails",
    "SmsMessageRegistration",
    "SmsReceivedEventDetails",
    "SmsSendMessageResult",
    "SmsStatusMessage",
    "SmsTextMessage",
    "SmsTextMessage2",
    "SmsVoicemailMessage",
    "SmsWapMessage",
    "ISmsBinaryMessage",
    "ISmsDevice",
    "ISmsMessage",
    "ISmsMessageBase",
    "ISmsTextMessage",
    "SmsDeviceStatusChangedEventHandler",
    "SmsMessageReceivedEventHandler",
]

class CellularClass(enum.IntEnum):
    NONE = 0
    GSM = 1
    CDMA = 2

class SmsBroadcastType(enum.IntEnum):
    OTHER = 0
    CMAS_PRESIDENTIAL = 1
    CMAS_EXTREME = 2
    CMAS_SEVERE = 3
    CMAS_AMBER = 4
    CMAS_TEST = 5
    EU_ALERT1 = 6
    EU_ALERT2 = 7
    EU_ALERT3 = 8
    EU_ALERT_AMBER = 9
    EU_ALERT_INFO = 10
    ETWS_EARTHQUAKE = 11
    ETWS_TSUNAMI = 12
    ETWS_TSUNAMI_AND_EARTHQUAKE = 13
    LAT_ALERT_LOCAL = 14

class SmsDataFormat(enum.IntEnum):
    UNKNOWN = 0
    CDMA_SUBMIT = 1
    GSM_SUBMIT = 2
    CDMA_DELIVER = 3
    GSM_DELIVER = 4

class SmsDeviceStatus(enum.IntEnum):
    OFF = 0
    READY = 1
    SIM_NOT_INSERTED = 2
    BAD_SIM = 3
    DEVICE_FAILURE = 4
    SUBSCRIPTION_NOT_ACTIVATED = 5
    DEVICE_LOCKED = 6
    DEVICE_BLOCKED = 7

class SmsEncoding(enum.IntEnum):
    UNKNOWN = 0
    OPTIMAL = 1
    SEVEN_BIT_ASCII = 2
    UNICODE = 3
    GSM_SEVEN_BIT = 4
    EIGHT_BIT = 5
    LATIN = 6
    KOREAN = 7
    IA5 = 8
    SHIFT_JIS = 9
    LATIN_HEBREW = 10

class SmsFilterActionType(enum.IntEnum):
    ACCEPT_IMMEDIATELY = 0
    DROP = 1
    PEEK = 2
    ACCEPT = 3

class SmsGeographicalScope(enum.IntEnum):
    NONE = 0
    CELL_WITH_IMMEDIATE_DISPLAY = 1
    LOCATION_AREA = 2
    PLMN = 3
    CELL = 4

class SmsMessageClass(enum.IntEnum):
    NONE = 0
    CLASS0 = 1
    CLASS1 = 2
    CLASS2 = 3
    CLASS3 = 4

class SmsMessageFilter(enum.IntEnum):
    ALL = 0
    UNREAD = 1
    READ = 2
    SENT = 3
    DRAFT = 4

class SmsMessageType(enum.IntEnum):
    BINARY = 0
    TEXT = 1
    WAP = 2
    APP = 3
    BROADCAST = 4
    VOICEMAIL = 5
    STATUS = 6

class SmsModemErrorCode(enum.IntEnum):
    OTHER = 0
    MESSAGING_NETWORK_ERROR = 1
    SMS_OPERATION_NOT_SUPPORTED_BY_DEVICE = 2
    SMS_SERVICE_NOT_SUPPORTED_BY_NETWORK = 3
    DEVICE_FAILURE = 4
    MESSAGE_NOT_ENCODED_PROPERLY = 5
    MESSAGE_TOO_LARGE = 6
    DEVICE_NOT_READY = 7
    NETWORK_NOT_READY = 8
    INVALID_SMSC_ADDRESS = 9
    NETWORK_FAILURE = 10
    FIXED_DIALING_NUMBER_RESTRICTED = 11

SmsDeviceStatusChangedEventHandler = typing.Callable[[SmsDevice], None]
SmsMessageReceivedEventHandler = typing.Callable[[SmsDevice, SmsMessageReceivedEventArgs], None]
