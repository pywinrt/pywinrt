// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Microsoft_UI_Dispatching_2_H
#define WINRT_Microsoft_UI_Dispatching_2_H
#include "winrt/impl/Microsoft.UI.Dispatching.1.h"
WINRT_EXPORT namespace winrt::Microsoft::UI::Dispatching
{
    struct DispatcherQueueHandler : winrt::Windows::Foundation::IUnknown
    {
        DispatcherQueueHandler(std::nullptr_t = nullptr) noexcept {}
        DispatcherQueueHandler(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Foundation::IUnknown(ptr, take_ownership_from_abi) {}
        template <typename L> DispatcherQueueHandler(L lambda);
        template <typename F> DispatcherQueueHandler(F* function);
        template <typename O, typename M> DispatcherQueueHandler(O* object, M method);
        template <typename O, typename M> DispatcherQueueHandler(com_ptr<O>&& object, M method);
        template <typename O, typename LM> DispatcherQueueHandler(weak_ref<O>&& object, LM&& lambda_or_method);
        template <typename O, typename M> DispatcherQueueHandler(std::shared_ptr<O>&& object, M method);
        template <typename O, typename LM> DispatcherQueueHandler(std::weak_ptr<O>&& object, LM&& lambda_or_method);
        auto operator()() const;
    };
    struct WINRT_IMPL_EMPTY_BASES DispatcherExitDeferral : winrt::Microsoft::UI::Dispatching::IDispatcherExitDeferral
    {
        DispatcherExitDeferral(std::nullptr_t) noexcept {}
        DispatcherExitDeferral(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Dispatching::IDispatcherExitDeferral(ptr, take_ownership_from_abi) {}
        DispatcherExitDeferral();
    };
    struct WINRT_IMPL_EMPTY_BASES DispatcherQueue : winrt::Microsoft::UI::Dispatching::IDispatcherQueue,
        impl::require<DispatcherQueue, winrt::Microsoft::UI::Dispatching::IDispatcherQueue2, winrt::Microsoft::UI::Dispatching::IDispatcherQueue3>
    {
        DispatcherQueue(std::nullptr_t) noexcept {}
        DispatcherQueue(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Dispatching::IDispatcherQueue(ptr, take_ownership_from_abi) {}
        static auto GetForCurrentThread();
    };
    struct WINRT_IMPL_EMPTY_BASES DispatcherQueueController : winrt::Microsoft::UI::Dispatching::IDispatcherQueueController,
        impl::require<DispatcherQueueController, winrt::Microsoft::UI::Dispatching::IDispatcherQueueController2>
    {
        DispatcherQueueController(std::nullptr_t) noexcept {}
        DispatcherQueueController(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Dispatching::IDispatcherQueueController(ptr, take_ownership_from_abi) {}
        static auto CreateOnDedicatedThread();
        static auto CreateOnCurrentThread();
    };
    struct WINRT_IMPL_EMPTY_BASES DispatcherQueueShutdownStartingEventArgs : winrt::Microsoft::UI::Dispatching::IDispatcherQueueShutdownStartingEventArgs
    {
        DispatcherQueueShutdownStartingEventArgs(std::nullptr_t) noexcept {}
        DispatcherQueueShutdownStartingEventArgs(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Dispatching::IDispatcherQueueShutdownStartingEventArgs(ptr, take_ownership_from_abi) {}
    };
    struct WINRT_IMPL_EMPTY_BASES DispatcherQueueTimer : winrt::Microsoft::UI::Dispatching::IDispatcherQueueTimer
    {
        DispatcherQueueTimer(std::nullptr_t) noexcept {}
        DispatcherQueueTimer(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Microsoft::UI::Dispatching::IDispatcherQueueTimer(ptr, take_ownership_from_abi) {}
    };
}
#endif
