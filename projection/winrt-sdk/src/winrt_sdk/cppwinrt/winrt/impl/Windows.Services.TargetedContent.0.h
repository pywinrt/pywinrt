// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Services_TargetedContent_0_H
#define WINRT_Windows_Services_TargetedContent_0_H
WINRT_EXPORT namespace winrt::Windows::Foundation
{
    struct Deferral;
    struct EventRegistrationToken;
    struct IAsyncAction;
    template <typename TSender, typename TResult> struct WINRT_IMPL_EMPTY_BASES TypedEventHandler;
    struct Uri;
}
WINRT_EXPORT namespace winrt::Windows::Storage::Streams
{
    struct IRandomAccessStreamReference;
}
WINRT_EXPORT namespace winrt::Windows::Services::TargetedContent
{
    enum class TargetedContentAppInstallationState : int32_t
    {
        NotApplicable = 0,
        NotInstalled = 1,
        Installed = 2,
    };
    enum class TargetedContentAvailability : int32_t
    {
        None = 0,
        Partial = 1,
        All = 2,
    };
    enum class TargetedContentInteraction : int32_t
    {
        Impression = 0,
        ClickThrough = 1,
        Hover = 2,
        Like = 3,
        Dislike = 4,
        Dismiss = 5,
        Ineligible = 6,
        Accept = 7,
        Decline = 8,
        Defer = 9,
        Canceled = 10,
        Conversion = 11,
        Opportunity = 12,
    };
    enum class TargetedContentObjectKind : int32_t
    {
        Collection = 0,
        Item = 1,
        Value = 2,
    };
    enum class TargetedContentValueKind : int32_t
    {
        String = 0,
        Uri = 1,
        Number = 2,
        Boolean = 3,
        File = 4,
        ImageFile = 5,
        Action = 6,
        Strings = 7,
        Uris = 8,
        Numbers = 9,
        Booleans = 10,
        Files = 11,
        ImageFiles = 12,
        Actions = 13,
    };
    struct ITargetedContentAction;
    struct ITargetedContentAvailabilityChangedEventArgs;
    struct ITargetedContentChangedEventArgs;
    struct ITargetedContentCollection;
    struct ITargetedContentContainer;
    struct ITargetedContentContainerStatics;
    struct ITargetedContentImage;
    struct ITargetedContentItem;
    struct ITargetedContentItemState;
    struct ITargetedContentObject;
    struct ITargetedContentStateChangedEventArgs;
    struct ITargetedContentSubscription;
    struct ITargetedContentSubscriptionOptions;
    struct ITargetedContentSubscriptionStatics;
    struct ITargetedContentValue;
    struct TargetedContentAction;
    struct TargetedContentAvailabilityChangedEventArgs;
    struct TargetedContentChangedEventArgs;
    struct TargetedContentCollection;
    struct TargetedContentContainer;
    struct TargetedContentFile;
    struct TargetedContentImage;
    struct TargetedContentItem;
    struct TargetedContentItemState;
    struct TargetedContentObject;
    struct TargetedContentStateChangedEventArgs;
    struct TargetedContentSubscription;
    struct TargetedContentSubscriptionOptions;
    struct TargetedContentValue;
    struct TargetedContentContract;
}
namespace winrt::impl
{
    template <> struct category<winrt::Windows::Services::TargetedContent::ITargetedContentAction>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::ITargetedContentAvailabilityChangedEventArgs>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::ITargetedContentChangedEventArgs>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::ITargetedContentCollection>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::ITargetedContentContainer>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::ITargetedContentContainerStatics>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::ITargetedContentImage>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::ITargetedContentItem>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::ITargetedContentItemState>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::ITargetedContentObject>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::ITargetedContentStateChangedEventArgs>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::ITargetedContentSubscription>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::ITargetedContentSubscriptionOptions>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::ITargetedContentSubscriptionStatics>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::ITargetedContentValue>{ using type = interface_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::TargetedContentAction>{ using type = class_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::TargetedContentAvailabilityChangedEventArgs>{ using type = class_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::TargetedContentChangedEventArgs>{ using type = class_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::TargetedContentCollection>{ using type = class_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::TargetedContentContainer>{ using type = class_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::TargetedContentFile>{ using type = class_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::TargetedContentImage>{ using type = class_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::TargetedContentItem>{ using type = class_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::TargetedContentItemState>{ using type = class_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::TargetedContentObject>{ using type = class_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::TargetedContentStateChangedEventArgs>{ using type = class_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::TargetedContentSubscription>{ using type = class_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::TargetedContentSubscriptionOptions>{ using type = class_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::TargetedContentValue>{ using type = class_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::TargetedContentAppInstallationState>{ using type = enum_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::TargetedContentAvailability>{ using type = enum_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::TargetedContentInteraction>{ using type = enum_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::TargetedContentObjectKind>{ using type = enum_category; };
    template <> struct category<winrt::Windows::Services::TargetedContent::TargetedContentValueKind>{ using type = enum_category; };
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::TargetedContentAction> = L"Windows.Services.TargetedContent.TargetedContentAction";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::TargetedContentAvailabilityChangedEventArgs> = L"Windows.Services.TargetedContent.TargetedContentAvailabilityChangedEventArgs";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::TargetedContentChangedEventArgs> = L"Windows.Services.TargetedContent.TargetedContentChangedEventArgs";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::TargetedContentCollection> = L"Windows.Services.TargetedContent.TargetedContentCollection";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::TargetedContentContainer> = L"Windows.Services.TargetedContent.TargetedContentContainer";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::TargetedContentFile> = L"Windows.Services.TargetedContent.TargetedContentFile";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::TargetedContentImage> = L"Windows.Services.TargetedContent.TargetedContentImage";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::TargetedContentItem> = L"Windows.Services.TargetedContent.TargetedContentItem";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::TargetedContentItemState> = L"Windows.Services.TargetedContent.TargetedContentItemState";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::TargetedContentObject> = L"Windows.Services.TargetedContent.TargetedContentObject";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::TargetedContentStateChangedEventArgs> = L"Windows.Services.TargetedContent.TargetedContentStateChangedEventArgs";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::TargetedContentSubscription> = L"Windows.Services.TargetedContent.TargetedContentSubscription";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::TargetedContentSubscriptionOptions> = L"Windows.Services.TargetedContent.TargetedContentSubscriptionOptions";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::TargetedContentValue> = L"Windows.Services.TargetedContent.TargetedContentValue";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::TargetedContentAppInstallationState> = L"Windows.Services.TargetedContent.TargetedContentAppInstallationState";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::TargetedContentAvailability> = L"Windows.Services.TargetedContent.TargetedContentAvailability";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::TargetedContentInteraction> = L"Windows.Services.TargetedContent.TargetedContentInteraction";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::TargetedContentObjectKind> = L"Windows.Services.TargetedContent.TargetedContentObjectKind";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::TargetedContentValueKind> = L"Windows.Services.TargetedContent.TargetedContentValueKind";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::ITargetedContentAction> = L"Windows.Services.TargetedContent.ITargetedContentAction";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::ITargetedContentAvailabilityChangedEventArgs> = L"Windows.Services.TargetedContent.ITargetedContentAvailabilityChangedEventArgs";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::ITargetedContentChangedEventArgs> = L"Windows.Services.TargetedContent.ITargetedContentChangedEventArgs";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::ITargetedContentCollection> = L"Windows.Services.TargetedContent.ITargetedContentCollection";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::ITargetedContentContainer> = L"Windows.Services.TargetedContent.ITargetedContentContainer";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::ITargetedContentContainerStatics> = L"Windows.Services.TargetedContent.ITargetedContentContainerStatics";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::ITargetedContentImage> = L"Windows.Services.TargetedContent.ITargetedContentImage";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::ITargetedContentItem> = L"Windows.Services.TargetedContent.ITargetedContentItem";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::ITargetedContentItemState> = L"Windows.Services.TargetedContent.ITargetedContentItemState";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::ITargetedContentObject> = L"Windows.Services.TargetedContent.ITargetedContentObject";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::ITargetedContentStateChangedEventArgs> = L"Windows.Services.TargetedContent.ITargetedContentStateChangedEventArgs";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::ITargetedContentSubscription> = L"Windows.Services.TargetedContent.ITargetedContentSubscription";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::ITargetedContentSubscriptionOptions> = L"Windows.Services.TargetedContent.ITargetedContentSubscriptionOptions";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::ITargetedContentSubscriptionStatics> = L"Windows.Services.TargetedContent.ITargetedContentSubscriptionStatics";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::ITargetedContentValue> = L"Windows.Services.TargetedContent.ITargetedContentValue";
    template <> inline constexpr auto& name_v<winrt::Windows::Services::TargetedContent::TargetedContentContract> = L"Windows.Services.TargetedContent.TargetedContentContract";
    template <> inline constexpr guid guid_v<winrt::Windows::Services::TargetedContent::ITargetedContentAction>{ 0xD75B691E,0x6CD6,0x4CA0,{ 0x9D,0x8F,0x47,0x28,0xB0,0xB7,0xE6,0xB6 } }; // D75B691E-6CD6-4CA0-9D8F-4728B0B7E6B6
    template <> inline constexpr guid guid_v<winrt::Windows::Services::TargetedContent::ITargetedContentAvailabilityChangedEventArgs>{ 0xE0F59D26,0x5927,0x4450,{ 0x96,0x5C,0x1C,0xEB,0x7B,0xEC,0xDE,0x65 } }; // E0F59D26-5927-4450-965C-1CEB7BECDE65
    template <> inline constexpr guid guid_v<winrt::Windows::Services::TargetedContent::ITargetedContentChangedEventArgs>{ 0x99D488C9,0x587E,0x4586,{ 0x8E,0xF7,0xB5,0x4C,0xA9,0x45,0x3A,0x16 } }; // 99D488C9-587E-4586-8EF7-B54CA9453A16
    template <> inline constexpr guid guid_v<winrt::Windows::Services::TargetedContent::ITargetedContentCollection>{ 0x2D4B66C5,0xF163,0x44BA,{ 0x9F,0x6E,0xE1,0xA4,0xC2,0xBB,0x55,0x9D } }; // 2D4B66C5-F163-44BA-9F6E-E1A4C2BB559D
    template <> inline constexpr guid guid_v<winrt::Windows::Services::TargetedContent::ITargetedContentContainer>{ 0xBC2494C9,0x8837,0x47C2,{ 0x85,0x0F,0xD7,0x9D,0x64,0x59,0x59,0x26 } }; // BC2494C9-8837-47C2-850F-D79D64595926
    template <> inline constexpr guid guid_v<winrt::Windows::Services::TargetedContent::ITargetedContentContainerStatics>{ 0x5B47E7FB,0x2140,0x4C1F,{ 0xA7,0x36,0xC5,0x95,0x83,0xF2,0x27,0xD8 } }; // 5B47E7FB-2140-4C1F-A736-C59583F227D8
    template <> inline constexpr guid guid_v<winrt::Windows::Services::TargetedContent::ITargetedContentImage>{ 0xA7A585D9,0x779F,0x4B1E,{ 0xBB,0xB1,0x8E,0xAF,0x53,0xFB,0xEA,0xB2 } }; // A7A585D9-779F-4B1E-BBB1-8EAF53FBEAB2
    template <> inline constexpr guid guid_v<winrt::Windows::Services::TargetedContent::ITargetedContentItem>{ 0x38168DC4,0x276C,0x4C32,{ 0x96,0xBA,0x56,0x5C,0x6E,0x40,0x6E,0x74 } }; // 38168DC4-276C-4C32-96BA-565C6E406E74
    template <> inline constexpr guid guid_v<winrt::Windows::Services::TargetedContent::ITargetedContentItemState>{ 0x73935454,0x4C65,0x4B47,{ 0xA4,0x41,0x47,0x2D,0xE5,0x3C,0x79,0xB6 } }; // 73935454-4C65-4B47-A441-472DE53C79B6
    template <> inline constexpr guid guid_v<winrt::Windows::Services::TargetedContent::ITargetedContentObject>{ 0x041D7969,0x2212,0x42D1,{ 0x9D,0xFA,0x88,0xA8,0xE3,0x03,0x3A,0xA3 } }; // 041D7969-2212-42D1-9DFA-88A8E3033AA3
    template <> inline constexpr guid guid_v<winrt::Windows::Services::TargetedContent::ITargetedContentStateChangedEventArgs>{ 0x9A1CEF3D,0x8073,0x4416,{ 0x8D,0xF2,0x54,0x68,0x35,0xA6,0x41,0x4F } }; // 9A1CEF3D-8073-4416-8DF2-546835A6414F
    template <> inline constexpr guid guid_v<winrt::Windows::Services::TargetedContent::ITargetedContentSubscription>{ 0x882C2C49,0xC652,0x4C7A,{ 0xAC,0xAD,0x1F,0x7F,0xA2,0x98,0x6C,0x73 } }; // 882C2C49-C652-4C7A-ACAD-1F7FA2986C73
    template <> inline constexpr guid guid_v<winrt::Windows::Services::TargetedContent::ITargetedContentSubscriptionOptions>{ 0x61EE6AD0,0x2C83,0x421B,{ 0x84,0x67,0x41,0x3E,0xAF,0x1A,0xEB,0x97 } }; // 61EE6AD0-2C83-421B-8467-413EAF1AEB97
    template <> inline constexpr guid guid_v<winrt::Windows::Services::TargetedContent::ITargetedContentSubscriptionStatics>{ 0xFADDFE80,0x360D,0x4916,{ 0xB5,0x3C,0x7E,0xA2,0x70,0x90,0xD0,0x2A } }; // FADDFE80-360D-4916-B53C-7EA27090D02A
    template <> inline constexpr guid guid_v<winrt::Windows::Services::TargetedContent::ITargetedContentValue>{ 0xAAFDE4B3,0x4215,0x4BF8,{ 0x86,0x7F,0x43,0xF0,0x48,0x65,0xF9,0xBF } }; // AAFDE4B3-4215-4BF8-867F-43F04865F9BF
    template <> struct default_interface<winrt::Windows::Services::TargetedContent::TargetedContentAction>{ using type = winrt::Windows::Services::TargetedContent::ITargetedContentAction; };
    template <> struct default_interface<winrt::Windows::Services::TargetedContent::TargetedContentAvailabilityChangedEventArgs>{ using type = winrt::Windows::Services::TargetedContent::ITargetedContentAvailabilityChangedEventArgs; };
    template <> struct default_interface<winrt::Windows::Services::TargetedContent::TargetedContentChangedEventArgs>{ using type = winrt::Windows::Services::TargetedContent::ITargetedContentChangedEventArgs; };
    template <> struct default_interface<winrt::Windows::Services::TargetedContent::TargetedContentCollection>{ using type = winrt::Windows::Services::TargetedContent::ITargetedContentCollection; };
    template <> struct default_interface<winrt::Windows::Services::TargetedContent::TargetedContentContainer>{ using type = winrt::Windows::Services::TargetedContent::ITargetedContentContainer; };
    template <> struct default_interface<winrt::Windows::Services::TargetedContent::TargetedContentFile>{ using type = winrt::Windows::Storage::Streams::IRandomAccessStreamReference; };
    template <> struct default_interface<winrt::Windows::Services::TargetedContent::TargetedContentImage>{ using type = winrt::Windows::Services::TargetedContent::ITargetedContentImage; };
    template <> struct default_interface<winrt::Windows::Services::TargetedContent::TargetedContentItem>{ using type = winrt::Windows::Services::TargetedContent::ITargetedContentItem; };
    template <> struct default_interface<winrt::Windows::Services::TargetedContent::TargetedContentItemState>{ using type = winrt::Windows::Services::TargetedContent::ITargetedContentItemState; };
    template <> struct default_interface<winrt::Windows::Services::TargetedContent::TargetedContentObject>{ using type = winrt::Windows::Services::TargetedContent::ITargetedContentObject; };
    template <> struct default_interface<winrt::Windows::Services::TargetedContent::TargetedContentStateChangedEventArgs>{ using type = winrt::Windows::Services::TargetedContent::ITargetedContentStateChangedEventArgs; };
    template <> struct default_interface<winrt::Windows::Services::TargetedContent::TargetedContentSubscription>{ using type = winrt::Windows::Services::TargetedContent::ITargetedContentSubscription; };
    template <> struct default_interface<winrt::Windows::Services::TargetedContent::TargetedContentSubscriptionOptions>{ using type = winrt::Windows::Services::TargetedContent::ITargetedContentSubscriptionOptions; };
    template <> struct default_interface<winrt::Windows::Services::TargetedContent::TargetedContentValue>{ using type = winrt::Windows::Services::TargetedContent::ITargetedContentValue; };
    template <> struct abi<winrt::Windows::Services::TargetedContent::ITargetedContentAction>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall InvokeAsync(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Services::TargetedContent::ITargetedContentAvailabilityChangedEventArgs>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall GetDeferral(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Services::TargetedContent::ITargetedContentChangedEventArgs>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall GetDeferral(void**) noexcept = 0;
            virtual int32_t __stdcall get_HasPreviousContentExpired(bool*) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Services::TargetedContent::ITargetedContentCollection>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_Id(void**) noexcept = 0;
            virtual int32_t __stdcall ReportInteraction(int32_t) noexcept = 0;
            virtual int32_t __stdcall ReportCustomInteraction(void*) noexcept = 0;
            virtual int32_t __stdcall get_Path(void**) noexcept = 0;
            virtual int32_t __stdcall get_Properties(void**) noexcept = 0;
            virtual int32_t __stdcall get_Collections(void**) noexcept = 0;
            virtual int32_t __stdcall get_Items(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Services::TargetedContent::ITargetedContentContainer>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_Id(void**) noexcept = 0;
            virtual int32_t __stdcall get_Timestamp(int64_t*) noexcept = 0;
            virtual int32_t __stdcall get_Availability(int32_t*) noexcept = 0;
            virtual int32_t __stdcall get_Content(void**) noexcept = 0;
            virtual int32_t __stdcall SelectSingleObject(void*, void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Services::TargetedContent::ITargetedContentContainerStatics>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall GetAsync(void*, void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Services::TargetedContent::ITargetedContentImage>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_Height(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_Width(uint32_t*) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Services::TargetedContent::ITargetedContentItem>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_Path(void**) noexcept = 0;
            virtual int32_t __stdcall ReportInteraction(int32_t) noexcept = 0;
            virtual int32_t __stdcall ReportCustomInteraction(void*) noexcept = 0;
            virtual int32_t __stdcall get_State(void**) noexcept = 0;
            virtual int32_t __stdcall get_Properties(void**) noexcept = 0;
            virtual int32_t __stdcall get_Collections(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Services::TargetedContent::ITargetedContentItemState>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_ShouldDisplay(bool*) noexcept = 0;
            virtual int32_t __stdcall get_AppInstallationState(int32_t*) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Services::TargetedContent::ITargetedContentObject>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_ObjectKind(int32_t*) noexcept = 0;
            virtual int32_t __stdcall get_Collection(void**) noexcept = 0;
            virtual int32_t __stdcall get_Item(void**) noexcept = 0;
            virtual int32_t __stdcall get_Value(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Services::TargetedContent::ITargetedContentStateChangedEventArgs>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall GetDeferral(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Services::TargetedContent::ITargetedContentSubscription>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_Id(void**) noexcept = 0;
            virtual int32_t __stdcall GetContentContainerAsync(void**) noexcept = 0;
            virtual int32_t __stdcall add_ContentChanged(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_ContentChanged(winrt::event_token) noexcept = 0;
            virtual int32_t __stdcall add_AvailabilityChanged(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_AvailabilityChanged(winrt::event_token) noexcept = 0;
            virtual int32_t __stdcall add_StateChanged(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_StateChanged(winrt::event_token) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Services::TargetedContent::ITargetedContentSubscriptionOptions>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_SubscriptionId(void**) noexcept = 0;
            virtual int32_t __stdcall get_AllowPartialContentAvailability(bool*) noexcept = 0;
            virtual int32_t __stdcall put_AllowPartialContentAvailability(bool) noexcept = 0;
            virtual int32_t __stdcall get_CloudQueryParameters(void**) noexcept = 0;
            virtual int32_t __stdcall get_LocalFilters(void**) noexcept = 0;
            virtual int32_t __stdcall Update() noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Services::TargetedContent::ITargetedContentSubscriptionStatics>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall GetAsync(void*, void**) noexcept = 0;
            virtual int32_t __stdcall GetOptions(void*, void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::Services::TargetedContent::ITargetedContentValue>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_ValueKind(int32_t*) noexcept = 0;
            virtual int32_t __stdcall get_Path(void**) noexcept = 0;
            virtual int32_t __stdcall get_String(void**) noexcept = 0;
            virtual int32_t __stdcall get_Uri(void**) noexcept = 0;
            virtual int32_t __stdcall get_Number(double*) noexcept = 0;
            virtual int32_t __stdcall get_Boolean(bool*) noexcept = 0;
            virtual int32_t __stdcall get_File(void**) noexcept = 0;
            virtual int32_t __stdcall get_ImageFile(void**) noexcept = 0;
            virtual int32_t __stdcall get_Action(void**) noexcept = 0;
            virtual int32_t __stdcall get_Strings(void**) noexcept = 0;
            virtual int32_t __stdcall get_Uris(void**) noexcept = 0;
            virtual int32_t __stdcall get_Numbers(void**) noexcept = 0;
            virtual int32_t __stdcall get_Booleans(void**) noexcept = 0;
            virtual int32_t __stdcall get_Files(void**) noexcept = 0;
            virtual int32_t __stdcall get_ImageFiles(void**) noexcept = 0;
            virtual int32_t __stdcall get_Actions(void**) noexcept = 0;
        };
    };
    template <typename D>
    struct consume_Windows_Services_TargetedContent_ITargetedContentAction
    {
        auto InvokeAsync() const;
    };
    template <> struct consume<winrt::Windows::Services::TargetedContent::ITargetedContentAction>
    {
        template <typename D> using type = consume_Windows_Services_TargetedContent_ITargetedContentAction<D>;
    };
    template <typename D>
    struct consume_Windows_Services_TargetedContent_ITargetedContentAvailabilityChangedEventArgs
    {
        auto GetDeferral() const;
    };
    template <> struct consume<winrt::Windows::Services::TargetedContent::ITargetedContentAvailabilityChangedEventArgs>
    {
        template <typename D> using type = consume_Windows_Services_TargetedContent_ITargetedContentAvailabilityChangedEventArgs<D>;
    };
    template <typename D>
    struct consume_Windows_Services_TargetedContent_ITargetedContentChangedEventArgs
    {
        auto GetDeferral() const;
        [[nodiscard]] auto HasPreviousContentExpired() const;
    };
    template <> struct consume<winrt::Windows::Services::TargetedContent::ITargetedContentChangedEventArgs>
    {
        template <typename D> using type = consume_Windows_Services_TargetedContent_ITargetedContentChangedEventArgs<D>;
    };
    template <typename D>
    struct consume_Windows_Services_TargetedContent_ITargetedContentCollection
    {
        [[nodiscard]] auto Id() const;
        auto ReportInteraction(winrt::Windows::Services::TargetedContent::TargetedContentInteraction const& interaction) const;
        auto ReportCustomInteraction(param::hstring const& customInteractionName) const;
        [[nodiscard]] auto Path() const;
        [[nodiscard]] auto Properties() const;
        [[nodiscard]] auto Collections() const;
        [[nodiscard]] auto Items() const;
    };
    template <> struct consume<winrt::Windows::Services::TargetedContent::ITargetedContentCollection>
    {
        template <typename D> using type = consume_Windows_Services_TargetedContent_ITargetedContentCollection<D>;
    };
    template <typename D>
    struct consume_Windows_Services_TargetedContent_ITargetedContentContainer
    {
        [[nodiscard]] auto Id() const;
        [[nodiscard]] auto Timestamp() const;
        [[nodiscard]] auto Availability() const;
        [[nodiscard]] auto Content() const;
        auto SelectSingleObject(param::hstring const& path) const;
    };
    template <> struct consume<winrt::Windows::Services::TargetedContent::ITargetedContentContainer>
    {
        template <typename D> using type = consume_Windows_Services_TargetedContent_ITargetedContentContainer<D>;
    };
    template <typename D>
    struct consume_Windows_Services_TargetedContent_ITargetedContentContainerStatics
    {
        auto GetAsync(param::hstring const& contentId) const;
    };
    template <> struct consume<winrt::Windows::Services::TargetedContent::ITargetedContentContainerStatics>
    {
        template <typename D> using type = consume_Windows_Services_TargetedContent_ITargetedContentContainerStatics<D>;
    };
    template <typename D>
    struct consume_Windows_Services_TargetedContent_ITargetedContentImage
    {
        [[nodiscard]] auto Height() const;
        [[nodiscard]] auto Width() const;
    };
    template <> struct consume<winrt::Windows::Services::TargetedContent::ITargetedContentImage>
    {
        template <typename D> using type = consume_Windows_Services_TargetedContent_ITargetedContentImage<D>;
    };
    template <typename D>
    struct consume_Windows_Services_TargetedContent_ITargetedContentItem
    {
        [[nodiscard]] auto Path() const;
        auto ReportInteraction(winrt::Windows::Services::TargetedContent::TargetedContentInteraction const& interaction) const;
        auto ReportCustomInteraction(param::hstring const& customInteractionName) const;
        [[nodiscard]] auto State() const;
        [[nodiscard]] auto Properties() const;
        [[nodiscard]] auto Collections() const;
    };
    template <> struct consume<winrt::Windows::Services::TargetedContent::ITargetedContentItem>
    {
        template <typename D> using type = consume_Windows_Services_TargetedContent_ITargetedContentItem<D>;
    };
    template <typename D>
    struct consume_Windows_Services_TargetedContent_ITargetedContentItemState
    {
        [[nodiscard]] auto ShouldDisplay() const;
        [[nodiscard]] auto AppInstallationState() const;
    };
    template <> struct consume<winrt::Windows::Services::TargetedContent::ITargetedContentItemState>
    {
        template <typename D> using type = consume_Windows_Services_TargetedContent_ITargetedContentItemState<D>;
    };
    template <typename D>
    struct consume_Windows_Services_TargetedContent_ITargetedContentObject
    {
        [[nodiscard]] auto ObjectKind() const;
        [[nodiscard]] auto Collection() const;
        [[nodiscard]] auto Item() const;
        [[nodiscard]] auto Value() const;
    };
    template <> struct consume<winrt::Windows::Services::TargetedContent::ITargetedContentObject>
    {
        template <typename D> using type = consume_Windows_Services_TargetedContent_ITargetedContentObject<D>;
    };
    template <typename D>
    struct consume_Windows_Services_TargetedContent_ITargetedContentStateChangedEventArgs
    {
        auto GetDeferral() const;
    };
    template <> struct consume<winrt::Windows::Services::TargetedContent::ITargetedContentStateChangedEventArgs>
    {
        template <typename D> using type = consume_Windows_Services_TargetedContent_ITargetedContentStateChangedEventArgs<D>;
    };
    template <typename D>
    struct consume_Windows_Services_TargetedContent_ITargetedContentSubscription
    {
        [[nodiscard]] auto Id() const;
        auto GetContentContainerAsync() const;
        auto ContentChanged(winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Services::TargetedContent::TargetedContentSubscription, winrt::Windows::Services::TargetedContent::TargetedContentChangedEventArgs> const& handler) const;
        using ContentChanged_revoker = impl::event_revoker<winrt::Windows::Services::TargetedContent::ITargetedContentSubscription, &impl::abi_t<winrt::Windows::Services::TargetedContent::ITargetedContentSubscription>::remove_ContentChanged>;
        [[nodiscard]] auto ContentChanged(auto_revoke_t, winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Services::TargetedContent::TargetedContentSubscription, winrt::Windows::Services::TargetedContent::TargetedContentChangedEventArgs> const& handler) const;
        auto ContentChanged(winrt::event_token const& cookie) const noexcept;
        auto AvailabilityChanged(winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Services::TargetedContent::TargetedContentSubscription, winrt::Windows::Services::TargetedContent::TargetedContentAvailabilityChangedEventArgs> const& handler) const;
        using AvailabilityChanged_revoker = impl::event_revoker<winrt::Windows::Services::TargetedContent::ITargetedContentSubscription, &impl::abi_t<winrt::Windows::Services::TargetedContent::ITargetedContentSubscription>::remove_AvailabilityChanged>;
        [[nodiscard]] auto AvailabilityChanged(auto_revoke_t, winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Services::TargetedContent::TargetedContentSubscription, winrt::Windows::Services::TargetedContent::TargetedContentAvailabilityChangedEventArgs> const& handler) const;
        auto AvailabilityChanged(winrt::event_token const& cookie) const noexcept;
        auto StateChanged(winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Services::TargetedContent::TargetedContentSubscription, winrt::Windows::Services::TargetedContent::TargetedContentStateChangedEventArgs> const& handler) const;
        using StateChanged_revoker = impl::event_revoker<winrt::Windows::Services::TargetedContent::ITargetedContentSubscription, &impl::abi_t<winrt::Windows::Services::TargetedContent::ITargetedContentSubscription>::remove_StateChanged>;
        [[nodiscard]] auto StateChanged(auto_revoke_t, winrt::Windows::Foundation::TypedEventHandler<winrt::Windows::Services::TargetedContent::TargetedContentSubscription, winrt::Windows::Services::TargetedContent::TargetedContentStateChangedEventArgs> const& handler) const;
        auto StateChanged(winrt::event_token const& cookie) const noexcept;
    };
    template <> struct consume<winrt::Windows::Services::TargetedContent::ITargetedContentSubscription>
    {
        template <typename D> using type = consume_Windows_Services_TargetedContent_ITargetedContentSubscription<D>;
    };
    template <typename D>
    struct consume_Windows_Services_TargetedContent_ITargetedContentSubscriptionOptions
    {
        [[nodiscard]] auto SubscriptionId() const;
        [[nodiscard]] auto AllowPartialContentAvailability() const;
        auto AllowPartialContentAvailability(bool value) const;
        [[nodiscard]] auto CloudQueryParameters() const;
        [[nodiscard]] auto LocalFilters() const;
        auto Update() const;
    };
    template <> struct consume<winrt::Windows::Services::TargetedContent::ITargetedContentSubscriptionOptions>
    {
        template <typename D> using type = consume_Windows_Services_TargetedContent_ITargetedContentSubscriptionOptions<D>;
    };
    template <typename D>
    struct consume_Windows_Services_TargetedContent_ITargetedContentSubscriptionStatics
    {
        auto GetAsync(param::hstring const& subscriptionId) const;
        auto GetOptions(param::hstring const& subscriptionId) const;
    };
    template <> struct consume<winrt::Windows::Services::TargetedContent::ITargetedContentSubscriptionStatics>
    {
        template <typename D> using type = consume_Windows_Services_TargetedContent_ITargetedContentSubscriptionStatics<D>;
    };
    template <typename D>
    struct consume_Windows_Services_TargetedContent_ITargetedContentValue
    {
        [[nodiscard]] auto ValueKind() const;
        [[nodiscard]] auto Path() const;
        [[nodiscard]] auto String() const;
        [[nodiscard]] auto Uri() const;
        [[nodiscard]] auto Number() const;
        [[nodiscard]] auto Boolean() const;
        [[nodiscard]] auto File() const;
        [[nodiscard]] auto ImageFile() const;
        [[nodiscard]] auto Action() const;
        [[nodiscard]] auto Strings() const;
        [[nodiscard]] auto Uris() const;
        [[nodiscard]] auto Numbers() const;
        [[nodiscard]] auto Booleans() const;
        [[nodiscard]] auto Files() const;
        [[nodiscard]] auto ImageFiles() const;
        [[nodiscard]] auto Actions() const;
    };
    template <> struct consume<winrt::Windows::Services::TargetedContent::ITargetedContentValue>
    {
        template <typename D> using type = consume_Windows_Services_TargetedContent_ITargetedContentValue<D>;
    };
}
#endif
