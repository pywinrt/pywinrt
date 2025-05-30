// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Microsoft_UI_Dispatching_0_H
#define WINRT_Microsoft_UI_Dispatching_0_H
WINRT_EXPORT namespace winrt::Windows::Foundation
{
    struct Deferral;
    struct EventRegistrationToken;
    struct IAsyncAction;
    template <typename TSender, typename TResult> struct WINRT_IMPL_EMPTY_BASES TypedEventHandler;
}
WINRT_EXPORT namespace winrt::Microsoft::UI::Dispatching
{
    enum class DispatcherQueuePriority : int32_t
    {
        Low = -10,
        Normal = 0,
        High = 10,
    };
    enum class DispatcherRunOptions : uint32_t
    {
        None = 0,
        ContinueOnQuit = 0x1,
        QuitOnlyLocalLoop = 0x2,
    };
    struct IDispatcherExitDeferral;
    struct IDispatcherQueue;
    struct IDispatcherQueue2;
    struct IDispatcherQueue3;
    struct IDispatcherQueueController;
    struct IDispatcherQueueController2;
    struct IDispatcherQueueControllerStatics;
    struct IDispatcherQueueShutdownStartingEventArgs;
    struct IDispatcherQueueStatics;
    struct IDispatcherQueueTimer;
    struct DispatcherExitDeferral;
    struct DispatcherQueue;
    struct DispatcherQueueController;
    struct DispatcherQueueShutdownStartingEventArgs;
    struct DispatcherQueueTimer;
    struct DispatcherQueueHandler;
}
namespace winrt::impl
{
    template <> struct category<winrt::Microsoft::UI::Dispatching::IDispatcherExitDeferral>{ using type = interface_category; };
    template <> struct category<winrt::Microsoft::UI::Dispatching::IDispatcherQueue>{ using type = interface_category; };
    template <> struct category<winrt::Microsoft::UI::Dispatching::IDispatcherQueue2>{ using type = interface_category; };
    template <> struct category<winrt::Microsoft::UI::Dispatching::IDispatcherQueue3>{ using type = interface_category; };
    template <> struct category<winrt::Microsoft::UI::Dispatching::IDispatcherQueueController>{ using type = interface_category; };
    template <> struct category<winrt::Microsoft::UI::Dispatching::IDispatcherQueueController2>{ using type = interface_category; };
    template <> struct category<winrt::Microsoft::UI::Dispatching::IDispatcherQueueControllerStatics>{ using type = interface_category; };
    template <> struct category<winrt::Microsoft::UI::Dispatching::IDispatcherQueueShutdownStartingEventArgs>{ using type = interface_category; };
    template <> struct category<winrt::Microsoft::UI::Dispatching::IDispatcherQueueStatics>{ using type = interface_category; };
    template <> struct category<winrt::Microsoft::UI::Dispatching::IDispatcherQueueTimer>{ using type = interface_category; };
    template <> struct category<winrt::Microsoft::UI::Dispatching::DispatcherExitDeferral>{ using type = class_category; };
    template <> struct category<winrt::Microsoft::UI::Dispatching::DispatcherQueue>{ using type = class_category; };
    template <> struct category<winrt::Microsoft::UI::Dispatching::DispatcherQueueController>{ using type = class_category; };
    template <> struct category<winrt::Microsoft::UI::Dispatching::DispatcherQueueShutdownStartingEventArgs>{ using type = class_category; };
    template <> struct category<winrt::Microsoft::UI::Dispatching::DispatcherQueueTimer>{ using type = class_category; };
    template <> struct category<winrt::Microsoft::UI::Dispatching::DispatcherQueuePriority>{ using type = enum_category; };
    template <> struct category<winrt::Microsoft::UI::Dispatching::DispatcherRunOptions>{ using type = enum_category; };
    template <> struct category<winrt::Microsoft::UI::Dispatching::DispatcherQueueHandler>{ using type = delegate_category; };
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::Dispatching::DispatcherExitDeferral> = L"Microsoft.UI.Dispatching.DispatcherExitDeferral";
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::Dispatching::DispatcherQueue> = L"Microsoft.UI.Dispatching.DispatcherQueue";
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::Dispatching::DispatcherQueueController> = L"Microsoft.UI.Dispatching.DispatcherQueueController";
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::Dispatching::DispatcherQueueShutdownStartingEventArgs> = L"Microsoft.UI.Dispatching.DispatcherQueueShutdownStartingEventArgs";
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::Dispatching::DispatcherQueueTimer> = L"Microsoft.UI.Dispatching.DispatcherQueueTimer";
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::Dispatching::DispatcherQueuePriority> = L"Microsoft.UI.Dispatching.DispatcherQueuePriority";
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::Dispatching::DispatcherRunOptions> = L"Microsoft.UI.Dispatching.DispatcherRunOptions";
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::Dispatching::IDispatcherExitDeferral> = L"Microsoft.UI.Dispatching.IDispatcherExitDeferral";
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::Dispatching::IDispatcherQueue> = L"Microsoft.UI.Dispatching.IDispatcherQueue";
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::Dispatching::IDispatcherQueue2> = L"Microsoft.UI.Dispatching.IDispatcherQueue2";
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::Dispatching::IDispatcherQueue3> = L"Microsoft.UI.Dispatching.IDispatcherQueue3";
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::Dispatching::IDispatcherQueueController> = L"Microsoft.UI.Dispatching.IDispatcherQueueController";
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::Dispatching::IDispatcherQueueController2> = L"Microsoft.UI.Dispatching.IDispatcherQueueController2";
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::Dispatching::IDispatcherQueueControllerStatics> = L"Microsoft.UI.Dispatching.IDispatcherQueueControllerStatics";
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::Dispatching::IDispatcherQueueShutdownStartingEventArgs> = L"Microsoft.UI.Dispatching.IDispatcherQueueShutdownStartingEventArgs";
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::Dispatching::IDispatcherQueueStatics> = L"Microsoft.UI.Dispatching.IDispatcherQueueStatics";
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::Dispatching::IDispatcherQueueTimer> = L"Microsoft.UI.Dispatching.IDispatcherQueueTimer";
    template <> inline constexpr auto& name_v<winrt::Microsoft::UI::Dispatching::DispatcherQueueHandler> = L"Microsoft.UI.Dispatching.DispatcherQueueHandler";
    template <> inline constexpr guid guid_v<winrt::Microsoft::UI::Dispatching::IDispatcherExitDeferral>{ 0x910B5AAC,0x3310,0x563E,{ 0x84,0x18,0xF3,0x00,0x55,0x79,0x72,0x9E } }; // 910B5AAC-3310-563E-8418-F3005579729E
    template <> inline constexpr guid guid_v<winrt::Microsoft::UI::Dispatching::IDispatcherQueue>{ 0xF6EBF8FA,0xBE1C,0x5BF6,{ 0xA4,0x67,0x73,0xDA,0x28,0x73,0x8A,0xE8 } }; // F6EBF8FA-BE1C-5BF6-A467-73DA28738AE8
    template <> inline constexpr guid guid_v<winrt::Microsoft::UI::Dispatching::IDispatcherQueue2>{ 0x0CF48751,0xF1AC,0x59B8,{ 0xBA,0x52,0x6C,0xE7,0xA1,0x44,0x4D,0x6F } }; // 0CF48751-F1AC-59B8-BA52-6CE7A1444D6F
    template <> inline constexpr guid guid_v<winrt::Microsoft::UI::Dispatching::IDispatcherQueue3>{ 0x14A7A175,0x5C27,0x5A35,{ 0xB0,0x79,0x21,0x96,0x0C,0xF7,0x64,0xA8 } }; // 14A7A175-5C27-5A35-B079-21960CF764A8
    template <> inline constexpr guid guid_v<winrt::Microsoft::UI::Dispatching::IDispatcherQueueController>{ 0xBCE8178D,0x2183,0x584C,{ 0x9E,0x5B,0xF9,0x36,0x6F,0x6A,0xE4,0x84 } }; // BCE8178D-2183-584C-9E5B-F9366F6AE484
    template <> inline constexpr guid guid_v<winrt::Microsoft::UI::Dispatching::IDispatcherQueueController2>{ 0x4C68EE2A,0x1CB1,0x5591,{ 0xA3,0xA2,0x9B,0x59,0x0B,0x8F,0x8B,0x9A } }; // 4C68EE2A-1CB1-5591-A3A2-9B590B8F8B9A
    template <> inline constexpr guid guid_v<winrt::Microsoft::UI::Dispatching::IDispatcherQueueControllerStatics>{ 0xF18D6145,0x722B,0x593D,{ 0xBC,0xF2,0xA6,0x1E,0x71,0x3F,0x00,0x37 } }; // F18D6145-722B-593D-BCF2-A61E713F0037
    template <> inline constexpr guid guid_v<winrt::Microsoft::UI::Dispatching::IDispatcherQueueShutdownStartingEventArgs>{ 0x32519BE5,0x072B,0x5660,{ 0xA7,0x0E,0x88,0x35,0xC9,0xB8,0x15,0x7D } }; // 32519BE5-072B-5660-A70E-8835C9B8157D
    template <> inline constexpr guid guid_v<winrt::Microsoft::UI::Dispatching::IDispatcherQueueStatics>{ 0xCD3382EA,0xA455,0x5124,{ 0xB6,0x3A,0xCA,0x40,0xD3,0x4C,0xA2,0x3C } }; // CD3382EA-A455-5124-B63A-CA40D34CA23C
    template <> inline constexpr guid guid_v<winrt::Microsoft::UI::Dispatching::IDispatcherQueueTimer>{ 0xAD4D63FD,0x88FE,0x541F,{ 0xAC,0x11,0xBF,0x2D,0xC1,0xED,0x2C,0xE5 } }; // AD4D63FD-88FE-541F-AC11-BF2DC1ED2CE5
    template <> inline constexpr guid guid_v<winrt::Microsoft::UI::Dispatching::DispatcherQueueHandler>{ 0x2E0872A9,0x4E29,0x5F14,{ 0xB6,0x88,0xFB,0x96,0xD5,0xF9,0xD5,0xF8 } }; // 2E0872A9-4E29-5F14-B688-FB96D5F9D5F8
    template <> struct default_interface<winrt::Microsoft::UI::Dispatching::DispatcherExitDeferral>{ using type = winrt::Microsoft::UI::Dispatching::IDispatcherExitDeferral; };
    template <> struct default_interface<winrt::Microsoft::UI::Dispatching::DispatcherQueue>{ using type = winrt::Microsoft::UI::Dispatching::IDispatcherQueue; };
    template <> struct default_interface<winrt::Microsoft::UI::Dispatching::DispatcherQueueController>{ using type = winrt::Microsoft::UI::Dispatching::IDispatcherQueueController; };
    template <> struct default_interface<winrt::Microsoft::UI::Dispatching::DispatcherQueueShutdownStartingEventArgs>{ using type = winrt::Microsoft::UI::Dispatching::IDispatcherQueueShutdownStartingEventArgs; };
    template <> struct default_interface<winrt::Microsoft::UI::Dispatching::DispatcherQueueTimer>{ using type = winrt::Microsoft::UI::Dispatching::IDispatcherQueueTimer; };
    template <> struct abi<winrt::Microsoft::UI::Dispatching::IDispatcherExitDeferral>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall Complete() noexcept = 0;
        };
    };
    template <> struct abi<winrt::Microsoft::UI::Dispatching::IDispatcherQueue>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall CreateTimer(void**) noexcept = 0;
            virtual int32_t __stdcall TryEnqueue(void*, bool*) noexcept = 0;
            virtual int32_t __stdcall TryEnqueueWithPriority(int32_t, void*, bool*) noexcept = 0;
            virtual int32_t __stdcall add_ShutdownStarting(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_ShutdownStarting(winrt::event_token) noexcept = 0;
            virtual int32_t __stdcall add_ShutdownCompleted(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_ShutdownCompleted(winrt::event_token) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Microsoft::UI::Dispatching::IDispatcherQueue2>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_HasThreadAccess(bool*) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Microsoft::UI::Dispatching::IDispatcherQueue3>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall EnqueueEventLoopExit() noexcept = 0;
            virtual int32_t __stdcall EnsureSystemDispatcherQueue() noexcept = 0;
            virtual int32_t __stdcall RunEventLoop() noexcept = 0;
            virtual int32_t __stdcall RunEventLoopWithOptions(uint32_t, void*) noexcept = 0;
            virtual int32_t __stdcall add_FrameworkShutdownStarting(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_FrameworkShutdownStarting(winrt::event_token) noexcept = 0;
            virtual int32_t __stdcall add_FrameworkShutdownCompleted(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_FrameworkShutdownCompleted(winrt::event_token) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Microsoft::UI::Dispatching::IDispatcherQueueController>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_DispatcherQueue(void**) noexcept = 0;
            virtual int32_t __stdcall ShutdownQueueAsync(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Microsoft::UI::Dispatching::IDispatcherQueueController2>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall ShutdownQueue() noexcept = 0;
        };
    };
    template <> struct abi<winrt::Microsoft::UI::Dispatching::IDispatcherQueueControllerStatics>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall CreateOnDedicatedThread(void**) noexcept = 0;
            virtual int32_t __stdcall CreateOnCurrentThread(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Microsoft::UI::Dispatching::IDispatcherQueueShutdownStartingEventArgs>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall GetDeferral(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Microsoft::UI::Dispatching::IDispatcherQueueStatics>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall GetForCurrentThread(void**) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Microsoft::UI::Dispatching::IDispatcherQueueTimer>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_Interval(int64_t*) noexcept = 0;
            virtual int32_t __stdcall put_Interval(int64_t) noexcept = 0;
            virtual int32_t __stdcall get_IsRunning(bool*) noexcept = 0;
            virtual int32_t __stdcall get_IsRepeating(bool*) noexcept = 0;
            virtual int32_t __stdcall put_IsRepeating(bool) noexcept = 0;
            virtual int32_t __stdcall Start() noexcept = 0;
            virtual int32_t __stdcall Stop() noexcept = 0;
            virtual int32_t __stdcall add_Tick(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_Tick(winrt::event_token) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Microsoft::UI::Dispatching::DispatcherQueueHandler>
    {
        struct WINRT_IMPL_NOVTABLE type : unknown_abi
        {
            virtual int32_t __stdcall Invoke() noexcept = 0;
        };
    };
    template <typename D>
    struct consume_Microsoft_UI_Dispatching_IDispatcherExitDeferral
    {
        auto Complete() const;
    };
    template <> struct consume<winrt::Microsoft::UI::Dispatching::IDispatcherExitDeferral>
    {
        template <typename D> using type = consume_Microsoft_UI_Dispatching_IDispatcherExitDeferral<D>;
    };
    template <typename D>
    struct consume_Microsoft_UI_Dispatching_IDispatcherQueue
    {
        auto CreateTimer() const;
        auto TryEnqueue(winrt::Microsoft::UI::Dispatching::DispatcherQueueHandler const& callback) const;
        auto TryEnqueue(winrt::Microsoft::UI::Dispatching::DispatcherQueuePriority const& priority, winrt::Microsoft::UI::Dispatching::DispatcherQueueHandler const& callback) const;
        auto ShutdownStarting(winrt::Windows::Foundation::TypedEventHandler<winrt::Microsoft::UI::Dispatching::DispatcherQueue, winrt::Microsoft::UI::Dispatching::DispatcherQueueShutdownStartingEventArgs> const& handler) const;
        using ShutdownStarting_revoker = impl::event_revoker<winrt::Microsoft::UI::Dispatching::IDispatcherQueue, &impl::abi_t<winrt::Microsoft::UI::Dispatching::IDispatcherQueue>::remove_ShutdownStarting>;
        [[nodiscard]] auto ShutdownStarting(auto_revoke_t, winrt::Windows::Foundation::TypedEventHandler<winrt::Microsoft::UI::Dispatching::DispatcherQueue, winrt::Microsoft::UI::Dispatching::DispatcherQueueShutdownStartingEventArgs> const& handler) const;
        auto ShutdownStarting(winrt::event_token const& token) const noexcept;
        auto ShutdownCompleted(winrt::Windows::Foundation::TypedEventHandler<winrt::Microsoft::UI::Dispatching::DispatcherQueue, winrt::Windows::Foundation::IInspectable> const& handler) const;
        using ShutdownCompleted_revoker = impl::event_revoker<winrt::Microsoft::UI::Dispatching::IDispatcherQueue, &impl::abi_t<winrt::Microsoft::UI::Dispatching::IDispatcherQueue>::remove_ShutdownCompleted>;
        [[nodiscard]] auto ShutdownCompleted(auto_revoke_t, winrt::Windows::Foundation::TypedEventHandler<winrt::Microsoft::UI::Dispatching::DispatcherQueue, winrt::Windows::Foundation::IInspectable> const& handler) const;
        auto ShutdownCompleted(winrt::event_token const& token) const noexcept;
    };
    template <> struct consume<winrt::Microsoft::UI::Dispatching::IDispatcherQueue>
    {
        template <typename D> using type = consume_Microsoft_UI_Dispatching_IDispatcherQueue<D>;
    };
    template <typename D>
    struct consume_Microsoft_UI_Dispatching_IDispatcherQueue2
    {
        [[nodiscard]] auto HasThreadAccess() const;
    };
    template <> struct consume<winrt::Microsoft::UI::Dispatching::IDispatcherQueue2>
    {
        template <typename D> using type = consume_Microsoft_UI_Dispatching_IDispatcherQueue2<D>;
    };
    template <typename D>
    struct consume_Microsoft_UI_Dispatching_IDispatcherQueue3
    {
        auto EnqueueEventLoopExit() const;
        auto EnsureSystemDispatcherQueue() const;
        auto RunEventLoop() const;
        auto RunEventLoop(winrt::Microsoft::UI::Dispatching::DispatcherRunOptions const& options, winrt::Microsoft::UI::Dispatching::DispatcherExitDeferral const& deferral) const;
        auto FrameworkShutdownStarting(winrt::Windows::Foundation::TypedEventHandler<winrt::Microsoft::UI::Dispatching::DispatcherQueue, winrt::Microsoft::UI::Dispatching::DispatcherQueueShutdownStartingEventArgs> const& handler) const;
        using FrameworkShutdownStarting_revoker = impl::event_revoker<winrt::Microsoft::UI::Dispatching::IDispatcherQueue3, &impl::abi_t<winrt::Microsoft::UI::Dispatching::IDispatcherQueue3>::remove_FrameworkShutdownStarting>;
        [[nodiscard]] auto FrameworkShutdownStarting(auto_revoke_t, winrt::Windows::Foundation::TypedEventHandler<winrt::Microsoft::UI::Dispatching::DispatcherQueue, winrt::Microsoft::UI::Dispatching::DispatcherQueueShutdownStartingEventArgs> const& handler) const;
        auto FrameworkShutdownStarting(winrt::event_token const& token) const noexcept;
        auto FrameworkShutdownCompleted(winrt::Windows::Foundation::TypedEventHandler<winrt::Microsoft::UI::Dispatching::DispatcherQueue, winrt::Windows::Foundation::IInspectable> const& handler) const;
        using FrameworkShutdownCompleted_revoker = impl::event_revoker<winrt::Microsoft::UI::Dispatching::IDispatcherQueue3, &impl::abi_t<winrt::Microsoft::UI::Dispatching::IDispatcherQueue3>::remove_FrameworkShutdownCompleted>;
        [[nodiscard]] auto FrameworkShutdownCompleted(auto_revoke_t, winrt::Windows::Foundation::TypedEventHandler<winrt::Microsoft::UI::Dispatching::DispatcherQueue, winrt::Windows::Foundation::IInspectable> const& handler) const;
        auto FrameworkShutdownCompleted(winrt::event_token const& token) const noexcept;
    };
    template <> struct consume<winrt::Microsoft::UI::Dispatching::IDispatcherQueue3>
    {
        template <typename D> using type = consume_Microsoft_UI_Dispatching_IDispatcherQueue3<D>;
    };
    template <typename D>
    struct consume_Microsoft_UI_Dispatching_IDispatcherQueueController
    {
        [[nodiscard]] auto DispatcherQueue() const;
        auto ShutdownQueueAsync() const;
    };
    template <> struct consume<winrt::Microsoft::UI::Dispatching::IDispatcherQueueController>
    {
        template <typename D> using type = consume_Microsoft_UI_Dispatching_IDispatcherQueueController<D>;
    };
    template <typename D>
    struct consume_Microsoft_UI_Dispatching_IDispatcherQueueController2
    {
        auto ShutdownQueue() const;
    };
    template <> struct consume<winrt::Microsoft::UI::Dispatching::IDispatcherQueueController2>
    {
        template <typename D> using type = consume_Microsoft_UI_Dispatching_IDispatcherQueueController2<D>;
    };
    template <typename D>
    struct consume_Microsoft_UI_Dispatching_IDispatcherQueueControllerStatics
    {
        auto CreateOnDedicatedThread() const;
        auto CreateOnCurrentThread() const;
    };
    template <> struct consume<winrt::Microsoft::UI::Dispatching::IDispatcherQueueControllerStatics>
    {
        template <typename D> using type = consume_Microsoft_UI_Dispatching_IDispatcherQueueControllerStatics<D>;
    };
    template <typename D>
    struct consume_Microsoft_UI_Dispatching_IDispatcherQueueShutdownStartingEventArgs
    {
        auto GetDeferral() const;
    };
    template <> struct consume<winrt::Microsoft::UI::Dispatching::IDispatcherQueueShutdownStartingEventArgs>
    {
        template <typename D> using type = consume_Microsoft_UI_Dispatching_IDispatcherQueueShutdownStartingEventArgs<D>;
    };
    template <typename D>
    struct consume_Microsoft_UI_Dispatching_IDispatcherQueueStatics
    {
        auto GetForCurrentThread() const;
    };
    template <> struct consume<winrt::Microsoft::UI::Dispatching::IDispatcherQueueStatics>
    {
        template <typename D> using type = consume_Microsoft_UI_Dispatching_IDispatcherQueueStatics<D>;
    };
    template <typename D>
    struct consume_Microsoft_UI_Dispatching_IDispatcherQueueTimer
    {
        [[nodiscard]] auto Interval() const;
        auto Interval(winrt::Windows::Foundation::TimeSpan const& value) const;
        [[nodiscard]] auto IsRunning() const;
        [[nodiscard]] auto IsRepeating() const;
        auto IsRepeating(bool value) const;
        auto Start() const;
        auto Stop() const;
        auto Tick(winrt::Windows::Foundation::TypedEventHandler<winrt::Microsoft::UI::Dispatching::DispatcherQueueTimer, winrt::Windows::Foundation::IInspectable> const& handler) const;
        using Tick_revoker = impl::event_revoker<winrt::Microsoft::UI::Dispatching::IDispatcherQueueTimer, &impl::abi_t<winrt::Microsoft::UI::Dispatching::IDispatcherQueueTimer>::remove_Tick>;
        [[nodiscard]] auto Tick(auto_revoke_t, winrt::Windows::Foundation::TypedEventHandler<winrt::Microsoft::UI::Dispatching::DispatcherQueueTimer, winrt::Windows::Foundation::IInspectable> const& handler) const;
        auto Tick(winrt::event_token const& token) const noexcept;
    };
    template <> struct consume<winrt::Microsoft::UI::Dispatching::IDispatcherQueueTimer>
    {
        template <typename D> using type = consume_Microsoft_UI_Dispatching_IDispatcherQueueTimer<D>;
    };
}
#endif
