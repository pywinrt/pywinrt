// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Phone_System_Power_2_H
#define WINRT_Windows_Phone_System_Power_2_H
#include "winrt/impl/Windows.Foundation.1.h"
#include "winrt/impl/Windows.Phone.System.Power.1.h"
WINRT_EXPORT namespace winrt::Windows::Phone::System::Power
{
    struct PowerManager
    {
        PowerManager() = delete;
        [[nodiscard]] static auto PowerSavingMode();
        static auto PowerSavingModeChanged(winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& changeHandler);
        using PowerSavingModeChanged_revoker = impl::factory_event_revoker<winrt::Windows::Phone::System::Power::IPowerManagerStatics, &impl::abi_t<winrt::Windows::Phone::System::Power::IPowerManagerStatics>::remove_PowerSavingModeChanged>;
        [[nodiscard]] static auto PowerSavingModeChanged(auto_revoke_t, winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& changeHandler);
        static auto PowerSavingModeChanged(winrt::event_token const& token);
        [[nodiscard]] static auto PowerSavingModeEnabled();
    };
}
#endif
