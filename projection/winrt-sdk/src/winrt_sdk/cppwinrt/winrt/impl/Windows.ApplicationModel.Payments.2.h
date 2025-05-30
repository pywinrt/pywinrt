// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_ApplicationModel_Payments_2_H
#define WINRT_Windows_ApplicationModel_Payments_2_H
#include "winrt/impl/Windows.Foundation.1.h"
#include "winrt/impl/Windows.Foundation.Collections.1.h"
#include "winrt/impl/Windows.ApplicationModel.Payments.1.h"
WINRT_EXPORT namespace winrt::Windows::ApplicationModel::Payments
{
    struct PaymentRequestChangedHandler : winrt::Windows::Foundation::IUnknown
    {
        PaymentRequestChangedHandler(std::nullptr_t = nullptr) noexcept {}
        PaymentRequestChangedHandler(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IUnknown(ptr, take_ownership_from_abi) {}
        template <typename L> PaymentRequestChangedHandler(L lambda);
        template <typename F> PaymentRequestChangedHandler(F* function);
        template <typename O, typename M> PaymentRequestChangedHandler(O* object, M method);
        template <typename O, typename M> PaymentRequestChangedHandler(com_ptr<O>&& object, M method);
        template <typename O, typename LM> PaymentRequestChangedHandler(weak_ref<O>&& object, LM&& lambda_or_method);
        template <typename O, typename M> PaymentRequestChangedHandler(std::shared_ptr<O>&& object, M method);
        template <typename O, typename LM> PaymentRequestChangedHandler(std::weak_ptr<O>&& object, LM&& lambda_or_method);
        auto operator()(winrt::Windows::ApplicationModel::Payments::PaymentRequest const& paymentRequest, winrt::Windows::ApplicationModel::Payments::PaymentRequestChangedArgs const& args) const;
    };
    struct WINRT_IMPL_EMPTY_BASES PaymentAddress : winrt::Windows::ApplicationModel::Payments::IPaymentAddress
    {
        PaymentAddress(std::nullptr_t) noexcept {}
        PaymentAddress(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Payments::IPaymentAddress(ptr, take_ownership_from_abi) {}
        PaymentAddress();
    };
    struct WINRT_IMPL_EMPTY_BASES PaymentCanMakePaymentResult : winrt::Windows::ApplicationModel::Payments::IPaymentCanMakePaymentResult
    {
        PaymentCanMakePaymentResult(std::nullptr_t) noexcept {}
        PaymentCanMakePaymentResult(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Payments::IPaymentCanMakePaymentResult(ptr, take_ownership_from_abi) {}
        explicit PaymentCanMakePaymentResult(winrt::Windows::ApplicationModel::Payments::PaymentCanMakePaymentResultStatus const& value);
    };
    struct WINRT_IMPL_EMPTY_BASES PaymentCurrencyAmount : winrt::Windows::ApplicationModel::Payments::IPaymentCurrencyAmount
    {
        PaymentCurrencyAmount(std::nullptr_t) noexcept {}
        PaymentCurrencyAmount(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Payments::IPaymentCurrencyAmount(ptr, take_ownership_from_abi) {}
        PaymentCurrencyAmount(param::hstring const& value, param::hstring const& currency);
        PaymentCurrencyAmount(param::hstring const& value, param::hstring const& currency, param::hstring const& currencySystem);
    };
    struct WINRT_IMPL_EMPTY_BASES PaymentDetails : winrt::Windows::ApplicationModel::Payments::IPaymentDetails
    {
        PaymentDetails(std::nullptr_t) noexcept {}
        PaymentDetails(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Payments::IPaymentDetails(ptr, take_ownership_from_abi) {}
        PaymentDetails();
        explicit PaymentDetails(winrt::Windows::ApplicationModel::Payments::PaymentItem const& total);
        PaymentDetails(winrt::Windows::ApplicationModel::Payments::PaymentItem const& total, param::iterable<winrt::Windows::ApplicationModel::Payments::PaymentItem> const& displayItems);
    };
    struct WINRT_IMPL_EMPTY_BASES PaymentDetailsModifier : winrt::Windows::ApplicationModel::Payments::IPaymentDetailsModifier
    {
        PaymentDetailsModifier(std::nullptr_t) noexcept {}
        PaymentDetailsModifier(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Payments::IPaymentDetailsModifier(ptr, take_ownership_from_abi) {}
        PaymentDetailsModifier(param::iterable<hstring> const& supportedMethodIds, winrt::Windows::ApplicationModel::Payments::PaymentItem const& total);
        PaymentDetailsModifier(param::iterable<hstring> const& supportedMethodIds, winrt::Windows::ApplicationModel::Payments::PaymentItem const& total, param::iterable<winrt::Windows::ApplicationModel::Payments::PaymentItem> const& additionalDisplayItems);
        PaymentDetailsModifier(param::iterable<hstring> const& supportedMethodIds, winrt::Windows::ApplicationModel::Payments::PaymentItem const& total, param::iterable<winrt::Windows::ApplicationModel::Payments::PaymentItem> const& additionalDisplayItems, param::hstring const& jsonData);
    };
    struct WINRT_IMPL_EMPTY_BASES PaymentItem : winrt::Windows::ApplicationModel::Payments::IPaymentItem
    {
        PaymentItem(std::nullptr_t) noexcept {}
        PaymentItem(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Payments::IPaymentItem(ptr, take_ownership_from_abi) {}
        PaymentItem(param::hstring const& label, winrt::Windows::ApplicationModel::Payments::PaymentCurrencyAmount const& amount);
    };
    struct WINRT_IMPL_EMPTY_BASES PaymentMediator : winrt::Windows::ApplicationModel::Payments::IPaymentMediator,
        impl::require<PaymentMediator, winrt::Windows::ApplicationModel::Payments::IPaymentMediator2>
    {
        PaymentMediator(std::nullptr_t) noexcept {}
        PaymentMediator(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Payments::IPaymentMediator(ptr, take_ownership_from_abi) {}
        PaymentMediator();
    };
    struct WINRT_IMPL_EMPTY_BASES PaymentMerchantInfo : winrt::Windows::ApplicationModel::Payments::IPaymentMerchantInfo
    {
        PaymentMerchantInfo(std::nullptr_t) noexcept {}
        PaymentMerchantInfo(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Payments::IPaymentMerchantInfo(ptr, take_ownership_from_abi) {}
        PaymentMerchantInfo();
        explicit PaymentMerchantInfo(winrt::Windows::Foundation::Uri const& uri);
    };
    struct WINRT_IMPL_EMPTY_BASES PaymentMethodData : winrt::Windows::ApplicationModel::Payments::IPaymentMethodData
    {
        PaymentMethodData(std::nullptr_t) noexcept {}
        PaymentMethodData(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Payments::IPaymentMethodData(ptr, take_ownership_from_abi) {}
        explicit PaymentMethodData(param::iterable<hstring> const& supportedMethodIds);
        PaymentMethodData(param::iterable<hstring> const& supportedMethodIds, param::hstring const& jsonData);
    };
    struct WINRT_IMPL_EMPTY_BASES PaymentOptions : winrt::Windows::ApplicationModel::Payments::IPaymentOptions
    {
        PaymentOptions(std::nullptr_t) noexcept {}
        PaymentOptions(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Payments::IPaymentOptions(ptr, take_ownership_from_abi) {}
        PaymentOptions();
    };
    struct WINRT_IMPL_EMPTY_BASES PaymentRequest : winrt::Windows::ApplicationModel::Payments::IPaymentRequest,
        impl::require<PaymentRequest, winrt::Windows::ApplicationModel::Payments::IPaymentRequest2>
    {
        PaymentRequest(std::nullptr_t) noexcept {}
        PaymentRequest(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Payments::IPaymentRequest(ptr, take_ownership_from_abi) {}
        PaymentRequest(winrt::Windows::ApplicationModel::Payments::PaymentDetails const& details, param::iterable<winrt::Windows::ApplicationModel::Payments::PaymentMethodData> const& methodData);
        PaymentRequest(winrt::Windows::ApplicationModel::Payments::PaymentDetails const& details, param::iterable<winrt::Windows::ApplicationModel::Payments::PaymentMethodData> const& methodData, winrt::Windows::ApplicationModel::Payments::PaymentMerchantInfo const& merchantInfo);
        PaymentRequest(winrt::Windows::ApplicationModel::Payments::PaymentDetails const& details, param::iterable<winrt::Windows::ApplicationModel::Payments::PaymentMethodData> const& methodData, winrt::Windows::ApplicationModel::Payments::PaymentMerchantInfo const& merchantInfo, winrt::Windows::ApplicationModel::Payments::PaymentOptions const& options);
        PaymentRequest(winrt::Windows::ApplicationModel::Payments::PaymentDetails const& details, param::iterable<winrt::Windows::ApplicationModel::Payments::PaymentMethodData> const& methodData, winrt::Windows::ApplicationModel::Payments::PaymentMerchantInfo const& merchantInfo, winrt::Windows::ApplicationModel::Payments::PaymentOptions const& options, param::hstring const& id);
    };
    struct WINRT_IMPL_EMPTY_BASES PaymentRequestChangedArgs : winrt::Windows::ApplicationModel::Payments::IPaymentRequestChangedArgs
    {
        PaymentRequestChangedArgs(std::nullptr_t) noexcept {}
        PaymentRequestChangedArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Payments::IPaymentRequestChangedArgs(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES PaymentRequestChangedResult : winrt::Windows::ApplicationModel::Payments::IPaymentRequestChangedResult
    {
        PaymentRequestChangedResult(std::nullptr_t) noexcept {}
        PaymentRequestChangedResult(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Payments::IPaymentRequestChangedResult(ptr, take_ownership_from_abi) {}
        explicit PaymentRequestChangedResult(bool changeAcceptedByMerchant);
        PaymentRequestChangedResult(bool changeAcceptedByMerchant, winrt::Windows::ApplicationModel::Payments::PaymentDetails const& updatedPaymentDetails);
    };
    struct WINRT_IMPL_EMPTY_BASES PaymentRequestSubmitResult : winrt::Windows::ApplicationModel::Payments::IPaymentRequestSubmitResult
    {
        PaymentRequestSubmitResult(std::nullptr_t) noexcept {}
        PaymentRequestSubmitResult(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Payments::IPaymentRequestSubmitResult(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES PaymentResponse : winrt::Windows::ApplicationModel::Payments::IPaymentResponse
    {
        PaymentResponse(std::nullptr_t) noexcept {}
        PaymentResponse(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Payments::IPaymentResponse(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES PaymentShippingOption : winrt::Windows::ApplicationModel::Payments::IPaymentShippingOption
    {
        PaymentShippingOption(std::nullptr_t) noexcept {}
        PaymentShippingOption(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Payments::IPaymentShippingOption(ptr, take_ownership_from_abi) {}
        PaymentShippingOption(param::hstring const& label, winrt::Windows::ApplicationModel::Payments::PaymentCurrencyAmount const& amount);
        PaymentShippingOption(param::hstring const& label, winrt::Windows::ApplicationModel::Payments::PaymentCurrencyAmount const& amount, bool selected);
        PaymentShippingOption(param::hstring const& label, winrt::Windows::ApplicationModel::Payments::PaymentCurrencyAmount const& amount, bool selected, param::hstring const& tag);
    };
    struct WINRT_IMPL_EMPTY_BASES PaymentToken : winrt::Windows::ApplicationModel::Payments::IPaymentToken
    {
        PaymentToken(std::nullptr_t) noexcept {}
        PaymentToken(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::ApplicationModel::Payments::IPaymentToken(ptr, take_ownership_from_abi) {}
        explicit PaymentToken(param::hstring const& paymentMethodId);
        PaymentToken(param::hstring const& paymentMethodId, param::hstring const& jsonDetails);
    };
}
#endif
