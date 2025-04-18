// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Web.Http.h>
#include <winrt/Windows.Web.Http.Headers.h>
#include <winrt/Microsoft.UI.h>

#include <winrt/Microsoft.Security.Authentication.OAuth.h>

namespace py::proj::Microsoft::Security::Authentication::OAuth
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::Security::Authentication::OAuth::CodeChallengeMethodKind> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Microsoft::Security::Authentication::OAuth::TokenFailureKind> = "i";


    template<>
    struct py_type<winrt::Microsoft::Security::Authentication::OAuth::CodeChallengeMethodKind>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.security.authentication.oauth.CodeChallengeMethodKind";
        static constexpr const char* module_name = "winui3.microsoft.security.authentication.oauth";
        static constexpr const char* type_name = "CodeChallengeMethodKind";
    };

    template<>
    struct py_type<winrt::Microsoft::Security::Authentication::OAuth::TokenFailureKind>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.security.authentication.oauth.TokenFailureKind";
        static constexpr const char* module_name = "winui3.microsoft.security.authentication.oauth";
        static constexpr const char* type_name = "TokenFailureKind";
    };

    template<>
    struct py_type<winrt::Microsoft::Security::Authentication::OAuth::AuthFailure>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.security.authentication.oauth.AuthFailure";
        static constexpr const char* module_name = "winui3.microsoft.security.authentication.oauth";
        static constexpr const char* type_name = "AuthFailure";
    };

    template<>
    struct py_type<winrt::Microsoft::Security::Authentication::OAuth::AuthRequestParams>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.security.authentication.oauth.AuthRequestParams";
        static constexpr const char* module_name = "winui3.microsoft.security.authentication.oauth";
        static constexpr const char* type_name = "AuthRequestParams";
    };

    template<>
    struct py_type<winrt::Microsoft::Security::Authentication::OAuth::AuthRequestResult>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.security.authentication.oauth.AuthRequestResult";
        static constexpr const char* module_name = "winui3.microsoft.security.authentication.oauth";
        static constexpr const char* type_name = "AuthRequestResult";
    };

    template<>
    struct py_type<winrt::Microsoft::Security::Authentication::OAuth::AuthResponse>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.security.authentication.oauth.AuthResponse";
        static constexpr const char* module_name = "winui3.microsoft.security.authentication.oauth";
        static constexpr const char* type_name = "AuthResponse";
    };

    template<>
    struct py_type<winrt::Microsoft::Security::Authentication::OAuth::ClientAuthentication>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.security.authentication.oauth.ClientAuthentication";
        static constexpr const char* module_name = "winui3.microsoft.security.authentication.oauth";
        static constexpr const char* type_name = "ClientAuthentication";
    };

    template<>
    struct py_type<winrt::Microsoft::Security::Authentication::OAuth::OAuth2Manager>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.security.authentication.oauth.OAuth2Manager";
        static constexpr const char* module_name = "winui3.microsoft.security.authentication.oauth";
        static constexpr const char* type_name = "OAuth2Manager";
    };

    template<>
    struct py_type<winrt::Microsoft::Security::Authentication::OAuth::TokenFailure>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.security.authentication.oauth.TokenFailure";
        static constexpr const char* module_name = "winui3.microsoft.security.authentication.oauth";
        static constexpr const char* type_name = "TokenFailure";
    };

    template<>
    struct py_type<winrt::Microsoft::Security::Authentication::OAuth::TokenRequestParams>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.security.authentication.oauth.TokenRequestParams";
        static constexpr const char* module_name = "winui3.microsoft.security.authentication.oauth";
        static constexpr const char* type_name = "TokenRequestParams";
    };

    template<>
    struct py_type<winrt::Microsoft::Security::Authentication::OAuth::TokenRequestResult>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.security.authentication.oauth.TokenRequestResult";
        static constexpr const char* module_name = "winui3.microsoft.security.authentication.oauth";
        static constexpr const char* type_name = "TokenRequestResult";
    };

    template<>
    struct py_type<winrt::Microsoft::Security::Authentication::OAuth::TokenResponse>
    {
        static constexpr std::string_view qualified_name = "winui3.microsoft.security.authentication.oauth.TokenResponse";
        static constexpr const char* module_name = "winui3.microsoft.security.authentication.oauth";
        static constexpr const char* type_name = "TokenResponse";
    };
}

#if __has_include("py.Windows.Data.Json.h")
#include "py.Windows.Data.Json.h"
#endif

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.Web.Http.h")
#include "py.Windows.Web.Http.h"
#endif

#if __has_include("py.Windows.Web.Http.Headers.h")
#include "py.Windows.Web.Http.Headers.h"
#endif

#if __has_include("py.Microsoft.UI.h")
#include "py.Microsoft.UI.h"
#endif

namespace py::impl::Microsoft::Security::Authentication::OAuth
{
}

namespace py::wrapper::Microsoft::Security::Authentication::OAuth
{
    using AuthFailure = py::winrt_wrapper<winrt::Microsoft::Security::Authentication::OAuth::AuthFailure>;
    using AuthRequestParams = py::winrt_wrapper<winrt::Microsoft::Security::Authentication::OAuth::AuthRequestParams>;
    using AuthRequestResult = py::winrt_wrapper<winrt::Microsoft::Security::Authentication::OAuth::AuthRequestResult>;
    using AuthResponse = py::winrt_wrapper<winrt::Microsoft::Security::Authentication::OAuth::AuthResponse>;
    using ClientAuthentication = py::winrt_wrapper<winrt::Microsoft::Security::Authentication::OAuth::ClientAuthentication>;
    using OAuth2Manager = py::winrt_wrapper<winrt::Microsoft::Security::Authentication::OAuth::OAuth2Manager>;
    using TokenFailure = py::winrt_wrapper<winrt::Microsoft::Security::Authentication::OAuth::TokenFailure>;
    using TokenRequestParams = py::winrt_wrapper<winrt::Microsoft::Security::Authentication::OAuth::TokenRequestParams>;
    using TokenRequestResult = py::winrt_wrapper<winrt::Microsoft::Security::Authentication::OAuth::TokenRequestResult>;
    using TokenResponse = py::winrt_wrapper<winrt::Microsoft::Security::Authentication::OAuth::TokenResponse>;
}

namespace py
{
}
