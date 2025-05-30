// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Security_Credentials_UI_0_H
#define WINRT_Windows_Security_Credentials_UI_0_H
WINRT_EXPORT namespace winrt::Windows::Storage::Streams
{
    struct IBuffer;
}
WINRT_EXPORT namespace winrt::Windows::Security::Credentials::UI
{
    enum class AuthenticationProtocol : int32_t
    {
        Basic = 0,
        Digest = 1,
        Ntlm = 2,
        Kerberos = 3,
        Negotiate = 4,
        CredSsp = 5,
        Custom = 6,
    };
    enum class CredentialSaveOption : int32_t
    {
        Unselected = 0,
        Selected = 1,
        Hidden = 2,
    };
    enum class UserConsentVerificationResult : int32_t
    {
        Verified = 0,
        DeviceNotPresent = 1,
        NotConfiguredForUser = 2,
        DisabledByPolicy = 3,
        DeviceBusy = 4,
        RetriesExhausted = 5,
        Canceled = 6,
    };
    enum class UserConsentVerifierAvailability : int32_t
    {
        Available = 0,
        DeviceNotPresent = 1,
        NotConfiguredForUser = 2,
        DisabledByPolicy = 3,
        DeviceBusy = 4,
    };
    struct ICredentialPickerOptions;
    struct ICredentialPickerResults;
    struct ICredentialPickerStatics;
    struct IUserConsentVerifierStatics;
    struct CredentialPicker;
    struct CredentialPickerOptions;
    struct CredentialPickerResults;
    struct UserConsentVerifier;
}
namespace winrt::impl
{
    template <> struct category<winrt::Windows::Security::Credentials::UI::ICredentialPickerOptions>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Security::Credentials::UI::ICredentialPickerResults>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Security::Credentials::UI::ICredentialPickerStatics>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Security::Credentials::UI::IUserConsentVerifierStatics>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Security::Credentials::UI::CredentialPicker>{ using type = class_category; };
    template <> struct category<winrt::Windows::Security::Credentials::UI::CredentialPickerOptions>{ using type = class_category; };
    template <> struct category<winrt::Windows::Security::Credentials::UI::CredentialPickerResults>{ using type = class_category; };
    template <> struct category<winrt::Windows::Security::Credentials::UI::UserConsentVerifier>{ using type = class_category; };
    template <> struct category<winrt::Windows::Security::Credentials::UI::AuthenticationProtocol>{ using type = enum_category; };
    template <> struct category<winrt::Windows::Security::Credentials::UI::CredentialSaveOption>{ using type = enum_category; };
    template <> struct category<winrt::Windows::Security::Credentials::UI::UserConsentVerificationResult>{ using type = enum_category; };
    template <> struct category<winrt::Windows::Security::Credentials::UI::UserConsentVerifierAvailability>{ using type = enum_category; };
    template <> inline constexpr auto& name_v<winrt::Windows::Security::Credentials::UI::CredentialPicker> = L"Windows.Security.Credentials.UI.CredentialPicker";
    template <> inline constexpr auto& name_v<winrt::Windows::Security::Credentials::UI::CredentialPickerOptions> = L"Windows.Security.Credentials.UI.CredentialPickerOptions";
    template <> inline constexpr auto& name_v<winrt::Windows::Security::Credentials::UI::CredentialPickerResults> = L"Windows.Security.Credentials.UI.CredentialPickerResults";
    template <> inline constexpr auto& name_v<winrt::Windows::Security::Credentials::UI::UserConsentVerifier> = L"Windows.Security.Credentials.UI.UserConsentVerifier";
    template <> inline constexpr auto& name_v<winrt::Windows::Security::Credentials::UI::AuthenticationProtocol> = L"Windows.Security.Credentials.UI.AuthenticationProtocol";
    template <> inline constexpr auto& name_v<winrt::Windows::Security::Credentials::UI::CredentialSaveOption> = L"Windows.Security.Credentials.UI.CredentialSaveOption";
    template <> inline constexpr auto& name_v<winrt::Windows::Security::Credentials::UI::UserConsentVerificationResult> = L"Windows.Security.Credentials.UI.UserConsentVerificationResult";
    template <> inline constexpr auto& name_v<winrt::Windows::Security::Credentials::UI::UserConsentVerifierAvailability> = L"Windows.Security.Credentials.UI.UserConsentVerifierAvailability";
    template <> inline constexpr auto& name_v<winrt::Windows::Security::Credentials::UI::ICredentialPickerOptions> = L"Windows.Security.Credentials.UI.ICredentialPickerOptions";
    template <> inline constexpr auto& name_v<winrt::Windows::Security::Credentials::UI::ICredentialPickerResults> = L"Windows.Security.Credentials.UI.ICredentialPickerResults";
    template <> inline constexpr auto& name_v<winrt::Windows::Security::Credentials::UI::ICredentialPickerStatics> = L"Windows.Security.Credentials.UI.ICredentialPickerStatics";
    template <> inline constexpr auto& name_v<winrt::Windows::Security::Credentials::UI::IUserConsentVerifierStatics> = L"Windows.Security.Credentials.UI.IUserConsentVerifierStatics";
    template <> inline constexpr guid guid_v<winrt::Windows::Security::Credentials::UI::ICredentialPickerOptions>{ 0x965A0B4C,0x95FA,0x467F,{ 0x99,0x2B,0x0B,0x22,0xE5,0x85,0x9B,0xF6 } }; // 965A0B4C-95FA-467F-992B-0B22E5859BF6
    template <> inline constexpr guid guid_v<winrt::Windows::Security::Credentials::UI::ICredentialPickerResults>{ 0x1948F99A,0xCC30,0x410C,{ 0x9C,0x38,0xCC,0x08,0x84,0xC5,0xB3,0xD7 } }; // 1948F99A-CC30-410C-9C38-CC0884C5B3D7
    template <> inline constexpr guid guid_v<winrt::Windows::Security::Credentials::UI::ICredentialPickerStatics>{ 0xAA3A5C73,0xC9EA,0x4782,{ 0x99,0xFB,0xE6,0xD7,0xE9,0x38,0xE1,0x2D } }; // AA3A5C73-C9EA-4782-99FB-E6D7E938E12D
    template <> inline constexpr guid guid_v<winrt::Windows::Security::Credentials::UI::IUserConsentVerifierStatics>{ 0xAF4F3F91,0x564C,0x4DDC,{ 0xB8,0xB5,0x97,0x34,0x47,0x62,0x7C,0x65 } }; // AF4F3F91-564C-4DDC-B8B5-973447627C65
    template <> struct default_interface<winrt::Windows::Security::Credentials::UI::CredentialPickerOptions>{ using type = winrt::Windows::Security::Credentials::UI::ICredentialPickerOptions; };
    template <> struct default_interface<winrt::Windows::Security::Credentials::UI::CredentialPickerResults>{ using type = winrt::Windows::Security::Credentials::UI::ICredentialPickerResults; };
    template <> struct abi<winrt::Windows::Security::Credentials::UI::ICredentialPickerOptions>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall put_Caption(void*) noexcept = 0;
            virtual int32_t __stdcall get_Caption(void**) noexcept = 0;
            virtual int32_t __stdcall put_Message(void*) noexcept = 0;
            virtual int32_t __stdcall get_Message(void**) noexcept = 0;
            virtual int32_t __stdcall put_ErrorCode(uint32_t) noexcept = 0;
            virtual int32_t __stdcall get_ErrorCode(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall put_TargetName(void*) noexcept = 0;
            virtual int32_t __stdcall get_TargetName(void**) noexcept = 0;
            virtual int32_t __stdcall put_AuthenticationProtocol(int32_t) noexcept = 0;
            virtual int32_t __stdcall get_AuthenticationProtocol(int32_t*) noexcept = 0;
            virtual int32_t __stdcall put_CustomAuthenticationProtocol(void*) noexcept = 0;
            virtual int32_t __stdcall get_CustomAuthenticationProtocol(void**) noexcept = 0;
            virtual int32_t __stdcall put_PreviousCredential(void*) noexcept = 0;
            virtual int32_t __stdcall get_PreviousCredential(void**) noexcept = 0;
            virtual int32_t __stdcall put_AlwaysDisplayDialog(bool) noexcept = 0;
            virtual int32_t __stdcall get_AlwaysDisplayDialog(bool*) noexcept = 0;
            virtual int32_t __stdcall put_CallerSavesCredential(bool) noexcept = 0;
            virtual int32_t __stdcall get_CallerSavesCredential(bool*) noexcept = 0;
            virtual int32_t __stdcall put_CredentialSaveOption(int32_t) noexcept = 0;
            virtual int32_t __stdcall get_CredentialSaveOption(int32_t*) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Security::Credentials::UI::ICredentialPickerResults>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_ErrorCode(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_CredentialSaveOption(int32_t*) noexcept = 0;
            virtual int32_t __stdcall get_CredentialSaved(bool*) noexcept = 0;
            virtual int32_t __stdcall get_Credential(void**) noexcept = 0;
            virtual int32_t __stdcall get_CredentialDomainName(void**) noexcept = 0;
            virtual int32_t __stdcall get_CredentialUserName(void**) noexcept = 0;
            virtual int32_t __stdcall get_CredentialPassword(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Security::Credentials::UI::ICredentialPickerStatics>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall PickWithOptionsAsync(void*, void**) noexcept = 0;
            virtual int32_t __stdcall PickWithMessageAsync(void*, void*, void**) noexcept = 0;
            virtual int32_t __stdcall PickWithCaptionAsync(void*, void*, void*, void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Security::Credentials::UI::IUserConsentVerifierStatics>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall CheckAvailabilityAsync(void**) noexcept = 0;
            virtual int32_t __stdcall RequestVerificationAsync(void*, void**) noexcept = 0;
        };
    };
    template <typename D>
    struct consume_Windows_Security_Credentials_UI_ICredentialPickerOptions
    {
        auto Caption(param::hstring const& value) const;
        [[nodiscard]] auto Caption() const;
        auto Message(param::hstring const& value) const;
        [[nodiscard]] auto Message() const;
        auto ErrorCode(uint32_t value) const;
        [[nodiscard]] auto ErrorCode() const;
        auto TargetName(param::hstring const& value) const;
        [[nodiscard]] auto TargetName() const;
        auto AuthenticationProtocol(winrt::Windows::Security::Credentials::UI::AuthenticationProtocol const& value) const;
        [[nodiscard]] auto AuthenticationProtocol() const;
        auto CustomAuthenticationProtocol(param::hstring const& value) const;
        [[nodiscard]] auto CustomAuthenticationProtocol() const;
        auto PreviousCredential(winrt::Windows::Storage::Streams::IBuffer const& value) const;
        [[nodiscard]] auto PreviousCredential() const;
        auto AlwaysDisplayDialog(bool value) const;
        [[nodiscard]] auto AlwaysDisplayDialog() const;
        auto CallerSavesCredential(bool value) const;
        [[nodiscard]] auto CallerSavesCredential() const;
        auto CredentialSaveOption(winrt::Windows::Security::Credentials::UI::CredentialSaveOption const& value) const;
        [[nodiscard]] auto CredentialSaveOption() const;
    };
    template <> struct consume<winrt::Windows::Security::Credentials::UI::ICredentialPickerOptions>
    {
        template <typename D> using type = consume_Windows_Security_Credentials_UI_ICredentialPickerOptions<D>;
    };
    template <typename D>
    struct consume_Windows_Security_Credentials_UI_ICredentialPickerResults
    {
        [[nodiscard]] auto ErrorCode() const;
        [[nodiscard]] auto CredentialSaveOption() const;
        [[nodiscard]] auto CredentialSaved() const;
        [[nodiscard]] auto Credential() const;
        [[nodiscard]] auto CredentialDomainName() const;
        [[nodiscard]] auto CredentialUserName() const;
        [[nodiscard]] auto CredentialPassword() const;
    };
    template <> struct consume<winrt::Windows::Security::Credentials::UI::ICredentialPickerResults>
    {
        template <typename D> using type = consume_Windows_Security_Credentials_UI_ICredentialPickerResults<D>;
    };
    template <typename D>
    struct consume_Windows_Security_Credentials_UI_ICredentialPickerStatics
    {
        auto PickAsync(winrt::Windows::Security::Credentials::UI::CredentialPickerOptions const& options) const;
        auto PickAsync(param::hstring const& targetName, param::hstring const& message) const;
        auto PickAsync(param::hstring const& targetName, param::hstring const& message, param::hstring const& caption) const;
    };
    template <> struct consume<winrt::Windows::Security::Credentials::UI::ICredentialPickerStatics>
    {
        template <typename D> using type = consume_Windows_Security_Credentials_UI_ICredentialPickerStatics<D>;
    };
    template <typename D>
    struct consume_Windows_Security_Credentials_UI_IUserConsentVerifierStatics
    {
        auto CheckAvailabilityAsync() const;
        auto RequestVerificationAsync(param::hstring const& message) const;
    };
    template <> struct consume<winrt::Windows::Security::Credentials::UI::IUserConsentVerifierStatics>
    {
        template <typename D> using type = consume_Windows_Security_Credentials_UI_IUserConsentVerifierStatics<D>;
    };
}
#endif
