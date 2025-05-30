// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Globalization_PhoneNumberFormatting_H
#define WINRT_Windows_Globalization_PhoneNumberFormatting_H
#include "winrt/base.h"
static_assert(winrt::check_version(CPPWINRT_VERSION, "2.0.250303.1"), "Mismatched C++/WinRT headers.");
#define CPPWINRT_VERSION "2.0.250303.1"
#include "winrt/Windows.Globalization.h"
#include "winrt/impl/Windows.Foundation.2.h"
#include "winrt/impl/Windows.Globalization.PhoneNumberFormatting.2.h"
namespace winrt::impl
{
    template <typename D> auto consume_Windows_Globalization_PhoneNumberFormatting_IPhoneNumberFormatter<D>::Format(winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberInfo const& number) const
    {
        void* result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->Format(*(void**)(&number), &result));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter>**)this;
            check_hresult(_winrt_abi_type->Format(*(void**)(&number), &result));
        }
        return hstring{ result, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Globalization_PhoneNumberFormatting_IPhoneNumberFormatter<D>::Format(winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberInfo const& number, winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberFormat const& numberFormat) const
    {
        void* result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->FormatWithOutputFormat(*(void**)(&number), static_cast<int32_t>(numberFormat), &result));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter>**)this;
            check_hresult(_winrt_abi_type->FormatWithOutputFormat(*(void**)(&number), static_cast<int32_t>(numberFormat), &result));
        }
        return hstring{ result, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Globalization_PhoneNumberFormatting_IPhoneNumberFormatter<D>::FormatPartialString(param::hstring const& number) const
    {
        void* result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->FormatPartialString(*(void**)(&number), &result));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter>**)this;
            check_hresult(_winrt_abi_type->FormatPartialString(*(void**)(&number), &result));
        }
        return hstring{ result, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Globalization_PhoneNumberFormatting_IPhoneNumberFormatter<D>::FormatString(param::hstring const& number) const
    {
        void* result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->FormatString(*(void**)(&number), &result));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter>**)this;
            check_hresult(_winrt_abi_type->FormatString(*(void**)(&number), &result));
        }
        return hstring{ result, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Globalization_PhoneNumberFormatting_IPhoneNumberFormatter<D>::FormatStringWithLeftToRightMarkers(param::hstring const& number) const
    {
        void* result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->FormatStringWithLeftToRightMarkers(*(void**)(&number), &result));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter>**)this;
            check_hresult(_winrt_abi_type->FormatStringWithLeftToRightMarkers(*(void**)(&number), &result));
        }
        return hstring{ result, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Globalization_PhoneNumberFormatting_IPhoneNumberFormatterStatics<D>::TryCreate(param::hstring const& regionCode, winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberFormatter& phoneNumber) const
    {
        if constexpr (!std::is_same_v<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatterStatics>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatterStatics, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatterStatics>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->TryCreate(*(void**)(&regionCode), impl::bind_out(phoneNumber)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatterStatics>**)this;
            check_hresult(_winrt_abi_type->TryCreate(*(void**)(&regionCode), impl::bind_out(phoneNumber)));
        }
    }
    template <typename D> auto consume_Windows_Globalization_PhoneNumberFormatting_IPhoneNumberFormatterStatics<D>::GetCountryCodeForRegion(param::hstring const& regionCode) const
    {
        int32_t result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatterStatics>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatterStatics, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatterStatics>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->GetCountryCodeForRegion(*(void**)(&regionCode), &result));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatterStatics>**)this;
            check_hresult(_winrt_abi_type->GetCountryCodeForRegion(*(void**)(&regionCode), &result));
        }
        return result;
    }
    template <typename D> auto consume_Windows_Globalization_PhoneNumberFormatting_IPhoneNumberFormatterStatics<D>::GetNationalDirectDialingPrefixForRegion(param::hstring const& regionCode, bool stripNonDigit) const
    {
        void* result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatterStatics>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatterStatics, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatterStatics>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->GetNationalDirectDialingPrefixForRegion(*(void**)(&regionCode), stripNonDigit, &result));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatterStatics>**)this;
            check_hresult(_winrt_abi_type->GetNationalDirectDialingPrefixForRegion(*(void**)(&regionCode), stripNonDigit, &result));
        }
        return hstring{ result, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Globalization_PhoneNumberFormatting_IPhoneNumberFormatterStatics<D>::WrapWithLeftToRightMarkers(param::hstring const& number) const
    {
        void* result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatterStatics>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatterStatics, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatterStatics>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->WrapWithLeftToRightMarkers(*(void**)(&number), &result));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatterStatics>**)this;
            check_hresult(_winrt_abi_type->WrapWithLeftToRightMarkers(*(void**)(&number), &result));
        }
        return hstring{ result, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Globalization_PhoneNumberFormatting_IPhoneNumberInfo<D>::CountryCode() const
    {
        int32_t value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_CountryCode(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>**)this;
            check_hresult(_winrt_abi_type->get_CountryCode(&value));
        }
        return value;
    }
    template <typename D> auto consume_Windows_Globalization_PhoneNumberFormatting_IPhoneNumberInfo<D>::PhoneNumber() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_PhoneNumber(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>**)this;
            check_hresult(_winrt_abi_type->get_PhoneNumber(&value));
        }
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Globalization_PhoneNumberFormatting_IPhoneNumberInfo<D>::GetLengthOfGeographicalAreaCode() const
    {
        int32_t result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->GetLengthOfGeographicalAreaCode(&result));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>**)this;
            check_hresult(_winrt_abi_type->GetLengthOfGeographicalAreaCode(&result));
        }
        return result;
    }
    template <typename D> auto consume_Windows_Globalization_PhoneNumberFormatting_IPhoneNumberInfo<D>::GetNationalSignificantNumber() const
    {
        void* result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->GetNationalSignificantNumber(&result));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>**)this;
            check_hresult(_winrt_abi_type->GetNationalSignificantNumber(&result));
        }
        return hstring{ result, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Globalization_PhoneNumberFormatting_IPhoneNumberInfo<D>::GetLengthOfNationalDestinationCode() const
    {
        int32_t result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->GetLengthOfNationalDestinationCode(&result));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>**)this;
            check_hresult(_winrt_abi_type->GetLengthOfNationalDestinationCode(&result));
        }
        return result;
    }
    template <typename D> auto consume_Windows_Globalization_PhoneNumberFormatting_IPhoneNumberInfo<D>::PredictNumberKind() const
    {
        winrt::Windows::Globalization::PhoneNumberFormatting::PredictedPhoneNumberKind result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->PredictNumberKind(reinterpret_cast<int32_t*>(&result)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>**)this;
            check_hresult(_winrt_abi_type->PredictNumberKind(reinterpret_cast<int32_t*>(&result)));
        }
        return result;
    }
    template <typename D> auto consume_Windows_Globalization_PhoneNumberFormatting_IPhoneNumberInfo<D>::GetGeographicRegionCode() const
    {
        void* result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->GetGeographicRegionCode(&result));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>**)this;
            check_hresult(_winrt_abi_type->GetGeographicRegionCode(&result));
        }
        return hstring{ result, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Globalization_PhoneNumberFormatting_IPhoneNumberInfo<D>::CheckNumberMatch(winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberInfo const& otherNumber) const
    {
        winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberMatchResult result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->CheckNumberMatch(*(void**)(&otherNumber), reinterpret_cast<int32_t*>(&result)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>**)this;
            check_hresult(_winrt_abi_type->CheckNumberMatch(*(void**)(&otherNumber), reinterpret_cast<int32_t*>(&result)));
        }
        return result;
    }
    template <typename D> auto consume_Windows_Globalization_PhoneNumberFormatting_IPhoneNumberInfoFactory<D>::Create(param::hstring const& number) const
    {
        void* result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfoFactory>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfoFactory, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfoFactory>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->Create(*(void**)(&number), &result));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfoFactory>**)this;
            check_hresult(_winrt_abi_type->Create(*(void**)(&number), &result));
        }
        return winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberInfo{ result, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Globalization_PhoneNumberFormatting_IPhoneNumberInfoStatics<D>::TryParse(param::hstring const& input, winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberInfo& phoneNumber) const
    {
        winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberParseResult result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfoStatics>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfoStatics, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfoStatics>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->TryParse(*(void**)(&input), impl::bind_out(phoneNumber), reinterpret_cast<int32_t*>(&result)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfoStatics>**)this;
            check_hresult(_winrt_abi_type->TryParse(*(void**)(&input), impl::bind_out(phoneNumber), reinterpret_cast<int32_t*>(&result)));
        }
        return result;
    }
    template <typename D> auto consume_Windows_Globalization_PhoneNumberFormatting_IPhoneNumberInfoStatics<D>::TryParse(param::hstring const& input, param::hstring const& regionCode, winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberInfo& phoneNumber) const
    {
        winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberParseResult result{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfoStatics>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfoStatics, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfoStatics>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->TryParseWithRegion(*(void**)(&input), *(void**)(&regionCode), impl::bind_out(phoneNumber), reinterpret_cast<int32_t*>(&result)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfoStatics>**)this;
            check_hresult(_winrt_abi_type->TryParseWithRegion(*(void**)(&input), *(void**)(&regionCode), impl::bind_out(phoneNumber), reinterpret_cast<int32_t*>(&result)));
        }
        return result;
    }
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter> : produce_base<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter>
    {
        int32_t __stdcall Format(void* number, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<hstring>(this->shim().Format(*reinterpret_cast<winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberInfo const*>(&number)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall FormatWithOutputFormat(void* number, int32_t numberFormat, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<hstring>(this->shim().Format(*reinterpret_cast<winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberInfo const*>(&number), *reinterpret_cast<winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberFormat const*>(&numberFormat)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall FormatPartialString(void* number, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<hstring>(this->shim().FormatPartialString(*reinterpret_cast<hstring const*>(&number)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall FormatString(void* number, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<hstring>(this->shim().FormatString(*reinterpret_cast<hstring const*>(&number)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall FormatStringWithLeftToRightMarkers(void* number, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<hstring>(this->shim().FormatStringWithLeftToRightMarkers(*reinterpret_cast<hstring const*>(&number)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatterStatics> : produce_base<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatterStatics>
    {
        int32_t __stdcall TryCreate(void* regionCode, void** phoneNumber) noexcept final try
        {
            clear_abi(phoneNumber);
            typename D::abi_guard guard(this->shim());
            this->shim().TryCreate(*reinterpret_cast<hstring const*>(&regionCode), *reinterpret_cast<winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberFormatter*>(phoneNumber));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetCountryCodeForRegion(void* regionCode, int32_t* result) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *result = detach_from<int32_t>(this->shim().GetCountryCodeForRegion(*reinterpret_cast<hstring const*>(&regionCode)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetNationalDirectDialingPrefixForRegion(void* regionCode, bool stripNonDigit, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<hstring>(this->shim().GetNationalDirectDialingPrefixForRegion(*reinterpret_cast<hstring const*>(&regionCode), stripNonDigit));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall WrapWithLeftToRightMarkers(void* number, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<hstring>(this->shim().WrapWithLeftToRightMarkers(*reinterpret_cast<hstring const*>(&number)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo> : produce_base<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo>
    {
        int32_t __stdcall get_CountryCode(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<int32_t>(this->shim().CountryCode());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_PhoneNumber(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().PhoneNumber());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetLengthOfGeographicalAreaCode(int32_t* result) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *result = detach_from<int32_t>(this->shim().GetLengthOfGeographicalAreaCode());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetNationalSignificantNumber(void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<hstring>(this->shim().GetNationalSignificantNumber());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetLengthOfNationalDestinationCode(int32_t* result) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *result = detach_from<int32_t>(this->shim().GetLengthOfNationalDestinationCode());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall PredictNumberKind(int32_t* result) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *result = detach_from<winrt::Windows::Globalization::PhoneNumberFormatting::PredictedPhoneNumberKind>(this->shim().PredictNumberKind());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetGeographicRegionCode(void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<hstring>(this->shim().GetGeographicRegionCode());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall CheckNumberMatch(void* otherNumber, int32_t* result) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *result = detach_from<winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberMatchResult>(this->shim().CheckNumberMatch(*reinterpret_cast<winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberInfo const*>(&otherNumber)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfoFactory> : produce_base<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfoFactory>
    {
        int32_t __stdcall Create(void* number, void** result) noexcept final try
        {
            clear_abi(result);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberInfo>(this->shim().Create(*reinterpret_cast<hstring const*>(&number)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfoStatics> : produce_base<D, winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfoStatics>
    {
        int32_t __stdcall TryParse(void* input, void** phoneNumber, int32_t* result) noexcept final try
        {
            clear_abi(phoneNumber);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberParseResult>(this->shim().TryParse(*reinterpret_cast<hstring const*>(&input), *reinterpret_cast<winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberInfo*>(phoneNumber)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall TryParseWithRegion(void* input, void* regionCode, void** phoneNumber, int32_t* result) noexcept final try
        {
            clear_abi(phoneNumber);
            typename D::abi_guard guard(this->shim());
            *result = detach_from<winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberParseResult>(this->shim().TryParse(*reinterpret_cast<hstring const*>(&input), *reinterpret_cast<hstring const*>(&regionCode), *reinterpret_cast<winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberInfo*>(phoneNumber)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
}
WINRT_EXPORT namespace winrt::Windows::Globalization::PhoneNumberFormatting
{
    inline PhoneNumberFormatter::PhoneNumberFormatter() :
        PhoneNumberFormatter(impl::call_factory_cast<PhoneNumberFormatter(*)(winrt::Windows::Foundation::IActivationFactory const&), PhoneNumberFormatter>([](winrt::Windows::Foundation::IActivationFactory const& f) { return f.template ActivateInstance<PhoneNumberFormatter>(); }))
    {
    }
    inline auto PhoneNumberFormatter::TryCreate(param::hstring const& regionCode, winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberFormatter& phoneNumber)
    {
        impl::call_factory<PhoneNumberFormatter, IPhoneNumberFormatterStatics>([&](IPhoneNumberFormatterStatics const& f) { return f.TryCreate(regionCode, phoneNumber); });
    }
    inline auto PhoneNumberFormatter::GetCountryCodeForRegion(param::hstring const& regionCode)
    {
        return impl::call_factory<PhoneNumberFormatter, IPhoneNumberFormatterStatics>([&](IPhoneNumberFormatterStatics const& f) { return f.GetCountryCodeForRegion(regionCode); });
    }
    inline auto PhoneNumberFormatter::GetNationalDirectDialingPrefixForRegion(param::hstring const& regionCode, bool stripNonDigit)
    {
        return impl::call_factory<PhoneNumberFormatter, IPhoneNumberFormatterStatics>([&](IPhoneNumberFormatterStatics const& f) { return f.GetNationalDirectDialingPrefixForRegion(regionCode, stripNonDigit); });
    }
    inline auto PhoneNumberFormatter::WrapWithLeftToRightMarkers(param::hstring const& number)
    {
        return impl::call_factory<PhoneNumberFormatter, IPhoneNumberFormatterStatics>([&](IPhoneNumberFormatterStatics const& f) { return f.WrapWithLeftToRightMarkers(number); });
    }
    inline PhoneNumberInfo::PhoneNumberInfo(param::hstring const& number) :
        PhoneNumberInfo(impl::call_factory<PhoneNumberInfo, IPhoneNumberInfoFactory>([&](IPhoneNumberInfoFactory const& f) { return f.Create(number); }))
    {
    }
    inline auto PhoneNumberInfo::TryParse(param::hstring const& input, winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberInfo& phoneNumber)
    {
        return impl::call_factory<PhoneNumberInfo, IPhoneNumberInfoStatics>([&](IPhoneNumberInfoStatics const& f) { return f.TryParse(input, phoneNumber); });
    }
    inline auto PhoneNumberInfo::TryParse(param::hstring const& input, param::hstring const& regionCode, winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberInfo& phoneNumber)
    {
        return impl::call_factory<PhoneNumberInfo, IPhoneNumberInfoStatics>([&](IPhoneNumberInfoStatics const& f) { return f.TryParse(input, regionCode, phoneNumber); });
    }
}
namespace std
{
#ifndef WINRT_LEAN_AND_MEAN
    template<> struct hash<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatter> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberFormatterStatics> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfo> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfoFactory> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Globalization::PhoneNumberFormatting::IPhoneNumberInfoStatics> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberFormatter> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberInfo> : winrt::impl::hash_base {};
#endif
#ifdef __cpp_lib_format
    template<> struct formatter<winrt::Windows::Globalization::PhoneNumberFormatting::PhoneNumberInfo, wchar_t> : formatter<winrt::Windows::Foundation::IStringable, wchar_t> {};
#endif
}
#endif
