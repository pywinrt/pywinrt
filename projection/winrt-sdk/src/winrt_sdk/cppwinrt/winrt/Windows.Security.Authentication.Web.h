// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Security_Authentication_Web_H
#define WINRT_Windows_Security_Authentication_Web_H
#include "winrt/base.h"
static_assert(winrt::check_version(CPPWINRT_VERSION, "2.0.250303.1"), "Mismatched C++/WinRT headers.");
#define CPPWINRT_VERSION "2.0.250303.1"
#include "winrt/impl/Windows.Foundation.2.h"
#include "winrt/impl/Windows.Foundation.Collections.2.h"
#include "winrt/impl/Windows.Security.Authentication.Web.2.h"
namespace winrt::impl
{
    template <typename D> auto consume_Windows_Security_Authentication_Web_IWebAuthenticationBrokerStatics<D>::AuthenticateAsync(winrt::Windows::Security::Authentication::Web::WebAuthenticationOptions const& options, winrt::Windows::Foundation::Uri const& requestUri, winrt::Windows::Foundation::Uri const& callbackUri) const
    {
        void* asyncInfo{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->AuthenticateWithCallbackUriAsync(static_cast<uint32_t>(options), *(void**)(&requestUri), *(void**)(&callbackUri), &asyncInfo));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics>**)this;
            check_hresult(_winrt_abi_type->AuthenticateWithCallbackUriAsync(static_cast<uint32_t>(options), *(void**)(&requestUri), *(void**)(&callbackUri), &asyncInfo));
        }
        return winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Security::Authentication::Web::WebAuthenticationResult>{ asyncInfo, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Security_Authentication_Web_IWebAuthenticationBrokerStatics<D>::AuthenticateAsync(winrt::Windows::Security::Authentication::Web::WebAuthenticationOptions const& options, winrt::Windows::Foundation::Uri const& requestUri) const
    {
        void* asyncInfo{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->AuthenticateWithoutCallbackUriAsync(static_cast<uint32_t>(options), *(void**)(&requestUri), &asyncInfo));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics>**)this;
            check_hresult(_winrt_abi_type->AuthenticateWithoutCallbackUriAsync(static_cast<uint32_t>(options), *(void**)(&requestUri), &asyncInfo));
        }
        return winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Security::Authentication::Web::WebAuthenticationResult>{ asyncInfo, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Security_Authentication_Web_IWebAuthenticationBrokerStatics<D>::GetCurrentApplicationCallbackUri() const
    {
        void* callbackUri{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->GetCurrentApplicationCallbackUri(&callbackUri));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics>**)this;
            check_hresult(_winrt_abi_type->GetCurrentApplicationCallbackUri(&callbackUri));
        }
        return winrt::Windows::Foundation::Uri{ callbackUri, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Security_Authentication_Web_IWebAuthenticationBrokerStatics2<D>::AuthenticateAndContinue(winrt::Windows::Foundation::Uri const& requestUri) const
    {
        if constexpr (!std::is_same_v<D, winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->AuthenticateAndContinue(*(void**)(&requestUri)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2>**)this;
            check_hresult(_winrt_abi_type->AuthenticateAndContinue(*(void**)(&requestUri)));
        }
    }
    template <typename D> auto consume_Windows_Security_Authentication_Web_IWebAuthenticationBrokerStatics2<D>::AuthenticateAndContinue(winrt::Windows::Foundation::Uri const& requestUri, winrt::Windows::Foundation::Uri const& callbackUri) const
    {
        if constexpr (!std::is_same_v<D, winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->AuthenticateWithCallbackUriAndContinue(*(void**)(&requestUri), *(void**)(&callbackUri)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2>**)this;
            check_hresult(_winrt_abi_type->AuthenticateWithCallbackUriAndContinue(*(void**)(&requestUri), *(void**)(&callbackUri)));
        }
    }
    template <typename D> auto consume_Windows_Security_Authentication_Web_IWebAuthenticationBrokerStatics2<D>::AuthenticateAndContinue(winrt::Windows::Foundation::Uri const& requestUri, winrt::Windows::Foundation::Uri const& callbackUri, winrt::Windows::Foundation::Collections::ValueSet const& continuationData, winrt::Windows::Security::Authentication::Web::WebAuthenticationOptions const& options) const
    {
        if constexpr (!std::is_same_v<D, winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->AuthenticateWithCallbackUriContinuationDataAndOptionsAndContinue(*(void**)(&requestUri), *(void**)(&callbackUri), *(void**)(&continuationData), static_cast<uint32_t>(options)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2>**)this;
            check_hresult(_winrt_abi_type->AuthenticateWithCallbackUriContinuationDataAndOptionsAndContinue(*(void**)(&requestUri), *(void**)(&callbackUri), *(void**)(&continuationData), static_cast<uint32_t>(options)));
        }
    }
    template <typename D> auto consume_Windows_Security_Authentication_Web_IWebAuthenticationBrokerStatics2<D>::AuthenticateSilentlyAsync(winrt::Windows::Foundation::Uri const& requestUri) const
    {
        void* asyncInfo{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->AuthenticateSilentlyAsync(*(void**)(&requestUri), &asyncInfo));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2>**)this;
            check_hresult(_winrt_abi_type->AuthenticateSilentlyAsync(*(void**)(&requestUri), &asyncInfo));
        }
        return winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Security::Authentication::Web::WebAuthenticationResult>{ asyncInfo, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Security_Authentication_Web_IWebAuthenticationBrokerStatics2<D>::AuthenticateSilentlyAsync(winrt::Windows::Foundation::Uri const& requestUri, winrt::Windows::Security::Authentication::Web::WebAuthenticationOptions const& options) const
    {
        void* asyncInfo{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->AuthenticateSilentlyWithOptionsAsync(*(void**)(&requestUri), static_cast<uint32_t>(options), &asyncInfo));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2>**)this;
            check_hresult(_winrt_abi_type->AuthenticateSilentlyWithOptionsAsync(*(void**)(&requestUri), static_cast<uint32_t>(options), &asyncInfo));
        }
        return winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Security::Authentication::Web::WebAuthenticationResult>{ asyncInfo, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Security_Authentication_Web_IWebAuthenticationResult<D>::ResponseData() const
    {
        void* value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Security::Authentication::Web::IWebAuthenticationResult>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Security::Authentication::Web::IWebAuthenticationResult, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationResult>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_ResponseData(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationResult>**)this;
            check_hresult(_winrt_abi_type->get_ResponseData(&value));
        }
        return hstring{ value, take_ownership_from_abi };
    }
    template <typename D> auto consume_Windows_Security_Authentication_Web_IWebAuthenticationResult<D>::ResponseStatus() const
    {
        winrt::Windows::Security::Authentication::Web::WebAuthenticationStatus value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Security::Authentication::Web::IWebAuthenticationResult>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Security::Authentication::Web::IWebAuthenticationResult, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationResult>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_ResponseStatus(reinterpret_cast<int32_t*>(&value)));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationResult>**)this;
            check_hresult(_winrt_abi_type->get_ResponseStatus(reinterpret_cast<int32_t*>(&value)));
        }
        return value;
    }
    template <typename D> auto consume_Windows_Security_Authentication_Web_IWebAuthenticationResult<D>::ResponseErrorDetail() const
    {
        uint32_t value{};
        if constexpr (!std::is_same_v<D, winrt::Windows::Security::Authentication::Web::IWebAuthenticationResult>)
        {
            winrt::hresult _winrt_cast_result_code;
            auto const _winrt_casted_result = impl::try_as_with_reason<winrt::Windows::Security::Authentication::Web::IWebAuthenticationResult, D const*>(static_cast<D const*>(this), _winrt_cast_result_code);
            check_hresult(_winrt_cast_result_code);
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationResult>**)&_winrt_casted_result;
            check_hresult(_winrt_abi_type->get_ResponseErrorDetail(&value));
        }
        else
        {
            auto const _winrt_abi_type = *(abi_t<winrt::Windows::Security::Authentication::Web::IWebAuthenticationResult>**)this;
            check_hresult(_winrt_abi_type->get_ResponseErrorDetail(&value));
        }
        return value;
    }
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics> : produce_base<D, winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics>
    {
        int32_t __stdcall AuthenticateWithCallbackUriAsync(uint32_t options, void* requestUri, void* callbackUri, void** asyncInfo) noexcept final try
        {
            clear_abi(asyncInfo);
            typename D::abi_guard guard(this->shim());
            *asyncInfo = detach_from<winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Security::Authentication::Web::WebAuthenticationResult>>(this->shim().AuthenticateAsync(*reinterpret_cast<winrt::Windows::Security::Authentication::Web::WebAuthenticationOptions const*>(&options), *reinterpret_cast<winrt::Windows::Foundation::Uri const*>(&requestUri), *reinterpret_cast<winrt::Windows::Foundation::Uri const*>(&callbackUri)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall AuthenticateWithoutCallbackUriAsync(uint32_t options, void* requestUri, void** asyncInfo) noexcept final try
        {
            clear_abi(asyncInfo);
            typename D::abi_guard guard(this->shim());
            *asyncInfo = detach_from<winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Security::Authentication::Web::WebAuthenticationResult>>(this->shim().AuthenticateAsync(*reinterpret_cast<winrt::Windows::Security::Authentication::Web::WebAuthenticationOptions const*>(&options), *reinterpret_cast<winrt::Windows::Foundation::Uri const*>(&requestUri)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall GetCurrentApplicationCallbackUri(void** callbackUri) noexcept final try
        {
            clear_abi(callbackUri);
            typename D::abi_guard guard(this->shim());
            *callbackUri = detach_from<winrt::Windows::Foundation::Uri>(this->shim().GetCurrentApplicationCallbackUri());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2> : produce_base<D, winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2>
    {
        int32_t __stdcall AuthenticateAndContinue(void* requestUri) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().AuthenticateAndContinue(*reinterpret_cast<winrt::Windows::Foundation::Uri const*>(&requestUri));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall AuthenticateWithCallbackUriAndContinue(void* requestUri, void* callbackUri) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().AuthenticateAndContinue(*reinterpret_cast<winrt::Windows::Foundation::Uri const*>(&requestUri), *reinterpret_cast<winrt::Windows::Foundation::Uri const*>(&callbackUri));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall AuthenticateWithCallbackUriContinuationDataAndOptionsAndContinue(void* requestUri, void* callbackUri, void* continuationData, uint32_t options) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            this->shim().AuthenticateAndContinue(*reinterpret_cast<winrt::Windows::Foundation::Uri const*>(&requestUri), *reinterpret_cast<winrt::Windows::Foundation::Uri const*>(&callbackUri), *reinterpret_cast<winrt::Windows::Foundation::Collections::ValueSet const*>(&continuationData), *reinterpret_cast<winrt::Windows::Security::Authentication::Web::WebAuthenticationOptions const*>(&options));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall AuthenticateSilentlyAsync(void* requestUri, void** asyncInfo) noexcept final try
        {
            clear_abi(asyncInfo);
            typename D::abi_guard guard(this->shim());
            *asyncInfo = detach_from<winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Security::Authentication::Web::WebAuthenticationResult>>(this->shim().AuthenticateSilentlyAsync(*reinterpret_cast<winrt::Windows::Foundation::Uri const*>(&requestUri)));
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall AuthenticateSilentlyWithOptionsAsync(void* requestUri, uint32_t options, void** asyncInfo) noexcept final try
        {
            clear_abi(asyncInfo);
            typename D::abi_guard guard(this->shim());
            *asyncInfo = detach_from<winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Security::Authentication::Web::WebAuthenticationResult>>(this->shim().AuthenticateSilentlyAsync(*reinterpret_cast<winrt::Windows::Foundation::Uri const*>(&requestUri), *reinterpret_cast<winrt::Windows::Security::Authentication::Web::WebAuthenticationOptions const*>(&options)));
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
#ifndef WINRT_LEAN_AND_MEAN
    template <typename D>
    struct produce<D, winrt::Windows::Security::Authentication::Web::IWebAuthenticationResult> : produce_base<D, winrt::Windows::Security::Authentication::Web::IWebAuthenticationResult>
    {
        int32_t __stdcall get_ResponseData(void** value) noexcept final try
        {
            clear_abi(value);
            typename D::abi_guard guard(this->shim());
            *value = detach_from<hstring>(this->shim().ResponseData());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ResponseStatus(int32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<winrt::Windows::Security::Authentication::Web::WebAuthenticationStatus>(this->shim().ResponseStatus());
            return 0;
        }
        catch (...) { return to_hresult(); }
        int32_t __stdcall get_ResponseErrorDetail(uint32_t* value) noexcept final try
        {
            typename D::abi_guard guard(this->shim());
            *value = detach_from<uint32_t>(this->shim().ResponseErrorDetail());
            return 0;
        }
        catch (...) { return to_hresult(); }
    };
#endif
}
WINRT_EXPORT namespace winrt::Windows::Security::Authentication::Web
{
    constexpr auto operator|(WebAuthenticationOptions const left, WebAuthenticationOptions const right) noexcept
    {
        return static_cast<WebAuthenticationOptions>(impl::to_underlying_type(left) | impl::to_underlying_type(right));
    }
    constexpr auto operator|=(WebAuthenticationOptions& left, WebAuthenticationOptions const right) noexcept
    {
        left = left | right;
        return left;
    }
    constexpr auto operator&(WebAuthenticationOptions const left, WebAuthenticationOptions const right) noexcept
    {
        return static_cast<WebAuthenticationOptions>(impl::to_underlying_type(left) & impl::to_underlying_type(right));
    }
    constexpr auto operator&=(WebAuthenticationOptions& left, WebAuthenticationOptions const right) noexcept
    {
        left = left & right;
        return left;
    }
    constexpr auto operator~(WebAuthenticationOptions const value) noexcept
    {
        return static_cast<WebAuthenticationOptions>(~impl::to_underlying_type(value));
    }
    constexpr auto operator^(WebAuthenticationOptions const left, WebAuthenticationOptions const right) noexcept
    {
        return static_cast<WebAuthenticationOptions>(impl::to_underlying_type(left) ^ impl::to_underlying_type(right));
    }
    constexpr auto operator^=(WebAuthenticationOptions& left, WebAuthenticationOptions const right) noexcept
    {
        left = left ^ right;
        return left;
    }
    inline auto WebAuthenticationBroker::AuthenticateAsync(winrt::Windows::Security::Authentication::Web::WebAuthenticationOptions const& options, winrt::Windows::Foundation::Uri const& requestUri, winrt::Windows::Foundation::Uri const& callbackUri)
    {
        return impl::call_factory<WebAuthenticationBroker, IWebAuthenticationBrokerStatics>([&](IWebAuthenticationBrokerStatics const& f) { return f.AuthenticateAsync(options, requestUri, callbackUri); });
    }
    inline auto WebAuthenticationBroker::AuthenticateAsync(winrt::Windows::Security::Authentication::Web::WebAuthenticationOptions const& options, winrt::Windows::Foundation::Uri const& requestUri)
    {
        return impl::call_factory<WebAuthenticationBroker, IWebAuthenticationBrokerStatics>([&](IWebAuthenticationBrokerStatics const& f) { return f.AuthenticateAsync(options, requestUri); });
    }
    inline auto WebAuthenticationBroker::GetCurrentApplicationCallbackUri()
    {
        return impl::call_factory_cast<winrt::Windows::Foundation::Uri(*)(IWebAuthenticationBrokerStatics const&), WebAuthenticationBroker, IWebAuthenticationBrokerStatics>([](IWebAuthenticationBrokerStatics const& f) { return f.GetCurrentApplicationCallbackUri(); });
    }
    inline auto WebAuthenticationBroker::AuthenticateAndContinue(winrt::Windows::Foundation::Uri const& requestUri)
    {
        impl::call_factory<WebAuthenticationBroker, IWebAuthenticationBrokerStatics2>([&](IWebAuthenticationBrokerStatics2 const& f) { return f.AuthenticateAndContinue(requestUri); });
    }
    inline auto WebAuthenticationBroker::AuthenticateAndContinue(winrt::Windows::Foundation::Uri const& requestUri, winrt::Windows::Foundation::Uri const& callbackUri)
    {
        impl::call_factory<WebAuthenticationBroker, IWebAuthenticationBrokerStatics2>([&](IWebAuthenticationBrokerStatics2 const& f) { return f.AuthenticateAndContinue(requestUri, callbackUri); });
    }
    inline auto WebAuthenticationBroker::AuthenticateAndContinue(winrt::Windows::Foundation::Uri const& requestUri, winrt::Windows::Foundation::Uri const& callbackUri, winrt::Windows::Foundation::Collections::ValueSet const& continuationData, winrt::Windows::Security::Authentication::Web::WebAuthenticationOptions const& options)
    {
        impl::call_factory<WebAuthenticationBroker, IWebAuthenticationBrokerStatics2>([&](IWebAuthenticationBrokerStatics2 const& f) { return f.AuthenticateAndContinue(requestUri, callbackUri, continuationData, options); });
    }
    inline auto WebAuthenticationBroker::AuthenticateSilentlyAsync(winrt::Windows::Foundation::Uri const& requestUri)
    {
        return impl::call_factory<WebAuthenticationBroker, IWebAuthenticationBrokerStatics2>([&](IWebAuthenticationBrokerStatics2 const& f) { return f.AuthenticateSilentlyAsync(requestUri); });
    }
    inline auto WebAuthenticationBroker::AuthenticateSilentlyAsync(winrt::Windows::Foundation::Uri const& requestUri, winrt::Windows::Security::Authentication::Web::WebAuthenticationOptions const& options)
    {
        return impl::call_factory<WebAuthenticationBroker, IWebAuthenticationBrokerStatics2>([&](IWebAuthenticationBrokerStatics2 const& f) { return f.AuthenticateSilentlyAsync(requestUri, options); });
    }
}
namespace std
{
#ifndef WINRT_LEAN_AND_MEAN
    template<> struct hash<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Security::Authentication::Web::IWebAuthenticationBrokerStatics2> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Security::Authentication::Web::IWebAuthenticationResult> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Security::Authentication::Web::WebAuthenticationBroker> : winrt::impl::hash_base {};
    template<> struct hash<winrt::Windows::Security::Authentication::Web::WebAuthenticationResult> : winrt::impl::hash_base {};
#endif
#ifdef __cpp_lib_format
#endif
}
#endif
