// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_System_Power_0_H
#define WINRT_Windows_System_Power_0_H
WINRT_EXPORT namespace winrt::Windows::Foundation
{
    template <typename T> struct WINRT_IMPL_EMPTY_BASES EventHandler;
    struct EventRegistrationToken;
}
WINRT_EXPORT namespace winrt::Windows::System::Power
{
    enum class BatteryStatus : int32_t
    {
        NotPresent = 0,
        Discharging = 1,
        Idle = 2,
        Charging = 3,
    };
    enum class EnergySaverStatus : int32_t
    {
        Disabled = 0,
        Off = 1,
        On = 2,
    };
    enum class PowerSupplyStatus : int32_t
    {
        NotPresent = 0,
        Inadequate = 1,
        Adequate = 2,
    };
    struct IBackgroundEnergyManagerStatics;
    struct IForegroundEnergyManagerStatics;
    struct IPowerManagerStatics;
    struct BackgroundEnergyManager;
    struct ForegroundEnergyManager;
    struct PowerManager;
}
namespace winrt::impl
{
    template <> struct category<winrt::Windows::System::Power::IBackgroundEnergyManagerStatics>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Power::IForegroundEnergyManagerStatics>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Power::IPowerManagerStatics>{ using type = interface_category; };
    template <> struct category<winrt::Windows::System::Power::BackgroundEnergyManager>{ using type = class_category; };
    template <> struct category<winrt::Windows::System::Power::ForegroundEnergyManager>{ using type = class_category; };
    template <> struct category<winrt::Windows::System::Power::PowerManager>{ using type = class_category; };
    template <> struct category<winrt::Windows::System::Power::BatteryStatus>{ using type = enum_category; };
    template <> struct category<winrt::Windows::System::Power::EnergySaverStatus>{ using type = enum_category; };
    template <> struct category<winrt::Windows::System::Power::PowerSupplyStatus>{ using type = enum_category; };
    template <> inline constexpr auto& name_v<winrt::Windows::System::Power::BackgroundEnergyManager> = L"Windows.System.Power.BackgroundEnergyManager";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Power::ForegroundEnergyManager> = L"Windows.System.Power.ForegroundEnergyManager";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Power::PowerManager> = L"Windows.System.Power.PowerManager";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Power::BatteryStatus> = L"Windows.System.Power.BatteryStatus";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Power::EnergySaverStatus> = L"Windows.System.Power.EnergySaverStatus";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Power::PowerSupplyStatus> = L"Windows.System.Power.PowerSupplyStatus";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Power::IBackgroundEnergyManagerStatics> = L"Windows.System.Power.IBackgroundEnergyManagerStatics";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Power::IForegroundEnergyManagerStatics> = L"Windows.System.Power.IForegroundEnergyManagerStatics";
    template <> inline constexpr auto& name_v<winrt::Windows::System::Power::IPowerManagerStatics> = L"Windows.System.Power.IPowerManagerStatics";
    template <> inline constexpr guid guid_v<winrt::Windows::System::Power::IBackgroundEnergyManagerStatics>{ 0xB3161D95,0x1180,0x4376,{ 0x96,0xE1,0x40,0x95,0x56,0x81,0x47,0xCE } }; // B3161D95-1180-4376-96E1-4095568147CE
    template <> inline constexpr guid guid_v<winrt::Windows::System::Power::IForegroundEnergyManagerStatics>{ 0x9FF86872,0xE677,0x4814,{ 0x9A,0x20,0x53,0x37,0xCA,0x73,0x2B,0x98 } }; // 9FF86872-E677-4814-9A20-5337CA732B98
    template <> inline constexpr guid guid_v<winrt::Windows::System::Power::IPowerManagerStatics>{ 0x1394825D,0x62CE,0x4364,{ 0x98,0xD5,0xAA,0x28,0xC7,0xFB,0xD1,0x5B } }; // 1394825D-62CE-4364-98D5-AA28C7FBD15B
    template <> struct abi<winrt::Windows::System::Power::IBackgroundEnergyManagerStatics>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_LowUsageLevel(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_NearMaxAcceptableUsageLevel(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_MaxAcceptableUsageLevel(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_ExcessiveUsageLevel(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_NearTerminationUsageLevel(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_TerminationUsageLevel(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_RecentEnergyUsage(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_RecentEnergyUsageLevel(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall add_RecentEnergyUsageIncreased(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_RecentEnergyUsageIncreased(winrt::event_token) noexcept = 0;
            virtual int32_t __stdcall add_RecentEnergyUsageReturnedToLow(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_RecentEnergyUsageReturnedToLow(winrt::event_token) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Power::IForegroundEnergyManagerStatics>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_LowUsageLevel(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_NearMaxAcceptableUsageLevel(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_MaxAcceptableUsageLevel(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_ExcessiveUsageLevel(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_RecentEnergyUsage(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall get_RecentEnergyUsageLevel(uint32_t*) noexcept = 0;
            virtual int32_t __stdcall add_RecentEnergyUsageIncreased(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_RecentEnergyUsageIncreased(winrt::event_token) noexcept = 0;
            virtual int32_t __stdcall add_RecentEnergyUsageReturnedToLow(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_RecentEnergyUsageReturnedToLow(winrt::event_token) noexcept = 0;
        };
    };
    template <> struct abi<winrt::Windows::System::Power::IPowerManagerStatics>
    {
        struct WINRT_IMPL_NOVTABLE type : inspectable_abi
        {
            virtual int32_t __stdcall get_EnergySaverStatus(int32_t*) noexcept = 0;
            virtual int32_t __stdcall add_EnergySaverStatusChanged(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_EnergySaverStatusChanged(winrt::event_token) noexcept = 0;
            virtual int32_t __stdcall get_BatteryStatus(int32_t*) noexcept = 0;
            virtual int32_t __stdcall add_BatteryStatusChanged(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_BatteryStatusChanged(winrt::event_token) noexcept = 0;
            virtual int32_t __stdcall get_PowerSupplyStatus(int32_t*) noexcept = 0;
            virtual int32_t __stdcall add_PowerSupplyStatusChanged(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_PowerSupplyStatusChanged(winrt::event_token) noexcept = 0;
            virtual int32_t __stdcall get_RemainingChargePercent(int32_t*) noexcept = 0;
            virtual int32_t __stdcall add_RemainingChargePercentChanged(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_RemainingChargePercentChanged(winrt::event_token) noexcept = 0;
            virtual int32_t __stdcall get_RemainingDischargeTime(int64_t*) noexcept = 0;
            virtual int32_t __stdcall add_RemainingDischargeTimeChanged(void*, winrt::event_token*) noexcept = 0;
            virtual int32_t __stdcall remove_RemainingDischargeTimeChanged(winrt::event_token) noexcept = 0;
        };
    };
    template <typename D>
    struct consume_Windows_System_Power_IBackgroundEnergyManagerStatics
    {
        [[nodiscard]] auto LowUsageLevel() const;
        [[nodiscard]] auto NearMaxAcceptableUsageLevel() const;
        [[nodiscard]] auto MaxAcceptableUsageLevel() const;
        [[nodiscard]] auto ExcessiveUsageLevel() const;
        [[nodiscard]] auto NearTerminationUsageLevel() const;
        [[nodiscard]] auto TerminationUsageLevel() const;
        [[nodiscard]] auto RecentEnergyUsage() const;
        [[nodiscard]] auto RecentEnergyUsageLevel() const;
        auto RecentEnergyUsageIncreased(winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler) const;
        using RecentEnergyUsageIncreased_revoker = impl::event_revoker<winrt::Windows::System::Power::IBackgroundEnergyManagerStatics, &impl::abi_t<winrt::Windows::System::Power::IBackgroundEnergyManagerStatics>::remove_RecentEnergyUsageIncreased>;
        [[nodiscard]] auto RecentEnergyUsageIncreased(auto_revoke_t, winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler) const;
        auto RecentEnergyUsageIncreased(winrt::event_token const& token) const noexcept;
        auto RecentEnergyUsageReturnedToLow(winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler) const;
        using RecentEnergyUsageReturnedToLow_revoker = impl::event_revoker<winrt::Windows::System::Power::IBackgroundEnergyManagerStatics, &impl::abi_t<winrt::Windows::System::Power::IBackgroundEnergyManagerStatics>::remove_RecentEnergyUsageReturnedToLow>;
        [[nodiscard]] auto RecentEnergyUsageReturnedToLow(auto_revoke_t, winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler) const;
        auto RecentEnergyUsageReturnedToLow(winrt::event_token const& token) const noexcept;
    };
    template <> struct consume<winrt::Windows::System::Power::IBackgroundEnergyManagerStatics>
    {
        template <typename D> using type = consume_Windows_System_Power_IBackgroundEnergyManagerStatics<D>;
    };
    template <typename D>
    struct consume_Windows_System_Power_IForegroundEnergyManagerStatics
    {
        [[nodiscard]] auto LowUsageLevel() const;
        [[nodiscard]] auto NearMaxAcceptableUsageLevel() const;
        [[nodiscard]] auto MaxAcceptableUsageLevel() const;
        [[nodiscard]] auto ExcessiveUsageLevel() const;
        [[nodiscard]] auto RecentEnergyUsage() const;
        [[nodiscard]] auto RecentEnergyUsageLevel() const;
        auto RecentEnergyUsageIncreased(winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler) const;
        using RecentEnergyUsageIncreased_revoker = impl::event_revoker<winrt::Windows::System::Power::IForegroundEnergyManagerStatics, &impl::abi_t<winrt::Windows::System::Power::IForegroundEnergyManagerStatics>::remove_RecentEnergyUsageIncreased>;
        [[nodiscard]] auto RecentEnergyUsageIncreased(auto_revoke_t, winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler) const;
        auto RecentEnergyUsageIncreased(winrt::event_token const& token) const noexcept;
        auto RecentEnergyUsageReturnedToLow(winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler) const;
        using RecentEnergyUsageReturnedToLow_revoker = impl::event_revoker<winrt::Windows::System::Power::IForegroundEnergyManagerStatics, &impl::abi_t<winrt::Windows::System::Power::IForegroundEnergyManagerStatics>::remove_RecentEnergyUsageReturnedToLow>;
        [[nodiscard]] auto RecentEnergyUsageReturnedToLow(auto_revoke_t, winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler) const;
        auto RecentEnergyUsageReturnedToLow(winrt::event_token const& token) const noexcept;
    };
    template <> struct consume<winrt::Windows::System::Power::IForegroundEnergyManagerStatics>
    {
        template <typename D> using type = consume_Windows_System_Power_IForegroundEnergyManagerStatics<D>;
    };
    template <typename D>
    struct consume_Windows_System_Power_IPowerManagerStatics
    {
        [[nodiscard]] auto EnergySaverStatus() const;
        auto EnergySaverStatusChanged(winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler) const;
        using EnergySaverStatusChanged_revoker = impl::event_revoker<winrt::Windows::System::Power::IPowerManagerStatics, &impl::abi_t<winrt::Windows::System::Power::IPowerManagerStatics>::remove_EnergySaverStatusChanged>;
        [[nodiscard]] auto EnergySaverStatusChanged(auto_revoke_t, winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler) const;
        auto EnergySaverStatusChanged(winrt::event_token const& token) const noexcept;
        [[nodiscard]] auto BatteryStatus() const;
        auto BatteryStatusChanged(winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler) const;
        using BatteryStatusChanged_revoker = impl::event_revoker<winrt::Windows::System::Power::IPowerManagerStatics, &impl::abi_t<winrt::Windows::System::Power::IPowerManagerStatics>::remove_BatteryStatusChanged>;
        [[nodiscard]] auto BatteryStatusChanged(auto_revoke_t, winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler) const;
        auto BatteryStatusChanged(winrt::event_token const& token) const noexcept;
        [[nodiscard]] auto PowerSupplyStatus() const;
        auto PowerSupplyStatusChanged(winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler) const;
        using PowerSupplyStatusChanged_revoker = impl::event_revoker<winrt::Windows::System::Power::IPowerManagerStatics, &impl::abi_t<winrt::Windows::System::Power::IPowerManagerStatics>::remove_PowerSupplyStatusChanged>;
        [[nodiscard]] auto PowerSupplyStatusChanged(auto_revoke_t, winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler) const;
        auto PowerSupplyStatusChanged(winrt::event_token const& token) const noexcept;
        [[nodiscard]] auto RemainingChargePercent() const;
        auto RemainingChargePercentChanged(winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler) const;
        using RemainingChargePercentChanged_revoker = impl::event_revoker<winrt::Windows::System::Power::IPowerManagerStatics, &impl::abi_t<winrt::Windows::System::Power::IPowerManagerStatics>::remove_RemainingChargePercentChanged>;
        [[nodiscard]] auto RemainingChargePercentChanged(auto_revoke_t, winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler) const;
        auto RemainingChargePercentChanged(winrt::event_token const& token) const noexcept;
        [[nodiscard]] auto RemainingDischargeTime() const;
        auto RemainingDischargeTimeChanged(winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler) const;
        using RemainingDischargeTimeChanged_revoker = impl::event_revoker<winrt::Windows::System::Power::IPowerManagerStatics, &impl::abi_t<winrt::Windows::System::Power::IPowerManagerStatics>::remove_RemainingDischargeTimeChanged>;
        [[nodiscard]] auto RemainingDischargeTimeChanged(auto_revoke_t, winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler) const;
        auto RemainingDischargeTimeChanged(winrt::event_token const& token) const noexcept;
    };
    template <> struct consume<winrt::Windows::System::Power::IPowerManagerStatics>
    {
        template <typename D> using type = consume_Windows_System_Power_IPowerManagerStatics<D>;
    };
}
#endif
