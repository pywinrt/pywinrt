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

from winrt.windows.applicationmodel.payments import PaymentCanMakePaymentResultStatus, PaymentOptionPresence, PaymentRequestChangeKind, PaymentRequestCompletionStatus, PaymentRequestStatus, PaymentShippingType
from winrt.windows.applicationmodel.payments import PaymentRequestChangedHandler

Self = typing.TypeVar('Self')

@typing.final
class PaymentAddress(winrt.system.Object):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # System.String Windows.ApplicationModel.Payments.PaymentAddress::get_SortingCode()
    @_property
    def sorting_code(self) -> str: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentAddress::put_SortingCode(System.String)
    @sorting_code.setter
    def sorting_code(self, value: str) -> None: ...
    # System.String Windows.ApplicationModel.Payments.PaymentAddress::get_Region()
    @_property
    def region(self) -> str: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentAddress::put_Region(System.String)
    @region.setter
    def region(self, value: str) -> None: ...
    # System.String Windows.ApplicationModel.Payments.PaymentAddress::get_Recipient()
    @_property
    def recipient(self) -> str: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentAddress::put_Recipient(System.String)
    @recipient.setter
    def recipient(self, value: str) -> None: ...
    # System.String Windows.ApplicationModel.Payments.PaymentAddress::get_PostalCode()
    @_property
    def postal_code(self) -> str: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentAddress::put_PostalCode(System.String)
    @postal_code.setter
    def postal_code(self, value: str) -> None: ...
    # System.String Windows.ApplicationModel.Payments.PaymentAddress::get_PhoneNumber()
    @_property
    def phone_number(self) -> str: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentAddress::put_PhoneNumber(System.String)
    @phone_number.setter
    def phone_number(self, value: str) -> None: ...
    # System.String Windows.ApplicationModel.Payments.PaymentAddress::get_Organization()
    @_property
    def organization(self) -> str: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentAddress::put_Organization(System.String)
    @organization.setter
    def organization(self, value: str) -> None: ...
    # System.String Windows.ApplicationModel.Payments.PaymentAddress::get_LanguageCode()
    @_property
    def language_code(self) -> str: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentAddress::put_LanguageCode(System.String)
    @language_code.setter
    def language_code(self, value: str) -> None: ...
    # System.String Windows.ApplicationModel.Payments.PaymentAddress::get_DependentLocality()
    @_property
    def dependent_locality(self) -> str: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentAddress::put_DependentLocality(System.String)
    @dependent_locality.setter
    def dependent_locality(self, value: str) -> None: ...
    # System.String Windows.ApplicationModel.Payments.PaymentAddress::get_Country()
    @_property
    def country(self) -> str: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentAddress::put_Country(System.String)
    @country.setter
    def country(self, value: str) -> None: ...
    # System.String Windows.ApplicationModel.Payments.PaymentAddress::get_City()
    @_property
    def city(self) -> str: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentAddress::put_City(System.String)
    @city.setter
    def city(self, value: str) -> None: ...
    # Windows.Foundation.Collections.IVectorView`1<System.String> Windows.ApplicationModel.Payments.PaymentAddress::get_AddressLines()
    @_property
    def address_lines(self) -> typing.Sequence[str]: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentAddress::put_AddressLines(Windows.Foundation.Collections.IVectorView`1<System.String>)
    @address_lines.setter
    def address_lines(self, value: typing.Sequence[str]) -> None: ...
    # Windows.Foundation.Collections.ValueSet Windows.ApplicationModel.Payments.PaymentAddress::get_Properties()
    @_property
    def properties(self) -> windows_foundation_collections.ValueSet: ...

@typing.final
class PaymentCanMakePaymentResult(winrt.system.Object):
    def __new__(cls: typing.Type[Self], value: PaymentCanMakePaymentResultStatus) -> Self: ...
    # Windows.ApplicationModel.Payments.PaymentCanMakePaymentResultStatus Windows.ApplicationModel.Payments.PaymentCanMakePaymentResult::get_Status()
    @_property
    def status(self) -> PaymentCanMakePaymentResultStatus: ...

@typing.final
class PaymentCurrencyAmount(winrt.system.Object):
    @typing.overload
    def __new__(cls: typing.Type[Self], value: str, currency: str) -> Self: ...
    @typing.overload
    def __new__(cls: typing.Type[Self], value: str, currency: str, currency_system: str) -> Self: ...
    # System.String Windows.ApplicationModel.Payments.PaymentCurrencyAmount::get_Value()
    @_property
    def value(self) -> str: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentCurrencyAmount::put_Value(System.String)
    @value.setter
    def value(self, value: str) -> None: ...
    # System.String Windows.ApplicationModel.Payments.PaymentCurrencyAmount::get_CurrencySystem()
    @_property
    def currency_system(self) -> str: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentCurrencyAmount::put_CurrencySystem(System.String)
    @currency_system.setter
    def currency_system(self, value: str) -> None: ...
    # System.String Windows.ApplicationModel.Payments.PaymentCurrencyAmount::get_Currency()
    @_property
    def currency(self) -> str: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentCurrencyAmount::put_Currency(System.String)
    @currency.setter
    def currency(self, value: str) -> None: ...

@typing.final
class PaymentDetails(winrt.system.Object):
    @typing.overload
    def __new__(cls: typing.Type[Self], total: PaymentItem) -> Self: ...
    @typing.overload
    def __new__(cls: typing.Type[Self], total: PaymentItem, display_items: typing.Iterable[PaymentItem]) -> Self: ...
    @typing.overload
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # Windows.ApplicationModel.Payments.PaymentItem Windows.ApplicationModel.Payments.PaymentDetails::get_Total()
    @_property
    def total(self) -> PaymentItem: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentDetails::put_Total(Windows.ApplicationModel.Payments.PaymentItem)
    @total.setter
    def total(self, value: PaymentItem) -> None: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.Payments.PaymentShippingOption> Windows.ApplicationModel.Payments.PaymentDetails::get_ShippingOptions()
    @_property
    def shipping_options(self) -> typing.Sequence[PaymentShippingOption]: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentDetails::put_ShippingOptions(Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.Payments.PaymentShippingOption>)
    @shipping_options.setter
    def shipping_options(self, value: typing.Sequence[PaymentShippingOption]) -> None: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.Payments.PaymentDetailsModifier> Windows.ApplicationModel.Payments.PaymentDetails::get_Modifiers()
    @_property
    def modifiers(self) -> typing.Sequence[PaymentDetailsModifier]: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentDetails::put_Modifiers(Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.Payments.PaymentDetailsModifier>)
    @modifiers.setter
    def modifiers(self, value: typing.Sequence[PaymentDetailsModifier]) -> None: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.Payments.PaymentItem> Windows.ApplicationModel.Payments.PaymentDetails::get_DisplayItems()
    @_property
    def display_items(self) -> typing.Sequence[PaymentItem]: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentDetails::put_DisplayItems(Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.Payments.PaymentItem>)
    @display_items.setter
    def display_items(self, value: typing.Sequence[PaymentItem]) -> None: ...

@typing.final
class PaymentDetailsModifier(winrt.system.Object):
    @typing.overload
    def __new__(cls: typing.Type[Self], supported_method_ids: typing.Iterable[str], total: PaymentItem) -> Self: ...
    @typing.overload
    def __new__(cls: typing.Type[Self], supported_method_ids: typing.Iterable[str], total: PaymentItem, additional_display_items: typing.Iterable[PaymentItem]) -> Self: ...
    @typing.overload
    def __new__(cls: typing.Type[Self], supported_method_ids: typing.Iterable[str], total: PaymentItem, additional_display_items: typing.Iterable[PaymentItem], json_data: str) -> Self: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.Payments.PaymentItem> Windows.ApplicationModel.Payments.PaymentDetailsModifier::get_AdditionalDisplayItems()
    @_property
    def additional_display_items(self) -> typing.Sequence[PaymentItem]: ...
    # System.String Windows.ApplicationModel.Payments.PaymentDetailsModifier::get_JsonData()
    @_property
    def json_data(self) -> str: ...
    # Windows.Foundation.Collections.IVectorView`1<System.String> Windows.ApplicationModel.Payments.PaymentDetailsModifier::get_SupportedMethodIds()
    @_property
    def supported_method_ids(self) -> typing.Sequence[str]: ...
    # Windows.ApplicationModel.Payments.PaymentItem Windows.ApplicationModel.Payments.PaymentDetailsModifier::get_Total()
    @_property
    def total(self) -> PaymentItem: ...

@typing.final
class PaymentItem(winrt.system.Object):
    def __new__(cls: typing.Type[Self], label: str, amount: PaymentCurrencyAmount) -> Self: ...
    # System.Boolean Windows.ApplicationModel.Payments.PaymentItem::get_Pending()
    @_property
    def pending(self) -> bool: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentItem::put_Pending(System.Boolean)
    @pending.setter
    def pending(self, value: bool) -> None: ...
    # System.String Windows.ApplicationModel.Payments.PaymentItem::get_Label()
    @_property
    def label(self) -> str: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentItem::put_Label(System.String)
    @label.setter
    def label(self, value: str) -> None: ...
    # Windows.ApplicationModel.Payments.PaymentCurrencyAmount Windows.ApplicationModel.Payments.PaymentItem::get_Amount()
    @_property
    def amount(self) -> PaymentCurrencyAmount: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentItem::put_Amount(Windows.ApplicationModel.Payments.PaymentCurrencyAmount)
    @amount.setter
    def amount(self, value: PaymentCurrencyAmount) -> None: ...

@typing.final
class PaymentMediator(winrt.system.Object):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.Payments.PaymentCanMakePaymentResult> Windows.ApplicationModel.Payments.PaymentMediator::CanMakePaymentAsync(Windows.ApplicationModel.Payments.PaymentRequest)
    def can_make_payment_async(self, payment_request: PaymentRequest, /) -> windows_foundation.IAsyncOperation[PaymentCanMakePaymentResult]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.Foundation.Collections.IVectorView`1<System.String>> Windows.ApplicationModel.Payments.PaymentMediator::GetSupportedMethodIdsAsync()
    def get_supported_method_ids_async(self) -> windows_foundation.IAsyncOperation[typing.Sequence[str]]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.Payments.PaymentRequestSubmitResult> Windows.ApplicationModel.Payments.PaymentMediator::SubmitPaymentRequestAsync(Windows.ApplicationModel.Payments.PaymentRequest)
    def submit_payment_request_async(self, payment_request: PaymentRequest, /) -> windows_foundation.IAsyncOperation[PaymentRequestSubmitResult]: ...
    # Windows.Foundation.IAsyncOperation`1<Windows.ApplicationModel.Payments.PaymentRequestSubmitResult> Windows.ApplicationModel.Payments.PaymentMediator::SubmitPaymentRequestAsync(Windows.ApplicationModel.Payments.PaymentRequest,Windows.ApplicationModel.Payments.PaymentRequestChangedHandler)
    def submit_payment_request_with_change_handler_async(self, payment_request: PaymentRequest, change_handler: PaymentRequestChangedHandler, /) -> windows_foundation.IAsyncOperation[PaymentRequestSubmitResult]: ...

@typing.final
class PaymentMerchantInfo(winrt.system.Object):
    @typing.overload
    def __new__(cls: typing.Type[Self], uri: windows_foundation.Uri) -> Self: ...
    @typing.overload
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # System.String Windows.ApplicationModel.Payments.PaymentMerchantInfo::get_PackageFullName()
    @_property
    def package_full_name(self) -> str: ...
    # Windows.Foundation.Uri Windows.ApplicationModel.Payments.PaymentMerchantInfo::get_Uri()
    @_property
    def uri(self) -> windows_foundation.Uri: ...

@typing.final
class PaymentMethodData(winrt.system.Object):
    @typing.overload
    def __new__(cls: typing.Type[Self], supported_method_ids: typing.Iterable[str]) -> Self: ...
    @typing.overload
    def __new__(cls: typing.Type[Self], supported_method_ids: typing.Iterable[str], json_data: str) -> Self: ...
    # System.String Windows.ApplicationModel.Payments.PaymentMethodData::get_JsonData()
    @_property
    def json_data(self) -> str: ...
    # Windows.Foundation.Collections.IVectorView`1<System.String> Windows.ApplicationModel.Payments.PaymentMethodData::get_SupportedMethodIds()
    @_property
    def supported_method_ids(self) -> typing.Sequence[str]: ...

@typing.final
class PaymentOptions(winrt.system.Object):
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # Windows.ApplicationModel.Payments.PaymentShippingType Windows.ApplicationModel.Payments.PaymentOptions::get_ShippingType()
    @_property
    def shipping_type(self) -> PaymentShippingType: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentOptions::put_ShippingType(Windows.ApplicationModel.Payments.PaymentShippingType)
    @shipping_type.setter
    def shipping_type(self, value: PaymentShippingType) -> None: ...
    # System.Boolean Windows.ApplicationModel.Payments.PaymentOptions::get_RequestShipping()
    @_property
    def request_shipping(self) -> bool: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentOptions::put_RequestShipping(System.Boolean)
    @request_shipping.setter
    def request_shipping(self, value: bool) -> None: ...
    # Windows.ApplicationModel.Payments.PaymentOptionPresence Windows.ApplicationModel.Payments.PaymentOptions::get_RequestPayerPhoneNumber()
    @_property
    def request_payer_phone_number(self) -> PaymentOptionPresence: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentOptions::put_RequestPayerPhoneNumber(Windows.ApplicationModel.Payments.PaymentOptionPresence)
    @request_payer_phone_number.setter
    def request_payer_phone_number(self, value: PaymentOptionPresence) -> None: ...
    # Windows.ApplicationModel.Payments.PaymentOptionPresence Windows.ApplicationModel.Payments.PaymentOptions::get_RequestPayerName()
    @_property
    def request_payer_name(self) -> PaymentOptionPresence: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentOptions::put_RequestPayerName(Windows.ApplicationModel.Payments.PaymentOptionPresence)
    @request_payer_name.setter
    def request_payer_name(self, value: PaymentOptionPresence) -> None: ...
    # Windows.ApplicationModel.Payments.PaymentOptionPresence Windows.ApplicationModel.Payments.PaymentOptions::get_RequestPayerEmail()
    @_property
    def request_payer_email(self) -> PaymentOptionPresence: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentOptions::put_RequestPayerEmail(Windows.ApplicationModel.Payments.PaymentOptionPresence)
    @request_payer_email.setter
    def request_payer_email(self, value: PaymentOptionPresence) -> None: ...

@typing.final
class PaymentRequest(winrt.system.Object):
    @typing.overload
    def __new__(cls: typing.Type[Self], details: PaymentDetails, method_data: typing.Iterable[PaymentMethodData], merchant_info: PaymentMerchantInfo, options: PaymentOptions, id: str) -> Self: ...
    @typing.overload
    def __new__(cls: typing.Type[Self], details: PaymentDetails, method_data: typing.Iterable[PaymentMethodData]) -> Self: ...
    @typing.overload
    def __new__(cls: typing.Type[Self], details: PaymentDetails, method_data: typing.Iterable[PaymentMethodData], merchant_info: PaymentMerchantInfo) -> Self: ...
    @typing.overload
    def __new__(cls: typing.Type[Self], details: PaymentDetails, method_data: typing.Iterable[PaymentMethodData], merchant_info: PaymentMerchantInfo, options: PaymentOptions) -> Self: ...
    # Windows.ApplicationModel.Payments.PaymentDetails Windows.ApplicationModel.Payments.PaymentRequest::get_Details()
    @_property
    def details(self) -> PaymentDetails: ...
    # Windows.ApplicationModel.Payments.PaymentMerchantInfo Windows.ApplicationModel.Payments.PaymentRequest::get_MerchantInfo()
    @_property
    def merchant_info(self) -> PaymentMerchantInfo: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.ApplicationModel.Payments.PaymentMethodData> Windows.ApplicationModel.Payments.PaymentRequest::get_MethodData()
    @_property
    def method_data(self) -> typing.Sequence[PaymentMethodData]: ...
    # Windows.ApplicationModel.Payments.PaymentOptions Windows.ApplicationModel.Payments.PaymentRequest::get_Options()
    @_property
    def options(self) -> PaymentOptions: ...
    # System.String Windows.ApplicationModel.Payments.PaymentRequest::get_Id()
    @_property
    def id(self) -> str: ...

@typing.final
class PaymentRequestChangedArgs(winrt.system.Object):
    # System.Void Windows.ApplicationModel.Payments.PaymentRequestChangedArgs::Acknowledge(Windows.ApplicationModel.Payments.PaymentRequestChangedResult)
    def acknowledge(self, change_result: PaymentRequestChangedResult, /) -> None: ...
    # Windows.ApplicationModel.Payments.PaymentRequestChangeKind Windows.ApplicationModel.Payments.PaymentRequestChangedArgs::get_ChangeKind()
    @_property
    def change_kind(self) -> PaymentRequestChangeKind: ...
    # Windows.ApplicationModel.Payments.PaymentShippingOption Windows.ApplicationModel.Payments.PaymentRequestChangedArgs::get_SelectedShippingOption()
    @_property
    def selected_shipping_option(self) -> PaymentShippingOption: ...
    # Windows.ApplicationModel.Payments.PaymentAddress Windows.ApplicationModel.Payments.PaymentRequestChangedArgs::get_ShippingAddress()
    @_property
    def shipping_address(self) -> PaymentAddress: ...

@typing.final
class PaymentRequestChangedResult(winrt.system.Object):
    @typing.overload
    def __new__(cls: typing.Type[Self], change_accepted_by_merchant: bool) -> Self: ...
    @typing.overload
    def __new__(cls: typing.Type[Self], change_accepted_by_merchant: bool, updated_payment_details: PaymentDetails) -> Self: ...
    # Windows.ApplicationModel.Payments.PaymentDetails Windows.ApplicationModel.Payments.PaymentRequestChangedResult::get_UpdatedPaymentDetails()
    @_property
    def updated_payment_details(self) -> PaymentDetails: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentRequestChangedResult::put_UpdatedPaymentDetails(Windows.ApplicationModel.Payments.PaymentDetails)
    @updated_payment_details.setter
    def updated_payment_details(self, value: PaymentDetails) -> None: ...
    # System.String Windows.ApplicationModel.Payments.PaymentRequestChangedResult::get_Message()
    @_property
    def message(self) -> str: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentRequestChangedResult::put_Message(System.String)
    @message.setter
    def message(self, value: str) -> None: ...
    # System.Boolean Windows.ApplicationModel.Payments.PaymentRequestChangedResult::get_ChangeAcceptedByMerchant()
    @_property
    def change_accepted_by_merchant(self) -> bool: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentRequestChangedResult::put_ChangeAcceptedByMerchant(System.Boolean)
    @change_accepted_by_merchant.setter
    def change_accepted_by_merchant(self, value: bool) -> None: ...

@typing.final
class PaymentRequestSubmitResult(winrt.system.Object):
    # Windows.ApplicationModel.Payments.PaymentResponse Windows.ApplicationModel.Payments.PaymentRequestSubmitResult::get_Response()
    @_property
    def response(self) -> PaymentResponse: ...
    # Windows.ApplicationModel.Payments.PaymentRequestStatus Windows.ApplicationModel.Payments.PaymentRequestSubmitResult::get_Status()
    @_property
    def status(self) -> PaymentRequestStatus: ...

@typing.final
class PaymentResponse(winrt.system.Object):
    # Windows.Foundation.IAsyncAction Windows.ApplicationModel.Payments.PaymentResponse::CompleteAsync(Windows.ApplicationModel.Payments.PaymentRequestCompletionStatus)
    def complete_async(self, status: PaymentRequestCompletionStatus, /) -> windows_foundation.IAsyncAction: ...
    # System.String Windows.ApplicationModel.Payments.PaymentResponse::get_PayerEmail()
    @_property
    def payer_email(self) -> str: ...
    # System.String Windows.ApplicationModel.Payments.PaymentResponse::get_PayerName()
    @_property
    def payer_name(self) -> str: ...
    # System.String Windows.ApplicationModel.Payments.PaymentResponse::get_PayerPhoneNumber()
    @_property
    def payer_phone_number(self) -> str: ...
    # Windows.ApplicationModel.Payments.PaymentToken Windows.ApplicationModel.Payments.PaymentResponse::get_PaymentToken()
    @_property
    def payment_token(self) -> PaymentToken: ...
    # Windows.ApplicationModel.Payments.PaymentAddress Windows.ApplicationModel.Payments.PaymentResponse::get_ShippingAddress()
    @_property
    def shipping_address(self) -> PaymentAddress: ...
    # Windows.ApplicationModel.Payments.PaymentShippingOption Windows.ApplicationModel.Payments.PaymentResponse::get_ShippingOption()
    @_property
    def shipping_option(self) -> PaymentShippingOption: ...

@typing.final
class PaymentShippingOption(winrt.system.Object):
    @typing.overload
    def __new__(cls: typing.Type[Self], label: str, amount: PaymentCurrencyAmount) -> Self: ...
    @typing.overload
    def __new__(cls: typing.Type[Self], label: str, amount: PaymentCurrencyAmount, selected: bool) -> Self: ...
    @typing.overload
    def __new__(cls: typing.Type[Self], label: str, amount: PaymentCurrencyAmount, selected: bool, tag: str) -> Self: ...
    # System.String Windows.ApplicationModel.Payments.PaymentShippingOption::get_Tag()
    @_property
    def tag(self) -> str: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentShippingOption::put_Tag(System.String)
    @tag.setter
    def tag(self, value: str) -> None: ...
    # System.String Windows.ApplicationModel.Payments.PaymentShippingOption::get_Label()
    @_property
    def label(self) -> str: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentShippingOption::put_Label(System.String)
    @label.setter
    def label(self, value: str) -> None: ...
    # System.Boolean Windows.ApplicationModel.Payments.PaymentShippingOption::get_IsSelected()
    @_property
    def is_selected(self) -> bool: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentShippingOption::put_IsSelected(System.Boolean)
    @is_selected.setter
    def is_selected(self, value: bool) -> None: ...
    # Windows.ApplicationModel.Payments.PaymentCurrencyAmount Windows.ApplicationModel.Payments.PaymentShippingOption::get_Amount()
    @_property
    def amount(self) -> PaymentCurrencyAmount: ...
    # System.Void Windows.ApplicationModel.Payments.PaymentShippingOption::put_Amount(Windows.ApplicationModel.Payments.PaymentCurrencyAmount)
    @amount.setter
    def amount(self, value: PaymentCurrencyAmount) -> None: ...

@typing.final
class PaymentToken(winrt.system.Object):
    @typing.overload
    def __new__(cls: typing.Type[Self], payment_method_id: str) -> Self: ...
    @typing.overload
    def __new__(cls: typing.Type[Self], payment_method_id: str, json_details: str) -> Self: ...
    # System.String Windows.ApplicationModel.Payments.PaymentToken::get_JsonDetails()
    @_property
    def json_details(self) -> str: ...
    # System.String Windows.ApplicationModel.Payments.PaymentToken::get_PaymentMethodId()
    @_property
    def payment_method_id(self) -> str: ...

