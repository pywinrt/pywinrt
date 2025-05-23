# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum
import typing
import uuid as _uuid

import winrt.system
from winrt._winrt_windows_applicationmodel_payments import (
    PaymentAddress,
    PaymentCanMakePaymentResult,
    PaymentCurrencyAmount,
    PaymentDetails,
    PaymentDetailsModifier,
    PaymentItem,
    PaymentMediator,
    PaymentMerchantInfo,
    PaymentMethodData,
    PaymentOptions,
    PaymentRequest,
    PaymentRequestChangedArgs,
    PaymentRequestChangedResult,
    PaymentRequestSubmitResult,
    PaymentResponse,
    PaymentShippingOption,
    PaymentToken,
)

__all__ = [
    "PaymentCanMakePaymentResultStatus",
    "PaymentOptionPresence",
    "PaymentRequestChangeKind",
    "PaymentRequestCompletionStatus",
    "PaymentRequestStatus",
    "PaymentShippingType",
    "PaymentAddress",
    "PaymentCanMakePaymentResult",
    "PaymentCurrencyAmount",
    "PaymentDetails",
    "PaymentDetailsModifier",
    "PaymentItem",
    "PaymentMediator",
    "PaymentMerchantInfo",
    "PaymentMethodData",
    "PaymentOptions",
    "PaymentRequest",
    "PaymentRequestChangedArgs",
    "PaymentRequestChangedResult",
    "PaymentRequestSubmitResult",
    "PaymentResponse",
    "PaymentShippingOption",
    "PaymentToken",
    "PaymentRequestChangedHandler",
]

class PaymentCanMakePaymentResultStatus(enum.IntEnum):
    UNKNOWN = 0
    YES = 1
    NO = 2
    NOT_ALLOWED = 3
    USER_NOT_SIGNED_IN = 4
    SPECIFIED_PAYMENT_METHOD_IDS_NOT_SUPPORTED = 5
    NO_QUALIFYING_CARD_ON_FILE = 6

class PaymentOptionPresence(enum.IntEnum):
    NONE = 0
    OPTIONAL = 1
    REQUIRED = 2

class PaymentRequestChangeKind(enum.IntEnum):
    SHIPPING_OPTION = 0
    SHIPPING_ADDRESS = 1

class PaymentRequestCompletionStatus(enum.IntEnum):
    SUCCEEDED = 0
    FAILED = 1
    UNKNOWN = 2

class PaymentRequestStatus(enum.IntEnum):
    SUCCEEDED = 0
    FAILED = 1
    CANCELED = 2

class PaymentShippingType(enum.IntEnum):
    SHIPPING = 0
    DELIVERY = 1
    PICKUP = 2

PaymentRequestChangedHandler = typing.Callable[[PaymentRequest, PaymentRequestChangedArgs], None]
